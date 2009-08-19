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

/* Lightning */

#include "part6.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL_image.h>

#define LIGHTINING_DURATION 120 /**< Duration of the Lightning */

/****************************************************************************
 *                             Constructor                                  *
 ****************************************************************************/
part6::part6(float cX,float cY,float cZ, string fileName):
                               particleSystem(fileName,PARTICLE_DRAW_GROUPS)
{
   dirs dir;
   centerX = cX; 
   centerY = cY; 
   centerZ = cZ;
   actualY = centerY;
   actualParticles = 0;
   type = DNT_PARTICLE_TYPE_LIGHTNING;
   partTexture = loadTexture(dir.getRealFile("particles/part2.png"));
   setDurationTime(LIGHTINING_DURATION);
}

/****************************************************************************
 *                              Destructor                                  *
 ****************************************************************************/
part6::~part6()
{
   glDeleteTextures(1,&partTexture);
}

/****************************************************************************
 *                                Render                                    *
 ****************************************************************************/
void part6::render(particle* part)
{
}

/****************************************************************************
 *                              InitRender                                  *
 ****************************************************************************/
void part6::initRender()
{
  glDisable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_FALSE);
   glEnable(GL_CULL_FACE);

   float MaxPointSize = 0;

   if( ext.hasPointTexture() )
   {
      glEnable(GL_TEXTURE_2D);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);

      glGetFloatv( GL_POINT_SIZE_MAX_ARB, &MaxPointSize );
      glPointSize(16);
      ext.arbPointParameterf( GL_POINT_SIZE_MIN_ARB, 5.0 );
      ext.arbPointParameterf( GL_POINT_SIZE_MAX_ARB, MaxPointSize);

      glBindTexture(GL_TEXTURE_2D, partTexture);
      glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
      glEnable(GL_POINT_SPRITE_ARB);
   }
   else
   {
      glEnable(GL_COLOR);
      glPointSize(4);
   }
}

/****************************************************************************
 *                              EndRender                                   *
 ****************************************************************************/
void part6::endRender()
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
void part6::update(particle* part)
{
   //Not Used!
}

/****************************************************************************
 *                             continueLive                                 *
 ****************************************************************************/
bool part6::continueLive(particle* part)
{
   return( true );
}

/****************************************************************************
 *                             needCreate                                   *
 ****************************************************************************/
int part6::needCreate()
{
   if(actualY <= 0 )
      return(0);
   else
      return((int)gravity);
}

/****************************************************************************
 *                            createParticle                                *
 ****************************************************************************/
void part6::createParticle(particle* part)
{
   dSumVel[0] += (2*(rand() / ((double)RAND_MAX + 1))) - 1;
   part->posX = dSumVel[0] + centerX;
   part->posY = actualY;
   dSumVel[2] += (2*(rand() / ((double)RAND_MAX + 1))) - 1;
   part->posZ = dSumVel[2] + centerZ;
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
   part->status =  PARTICLE_STATUS_STATIC;
   actualY+=(-1*dSumVel[1]);
}

/****************************************************************************
 *                               NextStep                                   *
 ****************************************************************************/
void part6::nextStep(GLfloat** matriz)
{
   seconds = 0.02;
   doStep(matriz);
}

