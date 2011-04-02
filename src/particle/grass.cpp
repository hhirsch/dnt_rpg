/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "grass.h"
#include "../gui/draw.h"
#include "../map/map.h"
#include "../engine/util.h"
#include "../etc/dirs.h"

#include <iostream>
using namespace std;

#define ROT_STEP 0.5
#define ROT_MAX  deg2Rad(4)

/**************************************************************************
 *                             Constructor                                *
 **************************************************************************/
grass::grass(float cX1,float cZ1, float cX2, float cZ2, int total, 
             float scale, string fileName)
                                :particleSystem(total,PARTICLE_DRAW_INDIVIDUAL)
{

   dirs dir;
   
   /* NOTE: althought the constructor says PARTICLE_DRAW_INDIVIDUAL,
    * the grass is rendered as glArrays, at the endRender() function.
    * It's done this way since the array sizes of the grass is
    * distinct of the normal particles array sizes (grass = 4 points,
    * normal particles = 1 point). */

   SDL_Surface* img;
   centerX1 = cX1;
   centerX2 = cX2;
   centerZ1 = cZ1;
   centerZ2 = cZ2;
   scaleFactor = scale;
   usedMap = NULL;

   pcX = -1;
   pcY = -1;
   pcZ = -1;

   type = DNT_PARTICLE_TYPE_GRASS;

   /* Load Texture */
   grassFileName = fileName;
   img = IMG_Load(dir.getRealFile(fileName).c_str());
   if(!img)
   {
      cerr << "Couldn't load grass! Will Crash Soon!" << endl;
      cerr << "Grass file was: " << dir.getRealFile(fileName) << endl;
   }
   glGenTextures(1, &(grassTexture));
   glBindTexture(GL_TEXTURE_2D, grassTexture);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                   GL_LINEAR/*_MIPMAP_NEAREST*/);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

   /*gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img->w,
                     img->h, GL_RGBA, GL_UNSIGNED_BYTE, 
                     img->pixels );*/
   
   SDL_FreeSurface(img);

   /* Alloc Structs */
   partPosition = (quadPos*) new quadPos[total];
   vertexArray = (float*) new float[total*3*4];
   textureArray = (float*) new float[total*2*4];
}

/**************************************************************************
 *                              Destructor                                *
 **************************************************************************/
grass::~grass()
{
   glDeleteTextures(1, &grassTexture);
   usedMap = NULL;

   /* Dealloc structs */
   if(partPosition)
   {
      delete[] partPosition;
   }
   if(vertexArray)
   {
      delete[] vertexArray;
   }
   if(textureArray)
   {
      delete[] textureArray;
   }
}

/**************************************************************************
 *                                Render                                  *
 **************************************************************************/
void grass::render(particle* part)
{
   int n = part->internalNumber;

   rotatePoint(partPosition[n].x1, partPosition[n].y1, partPosition[n].z1, 
               part->R,
               partPosition[n].x3, partPosition[n].y3, partPosition[n].z3, 
               partPosition[n].x4, partPosition[n].y4, partPosition[n].z4,
               resX, resY, resZ);

   textureArray[tArrayPos] = 0.0;
   textureArray[tArrayPos+1] = 0.0;
   vertexArray[vArrayPos] = resX + part->posX;
   vertexArray[vArrayPos+1] = resY;
   vertexArray[vArrayPos+2] = resZ + part->posZ;


   rotatePoint(partPosition[n].x2, partPosition[n].y2, partPosition[n].z2, 
               part->R,
               partPosition[n].x3, partPosition[n].y3, partPosition[n].z3, 
               partPosition[n].x4, partPosition[n].y4, partPosition[n].z4,
               resX, resY, resZ);

   textureArray[tArrayPos+2] = 1.0;
   textureArray[tArrayPos+3] = 0.0;
   vertexArray[vArrayPos+3] = resX + part->posX;
   vertexArray[vArrayPos+4] = resY;
   vertexArray[vArrayPos+5] = resZ + part->posZ;

   rotatePoint(partPosition[n].x3, partPosition[n].y3, partPosition[n].z3, 
               part->R,
               partPosition[n].x3, partPosition[n].y3, partPosition[n].z3, 
               partPosition[n].x4, partPosition[n].y4, partPosition[n].z4,
               resX, resY, resZ);

   textureArray[tArrayPos+4] = 1.0;
   textureArray[tArrayPos+5] = 1.0;
   vertexArray[vArrayPos+6] = resX + part->posX;
   vertexArray[vArrayPos+7] = resY;
   vertexArray[vArrayPos+8] = resZ + part->posZ;

   rotatePoint(partPosition[n].x4, partPosition[n].y4, partPosition[n].z4, 
               part->R,
               partPosition[n].x3, partPosition[n].y3, partPosition[n].z3, 
               partPosition[n].x4, partPosition[n].y4, partPosition[n].z4,
               resX, resY, resZ);

   textureArray[tArrayPos+6] = 0.0;
   textureArray[tArrayPos+7] = 1.0;
   vertexArray[vArrayPos+9] = resX + part->posX;
   vertexArray[vArrayPos+10] = resY;
   vertexArray[vArrayPos+11] = resZ + part->posZ;

   vArrayPos += 12;
   tArrayPos += 8;

}

