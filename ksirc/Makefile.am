
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
INCLUDES= @all_includes@
DEFS = @DEFS@ -I. -I$(srcdir) -I..
# claim, which subdirectories you want to install
SUBDIRS = KSTicker KSProgress KSCutDialog KSPrefs KSColourPicker puke img doc

####### This part is very ksirc specific
# you can add here more. This one gets installed 
if COMPILE_KSIRC_SHARED
KSIRC_PROGRAM = ksirc
endif
if COMPILE_KSIRC_STATIC
KSIRCSTATIC_PROGRAM = ksircstatic
endif

bin_PROGRAMS = ${KSIRC_PROGRAM} ${KSIRCSTATIC_PROGRAM}

# Which sources should be compiled for ksirc.
ksirc_SOURCES	= toplevel.cpp \
ksirc.cpp \
irclistbox.cpp \
mirclistbox.cpp \
irclistitem.cpp \
mirclistitem.cpp \
alistbox.cpp \
mtoplevel.cpp \
ahistlineedit.cpp mahistlineedit.cpp \
malistbox.cpp \
iocontroller.cpp miocontroller.cpp \
open_top.cpp open_topData.cpp mopen_top.cpp mopen_topData.cpp \
messageReceiver.cpp \
ioBroadcast.cpp \
ksircprocess.cpp mksircprocess.cpp \
open_ksirc.cpp open_ksircData.cpp mopen_ksirc.cpp mopen_ksircData.cpp \
servercontroller.cpp mservercontroller.cpp \
KSircColour.cpp KSircColourData.cpp mKSircColour.cpp mKSircColourData.cpp \
FilterRuleEditor.cpp FilterRuleEditorData.cpp mFilterRuleEditor.cpp mFilterRuleEditorData.cpp \
ioDiscard.cpp \
ioDCC.cpp mioDCC.cpp dccDialog.cpp mdccDialog.cpp dccDialogData.cpp mdccDialogData.cpp \
linelistitem.cpp \
ioLAG.cpp mioLAG.cpp \
ioNotify.cpp mioNotify.cpp \
ssfeprompt.cpp mssfeprompt.cpp ssfepromptdata.cpp mssfepromptdata.cpp \
usercontrolmenu.cpp \
kspainter.cpp \
baserules.cpp \
estring.cpp \
chanparser.cpp \
objFinder.cpp \
mobjFinder.cpp

ksircstatic_SOURCES = $(ksirc_SOURCES)

# the library search path
ksirc_LDFLAGS = -export-dynamic @all_libraries@ # -L$(srcdir)/KSProgress -L$(srcdir)/KSTicker -L$(srcdir)/KSCutDialog -L$(srcdir)/KSPrefs -L$(srcdir)/KSColourPicker -L$(srcdir)/puke

ksircstatic_LDFLAGS = $(ksirc_LDFLAGS)

# the libraries to link against. Be aware of the order. First the libraries,
# that depend on the following ones.
ksirc_LDADD   = ./KSProgress/libksprogress.a ./KSTicker/libksticker.a ./KSCutDialog/libkscutdialog.a ./KSPrefs/libksprefs.a ./KSColourPicker/libkscolourpicker.a puke/libpuke.la -lkdeui -lkdecore -lkfile -lkfm -lqt -lX11 -lXext @LIBSOCKET@ -ldl $(KDE_RPATH)

ksircstatic_LDADD = ./KSProgress/libksprogress.a ./KSTicker/libksticker.a ./KSCutDialog/libkscutdialog.a ./KSPrefs/libksprefs.a ./KSColourPicker/libkscolourpicker.a ./puke/.libs/libpuke.a

if COMPILE_KSIRC_STATIC
STATIC_LIBS = $(static_libkde) $(static_libqt) $(static_libstdc) $(static_pthread) -lX11 -lXext @LIBSOCKET@ -ldl
endif

