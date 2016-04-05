Name:       phonenumber-utils
Summary:    Phone Number Utilities
Version:    0.1.9
Release:    0
Group:      Telephony/Utilities
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1:    %{name}.service
Source1001: %{name}.manifest
Source1002: %{name}-test.manifest
BuildRequires: cmake
BuildRequires: gettext-devel
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(icu-i18n)
BuildRequires: pkgconfig(protobuf)
BuildRequires: pkgconfig(capi-base-common)
BuildRequires: pkgconfig(capi-system-system-settings)
BuildRequires: pkgconfig(tapi)
BuildRequires: pkgconfig(capi-system-info)
BuildRequires: libphonenumber-devel

%define _unitdir /usr/lib/systemd/system
%define _dbus_name org.tizen.PhonenumberUtils.dbus

%description
Phone Number Utilities(location, formatted number, normalized number)

%package devel
Summary:    Phone Number Utilities (devel)
Group:      Telephony/Development
Requires:   %{name} = %{version}-%{release}

%description devel
Phone Number Utilities development kit

%package test
Summary:    Phone Number Utilities tester
Group:      Telephony/Utilities
Requires:   %{name} = %{version}

%description test
Tizen Phone Number Utilities tester

%prep
%setup -q
chmod g-w %_sourcedir/*
cp %{SOURCE1001} .
cp %{SOURCE1002} .

%build
%if 0%{?tizen_build_binary_release_type_eng}
export CFLAGS="$CFLAGS -DTIZEN_DEBUG_ENABLE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_DEBUG_ENABLE"
export FFLAGS="$FFLAGS -DTIZEN_DEBUG_ENABLE"
%endif

MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%cmake . -DMAJORVER=${MAJORVER} -DFULLVER=%{version} -DBIN_INSTALL_DIR:PATH=%{_bindir}


%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}%{_unitdir}/multi-user.target.wants
install -m 0644 %{SOURCE1} %{buildroot}%{_unitdir}/%{name}.service


%post
/sbin/ldconfig
systemctl daemon-reload
if [ $1 == 1 ]; then
    systemctl restart %{name}.service
fi

chown :5000 -R /opt/usr/data/%{name}
chmod 775 -R /opt/usr/data/%{name}

%postun
/sbin/ldconfig
if [ $1 == 0 ]; then
    systemctl stop %{name}.service
fi
systemctl daemon-reload


%files
%defattr(-,root,root,-)
%manifest %{name}.manifest
%{_unitdir}/%{name}.service
/opt/usr/data/%{name}
%{_bindir}/%{name}-daemon
%{_datadir}/dbus-1/system-services/%{_dbus_name}.service
%config %{_sysconfdir}/dbus-1/system.d/%{_dbus_name}.conf
%{_libdir}/lib%{name}.so.*
%license LICENSE.APLv2


%files devel
%defattr(-,root,root,-)
%{_libdir}/lib%{name}.so
%{_libdir}/pkgconfig/%{name}.pc
%{_includedir}/%{name}/*.h


%files test
%defattr(-,root,root,-)
%manifest %{name}-test.manifest
%{_bindir}/%{name}-test
%license LICENSE.APLv2

