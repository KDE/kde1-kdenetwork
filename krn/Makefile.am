
INCLUDES = $(all_includes)
DEFS = @DEFS@ -DKRN -DCHARSETS

LDFLAGS = $(all_libraries) $(KDE_RPATH)
krn_LDADD =  -lkspell  $(LIB_KHTMLW)  $(LIB_KFILE) \
	 -lmimelib -lgdbm $(LIBCOMPAT) 

kdecode_LDADD =    $(LIB_KFILE) \
	 $(LIBUU) $(LIBCOMPAT) 

newkrn_LDADD =   $(LIB_KHTMLW)  $(LIB_KFILE) \
	 -lmimelib -lgdbm $(LIBCOMPAT) 


ICONDIR = $(kde_icondir)
MINIDIR =  $(kde_minidir)
# better would be Internet
APPSDIR = $(kde_appsdir)/Internet

BUILTSOURCES =	groupdlg.moc \
		NNTP.moc \
                artdlg.moc\
                asker.moc\
                identDlg.moc\
                NNTPConfigDlg.moc\
                rmbpop.moc\
                decoderDlg.moc\
                kdecode.moc\
                aboutDlg.moc\
                fontsDlg.moc\
                kmcomposewin.moc\
                findArtDlg.moc\
                kmmsgpartdlg.moc\
                kbusyptr.moc\
                kalarmtimer.moc\
                kmreaderwin.moc\
                kpgp.moc\
                krnsender.moc\
                kmsender.moc\
                kmaddrbookdlg.moc\
                ktreeview.moc\
                expireDlg.moc\
                printdlg.moc\
                rulesDlg.moc\
                sortDlg.moc\
		charsetsDlg.moc\
                expirestatusdlg.moc\
                kmtopwidget.moc\
                TooManydlg.moc

DISTCLEANFILES = $(BUILTSOURCES)

bin_PROGRAMS =	krn kdecode newkrn

newkrn_SOURCES = new_main.cpp\
		 new_groupdlg.cpp\
                 new_globals.cpp\
                 kfileio.cpp

kdecode_SOURCES = kdecodemain.cpp\
                decoderDlg.cpp\
                kdecode.cpp

krn_SOURCES = 	main.cpp\
		NNTP.cpp\
		groupdlg.cpp\
		artdlg.cpp\
                newsgroup.cpp\
                asker.cpp\
                identDlg.cpp\
                NNTPConfigDlg.cpp\
                rmbpop.cpp\
                aboutDlg.cpp\
                ktempfile.cpp\
                kscorer.cpp\
                fontsDlg.cpp\
                kmmessage.cpp\
                kmmsgpart.cpp\
                kmmsginfo.cpp\
                kmcomposewin.cpp\
                kmimemagic.cpp\
                kmsender.cpp\
                findArtDlg.cpp\
                kmmsgbase.cpp\
                kmidentity.cpp\
                kfileio.cpp\
                kmmsgpartdlg.cpp\
                kbusyptr.cpp\
                kalarmtimer.cpp\
                kmreaderwin.cpp\
                kpgp.cpp\
                krnsender.cpp\
                kmaddrbookdlg.cpp\
                kmaddrbook.cpp\
                ktreeview.cpp\
                expireDlg.cpp\
                tlform.cpp\
                tlwidget.cpp\
                typelayout.cpp\
                printdlg.cpp\
                rulesDlg.cpp\
                sortDlg.cpp\
                rules.cpp\
		charsetsDlg.cpp\
                expirestatusdlg.cpp\
                kfontutils.cpp \
                kmtopwidget.cpp\
                kpgpbase.cpp\
		TooManydlg.cpp

noinst_HEADERS = 	NNTP.h\
                kmmessage.h\
		artdlg.h\
                groupdlg.h\
                asker.h\
                identDlg.h\
                NNTPConfigDlg.h\
                decoderDlg.h\
		rmbpop.h\
		kdecode.h\
		aboutDlg.h\
		ktempfile.h\
		kscorer.h\
                fontsDlg.h\
                kmmsgpart.h\
                kmmsginfo.h\
                kmcomposewin.h\
                kmimemagic.h\
                kmsender.h\
                mytree.h\
                findArtDlg.h\
                kmmsgbase.h\
                kmidentity.h\
                kfileio.h\
                kmmsgpartdlg.h\
                kbusyptr.h\
                kalarmtimer.h\
                kmreaderwin.h\
                kpgp.h\
                krnsender.h\
                kmaddrbookdlg.h\
                kmaddrbook.h\
                ktreeview.h\
                expireDlg.h\
                tlform.h\
                tlwidget.h\
                typelayout.h\
                printdlg.h\
                rulesDlg.h\
                sortDlg.h\
                rules.h\
		charsetsDlg.h\
                expirestatusdlg.h\
                kfontutils.h\
                kmtopwidget.h\
		kmiostatusdlg.h\
		kpgpbase.h\
		TooManydlg.h




SUBDIRS = docs pics

EXTRA_DIST = krn.xpm mini-krn.xpm Krn.kdelnk

install-data-local:
	$(mkinstalldirs) $(ICONDIR)
	$(INSTALL_DATA) krn.xpm $(ICONDIR)
	$(mkinstalldirs) $(MINIDIR)
	$(INSTALL_DATA) mini-krn.xpm $(MINIDIR)/krn.xpm
	$(mkinstalldirs) $(APPSDIR)
	$(INSTALL_DATA) Krn.kdelnk $(APPSDIR)

