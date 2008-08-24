#include "dirs.h"

#include <stdlib.h>
#include <stdio.h>

#ifdef _MSC_VER
   #include "../config_win.h"
#else
   #include "../config.h"
#endif

/*********************************************************************
 *                         findDataDirectories                       *
 *********************************************************************/
void dirs::findDataDirectories()
{
   FILE* arq;
   string file;

   /* Try to open locally */
   file = "../data/fnt/arial.ttf";
   if((arq = fopen(file.c_str(),"r")))
   {
      fclose(arq);
      dataPath = "../data/";
   }
   else
   {
      /* Try to open at defined datadir */
      file = DATADIR;
      file += "/";
      file += PACKAGE;
      file += "/fnt/arial.ttf";
      if((arq = fopen(file.c_str(),"r")))
      {
         dataPath = DATADIR;
         dataPath += "/";
         dataPath += PACKAGE;
         dataPath += "/";
         fclose(arq);
      }
      else
      {
         printf("Error: Couldn't define the data directories!\n");
         printf("Tried: %s and %s\n","../data/", file.c_str());
         exit(-3);
      }
   }
}

/*********************************************************************
 *                             getRealFile                           *
 *********************************************************************/
string dirs::getRealFile(string fileName)
{
   /* Only change if not already changed or if no dataPath */
   if((dataPath != "") && (fileName.find(dataPath,0) == string::npos))
   {
      return(dataPath + fileName);
   }
   return(fileName);
}

/*********************************************************************
 *                           getRelativeFile                         *
 *********************************************************************/
string dirs::getRelativeFile(string fileName)
{
   string::size_type pos = fileName.find(dataPath, 0);

   /* Only need to change if the dataPath is in fileName's beginning */
   if(pos == 0)
   {
      fileName.erase(0, dataPath.length());
      return(fileName);
   }

   return(fileName);
}

string dirs::dataPath = "";



