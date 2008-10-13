/* Snow */

#include "part7.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL_image.h>

/****************************************************************************
 *                             Constructor                                  *
 ****************************************************************************/
part7::part7(float cX,float cY,float cZ, string fileName):
                               particleSystem(fileName,PARTICLE_DRAW_GROUPS)
{
   dirs dir;
   centerX = cX; 
   centerY=cY; 
   centerZ=cZ;
   actualParticles = 0;
   partTexture = loadTexture(dir.getRealFile("particles/snow.png"));
}

/****************************************************************************
 *                              Destructor                                  *
 ****************************************************************************/
part7::~part7()
{
   glDeleteTextures(1,&partTexture);
}

/****************************************************************************
 *                                Render                                    *
 ****************************************************************************/
void part7::render(particle* part)
{
   //Not Used!
}

/****************************************************************************
 *                             InitRender                                   *
 ****************************************************************************/
void part7::initRender()
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
      float quadratic[] =  { 0.01f, 0.01f, 0.0f };
      ext.arbPointParameterfv( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );
      ext.arbPointParameterf( GL_POINT_SIZE_MIN_ARB, 2.0f );
      ext.arbPointParameterf( GL_POINT_SIZE_MAX_ARB, MaxPointSize);
      glPointSize(8);
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
 *                              EndRender                                   *
 ****************************************************************************/
void part7::endRender()
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
void part7::update(particle* part)
{
   if(part->posY <= 0 )
   {
     part->status =  PARTICLE_STATUS_STATIC;
     part->posY = 0;
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
   

      part->velY += seconds*gravity*(rand() / ((double)RAND_MAX + 1));
      part->velX += seconds*(dMultVel[0]*((rand() / ((double)RAND_MAX + 1))) 
                      + dSumVel[0]);
      part->velZ += seconds*(dMultVel[2]*((rand() / ((double)RAND_MAX + 1))) 
                      + dSumVel[2]);

      part->posY += part->velY*seconds;
      part->posX += part->velX*seconds;
      part->posZ += part->velZ*seconds;
   }
}

/****************************************************************************
 *                             continueLive                                 *
 ****************************************************************************/
bool part7::continueLive(particle* part)
{
   return( part->age < maxLive );
}

/****************************************************************************
 *                             needCreate                                   *
 ****************************************************************************/
int part7::needCreate()
{
   return(rand() % (int)alpha);
}

/****************************************************************************
 *                            createParticle                                *
 ****************************************************************************/
void part7::createParticle(particle* part)
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
void part7::nextStep(GLfloat** matriz)
{
   seconds = 0.02;
   doStep(matriz);
}

/****************************************************************************
 *                             numParticles                                 *
 ****************************************************************************/
int part7::numParticles()
{
   return(actualParticles);
}

