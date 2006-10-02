#include "potentAgent.h"
#include <math.h>
#include <stdio.h>

#define KA 0.005
#define KR 0.5

/********************************************************************
 *                         Constructor                              *
 ********************************************************************/
potentAgent::potentAgent(bool oriented):agent(oriented)
{
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
bool potentAgent::defineNextPosition()
{
   int aux;
   GLfloat fX = 0;
   GLfloat fZ = 0;
   GLfloat dX;
   GLfloat dZ;
   GLfloat dist;
   GLfloat cosA, senA;
   GLfloat force;
   
   for(aux = 0; aux < knowObstacles; aux++)
   {
      dX = obstacles[aux].x - actualX;
      dZ = obstacles[aux].z - actualZ;
      dist = sqrt( (dX*dX) + (dZ*dZ));
      cosA = dX / dist;
      senA = dZ / dist;
      force = -KR / dist;
      fX += force*cosA;
      fZ += force*senA;
      //printf("force: %.3f dist: %.3f\n",force, dist);
   }

   //Sum Goal
   dX = (destinyX - actualX);
   dZ = (destinyZ - actualZ);
   dist = sqrt( (dX*dX) + (dZ*dZ));
   cosA = dX / dist;
   senA = dZ / dist;
   force = KA * dist;
   fX += force*cosA;
   fZ += force*senA;

   //printf("KA: force: %.3f dist: %.3f\n",force, dist);
   //printf("fx: %.3f fz:%.3f\n",fX, fZ);

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

      //TODO Rotation, if holonomic
      
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


