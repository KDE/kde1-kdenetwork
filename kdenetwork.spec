%define version @VERSION@ 
%define name kdenetwork 
Name: %{name} Summary: K Desktop Environment - Network Applications
Version: %{version}
Release: 1
Source: ftp.kde.org:/pub/kde/stable/Beta4/distribution/rpm/source/%{name}-%{version}-1.src.tar.gz
Group: X11/KDE/Networking
BuildRoot: /tmp/realhot_%{name}
Copyright: GPL
Requires: qt >= 1.33
Packager: Preston Brown <pbrown@kde.org>
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
