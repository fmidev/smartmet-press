%define BINNAME press
Summary: press binary
Name: smartmet-%{BINNAME}
Version: 11.11.8
Release: 1.el6.fmi
License: FMI
Group: Development/Tools
URL: http://www.weatherproof.fi
Source0: %{name}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot-%(%{__id_u} -n)
BuildRequires: libsmartmet-newbase >= 11.11.8-1
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
* Tue Nov  8 2011 mheiskan <mika.heiskanen@fmi.fi> - 11.11.8-1.el5.fmi
- Translations to English by Lasse
* Wed Oct 19 2011 oksman <santeri.oksman@fmi.fi> - 11.10.19-1.el5.fmi
- Added support for selecting encoding.
- Get rid of the Muut subdirectory in Linux.
* Tue Oct 11 2011 oksman <santeri.oksman@fmi.fi> - 11.10.11-1.el5.fmi
- Changes to support English language by Lars Winberg.
* Tue Jan 11 2011 mheiskan <mika.heiskanen@fmi.fi> - 11.1.11-1.el5.fmi
- Upgraded to newbase 11.1.11 to avoid reading rsync files
* Mon Jul  5 2010 mheiskan <mika.heiskanen@fmi.fi> - 10.7.5-1.el5.fmi
- Upgrade to newbase 10.7.5-2
* Mon Jun  7 2010 mheiskan <mika.heiskanen@fmi.fi> - 10.6.7-1.el5.fmi
- Remove duplicate images from eps
* Tue May 11 2010 mheiskan <mika.heiskanen@fmi.fi> - 10.5.11-1.el5.fmi
- Hyphenation improvements by Lars Winberg
* Fri May  7 2010 mheiskan <mika.heiskanen@fmi.fi> - 10.5.7-1.el5.fmi
- Missing text file is now fatal
* Thu Mar 18 2010 mheiskan <mika.heiskanen@fmi.fi> - 10.3.18-1.el5.fmi
- Missing forecast data is no longer fatal (happened for wave height data)
* Wed Mar 17 2010 mheiskan <mika.heiskanen@fmi.fi> - 10.3.17-1.el5.fmi
- Missing segment time is no longer a fatal error
* Mon Mar 15 2010 mheiskan <mika.heiskanen@fmi.fi> - 10.3.14-1.el5.fmi
- Added moon phase and easier symbol definitions
* Wed Feb 10 2010 mheiskan <mika.heiskanen@fmi.fi> - 10.2.10-1.el5.fmi
- Improved error handling
* Fri Jan 15 2010 mheiskan <mika.heiskanen@fmi.fi> - 10.1.15-1.el5.fmi
- Compiled with latest newbase
* Mon Dec 14 2009 mheiskan <mika.heiskanen@fmi.fi> - 9.12.14-1.el5.fmi
- Improved error handling
- Added printout of command line options when no arguments are given
* Thu Oct 29 2009 mheiskan <mika.heiskanen@fmi.fi> - 9.10.29-1.el5.fmi
- Improvements required for the new Helsingin Sanomat layout
* Mon Oct 19 2009 oksman <santeri.oksman@fmi.fi> - 9.10.19-1.el5.fmi
- Fixed a bug which crashed qdpress while using manager files. Also improvements to code by Mika Heiskanen.
* Wed Sep 16 2009 mheiskan <mika.heiskanen@fmi.fi> - 9.9.16-1.el5.fmi
- Various improvements to hyphenation etc
* Thu Apr  9 2009 oksman <santeri.oksman@fmi.fi> - 9.4.9-2.el5.fmi
- Bug fix to former zero values bug fix.
* Thu Apr  9 2009 oksman <santeri.oksman@fmi.fi> - 9.4.9-1.el5.fmi
- Fixed bug with zero values and added variable press::managerspath. Also fixes to Makefile.
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

