/* Waterfall */

#include "part1.h"
#include <stdlib.h>
#include <stdio.h>

part1::part1(float cX,float cY,float cZ, string fileName):
                              particleSystem(fileName, PARTICLE_DRAW_INDIVIDUAL)
{
   centerX = cX; 
   centerY = cY; 
   centerZ = cZ;
   actualParticles = 0;
}

part1::~part1()
{
}

void part1::Render(particle* part)
{
   glColor4f(part->R,part->G,part->B,alpha);
   glVertex3f(part->prvX,part->prvY,part->prvZ);
   glVertex3f(part->posX,part->posY,part->posZ);
}

void part1::InitRender()
{
   glDisable(GL_LIGHTING);
   glDisable(GL_TEXTURE_2D);
   glEnable( GL_BLEND );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE );
   glBlendFunc( GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
   glBegin(GL_LINES);
}

void part1::EndRender()
{
   glEnd();
   glEnable(GL_LIGHTING);
   glDisable( GL_BLEND );
}

void part1::actualize(particle* part)
{

   //part->R -= 0.005*((rand() / ((double)RAND_MAX + 1)));

   part->prvX = part->posX;
   part->prvY = part->posY;
   part->prvZ = part->posZ;
   

   if( (part->posY <= 0) || ( (part->posY <= centerY-20) && 
                              (part->posY >= centerY-25) && 
                              (part->posX >= centerX-2)  &&
                              (part->posX <= centerX+3)  && 
                              (part->posZ >= centerZ-2)  &&
                              (part->posZ <= centerZ+3) ) )
   {
       part->velY = -0.2*part->velY+(-gravity)*seconds;

       part->velX += -1*part->velY*seconds*gravity*((rand() / ((double)RAND_MAX + 1)));

      part->posY += part->velY*seconds;
      part->posX += part->velX*seconds;
      part->posZ += part->velZ*seconds;
   }
   else
   {
      part->velY += seconds*gravity;

      part->velX += seconds*(4*((rand() / ((double)RAND_MAX + 1))) - 2);
      part->velZ += seconds*(4*((rand() / ((double)RAND_MAX + 1))) - 2);

      part->posY += part->velY*seconds;
      part->posX += part->velX*seconds;
      part->posZ += part->velZ*seconds;
   }
}

bool part1::continueLive(particle* part)
{
   return( (part->age < maxLive) /*&& (part->posY > 0)*/ &&
           ((rand() % 200) != 5) );
}

int part1::needCreate()
{
   return(rand() % 120);
}

void part1::createParticle(particle* part)
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
   part->velY = -1.0;
   part->velX = dMultVel[0]*(rand() / ((double)RAND_MAX + 1))+dSumVel[0];
   part->velZ = dMultVel[2]*(rand() / ((double)RAND_MAX + 1))+dSumVel[2];
   part->R = (0.1*(rand() / ((double)RAND_MAX + 1)))+initR;
   part->G = (0.1*(rand() / ((double)RAND_MAX + 1)))+initG;
   part->B = initB;
}

void part1::NextStep(float sec)
{
   seconds = 0.02;
   DoStep();
}

int part1::numParticles()
{
   return(actualParticles);
}

