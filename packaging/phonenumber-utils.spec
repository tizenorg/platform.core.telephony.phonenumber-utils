Name:       phonenumber-utils
Summary:    Phone Number Utilities
Version:    0.1.6
Release:    0
Group:      Telephony/Utilities
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1001: %{name}.manifest
BuildRequires: cmake
BuildRequires: gettext-devel
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(icu-i18n)
BuildRequires: pkgconfig(protobuf)
BuildRequires: pkgconfig(capi-base-common)
BuildRequires: libphonenumber-devel

%description
Phone Number Utilities(location, formatted number)

%package devel
Summary:    Phone Number Utilities (devel)
Group:      Telephony/Development
Requires:   %{name} = %{version}-%{release}

%description devel
Phone Number Utilities development kit

%prep
%setup -q
cp %{SOURCE1001} .

%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%cmake . -DMAJORVER=${MAJORVER} -DFULLVER=%{version}

%install
rm -rf %{buildroot}
%make_install

%post
/sbin/ldconfig

chown :5000 -R /opt/usr/data/%{name}
chmod 775 -R /opt/usr/data/%{name}

%postun -p /sbin/ldconfig

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/lib%{name}.so.*
/opt/usr/data/%{name}
%license LICENSE.APLv2

%files devel
%defattr(-,root,root,-)
%{_libdir}/lib%{name}.so
%{_libdir}/pkgconfig/%{name}.pc
%{_includedir}/%{name}/*.h
%license LICENSE.APLv2
