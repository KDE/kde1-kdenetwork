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
	$(mkinstalldirs) $(kde_datadir)/ksirc
	$(mkinstalldirs) $(kde_datadir)/ksirc/icons
	$(INSTALL_DATA) *.gif $(kde_datadir)/ksirc/icons
	$(INSTALL_DATA) ksirc.gif $(kde_icondir)

#
# remove ALL you have installed in install-data-local or install-exec-local
uninstall-local:
	-rm -f $(kde_datadir)/ksirc/icons/*.gif

#
# add a dependency for every moc file to be full portable
# I've added a key binding to emacs for this.
#m%.cpp: %.h
#	moc $< -o $@

# Removes ksirc.gif. The rest is automatic
uninstall-local:
	-rm -f $(kde_icondir)/ksirc.gif

