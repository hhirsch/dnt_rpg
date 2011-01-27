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

#include "partaux.h"

#include <string>
#include <iostream>
using namespace std;

/***********************************************************************
 *                               Constructor                           *
 ***********************************************************************/
partAux::partAux()
{
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
partAux::~partAux()
{
}

/***********************************************************************
 *                             getIntPtr                               *
 ***********************************************************************/
int* partAux::getIntPtr(string i)
{
   if(i == DNT_PART_AUX_TYPE)
   {
      return(&type);
   }
   else if(i == DNT_PART_AUX_MAX_PARTICLES)
   {
      return(&maxParticles);
   }
   else if(i == DNT_PART_AUX_MAX_LIFE_TIME)
   {
      return(&maxLifeTime);
   }
   else if(i == DNT_PART_AUX_MAX_PARTICLE_LIFE_TIME)
   {
      return(&maxParticleLifeTime);
   }
   else if(i == DNT_PART_AUX_DRAW_MODE)
   {
      return(&drawMode);
   }
   else if(i == DNT_PART_AUX_RENDER_MODE)
   {
      return(&renderMode);
   }
   else if(i == DNT_PART_AUX_POINT_SIZE)
   {
      return(&pointSize);
   }
  
   cerr << "Not found: " << i << endl;
   return(NULL);
}

/***********************************************************************
 *                              getInt                                 *
 ***********************************************************************/
int partAux::getInt(string i)
{
   int* p = getIntPtr(i);
   if(p)
   {
      return(*p);
   }

   return(0);
}

/***********************************************************************
 *                          deleteBuffers                              *
 ***********************************************************************/
void partAux::deleteBuffers()
{
   /* Delete any previous buffer (as INDIVIDUAL doesn't need) */
   if(vertexArray)
   {
      delete[] vertexArray;
      vertexArray = NULL;
   }
   if(colorArray)
   {
      delete[] colorArray;
      colorArray = NULL;
   }
   if(particles)
   {
      delete[] particles;
      particles = NULL;
   }
}

/***********************************************************************
 *                               reset                                 *
 ***********************************************************************/
void partAux::reset()
{
   deleteBuffers();
   init();
}
 
/***********************************************************************
 *                              setInt                                 *
 ***********************************************************************/     
void partAux::setInt(string i, int value)
{
   int* p = getIntPtr(i);
   if(p)
   {
      *p = value;
   }

   /* Special things for drawMode (create or not buffers) */
   if(i == DNT_PART_AUX_DRAW_MODE)
   {
      deleteBuffers();
      init();
   }
   /* Max particles changed, must recreate buffers (when used) */
   else if(i == DNT_PART_AUX_MAX_PARTICLES)
   {
      /* Avoid particle system without particles */
      if(maxParticles <= 0)
      {
         maxParticles = 1;
      }
      deleteBuffers();
      init();
   }
   /* Special things needed when changing maxParticleLife  */
   else if(i == DNT_PART_AUX_MAX_PARTICLE_LIFE_TIME)
   {
      /* TODO: Must recalculate all LINEAR elements */
   }
}

/***********************************************************************
 *                            getFloatPtr                              *
 ***********************************************************************/
float* partAux::getFloatPtr(string f)
{
   /* None! */
   cerr << "Not found: " << f << endl;
   return(NULL);
}

/***********************************************************************
 *                             getFloat                                *
 ***********************************************************************/
float partAux::getFloat(string f)
{
   float* p = getFloatPtr(f);
   if(p)
   {
      return(*p);
   }

   return(0.0f);
}
 
/***********************************************************************
 *                             setFloat                                *
 ***********************************************************************/     
void partAux::setFloat(string f, float value)
{
   float* p = getFloatPtr(f);
   if(p)
   {
      *p = value;
   }
}

/***********************************************************************
 *                            getBoolPtr                               *
 ***********************************************************************/
bool* partAux::getBoolPtr(string b)
{
   if(b == DNT_PART_AUX_WIND_AFFECT)
   {
      return(&windAffect);
   }
   else if(b == DNT_PART_AUX_FLOOR_COLLISION)
   {
      return(&floorCollision);
   }

   /* Unknow */
   cerr << "Not found: " << b << endl;
   return(NULL);
}

/***********************************************************************
 *                             getBool                                 *
 ***********************************************************************/
bool partAux::getBool(string b)
{
   bool* p = getBoolPtr(b);
   if(p)
   {
      return(*p);
   }

   return(false);
}
 
/***********************************************************************
 *                              setBool                                *
 ***********************************************************************/     
void partAux::setBool(string b, bool value)
{
   bool* p = getBoolPtr(b);
   if(p)
   {
      *p = value;
   }
}


/***********************************************************************
 *                             getElement                              *
 ***********************************************************************/
dntPartElement* partAux::getElement(string e)
{
   if(e == DNT_PART_AUX_PARTICLES_TO_CREATE)
   {
      return(&particlesToCreate);
   }
   else if(e == DNT_PART_AUX_RED)
   {
      return(&color[0]);
   }
   else if(e == DNT_PART_AUX_BLUE)
   {
      return(&color[2]);
   }
   else if(e == DNT_PART_AUX_GREEN)
   {
      return(&color[1]);
   }
   else if(e == DNT_PART_AUX_ALPHA)
   {
      return(&color[3]);
   }
   else if(e == DNT_PART_AUX_VELX)
   {
      return(&velocity[0]);
   }
   else if(e == DNT_PART_AUX_VELY)
   {
      return(&velocity[1]);
   }
   else if(e == DNT_PART_AUX_VELZ)
   {
      return(&velocity[2]);
   }
   else if(e == DNT_PART_AUX_POSX)
   {
      return(&position[0]);
   }
   else if(e == DNT_PART_AUX_POSY)
   {
      return(&position[1]);
   }
   else if(e == DNT_PART_AUX_POSZ)
   {
      return(&position[2]);
   }
   else if(e == DNT_PART_AUX_SCALE)
   {
      return(&scale);
   }



   cerr << "Not found: " << e << endl;
   return(NULL);
}

/***********************************************************************
 *                             getOrigin                               *
 ***********************************************************************/
dntPartOrigin* partAux::getOrigin()
{
   return(&origin);
}

