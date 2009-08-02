#!/bin/bash

# Directories definitions
dataDir=/usr/share/games/
binDir=/usr/games/bin/
fakeroot=./fakeroot
debdir=./packages/deb

## Define Package Name and Version
version=$(grep Version control | sed 's/Version:[ ]*//g')
dnt=$(grep Package control | sed 's/Package:[ ]*//g')
arch="i386"
debian="lenny"

package=${dnt}-${version}_${arch}_${debian}.deb
printf "Package: ${package}\n"

# Clean the Fake Root
rm -rf ${fakeroot}
mkdir -p ${fakeroot}

( 
  cd ../../
  printf "Configuring...\n"
  set -e $(./configure --datarootdir=${dataDir} --bindir=${binDir})
  printf "Cleaning...\n"
  set -e $(make clean) 
  printf "Compiling...\n"
  set -e $(make -j3)

  printf "Installing data...\n"
  set -e $(./install.sh ${debdir}/${fakeroot}/${dataDir} ${dnt} install)
  set -e $(cp ./bin/dcc.opc ${debdir}/${fakeroot}/${dataDir}/${dnt} )
  set -e $(cp ./bin/options.cfg ${debdir}/${fakeroot}/${dataDir}/${dnt} )

  printf "Installing binaries...\n"
)

