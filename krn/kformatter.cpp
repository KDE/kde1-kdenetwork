#include "kformatter.h"
#include "kstrtable.h"
#include <unistd.h>
#include <stdio.h>
#include <qfileinf.h>
#include <qregexp.h>
#include "kdecode.h"
#include <malloc.h>
#include <qregexp.h>
#include <kapp.h>
#include <mimelib/mimepp.h>

KFormatter::KFormatter(QString sWN, QString vWN, QString s)
{
    saveWidgetName=sWN;
    viewWidgetName=vWN;
    message=new DwMessage(s.data());
    message->Parse();
    
    //Get the date format from the config
    QString *defFmt=new QString("%y%m%d %H:%M (%Z)");
    kapp->getConfig()->setGroup("Appearance");
    dateFmt=new QString(kapp->getConfig()->readEntry("Dateformat",defFmt));
    CHECK_PTR(dateFmt);
    if(dateFmt->isEmpty())
        dateFmt=defFmt;
    else
        delete defFmt;
    debug("date format: %s",dateFmt->data());
}

KFormatter::~KFormatter()
{
    delete dateFmt;
}

DwBodyPart* KFormatter::ffwdPart(int n, DwBodyPart* body)
{
    //debug("ffdwing to %d",n);
    DwBodyPart* curr=body;
    for(int i=0; i<n; i++)
    {
        curr=curr->Next();
        //Fail if we have returned to the start of the part ring
        if(!curr) return NULL;
    }
    return curr;
}

DwBodyPart* KFormatter::getPartPrim(QList<int> partno, DwBodyPart* body)
{
    //debug("Getting part number %d on this level",*(partno.first()));
    //The base case
    if(partno.count()==1) return ffwdPart(*(partno.first()),body);
    
    DwBodyPart* curr;
    curr=ffwdPart(*(partno.first()),body);
    int dummy=0;
    partno.remove(dummy);
    return getPartPrim(partno, curr->Body().FirstBodyPart());
}

DwBodyPart* KFormatter::getPart(QList<int> partno)
{
    DwBodyPart* iter;
    //debug("gP: getting part %s",listToStr(partno).data());
    iter=message->Body().FirstBodyPart();
    //Clone the main body is there was no bodyparts at all, and the first was
    //requested. Fail if the article does not exist at all
    if(!iter)
    {
        //debug("Got NULL");
        if(partno.count()==1 && *(partno.at(0))==0)
        {
            //debug("Creating DwBodyPart identical to message->Body()");
            return new DwBodyPart(message->AsString(), message);
        }
        else
        {
            //debug("Part not found");
            return NULL;
        }
    }
    
    //debug("starting gPP");
    return getPartPrim(partno,iter);
}

bool KFormatter::isMultiPart(QList<int> part)
{
    DwBodyPart* p=getPart(part);
    return (p->Body().FirstBodyPart()!=NULL);
}

const char* KFormatter::rawPart(QList<int> partno)
{
    DwBodyPart* body=getPart(partno);
    const char* data=body->Body().AsString().data();
    return data;
}   

QString KFormatter::htmlAll()
{
    QList<int> l;
    int i=0;
    bool done=FALSE;
    QString text;
    int dummy=0;
    
    while(!done)
    {
        l.append(&i);
        debug("SEARCHING FOR PART %s",listToStr(l).data());
        if(getPart(l)) text+=htmlPart(l);
        else done=TRUE;
        l.remove(dummy);
        i++;
        if(!done) text+="<hr>";
        text+="\n";
    }
    debug("All parts done");
    return text;
}


