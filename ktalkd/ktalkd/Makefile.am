## -*- makefile -*-
# Ktalkd - Makefile.am

SUBDIRS 	=	machines

bin_PROGRAMS	=	ktalkd
EXTRA_HEADERS	=	readcfg++.h
INCLUDES	=	$(all_includes)
noinst_HEADERS	=	defs.h includ.h find_user.h announce.h print.h process.h readconf.h table.h threads.h
ktalkd_DEPS	=	machines/libmach.a

###########################################################
if KDE_INSTALLED

# Config file location
TALKD_CONF	=	$(kde_confdir)/ktalkdrc
TALKD_CONF_NAME	=	ktalkdrc
DEFS 		+=       -DHAVE_KDE
ktalkd_SOURCES	=	announce.c print.c process.c readcfg++.cpp table.c talkd.c find_user.c threads.c
LDADD           =       machines/libmach.a -lkdecore -lqt -lXext -lX11 @LIBBSD@ @LIBSOCKET@

###########################################################
else

# Config file location
TALKD_CONF 	=	/etc/talkd.conf
TALKD_CONF_NAME	=	talkd.conf
DEFS		+=	-DTALKD_CONF=\"$(TALKD_CONF)\"
ktalkd_SOURCES	=	announce.c print.c process.c readconf.c table.c talkd.c find_user.c threads.c
LDADD		=	machines/libmach.a @LIBBSD@ @LIBSOCKET@

endif
###########################################################

LDFLAGS         =       $(all_libraries) $(KDE_RPATH)

EXTRA_DIST	=	.talkdrc talkd.conf ktalkdrc

install-data-local:
if KDE_INSTALLED
	$(mkinstalldirs) $(kde_confdir)
	$(mkinstalldirs) $(kde_sounddir)
	$(INSTALL_DATA) ktalkd.wav $(kde_sounddir)
endif
	@echo "**************************************************************************"
	@echo
	@if [ -f $(TALKD_CONF) ]; then \
	  echo "Please check $(TALKD_CONF) to be up-to-date."; \
	else \
	  $(INSTALL_DATA) $(TALKD_CONF_NAME) $(install_root)/$(TALKD_CONF); \
	fi
