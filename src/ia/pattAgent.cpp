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

#include "pattAgent.h"
#include "../engine/util.h"

#include <iostream>
#include <math.h>

using namespace std;


#define KA 0.005
#define KR 0.5

/********************************************************************
 *                         Constructor                              *
 ********************************************************************/
wayPointList::wayPointList()
{
}

/********************************************************************
 *                          Destructor                              *
 ********************************************************************/
wayPointList::~wayPointList()
{
   clearList();
}

/********************************************************************
 *                         freeElement                              *
 ********************************************************************/
void wayPointList::freeElement(dntListElement* obj)
{
   wayPoint* w = (wayPoint*)obj;
   delete(w);
}

/********************************************************************
 *                         Constructor                              *
 ********************************************************************/
pattAgent::pattAgent(bool oriented):agent(oriented)
{
   actualWayPoint = NULL;
   totalWalked = 0;
   origX = 0;
   origZ = 0;
}

/********************************************************************
 *                          Destructor                              *
 ********************************************************************/
pattAgent::~pattAgent()
{
   list.clearList();
}

/********************************************************************
 *                         getTotalWayPoints                        *
 ********************************************************************/
int pattAgent::getTotalWayPoints()
{
   return(list.getTotal());
}

/********************************************************************
 *                        getTotalWalked                            *
 ********************************************************************/
float pattAgent::getTotalWalked()
{
   return(totalWalked);
}

/********************************************************************
 *                      getActualWayPoint                           *
 ********************************************************************/
wayPoint* pattAgent::getActualWayPoint()
{
   return(actualWayPoint);
}

/********************************************************************
 *                          setOrigin                               *
 ********************************************************************/
void pattAgent::setOrigin(GLfloat x, GLfloat z)
{
   origX = x;
   origZ = z;
}

/********************************************************************
 *                         calculateAngle                           *
 ********************************************************************/
void pattAgent::calculateAngle(wayPoint* way, wayPoint* previous)
{
   if(withOrientation)
   {
      way->angle = getAngle(previous->x, previous->z, way->x, way->z);
   }
}

/********************************************************************
 *                    define Next Position                          *
 ********************************************************************/
bool pattAgent::defineNextPosition(bool run)
{
   float varX=0, varZ=0;
   float xIncCur, zIncCur;

   if(run)
   {
      xIncCur = xInc*ENGINE_RUN_MULTIPLIER;
      zIncCur = zInc*ENGINE_RUN_MULTIPLIER;
   }
   else
   {
      xIncCur = xInc;
      zIncCur = zInc;
   }

   if(!actualWayPoint)
   {
      /* No wayPoints defined. don't move. */
      return(false); 
   }

   if( (actualX == actualWayPoint->x) && (actualZ == actualWayPoint->z))
   {
      /* Arrived at the actual Way Point, so change to next! */
      changeToNextWayPoint();
      
      /* calculate angle */
      desiredAngle = actualWayPoint->angle;
      if(doAngle())
      {
         return(true);
      }
      
   }
   else if(doAngle())
   {
      return(true);
   }

  
   /* Update position, making sure that goes exactly to the waypoint */
   if( ((xIncCur > 0) && (actualX + xIncCur > actualWayPoint->x)) ||
       ((xIncCur < 0) && (actualX + xIncCur < actualWayPoint->x)) ||
       (xIncCur == 0))
   {
      actualX = actualWayPoint->x;
   }
   else
   {
      actualX += xIncCur;
   }

   if( ((zIncCur > 0) && (actualZ + zIncCur > actualWayPoint->z)) ||
       ((zIncCur < 0) && (actualZ + zIncCur < actualWayPoint->z)) ||
       (zIncCur == 0))
   {
      actualZ = actualWayPoint->z;
   }
   else
   {
      actualZ += zIncCur;
   }

   /* Calculate the linear distance to the orign */ 
   varX = actualX - origX;
   varZ = actualZ - origZ;
   totalWalked = sqrt(varX*varX + varZ*varZ);

   return(true);
}

/********************************************************************
 *                           addWayPoint                            *
 ********************************************************************/
void pattAgent::addWayPoint(GLfloat x, GLfloat z)
{
   wayPoint* newWay = new(wayPoint);
   newWay->x = x;
   newWay->z = z;
   
   /* insert at the list */
   list.insertAtEnd(newWay);

   /* Calculate the orientation angle */
   if( (newWay) != (newWay->getPrevious()))
   {
      /* Calculate angle between this wayPoint and the previous one  */
      calculateAngle(newWay, (wayPoint*)newWay->getPrevious());
   }
   else
   {
      /* First wayPoint, so orientation is equal to current */
      newWay->angle = orientation;
   }

   /* Set the current, if not defined */
   if(actualWayPoint == NULL)
   {
      actualWayPoint = newWay;
   }
}

