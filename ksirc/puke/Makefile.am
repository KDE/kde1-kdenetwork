
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
mpwidget.cpp


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

libpuke: $(libpukeOBJECTS) commands.h pframe.so plined.so pbutton.so ppushbt.so pprogress.so ptablevw.so plistbox.so plabel.so HOWTO-PUKE.txt
	./convert_commands.pl
	ar cr libpuke.a $(libpukeOBJECTS)
	ranlib libpuke.a || true

pframe.so: pframe.o pframe.h mpframe.o
	$(CXXCOMPILE) -fPIC -c pframe.cpp
	gcc -shared -Wl -o pframe.so pframe.o mpframe.o

mpframe.cpp: pframe.h
	moc pframe.h -o mpframe.cpp

mpframe.o: pframe.h mpframe.cpp
	$(CXXCOMPILE) -fPIC -c mpframe.cpp

plined.so: plined.o mplined.o
	$(CXXCOMPILE) -fPIC -c plined.cpp
	gcc -shared -Wl -o plined.so plined.o mplined.o

mplined.cpp: plined.h
	moc plined.h -o mplined.cpp

mplined.o: mplined.cpp plined.h
	$(CXXCOMPILE) -fPIC -c mplined.cpp

pbutton.so: pbutton.o mpbutton.o
	$(CXXCOMPILE) -fPIC -c pbutton.cpp
	gcc -shared -Wl -o pbutton.so pbutton.o mpbutton.o

mpbutton.cpp: pbutton.h
	moc pbutton.h -o mpbutton.cpp

mpbutton.o: mpbutton.cpp pbutton.h
	$(CXXCOMPILE) -fPIC -c mpbutton.cpp

ppushbt.so: ppushbt.cpp mppushbt.o
	$(CXXCOMPILE) -fPIC -c ppushbt.cpp
	gcc -shared -Wl -o ppushbt.so ppushbt.o mppushbt.o

mppushbt.cpp: ppushbt.h
	moc ppushbt.h -o mppushbt.cpp

mppushbt.o: mppushbt.cpp ppushbt.h
	$(CXXCOMPILE) -fPIC -c mppushbt.cpp

pprogress.so: pprogress.cpp mpprogress.o
	$(CXXCOMPILE) -fPIC -c pprogress.cpp
	gcc -shared -Wl -o pprogress.so pprogress.o mpprogress.o

mpprogress.cpp: pprogress.h
	moc pprogress.h -o mpprogress.cpp

mpprogress.o: mpprogress.cpp pprogress.h
	$(CXXCOMPILE) -fPIC -c mpprogress.cpp

ptablevw.so: ptablevw.cpp mptablevw.o
	$(CXXCOMPILE) -fPIC -c ptablevw.cpp
	gcc -shared -Wl -o ptablevw.so ptablevw.o mptablevw.o

mptablevw.cpp: ptablevw.h
	moc ptablevw.h -o mptablevw.cpp

mptablevw.o: mptablevw.cpp ptablevw.h
	$(CXXCOMPILE) -fPIC -c mptablevw.cpp
        
plistbox.so: plistbox.cpp mplistbox.o
	$(CXXCOMPILE) -fPIC -c plistbox.cpp
	gcc -shared -Wl -o plistbox.so plistbox.o mplistbox.o

mplistbox.cpp: plistbox.h
	moc plistbox.h -o mplistbox.cpp

mplistbox.o: mplistbox.cpp plistbox.h
	$(CXXCOMPILE) -fPIC -c mplistbox.cpp

plabel.so: plabel.cpp mplabel.o
	$(CXXCOMPILE) -fPIC -c plabel.cpp
	gcc -shared -Wl -o plabel.so plabel.o mplabel.o

mplabel.cpp: plabel.h
	moc plabel.h -o mplabel.cpp

mplabel.o: mplabel.cpp plabel.h
	$(CXXCOMPILE) -fPIC -c mplabel.cpp

HOWTO-PUKE.txt: HOWTO-PUKE.pod
	pod2text HOWTO-PUKE.pod > HOWTO-PUKE.txt

# no install headers

noinst_HEADERS = \
commands.h layout.h plined.h pwidget.h widgethdlr.h \
controller.h pframe.h pmessage.h widget.h HOWTO-PUKE.pod \
commands-handler.pl  pboxlayout.pm pwidget.pm \
commands-perl.pl pframe.pm test.pl convert_commands.pl \
plined.pm tester.pl pbase.pm puke.pl pframe.cpp plined.cpp \
ppushbt.h ppushbt.cpp \
pbutton.h pbutton.cpp \
pprogress.h pprogress.cpp \
ptablevw.cpp ptablevw.h \
plistbox.cpp plistbox.h \
plabel.cpp plabel.h



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
	$(INSTALL_DATA) *.pl *.pm *.so $(kde_datadir)/ksirc

#
# remove ALL you have installed in install-data-local or install-exec-local
uninstall-local:

#
# add a dependency for every moc file to be full portable
# I've added a key binding to emacs for this. 
m%.cpp: %.h
	$(MOC) $< -o $@
