/* Blood */

#include "part5.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL_image.h>

/****************************************************************************
 *                             Constructor                                  *
 ****************************************************************************/
part5::part5(float cX,float cY,float cZ, string fileName):
                               particleSystem(fileName,PARTICLE_DRAW_GROUPS)
{
   dirs dir;
   centerX = cX; 
   alpha=cY; 
   centerZ=cZ;
   actualParticles = 0;
   partTexture = LoadTexture(dir.getRealFile("particles/part2.png"));
   livingTime = 0;
   doneCreation = false;
}

/****************************************************************************
 *                              Destructor                                  *
 ****************************************************************************/
part5::~part5()
{
   glDeleteTextures(1,&partTexture);
}

/****************************************************************************
 *                                Render                                    *
 ****************************************************************************/
void part5::Render(particle* part)
{
   //Not Used
}

/****************************************************************************
 *                              InitRender                                  *
 ****************************************************************************/
void part5::InitRender()
{
   livingTime++;
   glDisable(GL_LIGHTING);
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
      glPointSize(8);
      ext.PointParameterf( GL_POINT_SIZE_MIN_ARB, 2.0f );
      ext.PointParameterf( GL_POINT_SIZE_MAX_ARB, MaxPointSize);

      glBindTexture(GL_TEXTURE_2D, partTexture);
      glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
      glEnable(GL_POINT_SPRITE_ARB);
   }
   else
   {
      glEnable(GL_COLOR);
      glPointSize(2);
   }
}

/****************************************************************************
 *                             EndRenderer                                  *
 ****************************************************************************/
void part5::EndRender()
{
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

   glEnable(GL_LIGHTING);
   glDisable( GL_BLEND );
}

/****************************************************************************
 *                              actualize                                   *
 ****************************************************************************/
void part5::actualize(particle* part)
{
   if(part->posY <= 0 )
   {
     /* FIXME do it based on map Height, not 0! */
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
      part->velX += seconds*(dMultVel[0]*((rand() / ((double)RAND_MAX + 1))) 
                      + dSumVel[0]);
      part->velZ += seconds*(dMultVel[2]*((rand() / ((double)RAND_MAX + 1))) 
                      + dSumVel[2]);

      part->posY += part->velY*seconds;
      part->posX += part->velX*seconds;
      part->posZ += part->velZ*seconds;
   }
}

/****************************************************************************
 *                             continueLive                                 *
 ****************************************************************************/
bool part5::continueLive(particle* part)
{
   return(part->age <= MAX_BLOOD_REMOVING);
}

/****************************************************************************
 *                             needCreate                                   *
 ****************************************************************************/
int part5::needCreate()
{
   if( (actualParticles < maxParticles) && (!doneCreation))
   {
      return(rand() % 30);
   }
   else
   {
      doneCreation = true;
      return(0);
   }
}

/****************************************************************************
 *                           createParticle                                 *
 ****************************************************************************/
void part5::createParticle(particle* part)
{
   part->posX = (dMultCenter[0]*(rand() / ((double)RAND_MAX + 1))+dSumCenter[0])
                + centerX;
   part->posY = (dMultCenter[1]*(rand() / ((double)RAND_MAX + 1))+dSumCenter[1])
                + alpha + centerY;
   part->posZ = (dMultCenter[2]*(rand() / ((double)RAND_MAX + 1))+dSumCenter[2])
                + centerZ;
   part->prvX = part->posX;
   part->prvY = part->posY;
   part->prvZ = part->posZ;
   part->size = 4; 
   part->velY = -1.0;
   part->velX = 1*(rand() / ((double)RAND_MAX + 1))-0.5;
   part->velZ = 1*(rand() / ((double)RAND_MAX + 1))-0.5;
   part->R = initR;
   part->G = initG;
   part->B = initB;
   part->prvR = part->R;
   part->prvG = part->G; 
   part->prvB = part->B;
}

/****************************************************************************
 *                               NextStep                                   *
 ****************************************************************************/
void part5::NextStep(GLfloat matriz[6][4])
{
   seconds = 0.02;
   DoStep(matriz);
}

/****************************************************************************
 *                             numParticles                                 *
 ****************************************************************************/
int part5::numParticles()
{
   return(actualParticles);
}

/****************************************************************************
 *                             LoadTexture                                  *
 ****************************************************************************/
GLuint part5::LoadTexture(string fileName)
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
 *                            getLivingTime                                 *
 ****************************************************************************/
GLuint part5::getLivingTime()
{
   return(livingTime);
}

