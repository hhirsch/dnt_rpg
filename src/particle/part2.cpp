/* Fire */

#include "part2.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

part2::part2(float cX,float cY,float cZ, string fileName):
                               particleSystem(fileName, PARTICLE_DRAW_GROUPS)
{
   centerX = cX; 
   centerY = cY; 
   centerZ = cZ;
   actualParticles = 0;
   partTexture = LoadTexture("../data/particles/part2.png");
}

part2::~part2()
{
   glDeleteTextures(1,&partTexture);
}

void part2::drawSphereToList(int lats, int longs) 
{
   sphereList = glGenLists(1);
   glNewList(sphereList,GL_COMPILE);
   int i, j;
   for(i = 0; i <= lats; i++) {
      double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
      double z0  = sin(lat0);
      double zr0 =  cos(lat0);
    
      double lat1 = M_PI * (-0.5 + (double) i / lats);
      double z1 = sin(lat1);
      double zr1 = cos(lat1);
 
      glBegin(GL_QUAD_STRIP);
      for(j = 0; j <= longs; j++) 
      {
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

void part2::Render(particle* part)
{
   glColor3f(part->R,part->G,part->B);
   glVertex3f(part->posX,part->posY,part->posZ);
   glColor3f(part->prvR,part->prvG,part->prvB);
   glVertex3f(part->prvX,part->prvY,part->prvZ);
}

void part2::InitRender()
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
    
   float MaxPointSize;
   glGetFloatv( GL_POINT_SIZE_MAX_ARB, &MaxPointSize );

   float quadratic[] =  { 0.01f, 0.01f, 0.0f };
   //float quadratic[] =  { 0.0f, 0.0f, 0.011831263f };
   PointParameterfv( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );

   //PointParameterf( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );
   PointParameterf( GL_POINT_SIZE_MIN_ARB, 2.0f );
   PointParameterf( GL_POINT_SIZE_MAX_ARB, MaxPointSize);

   glPointSize(32);

   glBindTexture(GL_TEXTURE_2D, partTexture);
   glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
   glEnable(GL_POINT_SPRITE_ARB);

}

void part2::EndRender()
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
 
void part2::actualize(particle* part)
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

bool part2::continueLive(particle* part)
{
   return( (part->age < maxLive) &&
           ((rand() % 200) != 5) );
}

int part2::needCreate()
{
   return(rand() % 80);
}

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

void part2::NextStep(GLfloat matriz[6][4])
{
   seconds = 0.02;
   DoStep(matriz);
}

int part2::numParticles()
{
   return(actualParticles);
}

GLuint part2::LoadTexture(char* fileName)
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

