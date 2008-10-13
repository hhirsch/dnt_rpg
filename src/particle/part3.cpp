/* Water Surface */

#include "part3.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL_image.h>

/****************************************************************************
 *                              Constructor                                 *
 ****************************************************************************/
part3::part3(float cX,float cY,float cZ):
                                    particleSystem(150,PARTICLE_DRAW_INDIVIDUAL)
{
   centerX = cX; 
   centerY = cY; 
   centerZ = cZ;
   otherY = cY + 5;
   actualParticles = 0;
   alpha = 0.6;
   gravity = -20;
   time = -1;
   xCoord = 0;
   yCoord = 0;
}

/****************************************************************************
 *                                Destuctor                                 *
 ****************************************************************************/
part3::~part3()
{
}

/****************************************************************************
 *                                 Render                                   *
 ****************************************************************************/
void part3::render(particle* part)
{
   glColor4f(part->R,part->G,part->B,alpha);
   glVertex3f(part->posX,
              part->posY,
              part->posZ);
   glNormal3f(0,1,0);
}

/****************************************************************************
 *                               InitRender                                 *
 ****************************************************************************/
void part3::initRender()
{
   glDisable(GL_LIGHTING);
   glEnable( GL_BLEND );
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_QUAD_STRIP);
}

/****************************************************************************
 *                               EndRender                                  *
 ****************************************************************************/
void part3::endRender()
{
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_LIGHTING);
   glDisable( GL_BLEND );
}

/****************************************************************************
 *                                 update                                   *
 ****************************************************************************/
void part3::update(particle* part)
{
   int next = part->internalNumber+2 ;
   if(part->internalNumber == 0)
   {
     if(time == 10)
     {
        part->prvY = 2 + 0.1*((rand() / ((double)RAND_MAX + 1))); 
        time = 1;
     }
     else
     {
        time++;
     }
     if(time <= 5)
        part->posY = (time/5.0)*part->prvY + centerY;
     else
        part->posY = ((5-(time-5))/5.0)*part->prvY + centerY;
     particles[part->internalNumber+1].posY = part->posY;
     particles[next].posY = part->posY;  
   }
   else
   {
      if(next < maxParticles )
      {
         particles[next].posY = part->prvY;
         part->prvY = part->posY;
      }
   }
}

/****************************************************************************
 *                              continueLive                                *
 ****************************************************************************/
bool part3::continueLive(particle* part)
{
   return( true );
}

/****************************************************************************
 *                               needCreate                                 *
 ****************************************************************************/
int part3::needCreate()
{
   if(actualParticles == 0)
      return(200);
   else
      return(0);
}

/****************************************************************************
 *                             createParticle                               *
 ****************************************************************************/
void part3::createParticle(particle* part)
{
   part->posX = centerX;
   part->posZ = centerZ;
   part->prvX = part->posX;
   part->prvZ = part->posZ;
   part->posY = centerY;
   part->prvY = 0;
   part->velY = -1.0;
   part->velX = 0.0;
   part->velZ = 0.0;
   part->R = 0.21;
   part->G = 0.50;
   part->B = 0.80;
   if((actualParticles % 2) == 0 )
   {
      centerZ -= 10;
      centerX += 10;
   }
   else
   {
      centerZ += 10;
   }
}

/****************************************************************************
 *                                NextStep                                  *
 ****************************************************************************/
void part3::nextStep(GLfloat** matriz)
{
   seconds = 0.02;
   doStep(matriz);
}

/****************************************************************************
 *                              numParticles                                *
 ****************************************************************************/
int part3::numParticles()
{
   return(actualParticles);
}

