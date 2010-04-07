/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "dirs.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

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
   string dataDir = DATADIR;
   dataDir += "/";
   dataDir += PACKAGE;

#if defined (__amigaos4__)
   dataDir = "PROGDIR:data";
#endif

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
      file = dataDir;
      file += "/fnt/arial.ttf";
      if((arq = fopen(file.c_str(),"r")))
      {
         dataPath = dataDir;
         dataPath += "/";
         fclose(arq);
      }
      else
      {
         cerr << "Error: Couldn't define the data directories!" << endl;
         cerr << "Tried: '../data' and " << file << endl;
         exit(-3);
      }
   }
}

/*********************************************************************
 *                             getRealFile                           *
 *********************************************************************/
string dirs::getRealFile(string fileName)
{
   /* Don't change full path fileNames (those with "/" at init)
      (or, Windows, those with ':' at second position) */
   if( (fileName[0] == '/') || (fileName[1] == ':') )
   {
      return(fileName);
   }

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



