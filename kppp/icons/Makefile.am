EXTRA_DIST = kppp.xpm mini-kppp.xpm

# just install datas here. Use install-exec-data for scripts and etc.
# the binary itself is already installed from automake
# use mkinstalldirs, not "install -d"
# don't install a list of file. Just one file per install.
# if you have more of them, create a subdirectory with an extra Makefile
install-data-local:
	$(mkinstalldirs) $(kde_icondir)
	$(INSTALL_DATA) kppp.xpm $(kde_icondir)
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-kppp.xpm $(kde_minidir)/kppp.xpm

