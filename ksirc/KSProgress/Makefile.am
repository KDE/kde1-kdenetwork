
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
#APPSDIR = $(kde_appsdir)/Internet
# set the include path for X, qt and KDE
INCLUDES= @all_includes@
# claim, which subdirectories you want to install
# SUBDIRS = 
# KSTicker KSprogress

####### This part is very khexdit specific
# you can add here more. This one gets installed 
noinst_LIBRARIES = libksprogress.a

libksprogress_a_SOURCES = \
ksprogress.cpp \
ksprogressdata.cpp \
mksprogress.cpp \
mksprogressdata.cpp

noinst_HEADERS = \
ksprogress.h      ksprogressdata.h

# just to make sure, automake makes them 
BUILTSOURCES = \
mksprogress.cpp \
mksprogressdata.cpp

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(BUILTSOURCES)

messages:
	$(XGETTEXT) -C -ktranslate -ki18n -x $(includedir)/kde.pot $(khexdit_SOURCES)

# just install datas here. Use install-exec-data for scripts and etc.
# the binary itself is already installed from automake
# use mkinstalldirs, not "install -d"
# don't install a list of file. Just one file per install.
# if you have more of them, create a subdirectory with an extra Makefile 
install-data-local: 
#	$(mkinstalldirs) $(APPSDIR)
#	$(INSTALL_DATA) ksirc.kdelnk $(APPSDIR)
#	$(mkinstalldirs) $(kde_icondir)
#	$(INSTALL_DATA) khexdit.xpm $(kde_icondir)
#	$(mkinstalldirs) $(kde_minidir)
#	$(INSTALL_DATA) mini-khexdit.xpm $(kde_minidir)/khexdit.xpm
#
# remove ALL you have installed in install-data-local or install-exec-local
uninstall-local:
#	-rm -f $(APPSDIR)/ksirc.kdelnk
#	-rm -f $(kde_icondir)/khexdit.xpm
#	-rm -f $(kde_minidir)/khexdit.xpm
#
# add a dependency for every moc file to be full portable
# I've added a key binding to emacs for this. 
m%.cpp: %.h
	$(MOC) $< -o $@

