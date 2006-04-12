/* Water Surface */

#include "part3.h"
#include <stdlib.h>
#include <stdio.h>

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
}

part3::~part3()
{
}

void part3::Render(particle* part)
{
   glColor4f(part->R,part->G,part->B,alpha);
   glVertex3f(part->posX,
              part->posY,
              part->posZ);
   glNormal3f(0,1,0);
}

void part3::InitRender()
{
   glDisable(GL_LIGHTING);
   glDisable(GL_TEXTURE_2D);
   glEnable( GL_BLEND );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE );
   glBlendFunc( GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
   glBegin(GL_QUAD_STRIP);
}

void part3::EndRender()
{
   glEnd();
   glEnable(GL_LIGHTING);
   glDisable( GL_BLEND );
}

void part3::actualize(particle* part)
{
   /*float aux = part->prvY;
   part->prvY = part->posY;*/
   int next = part->internalNumber+2 ;
   if(part->internalNumber == 0)
   {
     if(time == 6)
     {
        part->prvY = ((rand() / ((double)RAND_MAX + 1))); 
        time = 1;
     }
     else
     {
        time++;
     }
     if(time >= 3)
        part->posY = (time/3.0)*part->prvY + centerY;
     else
        part->posY = ((3-(time-3))/3.0)*part->prvY + centerY;
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

bool part3::continueLive(particle* part)
{
   return( true );
}

int part3::needCreate()
{
   if(actualParticles == 0)
      return(200);
   else
      return(0);
}

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
      centerX += 2;
      
      if( (actualParticles % 4 == 0))
      {
        centerY -= 5;
        otherY += 5;
      }
      else
      {
        centerY += 5;
        otherY -= 5;
      }
   }
   else
   {
      centerZ += 10;
   }
}

void part3::NextStep(float sec)
{
   seconds = 0.02;
   DoStep();
}

int part3::numParticles()
{
   return(actualParticles);
}