QString KFormatter::htmlPart(QList<int> partno)
{
    DwBodyPart* body;
    body=getPart(partno);
    CHECK_PTR(body);
    body->Parse();
    
    //debug("Reading type");
    QString baseType=body->Header().ContentType().TypeStr().data();
    baseType=baseType.lower();
    //debug("baseType=%s",baseType.data());
    
    QString subType=body->Header().ContentType().SubtypeStr().data();
    subType=subType.lower();
    //debug("subType=%s",subType.data());
    
    //Set a default type, just in case we lack a "content-type" header
    if (baseType.isEmpty())
    {
        debug ("no type, assuming text/plain");
        baseType="text";
        subType="plain";
    }
    
    QString wholeType;
    wholeType=baseType.copy();
    if(!subType.isEmpty()) wholeType+="/"+subType;
    
    //debug("Reading encoding type");
    QString encoding;
    if(body->Header().HasCte())
    {
        encoding=body->Header().ContentTransferEncoding().AsString().data();
        encoding=encoding.lower();
    }
    else{
        debug("No encoding, assuming 7bit");
        encoding="7bit";
    }
    
    debug("Formatting part %s: baseType: %s subType: %s wholeType: %s, encoding=%s",listToStr(partno).data(), baseType.data(), subType.data(), wholeType.data(),encoding.data());
    
    const char* udata=body->Body().AsString().data();
    debug("udata: %s",udata);
    
    const char* data=KDecode::decodeString(udata,encoding);
    debug("data: %s",data);
    
    if (baseType=="text")
    {
        DwToLocalEol(data,data);
        if (subType=="html")
        {
            debug ("Found text/html part.");
            return data;
        }
        else if (subType=="plain")
        {
            debug("Found text/plain part.");
            return text_plainFormatter(data, partno);
        }
        else if (subType=="richtext")
        {
            debug("Found text/richtext part");
            return text_richtextFormatter(data, partno);
        }
        else if (subType=="x-vcard")
        {
            debug("Found mozilla vcard.");
            return text_x_vcardFormatter(data, partno);
        }
        else
        {
            debug("Found unknown text part! (%s)", subType.data());
            return text_plainFormatter(data, partno);
        }
    }
    else if (baseType=="image")
    {
        if(subType=="jpeg")
        {
            debug("Found jpeg image");
            QByteArray a(strlen(data));
            a.setRawData(data,strlen(data));
            return image_jpegFormatter(a,partno);
        }
        else
        {
            debug("Found unknown image part!");
            QString part;
            part.sprintf("This message part consists of an image of an "
                         "unsupported type (%s)<br>.\n %s",
                         subType.data(), saveLink(partno, "").data() );
            return part;
        }
    }
    else if (baseType=="multipart")
    {
        if(subType=="alternative")
        {
            debug("Found multipart/alternative message parts");
            int i=0;
            int max=0, maxpos=0, score;
            DwBodyPart* curr;
            QString baseType, subType;
            bool done=FALSE;
            while(!done)
            {
                int* j=new int;
                *j=i;
                partno.append(j);
                //debug("Checking part: %s",listToStr(partno).data());                
                curr=getPart(partno);
                //debug("* curr=%p",curr);
                //debug("data: \"%s\"",curr->AsString().data());
                if(curr!=NULL)
                {
                    baseType=curr->Header().ContentType().TypeStr().data();
                    subType=curr->Header().ContentType().SubtypeStr().data();
                    score=rateType(baseType,subType);
                    //debug("This part scored %d",score);
                    if(score>=max)
                    {
                        //debug("A new world record!");
                        max=score;
                        maxpos=i;
                    }
                }
                else done=TRUE;
                partno.remove(partno.count()-1);
                i++;
            }
            partno.append(&maxpos);
            //debug("*** Best version: %s",listToStr(partno).data());
            if(getPart(partno)==NULL)
            {
                return "<strong>This part claims to be made up of several "
                    "parts, out of wich one should be displayd. However, no "
                    "part was found at all. This might indicate a bug in "
                    "krn's KFormatter class.</strong>\n";
            }
            else return htmlPart(partno);
        }
        else if(subType=="related")
        {
            debug("Found related multipart parts");
            QString part="<b>This part consists of several related parts. "
                "There is not yet any support for embedding such "
                "parts into each other, but who knows, that may come "
                "some day. Anyway, here are all the parts one at a "
                "time:</b><hr>\n";
            int i=0;
            DwBodyPart* curr;
            bool done=FALSE;
            while(!done)
            {
                int* j=new int;
                *j=i;
                partno.append(j);
                curr=getPart(partno);
                //debug("curr=%p",curr);
                if(curr!=NULL) part+=htmlPart(partno)+"<hr>\n";
                else done=TRUE;
                partno.remove(partno.count()-1);
                i++;
            }
            return part;
        }
        else if(subType=="digest")
        {
            debug("Found multipart/digest part");
            QString part="<b>This part consists of several parts, "
                "some being digests of thers. There is not yet any "
                "support for displaying such parts correctly, but "
                "who knows, that may come some day. Anyway, here "
                "are all the parts one at a time:</b><hr>\n";
            int i=0;
            DwBodyPart* curr;
            bool done=FALSE;
            while(!done)
            {
                int* j=new int;
                *j=i;
                partno.append(j);
                curr=getPart(partno);
                //debug("curr=%p",curr);
                if(curr!=NULL) part+=htmlPart(partno)+"<hr>\n";
                else done=TRUE;
                partno.remove(partno.count()-1);
                i++;
            }
            return part;
        }
        else
        {
            debug("unknown multipart subtype encountered: %s",subType.data());
            QString part;
            part.sprintf("<b>This type of multipart messages is not supported yet.</b>",subType.data());
            return part;
        }
        
    }
    else
    {
        debug("Type %s not handled natively. Searching for plug-in",
              wholeType.data());
        KConfig* conf=kapp->getConfig();
        conf->setGroup("Formatters");
        if(conf->hasKey(wholeType.data()))
        {
            QString plugin=conf->readEntry(wholeType);
            debug("Plug-in found: %s",plugin.data());
            int i=tempfile.create("format_in","");
            int o=tempfile.create("format_out","");
            QFile* f=tempfile.file(i);
            f->open(IO_WriteOnly);
            f->writeBlock(data,strlen(data));
            f->close();
            
            system(plugin+" <"+tempfile.file(i)->name()+" >"+
                   tempfile.file(o)->name());
            
            f=tempfile.file(o);
            f->open(IO_ReadOnly);
            char* ndata=(char*)malloc(f->size());
            f->readBlock(ndata,f->size());
            f->close();
            
            tempfile.remove(i);
            tempfile.remove(o);
            
            return ndata;
        }
        
        debug ("Found some part! (%s)",wholeType.data());
        QString part;
        part.sprintf("This message part consists of an attachment of an "
                     "unsupported type (%s)<br>.\n%s",
                     wholeType.data(),
                     saveLink(partno, "").data() );
        return part;
    }
}

