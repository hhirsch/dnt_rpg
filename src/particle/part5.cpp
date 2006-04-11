/* Blood */

#include "part5.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <SDL/SDL_image.h>

part5::part5(float cX,float cY,float cZ):
                               particleSystem(100,PARTICLE_DRAW_GROUPS)
{
   centerX = cX; 
   centerY=cY; 
   centerZ=cZ;
   actualParticles = 0;
   alpha = 1.0;
   gravity = -20;
   maxLive = 100;
   finalR = 1.0;
   finalG = 0;
   finalB = 0;
   initR = 0.6;
   initG = 0.2;
   initB = 0.2;
   partTexture = LoadTexture("../data/particles/part2.png");
   //drawSphereToList(3.0,6,6); 
}

part5::~part5()
{
   glDeleteTextures(1,&partTexture);
   //glDeleteLists(sphereList,1);
}

void part5::drawSphereToList(double r, int lats, int longs) 
{
   sphereList = glGenLists(1);
   glNewList(sphereList,GL_COMPILE);
   glScalef(r,r,r);
   int i, j;
   for(i = 0; i <= lats; i++) {
      double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
      double z0  = sin(lat0);
      double zr0 =  cos(lat0);
    
      double lat1 = M_PI * (-0.5 + (double) i / lats);
      double z1 = sin(lat1);
      double zr1 = cos(lat1);
 
      glBegin(GL_QUAD_STRIP);
      for(j = 0; j <= longs; j++) {
         double lng = 2 * M_PI * (double) (j - 1) / longs;
         double x = cos(lng);
         double y = sin(lng);
 
         glNormal3f(x * zr0, y * zr0, z0);
         glVertex3f(x * zr0, y * zr0, z0);
         glNormal3f(x * zr1, y * zr1, z1);
         glVertex3f(x * zr1, y * zr1, z1);
      }
      glEnd();
   }
   glEndList();
}

void part5::Render(particle* part)
{
}

void part5::InitRender()
{
  /* glBlendFunc( GL_SRC_ALPHA, GL_ONE );
   glBlendFunc( GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA );*/
  glDisable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_FALSE);
   glEnable(GL_CULL_FACE);
   //glEnable(GL_ALPHA_TEST);

   glEnable(GL_TEXTURE_2D);
   //glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   //glBlendFunc(GL_DST_ALPHA, GL_ZERO);
   glEnable(GL_BLEND);
    
   float MaxPointSize;
   glGetFloatv( GL_POINT_SIZE_MAX_ARB, &MaxPointSize );

   float quadratic[] =  { 0.01f, 0.01f, 0.0f };
   PointParameterfv( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );

   PointParameterf( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );
   PointParameterf( GL_POINT_SIZE_MIN_ARB, 2.0f );
   PointParameterf( GL_POINT_SIZE_MAX_ARB, MaxPointSize);

   //glBlendFunc(GL_DST_ALPHA, GL_SRC_ALPHA);
   //glBlendFunc( GL_DST_ALPHA, GL_ZERO_MINUS_SRC_ALPHA );
   //glEnable( GL_BLEND );
   /*glBlendFunc( GL_SRC_ALPHA, GL_ONE );
   glBlendFunc( GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
   glMaterialf(GL_FRONT_AND_BACK, GL_DIFFUSE, 50);*/
   glPointSize(8);

   glBindTexture(GL_TEXTURE_2D, partTexture);
   glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
   glEnable(GL_POINT_SPRITE_ARB);
}

void part5::EndRender()
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

void part5::actualize(particle* part)
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

      if(part->posY >= 5)
        part->velX += seconds*(gravity/2.0)*(((rand() / ((double)RAND_MAX + 1))));
      else
        part->velX += seconds*(8*((rand() / ((double)RAND_MAX + 1))) - 4);
      part->velZ += seconds*(8*((rand() / ((double)RAND_MAX + 1))) - 4);

      part->posY += part->velY*seconds;
      part->posX += part->velX*seconds;
      part->posZ += part->velZ*seconds;
   }
}

bool part5::continueLive(particle* part)
{
   return( /*(part->age < maxLive) &&
           ((rand() % 300) != 5)*/ true );
}

int part5::needCreate()
{
   return(rand() % 100);
}

void part5::createParticle(particle* part)
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
   part->R = initR;//(0.40*(rand() / ((double)RAND_MAX + 1))) + 0.12;
   part->G = initG;//(0.22*(rand() / ((double)RAND_MAX + 1))) + 0.42;
   part->B = initB;//0.84;
   part->prvR = part->R;
   part->prvG = part->G; 
   part->prvB = part->B;
}

void part5::NextStep(float sec)
{
   seconds = 0.02;
   DoStep();
}

int part5::numParticles()
{
   return(actualParticles);
}

GLuint part5::LoadTexture(char* fileName)
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

