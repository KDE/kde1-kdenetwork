# Yow Emacs, this is a -*- makefile -*-

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

# if you use a variable *dir and have *_DATA, it will be installed by
# make install
appsdir = $(kde_appsdir)/Internet
apps_DATA = Kppp.kdelnk

EXTRA_DIST = $(apps_DATA)

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)
# claim, which subdirectories you want to install
SUBDIRS = docs pixmaps icons logview Rules

# This one gets installed 
bin_PROGRAMS = 	kppp #kpppsetup

#kpppsetup_SOURCES= setup.cpp pppdata.cpp general.cpp edit.cpp scriptedit.cpp accounts.cpp  modeminfo.cpp miniterm.cpp modem.cpp modemcmds.cpp iplined.cpp pppdargs.cpp acctselect.cpp

#kpppsetup_CFLAGS= -DSETUPMODE

# Which sources should be compiled for kppp.
kppp_SOURCES	= accounts.cpp \
		connect.cpp \
		conwindow.cpp \
		debug.cpp \
		edit.cpp \
		general.cpp \
		iplined.cpp \
		main.cpp \
                modem.cpp \
		modemcmds.cpp \
		pppdargs.cpp \
		pppdata.cpp \
		scriptedit.cpp\
		pwentry.cpp \
		modeminfo.cpp\
		pppstatdlg.cpp\
		pppstats.cpp\
		miniterm.cpp\
		accounting.cpp \
		acctselect.cpp\
		ruleset.cpp \
		pap.cpp \
		chap.cpp \
		docking.cpp \
		runtests.cpp \
		loginterm.cpp \
		ppplog.cpp \
		log.cpp \
		groupbox.cpp \
		newwidget.cpp

# the library search path
kppp_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#kpppsetup_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# the libraries to link against. Be aware of the order. First the libraries,
# that depend on the following ones.
kppp_LDADD   = -lkdeui -lkdecore -lqt -lXext -lX11 -lm $(LIBSOCKET)
#kpppsetup_LDADD   = -lkdeui -lkdecore -lqt -lXext -lX11 -lm $(LIBSOCKET)

# this option you can leave out. Just, if you use "make dist", you need it
noinst_HEADERS = accounts.h \
		kpppconfig.h \
		connect.h \
		conwindow.h \
		debug.h \
		edit.h \
		general.h \
		iplined.h \
		main.h \
                modem.h \
		modemcmds.h \
		pppdargs.h \
		pppdata.h \
		scriptedit.h\
		pwentry.h \
		modeminfo.h\
		pppstatdlg.h\
		miniterm.h\
		accounting.h \
		acctselect.h\
		ruleset.h \
		pap.h \
		chap.h \
		docking.h \
		runtests.h \
		loginterm.h \
		auth.h \
		version.h \
		macros.h \
		ppplog.h \
		log.h \
		groupbox.h \
		newwidget.cpp

# just to make sure, automake makes them 
BUILTSOURCES = accounts.moc \
		connect.moc \
		conwindow.moc \
		debug.moc \
		edit.moc \
		general.moc \
		main.moc \
		modemcmds.moc \
		pppdargs.moc \
		scriptedit.moc\
		pwentry.moc\
		modeminfo.moc\
		pppstatdlg.moc\
		miniterm.moc\
		accounting.moc\
		acctselect.moc\
		docking.moc \
		setup.moc \
		groupbox.moc

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(BUILTSOURCES)

messages:
	$(XGETTEXT) -C -ktranslate -ki18n -x $(includedir)/kde.pot $(kppp_SOURCES) && mv messages.po ../po/kppp.pot

# just install datas here. Use install-exec-data for scripts and etc.
# the binary itself is already installed from automake
# use mkinstalldirs, not "install -d"
# don't install a list of file. Just one file per install.
# if you have more of them, create a subdirectory with an extra Makefile 
install-data-local:	
#	@(cd Rules && $(MAKE) install)
	@echo ""
	@echo "Don't forget to send me a phone tarif rules file for your"
	@echo "telecom company/country/region. Thanks."
	@echo ""
	@echo "I am maintaining an archive on the kppp page at"
	@echo "http://math.cornell.edu/~wuebben/kde.html"
	@echo "WARNING: kppp is by default installed with SETUID bit on!"
	@echo  "If you are concerned about the SETUID bit, create a modem"
	@echo "group instead."
	@echo ""
	@echo "Happy Dialing! -- the KPPP team"
	@echo ""

# kppp wants to be installed suid root
install-exec-hook:
	(chown root $(bindir)/kppp && chmod 4755 $(bindir)/kppp) || echo "Was not able to make kppp setuid root"

# remove ALL you have installed in install-data-local or install-exec-local
#uninstall-local:
#	@(cd Rules && $(MAKE) uninstall)

#distclean-local:
#	@(cd Rules && $(MAKE) distclean)

# MOC files
accounting.cpp: accounting.moc;
accounting.moc: accounting.h
	$(MOC) accounting.h -o accounting.moc

accounts.cpp: accounts.moc;
accounts.moc: accounts.h
	$(MOC) accounts.h -o accounts.moc

acctselect.cpp: acctselect.moc;
acctselect.moc: acctselect.h
	$(MOC) acctselect.h -o acctselect.moc

connect.cpp: connect.moc;
connect.moc: connect.h
	$(MOC) connect.h -o connect.moc

conwindow.cpp: conwindow.moc;
conwindow.moc: conwindow.h
	$(MOC) conwindow.h -o conwindow.moc

debug.cpp: debug.moc;
debug.moc: debug.h
	$(MOC) debug.h -o debug.moc

docking.cpp: docking.moc;
docking.moc: docking.h
	$(MOC) docking.h -o docking.moc

edit.cpp: edit.moc;
edit.moc: edit.h
	$(MOC) edit.h -o edit.moc

general.cpp: general.moc;
general.moc: general.h
	$(MOC) general.h -o general.moc

loginterm.cpp: loginterm.moc;
loginterm.moc: loginterm.h
	$(MOC) loginterm.h -o loginterm.moc

main.cpp: main.moc;
main.moc: main.h
	$(MOC) main.h -o main.moc

miniterm.cpp: miniterm.moc;
miniterm.moc: miniterm.h
	$(MOC) miniterm.h -o miniterm.moc

modemcmds.cpp: modemcmds.moc;
modemcmds.moc: modemcmds.h
	$(MOC) modemcmds.h -o modemcmds.moc

modeminfo.cpp: modeminfo.moc;
modeminfo.moc: modeminfo.h
	$(MOC) modeminfo.h -o modeminfo.moc

pppdargs.cpp: pppdargs.moc;
pppdargs.moc: pppdargs.h
	$(MOC) pppdargs.h -o pppdargs.moc

pppstatdlg.cpp: pppstatdlg.moc;
pppstatdlg.moc: pppstatdlg.h
	$(MOC) pppstatdlg.h -o pppstatdlg.moc

pwentry.cpp: pwentry.moc;
pwentry.moc: pwentry.h
	$(MOC) pwentry.h -o pwentry.moc

scriptedit.cpp: scriptedit.moc;
scriptedit.moc: scriptedit.h
	$(MOC) scriptedit.h -o scriptedit.moc

setup.cpp: setup.moc
setup.moc: setup.h
	$(MOC) setup.h -o setup.moc


groupbox.cpp: groupbox.moc
groupbox.moc: groupbox.h
	$(MOC) groupbox.h -o groupbox.moc
