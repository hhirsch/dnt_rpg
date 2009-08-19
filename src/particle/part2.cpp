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

/* Fire */

#include "part2.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

/****************************************************************************
 *                               Constuctor                                 *
 ****************************************************************************/
part2::part2(float cX,float cY,float cZ, string fileName):
                                  particleSystem(fileName, PARTICLE_DRAW_GROUPS)
{
   dirs dir;
   centerX = cX; 
   centerY = cY; 
   centerZ = cZ;
   actualParticles = 0;
   type = DNT_PARTICLE_TYPE_FIRE;
   partTexture = loadTexture(dir.getRealFile("particles/part2.png"));
}

/****************************************************************************
 *                                Destuctor                                 *
 ****************************************************************************/
part2::~part2()
{
   glDeleteTextures(1,&partTexture);
}

/****************************************************************************
 *                                 Render                                   *
 ****************************************************************************/
void part2::render(particle* part)
{
   //Not Used
}

/****************************************************************************
 *                               InitRender                                 *
 ****************************************************************************/
void part2::initRender()
{
   glDisable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_FALSE);
   glEnable(GL_CULL_FACE);

   glEnable(GL_TEXTURE_2D);
   glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA); 
   glBlendFunc(GL_SRC_ALPHA, GL_ONE);
   glEnable(GL_BLEND);
    
   float MaxPointSize = 0;
   
   float quadratic[] =  { 0.01f, 0.01f, 0.0f };
   //float quadratic[] =  { 0.0f, 0.0f, 0.011831263f };
   if( ext.hasPointTexture() )
   {
      glGetFloatv( GL_POINT_SIZE_MAX_ARB, &MaxPointSize );
      ext.arbPointParameterfv( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );

      //ext.PointParameterf( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );
      ext.arbPointParameterf( GL_POINT_SIZE_MIN_ARB, 2.0f );
      ext.arbPointParameterf( GL_POINT_SIZE_MAX_ARB, MaxPointSize);

      glPointSize(32);

      glBindTexture(GL_TEXTURE_2D, partTexture);
      glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
      glEnable(GL_POINT_SPRITE_ARB);
   }
   else
   {
      glEnable(GL_COLOR);
      glPointSize(8);
   }
}

/****************************************************************************
 *                               EndRenderer                                *
 ****************************************************************************/
void part2::endRender()
{
   glDisable(GL_COLOR_MATERIAL);
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
 *                                 update                                   *
 ****************************************************************************/
void part2::update(particle* part)
{
   float percent = (float) part->age / (float) (maxLive-1);

   part->size =  (1-percent)*5 + 0.1*(rand() / ((double)RAND_MAX + 1))+2;

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
   

   part->velY += seconds*gravity*(rand() / ((double)RAND_MAX + 1));

   float vXZ = dMultPos[0]*(1-percent) + dMultPos[2]*(percent);
   float vXZ2 = vXZ / 2.0;

   part->velX += seconds*(vXZ*((rand() / ((double)RAND_MAX + 1))) - vXZ2);
   part->velZ += seconds*(vXZ*((rand() / ((double)RAND_MAX + 1))) - vXZ2);

   part->posY += part->velY*seconds;
   part->posX += part->velX*seconds;
   part->posZ += part->velZ*seconds;
}

/****************************************************************************
 *                              continueLive                                *
 ****************************************************************************/
bool part2::continueLive(particle* part)
{
   return( (part->age < maxLive) &&
           ((rand() % 200) != 5) );
}

/****************************************************************************
 *                               needCreate                                 *
 ****************************************************************************/
int part2::needCreate()
{
   return(rand() % 80);
}

/****************************************************************************
 *                             createParticle                               *
 ****************************************************************************/
void part2::createParticle(particle* part)
{
   part->posX = (dMultCenter[0]*(rand() / ((double)RAND_MAX + 1))+dSumCenter[0])
                + centerX;
   part->posY = (dMultCenter[1]*(rand() / ((double)RAND_MAX + 1))+dSumCenter[1])
                + centerY;
   part->posZ = (dMultCenter[2]*(rand() / ((double)RAND_MAX + 1))+dSumCenter[2])
                + centerZ;
   part->prvX = part->posX;
   part->prvY = part->posY;
   part->prvZ = part->posZ;
   part->size = 2; 
   part->velY = 1.0;
   part->velX = dMultVel[0]*(rand() / ((double)RAND_MAX + 1))+dSumVel[0];
   part->velZ = dMultVel[2]*(rand() / ((double)RAND_MAX + 1))+dSumVel[2];
   part->R = initR;//(0.40*(rand() / ((double)RAND_MAX + 1))) + 0.12;
   part->G = initG;//(0.22*(rand() / ((double)RAND_MAX + 1))) + 0.42;
   part->B = initB;//0.84;
   part->prvR = part->R;
   part->prvG = part->G; 
   part->prvB = part->B;
}

/****************************************************************************
 *                                NextStep                                  *
 ****************************************************************************/
void part2::nextStep(GLfloat** matriz)
{
   seconds = 0.02;
   doStep(matriz);
}