uninstall-local:
	rm -f $(ICONDIR)/krn.xpm
	rm -f $(MINIDIR)/krn.xpm
	rm -f $(APPSDIR)/Krn.kdelnk


# moc rules

NNTPConfigDlg.cpp:NNTPConfigDlg.moc
NNTPConfigDlg.moc: NNTPConfigDlg.h
	$(MOC) NNTPConfigDlg.h -o NNTPConfigDlg.moc

rmbpop.cpp:rmbpop.moc
rmbpop.moc: rmbpop.h
	$(MOC) rmbpop.h -o rmbpop.moc

aboutDlg.cpp:aboutDlg.moc
aboutDlg.moc: aboutDlg.h
	$(MOC) aboutDlg.h -o aboutDlg.moc

fontsDlg.cpp:fontsDlg.moc
fontsDlg.moc: fontsDlg.h
	$(MOC) fontsDlg.h -o fontsDlg.moc

artdlg.cpp:artdlg.moc
artdlg.moc: artdlg.h
	$(MOC) artdlg.h -o artdlg.moc

kdecode.cpp:kdecode.moc
kdecode.moc: kdecode.h
	$(MOC) kdecode.h -o kdecode.moc

asker.cpp:asker.moc
asker.moc: asker.h
	$(MOC) asker.h -o asker.moc

NNTP.cpp:NNTP.moc
NNTP.moc: NNTP.h
	$(MOC) NNTP.h -o NNTP.moc

identDlg.cpp:identDlg.moc
identDlg.moc: identDlg.h
	$(MOC) identDlg.h -o identDlg.moc

decoderDlg.cpp:decoderDlg.moc
decoderDlg.moc: decoderDlg.h
	$(MOC) decoderDlg.h -o decoderDlg.moc

groupdlg.cpp:groupdlg.moc
groupdlg.moc: groupdlg.h
	$(MOC) groupdlg.h -o groupdlg.moc


findArtDlg.cpp:findArtDlg.moc
findArtDlg.moc: findArtDlg.h
	$(MOC) findArtDlg.h -o findArtDlg.moc

kmcomposewin.cpp:kmcomposewin.moc
kmcomposewin.moc: kmcomposewin.h
	$(MOC) kmcomposewin.h -o kmcomposewin.moc

kmmsgpartdlg.cpp:kmmsgpartdlg.moc
kmmsgpartdlg.moc: kmmsgpartdlg.h
	$(MOC) kmmsgpartdlg.h -o kmmsgpartdlg.moc

kbusyptr.cpp:kbusyptr.moc
kbusyptr.moc: kbusyptr.h
	$(MOC) kbusyptr.h -o kbusyptr.moc

kalarmtimer.cpp:kalarmtimer.moc
kalarmtimer.moc: kalarmtimer.h
	$(MOC) kalarmtimer.h -o kalarmtimer.moc

kmreaderwin.cpp:kmreaderwin.moc
kmreaderwin.moc: kmreaderwin.h
	$(MOC) kmreaderwin.h -o kmreaderwin.moc

kpgp.cpp:kpgp.moc
kpgp.moc: kpgp.h
	$(MOC) kpgp.h -o kpgp.moc

kmsender.cpp:kmsender.moc
kmsender.moc: kmsender.h
	$(MOC) kmsender.h -o kmsender.moc

krnsender.cpp:krnsender.moc
krnsender.moc: krnsender.h
	$(MOC) krnsender.h -o krnsender.moc

kmaddrbookdlg.cpp:kmaddrbookdlg.moc
kmaddrbookdlg.moc: kmaddrbookdlg.h
	$(MOC) kmaddrbookdlg.h -o kmaddrbookdlg.moc

ktreeview.cpp:ktreeview.moc
ktreeview.moc: ktreeview.h
	$(MOC) ktreeview.h -o ktreeview.moc


expireDlg.cpp:expireDlg.moc
expireDlg.moc: expireDlg.h
	$(MOC) expireDlg.h -o expireDlg.moc

printdlg.cpp:printdlg.moc
printdlg.moc: printdlg.h
	$(MOC) printdlg.h -o printdlg.moc

rulesDlg.cpp:rulesDlg.moc
rulesDlg.moc: rulesDlg.h
	$(MOC) rulesDlg.h -o rulesDlg.moc

sortDlg.cpp:sortDlg.moc
sortDlg.moc: sortDlg.h
	$(MOC) sortDlg.h -o sortDlg.moc

charsetsDlg.cpp:charsetsDlg.moc
charsetsDlg.moc: charsetsDlg.h
	$(MOC) charsetsDlg.h -o charsetsDlg.moc

expirestatusdlg.cpp:expirestatusdlg.moc
expirestatusdlg.moc: expirestatusdlg.h
	$(MOC) expirestatusdlg.h -o expirestatusdlg.moc


kmtopwidget.cpp:kmtopwidget.moc
kmtopwidget.moc: kmtopwidget.h
	$(MOC) kmtopwidget.h -o kmtopwidget.moc

new_groupdlg.cpp:new_groupdlg.moc
new_groupdlg.moc: new_groupdlg.h
	$(MOC) new_groupdlg.h -o new_groupdlg.moc

TooManydlg.cpp:TooManydlg.moc
TooManydlg.moc: TooManydlg.h
	$(MOC) TooManydlg.h -o TooManydlg.moc


messages:
	$(XGETTEXT) -C -ktranslate -ki18n -x $(includedir)/kde.pot $(krn_SOURCES) && mv messages.po ../po/krn.pot