QString KFormatter::getType(QList<int> part)
{
    QString baseType=getPart(part)->Header().ContentType().TypeStr().data();
    QString subType=getPart(part)->Header().ContentType().SubtypeStr().data();
    QString wholeType=baseType;
    if(!subType.isEmpty()) baseType+=subType;
    return wholeType;
}

QString KFormatter::saveLink(QList<int> part, char* text)
{
    QString type=getType(part);
    
    QString link;
    link.sprintf("<a href=\"save://%d/%s\">%s<img src=%s alt=\"save\"></a>",
                 listToStr(part).data(), type.data(), text,
                 saveWidgetName.data() );
    return link;
}

QString KFormatter::mailLink(QString reciptent, char* text)
{
    return "<a href=\"mailto:" + reciptent + "\">" + text + "</a>";
}

QString KFormatter::htmlHeader()
{
    QString header;
    //Build the header
    QStrIList visheaders;
    visheaders.setAutoDelete(true);
    // This should be handled by a dialog
    // And stored in the config file
    visheaders.append("From");
    visheaders.append("To");
    visheaders.append("Subject");
    visheaders.append("Newsgroups");
    visheaders.append("Reply-To");
    visheaders.append("Followup-To");
    visheaders.append("Date");
    visheaders.append("References");
    
    for (char *iter=visheaders.first();!iter==0;iter=visheaders.next())
    {
        if (message->Header().HasField(iter))
        {
            QString headerName=iter;
            header+="<b>"+headerName+": </b>";
            
            if(headerName=="Newsgroups" || headerName=="Followup-to")
            {
                QString groups=message->Header().FieldBody(iter).AsString().data();
                groups.simplifyWhiteSpace();
                groups+=',';
                unsigned int index=0, len;
                QString group;
                while(index<groups.length())
                {
                    len=groups.find(',',index)-index;
                    group=groups.mid(index,len);
                    header+="<a href=\"news://newsserver/"+group+"\">"+group+"</a> ";
                    index+=len+1;
                }
            }
            if(headerName=="References")
            {
                QString articles=message->Header().FieldBody(iter).AsString().data();
                articles.simplifyWhiteSpace();
                articles+=' ';
                debug("header: %s",articles.data());
                unsigned int index=0, len;
                QString article,t;
                int count=1;
                while(index<articles.length())
                {
                    len=articles.find(' ',index)-index;
                    article=articles.mid(index,len);
                    index+=len+1;
                    article=article.mid(1,article.length()-2);
                    if (article.isEmpty()) continue;
                    debug("Found article \"%s\".",article.data());
                    t.setNum(count++);
                    t="<a href=\"news:///"+article+"\">"+t+"</a> ";
                    debug ("t-->%s",t.data());
                    header+=t;
                }
            }
            else if(headerName=="Date")
            {
                debug ("Date formatter:%s",dateFmt->data());
                DwDateTime realDate(message->Header().FieldBody(iter).AsString().data());
                QString textDate="";
                int ival;
                QString sval;
                debug("Looping %d chars", dateFmt->length());
                for(int pos=0; pos<(int)dateFmt->length(); pos++)
                {
                    if( dateFmt->at(pos)=='%' && pos<(int)dateFmt->length() )
                    {
                        ival=-1; sval="";
                        switch(dateFmt->at(pos+1))
                        {
                        case '%' : sval='%';                 break;
                        case 'd' : ival=realDate.Day();      break;
                        case 'k' :
                        case 'H' : ival=realDate.Hour();     break;
                        case 'l' :
                        case 'I' : ival=realDate.Hour()%12;  break;
                        case 'M' : ival=realDate.Minute();   break;
                        case 'm' : ival=realDate.Month();    break;
                        case 'p' : sval=realDate.Hour()>11?"PM":"AM"; break;
                        case 'S' : ival=realDate.Second();   break;
                        case 'Y' : ival=realDate.Year();     break;
                        case 'y' : ival=realDate.Year()%100; break;
                        case 'Z' : ival=realDate.Zone();     break;
                        default  : warning("The format character %c is not"
                                           " yet supported.",dateFmt->at(pos+1));
                        }
                        if(ival!=-1) textDate.sprintf("%s%02d",textDate.data(),ival);
                        if(!sval.isEmpty()) textDate+=sval;
                        pos++;
                    }
                    else textDate+=dateFmt->at(pos);
                }
                header+=textDate;
            }
            else
                // header without special formatting
                // needs to escape < and >
            {
                QString s(message->Header().FieldBody(iter).AsString().data());
                s.replace(QRegExp("<"),"&lt;");
                s.replace(QRegExp(">"),"&gt;");
                header=header+s+"\n";
            }
            
            header+=+"<br>\n";
        }
    }
    return header;
}

