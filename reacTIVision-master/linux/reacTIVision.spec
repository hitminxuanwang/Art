%define version 1.6
%define release 1

Summary:      tangible interaction framework
Name:         reacTIVision
Version:      %{version}
Release:      %{release}
License:      GPL
Group:        Applications/Multimedia
URL:          http://reactivision.sourceforge.net/
Source0:      %{name}-%{version}.tar.gz
BuildRoot:    %{_tmppath}/%{name}-%{version}-buildroot
Packager:     Martin Kaltenbrunner

Requires: SDL libdc1394
BuildRequires: gcc
BuildRequires: gcc-c++
BuildRequires: SDL2-devel >= 2.0
BuildRequires: libdc1394-devel >= 2.0
BuildRequires: turbojpeg-devel >= 1.3
Requires: SDL2 >= 2.0
Requires: libdc1394 >= 2.0
Requires: turbojpeg >= 1.3

%description
reacTIVision is an open source, cross-platform computer vision
framework for the fast and robust tracking of fiducial markers
attached onto physical objects, as well as for multi-touch
finger tracking. It was mainly designed as a toolkit for the 
rapid development of table-based tangible user interfaces (TUI)
and multi-touch interactive surfaces. This framework has been 
developed by Martin Kaltenbrunner and Ross Bencina at the Music 
Technology Group at the Universitat Pompeu Fabra in Barcelona, Spain
as part of the the Reactable project, a novel electronic
musical instrument with a table-top tangible user interface.

%prep
%setup -q -n %{name}-%{version}

%build
cd linux
make -j4

%install
rm -rf $RPM_BUILD_ROOT
cd linux
make DESTDIR=$RPM_BUILD_ROOT BINDIR=%{_bindir} PREFIX=%{_prefix} install

%clean
rm -rf %{buildroot}

%files
%{_bindir}/%{name}
%{_prefix}/share/%{name}/*.txt
%{_prefix}/share/%{name}/*.xml
%{_prefix}/share/%{name}/symbols/*.pdf
%{_prefix}/share/%{name}/calibration/*.pdf
%{_prefix}/share/applications/%{name}.desktop
%{_prefix}/share/pixmaps/%{name}.png
%{_prefix}/share/icons/hicolor/128x128/apps/%{name}.png
%{_prefix}/share/icons/hicolor/scalable/apps/%{name}.svg