/**************************************************************************
 *                              InitRender                                *
 **************************************************************************/
void grass::initRender()
{
   glColor4f(1.0,1.0,1.0, 1.0);
   glEnable(GL_COLOR_MATERIAL);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_TRUE);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, grassTexture);
   glEnable(GL_ALPHA_TEST);
   glAlphaFunc(GL_GREATER,0.1f);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   vArrayPos = 0;
   tArrayPos = 0;
}

/**************************************************************************
 *                              EndRender                                 *
 **************************************************************************/
void grass::endRender()
{
   /* Draw as arrays */
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   glTexCoordPointer(2, GL_FLOAT, 0, textureArray);
   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, vertexArray);
   glDrawArrays(GL_QUADS, 0, tArrayPos / 2);
   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_TEXTURE_COORD_ARRAY);

   /* Undo the changes on the GL state */
   glDisable(GL_COLOR_MATERIAL);
   glDisable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_TRUE);
   glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
   glEnable(GL_ALPHA_TEST);
   glAlphaFunc(GL_ALWAYS,0.0f);
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
   glEnable(GL_LIGHTING);

}

/**************************************************************************
 *                                update                                  *
 **************************************************************************/
void grass::update(particle* part)
{
   bool done = false;
   /* Verify the PC Influence */
   if( (pcX > 0) && /*(pcY > 0) &&*/ (pcZ > 0) )
   {
      /* Verify if is inner the influence area */
      if( (part->posX >= pcX - GRASS_INFLUENCE_AREA) &&
          (part->posX <= pcX + GRASS_INFLUENCE_AREA) && 
          /*(part->posY >= pcY - GRASS_INFLUENCE_AREA) &&
          (part->posY <= pcY + GRASS_INFLUENCE_AREA) &&*/
          (part->posZ >= pcZ - GRASS_INFLUENCE_AREA) &&
          (part->posZ <= pcZ + GRASS_INFLUENCE_AREA)
        )
      {
         done =true;
         /* Verify if is left or right */
         /*if(part->posX <= pcX)
         {*/
            part->R = deg2Rad(-85);
         /*}
         else
         {  
            part->R = deg2Rad(85);
         }*/
      }
   }

   if(!done)
   {
      part->R += deg2Rad(part->size);
   }

   if(part->R <= part->prvR - ROT_MAX)
   {
      part->size = ROT_STEP;
   }
   else if(part->R >= part->prvR + ROT_MAX)
   {
      part->size = -ROT_STEP;
   }
}

/**************************************************************************
 *                            continueLive                                *
 **************************************************************************/
bool grass::continueLive(particle* part)
{
   return( true );
}

/**************************************************************************
 *                             needCreate                                 *
 **************************************************************************/
int grass::needCreate()
{
   if(actualParticles == 0)
   {
      return(maxParticles);
   }
   return(0);
}

/**************************************************************************
 *                           createParticle                               *
 **************************************************************************/
void grass::createParticle(particle* part)
{
   /* Define all Rotations */
   part->R = deg2Rad(20*(rand() / ((double)RAND_MAX + 1)));
   part->B = 15*(rand() / ((double)RAND_MAX + 1));

   part->velY = sin(deg2Rad(part->R));
   
   /* Define Rotation variation */
   if((rand()/ ((double)RAND_MAX + 1)) > 0.5)
   {
      part->size = -ROT_STEP;
   }
   else
   {
      part->size = ROT_STEP;
   }

   part->prvR = part->R;

   /* Define Particle Size and Position */
   defineSize(part);
}

/**************************************************************************
 *                               NextStep                                 *
 **************************************************************************/
void grass::nextStep(GLfloat** matriz, 
                     GLfloat pcPosX, GLfloat pcPosY, GLfloat pcPosZ,
                     wind* affectWind)
{
   seconds = 0.02;

   internalWind = affectWind;
   pcX = pcPosX;
   pcY = pcPosY;
   pcZ = pcPosZ;

   doStep(matriz);
}

