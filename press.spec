%define BINNAME press
Summary: press binary
Name: smartmet-%{BINNAME}
Version: 9.4.6
Release: 4.el5.fmi
License: FMI
Group: Development/Tools
URL: http://www.weatherproof.fi
Source0: %{name}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot-%(%{__id_u} -n)
BuildRequires: libsmartmet-newbase >= 9.4.6-1
Provides: qdpress

%description
FMI press binary

%prep
rm -rf $RPM_BUILD_ROOT

%setup -q -n %{BINNAME}
 
%build
make %{_smp_mflags}

%install
%makeinstall

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,0775)
%{_bindir}/qdpress

%changelog
* Mon Apr  6 2009 mheiskan <mika.heiskanen@fmi.fi> - 9.4.6-1.el5.fmi
- Fixed crash for karjalainen.pre
* Wed Apr  1 2009 oksman <santeri.oksman@fmi.fi> - 9.4.1-3.el5.fmi
- Fixed overflowing string bug.
* Wed Apr  1 2009 oksman <santeri.oksman@fmi.fi> - 9.4.1-2.el5.fmi
- Fixed a bug regarding directory detection.
* Wed Apr  1 2009 mheiskan <mika.heiskanen@fmi.fi> - 9.4.1-1.el5.fmi
- Bug fixes by Lars Winberg
* Wed Mar  4 2009 oksman <santeri.oksman@fmi.fi> - 9.3.4-1.el5.fmi
- Linked against libsmartmet-newbase-9.3.4-2
* Tue Feb 10 2009 oksman <santeri.oksman@fmi.fi> - 9.2.10-1.el5.fmi
- Merged Linux branch to HEAD
* Tue Jan 27 2009 oksman <santeri.oksman@fmi.fi> - 9.1.27-1.el5.fmi
- VakioSymboli directive works now also in Linux.
* Thu Jan 15 2009 oksman <santeri.oksman@fmi.fi> - 9.1.15-1.el5.fmi
- Fixed a bug in NFmiPressText, which caused duplication of last word.
* Thu Dec 11 2008 oksman <santeri.oksman@fmi.fi> - 8.12.11-1.el5.fmi
- Added -s option which shifts the date of the product.
* Mon Nov 24 2008 oksman <santeri.oksman@fmi.fi> - 8.11.24-1.el5.fmi
- Production ready release.
* Wed Sep  3 2008 oksman <santeri.oksman@fmi.fi> - 8.9.3-1.el5.fmi
- Test version for production servers.
* Wed Aug  6 2008 oksman <santeri.oksman@fmi.fi> - 8.8.6-1.el5.fmi
- New branch, makes a binary RPM instead of library.
* Tue Apr 15 2008 mheiskan <mika.heiskanen@fmi.fi> - 8.4.15-1.el5.fmi
- Linked with newbase 8.4.15-2
* Mon Sep 24 2007 mheiskan <mika.heiskanen@fmi.fi> - 1.0.1-4.el5.fmi
- Fixed "make depend".
* Fri Sep 14 2007 mheiskan <mika.heiskanen@fmi.fi> - 1.0.1-3.el5.fmi
- Added "make tag" feature.
* Thu Sep 13 2007 mheiskan <mika.heiskanen@fmi.fi> - 1.0.1-2.el5.fmi
- Improved make system.
* Thu Jun  7 2007 tervo <tervo@xodin.weatherproof.fi> - 
- Initial build.

