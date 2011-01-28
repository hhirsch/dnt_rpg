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

#ifndef _dnt_part_editor_part_aux
#define _dnt_part_editor_part_aux


#include "../../particle/particle.h"

/* Integers */
#define DNT_PART_AUX_TYPE                      "type"
#define DNT_PART_AUX_MAX_PARTICLES             "maxParticles"
#define DNT_PART_AUX_MAX_LIFE_TIME             "maxLifeTime"
#define DNT_PART_AUX_MAX_PARTICLE_LIFE_TIME    "particleLifeTime"
#define DNT_PART_AUX_DRAW_MODE                 "drawMode"
#define DNT_PART_AUX_RENDER_MODE               "renderMode"
#define DNT_PART_AUX_POINT_SIZE                "pointSize"
/* Booleans */
#define DNT_PART_AUX_WIND_AFFECT               "windAffect"
#define DNT_PART_AUX_FLOOR_COLLISION           "floorCollision"
/* Elements */
#define DNT_PART_AUX_PARTICLES_TO_CREATE       "particlesToCreate"
#define DNT_PART_AUX_RED                       "red"
#define DNT_PART_AUX_BLUE                      "blue"
#define DNT_PART_AUX_GREEN                     "green"
#define DNT_PART_AUX_ALPHA                     "alpha"
#define DNT_PART_AUX_VELX                      "velocityX"
#define DNT_PART_AUX_VELY                      "velocityY"
#define DNT_PART_AUX_VELZ                      "velocityZ"
#define DNT_PART_AUX_POSX                      "posX"
#define DNT_PART_AUX_POSY                      "posY"
#define DNT_PART_AUX_POSZ                      "posZ"
#define DNT_PART_AUX_SCALE                     "scale"


/*! The partAux class represents a particleSystem. It is defined
 * to have access to some particleSystem methods, only usefull for
 * partEditor use (i.e. no need to be implemented at the 
 * particleSystem class itself). */
class partAux: public particleSystem
{
   public:
      /*! Constructor */
      partAux();
      /*! Destructor */
      ~partAux();

      /*! Get integer parameter
       * \param i -> parameter name 
       * \return paramater value */
      int getInt(string i);
      /*! Set integer parameter
       * \param i -> parameter name
       * \param value -> parameter value */
      void setInt(string i, int value);

      /*! Get float parameter
       * \param f -> parameter name 
       * \return paramater value */
      float getFloat(string f);
      /*! Set float parameter
       * \param f -> parameter name
       * \param value -> parameter value */
      void setFloat(string f, float value);

      /*! Get boolean parameter
       * \param b -> parameter name 
       * \return paramater value */
      bool getBool(string b);
      /*! Set boolean parameter
       * \param b -> parameter name
       * \param value -> parameter value */
      void setBool(string b, bool value);

      /*! Get particle element
       * \param e -> element name
       * \return pointer to the particle element got */
      dntPartElement* getElement(string e);

      /*! Get the particle origin
       * \return pointer to the particle origin */
      dntPartOrigin* getOrigin();

      /*! Get the current gravity value */
      float getGravity();
      /*! Set the gravity value
       * \param g -> new gravity value */
      void setGravity(float g);

      /*! Reset the particle system, returning to inital state */
      void reset();

      /*! Get the texture filename
       * \return filename of the texture used */
      string getTextureFileName();

      /*! Set the filename of the texture to use */
      void setTextureFileName(string f);

   private:
      /*! Get the pointer to an internal integer */
      int* getIntPtr(string i);
      /*! Get the pointer to an internal boolean */
      bool* getBoolPtr(string b);
      /*! Get the pointer to an internal float */
      float* getFloatPtr(string f);
      /*! Delete all buffers */
      void deleteBuffers();
};


#endif


