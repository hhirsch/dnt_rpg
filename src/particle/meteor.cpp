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
}

/*****************************************************************
 *                           Destructor                          *
 *****************************************************************/
meteor::~meteor()
{
   delete(intFire);
}

/*****************************************************************
 *                           initRender                          *
 *****************************************************************/
void meteor::InitRender()
{
   if(!dead)
   {
      curPosY += varY;
      curPosX += varX;
      curPosZ += varZ;
   }
   else
   {
      actualLiving++;
   }

   /* Verify if hits the target */
   dead = ( ( ((varX > 0) && (curPosX >= targX)) ||
              ((varX <= 0) && (curPosX <= targX))) &&
            ( ((varZ > 0) && (curPosZ >= targZ)) ||
              ((varZ <= 0) && (curPosZ <= targZ))) &&
            ( ((varY > 0) && (curPosY >= targY)) ||
              ((varY <= 0) && (curPosY <= targY))) );
   
   glPushMatrix();
      //Don't translate, but actualize the initial position!
      intFire->definePosition(curPosX, curPosY, curPosZ);
      //glTranslatef(curPosX, curPosY, curPosZ );

      /* Set the Correct Orientation of the meteor */
      if(varX < 0)
      {
         glRotatef(-90.0, 0, 0, 1);
      }
      else if(varX > 0)
      {
         glRotatef(90.0, 0, 0, 1);
      }
      if(varZ < 0)
      {
         glRotatef(-90.0, 1, 0, 0);
      }
      else if(varZ > 0)
      {
         glRotatef(90.0, 1, 0, 0);
      }
      if(varY > 0)
      {
         glRotatef(180.0, 1, 0, 0);
      }
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

