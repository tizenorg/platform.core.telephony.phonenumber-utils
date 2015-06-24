Name:       phonenumber-utils
Summary:    Phone Number Utilities
Version:    0.1.5
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
BuildRequires: pkgconfig(capi-system-system-settings)
BuildRequires: libphonenumber-devel

%description
Phone Number Utilities

%package devel
Summary:    Phone Number Utilities (devel)
Group:      Telephony/Development
Requires:   %{name} = %{version}-%{release}

%description -n phonenumber-utils-devel
Phone Number Utilities development kit

%prep
%setup -q
cp %{SOURCE1001} .

%build
export CFLAGS="$CFLAGS -DTIZEN_ENGINEER_MODE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_ENGINEER_MODE"
export FFLAGS="$FFLAGS -DTIZEN_ENGINEER_MODE"

MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%cmake . -DMAJORVER=${MAJORVER} -DFULLVER=%{version}

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}/usr/share/license
cp LICENSE.APLv2 %{buildroot}/usr/share/license/%{name}
cp LICENSE.APLv2 %{buildroot}/usr/share/license/%{name}-devel

%post -n %{name}
/sbin/ldconfig

chown :5000 -R /opt/usr/data/%{name}
chmod 775 -R /opt/usr/data/%{name}

%postun -p /sbin/ldconfig

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/lib%{name}.so.*
/opt/usr/data/%{name}
/opt/usr/data/%{name}/downloads
/usr/share/license/%{name}

%files devel
%defattr(-,root,root,-)
%{_libdir}/lib%{name}.so
%{_libdir}/pkgconfig/%{name}.pc
%{_includedir}/%{name}/*.h
/usr/share/license/%{name}-devel
