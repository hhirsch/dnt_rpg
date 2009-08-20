/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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

/* Waterfall */

#include "part1.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL_image.h>

/****************************************************************************
 *                               Constuctor                                 *
 ****************************************************************************/
interPlaneList::interPlaneList():dntList(DNT_LIST_TYPE_ADD_AT_END)
{
}

/****************************************************************************
 *                               Destuctor                                  *
 ****************************************************************************/
interPlaneList::~interPlaneList()
{
   clearList();
}

/****************************************************************************
 *                              freeElement                                 *
 ****************************************************************************/
void interPlaneList::freeElement(dntListElement* obj)
{
   interPlane* pl = (interPlane*)obj;
   delete(pl);
}

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
   type = DNT_PARTICLE_TYPE_WATERFALL;
   partTexture = loadTexture(dir.getRealFile("particles/water.png"));
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
void part1::render(particle* part)
{
   //Not Used    
}

/****************************************************************************
 *                               InitRender                                 *
 ****************************************************************************/
void part1::initRender()
{
  glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_FALSE);
  glEnable(GL_CULL_FACE);

  float MaxPointSize = 0;
  float quadratic[] =  { 0.01f, 0.01f, 0.0f };

  if( ext.hasPointTexture() )
  {
     glEnable(GL_TEXTURE_2D);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

     glEnable(GL_BLEND);
     glGetFloatv( GL_POINT_SIZE_MAX_ARB, &MaxPointSize );
     ext.arbPointParameterfv( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );
     ext.arbPointParameterf( GL_POINT_SIZE_MIN_ARB, 2.0f );
     ext.arbPointParameterf( GL_POINT_SIZE_MAX_ARB, MaxPointSize);
   
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
void part1::endRender()
{
   glDisable(GL_COLOR_MATERIAL);
   glDisable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_TRUE);
   glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
   glDisable(GL_TEXTURE_2D);
   if( ext.hasPointTexture() )
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
 *                                 update                                   *
 ****************************************************************************/
void part1::update(particle* part)
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
void part1::nextStep(GLfloat** matriz)
{
   seconds = 0.02;
   doStep(matriz);
}

/****************************************************************************
 *                               addPlane                                   *
 ****************************************************************************/
interPlane* part1::addPlane(float x1, float y1, float z1, 
                            float x2, float y2, float z2,
                            float dX, float dZ, int inclination)
{
   interPlane* ip = new interPlane;

   /* Set values */
   ip->x1 = x1;
   ip->y1 = y1;
   ip->z1 = z1;
   ip->x2 = x2;
   ip->y2 = y2;
   ip->z2 = z2;
   ip->dX = dX;
   ip->dZ = dZ;
   ip->inclination = inclination;

   /* Insert at the list */
   intersections.insert(ip);

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
   
   interPlane* ip = (interPlane*)intersections.getFirst();

   for(i = 0; i < intersections.getTotal(); i++)
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
      }
      ip = (interPlane*)ip->getNext();
   }
   return(false);
}

/****************************************************************************
 *                              removePlane                                 *
 ****************************************************************************/
void part1::removePlane(interPlane* ip)
{
   intersections.remove(ip);
}

/****************************************************************************
 *                          removeCharacterPlanes                           *
 ****************************************************************************/
void part1::removeCharacterPlanes()
{
   int i;
   for(i=0; i < 4; i++)
   {
      removeLastPlane();
   }
}

/****************************************************************************
 *                            removeLastPlanes                              *
 ****************************************************************************/
void part1::removeLastPlane()
{
   interPlane* first;
   interPlane* last;
  
   first = (interPlane*)intersections.getFirst();
   if(first)
   {
      last = (interPlane*)first->getPrevious();
      removePlane(last);
   }
}

/****************************************************************************
 *                             getTotalPlanes                               *
 ****************************************************************************/
int part1::getTotalPlanes()
{
   return(intersections.getTotal());
}

/****************************************************************************
 *                              getLastPlane                                *
 ****************************************************************************/
interPlane* part1::getLastPlane()
{
   interPlane* first; 
   interPlane* last; 

   first = (interPlane*)intersections.getFirst();
   if(first)
   {
      last = (interPlane*)first->getPrevious();
      return(last);
   }

   return(NULL);
}