kdoc:
	kdoc -ddoc ksirc *.h

getdate: getdate.c
	$(COMPILE) -c getdate.c
	$(LINK) getdate.o

cdate.h: getdate
	./getdate > cdate.h

ksircstatic: $(ksircstatic_OBJECTS) $(ksircstatic_DEPENDENCIES) $(static_libkde) $(static_libqt)
	@rm -f ksircstatic
	$(LINK) $(ksircstatic_LDFLAGS) $(ksircstatic_OBJECTS) $(ksircstatic_LDADD) $(STATIC_LIBS) $(LIBS)

#dummy:


# this option you can leave out. Just, if you use "make dist", you need it
#
# DSIRC is on the bottom so make dist includes it!!!!!!!!!
#
noinst_HEADERS = \
FilterRuleEditor.h        control_message.h         linelistitem.h \
FilterRuleEditorData.h    dccDialog.h               messageReceiver.h \
KSircColour.h             dccDialogData.h           open_ksirc.h \
KSircColourData.h         ioBroadcast.h             open_ksircData.h \
ioDCC.h                   open_top.h                usercontrolmenu.h \
ioDiscard.h               open_topData.h            kspainter.h \
ahistlineedit.h           iocontroller.h            servercontroller.h \
alistbox.h                irclistbox.h              chanparser.h \
config.h                  irclistitem.h             toplevel.h \
ksircprocess.h            ksirc.kdelnk              cdate.h \
ioLAG.h                   ioNotify.h                ssfeprompt.h \
ssfepromptdata.h          usercontrolmenu.h         objFinder.h \
dsirc                     filters.pl                ksirc.pl \
sirc.help.gz              baserules.h               estring.h 


# just to make sure, automake makes them 
BUILTSOURCES = \
mFilterRuleEditor.cpp \
mFilterRuleEditorData.cpp \
mKSircColour.cpp \
mKSircColourData.cpp \
mahistlineedit.cpp \
malistbox.cpp \
mdccDialog.cpp \
mdccDialogData.cpp \
mioDCC.cpp \
mioLAG.cpp \
mioNotify.cpp \
miocontroller.cpp \
mirclistbox.cpp \
mirclistitem.cpp \
mksircprocess.cpp \
mobjFinder.cpp \
mopen_ksirc.cpp \
mopen_ksircData.cpp \
mopen_top.cpp \
mopen_topData.cpp \
mservercontroller.cpp \
mssfeprompt.cpp \
mssfepromptdata.cpp \
mtoplevel.cpp

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(BUILTSOURCES)

# make messages.po. Move this one to ../po/ and "make merge" in po
messages:
	$(XGETTEXT) -C -ktranslate $(ksirc_SOURCES)

# just install datas here. Use install-exec-data for scripts and etc.
# the binary itself is already installed from automake
# use mkinstalldirs, not "install -d"
# don't install a list of file. Just one file per install.
# if you have more of them, create a subdirectory with an extra Makefile 
install-data-local: 
	$(mkinstalldirs) $(APPSDIR)
	$(INSTALL_DATA) ksirc.kdelnk $(APPSDIR)
	$(mkinstalldirs) $(kde_datadir)/ksirc
	$(INSTALL_DATA) dsirc $(bindir)
	$(INSTALL_DATA) ksirc.pl $(kde_datadir)/ksirc
	$(INSTALL_DATA) filters.pl $(kde_datadir)/ksirc
	$(INSTALL_DATA) sirc.help.gz $(kde_datadir)/ksirc
#
# remove ALL you have installed in install-data-local or install-exec-local
uninstall-local:
	-rm -f $(APPSDIR)/ksirc.kdelnk
	-rm -f $(bindir)/dsirc
	-rm -f $(kde_datadir)/ksirc/filters.pl
#
# add a dependency for every moc file to be full portable
# I've added a key binding to emacs for this. 
m%.cpp: %.h
	moc $< -o $@

