
INCLUDES = $(all_includes)
DEFS += -DNDEBUG 

LDADD = $(LIBCOMPAT) $(LIBSOCKET) $(all_libraries) -lkfm -lkhtmlw -ljscript -lkdeui -lkdecore $(LIBJPEG) -lgif -lqt -lXext -lX11 -lmimelib -luulib -lgdbm $(KDE_RPATH)

ICONDIR = $(kde_icondir)
MINIDIR =  $(kde_minidir)
# better would be Internet
UTILSDIR = $(kde_appsdir)/Utilities

BUILTSOURCES =	groupdlg.moc \
		NNTP.moc \
                artdlg.moc\
                kmessage.moc\
                asker.moc\
                identDlg.moc\
                NNTPConfigDlg.moc\
                rmbpop.moc\
                decoderDlg.moc\
                kdecode.moc\
                aboutDlg.moc\
                fontsDlg.moc\
                kmfolder.moc\
                kmfoldernode.moc\
                kmcomposewin.moc\
                KEdit.moc\
                findArtDlg.moc

DISTCLEANFILES = $(BUILTSOURCES)

bin_PROGRAMS =	krn

krn_SOURCES = 	main.cpp\
		NNTP.cpp\
		groupdlg.cpp\
		artdlg.cpp\
		kmessage.cpp\
		kmmessage.cpp\
                newsgroup.cpp\
                kstrtable.cpp\
                asker.cpp\
                askerData.cpp\
                identDlg.cpp\
                identDlgData.cpp\
                NNTPConfigDlg.cpp\
                NNTPConfigDlgData.cpp\
                decoderDlg.cpp\
                kformatter.cpp\
                rmbpop.cpp\
                kdecode.cpp\
                aboutDlg.cpp\
                aboutDlgData.cpp\
                ktempfile.cpp\
                kscorer.cpp\
                fontsDlg.cpp\
                fontsDlgData.cpp\
                kmmsgpart.cpp\
                kmfolder.cpp\
                kmmsginfo.cpp\
                kmfoldernode.cpp\
                kmcomposewin.cpp\
                KEdit.cpp\
                kmimemagic.cpp\
                kmsender.cpp\
                findArtDlgData.cpp\
                findArtDlg.cpp


noinst_HEADERS = 	NNTP.h\
                kmessage.h\
                kmmessage.h\
		artdlg.h\
                groupdlg.h\
                kmessage.h\
                kstrtable.h\
                asker.h\
                askerData.h\
                identDlg.h\
                identDlgData.h\
                NNTPConfigDlg.h\
                NNTPConfigDlgData.h\
                decoderDlg.h\
		kformatter.h\
		rmbpop.h\
		kdecode.h\
		aboutDlg.h\
		aboutDlgData.h\
		ktempfile.h\
		kscorer.h\
                fontsDlg.h\
                fontsDlgData.h\
                kmmsgpart.h\
                kmfolder.h\
                kmmsginfo.h\
                kmfoldernode.h\
                kmcomposewin.h\
                KEdit.h\
                kmimemagic.h\
                kmsender.h\
                mytree.h\
                findArtDlgData.h\
                findArtDlg.h

SUBDIRS = docs pics

install-local-data: 
	$(INSTALL_DIR) $(ICONDIR)
	$(INSTALL_DATA) krn.xpm $(ICONDIR)
	$(INSTALL_DIR) $(MINIDIR)
	$(INSTALL_DATA) mini-krn.xpm $(MINIDIR)/krn.xpm
	$(INSTALL_DIR) $(UTILSDIR)
	$(INSTALL_DATA) Krn.kdelnk $(UTILSDIR)

uninstall-local:
	rm -f $(ICONDIR)/krn.xpm
	rm -f $(MINIDIR)/krn.xpm
	rm -f $(UTILSDIR)/Krn.kdelnk


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

kmessage.cpp:kmessage.moc
kmessage.moc: kmessage.h
	$(MOC) kmessage.h -o kmessage.moc

asker.cpp:asker.moc
asker.moc: asker.h
	$(MOC) asker.h -o asker.moc

identDlg.cpp:identDlg.moc
identDlg.moc: identDlg.h
	$(MOC) identDlg.h -o identDlg.moc

decoderDlg.cpp:decoderDlg.moc
decoderDlg.moc: decoderDlg.h
	$(MOC) decoderDlg.h -o decoderDlg.moc

groupdlg.cpp:groupdlg.moc
groupdlg.moc: groupdlg.h
	$(MOC) groupdlg.h -o groupdlg.moc

NNTP.cpp:NNTP.moc
NNTP.moc: NNTP.h
	$(MOC) NNTP.h -o NNTP.moc

findArtDlg.cpp:findArtDlg.moc
findArtDlg.moc: findArtDlg.h
	$(MOC) findArtDlg.h -o findArtDlg.moc

KEdit.cpp:KEdit.moc
KEdit.moc: KEdit.h
	$(MOC) KEdit.h -o KEdit.moc

kmcomposewin.cpp:kmcomposewin.moc
kmcomposewin.moc: kmcomposewin.h
	$(MOC) kmcomposewin.h -o kmcomposewin.moc

kmfolder.cpp:kmfolder.moc
kmfolder.moc: kmfolder.h
	$(MOC) kmfolder.h -o kmfolder.moc

kmfoldernode.cpp:kmfoldernode.moc
kmfoldernode.moc: kmfoldernode.h
	$(MOC) kmfoldernode.h -o kmfoldernode.moc

