/* Smoke */

#include "part4.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL_image.h>

part4::part4(float cX,float cY,float cZ):
                               particleSystem(50,PARTICLE_DRAW_GROUPS)
{
   centerX = cX; 
   centerY=cY; 
   centerZ=cZ;
   actualParticles = 0;
   alpha = 1.0;
   gravity = 20;
   maxLive = 100;
   finalR = 1.0;
   finalG = 1.0;
   finalB = 1.0;
   initR = 0;
   initG = 0;
   initB = 0;
   partTexture = LoadTexture("../data/particles/part5.png");
}

part4::~part4()
{
   glDeleteTextures(1,&partTexture);
}

void part4::Render(particle* part)
{
}

void part4::InitRender()
{
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
   PointParameterfv( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );

   PointParameterf( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );
   PointParameterf( GL_POINT_SIZE_MIN_ARB, 2.0f );
   PointParameterf( GL_POINT_SIZE_MAX_ARB, MaxPointSize);

   glPointSize(32);

   glBindTexture(GL_TEXTURE_2D, partTexture);
   glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
   glEnable(GL_POINT_SPRITE_ARB);
}

void part4::EndRender()
{
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

void part4::actualize(particle* part)
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

   if(part->posY >= 5)
     part->velX += seconds*(gravity/2.0)*(((rand() / ((double)RAND_MAX + 1))));
   else
     part->velX += seconds*(8*((rand() / ((double)RAND_MAX + 1))) - 4);
   part->velZ += seconds*(8*((rand() / ((double)RAND_MAX + 1))) - 4);

   part->posY += part->velY*seconds;
   part->posX += part->velX*seconds;
   part->posZ += part->velZ*seconds;
}

bool part4::continueLive(particle* part)
{
   return( (part->age < maxLive) &&
           ((rand() % 300) != 5) );
}

int part4::needCreate()
{
   return(rand() % 100);
}

void part4::createParticle(particle* part)
{
   part->posX = (0.20*(rand() / ((double)RAND_MAX + 1))+0.10) +centerX;
   part->posY = (0.20*(rand() / ((double)RAND_MAX + 1))+0.10) +centerY;
   part->posZ = (0.20*(rand() / ((double)RAND_MAX + 1))+0.10) +centerZ;
   part->prvX = part->posX;
   part->prvY = part->posY;
   part->prvZ = part->posZ;
   part->size = 4; 
   part->velY = 1.0;
   part->velX = 1*(rand() / ((double)RAND_MAX + 1))-0.5;
   part->velZ = 1*(rand() / ((double)RAND_MAX + 1))-0.5;
   part->R = initR;
   part->G = initG;
   part->B = initB;
   part->prvR = part->R;
   part->prvG = part->G; 
   part->prvB = part->B;
}

void part4::NextStep(float sec)
{
   seconds = 0.02;
   DoStep();
}

int part4::numParticles()
{
   return(actualParticles);
}

GLuint part4::LoadTexture(char* fileName)
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

