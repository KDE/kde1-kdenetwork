# List of files to install
data_DATA = \
arrow.gif         greenpin.gif      madsmiley.gif     smiley.gif \
blueball.gif      info.gif          mini-display.gif  star.gif \
bluepin.gif       info1.gif         mini-edit.gif \
mini-run.gif      gf.gif            ppl.gif           ksirc.gif

# Directory where to install them
datadir = $(kde_datadir)/ksirc/icons

# Special case for ksirc.gif
install-data-local:
	$(INSTALL_DATA) ksirc.gif $(kde_icondir)

# Removes ksirc.gif. The rest is automatic
uninstall-local:
	-rm -f $(kde_icondir)/ksirc.gif

