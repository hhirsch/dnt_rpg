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
   actualPlanes = 0;
   partTexture = LoadTexture("../data/particles/part2.png");
}

part1::~part1()
{
   glDeleteTextures(1,&partTexture);
}

void part1::Render(particle* part)
{
}

void part1::InitRender()
{
   glDisable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_FALSE);
   glEnable(GL_CULL_FACE);

       
   float MaxPointSize = 0;
   float quadratic[] =  { 0.01f, 0.01f, 0.0f };

   if( (PointParameterf != NULL) && (PointParameterfv != NULL) )
   {
      glEnable(GL_TEXTURE_2D);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);

      glGetFloatv( GL_POINT_SIZE_MAX_ARB, &MaxPointSize );
      PointParameterfv( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );

      PointParameterf( GL_POINT_SIZE_MIN_ARB, 2.0f );
      PointParameterf( GL_POINT_SIZE_MAX_ARB, MaxPointSize);
   
      glPointSize(4);

      glBindTexture(GL_TEXTURE_2D, partTexture);
      glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
      glEnable(GL_POINT_SPRITE_ARB);
   }
   else
   {
      glEnable(GL_COLOR);
      glPointSize(1);
      /*glEnable(GL_POINT_SMOOTH);
      glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);*/
   }
}

void part1::EndRender()
{
   glDisable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_TRUE);
   glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
   glDisable(GL_TEXTURE_2D);
   if( (PointParameterf != NULL) && (PointParameterfv != NULL) )
   {
      glDisable(GL_POINT_SPRITE_ARB);
   }
   else
   {
      glDisable(GL_COLOR);
      glDisable(GL_POINT_SMOOTH);
   }
   glDisable( GL_BLEND );

   glEnable(GL_LIGHTING);
}

void part1::actualize(particle* part)
{
   float dX = -1.0;   
   float dZ = 0.0;

   if( (part->posY <= 0) || intersectPlanes(part,&dX,&dZ) )
   {
       part->velY = -0.02*part->velY+(-gravity)*seconds;

       part->velX += dX*part->velY*seconds*gravity*((rand() / ((double)RAND_MAX + 1)));
       part->velZ += dZ*part->velY*seconds*gravity*((rand() / ((double)RAND_MAX + 1)));

      part->prvX = part->posX;
      part->prvY = part->posY;
      part->prvZ = part->posZ;
      part->posY += part->velY*seconds;
      part->posX += part->velX*seconds;
      part->posZ += part->velZ*seconds;
   }
   else
   {
      part->velY += seconds*gravity;

      part->velX += seconds*(4*((rand() / ((double)RAND_MAX + 1))) - 2);
      part->velZ += seconds*(4*((rand() / ((double)RAND_MAX + 1))) - 2);

      part->prvX = part->posX;
      part->prvY = part->posY;
      part->prvZ = part->posZ;
      part->posY += part->velY*seconds;
      part->posX += part->velX*seconds;
      part->posZ += part->velZ*seconds;
   }
}

bool part1::continueLive(particle* part)
{
   return( (part->age < maxLive) /*&&
           ((rand() % 200) != 5)*/ );
}

int part1::needCreate()
{
   return(rand() % (int) (maxParticles / maxLive));
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

void part1::NextStep(GLfloat matriz[6][4])
{
   seconds = 0.02;
   DoStep(matriz);
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

int part1::addPlane(float x1, float y1, float z1, 
                    float x2, float y2, float z2,
                    float dX, float dZ, int inclination)
{
   if(actualPlanes < PART1_MAX_PLANES)
   {
      intersections[actualPlanes].x1 = x1;
      intersections[actualPlanes].y1 = y1;
      intersections[actualPlanes].z1 = z1;
      intersections[actualPlanes].x2 = x2;
      intersections[actualPlanes].y2 = y2;
      intersections[actualPlanes].z2 = z2;
      intersections[actualPlanes].dX = dX;
      intersections[actualPlanes].dZ = dZ;
      intersections[actualPlanes].inclination = inclination;
      actualPlanes++;
      return(actualPlanes-1);
   }
   return(-1);
}


bool part1::intersectPlanes(particle* part, float* dX, float* dZ)
{
   int i;
   float yOnPlane = 0;
   float size;
   for(i = 0; i < actualPlanes; i++)
   {
      if( (part->posX <= intersections[i].x2) && 
          (part->posX >= intersections[i].x1) &&
          (part->posZ <= intersections[i].z2) && 
          (part->posZ >= intersections[i].z1) )
      {
         switch(intersections[i].inclination)
         { 
             case PLANE_NO_INCLINATION: 
                   yOnPlane = intersections[i].y1;
             break;
             case PLANE_INCLINATION_X:
             {
                size = (intersections[i].x2 - intersections[i].x1);
                yOnPlane = ((intersections[i].x2 - part->posX) / size) * 
                             intersections[i].y1 +
                           ((part->posX - intersections[i].x1) / size) * 
                             intersections[i].y2;
             }
             break;
             case PLANE_INCLINATION_Z:
             {
                size = (intersections[i].z2 - intersections[i].z1);
                yOnPlane = ((intersections[i].z2 - part->posZ) / size) * 
                             intersections[i].y1 +
                           ((part->posZ - intersections[i].z1) / size) * 
                             intersections[i].y2;
             }
             break;
         }
         if( ((part->posY >= yOnPlane - 1) && (part->posY <= yOnPlane + 1)) )
             
         {
            *dX = intersections[i].dX;
            *dZ = intersections[i].dZ;
            return(true);
         }
         /*else if (((part->posY <= yOnPlane +1) && (part->prvY >= yOnPlane -1) ))
         {
            printf("Alguem\n");
            part->posY = yOnPlane;
            *dX = intersections[i].dX;
            *dZ = intersections[i].dZ;
            return(true);
         }*/
         /*else
         { printf("prv: %.3f act: %.3f onP: %.3f",part->prvY,part->posY,yOnPlane); }*/
      }
   }
   return(false);
}

void part1::removeCharacterPlanes()
{
   actualPlanes -= 4;
}

void part1::removeLastPlane()
{
   actualPlanes--;
}

