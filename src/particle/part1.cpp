/* Waterfall */

#include "part1.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL_image.h>

/****************************************************************************
 *                               Constuctor                                 *
 ****************************************************************************/
part1::part1(float cX,float cY,float cZ, string fileName):
                              particleSystem(fileName, PARTICLE_DRAW_GROUPS)
{
   dirs dir;
   centerX = cX; 
   centerY = cY; 
   centerZ = cZ;
   actualParticles = 0;
   actualPlanes = 0;
   intersections = NULL;
   partTexture = LoadTexture(dir.getRealFile("particles/water.png"));
}

/****************************************************************************
 *                               Destructor                                 *
 ****************************************************************************/
part1::~part1()
{
   glDeleteTextures(1,&partTexture);
}

/****************************************************************************
 *                                 Render                                   *
 ****************************************************************************/
void part1::Render(particle* part)
{
   //Not Used    
}

/****************************************************************************
 *                               InitRender                                 *
 ****************************************************************************/
void part1::InitRender()
{
  glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_FALSE);
  glEnable(GL_CULL_FACE);

  float MaxPointSize = 0;
  float quadratic[] =  { 0.01f, 0.01f, 0.0f };

  if( (ext.PointParameterf != NULL) && (ext.PointParameterfv != NULL) )
  {
     glEnable(GL_TEXTURE_2D);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

     glEnable(GL_BLEND);
     glGetFloatv( GL_POINT_SIZE_MAX_ARB, &MaxPointSize );
     ext.PointParameterfv( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );
     ext.PointParameterf( GL_POINT_SIZE_MIN_ARB, 2.0f );
     ext.PointParameterf( GL_POINT_SIZE_MAX_ARB, MaxPointSize);
   
     glPointSize(32);

     glBindTexture(GL_TEXTURE_2D, partTexture);
     glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
     glEnable(GL_POINT_SPRITE_ARB);
  }
  else
  {
     glEnable(GL_COLOR);
     glPointSize(1);
  }
}

/****************************************************************************
 *                               EndRenderer                                *
 ****************************************************************************/
void part1::EndRender()
{
   glDisable(GL_COLOR_MATERIAL);
   glDisable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_TRUE);
   glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
   glDisable(GL_TEXTURE_2D);
   if( (ext.PointParameterf != NULL) && (ext.PointParameterfv != NULL) )
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

/****************************************************************************
 *                               actualize                                  *
 ****************************************************************************/
