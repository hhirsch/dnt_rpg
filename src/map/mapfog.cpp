/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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

#include "mapfog.h"
#include "../etc/dirs.h"

#include <stdio.h>

#include <iostream>
#include <fstream>
using namespace std;

/****************************************************************
 *                         Constructor                          *
 ****************************************************************/
mapFog::mapFog()
{
   int i;
   for(i=0; i < 4; i++)
   {
      color[i] = 0;
   }
   density = 0;
   start = 0;
   end = 0;
   enabled = false;
   fileName = "";
}

/****************************************************************
 *                          Destructor                          *
 ****************************************************************/
mapFog::~mapFog()
{
   fileName = "";
}

/****************************************************************
 *                             Load                             *
 ****************************************************************/
void mapFog::load(string arq)
{
   ifstream file;
   string buffer;
   dirs dir;

   file.open(dir.getRealFile(arq).c_str(), ios::in | ios::binary);

   if(!file)
   {
       cerr << "Error while opening fog file: " << arq << endl;
       return;
   }
   fileName = arq;
   getline(file, buffer);
   sscanf(buffer.c_str(),"%f %f %f %f",&color[0],&color[1],&color[2],&color[3]);
   getline(file, buffer);
   sscanf(buffer.c_str(),"%f",&density);
   getline(file, buffer);
   sscanf(buffer.c_str(),"%f",&start);
   getline(file, buffer);
   sscanf(buffer.c_str(),"%f",&end);
   enabled = true;

   file.close();
}

/****************************************************************
 *                             save                             *
 ****************************************************************/
bool mapFog::save()
{
   FILE* file;

   if(!(file=fopen(fileName.c_str(),"w")))
   {
       printf("Error while opening fog: %s\n",fileName.c_str());
       return(false);
   }
   fprintf(file,"%f %f %f %f\n",color[0],color[1],color[2],color[3]);
   fprintf(file,"%f\n",density);
   fprintf(file,"%f\n",start);
   fprintf(file,"%f\n",end);
   fclose(file);
   return(true);
}

