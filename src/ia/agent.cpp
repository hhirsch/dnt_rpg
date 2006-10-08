#include "agent.h"
#include "../etc/distance.h"
#include <math.h>
#include <stdio.h>

#ifndef PID180
   #define TWOPI  2 * M_PI
   #define PID180 M_PI / 180.0
#endif


/********************************************************************
 *                         Constructor                              *
 ********************************************************************/
agent::agent(bool orientation)
{
   stepSize = 0;
   destinyX = -1;
   destinyZ = -1;
   actualX = -1;
   actualZ = -1;
   withOrientation = orientation;
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
   defineNextPosition();
}

/********************************************************************
 *                           addObstacle                            *
 ********************************************************************/
void agent::addObstacle(GLfloat x, GLfloat z)
{
   if(knowObstacles < MAX_OBSTACLES)
   {
      obstacles[knowObstacles].x = x;
      obstacles[knowObstacles].z = z;
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
   if(orientation != desiredAngle)
   {
      if(desiredAngle < 0)
      {
         desiredAngle += 360;
      }
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
         if(d1 < MAX_ROTATION)
         {
            orientation = desiredAngle;
         }
         else
         {
            orientation -= MAX_ROTATION;
         }
      }
      else
      {
         //Sum Value
         if(d2 < MAX_ROTATION)
         {
            orientation = desiredAngle;
         }
         else
         {
            orientation += MAX_ROTATION;
         }

      }

      if(orientation < 0)
      {
         orientation += 360;
      }
      
      return(true);
   }
   return(false);
}

/********************************************************************
 *                             addIfVisible                         *
 ********************************************************************/
bool agent::addIfVisible(agent* testAg)
{
   GLfloat x, z, dist;
   GLfloat agX, agZ, agSightDistance, agSightAngle;

   /* Take Current Agent Position */
   getPosition( agX, agZ );
   getSight(agSightDistance, agSightAngle);
   

   testAg->getPosition(x,z);

   dist = sqrt( (agX-x)*(agX-x) + (agZ-z)*(agZ-z) );

   if(fabs(dist) <= agSightDistance)
   {
      addObstacle(x,z);
      return(true);
   }

   return(false);
}


