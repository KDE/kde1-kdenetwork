
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
bin_PROGRAMS = 	libpuke

libpuke_SOURCES = \
controller.cpp \
mcontroller.cpp \
widget.cpp \
mwidget.cpp \
layout.cpp \
mlayout.cpp \
pwidget.cpp \
mpwidget.cpp \

noinst_HEADERS = \
commands.h    pframe.h      pwidget.h     widgethdlr.h \
controller.h  pmessage.h    widget.h \
commands-handler.pl  convert_commands.pl  tester.pl \
commands-perl.pl     puke.pl \
pframe.pm   pwidget.pm layout.h


libpukeSOURCES = $(libkscutdialog_SOURCES)

libpukeOBJECTS = \
controller.o \
mcontroller.o \
widget.o \
mwidget.o \
layout.o \
mlayout.o \
pwidget.o \
mpwidget.o

libpuke.c: commands.h
	perl convert_commands.pl
	@touch libpuke.c
	@echo "Done"

libpuke: $(libpukeOBJECTS) commands.h pframe.so plined.so HOWTO-PUKE.txt
	./convert_commands.pl
	ar cr libpuke.a $(libpukeOBJECTS)
	ranlib libpuke.a || true

pframe.so: pframe.cpp pframe.h mpframe.o
	$(CXXCOMPILE) -fPIC -c pframe.cpp
	gcc -shared -Wl -o pframe.so pframe.o mpframe.o

mpframe.o: pframe.h
	$(CXXCOMPILE) -fPIC -c mpframe.cpp

plined.so: plined.cpp plined.h mplined.o
	$(CXXCOMPILE) -fPIC -c plined.cpp
	gcc -shared -Wl -o plined.so plined.o mplined.o

mplined.o: mplined.cpp plined.h
	$(CXXCOMPILE) -fPIC -c mplined.cpp

HOWTO-PUKE.txt: HOWTO-PUKE.pod
	pod2text HOWTO-PUKE.pod > HOWTO-PUKE.txt

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
	moc $< -o $@

