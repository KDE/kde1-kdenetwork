
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
noinst_LIBRARIES = libpuke.a

lib_LTLIBRARIES = libpframe.la libplined.la libpbutton.la libppushbt.la \
                  libpprogress.la libptablevw.la libplistbox.la libplabel.la \
                  libpmenudta.la libppopmenu.la

libpuke_a_SOURCES = \
controller.cpp \
mcontroller.cpp \
pwidget.cpp \
mpwidget.cpp \
pobject.cpp \
mpobject.cpp \
playout.cpp \
mplayout.cpp 


libpframe_la_SOURCES = \
pframe.cpp \
mpframe.cpp

libplined_la_SOURCES = \
plined.cpp \
mplined.cpp

libpbutton_la_SOURCES = \
pbutton.cpp \
mpbutton.cpp

libppushbt_la_SOURCES = \
ppushbt.cpp \
mppushbt.cpp

libpprogress_la_SOURCES = \
pprogress.cpp \
mpprogress.cpp

libptablevw_la_SOURCES = \
ptablevw.cpp \
mptablevw.cpp

libplistbox_la_SOURCES = \
plistbox.cpp \
mplistbox.cpp

libplabel_la_SOURCES = \
plabel.cpp \
mplabel.cpp

libpmenudta_la_SOURCES = \
pmenudta.cpp \
mpmenudta.cpp

libppopmenu_la_SOURCES = \
ppopmenu.cpp  \
mppopmenu.cpp

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
ppopmenu.h

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
	$(mkinstalldirs) $(kde_datadir)/ksirc
	$(INSTALL_DATA) *.pl *.pm $(kde_datadir)/ksirc

#
# remove ALL you have installed in install-data-local or install-exec-local
uninstall-local:

#
# add a dependency for every moc file to be full portable
# I've added a key binding to emacs for this. 
m%.cpp: %.h
	$(MOC) $< -o $@
