## kdebase/Makefile.am
## (C) 1997 Stephan Kulow

AUTOMAKE_OPTIONS = foreign 
SUBDIRS = @TOPSUBDIRS@

install-data-local:
	@echo ""
	@echo ""
	@echo ""
	@echo "*************** Important *************************"
	@echo ""
	@echo " set KDEDIR to "$(prefix)
	@echo " add "$(libdir)" to your LD_LIBRARY_PATH"
	@echo " add "$(bindir)" to your PATH"
	@echo ""
	@echo " Please report bugs with our web form at"
	@echo " http://buglist.kde.org."
	@echo ""
	@echo " Fun with the KDE wishes you Stephan Kulow"
	@echo " <coolo@kde.org> and all the KDE developers."
	@echo "****************************************************"
	@echo ""

messages:
	@for subdir in $(SUBDIRS); do \
	echo "Making messages in $$subdir"; \
	  (cd $$subdir && $(MAKE) messages) \
	done 
	(cd po && $(MAKE) merge)
