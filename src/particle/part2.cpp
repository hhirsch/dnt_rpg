/* Fire */

#include "part2.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <GL/glext.h>

part2::part2(float cX,float cY,float cZ):
                               particleSystem(200, PARTICLE_DRAW_GROUPS)
{
   centerX = cX; 
   centerY=cY; 
   centerZ=cZ;
   actualParticles = 0;
   alpha = 1.0;
   gravity = 12;
   maxLive = 100;
   finalR = 0.27;
   finalG = 0.04;
   finalB = 0.04;
   /*initR = 1;
   initG = 0.964;
   initB = 0;*/
   initR = 0.18;
   initG = 0.18;
   initB = 0;
   partTexture = LoadTexture("../data/particles/part2.png");
   //drawSphereToList(5,5); 
}

part2::~part2()
{
   //glDeleteLists(sphereList,1);
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
   //glLineWidth(part->size);
   //glPointSize(part->size);
   //glBegin(GL_POINTS);
   glColor3f(part->R,part->G,part->B);
   glVertex3f(part->posX,part->posY,part->posZ);
   glColor3f(part->prvR,part->prvG,part->prvB);
   glVertex3f(part->prvX,part->prvY,part->prvZ);
   /*glColor4f( (0.67*part->prvR + 0.33*part->R), 
              (0.67*part->prvG + 0.33*part->G),
              (0.67*part->prvB + 0.33*part->B), alpha);
   glVertex3f((0.67*part->prvX + 0.33*part->posX),
                (0.67*part->prvY + 0.33*part->posY),
                (0.67*part->prvZ + 0.33*part->posZ));
   glColor4f( (0.33*part->prvR + 0.67*part->R), 
              (0.33*part->prvG + 0.67*part->G),
              (0.33*part->prvB + 0.67*part->B), alpha);
   glVertex3f((0.33*part->prvX + 0.67*part->posX),
                (0.33*part->prvY+ 0.67*part->posY),
                (0.33*part->prvZ + 0.67*part->posZ));*/
   //glEnd();
   /*glPushMatrix();
   glColor4f(part->R,part->G,part->B,alpha);
   glTranslatef(part->posX,part->posY,part->posZ);
   glScalef(part->size,part->size,part->size);
   glCallList(sphereList);
   glPopMatrix();
   glPushMatrix();
   glColor4f(part->prvR,part->prvG,part->prvB,alpha);
   glTranslatef(part->prvX,part->prvY,part->prvZ);
   glScalef(part->size,part->size,part->size);
   glCallList(sphereList);
   glPopMatrix();
   glPushMatrix();
   glColor4f( (0.67*part->prvR + 0.33*part->R), 
              (0.67*part->prvG + 0.33*part->G),
              (0.67*part->prvB + 0.33*part->B), alpha);
   glTranslatef((0.67*part->prvX + 0.33*part->posX),
                (0.67*part->prvY + 0.33*part->posY),
                (0.67*part->prvZ + 0.33*part->posZ));
   glScalef(part->size,part->size,part->size);
   glCallList(sphereList);
   glPopMatrix();
   glPushMatrix();
   glColor4f( (0.33*part->prvR + 0.67*part->R), 
              (0.33*part->prvG + 0.67*part->G),
              (0.33*part->prvB + 0.67*part->B), alpha);
   glTranslatef((0.33*part->prvX + 0.67*part->posX),
                (0.33*part->prvY+ 0.67*part->posY),
                (0.33*part->prvZ + 0.67*part->posZ));
   glScalef(part->size,part->size,part->size);
   glCallList(sphereList);
   glPopMatrix();*/
}

void part2::InitRender()
{
   //glPushMatrix();
   /*glDrawBuffer(GL_AUX0);
   glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);*/
   glDisable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_FALSE);
   glEnable(GL_CULL_FACE);
   //glEnable(GL_ALPHA_TEST);

   glEnable(GL_TEXTURE_2D);
   glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA); 
   glBlendFunc(GL_SRC_ALPHA, GL_ONE);
   //glBlendFunc(GL_DST_ALPHA, GL_ONE);
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
   glPointSize(32);

   glBindTexture(GL_TEXTURE_2D, partTexture);
   glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
   glEnable(GL_POINT_SPRITE_ARB);

   //glPushMatrix();
   //glBegin(GL_POINTS);
   //glEnable(GL_POINT_SMOOTH);
}

void part2::EndRender()
{
   //glEnd();
   //glPopMatrix();
   glDisable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_TRUE);
   glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_POINT_SPRITE_ARB);
   /*glDrawBuffer(GL_BACK);
   glReadBuffer(GL_AUX0);
   glCopyPixels(0,0,800,600,GL_COLOR);
   //glCopyPixels(0,0,800,600,GL_DEPTH);
   glReadBuffer(GL_BACK);*/
   glEnable(GL_LIGHTING);
   glDisable( GL_BLEND );
  
   //glPopMatrix();

   //glReadBuffer(GL_BACK);
   //glDrawBuffer(GL_BACK);
   
   //glDisable(GL_POINT_SMOOTH);
}
 
void part2::actualize(particle* part)
{
   float percent = (float) part->age / (float) (maxLive-1);

   //part->size =  (1-percent)*1 + 0.1*(rand() / ((double)RAND_MAX + 1))+1;

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

   float vXZ = 30*(1-percent) + 10*(percent);
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
   part->posX = (2*(rand() / ((double)RAND_MAX + 1))-1) +centerX;
   part->posY = (2*(rand() / ((double)RAND_MAX + 1))-1) +centerY;
   part->posZ = ((rand() / ((double)RAND_MAX + 1))) +centerZ;
   part->prvX = part->posX;
   part->prvY = part->posY;
   part->prvZ = part->posZ;
   part->size = 2; 
   part->velY = 1.0;
   part->velX = 2*(rand() / ((double)RAND_MAX + 1))-1;
   part->velZ = 2*(rand() / ((double)RAND_MAX + 1))-1;
   part->R = initR;//(0.40*(rand() / ((double)RAND_MAX + 1))) + 0.12;
   part->G = initG;//(0.22*(rand() / ((double)RAND_MAX + 1))) + 0.42;
   part->B = initB;//0.84;
   part->prvR = part->R;
   part->prvG = part->G; 
   part->prvB = part->B;
}

void part2::NextStep(float sec)
{
   seconds = 0.02;
   DoStep();
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

