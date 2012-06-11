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

#include "potentAgent.h"
#include "../engine/util.h"
#include <math.h>
#include <stdio.h>

/********************************************************************
 *                         Constructor                              *
 ********************************************************************/
potentAgent::potentAgent(bool oriented):agent(oriented)
{
   defineConstants(0.005, 100000, 0.5);
}

/********************************************************************
 *                         defineConstants                          *
 ********************************************************************/
void potentAgent::defineConstants(GLfloat a, GLfloat c, GLfloat r)
{
   ka = a;
   kc = c;
   kr = r;
}

/********************************************************************
 *                          Destructor                              *
 ********************************************************************/
potentAgent::~potentAgent()
{
}

/********************************************************************
 *                    define Next Position                          *
 ********************************************************************/
bool potentAgent::defineNextPosition(bool run)
{
   int aux;
   GLfloat fX = 0;
   GLfloat fZ = 0;
   GLfloat dX;
   GLfloat dZ;

#if 0
   GLfloat min1[3], min2[3], max1[3], max2[3];

   min1[0] = x1+actualX;
   min1[1] = 0;
   min1[2] = z1+actualZ;
   max1[0] = x2+actualX;
   max1[1] = 0;
   max1[2] = z2+actualZ;

   min2[1] = 0;
   max2[1] = 0;
#endif

   for(aux = 0; aux < knowObstacles; aux++)
   {
#if 0
      min2[0] = obstacles[aux].x1;
      min2[2] = obstacles[aux].z1;
      max2[0] = obstacles[aux].x2;
      max2[2] = obstacles[aux].z2;
#endif

      dX = obstacles[aux].x - actualX;
      dZ = obstacles[aux].z - actualZ;
     
#if 0
      if(intercepts(min1, max1, min2, max2))
      {
         fX += -kc*dX;
         fZ += -kc*dZ;
      }
      else
      {
         fX += -kr/dX;
         fZ += -kr/dZ;
      }
#endif
   }


   //Sum Goal
   dX = (destinyX - actualX);
   dZ = (destinyZ - actualZ);
   fX += ka*dX;
   fZ += ka*dX;

   dX = 0;
   dZ = 0;

   if( (fX != 0) || (fZ != 0))
   {
      if(fX != 0)
      {
         dX = fX;
      }
      if(fZ != 0)
      {
         dZ = fZ;
      }

      //FIXME Correct Saturation
      if(dX > stepSize)
      {
         dX = stepSize;
      }
      else if(dX < -stepSize)
      {
         dX = -stepSize;
      }
      if(dZ > stepSize)
      {
         dZ = stepSize;
      }
      else if(dZ < -stepSize)
      {
         dZ = -stepSize;
      }

      actualX += dX;
      actualZ += dZ;
      
      return(true);
   }
   else
   {
      return(false);
   }
}


