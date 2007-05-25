#include "meteor.h"
#include "../engine/util.h"

/*****************************************************************
 *                          Constructor                          *
 *****************************************************************/
meteor::meteor(float cX, float cY, float cZ, float vX, float vY, float vZ,
               float tX, float tY, float tZ, string fileName)
                                     
{
   /* Create the Fire Particle */
   intFire = new part2(0, 0, 0, fileName);

   /* Define Initial Position */
   curPosY = cY;
   curPosX = cX;
   curPosZ = cZ;

   /* Define Variations */
   varX = vX;
   varY = vY;
   varZ = vZ;

   /* Define Target */
   targX = tX;
   targY = tY;
   targZ = tZ;

   /* Init Variables */
   actualLiving = 0;
   colDetect = NULL;
   dead = false;
}

/*****************************************************************
 *                           Destructor                          *
 *****************************************************************/
meteor::~meteor()
{
   delete(intFire);
}

/*****************************************************************
 *                         defineCollision                       *
 *****************************************************************/
void meteor::defineCollision(collision* col)
{
   colDetect = col;
}

/*****************************************************************
 *                           initRender                          *
 *****************************************************************/
void meteor::InitRender()
{
   if(dead)
   {
      actualLiving++;
   }
   else
   {
      /* Actualize Position */
      curPosY += varY;
      curPosX += varX;
      curPosZ += varZ;

      /* Verify if hits the target */
      dead = ( ( ((varX > 0) && (curPosX >= targX)) ||
                 ((varX <= 0) && (curPosX <= targX))) &&
               ( ((varZ > 0) && (curPosZ >= targZ)) ||
                 ((varZ <= 0) && (curPosZ <= targZ))) &&
               ( ((varY > 0) && (curPosY >= targY)) ||
                 ((varY <= 0) && (curPosY <= targY))) );

      if( (colDetect) && (!dead))
      {
         /* Not Used Variables */
         GLfloat varHeight=0, 
                 nX = 0, 
                 nZ = 0;
         /* Verify Collisions */
         dead = !colDetect->canWalk(curPosX, curPosY, curPosZ, 
                                    -0.5,-0.5,-0.5, 0.5,0.5,0.5,
                                     0.0, NULL, varHeight, nX, nZ);
      }
   }
   
   glPushMatrix();
      intFire->definePosition(curPosX, curPosY, curPosZ);
}

/*****************************************************************
 *                            endRender                          *
 *****************************************************************/
void meteor::EndRender()
{
   glPopMatrix();
}

/*****************************************************************
 *                            isLiving                           *
 *****************************************************************/
bool meteor::isLiving()
{
   return((!dead) || (actualLiving <= METEOR_STATIC_LIVING));
}

/****************************************************************************
 *                                NextStep                                  *
 ****************************************************************************/
void meteor::NextStep(GLfloat matriz[6][4])
{
   InitRender();
   intFire->NextStep(matriz);
   EndRender();
}

int meteor::numParticles()
{
   return(intFire->numParticles());
}

