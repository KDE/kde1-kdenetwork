
APPSDIR = $(kde_appsdir)/Settings/Network

INCLUDES = $(all_includes)
LDFLAGS = $(all_libraries) $(KDE_RPATH)
LDADD = -lkdeui -lkdecore -lmediatool  -lXext  $(LIB_QT) $(LIBCOMPAT) 

bin_PROGRAMS = kcmktalkd

#for extra warnings during compilation :
#DEFS		+=	-ansi -pedantic -D_POSIX_SOURCE -D_BSD_SOURCE

kcmktalkd_SOURCES = main.cpp soundpage.cpp answmachpage.cpp forwmachpage.cpp
kcmktalkd_METASOURCES = USE_AUTOMOC 

install-data-local: 
	$(mkinstalldirs) $(APPSDIR)
	$(INSTALL_DATA) kcmktalkd.kdelnk $(APPSDIR) 

uninstall-local:
	-rm -f $(APPSDIR)/syssound.kdelnk

messages:
	$(XGETTEXT) -C -ktranslate -ki18n -x $(includedir)/kde.pot $(kcmktalkd_SOURCES) && mv messages.po ../../po/kcmktalkd.pot

DISTCLEANFILES = $(kcmktalkd_METASOURCES)

