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
DEFS = @DEFS@ -I. -I../.. -include ../kplunger/kplunger.h
# claim, which subdirectories you want to install

#DEFS = @DEFS@ -I. -I$(srcdir) -I.. -DDOTRACE
####### This part is very pws specific
# you can add here more. This one gets installed 
noinst_LIBRARIES = libksopenksirc.a

# Which sources should be compiled for dialog

libksopenksirc_a_SOURCES = \
	open_ksirc.cpp\
	mopen_ksirc.cpp\
	open_ksircData.cpp\
	mopen_ksircData.cpp\
	serverFileParser.cpp\
        enter_combo.cpp\
        menter_combo.cpp
        
# this option you can leave out. Just, if you use "make dist", you need it
noinst_HEADERS = open_ksirc.h open_ksircData.h serverDataType.h serverFileParser.h enter_combo.h

# just to make sure, automake makes them 
BUILTSOURCES = mopen_ksirc.cpp mopen_ksircData.cpp menter_combo.cpp

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(BUILTSOURCES)

# make messages.po. Move this one to ../po/ and "make merge" in po
messages:
	$(XGETTEXT) -C -ki18n $(pws_SOURCES)

# just install datas here. Use install-exec-data for scripts and etc.
# the binary itself is already installed from automake
# use mkinstalldirs, not "install -d"
# don't install a list of file. Just one file per install.
# if you have more of them, create a subdirectory with an extra Makefile 
install-data-local:
	$(mkinstalldirs) $(kde_datadir)/ksirc
	$(INSTALL_DATA) servers.txt $(kde_datadir)/ksirc
	$(INSTALL_DATA) servers.ini $(kde_datadir)/ksirc


# remove ALL you have installed in install-data-local or install-exec-local
uninstall-local:
	-rm -f $(kde_datadir)/ksirc/servers.txt
	-rm -f $(kde_datadir)/ksirc/servers.ini

# add a dependency for every moc file to be full portable
m%.cpp: %.h
	$(MOC) $< -o $@
