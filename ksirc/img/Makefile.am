# List of files to install
data_DATA = \
server.xpm channel.xpm channels.xpm ksirc_a.xpm ksirc_b.xpm ksirc_dock.xpm\
arrow.gif         greenpin.gif      madsmiley.gif     smiley.gif \
blueball.gif      info.gif          star.gif \
bluepin.gif       info1.gif         \
mini-run.gif      ksirc.gif

# Directory where to install them
datadir = $(kde_datadir)/ksirc/icons

# Special case for ksirc.gif
install-data-local:
	$(INSTALL_DATA) ksirc.gif $(kde_icondir)

# Removes ksirc.gif. The rest is automatic
uninstall-local:
	-rm -f $(kde_icondir)/ksirc.gif

