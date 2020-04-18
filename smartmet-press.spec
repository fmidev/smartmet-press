%define BINNAME press
%define RPMNAME smartmet-%{BINNAME}
Summary: Press Weather Chart Renderer
Name: %{RPMNAME}
Version: 20.4.18
Release: 1%{?dist}.fmi
License: FMI
Group: Development/Tools
URL: https://github.com/fmidev/smartmet-press
Source0: %{name}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot-%(%{__id_u} -n)
BuildRequires: rpm-build
BuildRequires: gcc-c++
BuildRequires: make
BuildRequires: smartmet-library-newbase-devel >= 20.4.18
BuildRequires: boost169-devel
Requires: smartmet-library-newbase >= 20.4.18
Requires: boost169-iostreams
Requires: boost169-filesystem
Requires: boost169-system
Provides: qdpress

%description
Press Weather Chart production system

%prep
rm -rf $RPM_BUILD_ROOT

%setup -q -n %{RPMNAME}
 
%build
make %{_smp_mflags}

%install
%makeinstall

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(0775,root,root,-)
%{_bindir}/qdpress

%changelog
* Sat Apr 18 2020 Mika Heiskanen <mika.heiskanen@fmi.fi> - 20.4.18-1.fmi
- Upgrade to Boost 1.69

* Wed Nov 20 2019 Mika Heiskanen <mika.heiskanen@fmi.fi> - 19.11.20-1.fmi
- Repackaged due to newbase API changes

* Thu Oct 31 2019 Mika Heiskanen <mika.heiskanen@fmi.fi> - 19.10.31-1.fmi
- Rebuild due to newbase API/ABI changes

* Mon Sep 30 2019 Mika Heiskanen <mika.heiskanen@fmi.fi> - 19.9.30-1.fmi
- Repackaged from master instead of WGS84

* Fri Sep 27 2019 Mika Heiskanen <mika.heiskanen@fmi.fi> - 19.9.27-1.fmi
- Repackaged due to ABI changes in newbase

* Wed Aug 14 2019 Teemu Sirviö <teemu.sirvio@fmi.fi> - 19.8.14-1.fmi
- Allow adding text content from timestamped files

* Tue Jul 16 2019 Teemu Sirviö <teemu.sirvio@fmi.fi> - 19.7.16-1.fmi
- Include FileWithTimeStamp-images from any given directory

* Wed May  2 2018 Mika Heiskanen <mika.heiskanen@fmi.fi> - 18.5.2-1.fmi
- Repackaged since newbase NFmiEnumConverter ABI changed

* Sat Apr  7 2018 Mika Heiskanen <mika.heiskanen@fmi.fi> - 18.4.7-1.fmi
- Upgrade to boost 1.66

* Tue Jan 23 2018 Teemu Sirviö <teemu.sirvio@fmi.fi> - 18.1.23-1.fmi
- Allow #FixedSymbol to use subdirectory in SymbolSet value

* Mon Aug 28 2017 Mika Heiskanen <mika.heiskanen@fmi.fi> - 17.8.28-1.fmi
- Upgrade to boost 1.65

* Wed Apr  5 2017 Teemu Sirviö <teemu.sirvio@fmi.fi> - 17.4.5-1.fmi
- Allow relative paths to included eps-images

* Tue Apr  4 2017 Mika Heiskanen <mika.heiskanen@fmi.fi> - 17.4.4-1.fmi
- Fix to weather symbol multimapping

* Thu Mar 16 2017 Mika Heiskanen <mika.heiskanen@fmi.fi> - 17.3.16-1.fmi
- Fixed relative location values to be initialized to zero, otherwise locations could become random

* Thu Feb 23 2017 Mika Heiskanen <mika.heiskanen@fmi.fi> - 17.2.23-1.fmi
- Initialized several variables explicitly to avoid random behaviour

* Mon Feb 13 2017 Mika Heiskanen <mika.heiskanen@fmi.fi> - 17.2.13-1.fmi
- Recompiled due to newbase API changes

* Fri Jan 27 2017 Mika Heiskanen <mika.heiskanen@fmi.fi> - 17.1.27-1.fmi
- Recompiled due to NFmiQueryData object size change

* Wed Jan 11 2017 Mika Heiskanen <mika.heiskanen@fmi.fi> - 17.1.10-1.fmi
- Switched to FMI open source naming conventions

* Sun Jan 17 2016 Mika Heiskanen <mika.heiskanen@fmi.fi> - 16.1.17-1.fmi
- newbase API changed

* Thu Nov 26 2015 Mika Heiskanen <mika.heiskanen@fmi.fi> - 15.11.26-1.fmi
- Use memory mapped querydata for speed

* Wed Oct 15 2014 Mika Heiskanen <mika.heiskanen@fmi.fi> - 14.10.15-1.fmi
- Failing to write the preprocessed file to disk is now a fatal error

* Mon Aug  5 2013 mheiskan <mika.heiskanen@fmi.fi> - 13.8.5-1.fmi
- Do not show units in texts if the value is "-"

* Wed Jan 16 2013 mheiskan <mika.heiskanen@fmi.fi> - 13.1.16-1.fmi
- Recompiled due to a bug fix in newbase NFmiLocationFinder

* Mon Aug  6 2012 mheiskan <mika.heiskanen@fmi.fi> - 12.8.6-1.fmi
- Keyword translations added by Lars Winberg

* Fri Apr 13 2012 mheiskan <mika.heiskanen@fmi.fi> - 12.4.13-2.el6.fmi
- Recompiled with newbase to get latin1 month names and time phrases

* Fri Apr 13 2012 mheiskan <mika.heiskanen@fmi.fi> - 12.4.13-1.el6.fmi
- Added option -n to disable appending product name to output directory

* Thu Apr 12 2012 mheiskan <mika.heiskanen@fmi.fi> - 12.4.12-1.el5.fmi
- Newbase fixed Swedish weekday names encoding

* Tue Apr 10 2012 mheiskan <mika.heiskanen@fmi.fi> - 12.4.10-1.el5.fmi
- Fixed encoding setting to be global

* Mon Feb 27 2012 oksman <santeri.oksman@fmi.fi> - 12.2.27-2.el6.fmi
- Some log messages translated to English.

* Mon Feb 27 2012 oksman <santeri.oksman@fmi.fi> - 12.2.27-1.el6.fmi
- New RPM for RHEL6. Contains fix for time zones in Latvia. Removed -DBOOST_DISABLE_THREADS flag from Makefile.

* Fri Feb 17 2012 oksman <santeri.oksman@fmi.fi> - 12.2.17-1.el6.fmi
- New RPM for RHEL6. Contains fixes made by Lasse.

* Tue Dec 13 2011 oksman <santeri.oksman@fmi.fi> - 11.12.13-2.el5.fmi
- Still a couple of fixes to encodings

* Tue Dec 13 2011 oksman <santeri.oksman@fmi.fi> - 11.12.13-1.el5.fmi
- Lots of changes by Lasse.
- Now all texts should comply the given encoding. Default is still latin(1).

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

