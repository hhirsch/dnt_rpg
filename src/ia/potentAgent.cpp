#include "potentAgent.h"
#include <math.h>

#define KA 0.05
#define KR 0.05

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
   
   for(aux = 0; aux < knowObstacles; aux++)
   {
      dX = obstacles[aux].x - actualX;
      dZ = obstacles[aux].z - actualZ;
      fX += KR / dX;
      fZ += KR / dZ;
   }

   //Sum Goal
   fX += KA * (destinyX - actualX);
   fZ += KA * (destinyZ - actualZ);

   dX = 0;
   dZ = 0;

   if( (fX != 0) || (fZ != 0))
   {
      if(fX != 0)
      {
         dX = 1 / fX;
      }
      if(fZ != 0)
      {
         dZ = 1 / fX;
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