void part1::actualize(particle* part)
{
   float dX = -1.0;   
   float dZ = 0.0;

   if( (part->posY <= 0) || intersectPlanes(part,&dX,&dZ) )
   {
       part->velY = -0.02*part->velY+(-gravity)*seconds;

       part->velX += dX*part->velY*seconds*gravity*
                     ((rand() / ((double)RAND_MAX + 1)));
       part->velZ += dZ*part->velY*seconds*gravity*
                     ((rand() / ((double)RAND_MAX + 1)));
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

/****************************************************************************
 *                             ContinueLive                                 *
 ****************************************************************************/
bool part1::continueLive(particle* part)
{
   return( (part->age < maxLive) );
}

/****************************************************************************
 *                              needCreate                                  *
 ****************************************************************************/
int part1::needCreate()
{
   /*return((int) ((maxParticles / maxLive)*0.8) + 
          (rand() % (int) ((maxParticles / maxLive))));*/

   return( (int)( 1+(maxParticles / maxLive)*(rand() / (RAND_MAX + 1.0)) ));
}

/****************************************************************************
 *                             createParticle                               *
 ****************************************************************************/
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

/****************************************************************************
 *                                NextStep                                  *
 ****************************************************************************/
void part1::NextStep(GLfloat matriz[6][4])
{
   seconds = 0.02;
   DoStep(matriz);
}

/****************************************************************************
 *                              numParticles                                *
 ****************************************************************************/
int part1::numParticles()
{
   return(actualParticles);
}

/****************************************************************************
 *                              LoadTexture                                 *
 ****************************************************************************/
GLuint part1::LoadTexture(string fileName)
{
   GLuint indice;
   SDL_Surface* img = IMG_Load(fileName.c_str());

   glGenTextures(1, &(indice));
   glBindTexture(GL_TEXTURE_2D, indice);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   SDL_FreeSurface(img);
   return(indice);
}

/****************************************************************************
 *                               addPlane                                   *
 ****************************************************************************/
interPlane* part1::addPlane(float x1, float y1, float z1, 
                            float x2, float y2, float z2,
                            float dX, float dZ, int inclination)
{
   interPlane* ip = new interPlane;
   ip->x1 = x1;
   ip->y1 = y1;
   ip->z1 = z1;
   ip->x2 = x2;
   ip->y2 = y2;
   ip->z2 = z2;
   ip->dX = dX;
   ip->dZ = dZ;
   ip->inclination = inclination;

   if(actualPlanes == 0)
   {
      ip->next = ip;
      ip->previous = ip;
   }
   else
   {
      ip->next = intersections;
      ip->previous = intersections->previous;
      ip->next->previous = ip;
      ip->previous->next = ip;
   }

   intersections = ip;
   actualPlanes++;
   return(ip);
}

/****************************************************************************
 *                            intersectPlanes                               *
 ****************************************************************************/
bool part1::intersectPlanes(particle* part, float* dX, float* dZ)
{
   int i;
   float yOnPlane = 0;
   float size;
   interPlane* ip = intersections;
   for(i = 0; i < actualPlanes; i++)
   {
      if( (part->posX <= ip->x2) && 
          (part->posX >= ip->x1) &&
          (part->posZ <= ip->z2) && 
          (part->posZ >= ip->z1) )
      {
         switch(ip->inclination)
         { 
             case PLANE_NO_INCLINATION: 
                   yOnPlane = ip->y1;
             break;
             case PLANE_INCLINATION_X:
             {
                size = (ip->x2 - ip->x1);
                yOnPlane = ((ip->x2 - part->posX) / size) * ip->y1 +
                           ((part->posX - ip->x1) / size) * ip->y2;
             }
             break;
             case PLANE_INCLINATION_Z:
             {
                size = (ip->z2 - ip->z1);
                yOnPlane = ((ip->z2 - part->posZ) / size) * ip->y1 +
                           ((part->posZ - ip->z1) / size) * ip->y2;
             }
             break;
         }
         if( ((part->posY >= yOnPlane - 1) && (part->posY <= yOnPlane + 1)) )
             
         {
            *dX = ip->dX;
            *dZ = ip->dZ;
            return(true);
         }
         /*else if (((part->posY <= yOnPlane +1) && (part->prvY >= yOnPlane -1) ))
         {
            printf("Alguem\n");
            part->posY = yOnPlane;
            *dX = ip->dX;
            *dZ = ip->dZ;
            return(true);
         }*/
         /*else
         { printf("prv: %.3f act: %.3f onP: %.3f",part->prvY,part->posY,yOnPlane); }*/
      }
      ip = ip->next;
   }
   return(false);
}

/****************************************************************************
 *                              removePlane                                 *
 ****************************************************************************/
void part1::removePlane(interPlane* ip)
{
   if(ip)
   {
      if(intersections == ip)
      {
         intersections = ip->next;
      }
      ip->next->previous = ip->previous;
      ip->previous->next = ip->next;
      delete(ip);
      actualPlanes--;
      if(actualPlanes == 0)
      {
         intersections = NULL;
      }
   }
}

/****************************************************************************
 *                          removeCharacterPlanes                           *
 ****************************************************************************/
void part1::removeCharacterPlanes()
{
   int i;
   for(i=0; i < 4; i++)
   {
      removePlane(intersections);
   }
}

/****************************************************************************
 *                            removeLastPlanes                              *
 ****************************************************************************/
void part1::removeLastPlane()
{
   removePlane(intersections);
}

/****************************************************************************
 *                             getTotalPlanes                               *
 ****************************************************************************/
int part1::getTotalPlanes()
{
   return(actualPlanes);
}

/****************************************************************************
 *                              getLastPlane                                *
 ****************************************************************************/
interPlane* part1::getLastPlane()
{
   return(intersections);
}


