/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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

#include "wave.h"
#include "../engine/util.h"
#include <math.h>

#define SURFACE_VX 8
#define SURFACE_VZ 8

/**********************************************************************
 *                             Constructor                            *
 **********************************************************************/
waves::waves(string waveFile, GLfloat X, GLfloat Y, GLfloat Z, int nX, int nZ)
{
   int i,w;
   actualWaves = NULL;
   totalWaves = 0;
   surfX = nX;
   surfZ = nZ;
   initialX = X;
   initialY = Y;
   initialZ = Z;
   surface = (position**) malloc(nX*sizeof(position*));
   for(i = 0; i < surfX; i++)
   {
      surface[i] = (position*) malloc(nZ*sizeof(position));
   }

   for(i=0; i < surfX; i++)
   {
      for(w=0; w < surfZ; w++)
      {
         surface[i][w].x = i*SURFACE_VX;
         surface[i][w].y = 0;
         surface[i][w].z = w*SURFACE_VZ;
      }
   }
}

/**********************************************************************
 *                              Destructor                            *
 **********************************************************************/
waves::~waves()
{
   int i;
   while(actualWaves)
   {
      removeWave(actualWaves);
   }
   for(i = 0; i < surfX; i++)
   {
      free(surface[i]);
   }
   free(surface);
}

/**********************************************************************
 *                            removeDeadWaves                         *
 **********************************************************************/
void waves::removeDeadWaves()
{
   int i;
   wave* aux = actualWaves;
   for(i=0; i < getTotalWaves(); i++)
   {
      if(aux->dead)
      {
         aux = aux->next;
         removeWave(aux->previous);
      }
      else
      {
         aux = aux->next;
      }
   }
}

/**********************************************************************
 *                             insertWave                             *
 **********************************************************************/
void waves::insertWave(int initialX, int initialZ, 
                       GLfloat amplitude, GLfloat deltaAmplitude, 
                       int generationTime, int attrition, int direction)
{
   wave* aux;

   aux = new(wave);
   aux->initialX = initialX;
   aux->initialZ = initialZ;
   aux->amplitude = amplitude;
   aux->deltaAmplitude = deltaAmplitude;
   aux->generationTime = generationTime;
   aux->attrition = attrition;
   aux->direction = direction;
   aux->nearLoop = 0;
   aux->farLoop = 0;
   aux->actualLifeTime = 0;
   aux->dead = false;

   if(!actualWaves)
   {
      aux->next = aux;
      aux->previous = aux;
      actualWaves = aux;
   }
   else
   {
      aux->next = actualWaves;
      aux->previous = actualWaves->previous;
      aux->previous->next = aux;
      actualWaves->previous = aux;
      actualWaves = aux;
   }
   
   totalWaves++;
}

/**********************************************************************
 *                            removeWave                              *
 **********************************************************************/
void waves::removeWave(wave* remWave)
{
   if(remWave)
   {
      if(getTotalWaves() > 1)
      {
         remWave->next->previous = remWave->previous;
         remWave->previous->next = remWave->next;
         if(remWave == actualWaves)
         {
            actualWaves = remWave->next;
         }
      }
      else
      {
         actualWaves = NULL;
      }
      delete(remWave);
      totalWaves--;
   }
}

/**********************************************************************
 *                             getTotalWaves                          *
 **********************************************************************/
int waves::getTotalWaves()
{
   return(totalWaves);
}

/**********************************************************************
 *                                doStep                              *
 **********************************************************************/
