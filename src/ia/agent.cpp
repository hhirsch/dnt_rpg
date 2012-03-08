/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

#include "agent.h"
#include "../etc/distance.h"
#include "../engine/util.h"
#include <math.h>
#include <stdio.h>

#ifndef PID180
   #define TWOPI  2 * M_PI
   #define PID180 M_PI / 180.0
#endif


/********************************************************************
 *                         Constructor                              *
 ********************************************************************/
agent::agent(bool oriented)
{
   stepSize = 0;
   destinyX = -1;
   destinyZ = -1;
   actualX = -1;
   actualZ = -1;
   withOrientation = oriented;
   orientation = 0;
   knowObstacles = 0;
}

/********************************************************************
 *                          Destructor                              *
 ********************************************************************/
agent::~agent()
{
   stepSize = 0;
   destinyX = -1;
   destinyZ = -1;
   actualX = -1;
   actualZ = -1;
}

/********************************************************************
 *                     Define Step Size                             *
 ********************************************************************/
void agent::defineStepSize(GLfloat size)
{
   stepSize = size;
}

/********************************************************************
 *                          Define Destiny                          *
 ********************************************************************/
void agent::defineDestiny(GLfloat x, GLfloat z)
{
   destinyX = x;
   destinyZ = z;
}

/********************************************************************
 *                          Define Position                         *
 ********************************************************************/
void agent::definePosition(GLfloat x, GLfloat z)
{
   actualX = x;
   actualZ = z;
}

/********************************************************************
 *                           Get Position                           *
 ********************************************************************/
void agent::getPosition(GLfloat& x, GLfloat& z)
{
   x = actualX;
   z = actualZ;
}

/********************************************************************
 *                           define Sight                           *
 ********************************************************************/
void agent::defineSight(GLfloat sightDist, GLfloat sightAng)
{
   sightDistance = sightDist;
   sightAngle = sightAng;
}

/********************************************************************
 *                           define Sight                           *
 ********************************************************************/
void agent::getSight(GLfloat& sightDist, GLfloat& sightAng)
{
   sightDist = sightDistance;
   sightAng = sightAngle;
}


/********************************************************************
 *                            Actualize                             *
 ********************************************************************/
void agent::actualize()
{
   defineNextPosition(false);
}

/********************************************************************
 *                           addObstacle                            *
 ********************************************************************/
void agent::addObstacle(GLfloat x, GLfloat z, GLfloat x1, GLfloat z1,
                       GLfloat x2, GLfloat z2)
{
   if(knowObstacles < MAX_OBSTACLES)
   {
      obstacles[knowObstacles].x = x;
      obstacles[knowObstacles].z = z;
      obstacles[knowObstacles].x1 = x1;
      obstacles[knowObstacles].z1 = z1;
      obstacles[knowObstacles].x2 = x2;
      obstacles[knowObstacles].z2 = z2;
      knowObstacles++;
   }
}

/********************************************************************
 *                            clearObstacle                         *
 ********************************************************************/
void agent::clearObstacles()
{
   knowObstacles = 0;
}

/********************************************************************
 *                             doAngle                              *
 ********************************************************************/
bool agent::doAngle()
{
   /* Must put at range before check */
   while(desiredAngle < 0)
   {
      desiredAngle += 360;
   }
   while(orientation < 0)
   {
      orientation += 360;
   }

   /* Now recalculate, if are differents */
   if(orientation != desiredAngle)
   {
      GLfloat d1;
      GLfloat d2;
      if(orientation > desiredAngle)
      {
         d1 = (orientation - desiredAngle);
         d2 = (360 - orientation + desiredAngle);
      }
      else
      {
         d2 = (desiredAngle - orientation);
         d1 = (360 - desiredAngle + orientation);
      }

      if(d1 < d2)
      {
         //Sub Value
         if(d1 < 2*MAX_ROTATION)
         {
            orientation = desiredAngle;
         }
         else
         {
            orientation -= 2*MAX_ROTATION;
         }
      }
      else
      {
         //Sum Value
         if(d2 < 2*MAX_ROTATION)
         {
            orientation = desiredAngle;
         }
         else
         {
            orientation += 2*MAX_ROTATION;
         }

      }

      while(orientation < 0)
      {
         orientation += 360;
      }
      
      return(true);
   }
   return(false);
}

/********************************************************************
 *                        define Bounding Box                       *
 ********************************************************************/
void agent::defineBoundingBox(GLfloat xa, GLfloat za, GLfloat xb, GLfloat zb)
{
   x1 = xa;
   z1 = za;
   x2 = xb;
   z2 = zb;
}

/********************************************************************
 *                          get Bounding Box                        *
 ********************************************************************/
void agent::getBoundingBox(GLfloat &xa, GLfloat &za, GLfloat &xb, GLfloat &zb)
{
   xa = x1;
   za = z1;
   xb = x2;
   zb = z2;
}

