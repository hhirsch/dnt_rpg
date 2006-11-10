/* Lightning */

#include "part6.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL_image.h>

part6::part6(float cX,float cY,float cZ, string fileName):
                               particleSystem(fileName,PARTICLE_DRAW_GROUPS)
{
   centerX = cX; 
   centerY = cY; 
   centerZ = cZ;
   actualY = centerY;
   actualParticles = 0;
   partTexture = LoadTexture("../data/particles/part2.png");
}

part6::~part6()
{
   glDeleteTextures(1,&partTexture);
}

void part6::Render(particle* part)
{
}

void part6::InitRender()
{
  /* test atenuation */
  /*int viewPort[4];
  float mat[16];
  glGetFloatv(GL_PROJECTION_MATRIX,mat);
  glGetIntegerv(GL_VIEWPORT, viewPort);

  printf("V2 = %d M0 = %.3f \n",viewPort[2], mat[0]);*/
   
  glDisable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_FALSE);
   glEnable(GL_CULL_FACE);

   float MaxPointSize = 0;

   if( (PointParameterf != NULL) && (PointParameterfv != NULL) )
   {
      glEnable(GL_TEXTURE_2D);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);

      glGetFloatv( GL_POINT_SIZE_MAX_ARB, &MaxPointSize );

      //float quadratic[] =  { 0.0f, 0.0f, 0.011831263f };
       //float quadratic[] =  { 0.01f, 0.01f, 0.0f };
       //float quadratic[] =  { 1.0f, 0.0f, 0.0f };
       //PointParameterfv( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );
       //
      glPointSize(16);


      //PointParameterf( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );
      PointParameterf( GL_POINT_SIZE_MIN_ARB, 5.0 );
      PointParameterf( GL_POINT_SIZE_MAX_ARB, MaxPointSize);

      glBindTexture(GL_TEXTURE_2D, partTexture);
      glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
      glEnable(GL_POINT_SPRITE_ARB);
   }
   else
   {
      glEnable(GL_COLOR);
      glPointSize(4);

      //glEnable(GL_POINT_SMOOTH);
      //glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
   }
}

void part6::EndRender()
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
   glEnable(GL_LIGHTING);
   glDisable( GL_BLEND );
}

void part6::actualize(particle* part)
{
   /*if(part->posY <= 0 )
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
*/
      /*if(part->posY >= 5)
      {
        part->velX += seconds*(gravity/2.0)*(((rand() / ((double)RAND_MAX + 1))));
      }
      else
      {*/
  /*      part->velX += seconds*(dMultVel[0]*((rand() / ((double)RAND_MAX + 1))) 
                      + dSumVel[0]);
      //}
      part->velZ += seconds*(dMultVel[2]*((rand() / ((double)RAND_MAX + 1))) 
                      + dSumVel[2]);

      part->posY += part->velY*seconds;
      part->posX += part->velX*seconds;
      part->posZ += part->velZ*seconds;
   }*/
}

bool part6::continueLive(particle* part)
{
   return( true );
}

int part6::needCreate()
{
   if(actualY <= 0 )
      return(0);
   else
      return((int)gravity);
}

void part6::createParticle(particle* part)
{
   dSumVel[0] += (2*(rand() / ((double)RAND_MAX + 1))) - 1;
   part->posX = dSumVel[0] + centerX;
   part->posY = actualY;/*(dMultCenter[1]*(rand() / ((double)RAND_MAX + 1))+dSumCenter[1])
                + centerY;*/
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

void part6::NextStep(GLfloat matriz[6][4])
{
   seconds = 0.02;
   DoStep(matriz);
}

int part6::numParticles()
{
   return(actualParticles);
}

GLuint part6::LoadTexture(char* fileName)
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