void waves::doStep()
{
   int i, w;
   int vX = 0, vZ = 0;
   wave* aux = actualWaves;

   GLfloat x = 0,y = 0,z = 0;

   /* First, reset Surface */
   for(i=0; i < surfX; i++)
   {
      for(w=0; w < surfZ; w++)
      {
         surface[i][w].px = surface[i][w].x;
         surface[i][w].x = i*SURFACE_VX;
         surface[i][w].py = surface[i][w].y;
         surface[i][w].y = 0;
         surface[i][w].pz = surface[i][w].z;
         surface[i][w].z = w*SURFACE_VZ;
      }
   }
   
   for(i=0; i < getTotalWaves(); i++)
   {
      if(aux->dead)
      {
         /* Remove Dead Waves */
         aux = aux->next;
         removeWave(aux->previous);
      }
      else
      {
         /* Only Actualize Living Waves */
         aux->dead = true; //mark as dead unless is alive after
         aux->actualLifeTime++;
         if(aux->generationTime >= aux->actualLifeTime)
         {
            aux->nearLoop = 0;
         }
         else
         {
            aux->nearLoop++;
         }
         aux->farLoop++;

         for(w = aux->nearLoop; w < aux->farLoop; w++)
         {
            int mod;
            int div;
            y = w*aux->deltaAmplitude + aux->actualLifeTime*aux->deltaAmplitude
                + aux->amplitude;
            div = (int)(y / (2*aux->amplitude));
            mod = (int) (div * 
                  (int)(2*aux->amplitude));
            y = y - mod;

            if(div % 2 != 0)
            {
               y = 2*aux->amplitude - y;
            }

            if(y >= 0)
            {
               y = y - aux->amplitude;
               if(aux->direction == WAVE_DIRECTION_UP)
               {
                  vX = aux->initialX;
                  vZ = aux->initialZ - w;
                  z = sqrt((aux->amplitude*aux->amplitude)-(y*y));
                  x = 0;
               }
               else if(aux->direction == WAVE_DIRECTION_DOWN)
               {
                  vX = aux->initialX;
                  vZ = aux->initialZ + w;
                  z = sqrt((aux->amplitude*aux->amplitude)-(y*y));
                  x = 0;
               }
               else if(aux->direction == WAVE_DIRECTION_LEFT)
               {
                  vX = aux->initialX - w;
                  vZ = aux->initialZ;
                  x = sqrt((aux->amplitude*aux->amplitude)-(y*y));
                  z = 0;
               }
               else if(aux->direction == WAVE_DIRECTION_RIGHT)
               {
                  vX = aux->initialX + w;
                  vZ = aux->initialZ;
                  x = sqrt((aux->amplitude*aux->amplitude)-(y*y));
                  z = 0;
               }

               /* If Vertex Exists */
               if( (vX >= 0) && (vX < surfX) && (vZ >= 0) && (vZ < surfZ))
               {
                  aux->dead = false;
                  if(y < surface[vX][vZ].py)
                  {
                     if( (aux->direction == WAVE_DIRECTION_LEFT) ||
                         (aux->direction == WAVE_DIRECTION_RIGHT))
                     {
                        x = -x;
                     }
                     else
                     {
                        z = -z;
                     }
                  }
                  
                  surface[vX][vZ].x += x;
                  surface[vX][vZ].y += y;
                  surface[vX][vZ].z += z;
                  if( vX == 0)
                  {
                     surface[vX][vZ].x = 0;
                  }
                  else if( vX == surfX-1)
                  {
                     surface[vX][vZ].x = (surfX-1)*SURFACE_VX;
                  }
                  if( vZ == 0)
                  {
                     surface[vX][vZ].z = 0;
                  }
                  else if( vZ == surfZ-1)
                  {
                     surface[vX][vZ].z = (surfZ-1)*SURFACE_VZ;
                  }

               }
               else
               {
                  //TODO wave colision with end of the surface (create one with 
                  // opost direction)
               }
            }
         }

         
         aux = aux->next;
      }
   }
}

/**********************************************************************
 *                                 draw                               *
 **********************************************************************/
void waves::draw()
{
   int x,z;

   GLfloat nx=0, ny=0, nz=0;
   
   GLfloat ambient[] = { 0.29, 0.492, 0.82, 0.45 };
   GLfloat diffuse[] = { 0.705, 0.907, 1.0, 0.45 };
   GLfloat specular[] = { 0.714, 0.781, 0.886, 0.45 };
   glPushMatrix();

   glEnable( GL_BLEND );
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
   glBegin(GL_QUADS);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular );

   for(x=0; x < (surfX-1); x++)
   {
      for(z = 0; z < (surfZ-1); z++)
      {
        normal(surface[x][z].x + initialX, 
               surface[x][z].y + initialY,
               surface[x][z].z + initialZ, 
               surface[x+1][z].x + initialX,
               surface[x+1][z].y + initialY,
               surface[x+1][z].z + initialZ,
               surface[x+1][z+1].x + initialX,
               surface[x+1][z+1].y + initialY,
               surface[x+1][z+1].z + initialZ,
               nx, ny, nz);
        glNormal3f(nx, ny, nz);
        glVertex3f(surface[x][z].x + initialX,
                   surface[x][z].y + initialY,
                   surface[x][z].z + initialZ);
        glVertex3f(surface[x+1][z].x + initialX,
                   surface[x+1][z].y + initialY,
                   surface[x+1][z].z + initialZ);
        glVertex3f(surface[x+1][z+1].x + initialX,
                   surface[x+1][z+1].y + initialY,
                   surface[x+1][z+1].z + initialZ);
        glVertex3f(surface[x][z+1].x + initialX,
                   surface[x][z+1].y + initialY,
                   surface[x][z+1].z + initialZ);
      }
   }
   
   glEnd();
   glDisable( GL_BLEND );
   glPopMatrix();
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

