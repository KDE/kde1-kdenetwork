
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
INCLUDES= $(all_includes)
# claim, which subdirectories you want to install
# SUBDIRS = 
# KSTicker KSprogress

####### This part is very khexdit specific
# you can add here more. This one gets installed 
#bin_PROGRAMS = 	libpuke
#noinst_LIBRARIES = libpuke.a

lib_LIBRARIES = libkplunger.a

libkplunger_a_SOURCES = \
new.cpp

# no install headers

noinst_HEADERS = 

# just to make sure, automake makes them 
BUILTSOURCES = 

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(BUILTSOURCES)

# Datafiles to install / uninstall : *.pl *.pm
data_DATA = 

# Where to install them
datadir = $(kde_datadir)/ksirc

messages:
	$(XGETTEXT) -C -ktranslate -ki18n -x $(includedir)/kde.pot $(libpuke_la_SOURCES)

#
# add a dependency for every moc file to be full portable
# I've added a key binding to emacs for this. 
m%.cpp: %.h
	$(MOC) $< -o $@