QString KFormatter::image_jpegFormatter(QByteArray data, QList<int> partno)
{
    QString part;
    QString name=tmpnam(NULL);
    QFile file(name);
    file.open(IO_WriteOnly);
    file.writeBlock(data.data(),data.size());
    file.close();
    QString link;
    link.sprintf("<img src=%s alt=\"%d kb image\">",
                 name.data(), QFileInfo(name).size()/1024 );
    part.sprintf("Attached jpeg image<br>\n %s",
                 saveLink(partno, link.data()).data() );
    /*
     else
     {
     part.sprintf("This part consists of an jpeg image, wich unfortunately "
     "could not be automatically saved, and therefore can not be"
     "shown. To save this image, click on save %s",
     saveLink(partno, "").data() );
     }
     */
    return part;
}

QString KFormatter::text_plainFormatter(QString data, QList<int>)
{
    //debug("%s: working on \"%s\"",__FUNCTION__,data.data());
    bool insig=false;
    QString st,sig,body;
    while (!data.isEmpty())
    {	
        
        int i=data.find("\n");
        if (i==-1)
        {
            st=data;
            data="";
        }
        else
        {
            st=data.left(i);
            data=data.right(data.length()-i-1);
        };
        if (st.left(2)=="--")
        {
            //If it's more than 8 lines of 80 chars, it prabably isn't a sig
            if(st.length()<(8*80)) insig=true;
        }
        st.replace(QRegExp("<"),"&lt;");
        st.replace(QRegExp(">"),"&gt;");
        
        if (insig)
        {
            sig=sig+st+"\n";
        }
        else
        {
            if (st.left(4)=="&gt;")      //It's a quote
            {
                // This doesn't work. Why?
                st="<i>"+st+"</i>";
                
            }
            else
            {
                int pos=0;
                int* len=new int;
                QRegExp re("_[a-z]*_",FALSE);
                
                while(pos<(int)st.length())
                {
                    //debug("Searching for _word_ in \"%s\"",st.left(st.length()-pos).data());
                    pos=re.match(st.left(st.length()-pos), pos, len);
                    if(pos==-1) {
                        //debug("No more matches");
                        break;
                    }
                    QString word;
                    word=st.mid(pos+1,*len-2);
                    //debug("Replacing %s",word.data());
                    word.prepend("<i>");
                    word.append("</i>");
                    st.replace(pos, *len, word.data());
                }
                delete len;
            }
            
            if (st.isEmpty())    //Empty line (end of para)
                st="<p>";
            else                      //Plain line
                st=st+"<br>";
            body=body+st+"\n";
        }
    }
    return body+"<pre>"+sig+"</pre>";
}

