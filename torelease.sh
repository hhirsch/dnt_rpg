#!/bin/sh

#  DNT: a satirical post-apocalyptical RPG.
#  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
# 
#  This file is part of DNT.
# 
#  DNT is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  DNT is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with DNT.  If not, see <http://www.gnu.org/licenses/>.


# torelease.sh will create a dnt tar.bz2 with the needed files
# for a source release.

# Usage: torelease.sh [file.tar.bz2]
# if no file defined, will create as $project.tar.bz2

tarFile=$1
project="dnt"
workDirs="bin data po src syntax macosx windows"
homeFiles="aclocal.m4 configure configure.in Doxyfile.in \
           install.sh Makefile.in COPYING README \
           torelease.sh config.sub config.guess"

# Verify if file was defined
if [[ -z $tarFile ]]; then
   tarFile="$project.tar.bz2"
fi
echo "File Defined as: $tarFile"

# Clean the build tree
echo "Calling 'make clean'"
make clean > /dev/null

# Create directory dccnitghtmare
echo "Creating Directory"
rm -rf $project
mkdir -p $project
mkdir -p $project/doc/codeDocs

echo "Copying Needed Files"
# First copy the ./ files
cp $homeFiles $project

# Copy all needed files and directories to the created "$project"
for directory in $workDirs; do
   for curDir in $(find $directory -type d);do
      # Ignore CVS directories
      if [[ -z $(echo $curDir | grep CVS) ]]; then
         # Create the directory
         mkdir -p $project/$curDir

         # And copy all of its files
         for f in $curDir/*;do
            if [[ -f $f ]]; then
               if [ "$f" != "$curDir/*" ]; then
                  if [ "$f" != "$curDir/Makefile" ]; then
                     cp $f $project/$f
                  fi
               fi
            fi
         done
      fi
   done
done


# Finally, create the desired .tar.bz2
echo "Creating $tarFile"
tar -cjf $tarFile $project

# Tell it's done!
echo "Done!"

