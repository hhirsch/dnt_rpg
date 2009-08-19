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

#include "meteor.h"
#include "../engine/util.h"

#define METEOR_STATIC_LIVING_TIME  1500 /**< Static living time of the meteor */

/*****************************************************************
 *                          Constructor                          *
 *****************************************************************/
meteor::meteor(float cX, float cY, float cZ, float vX, float vY, float vZ,
               float tX, float tY, float tZ, string fileName):particleSystem()
                                     
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

   type = DNT_PARTICLE_TYPE_METEOR;

   setDurationTime(METEOR_STATIC_LIVING_TIME);

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
void meteor::initRender()
{
   if(!dead)
   {
      /* Clear the timer, since not dead */
      systemInitialLiveTime = SDL_GetTicks();

      /* Update Position */
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
         //GLfloat varHeight=0, 
         //        nX = 0, 
         //        nZ = 0;
         /* FIXME Verify Collisions */
         //dead = !colDetect->canWalk(curPosX, curPosY, curPosZ, 
         //                           -0.5,-0.5,-0.5, 0.5,0.5,0.5,
         //                            0.0, NULL, varHeight, nX, nZ);
      }
   }
   
   glPushMatrix();
      intFire->definePosition(curPosX, curPosY, curPosZ);
}

/*****************************************************************
 *                            endRender                          *
 *****************************************************************/
void meteor::endRender()
{
   glPopMatrix();
}

/*****************************************************************
 *                            isLiving                           *
 *****************************************************************/
bool meteor::isLiving()
{
   int time = SDL_GetTicks();

   if( (systemMaxLiveTime != 0) && 
       (time - systemInitialLiveTime >= systemMaxLiveTime) )
   {
      return(false);
   }
   return(true);
}

/****************************************************************************
 *                                NextStep                                  *
 ****************************************************************************/
void meteor::nextStep(GLfloat** matriz)
{
   initRender();
   intFire->nextStep(matriz);
   endRender();
}

void meteor::render(particle* p)
{
}

void meteor::update(particle* p)
{
}

bool meteor::continueLive(particle* p)
{
   return(true);
}

int meteor::needCreate()
{
   return(0);
}

void meteor::createParticle(particle* p)
{
}

