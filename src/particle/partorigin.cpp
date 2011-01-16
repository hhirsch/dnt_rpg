/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#include "partorigin.h"
#include <stdlib.h>
#include <math.h>

/***********************************************************************
 *                               Constructor                           *
 ***********************************************************************/
dntPartOrigin::dntPartOrigin()
{
   int i;

   /* NULL values */
   for(i=0; i<3; i++)
   {
      p[i] = 0.0f;
      sum[i] = 0.0f;
   }
   radius = 0.0f;
   
   /* Default as point at 0,0,0 */
   type = DNT_PART_ORIGIN_POINT;
}

/***********************************************************************
 *                                setPoint                             *
 ***********************************************************************/
void dntPartOrigin::setPoint(float x, float y, float z)
{
   p[0] = x;
   p[1] = y;
   p[2] = z;
   type = DNT_PART_ORIGIN_POINT;
}

/***********************************************************************
 *                                setQuad                              *
 ***********************************************************************/
void dntPartOrigin::setQuad(float x1, float y1, float z1, 
      float x2, float y2, float z2)
{
   /* Make sure all first coordinates are <= than seconds */
   if(x1 <= x2)
   {
      p[0] = x1;
      sum[0] = x2-x1;
   }
   else
   {
      p[0] = x2;
      sum[0] = x1-x2;
   }
   if(y1 <= y2)
   {
      p[1] = y1;
      sum[1] = y2-y1;
   }
   else
   {
      p[1] = y2;
      sum[1] = y1-y2;
   }
   if(z1 <= z2)
   {
      p[2] = z1;
      sum[2] = z2-z1;
   }
   else
   {
      p[2] = z2;
      sum[2] = z1-z2;
   }

   type = DNT_PART_ORIGIN_QUAD;
}

/***********************************************************************
 *                               setSphere                             *
 ***********************************************************************/
void dntPartOrigin::setSphere(float x, float y, float z, float r)
{
   p[0] = x;
   p[1] = y;
   p[2] = z;
   radius = r;
   type = DNT_PART_ORIGIN_SPHERE;
}

/***********************************************************************
 *                     generateNewInitialPosition                      *
 ***********************************************************************/
bool dntPartOrigin::generateNewInitialPosition(float& x, float& y, float& z)
{
   switch(type)
   {
      case DNT_PART_ORIGIN_POINT:
      {
         /* Always start at the same point */
         x = p[0];
         y = p[1];
         z = p[2];
      }
      break;
      case DNT_PART_ORIGIN_QUAD:
      {
         /* Randomically start at area */
         x = sum[0]*(rand() / ((double)RAND_MAX +1))+p[0];
         y = sum[1]*(rand() / ((double)RAND_MAX +1))+p[1];
         z = sum[2]*(rand() / ((double)RAND_MAX +1))+p[2];
      }
      break;
      case DNT_PART_ORIGIN_SPHERE:
      {
         float vX, vY, vZ;
         /* Randomically start at area */
         x = radius*(rand() / ((double)RAND_MAX +1))+p[0];
         y = radius*(rand() / ((double)RAND_MAX +1))+p[1];
         z = radius*(rand() / ((double)RAND_MAX +1))+p[2];

         /* Verify if inner sphere */
         vX = (x-p[0]);
         vY = (y-p[1]);
         vZ = (z-p[2]);
         if(sqrt(vX*vX + vY*vY + vZ*vZ) > radius)
         {
            /* Ops, out of sphere... force at center! */
            x = p[0];
            y = p[1];
            z = p[2];
         }
      }
      break;
      default:
      {
         return(false);
      }
      break;
   }

   return(true);
}

