# just set the variable
APPSDIR = $(kde_appsdir)/Internet
# set the include path for X, qt and KDE
INCLUDES= $(all_includes)
# claim, which subdirectories you want to install
SUBDIRS = doc pics

DEFS = @DEFS@ -I. -I$(srcdir) -I.. -DDO_TRACE
####### This part is very kbiff specific
# you can add here more. This one gets installed 
bin_PROGRAMS = 	kbiff

kbiff_METASOURCES =  kbiff.moc.cpp kbiffmonitor.moc.cpp notify.moc.cpp setupdlg.moc.cpp 

# Which sources should be compiled for kbiff.
#kbiff_SOURCES	= setupdlg.cpp kbiff.cpp main.cpp kbiffmonitor.cpp notify.cpp Trace.cpp ktreeview.cpp convert.cpp kwraplabel.cpp $(kbiff_METASOURCES)
kbiff_SOURCES	= setupdlg.cpp kbiff.cpp main.cpp kbiffmonitor.cpp notify.cpp Trace.cpp ktreeview.cpp $(kbiff_METASOURCES)

# the library search path
kbiff_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# the libraries to link against. Be aware of the order. First the libraries,
# that depend on the following ones.
kbiff_LDADD   = -lkfile -lkfm -lkdeui -lkdecore -lmediatool -lqt -lX11 -lXext $(LIBSOCKET)

# this option you can leave out. Just, if you use "make dist", you need it
#noinst_HEADERS = setupdlg.h kbiff.h kbiffmonitor.h notify.h Trace.h ktreeview.h convert.h kwraplabel.h
noinst_HEADERS = setupdlg.h kbiff.h kbiffmonitor.h notify.h Trace.h ktreeview.h

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(kbiff_METASOURCES)

messages:
	$(XGETTEXT) -C -ki18n -x $(includedir)/kde.pot $(kbiff_SOURCES) && mv messages.po ../po/kbiff.pot

# just install datas here. Use install-exec-data for scripts and etc.
# the binary itself is already installed from automake
# use mkinstalldirs, not "install -d"
# don't install a list of file. Just one file per install.
# if you have more of them, create a subdirectory with an extra Makefile 
install-data-local: 
	$(mkinstalldirs) $(APPSDIR)
	$(INSTALL_DATA) kbiff.kdelnk $(APPSDIR)
	$(mkinstalldirs) $(kde_icondir)
	$(INSTALL_DATA) kbiff.xpm $(kde_icondir)
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-kbiff.xpm $(kde_minidir)/kbiff.xpm

# remove ALL you have installed in install-data-local or install-exec-local
uninstall-local:
	-rm -f $(APPSDIR)/kbiff.kdelnk
	-rm -f $(kde_icondir)/kbiff.xpm
	-rm -f $(kde_minidir)/kbiff.xpm

ktreeview.o : ktreeview.moc
ktreeview.moc : ktreeview.h
	$(MOC) ktreeview.h -o ktreeview.moc

kbiff.moc.cpp : kbiff.h
	$(MOC) kbiff.h -o kbiff.moc.cpp
	
kbiffmonitor.moc.cpp : kbiffmonitor.h
	$(MOC) kbiffmonitor.h -o kbiffmonitor.moc.cpp

notify.moc.cpp : notify.h
	$(MOC) notify.h -o notify.moc.cpp

setupdlg.moc.cpp : setupdlg.h
	$(MOC) setupdlg.h -o setupdlg.moc.cpp
