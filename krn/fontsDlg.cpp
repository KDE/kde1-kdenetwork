#include "fontsDlg.h"

#define Inherited QDialog

#include <kapp.h>
#include <kconfig.h>
#include <qfont.h>
#include <qstrlist.h>
#include <X11/Xlib.h>

#include "fontsDlg.moc"

//Prototypes for utility functions
void addFont( QStrList &list, const char *xfont );
void getFontList( QStrList &list, const char *pattern );


extern KConfig *conf;

fontsDlg::fontsDlg
    (
     QWidget* parent,
     const char* name
    )
    :
    Inherited( parent, name, TRUE ),
    fontsDlgData( this )
{
    setCaption( klocale->translate("Message Display Fonts") );
    connect (b1,SIGNAL(clicked()),this,SLOT(accept()));
    connect (b1,SIGNAL(clicked()),this,SLOT(save()));
    connect (b2,SIGNAL(clicked()),this,SLOT(reject()));
    conf->setGroup("ArticleListOptions");
    fontSize->setCurrentItem(conf->readNumEntry("DefaultFontBase",3)-2);
    QStrList stdfl,fixedfl;
    getFontList( stdfl, "-*-*-*-*-*-*-*-*-*-*-p-*-*-*" );
    //I add these, because some people may prefer all-fixed width fonts
    //(for ascii art, I suppose)
    getFontList( stdfl, "-*-*-*-*-*-*-*-*-*-*-m-*-*-*" );
    stdFontName->insertStrList(&stdfl);
    getFontList( fixedfl, "-*-*-*-*-*-*-*-*-*-*-m-*-*-*" );
    fixedFontName->insertStrList(&fixedfl);

    connect (fixedFontName,SIGNAL(activated(int)),this,SLOT(syncFonts(int)));
    connect (stdFontName,SIGNAL(activated(int)),this,SLOT(syncFonts(int)));
    
    stdFontName->setCurrentItem(stdfl.find
                                (conf->readEntry("StandardFont",QString("helvetica").data())));
    fixedFontName->setCurrentItem(fixedfl.find
                                  (conf->readEntry("FixedFont",QString("courier").data())));
    syncFonts(0);
    samp1->setText(conf->readEntry("StandardFontTest",QString("Standard Font Test").data()));
    samp2->setText(conf->readEntry("FixedFontTest",QString("Fixed Font Test").data()));
    QColor c;
    c=QColor("white");
    bgColor->setColor(conf->readColorEntry("BackgroundColor",&c));
    c=QColor("black");
    fgColor->setColor(conf->readColorEntry("ForegroundColor",&c));
    c=QColor("blue");
    linkColor->setColor(conf->readColorEntry("LinkColor",&c));
    c=QColor("red");
    followColor->setColor(conf->readColorEntry("FollowedColor",&c));
}


void fontsDlg::syncFonts(int)
{
    samp1->setFont(QFont(stdFontName->currentText()));
    samp2->setFont(QFont(fixedFontName->currentText()));
}

fontsDlg::~fontsDlg()
{
}

void fontsDlg::save()
{
    conf->setGroup("ArticleListOptions");
    conf->writeEntry ("DefaultFontBase",fontSize->currentItem()+2);
    conf->writeEntry ("StandardFont",stdFontName->currentText());
    conf->writeEntry ("FixedFont",fixedFontName->currentText());
    conf->writeEntry ("StandardFontTest",samp1->text());
    conf->writeEntry ("FixedFontTest",samp2->text());
    conf->writeEntry ("BackgroundColor",bgColor->color());
    conf->writeEntry ("ForegroundColor",fgColor->color());
    conf->writeEntry ("LinkColor",linkColor->color());
    conf->writeEntry ("FollowedColor",followColor->color());
    conf->sync();
}

void addFont( QStrList &list, const char *xfont )
{
        const char *ptr = strchr( xfont, '-' );
        if ( !ptr )
                return;

        ptr = strchr( ptr + 1, '-' );
        if ( !ptr )
                return;

        QString font = ptr + 1;

        int pos;
        if ( ( pos = font.find( '-' ) ) > 0 )
        {
                font.truncate( pos );

                if ( font.find( "open look", 0, false ) >= 0 )
                        return;


                QStrListIterator it( list );

                for ( ; it.current(); ++it )
                        if ( it.current() == font )
                                return;

                list.inSort( font );
        }
}


void getFontList( QStrList &list, const char *pattern )
{
        int num;

        char **xFonts = XListFonts( qt_xdisplay(), pattern, 200, &num );

        for ( int i = 0; i < num; i++ )
        {
                addFont( list, xFonts[i] );
        }

        XFreeFontNames( xFonts );
}
