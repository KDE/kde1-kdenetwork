
APPSDIR = $(kde_appsdir)/Settings/Network

INCLUDES = $(all_includes)
LDADD = $(all_libraries) -lkdeui -lkdecore -lmediatool -lqt -lXext -lX11 $(LIBSOCKET) $(LIBCOMPAT) $(KDE_RPATH)

bin_PROGRAMS = kcmktalkd

kcmktalkd_SOURCES = main.cpp soundpage.cpp answmachpage.cpp
kcmktalkd_METASOURCES = USE_AUTOMOC 

install-data-local: 
	$(mkinstalldirs) $(APPSDIR)
	$(INSTALL_DATA) kcmktalkd.kdelnk $(APPSDIR) 

uninstall-local:
	-rm -f $(APPSDIR)/syssound.kdelnk

messages:
	$(XGETTEXT) -C -ktranslate -ki18n $(kcmktalkd_SOURCES) && mv messages.po kcmktalkd.pot

DISTCLEANFILES = $(kcmktalkd_METASOURCES)

