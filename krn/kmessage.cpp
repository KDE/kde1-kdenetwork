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
#include <Kconfig.h>
#include <kfm.h>

Kmessage::Kmessage
    (
     QWidget* parent,
     const char* name
    )
    :
    KHTMLView(parent,name)

{
    setFocusPolicy(QWidget::NoFocus);

    view=this;

    view->setScrolling(-1);
    view->resize(400,300);
    view->begin("file:/tmp/xxx");
    view->write ("<html><head><title>Krn message view</title></head>\n"
                 "<body bgcolor=#ffffff><hr><h4>Krn:&nbsp;A&nbsp;Newsreader&nbsp;for&nbsp;KDE</h4><hr>");
    debug("HTML Header=\""
          "<html><head><title>Krn message view</title></head>\n"
          "<body bgcolor=#ffffff><hr><h4>Krn:&nbsp;A&nbsp;Newsreader&nbsp;for&nbsp;KDE</h4><hr>"
          "\"");
    view->end();
    view->parse();

    view->setBackgroundColor(QColor("white"));
    kapp->processEvents();

    KApplication::connect(view,SIGNAL(URLSelected(const char*,int)),this,SLOT(URLClicked(const char*,int)));

    saveWidgetName=tmpnam(NULL);
    tmpFiles.append(saveWidgetName);
    viewWidgetName=tmpnam(NULL);
    tmpFiles.append(viewWidgetName);

    renderWidgets();
    KApplication::connect(kapp,SIGNAL(kdisplayPaletteChanged()),this, SLOT(renderWidgets()));
    KApplication::connect(kapp,SIGNAL(kdisplayStyleChanged()),this, SLOT(renderWidgets()));
    KApplication::connect(kapp,SIGNAL(kdisplayFontChanged()),this, SLOT(renderWidgets()));
}

Kmessage::~Kmessage()
{
    debug("DTOR");
    for(char* s=tmpFiles.first(); s!=NULL; s=tmpFiles.next())
        unlink(s);
}

void Kmessage::loadMessage( QString message, bool complete=TRUE )
{
    format=new KFormatter(saveWidgetName,viewWidgetName,message,complete);
    CHECK_PTR(format);

    view->begin("file:/tmp/xxx");
    QString header=format->htmlHeader();
    view->write(header+"<hr>");
    view->parse();
    QString body=format->htmlAll();
    view->write(body+"</html>\n");
    view->end();
    view->repaint();
    view->show();

}

void Kmessage::URLClicked(const char* s,int)
{
    debug("Got URL %s.",s);

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
        debug ("path-->%s",url.path());
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
        debug("Unknown URL type. Spawning KFM.");
        KFM f;
        f.openURL(s);
    }
}

bool Kmessage::dump(char* part, QString fileName)
{
//    debug("Dumping part %d as %s",part, fileName.data());

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
    unlink(saveWidgetName);
    QPushButton saveButton("save");
    QPixmap* saveImage = new QPixmap(saveButton.size());

    bitBlt(saveImage, QPoint(0,0), &saveButton, saveButton.rect(), CopyROP );
    if(saveImage->isNull()) debug("Strange");
    if(!saveImage->save(saveWidgetName,"XBM")) debug("Unable to save sW");
    else debug("save widget saved as %s",saveWidgetName.data());

    unlink(viewWidgetName);
    QPushButton viewButton("view");
    QPixmap* viewImage = new QPixmap(viewButton.size());

    bitBlt(viewImage, QPoint(0,0), &viewButton, viewButton.rect(), CopyROP );
    if(!viewImage->save(viewWidgetName,"XBM")) debug("Unable to save vW");
    else debug("view widget saved as %s",viewWidgetName.data());
}
