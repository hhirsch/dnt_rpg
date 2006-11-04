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
   if(!actualWayPoint)
   {
      return(false); //not defined yet the way points, so stay static.
   }

   if(doAngle())
   {
      return(true);
   }

   if( (actualX == actualWayPoint->x) && (actualZ == actualWayPoint->z))
   {
      //Arrived at the actual Way Point, so change to next!
      changeToNextWayPoint();
      printf("Change to %.3f %.3f!\n", actualWayPoint->x, actualWayPoint->z);
      //calculate angle
      if(withOrientation)
      {
         GLfloat ax;
         GLfloat az;
         ax = fabs(actualX - actualWayPoint->x);
         az = fabs(actualZ - actualWayPoint->z);
         GLfloat alpha;
         if( (az != 0) )
         {
            alpha = ( (atan(fabs(az / ax)) / M_PI) * 180);
            if( (actualX > actualWayPoint->x) && (actualZ < actualWayPoint->z) )
            {
               alpha += 180;
            }
            if( (actualX < actualWayPoint->x) && (actualZ < actualWayPoint->z) )
            {
               alpha = -alpha;
            }
            if( (actualX > actualWayPoint->x) && (actualZ > actualWayPoint->z) )
            {
               alpha = 180-alpha;
            }
            desiredAngle = alpha-90; /* -90 to correct model orientation*/
            if(doAngle())
            {
               return(true);
            }
         }
         else
         {
            //alpha = 0 or 180;
            if(actualX < actualWayPoint->x)
            {
               desiredAngle = 0;
            }
            else
            {
               desiredAngle = 180;
            }
            if(doAngle())
            {
               return(true);
            }
         }
      }
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
   }
   else
   {
      newWay->previous = wayPoints->previous;
      newWay->previous->next = newWay;
      newWay->next = wayPoints;
      newWay->next->previous = newWay;
      wayPoints = newWay;
   }

   totalWayPoints++;
   
   actualWayPoint = newWay;
}


/********************************************************************
 *                       changeTONextWayPoint                       *
 ********************************************************************/
void pattAgent::changeToNextWayPoint()
{
   int loops;
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

      if(fabs(dx) > fabs(dz))
      {
         loops = (int) (fabs(dx) / stepSize);
         if(loops == 0)
         {
            xInc = 0;
            zInc = 0;
         }
         else
         {
            xInc = (float) (dx) / (float) (loops);
            zInc = (float) (dz) / (float) (loops);
         }
         //sqrt( (dist*dist) - (xInc*xInc) );
      }
      else
      {
         loops = (int) (fabs(dz) / stepSize);
         if(loops == 0)
         {
            xInc = 0;
            zInc = 0;
         }
         else
         {
            zInc = (float) (dz) / (float) (loops);
            xInc = (float) (dx) / (float) (loops);
         }
         //sqrt( (dist*dist) - (zInc*zInc) );
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

