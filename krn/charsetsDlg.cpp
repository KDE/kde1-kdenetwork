#ifdef CHARSETS

#include "charsetsDlg.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qframe.h>
#include <qchkbox.h>
#include <qpushbt.h>

#include <kapp.h>
#include <klocale.h>
#include <kconfig.h>
#include <kcharsets.h>
#include <qcombo.h>

#include <qlayout.h>

#include "charsetsDlg.moc"


extern KConfig *conf;

CharsetsDlg::CharsetsDlg(const char *message,const char *composer) :
#ifdef KRN
    QDialog (0,klocale->translate("KRN - Charset Settings"),true)
#else
    QDialog (0,klocale->translate("KMail - Charset Settings"),true)
#endif
{
int i;
    QBoxLayout *mainl=new QVBoxLayout(this);
    QBoxLayout *buttonsl=new QHBoxLayout();
    QBoxLayout *optl=new QHBoxLayout();
    KCharsets *charsets=kapp->getCharsets();
    
      
    QLabel *l=new QLabel(klocale->translate("Message charset"),this,"l1");
    l->setAlignment(AlignCenter);
    mainl->addWidget(l);
    
    messageCharset=new QComboBox(this,"c1");
    mainl->addWidget(messageCharset);
    
    QStrList lst=charsets->registered();
    messageCharset->insertItem( "default" );
    for(const char *chset=lst.first();chset;chset=lst.next())
      messageCharset->insertItem( chset );
    int n=messageCharset->count();  
    for(i=0;i<n;i++)
      if (!stricmp(messageCharset->text(i),message)){
        messageCharset->setCurrentItem(i);
	break;
      }	
 
    l=new QLabel(klocale->translate("Composer charset"),this,"l2");
    l->setAlignment(AlignCenter);
    mainl->addWidget(l);

    composerCharset=new QComboBox(this,"c2");
    mainl->addWidget(composerCharset);
    
    QStrList lst1=charsets->displayable();
    composerCharset->insertItem( "default" );
    for(const char *chset=lst1.first();chset;chset=lst1.next())
      composerCharset->insertItem( chset );
    n=composerCharset->count();  
    for(i=0;i<n;i++)
      if (!stricmp(composerCharset->text(i),composer)){
        composerCharset->setCurrentItem(i);
	break;
       }	
        
      
    setDefault=new QCheckBox("Set as &default",this,"cb");
    mainl->addWidget(setDefault);
    
    mainl->addLayout(optl);
    
    is7BitASCII=new QCheckBox("&7 bit is ASCII",this,"7b");
    optl->addWidget(is7BitASCII);
    
    quoteUnknown=new QCheckBox("&Qute unknonw characters",this,"qu");
    optl->addWidget(quoteUnknown);

    mainl->addLayout(buttonsl);

    QPushButton *b1=new QPushButton(klocale->translate("OK"),this,"b1");
    buttonsl->addWidget(b1);
    QPushButton *b2=new QPushButton(klocale->translate("Cancel"),this,"b2");
    buttonsl->addWidget(b2);

    connect (b1,SIGNAL(clicked()),SLOT(save()));
    connect (b2,SIGNAL(clicked()),SLOT(reject()));

    mainl->activate();
    buttonsl->activate();
}

void CharsetsDlg::save()
{
    emit setCharsets(messageCharset->currentText()
                     ,composerCharset->currentText()
		     ,setDefault->isChecked());
    accept();
}

CharsetsDlg::~CharsetsDlg()
{
}
#endif
