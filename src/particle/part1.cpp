/* Waterfall */

#include "part1.h"
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
part1::part1(float cX,float cY,float cZ):
                               particleSystem(4000, PARTICLE_DRAW_INDIVIDUAL)
{
   centerX = cX; 
   centerY=cY; 
   centerZ=cZ;
   actualParticles = 0;
   alpha = 0.6;
   gravity = -20;
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
   //glLineWidth(10);
   glBegin(GL_LINES);
   //glMaterialf(GL_FRONT_AND_BACK, GL_DIFFUSE, 50);
}

void part1::EndRender()
{
   glEnd();
   glEnable(GL_LIGHTING);
   glDisable( GL_BLEND );
}

void part1::actualize(particle* part)
{

   //centerX += 0.50*((rand() / ((double)RAND_MAX + 1)))-0.25;

   part->R -= 0.005*((rand() / ((double)RAND_MAX + 1)));
   /*part->G -= rand() % 3; 
   part->B += rand() % 2;*/

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

      //if(part->velX > 0)
      //{
         part->velX += -1*part->velY*seconds*gravity*((rand() / ((double)RAND_MAX + 1)));
      /*}
      else
      {
         part->velX += gravity*seconds*((rand() / ((double)RAND_MAX + 1)));
      }
   
      if(part->velZ > 0)
      {
         part->velZ += -gravity*seconds*((rand() / ((double)RAND_MAX + 1)));
      }
      else
      {
         part->velZ += gravity*seconds*((rand() / ((double)RAND_MAX + 1)));
      }*/


      part->posY += part->velY*seconds;
      part->posX += part->velX*seconds;
      part->posZ += part->velZ*seconds;
   }
   else
   {
      part->velY += seconds*gravity;

      part->velX += seconds*(4*((rand() / ((double)RAND_MAX + 1))) - 2);
      part->velZ += seconds*(4*((rand() / ((double)RAND_MAX + 1))) - 2);

      //part->posX += part->velX;
      part->posY += part->velY*seconds;
      part->posX += part->velX*seconds;
      part->posZ += part->velZ*seconds;
   }
   //part->posZ += part->velZ;
}

bool part1::continueLive(particle* part)
{
   return( (part->age < 500) /*&& (part->posY > 0)*/ &&
           ((rand() % 200) != 5) );
}

int part1::needCreate()
{
   return(rand() % 120);
}

void part1::createParticle(particle* part)
{
   part->posX = (2 * (rand() / ((double)RAND_MAX + 1))) +centerX;
   part->posY = /*(2 * (rand() / ((double)RAND_MAX + 1))) +*/centerY;
   part->posZ = (2 * (rand() / ((double)RAND_MAX + 1))) +centerZ;
   part->prvX = part->posX;
   part->prvY = part->posY;
   part->prvZ = part->posZ;
   part->velY = -1.0;
   part->velX = (rand() / ((double)RAND_MAX + 1))-0.5;
   part->velZ = (rand() / ((double)RAND_MAX + 1))-0.5;
   part->R = (0.1*(rand() / ((double)RAND_MAX + 1)))+0.8;
   part->G = (0.1*(rand() / ((double)RAND_MAX + 1)))+0.8;
   part->B = 0.8;
}

void part1::NextStep(float sec)
{
   //gravity = -sec;
   seconds = 0.02;//sec;
   DoStep();
}

int part1::numParticles()
{
   return(actualParticles);
}

