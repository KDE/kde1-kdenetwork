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
# set the include path for X, qt, KDE and bsd
INCLUDES = $(all_includes) -I. -I..
# claim, which subdirectories you want to install
#SUBDIRS = doc pics
SUBDIRS = doc
VERSION=1.1.2

# own defines
DEFS = @DEFS@ -DKARCHIE -DKARCHIE_VERSION='"$(VERSION)"'
# -pedantic

####### This part is very  specific
# you can add here more. This one gets installed 
bin_PROGRAMS = 	karchie

# Which sources should be compiled for karchie.
karchie_SOURCES = KAMenu.cpp KAQueryList.cpp KAStatusBar.cpp KAQuery.cpp \
     KASettings.cpp KAView.cpp KAQueryFile.cpp \
     KASettingsTypes.cpp KArchie.cpp \
     aquery.c dirsend.c get_vdir.c stcopy.c vl_comp.c \
     atalloc.c get_pauth.c ptalloc.c support.c vlalloc.c 

#	  $(BUILTSOURCES)

# the library search path
karchie_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# the libraries to link against. Be aware of the order. First the libraries,
# that depend on the following ones.
karchie_LDADD   = -lkfile -lkfm       $(LIBCOMPAT) $(LIB_KDEUI)

# this option you can leave out. Just, if you use "make dist", you need it
noinst_HEADERS =

# extra files for "make dist"
#EXTRA_DIST += VERSION CHANGES karchie.kdelnk karchie.xpm mini-karchie.xpm
EXTRA_DIST += VERSION CHANGES karchierc

# just to make sure, automake makes them 
BUILTSOURCES = KAMenu.moc KAQueryList.moc KAStatusBar.moc KAQuery.moc \
  KASettings.moc KAView.moc KAQueryFile.moc KASettingsTypes.moc KArchie.moc


# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
CLEANFILES = $(BUILTSOURCES)

# make messages.po. Move this one to ../po/ and "make merge" in po
messages:
	$(XGETTEXT) -C -ki18n -x $(includedir)/kde.pot *.cpp && mv messages.po ../po/karchie.pot

# just install datas here. Use install-exec-data for scripts and etc.
# the binary itself is already installed from automake
# use mkinstalldirs, not "install -d"
# don't install a list of file. Just one file per install.
# if you have more of them, create a subdirectory with an extra Makefile 
install-data-local: 
	$(mkinstalldirs) $(kde_confdir)
	$(INSTALL_DATA) karchierc $(kde_confdir)
	$(mkinstalldirs) $(APPSDIR)
	$(INSTALL_DATA) karchie.kdelnk $(APPSDIR)
#	$(mkinstalldirs) $(kde_icondir)
#	$(INSTALL_DATA) karchie.xpm $(kde_icondir)
#	$(mkinstalldirs) $(kde_minidir)
#	$(INSTALL_DATA) mini-karchie.xpm $(kde_minidir)/karchie.xpm

# remove ALL you have installed in install-data-local or install-exec-local
uninstall-local:
	-rm -f $(kde_confdir)/karchierc
	-rm -f $(APPSDIR)/karchie.kdelnk
#	-rm -f $(kde_icondir)/karchie.xpm
#	-rm -f $(kde_minidir)/karchie.xpm

# add a dependency for every moc file to be full portable

KAMenu.cpp: KAMenu.moc
KAQuery.cpp: KAQuery.moc
KAQueryFile.cpp: KAQueryFile.moc
KAQueryList.cpp: KAQueryList.moc
KASettings.cpp: KASettings.moc
KAStatusBar.cpp: KAStatusBar.moc
KAView.cpp: KAView.moc
KArchie.cpp: KArchie.moc   

SUFFIXES = .h .moc

.h.moc: 
	$(MOC) $< -o $@