QString KFormatter::text_x_vcardFormatter(QString data, QList<int>)
{
    QString result;
    
    KStrTable card;
    card.read(data);
    
    result="<h3>This is the mozilla vcard of ";
    
    QString name;
    if(card.hasItem("fn")) name=card.getItem("fn");
    else if(card.hasItem("n")) name=card.getItem("n");
    else name="an unknown person.";
    if(card.hasItem("email;internet"))
        result+=mailLink(card.getItem("email;internet").data(), name.data());
    else result+=name;
    result+="</h3><br>\n";
    
    if(card.hasItem("n")) result+="<b>Name: </b>"+card.getItem("n")+"<br>\n";
    if(card.hasItem("fn")) result+="<b>Full name: </b>"+card.getItem("fn")+"<br>\n";
    if(card.hasItem("org")) result+="<b>Organization: </b>"+card.getItem("org")+"<br>\n";
    if(card.hasItem("title")) result+="<b>Title: </b>"+card.getItem("title")+"<br>\n";
    
    if(card.hasItem("x-mozilla-html"))
    {
        result+="<string>The sender prefers ";
        if(card.getItem("x-mozilla-html")=="yes") result+="html";
        else result+="plaintext";
        result+=" emails.</strong><br>\n";
    }
    
    if(card.hasItem("note")) result+="<b>Note: </b><blockquote>"+card.getItem("note") + "</blockquote><br>\n";
    
    return result + "<hr>The rest of this vcard can not be shown yet. Work is being done at"
        "supporting more fileds. The raw card follows:<br>\n<pre>" + data + "</pre>\n";
}

