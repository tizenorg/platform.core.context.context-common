Name:       context-common
Summary:    Context-Service Shared Library
Version:    0.8.1
Release:    1
Group:      Service/Context
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz

%define BUILD_PROFILE %{?profile}%{!?profile:%{?tizen_profile_name}}

%define LEGACY_FILE_PATH		0
%define LEGACY_PERIODIC_ALARM	0

%if "%{?BUILD_PROFILE}" == "tv"
ExcludeArch: %{arm} aarch64 %ix86 x86_64
%endif

BuildRequires: cmake
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(gio-2.0)
BuildRequires: pkgconfig(json-glib-1.0)
BuildRequires: pkgconfig(sqlite3)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(capi-base-common)
BuildRequires: pkgconfig(alarm-service)

%if ! %{LEGACY_FILE_PATH}
BuildRequires: pkgconfig(libtzplatform-config)
%endif

%description
Context-Service Shared Library

%prep
%setup -q

%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`

export   CFLAGS+=" -Wextra -Wcast-align -Wshadow -Wwrite-strings -Wswitch-default -Wno-unused-parameter"
export CXXFLAGS+=" -Wextra -Wcast-align -Wshadow -Wwrite-strings -Wswitch-default -Wno-unused-parameter"

export   CFLAGS+=" -Wno-empty-body -fomit-frame-pointer -fno-optimize-sibling-calls"
export CXXFLAGS+=" -Wno-empty-body -fomit-frame-pointer -fno-optimize-sibling-calls"

export   CFLAGS+=" -fno-strict-aliasing -fno-unroll-loops -fsigned-char -fstrict-overflow"
export CXXFLAGS+=" -fno-strict-aliasing -fno-unroll-loops -fsigned-char -fstrict-overflow"

export   CFLAGS+=" -fno-common"
export CXXFLAGS+=" -Wnon-virtual-dtor"
export CXXFLAGS+=" -std=c++0x"

#export   CFLAGS+=" -Wcast-qual"
#export CXXFLAGS+=" -Wcast-qual"

#export   CFLAGS+=" -DTIZEN_ENGINEER_MODE"
#export CXXFLAGS+=" -DTIZEN_ENGINEER_MODE"
#export   FFLAGS+=" -DTIZEN_ENGINEER_MODE"

cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix} -DMAJORVER=${MAJORVER} -DFULLVER=%{version} \
							   -DLEGACY_FILE_PATH=%{LEGACY_FILE_PATH} \
							   -DLEGACY_PERIODIC_ALARM=%{LEGACY_PERIODIC_ALARM}
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}/usr/share/license
cp LICENSE %{buildroot}/usr/share/license/%{name}

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%files
%manifest packaging/%{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/*.so*
/usr/share/license/%{name}

%package devel
Summary:    Context-Service Shared Library (Development)
Group:      System/Libraries
Requires:	%{name} = %{version}-%{release}

%description devel
Context-Service Shared Library (DEV)

%files devel
%defattr(-,root,root,-)
%{_includedir}/context-service/internal/*.h
%{_libdir}/pkgconfig/*.pc
