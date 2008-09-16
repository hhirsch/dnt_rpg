#!/bin/sh

# Usage install.sh datadir package installApplication
data=./data
datadir=$1
package=$2
instApp=$3

# Create destiny data directory
$instApp -d $datadir/$package

# Install All Data
for directory in $(find $data -type d ); do
  
   # Ignore CVS directories
   if [[ -z $(echo $directory | grep -i CVS) ]]; then
     
      
      # Ignore the ./data directory creation itself
      if [ "$directory" != "$data" ]; then
         # remove the ./data/
         dir=$(echo $directory | sed 's/\.\/data\///g')

         # install the directory
         $instApp -d $datadir/$package/$dir
      else
         dir=""
      fi
      
      # Get and Install all files in the directory
      for f in $data/$dir/*.*; do
         if [ "$f" != "$data/$dir/*.*" ]; then
            # install the file
            $instApp -c -m 644 $f $datadir/$package/$dir
         fi
      done

   fi

done

