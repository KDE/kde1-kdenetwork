
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
noinst_LIBRARIES = libksprefs.a

libksprefs_a_SOURCES = \
ksprefs.cpp \
mksprefs.cpp \
general.cpp \
mgeneral.cpp \
generalData.cpp \
mgeneralData.cpp \
startup.cpp \
mstartup.cpp \
startupdata.cpp \
mstartupdata.cpp \
serverchannel.cpp \
mserverchannel.cpp \
serverchanneldata.cpp \
mserverchanneldata.cpp \
UserMenuRef.cpp \
mUserMenuRef.cpp \
UserMenuRefData.cpp \
mUserMenuRefData.cpp \
kspainter.cpp \
mkspainter.cpp \
defaultfilters.cpp \
mdefaultfilters.cpp \
defaultfiltersdata.cpp \
mdefaultfiltersdata.cpp

noinst_HEADERS = \
ksprefs.h \
serverchanneldata.h \
generalData.h \
startupdata.h \
serverchannel.h \
general.h \
startup.h \
UserMenuRef.h \
UserMenuRefData.h \
kspainter.h \
defaultfilters.h \
defaultfiltersdata.h

# just to make sure, automake makes them 
BUILTSOURCES =	

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(BUILTSOURCES)

# make messages.po. Move this one to ../po/ and "make merge" in po
messages:
	$(XGETTEXT) -C -ktranslate $(khexdit_SOURCES)

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
	moc $< -o $@

