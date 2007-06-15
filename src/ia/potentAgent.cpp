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
bool potentAgent::defineNextPosition()
{
   int aux;
   GLfloat min1[3], min2[3], max1[3], max2[3];
   GLfloat fX = 0;
   GLfloat fZ = 0;
   GLfloat dX;
   GLfloat dZ;
   GLfloat dist;
   GLfloat cosA, senA;
   GLfloat force;

   min1[0] = x1+actualX;
   min1[1] = 0;
   min1[2] = z1+actualZ;
   max1[0] = x2+actualX;
   max1[1] = 0;
   max1[2] = z2+actualZ;

   min2[1] = 0;
   max2[1] = 0;

   for(aux = 0; aux < knowObstacles; aux++)
   {
      min2[0] = obstacles[aux].x1;
      min2[2] = obstacles[aux].z1;
      max2[0] = obstacles[aux].x2;
      max2[2] = obstacles[aux].z2;

      dX = obstacles[aux].x - actualX;
      dZ = obstacles[aux].z - actualZ;
      
      dist = sqrt( (dX*dX) + (dZ*dZ));
      cosA = dX / dist;
      senA = dZ / dist;

      if(intercepts(min1, max1, min2, max2, 1))
      {
         force = -kc*dist;
      }
      else
      {
         force = -kr / dist;
      }
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
   force = ka * dist;
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


