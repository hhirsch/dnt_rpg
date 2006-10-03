#include "pattAgent.h"
#include <math.h>
#include <stdio.h>

#define KA 0.005
#define KR 0.5

/********************************************************************
 *                         Constructor                              *
 ********************************************************************/
pattAgent::pattAgent(bool oriented):agent(oriented)
{
   wayPoints = NULL;
   actualWayPoint = NULL;
   totalWayPoints = 0;
}

/********************************************************************
 *                          Destructor                              *
 ********************************************************************/
pattAgent::~pattAgent()
{
   /* Delete Way Points List */
   wayPoint* aux;
   actualWayPoint = wayPoints;
   int wp;
   for(wp = 0; wp < totalWayPoints; wp++)
   {
      aux = actualWayPoint;
      actualWayPoint = actualWayPoint->next;
      delete(aux);
   }
}

/********************************************************************
 *                    define Next Position                          *
 ********************************************************************/
bool pattAgent::defineNextPosition()
{
   if( (actualX == actualWayPoint->x) && (actualZ == actualWayPoint->z))
   {
      //Arrived at the actual Way Point, so change to next!
      changeToNextWayPoint();
   }

  
   /* Actualize position, making sure that goes exactly to the waypoint */
   if( ((xInc > 0) && (actualX + xInc > actualWayPoint->x)) ||
       ((xInc < 0) && (actualX + xInc < actualWayPoint->x)) )
   {
      actualX = actualWayPoint->x;
   }
   else
   {
      actualX += xInc;
   }

   if( ((zInc > 0) && (actualZ + zInc > actualWayPoint->z)) ||
       ((zInc < 0) && (actualZ + zInc < actualWayPoint->z)) )
   {
      actualZ = actualWayPoint->z;
   }
   else
   {
      actualZ += zInc;
   }

   return(false);
}

/********************************************************************
 *                           addWayPoint                            *
 ********************************************************************/
void pattAgent::addWayPoint(GLfloat x, GLfloat z)
{
   wayPoint* newWay = new(wayPoint);
   newWay->x = x;
   newWay->z = z;
   if(wayPoints == NULL)
   {
      newWay->next = newWay;
      newWay->previous = newWay;
      wayPoints = newWay;
   }
   else
   {
      newWay->previous = wayPoints->previous;
      newWay->previous->next = newWay;
      newWay->next = wayPoints;
      wayPoints->previous = newWay;
   }

   totalWayPoints++;
   
   if(actualWayPoint == NULL)
   {
      actualWayPoint = newWay;
   }
}

/********************************************************************
 *                       changeTONextWayPoint                       *
 ********************************************************************/
void pattAgent::changeToNextWayPoint()
{
   actualWayPoint = actualWayPoint->next;

   if(actualWayPoint)
   {
      GLfloat dx = actualWayPoint->x - actualX;
      GLfloat dz = actualWayPoint->z - actualZ;
      //GLfloat dist = sqrt((dx*dx) + (dz*dz));

      actualLoop = 0;
   
      if(fabs(dx) > fabs(dz))
      {
         loops = (int) (fabs(dx) / stepSize);
         xInc = (float) (dx) / (float) (loops);
         zInc = (float) (dz) / (float) (loops);
         //sqrt( (dist*dist) - (xInc*xInc) );
      }
      else
      {
         loops = (int) (fabs(dz) / stepSize);
         zInc = (float) (dz) / (float) (loops);
         xInc = (float) (dx) / (float) (loops);
         //sqrt( (dist*dist) - (zInc*zInc) );
      }
   }
   
}

