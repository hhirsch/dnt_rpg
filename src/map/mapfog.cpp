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
       cerr << "Error while opening fog: " << fileName << endl;
       return(false);
   }
   fprintf(file,"%f %f %f %f\n",color[0],color[1],color[2],color[3]);
   fprintf(file,"%f\n",density);
   fprintf(file,"%f\n",start);
   fprintf(file,"%f\n",end);
   fclose(file);
   return(true);
}

/****************************************************************
 *                             apply                            *
 ****************************************************************/
void mapFog::apply(bool outdoor, float ofarview, float ifarview)
{
   if(enabled)
   {
      glEnable(GL_FOG);
      {
        //glFogi(GL_FOG_COORD_SRC, GL_FRAGMENT_DEPTH);
        glFogi(GL_FOG_MODE,GL_LINEAR);
        glFogfv(GL_FOG_COLOR, color);
        glFogf(GL_FOG_DENSITY, density);
        glHint(GL_FOG_HINT,GL_DONT_CARE);
        glFogf(GL_FOG_START, start);
        glFogf(GL_FOG_END, end);
      }
   }
   else
   {
      /* Default fog used */
      GLfloat fogEnd = (ofarview) + 4000;
      GLfloat fogStart = 200.0f;
      GLfloat fogDensity = 5.0f;
      GLfloat color[3] = {1.0f, 1.0f, 1.0f};
      if(!outdoor)
      {
         color[0] = 0.0f;
         color[1] = 0.0f;
         color[2] = 0.0f;
         fogStart = 40.0f;
         fogDensity = 1.0f;
         fogEnd = (ifarview)-2;
      }
      glEnable(GL_FOG);
      {
        glFogi(GL_FOG_MODE,GL_LINEAR);
        glFogfv(GL_FOG_COLOR, color);
        glFogf(GL_FOG_DENSITY, fogDensity);
        glHint(GL_FOG_HINT, GL_DONT_CARE);
        glFogf(GL_FOG_START, fogStart);
        glFogf(GL_FOG_END, fogEnd);
      }
   }
}

