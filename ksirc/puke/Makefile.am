
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
#bin_PROGRAMS = 	libpuke
#noinst_LIBRARIES = libpuke.a

lib_LTLIBRARIES = libpuke.la libpframe.la libplined.la libpbutton.la libppushbt.la \
                  libpprogress.la libptablevw.la libplistbox.la libplabel.la \
                  libpmenudta.la libppopmenu.la libpalistbox.la libptabdialog.la \
                  libksirclistbox.la libpkfiledialog.la

libpuke_la_SOURCES = \
controller.cpp \
mcontroller.cpp \
pwidget.cpp \
mpwidget.cpp \
pobject.cpp \
mpobject.cpp \
playout.cpp \
mplayout.cpp \
kdynlib.cpp

libpuke_la_LDFLAGS = -version-info 0:1:0 -export-dynamic

libpframe_la_SOURCES = \
pframe.cpp \
mpframe.cpp

libpframe_la_LDFLAGS =  -export-dynamic  -nostdlib

libplined_la_SOURCES = \
plined.cpp \
mplined.cpp

libplined_la_LDFLAGS = -export-dynamic  -nostdlib

libpbutton_la_SOURCES = \
pbutton.cpp \
mpbutton.cpp

libpbutton_la_LDFLAGS = -export-dynamic  -nostdlib

libppushbt_la_SOURCES = \
ppushbt.cpp \
mppushbt.cpp

libppushbt_la_LDFLAGS = -export-dynamic  -nostdlib

libpprogress_la_SOURCES = \
pprogress.cpp \
mpprogress.cpp

libpprogress_la_LDFLAGS = -export-dynamic  -nostdlib

libptablevw_la_SOURCES = \
ptablevw.cpp \
mptablevw.cpp

libptablevw_la_LDFLAGS = -export-dynamic -nostdlib

libplistbox_la_SOURCES = \
plistbox.cpp \
mplistbox.cpp

libplistbox_la_LDFLAGS = -export-dynamic  -nostdlib

libplabel_la_SOURCES = \
plabel.cpp \
mplabel.cpp

libplabel_la_LDFLAGS = -export-dynamic  -nostdlib

libpmenudta_la_SOURCES = \
pmenudta.cpp \
mpmenudta.cpp

libpmenudta_la_LDFLAGS = -export-dynamic  -nostdlib

libppopmenu_la_SOURCES = \
ppopmenu.cpp  \
mppopmenu.cpp

libppopmenu_la_LDFLAGS = -export-dynamic  -nostdlib

libpalistbox_la_SOURCES = \
palistbox.cpp \
mpalistbox.cpp

libpalistbox_la_LDFLAGS = -export-dynamic -nostdlib

libptabdialog_la_SOURCES = \
ptabdialog.cpp  \
mptabdialog.cpp

libptabdialog_la_LDFLAGS = -export-dynamic  -nostdlib

libksirclistbox_la_SOURCES = \
ksirclistbox.cpp \
mksirclistbox.cpp

libksirclistbox_la_LDFLAGS = -export-dynamic

libpkfiledialog_la_SOURCES = \
pkfiledialog.cpp \
mpkfiledialog.cpp

libpkfiledialog_la_LDFLAGS = -export-dynamic  -nostdlib


HOWTO-PUKE.txt: HOWTO-PUKE.pod
	pod2text HOWTO-PUKE.pod > HOWTO-PUKE.txt

# no install headers

noinst_HEADERS = \
commands.h plined.h pwidget.h widgethdlr.h \
controller.h pframe.h pmessage.h HOWTO-PUKE.pod \
commands-handler.pl  pboxlayout.pm pwidget.pm \
commands-perl.pl pframe.pm test.pl convert_commands.pl \
plined.pm tester.pl pbase.pm puke.pl \
ppushbt.h \
pbutton.h \
pprogress.h \
ptablevw.h \
plistbox.h \
plabel.h \
pobject.h \
playout.h \
pmenudta.h \
ppopmenu.h \
kdynlib.h \
palistbox.h \
ptabdialog.h \
ksirclistbox.h

# just to make sure, automake makes them 
BUILTSOURCES = \
mcontroller.cpp \
mksirclistbox.cpp \
mpalistbox.cpp \
mpbutton.cpp \
mpframe.cpp \
mplabel.cpp \
mplayout.cpp \
mplined.cpp \
mplistbox.cpp \
mpmenudta.cpp \
mpobject.cpp \
mppopmenu.cpp \
mpprogress.cpp \
mppushbt.cpp \
mptabdialog.cpp \
mptablevw.cpp \
mpwidget.cpp

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(BUILTSOURCES)

# Datafiles to install / uninstall : *.pl *.pm
data_DATA = commands-handler.pl commands-perl.pl convert_commands.pl \
	puke.pl small.pl test.pl tester.pl \
	dcc_progress.pm dcc_status.pm ksirclistbox.pm load_all.pm \
	palistbox.pm pbase.pm pboxlayout.pm pbutton.pm pframe.pm \
	pkfiledialog.pm plabel.pm plined.pm plistbox.pm pmenudta.pm \
	ppopmenu.pm pprogress.pm ppushbt.pm ptabdialog.pm ptablevw.pm \
	pwidget.pm 

# Where to install them
datadir = $(kde_datadir)/ksirc

messages:
	$(XGETTEXT) -C -ktranslate -ki18n -x $(includedir)/kde.pot $(libpuke_la_SOURCES)

#
# add a dependency for every moc file to be full portable
# I've added a key binding to emacs for this. 
m%.cpp: %.h
	$(MOC) $< -o $@
