## -*- makefile -*-
# Ktalkdlg - Makefile.am

EXTRA_DIST	 =	

if KDE_INSTALLED

bin_PROGRAMS	 =	ktalkdlg
ktalkdlg_SOURCES =	ktalkdlg.cpp

INCLUDES	=	$(all_includes)
LDFLAGS		=	$(all_libraries) $(KDE_RPATH)
LDADD		=	-lkdeui -lkdecore -lqt -lXext -lX11 -lmediatool $(LIBCOMPAT)

endif

#for extra warnings during compilation :
#DEFS		+=	-ansi -pedantic -D_POSIX_SOURCE -D_BSD_SOURCE

messages:
	$(XGETTEXT) -C -ki18n -x $(includedir)/kde.pot $(ktalkdlg_SOURCES) && mv messages.po ../../po/ktalkd.pot