/**************************************************************************
 *                               NextStep                                 *
 **************************************************************************/
void grass::nextStep(GLfloat** matriz)
{
  /* Not used */
}

/**************************************************************************
 *                             getPosition                                *
 **************************************************************************/
void grass::getPosition(GLfloat& cX1, GLfloat& cZ1, GLfloat& cX2, GLfloat& cZ2 )
{
   cX1 = centerX1;
   cX2 = centerX2;
   cZ1 = centerZ1;
   cZ2 = centerZ2;
}

/**************************************************************************
 *                           getGrassFileName                             *
 **************************************************************************/
string grass::getGrassFileName()
{
   return(grassFileName);
}

/**************************************************************************
 *                            getScaleFactor                              *
 **************************************************************************/
GLfloat grass::getScaleFactor()
{
   return(scaleFactor);
}

/**************************************************************************
 *                            setScaleFactor                              *
 **************************************************************************/
void grass::setScaleFactor(GLfloat scale)
{
   int i;
   scaleFactor = scale;

   /* Actualize All Particles */
   for(i = 0; i < maxParticles; i++)
   {
      defineSize(&particles[i]);
   }
}

/**************************************************************************
 *                                setTotal                                *
 **************************************************************************/
void grass::setTotal(int total)
{
   /* Delete the alloced structs */
   delete []particles;
   delete[] partPosition;
   delete[] vertexArray;
   delete[] textureArray;

   /* Reinit the system with the new total */
   init(total,PARTICLE_DRAW_INDIVIDUAL);

   /* Realloc structs */
   
   partPosition = (quadPos*) new quadPos[total];
   vertexArray = (float*) new float[total*3*4];
   textureArray = (float*) new float[total*2*4];
}

/**************************************************************************
 *                              defineSize                                *
 **************************************************************************/
void grass::defineSize(particle* part)
{
   /* Define Position X on Map */
   part->posX = ((centerX2-centerX1)*(rand() / ((double)RAND_MAX + 1)))
                + (centerX1);
   /* Define Position Z on Map */
   part->posZ = ((centerZ2-centerZ1)*(rand() / ((double)RAND_MAX + 1)))
                + (centerZ1);

   /* Define the grass orientation */
   GLfloat orientation = 90*(rand() / ((double)RAND_MAX + 1));

   /* Define fixed Rotations */
     /* Around Y axis */
   GLfloat cosOri = 8 * scaleFactor * cos(deg2Rad(orientation));
   GLfloat sinOri = 8 * scaleFactor * sin(deg2Rad(orientation));

   int n = part->internalNumber;

   /* Do the initial rotation around Y axys */
   partPosition[n].x1 = -cosOri;
   partPosition[n].z1 = -sinOri;
   partPosition[n].x2 = +cosOri;
   partPosition[n].z2 = +sinOri;
   partPosition[n].x3 = +cosOri;
   partPosition[n].z3 = +sinOri;
   partPosition[n].x4 = -cosOri;
   partPosition[n].z4 = -sinOri;

   /* Define Height  */
   if(usedMap)
   {
      Map* map = (Map*) usedMap;
      GLfloat ha = map->getHeight(-cosOri + part->posX, -sinOri + part->posZ);
      GLfloat hb = map->getHeight(cosOri + part->posX, sinOri + part->posZ);

      partPosition[n].y1 = ha + 10*scaleFactor;
      partPosition[n].y2 = hb + 10*scaleFactor;
      partPosition[n].y3 = ha;
      partPosition[n].y4 = hb;
   }
   else
   {
      partPosition[n].y1 = 10*scaleFactor;
      partPosition[n].y2 = 10*scaleFactor;
      partPosition[n].y3 = 0;
      partPosition[n].y4 = 0;
   }
}

/**************************************************************************
 *                              defineMap                                 *
 **************************************************************************/
void grass::defineMap(Map* actualMap)
{
   int i;
   usedMap = actualMap;
   for(i = 0; i < maxParticles; i++)
   {
      if(particles[i].status != PARTICLE_STATUS_DEAD)
      {
         particles[i].posY = actualMap->getHeight(particles[i].posX - 8.0, 
                                             particles[i].posZ) + 10;
         particles[i].prvX = actualMap->getHeight(particles[i].posX + 8.0, 
                                             particles[i].posZ) + 10;
         particles[i].prvY = actualMap->getHeight(particles[i].posX + 8.0, 
                                             particles[i].posZ);
         particles[i].prvZ = actualMap->getHeight(particles[i].posX - 8.0, 
                                             particles[i].posZ);
      }
   }
}


