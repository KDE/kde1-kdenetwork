
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
# claim, which subdirectories you want to install
SUBDIRS = KSTicker KSProgress KSCutDialog KSPrefs img doc

####### This part is very khexdit specific
# you can add here more. This one gets installed 
bin_PROGRAMS = 	ksirc ksircstatic

# Which sources should be compiled for khexdit.
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
kspainter.cpp

ksircstatic_SOURCES = $(ksirc_SOURCES)

# the library search path
ksirc_LDFLAGS = @all_libraries@ -L./KSProgress/ -L./KSTicker/ -L./KSCutDialog/ -LKSPrefs

ksircstatic_LDFLAGS = -static $(ksirc_LDFLAGS)

# the libraries to link against. Be aware of the order. First the libraries,
# that depend on the following ones.
ksirc_LDADD   = -lksprogress -lksticker -lkscutdialog -lksprefs -lkdeui -lkdecore -lqt -lX11 -lXext @LIBSOCKET@     

ksircstatic_LDADD = $(ksirc_LDADD)

kdoc:
	kdoc -ddoc ksirc *.h

ksircstatic: $(ksircstatic_OBJECTS) $(ksircstatic_DEPENDENCIES)
	@rm -f ksircstatic
	@echo "*** Ignore any failures ***"
	$(CXXLINK) $(ksircstatic_LDFLAGS) $(ksircstatic_OBJECTS) $(ksircstatic_LDADD) $(LIBS) || true
	@touch ksircstatic
	@echo "*** Ignore previous failure, if any ***"

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
alistbox.h                irclistbox.h              \
config.h                  irclistitem.h             toplevel.h \
ksircprocess.h            ksirc.kdelnk \
ioLAG.h                   ioNotify.h                ssfeprompt.h \
ssfepromptdata.h          usercontrolmenu.h\
dsirc                     filters.pl                ksirc.pl \
sirc.help.gz


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