QString KFormatter::text_richtextFormatter(QString data, QList<int>)
{
    data.replace(QRegExp("<lt>",FALSE),"&lt;");
    data.replace(QRegExp("<nl>",FALSE),"<br>");
    data.replace(QRegExp("<np>",FALSE),"<hr>");
    data.replace(">","&gt;");
    data.replace("\\n","<br>");
    
    data.replace(QRegExp("<bold>",FALSE),"<b>");
    data.replace(QRegExp("</bold>",FALSE),"</b>");
    
    data.replace(QRegExp("<italic>",FALSE),"<i>");
    data.replace(QRegExp("</italic>",FALSE),"</i>");
    
    data.replace(QRegExp("<fixed>",FALSE),"<tt>");
    data.replace(QRegExp("</fixed>",FALSE),"</tt>");
    
    data.replace(QRegExp("<flushleft>",FALSE),"<p align=left>");
    data.replace(QRegExp("</flushleft>",FALSE),"</p>");
    
    data.replace(QRegExp("<flushright>",FALSE),"<p align=right>");
    data.replace(QRegExp("</flushright>",FALSE),"</p>");
    
    data.replace(QRegExp("<bigger>",FALSE),"<big>");
    data.replace(QRegExp("</bigger>",FALSE),"</big>");
    
    data.replace(QRegExp("<smaller>",FALSE),"<small>");
    data.replace(QRegExp("</smaller>",FALSE),"</small>");
    
    data.replace(QRegExp("<subscript>",FALSE),"<sub>");
    data.replace(QRegExp("</subscript>",FALSE),"</sub>");
    
    data.replace(QRegExp("<superscript>",FALSE),"<sup>");
    data.replace(QRegExp("</superscript>",FALSE),"</sup>");
    
    data.replace(QRegExp("<excerpt>",FALSE),"<blockquote>");
    data.replace(QRegExp("</excerpt>",FALSE),"</blockquote>");
    
    data.replace(QRegExp("<comment>.*</comment>",FALSE),"");
    
    data.replace(QRegExp("<no-op>",FALSE),"");
    data.replace(QRegExp("</no-op>",FALSE),"");
    
    data.replace(QRegExp("<Underline>",FALSE),"<em>");
    data.replace(QRegExp("</Underline>",FALSE),"</em>");
    
    data.replace(QRegExp("<Indent>",FALSE),"");
    data.replace(QRegExp("</Indent>",FALSE),"");
    
    data.replace(QRegExp("<IndentRight>",FALSE),"");
    data.replace(QRegExp("</IndentRight>",FALSE),"");
    
    data.replace(QRegExp("<Outdent>",FALSE),"");
    data.replace(QRegExp("</Outdent>",FALSE),"");
    
    data.replace(QRegExp("<OutdentRight>",FALSE),"");
    data.replace(QRegExp("</OutdentRight>",FALSE),"");
    
    data.replace(QRegExp("<SamePage>",FALSE),"");
    data.replace(QRegExp("</SamePage>",FALSE),"");
    
    data.replace(QRegExp("<Heading>",FALSE),"");
    data.replace(QRegExp("</Heading>",FALSE),"");
    
    data.replace(QRegExp("<Footing>",FALSE),"");
    data.replace(QRegExp("</Footing>",FALSE),"");
    
    data.replace(QRegExp("<ISO-8859-.*>",FALSE),"<b>Krn note: This text is incoded in another scharacter set, and may look wierd</b>");
    data.replace(QRegExp("</ISO-8859-.*>",FALSE),"<b>Krn note: Character set reset.</b>");
    
    data.replace(QRegExp("<US-ASCII>",FALSE),"");
    data.replace(QRegExp("</US-ASCII>",FALSE),"");
    
    data.replace(QRegExp("<Paragraph>",FALSE),"<p>");
    data.replace(QRegExp("</Paragraph>",FALSE),"</p>");
    
    data.replace(QRegExp("<Signature>",FALSE),"<br><small><pre>");
    data.replace(QRegExp("</Signature>",FALSE),"</pre></small>>");
    
    /*
     data.replace(QRegExp("<>,FALSE)","<>");
     data.replace(QRegExp("</>,FALSE)","</>");
     */
    
    return data;
    
}

QString KFormatter::listToStr(QList<int> l)
{
    QString r;
    QString t;
    //debug("lTS: %d digits",l.count());
    for(int i=0; i<(int)l.count(); i++)
    {
        t.setNum(*l.at(i));
        r.append(t);
        if(i<(int)l.count()-1) r.append(".");
    }
    return r;
}

QList<int> KFormatter::strToList(QString s)
{
    QList<int> l;
    s+='.';
    unsigned int start=0,end;
    int* t;
    while(start<s.length()-1)
    {
        end=s.find('.',start);
        t=new int;
        *t=atoi(s.mid(start,end-start));
        l.append(t);
        start=end+1;
    }
    return l;
}

bool KFormatter::dump(QList<int> part, QString fileName)
{
    debug("Dumping part %s as %s",listToStr(part).data(), fileName.data());
    DwString src=getPart(part)->Body().AsString();
    
    QFile file(fileName);
    if(!file.open(IO_WriteOnly)) return FALSE;
    if(file.writeBlock(src.data(), src.length()) != (int)src.length())
    {
        file.close();
        return FALSE;
    }
    file.close();
    return TRUE;
}

//This function returns an integer that can be used to decide what part
//to display, when having to choose between several.
//The result may be between 0 and 999, inclusive.
unsigned int KFormatter::rateType(QString baseType, QString subType)
{
    debug("***Rating %s/%s",baseType.data(),subType.data());
    if(baseType=="message")
    {
        return 700;
    }
    if(baseType=="multipart")
    {
        if(subType=="digest")		return 603;
        if(subType=="parallel")		return 602;
        if(subType=="mixed")		return 601;
        if(subType=="alternative")	return 600;
    }
    //FIXME: rate other base types!
    //if(baseType!="text")
    //{
    //    return 200;
    //}
    if(baseType=="text")
    {
        if(subType=="html")	return 104;
        if(subType=="richtext")	return 103;
        if(subType=="plain")	return 102;
        else			return 100;
    }
    
    else return 0;
}
