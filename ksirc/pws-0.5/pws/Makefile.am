# this 10 paths are KDE specific. Use them:
# kde_htmldir       Where your docs should go to. (contains lang subdirs)
# kde_appsdir       Where your application file (.kdelnk) should go to. 
# kde_icondir       Where your icon should go to.
# kde_minidir       Where your mini icon should go to.
# kde_datadir       Where you install application data. (Use a subdir)
# kde_locale        Where translation files should go to.(contains lang subdirs)
# kde_cgidir        Where cgi-bin executables should go to.
# kde_confdir       Where config files should go to.
# kde_mimedir       Where mimetypes should go to.
# kde_toolbardir    Where general toolbar icons should go to.
# kde_wallpaperdir  Where general wallpapers should go to.

# just set the variable
APPSDIR = $(kde_appsdir)/Internet
# set the include path for X, qt and KDE
INCLUDES= $(all_includes) -I. -I../../.. -include ../../kplunger/kplunger.h

SUBDIRS = doc pics

####### This part is very pws specific
# you can add here more. This one gets installed 
noinst_LIBRARIES = libpws.a

# Which sources should be compiled for pws.


libpws_a_SOURCES = \
                  pws.cpp\
                  pwswidget.cpp\
                  typelayout.cpp\
                  tlform.cpp\
                  tlwidget.cpp\
                  pages.cpp\
                  kfileio.cpp\
                  addwizard.cpp\
                  server.cpp

# this option you can leave out. Just, if you use "make dist", you need it
noinst_HEADERS = pws.h pwswidget.h server.h

# just to make sure, automake makes them 
BUILTSOURCES =	pws.moc pwswidget.moc server.moc

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(BUILTSOURCES)

# make messages.po. Move this one to ../po/ and "make merge" in po
messages:
	$(XGETTEXT) -C -ki18n $(pws_SOURCES)

# just install datas here. Use install-exec-data for scripts and etc.
# the binary itself is already installed from automake
# use mkinstalldirs, not "install -d"
# don't install a list of file. Just one file per install.
# if you have more of them, create a subdirectory with an extra Makefile 
install-data-local: 
	$(mkinstalldirs) $(APPSDIR)
	$(INSTALL_DATA) pws.kdelnk $(APPSDIR)
	$(mkinstalldirs) $(kde_icondir)
	$(INSTALL_DATA) pws.xpm $(kde_icondir)
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-pws.xpm $(kde_minidir)/pws.xpm

# remove ALL you have installed in install-data-local or install-exec-local
uninstall-local:
	-rm -f $(APPSDIR)/pws.kdelnk
	-rm -f $(kde_icondir)/pws.xpm
	-rm -f $(kde_minidir)/pws.xpm

# add a dependency for every moc file to be full portable
pws.cpp:pws.moc
pws.moc: pws.h
	$(MOC) pws.h -o pws.moc

pwswidget.cpp:pwswidget.moc
pwswidget.moc: pwswidget.h
	$(MOC) pwswidget.h -o pwswidget.moc

typelayout.cpp:typelayout.moc
typelayout.moc: typelayout.h
	$(MOC) typelayout.h -o typelayout.moc

asker.cpp:asker.moc
asker.moc: asker.h
	$(MOC) asker.h -o asker.moc

pages.cpp:pages.moc
pages.moc: pages.h
	$(MOC) pages.h -o pages.moc

addwizard.cpp:addwizard.moc
addwizard.moc: addwizard.h
	$(MOC) addwizard.h -o addwizard.moc

server.cpp:server.moc
server.moc: server.h
	$(MOC) server.h -o server.moc
        
