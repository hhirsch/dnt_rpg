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
bool pattAgent::defineNextPosition()
{
   if(!actualWayPoint)
   {
      return(false); //not defined yet the way points, so stay static.
   }

   if( (actualX == actualWayPoint->x) && (actualZ == actualWayPoint->z))
   {
      //Arrived at the actual Way Point, so change to next!
      changeToNextWayPoint();
      
      //calculate angle
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
   if( ((xInc > 0) && (actualX + xInc > actualWayPoint->x)) ||
       ((xInc < 0) && (actualX + xInc < actualWayPoint->x)) ||
       (xInc == 0))
   {
      actualX = actualWayPoint->x;
   }
   else
   {
      actualX += xInc;
   }

   if( ((zInc > 0) && (actualZ + zInc > actualWayPoint->z)) ||
       ((zInc < 0) && (actualZ + zInc < actualWayPoint->z)) ||
       (zInc == 0))
   {
      actualZ = actualWayPoint->z;
   }
   else
   {
      actualZ += zInc;
   }

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
   if(wayPoints == NULL)
   {
      newWay->next = newWay;
      newWay->previous = newWay;
      wayPoints = newWay;
      newWay->angle = orientation;
   }
   else
   {
      newWay->previous = wayPoints->previous;
      newWay->previous->next = newWay;
      newWay->next = wayPoints;
      wayPoints->previous = newWay;
      calculateAngle(newWay, newWay->previous);
   }

   totalWayPoints++;
   
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
   if(wayPoints == NULL)
   {
      newWay->next = newWay;
      newWay->previous = newWay;
      wayPoints = newWay;
      newWay->angle = orientation;
   }
   else
   {
      newWay->previous = wayPoints->previous;
      newWay->previous->next = newWay;
      newWay->next = wayPoints;
      newWay->next->previous = newWay;
      wayPoints = newWay;
      calculateAngle(newWay->next, newWay);
   }

   totalWayPoints++;
   
   actualWayPoint = newWay;

}

/********************************************************************
 *                         removeWayPoint                           *
 ********************************************************************/
void pattAgent::removeWayPoint(wayPoint* way)
{
   if(way)
   {
      if(way == wayPoints)
      {
         wayPoints = wayPoints->next;
      }
      way->next->previous = way->previous;
      way->previous->next = way->next;
      delete(way);
      totalWayPoints--;
      if(totalWayPoints <= 0)
      {
         wayPoints = NULL;
      }
   }
}

/********************************************************************
 *                      removeLinearWayPoints                       *
 ********************************************************************/
void pattAgent::removeLinearWayPoints()
{
   /* note: Only can't remove the first and the last wayPoints. */
   int total = totalWayPoints;
   int i;
   wayPoint* way = wayPoints;
   wayPoint* oth;
   for(i = 0; i<total; i++)
   {
      oth = way;
      way = way->next;
      if( (oth != wayPoints) && (oth != wayPoints->previous) )
      {
         if(oth->angle == oth->next->angle)
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
   //FIXME -> diagonal Moves are > stepSize per time.
   float loops;
   if(!actualWayPoint)
   {
      return; //not defined yet the way points, so stay static.
   }
   actualWayPoint = actualWayPoint->next;

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
   wayPoint* tmp = wayPoints;
   int aux = 0;
   if(tmp)
   {
      glDisable(GL_LIGHTING);
      glLineWidth(3);
      glBegin(GL_LINE_LOOP);
      while(aux < totalWayPoints)
      {
         glVertex3f(tmp->x, 0.1, tmp->z);
         aux++;
         tmp = tmp->next;
      }
      glEnd();
      glLineWidth(1);
      glEnable(GL_LIGHTING);
   }
}

void pattAgent::drawWayPointsLinear()
{
   wayPoint* tmp = wayPoints;
   int aux = 0;
   if(tmp)
   {
      glDisable(GL_LIGHTING);
      glLineWidth(3);
      glBegin(GL_LINE_STRIP);
      while(aux < totalWayPoints)
      {
         glVertex3f(tmp->x, 0.1, tmp->z);
         aux++;
         tmp = tmp->next;
      }
      glEnd();
      glLineWidth(1);
      glEnable(GL_LIGHTING);
   }
}


