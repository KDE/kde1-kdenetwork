
BUILT_SOURCES = moc_kmailbox.cpp moc_korn.cpp moc_kornbutt.cpp moc_kornopt.cpp moc_optdata.cpp

INCLUDES= $(all_includes)
bin_PROGRAMS = korn
korn_SOURCES = kmailbox.cpp kmailbox.h korn.cpp korn.h kornbutt.cpp kornbutt.h kornopt.cpp kornopt.h kornset.cpp kornset.h main.cpp optdata.cpp optdata.h real_from.cpp $(BUILT_SOURCES)
korn_LDADD = $(all_libraries) -lkdeui -lkdecore -lqt -lX11 -lXext
CLEANFILES = $(BUILT_SOURCES)

moc_kmailbox.cpp: kmailbox.h
	$(MOC) kmailbox.h -o moc_kmailbox.cpp

moc_korn.cpp: korn.h
	$(MOC) korn.h -o moc_korn.cpp

moc_kornbutt.cpp: kornbutt.h
	$(MOC) kornbutt.h -o moc_kornbutt.cpp

moc_kornopt.cpp: kornopt.h
	$(MOC) kornopt.h -o moc_kornopt.cpp

moc_optdata.cpp: optdata.h
	$(MOC) optdata.h -o moc_optdata.cpp

install-data-local:
	$(mkinstalldirs) $(kde_appsdir)/Network
	$(mkinstalldirs) $(kde_htmldir)/en/korn
	$(mkinstalldirs) $(kde_icondir)
	$(INSTALL_DATA) KOrn.kdelnk $(kde_appsdir)/Network
	$(INSTALL_DATA) doc/index.html $(kde_htmldir)/en/korn
	$(INSTALL_DATA) korn.xpm $(kde_icondir)

uninstall-local:
	rm -f $(kde_icondir)/korn.xpm
	rm -f $(kde_htmldir)/en/korn/index.html
	rm -f $(kde_appsdir)/Network/KOrn.kdelnk
	-rmdir $(kde_htmldir)/en/korn
	-rmdir $(kde_htmldir)/en/
	-rmdir $(kde_appsdir)/Network

