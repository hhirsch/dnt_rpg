/* Lightning */

#include "part6.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL_image.h>

/****************************************************************************
 *                             Constructor                                  *
 ****************************************************************************/
part6::part6(float cX,float cY,float cZ, string fileName):
                               particleSystem(fileName,PARTICLE_DRAW_GROUPS)
{
   dirs dir;
   centerX = cX; 
   centerY = cY; 
   centerZ = cZ;
   actualY = centerY;
   actualParticles = 0;
   partTexture = LoadTexture(dir.getRealFile("particles/part2.png"));
}

/****************************************************************************
 *                              Destructor                                  *
 ****************************************************************************/
part6::~part6()
{
   glDeleteTextures(1,&partTexture);
}

/****************************************************************************
 *                                Render                                    *
 ****************************************************************************/
void part6::Render(particle* part)
{
}

/****************************************************************************
 *                              InitRender                                  *
 ****************************************************************************/
void part6::InitRender()
{
  glDisable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_FALSE);
   glEnable(GL_CULL_FACE);

   float MaxPointSize = 0;

   if( (ext.PointParameterf != NULL) && (ext.PointParameterfv != NULL) )
   {
      glEnable(GL_TEXTURE_2D);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);

      glGetFloatv( GL_POINT_SIZE_MAX_ARB, &MaxPointSize );
      glPointSize(16);
      ext.PointParameterf( GL_POINT_SIZE_MIN_ARB, 5.0 );
      ext.PointParameterf( GL_POINT_SIZE_MAX_ARB, MaxPointSize);

      glBindTexture(GL_TEXTURE_2D, partTexture);
      glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
      glEnable(GL_POINT_SPRITE_ARB);
   }
   else
   {
      glEnable(GL_COLOR);
      glPointSize(4);
   }
}

/****************************************************************************
 *                              EndRender                                   *
 ****************************************************************************/
void part6::EndRender()
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
void part6::actualize(particle* part)
{
   //Not Used!
}

/****************************************************************************
 *                             continueLive                                 *
 ****************************************************************************/
bool part6::continueLive(particle* part)
{
   return( true );
}

/****************************************************************************
 *                             needCreate                                   *
 ****************************************************************************/
int part6::needCreate()
{
   if(actualY <= 0 )
      return(0);
   else
      return((int)gravity);
}

/****************************************************************************
 *                            createParticle                                *
 ****************************************************************************/
void part6::createParticle(particle* part)
{
   dSumVel[0] += (2*(rand() / ((double)RAND_MAX + 1))) - 1;
   part->posX = dSumVel[0] + centerX;
   part->posY = actualY;
   dSumVel[2] += (2*(rand() / ((double)RAND_MAX + 1))) - 1;
   part->posZ = dSumVel[2] + centerZ;
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
   part->status =  PARTICLE_STATUS_STATIC;
   actualY+=(-1*dSumVel[1]);
}

/****************************************************************************
 *                               NextStep                                   *
 ****************************************************************************/
void part6::NextStep(GLfloat matriz[6][4])
{
   seconds = 0.02;
   DoStep(matriz);
}

/****************************************************************************
 *                             numParticles                                 *
 ****************************************************************************/
int part6::numParticles()
{
   return(actualParticles);
}

/****************************************************************************
 *                             LoadTexture                                  *
 ****************************************************************************/
GLuint part6::LoadTexture(string fileName)
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

