
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
INCLUDES= $(all_includes)
# claim, which subdirectories you want to install
SUBDIRS = doc toolbar

####### This part is very kfinger specific
# you can add here more. This one gets installed 
bin_PROGRAMS = 	kfinger

# Which sources should be compiled for khexdit.
kfinger_SOURCES	= finger.C   kfinger.C   

# the library search path. 
kfinger_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# the libraries to link against. Be aware of the order. First the libraries,
# that depend on the following ones.
kfinger_LDADD   =  -lkfm -lkdecore -lkdeui  -lXext  $(LIB_QT)

# this option you can leave out. Just, if you use "make dist", you need it
#noinst_HEADERS = hexfile.h hexwidget.h

# just to make sure, automake makes them 
BUILTSOURCES =	kfinger.moc finger.moc

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(BUILTSOURCES)

messages:
	$(XGETTEXT) -C -ktranslate -ki18n -x $(includedir)/kde.pot $(kfinger_SOURCES) && mv messages.po ../po/kfinger.pot

# just install datas here. Use install-exec-local for scripts and etc.
# the binary itself is already installed from automake
# use mkinstalldirs, not "install -d"
# don't install a list of file. Just one file per install.
# if you have more of them, create a subdirectory with an extra Makefile 
install-data-local: 
	$(mkinstalldirs) $(APPSDIR)
	$(INSTALL_DATA) kfinger.kdelnk $(APPSDIR)
	$(mkinstalldirs) $(kde_confdir)
	$(INSTALL_DATA) kfingerrc $(kde_confdir)
#	$(mkinstalldirs) $(kde_minidir)
#	$(INSTALL_DATA) mini-khexdit.xpm $(kde_minidir)/khexdit.xpm

# remove ALL you have installed in install-data-local or install-exec-local
uninstall-local:
#	-rm -f $(APPSDIR)/khexdit.kdelnk
#	-rm -f $(kde_icondir)/khexdit.xpm
#	-rm -f $(kde_minidir)/khexdit.xpm

# add a dependency for every moc file to be full portable
# I've added a key binding to emacs for this. 
finger.C  :finger.moc
kfinger.C  :kfinger.moc

finger.moc: finger.h
	$(MOC) finger.h -o finger.moc
	
kfinger.moc: kfinger.h
	$(MOC) kfinger.h -o kfinger.moc

