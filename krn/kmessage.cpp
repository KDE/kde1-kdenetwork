//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// This file os part of KRN, a newsreader for the KDE project.              //
// KRN is distributed under the GNU General Public License.                 //
// Read the acompanying file COPYING for more info.                         //
//                                                                          //
// KRN wouldn't be possible without these libraries, whose authors have     //
// made free to use on non-commercial software:                             //
//                                                                          //
// MIME++ by Doug Sauder                                                    //
// Qt     by Troll Tech                                                     //
//                                                                          //
// This file is copyright 1997 by                                           //
// Roberto Alsina <ralsina@unl.edu.ar>                                      //
// Magnus Reftel  <d96reftl@dtek.chalmers.se>                               //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#include <unistd.h>

#include <qtstream.h>
#include <qfile.h>
#include <qregexp.h>
#include <qstrlist.h>

#include "kmessage.h"

#include <kapp.h>
#include <kmsgbox.h>
#include <qfiledlg.h>
#include <qbutton.h>
#include <unistd.h>
#include <qpixmap.h>
#include <qlist.h>
#include <kconfig.h>
#include <kfm.h>
#include <kurl.h>
#include <qmsgbox.h>

#include "kmessage.moc"

extern KConfig *conf;


Kmessage::Kmessage
    (
     QWidget* parent,
     const char* name
    )
    :
    KHTMLView(parent,name)

{
    loadSettings();
    setFocusPolicy(QWidget::NoFocus);

    this->setScrolling(-1);
    this->resize(400,300);
    this->begin("file:/tmp/xxx");
    this->write ("<html><head><title>Krn message view</title></head>\n"
                   "<body><hr><h4>Krn:&nbsp;A&nbsp;Newsreader&nbsp;for&nbsp;KDE I</h4><hr>");
    this->write("");
    this->end();
    this->parse();
  
    this->loadSettings();
    kapp->processEvents();

    KApplication::connect(this,SIGNAL(URLSelected(const char*,int)),this,SLOT(URLClicked(const char*,int)));

    saveWidgetName=tmpnam(NULL);
    tmpFiles.append(saveWidgetName);
    viewWidgetName=tmpnam(NULL);
    tmpFiles.append(viewWidgetName);

    renderWidgets();
//    QObject::connect(kapp,SIGNAL(kdisplayPaletteChanged()),this, SLOT(renderWidgets()));
//    QObject::connect(kapp,SIGNAL(kdisplayStyleChanged()),this, SLOT(renderWidgets()));
//    QObject::connect(kapp,SIGNAL(kdisplayFontChanged()),this, SLOT(renderWidgets()));

    QObject::connect(this,
                     SIGNAL(URLSelected( KHTMLView *, const char *, int , const char *)),
                     this,
                     SLOT(URLClicked( KHTMLView *, const char *, int , const char *)));

}

Kmessage::~Kmessage()
{
    for(char* s=tmpFiles.first(); s!=NULL; s=tmpFiles.next())
        unlink(s);
}

void Kmessage::loadMessage( QString message, bool complete )
{
    format=new KFormatter(saveWidgetName,viewWidgetName,message,complete);
    CHECK_PTR(format);

    QString header=format->htmlHeader();
    QString body=format->htmlAll();
    this->loadSettings();
    this->begin();
    this->parse();
    this->write(header+"<hr>"+body);
    this->end();
}
void Kmessage::URLClicked(KHTMLView *, const char *s, int , const char * )
{

    KURL url(s);
    if( url.isMalformed() )
    {
        warning("Invalid URL clicked!");
        //Should
        //return;
        //It really should ;-)
    }

    if(strcmp(url.protocol(),"news")==0)
    {
        if(strchr(url.path(),'@')!=NULL)
        {
            QString s=url.path();
            s="<"+s.right(s.length()-1)+">";
            emit(spawnArticle(s));
        }
        else emit spawnGroup(url.path());
    }
    else if(strcmp(url.protocol(),"save")==0)
    {
        QString name=QFileDialog::getSaveFileName();
        if(name.isEmpty()) return;

        debug("saving url. protocol: %s, host part: %s, path part: %s",
              url.protocol(), url.host(), url.path() );
        dump(url.host(),name);

    }
    else if(strcmp(url.protocol(),"view")==0)
    {
        if(fork()==0)
        {
            KTempFile t;
            QString name=t.file(t.create("mailcap",""))->name();
            dump(url.host(),name);
            QString cmd;
            cmd.sprintf("metamail -b -c %s -m Krn -z %s",url.path(),name.data());
            system(cmd);
        }
    }
    else
    {
        bool success=false;
        debug("Unknown URL type. Spawning KFM.");
        KFM fm;
        //        QString t=tmpnam(NULL);
        fm.openURL(s);
        /*        fm.copy(s,t);
         QFile f(t);
         if (f.exists())
         {
         if (f.open(IO_ReadOnly))
         {
         char *buffer = new char[f.size()];
         f.readBlock (buffer,f.size());
         this->begin("file:/tmp/xxx");
         this->write(buffer);
         this->end();
         delete[] buffer;
         success=true;
         }
         }
         
         if (!success)
         QMessageBox::warning(0,"KFM - Warning","I can't get that message from the web for some reason");
         */
    }
    
}

bool Kmessage::dump(char* part, QString fileName)
{

    QList<int> n=format->strToList(part);
    const char* data=format->rawPart(n);
    n.clear();

    QFile file(fileName);
    if(!file.open(IO_WriteOnly)) return FALSE;
    if(file.writeBlock(data, strlen(data))!=(int)strlen(data))
    {
        file.close();
        return FALSE;
    }
    file.close();
    return TRUE;
}

void Kmessage::renderWidgets()
{
}
void Kmessage::getFromWeb(QString id)
{
    char *buffer=new char[4096];
    QString urldata("http://ww2.altavista.digital.com/cgi-bin/news.cgi?id@");
    id=id.mid(1,id.length()-2);
    //    KURL::encodeURL(id);
    debug ("encoded?-->%s",id.data());
    urldata+=id;
    debug ("urldata-->%s",urldata.data());
    KURL url(urldata.data());
    debug ("url-->%s",url.url().data());
    this->begin("file:/tmp/xxx");
    sprintf (buffer,
             "<h1>Error getting article</h1><hr>
             The article seems to have expired or be missing from both
             your news server and our local cache.<hr>
             However, if you have a functional Internet connection, you may
             be able to find it at Altavista following
             <a href=%s>this link</a>",url.url().data());
    this->write (buffer);
    this->end();
    this->parse();
}
                          
void Kmessage::loadSettings()
{
    conf->setGroup("ArticleListOptions");
    getKHTMLWidget()->setDefaultFontBase(conf->readNumEntry("DefaultFontBase",3));
    getKHTMLWidget()->setStandardFont(conf->readEntry("StandardFont",QString("helvetica")));
    getKHTMLWidget()->setFixedFont(conf->readEntry("FixedFont",QString("courier")));
    getKHTMLWidget()->setDefaultBGColor(conf->readColorEntry("BackgroundColor",&QColor("white")));
    getKHTMLWidget()->setDefaultTextColors(
                                           conf->readColorEntry("ForegroundColor",&QColor("black")),
                                           conf->readColorEntry("LinkColor",&QColor("blue")),
                                           conf->readColorEntry("FollowedColor",&QColor("red"))
                                          );
}

                          
