## -*- makefile -*-
# Ktalkd - Main Makefile.am

if KDE_INSTALLED
SUBDIRS 	=	ktalkd doc kcmktalkd ktalkdlg
else
SUBDIRS 	=	ktalkd doc 
endif

messages:
	@for subdir in $(SUBDIRS); do \
	echo "Making messages in $$subdir"; \
	 (cd $$subdir && $(MAKE) messages) \
	done


install-data-local:
	@echo "**************************************************************************"
	@echo
	@echo "Don't forget to change the line concerning in.ntalkd in /etc/inetd.conf."
	@echo
	@echo "For example, on a linux system, if kde is in /opt/kde, set it to :"
	@echo "ntalk	dgram	udp	wait	root	/usr/sbin/tcpd	/opt/kde/bin/ktalkd"
	@echo	
	@echo "Alternatively, you can use the script post-install.sh, to do the job"
	@echo
	@echo "Anyway, you'll have to restart inetd after this."
	@echo "On a Redhat 5 system, do a  '/etc/rc.d/init.d/inet restart'"
	@echo
	@echo "**************************************************************************"