/********************************************************************
 *                        addWayPointFirst                          *
 ********************************************************************/
void pattAgent::addWayPointFirst(GLfloat x, GLfloat z)
{
   wayPoint* newWay = new(wayPoint);
   newWay->x = x;
   newWay->z = z;

   /* insert at the list */
   list.insertAtBegin(newWay);

   /* Calculate the orientation angle */
   if( (newWay) != (newWay->getNext()))
   {
      /* Calculate angle between the next wayPoint and this one  */
      calculateAngle((wayPoint*)newWay->getNext(), newWay);
   }
   else
   {
      /* First wayPoint, so orientation is equal to current */
      newWay->angle = orientation;
   }

   /* Always set the current */
   actualWayPoint = newWay;
}

/********************************************************************
 *                         removeWayPoint                           *
 ********************************************************************/
void pattAgent::removeWayPoint(wayPoint* way)
{
   if(way)
   {
      /* Update the current */
      if(way == actualWayPoint)
      {
         actualWayPoint = (wayPoint*)actualWayPoint->getNext();
      }

      /* Remove from list */
      list.remove(way);

      /* Verify if has more elements */
      if(list.getTotal() == 0)
      {
         actualWayPoint = NULL;
      }
   }
}

/********************************************************************
 *                      removeLinearWayPoints                       *
 ********************************************************************/
void pattAgent::removeLinearWayPoints()
{
   /* note: Only can't remove the first and the last wayPoints. */
   int total = list.getTotal();
   int i;
   wayPoint* way = (wayPoint*)list.getFirst();
   wayPoint* oth;
   for(i = 0; i<total; i++)
   {
      oth = way;
      way = (wayPoint*)way->getNext();
      if( (oth != list.getFirst()) && (oth != list.getFirst()->getPrevious()) )
      {
         if(oth->angle == ((wayPoint*)oth->getNext())->angle)
         {
            /* Can remove the wayPoint */
            removeWayPoint(oth);
         }
      }
   } 
}


/********************************************************************
 *                       changeTONextWayPoint                       *
 ********************************************************************/
void pattAgent::changeToNextWayPoint()
{
   float loops;
   if(!actualWayPoint)
   {
      return; //not defined yet the way points, so stay static.
   }
   actualWayPoint = (wayPoint*)actualWayPoint->getNext();

   if(actualWayPoint)
   {
      GLfloat dx = actualWayPoint->x - actualX;
      GLfloat dz = actualWayPoint->z - actualZ;
      //GLfloat dist = sqrt((dx*dx) + (dz*dz));

      loops = (sqrt((dx*dx)+(dz*dz)) / stepSize);
      if(loops == 0)
      {
         xInc = 0;
         zInc = 0;
      }
      else
      {
         xInc = (float) (dx) / (loops);
         zInc = (float) (dz) / (loops);
      }
   }
   
}

/********************************************************************
 *                            drawWayPoints                         *
 ********************************************************************/
void pattAgent::drawWayPoints()
{
   wayPoint* tmp = (wayPoint*)list.getFirst();
   int aux = 0;
   if(tmp)
   {
      glDisable(GL_LIGHTING);
      glLineWidth(3);
      glBegin(GL_LINE_LOOP);
      while(aux < list.getTotal())
      {
         glVertex3f(tmp->x, 0.1, tmp->z);
         aux++;
         tmp = (wayPoint*)tmp->getNext();
      }
      glEnd();
      glLineWidth(1);
      glEnable(GL_LIGHTING);
   }
}

/********************************************************************
 *                            drawWayLinear                         *
 ********************************************************************/
void pattAgent::drawWayPointsLinear()
{
   wayPoint* tmp = (wayPoint*)list.getFirst();
   int aux = 0;
   if(tmp)
   {
      glDisable(GL_LIGHTING);
      glLineWidth(3);
      glBegin(GL_LINE_STRIP);
      while(aux < list.getTotal())
      {
         glVertex3f(tmp->x, 0.1, tmp->z);
         aux++;
         tmp = (wayPoint*)tmp->getNext();
      }
      glEnd();
      glLineWidth(1);
      glEnable(GL_LIGHTING);
   }
}


