/* Waterfall */

#include "part1.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL_image.h>

part1::part1(float cX,float cY,float cZ, string fileName):
                              particleSystem(fileName, PARTICLE_DRAW_GROUPS)
{
   centerX = cX; 
   centerY = cY; 
   centerZ = cZ;
   actualParticles = 0;
   partTexture = LoadTexture("../data/particles/part2.png");
}

part1::~part1()
{
   glDeleteTextures(1,&partTexture);
}

void part1::Render(particle* part)
{
  /* glColor4f(part->R,part->G,part->B,alpha);
   glVertex3f(part->prvX,part->prvY,part->prvZ);
   glVertex3f(part->posX,part->posY,part->posZ);*/
}

void part1::InitRender()
{
/*   glDisable(GL_LIGHTING);
   glDisable(GL_TEXTURE_2D);
   glEnable( GL_BLEND );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE );
   glBlendFunc( GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
   //glEnable(GL_LINE_SMOOTH);
   //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
   glLineWidth(1.0);
   glBegin(GL_LINES);*/

   glDisable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_FALSE);
   glEnable(GL_CULL_FACE);

   glEnable(GL_TEXTURE_2D);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
    
   float MaxPointSize;
   glGetFloatv( GL_POINT_SIZE_MAX_ARB, &MaxPointSize );

   float quadratic[] =  { 0.01f, 0.01f, 0.0f };
   //float quadratic[] =  { 0.0f, 0.0f, 0.011831263f };
   PointParameterfv( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );

   //PointParameterf( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );
   PointParameterf( GL_POINT_SIZE_MIN_ARB, 2.0f );
   PointParameterf( GL_POINT_SIZE_MAX_ARB, MaxPointSize);

   glPointSize(4);

   glBindTexture(GL_TEXTURE_2D, partTexture);
   glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
   glEnable(GL_POINT_SPRITE_ARB);
}

void part1::EndRender()
{
  /* glEnd();
   glEnable(GL_LIGHTING);
   glDisable( GL_BLEND ); 
   glDisable(GL_LINE_SMOOTH);*/
   glDisable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_TRUE);
   glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_POINT_SPRITE_ARB);
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

GLuint part1::LoadTexture(char* fileName)
{
   GLuint indice;
   SDL_Surface* img = IMG_Load(fileName);

   glGenTextures(1, &(indice));
   glBindTexture(GL_TEXTURE_2D, indice);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   SDL_FreeSurface(img);
   return(indice);
}

