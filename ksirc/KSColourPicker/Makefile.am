
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

INCLUDES= $(all_includes) -I. -I$(srcdir) -I../.. -include ../kplunger/kplunger.h

# claim, which subdirectories you want to install
# SUBDIRS = 
# KSTicker KSprogress

####### This part is very khexdit specific
# you can add here more. This one gets installed 
noinst_LIBRARIES = libkscolourpicker.a

libkscolourpicker_a_SOURCES = \
kscolourpicker.cpp \
mkscolourpicker.cpp \
kscolourpickerData.cpp \
mkscolourpickerData.cpp

noinst_HEADERS = \
kscolourpicker.h \
kscolourpickerData.h

# just to make sure, automake makes them 
BUILTSOURCES = \
mkscolourpicker.cpp \
mkscolourpickerData.cpp 

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(BUILTSOURCES)

messages:
	$(XGETTEXT) -C -ktranslate -ki18n -x $(includedir)/kde.pot $(libkscolourpicker_a_SOURCES)

# just install datas here. Use install-exec-data for scripts and etc.
# the binary itself is already installed from automake
# use mkinstalldirs, not "install -d"
# don't install a list of file. Just one file per install.
# if you have more of them, create a subdirectory with an extra Makefile 
install-data-local: 

#
# remove ALL you have installed in install-data-local or install-exec-local
uninstall-local:

#
# add a dependency for every moc file to be full portable
# I've added a key binding to emacs for this. 
m%.cpp: %.h
	$(MOC) $< -o $@

