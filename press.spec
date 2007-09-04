Summary: press library
Name: smartmet_press
Version: 1.0.1
Release: 1
License: FMI
Group: Development/Libraries
URL: http://www.weatherproof.fi
Source0: %{name}.tar.gz
BuildRoot: %{_tmppath}/%{name}
Requires: newbase >= 1.0.1-1
Provides: press

%description
FMI press library

%prep
rm -rf $RPM_BUILD_ROOT
mkdir $RPM_BUILD_ROOT

%setup -q -n %{name}
 
%build
make clean
make depend
make %{_smp_mflags} 

%install
make install PREFIX="${RPM_BUILD_ROOT}%{_usr}"

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,www,www,0775)
%{_includedir}/press
%{_libdir}/libpress.a


%changelog
* Thu Jun  7 2007 tervo <tervo@xodin.weatherproof.fi> - 
- Initial build.

