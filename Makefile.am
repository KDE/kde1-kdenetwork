## kdebase/Makefile.am
## (C) 1997 Stephan Kulow

AUTOMAKE_OPTIONS = foreign 
SUBDIRS = @TOPSUBDIRS@
DISTCLEANFILES = inst-apps 

install-data-local:
	@echo ""
	@echo ""
	@echo ""
	@echo "*************** Important *************************"
	@echo ""
	@echo " add "$(bindir)" to your PATH"
	@echo ""
	@echo " Please report bugs with our web form at"
	@echo " http://buglist.kde.org."
	@echo ""
	@echo " Stephan Kulow <coolo@kde.org> and all the KDE"
	@echo " developers wish you fun with the KDE."                     
	@echo "****************************************************"
	@echo ""


messages:
	rm -rf po.backup
	mkdir po.backup
	for i in `ls -1 po/*.pot | sed -e "s#po/##"`; do \
	  grep -v "\"POT-Creation" po/$$i > po.backup/$$i ; \
	  cp po/$$i po.backup/backup_$$i ;  \
	done
	files=`ls -1`; \
	dirs=`for i in $$files; do if test -d $$i && test -r $$i/Makefile.am; then echo $$i; fi; done`; \
	for subdir in $$dirs; do \
	  echo "Making messages in $$subdir"; \
	  (cd $$subdir && $(MAKE) -f Makefile.am messages) \
	done
	for  i in `ls -1 po/*.pot | sed -e "s#po/##"`; do \
	  grep -v "POT-Creation-Date:" po/$$i > temp.pot ;\
	  if diff -q temp.pot po.backup/$$i; then \
	     echo "I'm restoring $$i" ;\
	     mv po.backup/backup_$$i po/$$i; \
	   else \
	     echo "will leave $$i"; \
	fi ;\
	done
	rm -f temp.pot
	rm -rf po.backup
	(cd po && $(MAKE) -f Makefile.am merge)

configure.in: configure.in.in subdirs
	@mfs=`find . -name Makefile.am -print | sed -e "s#\.am##" | sed -e "s#\./##" | sort -r`; \
	cp -f configure.in.in configure.in ;\
	for i in $$mfs; do \
	sed -e "s#AC_OUTPUT(#AC_OUTPUT($$i #" configure.in > configure.in.new \
	 && mv configure.in.new configure.in ;\
	done 

subdirs:
	@files=`ls -1 | sort`; \
	dirs=`for i in $$files; do if test -d $$i; then echo $$i; fi; done`; \
	rm -f SUBDIRS ;\
	for i in $$dirs; do \
	echo $$i >> ./SUBDIRS; \
	done
	@if test -r subdirs && diff subdirs SUBDIRS > /dev/null; then \
	  rm -f SUBDIRS.new; \
	fi
	@test -r SUBDIRS && mv SUBDIRS subdirs

acinclude.m4: admin/acinclude.m4.in admin/libtool.m4.in acincktalk.m4
	cat admin/acinclude.m4.in admin/libtool.m4.in acincktalk.m4 > acinclude.m4

MAINTAINERCLEANFILES = subdirs configure.in acinclude.m4 SUBDIRS

