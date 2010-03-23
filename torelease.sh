#!/bin/sh

# torelease.sh will create a dccnitghtmare tar.bz2 with the needed files
# for a source release.

# Usage: torelease.sh [file.tar.bz2]
# if no file defined, will create as $project.tar.bz2

tarFile=$1
project="dccnitghtmare"
workDirs="bin data po src syntax"
homeFiles="aclocal.m4 configure configure.in DccNiTghtmare.dev \
           DNTMapEditor.dev Doxyfile.in \
           install.sh Makefile.in COPYING README \
           systems.txt torelease.sh config.sub config.guess"

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

