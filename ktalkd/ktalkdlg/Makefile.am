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

# make messages.po. Move this one to ../../po/ and "make merge" in po
messages:
	$(XGETTEXT) -C -ki18n -x$(includedir)/kde.pot $(ktalkdlg_SOURCES) && mv messages.po ../../po/ktalkd.pot

