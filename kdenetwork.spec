%define version Beta3
%define name kdenetwork
Name: %{name}
Summary: K Desktop Environment - Network Applications
Version: %{version}
Release: 1 
Source: ftp.kde.org:/pub/kde/unstable/CVS/snapshots/cuurent/%{name}-%{version}.tar.gz
Patch: kdenetwork.patch
Group: X11/KDE/Networking
BuildRoot: /tmp/realhot_%{name}
Copyright: GPL
Requires: qt >= 1.31
Packager: Magnus Pfeffer <pfeffer@unix-ag.uni-kl.de>
Distribution: KDE
Vendor: The KDE Team

%description
Network applications for the K Desktop Environment.

Included with this package are:

kmail: mail reader
knu: network utilities
korn: mailbox monitor tool
kppp: easy PPP connection configuration
krn: news reader

%prep
rm -rf $RPM_BUILD_ROOT

%setup -n kdenetwork

%patch -p1

%build
export KDEDIR=/opt/kde
./configure --prefix=$KDEDIR --with-install-root=$RPM_BUILD_ROOT
make 

%install
make install

cd $RPM_BUILD_ROOT
find . -type d | sed '1,2d;s,^\.,\%attr(-\,root\,root) \%dir ,' > $RPM_BUILD_DIR/file.list.%{name}
find . -type f | sed 's,^\.,\%attr(-\,root\,root) ,' >> $RPM_BUILD_DIR/file.list.%{name}
find . -type l | sed 's,^\.,\%attr(-\,root\,root) ,' >> $RPM_BUILD_DIR/file.list.%{name}

%clean
rm -rf $RPM_BUILD_ROOT

%files -f ../file.list.%{name}
