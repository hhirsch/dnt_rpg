Summary: a sarcastic 3D single player RPG
Name: dccnitghtmare
Version: 0.2
Release: 0
License: Public Domain
Group: System/Monitoring
Source: %name-%version.tar.bz2 
BuildRoot: /var/tmp/%{name}-buildroot
Vendor: DNTeam
Packager: Guilherme Farrer <farrer@dnteam.org>
Distribution: 
Requires: SDL_image SDL_ttf openal freeglut libvorbis libogg Mesa >= SDL-1.2.11

%description
a sarcastic 3D single player RPG

%prep
%setup -q

%build

%configure --prefix=/usr/
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
rm -rf $RPM_BUILD_ROOT

# create directories
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/usr/share/%name

# install bin dirs
make install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)

/usr/bin/%name
/usr/share/%name

%post

%postun

%changelog
* Thu Jul 7 2008 Guilherme Farrer <farrer@dnteam.org>
- Created the RPM spec

