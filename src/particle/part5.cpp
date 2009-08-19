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

/* Blood */

#include "part5.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL_image.h>

#define MAX_BLOOD_LIVING  1200 /**< Max Iterations that the blood continues */
#define BLOOD_REMOVING     200 /**< Time for removing blood particles */
#define MAX_BLOOD_LIVING_TIME MAX_BLOOD_LIVING * 30
#define MAX_BLOOD_REMOVING (MAX_BLOOD_LIVING - BLOOD_REMOVING)

/****************************************************************************
 *                             Constructor                                  *
 ****************************************************************************/
part5::part5(float cX,float cY,float cZ, string fileName):
                               particleSystem(fileName,PARTICLE_DRAW_GROUPS)
{
   dirs dir;

   type = DNT_PARTICLE_TYPE_BLOOD;
   centerX = cX; 
   alpha=cY; 
   centerZ=cZ;
   actualParticles = 0;
   terrainHeight = 0.0f;
   partTexture = loadTexture(dir.getRealFile("particles/part2.png"));
   doneCreation = false;
   setDurationTime(MAX_BLOOD_LIVING_TIME);
}

/****************************************************************************
 *                              Destructor                                  *
 ****************************************************************************/
part5::~part5()
{
   glDeleteTextures(1,&partTexture);
}

/****************************************************************************
 *                          setTerrainHeight                                *
 ****************************************************************************/
void part5::setTerrainHeight(float height)
{
   terrainHeight = height;
}

/****************************************************************************
 *                                Render                                    *
 ****************************************************************************/
void part5::render(particle* part)
{
   //Not Used
}

/****************************************************************************
 *                              InitRender                                  *
 ****************************************************************************/
void part5::initRender()
{
   glDisable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_FALSE);
   glEnable(GL_CULL_FACE);

   float MaxPointSize = 0;
   
   float quadratic[] =  { 0.01f, 0.01f, 0.0f };
   if( ext.hasPointTexture() )
   {
      glEnable(GL_TEXTURE_2D);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);

      glGetFloatv( GL_POINT_SIZE_MAX_ARB, &MaxPointSize );
      ext.arbPointParameterfv( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );
      glPointSize(8);
      ext.arbPointParameterf( GL_POINT_SIZE_MIN_ARB, 2.0f );
      ext.arbPointParameterf( GL_POINT_SIZE_MAX_ARB, MaxPointSize);

      glBindTexture(GL_TEXTURE_2D, partTexture);
      glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
      glEnable(GL_POINT_SPRITE_ARB);
   }
   else
   {
      glEnable(GL_COLOR);
      glPointSize(2);
   }
}

/****************************************************************************
 *                             EndRenderer                                  *
 ****************************************************************************/
void part5::endRender()
{
   glDisable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_TRUE);
   glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
   glDisable(GL_TEXTURE_2D);
   if( ext.hasPointTexture() )
   {
      glDisable(GL_POINT_SPRITE_ARB);
   }
   else
   {
      glDisable(GL_COLOR);
      glDisable(GL_POINT_SMOOTH);
   }

   glEnable(GL_LIGHTING);
   glDisable( GL_BLEND );
}

/****************************************************************************
 *                                update                                    *
 ****************************************************************************/
void part5::update(particle* part)
{
   if(part->posY <= terrainHeight )
   {
     part->status =  PARTICLE_STATUS_STATIC;
     part->posY = terrainHeight;
   }
   else
   {
      float percent = (float) part->age / (float) (maxLive-1);

      part->size -= 0.1*(rand() / ((double)RAND_MAX + 1));

      part->prvR = part->R;
      part->prvG = part->G;
      part->prvB = part->B;

      part->R = (1 - (percent) ) * initR + 
                  (percent) * finalR;
      part->G = (1 - (percent) ) * initG + 
                  (percent) * finalG; 
      part->B = (1 - (percent) ) * initB + 
                  (percent) * finalB;

      part->prvX = part->posX;
      part->prvY = part->posY;
      part->prvZ = part->posZ;
   

      part->velY += 0.02*gravity*(rand() / ((double)RAND_MAX + 1));
      part->velX += 0.02*(dMultVel[0]*((rand() / ((double)RAND_MAX + 1))) 
                      + dSumVel[0]);
      part->velZ += 0.02*(dMultVel[2]*((rand() / ((double)RAND_MAX + 1))) 
                      + dSumVel[2]);

      part->posY += 0.02*part->velY;
      part->posX += 0.02*part->velX;
      part->posZ += 0.02*part->velZ;
   }
}

/****************************************************************************
 *                             continueLive                                 *
 ****************************************************************************/
bool part5::continueLive(particle* part)
{
   return(part->age <= MAX_BLOOD_REMOVING);
}

/****************************************************************************
 *                             needCreate                                   *
 ****************************************************************************/
int part5::needCreate()
{
   if( (actualParticles < maxParticles) && (!doneCreation))
   {
      return(rand() % 30);
   }
   else
   {
      doneCreation = true;
      return(0);
   }
}

/****************************************************************************
 *                           createParticle                                 *
 ****************************************************************************/
void part5::createParticle(particle* part)
{
   part->posX = (dMultCenter[0]*(rand() / ((double)RAND_MAX + 1))+dSumCenter[0])
                + centerX;
   part->posY = (dMultCenter[1]*(rand() / ((double)RAND_MAX + 1))+dSumCenter[1])
                + alpha + centerY;
   part->posZ = (dMultCenter[2]*(rand() / ((double)RAND_MAX + 1))+dSumCenter[2])
                + centerZ;
   part->prvX = part->posX;
   part->prvY = part->posY;
   part->prvZ = part->posZ;
   part->size = 4; 
   part->velY = -1.0;
   part->velX = 1*(rand() / ((double)RAND_MAX + 1))-0.5;
   part->velZ = 1*(rand() / ((double)RAND_MAX + 1))-0.5;
   part->R = initR;
   part->G = initG;
   part->B = initB;
   part->prvR = part->R;
   part->prvG = part->G; 
   part->prvB = part->B;
}

/****************************************************************************
 *                               NextStep                                   *
 ****************************************************************************/
void part5::nextStep(GLfloat** matriz)
{
   doStep(matriz);
}

