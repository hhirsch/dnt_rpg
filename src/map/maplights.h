/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

#ifndef _dnt_maplights_h
#define _dnt_maplights_h

#include <SDL/SDL_opengl.h>
#include <string>

#define MAX_LIGHTS_PER_MAP       20

/*! Map Lights Implementation \todo not working for now. */
class mapLight
{
   public:
      GLfloat light_ambient[4];   /**< Ambient Color Light */
      GLfloat light_diffuse[4];   /**< Diffuse Color Light */
      GLfloat light_specular[4];  /**< Specular Color Light */
      GLfloat light_position[4];  /**< Light position */
      GLfloat light_direction[4]; /**< Light Direction */

      GLfloat linearAtenuation;   /**< Linear Atenuation */
      GLfloat quadricAtenuation;  /**< Quadric Atenuation */
      GLfloat constantAtenuation; /**< Constant Atenuation */

      GLfloat cutOff;             /**< Spot CutOff */

      bool enableLight;      /**< Enable Light */
      bool enableDiffuse;    /**< Enable Diffuse Light */
      bool enableSpecular;   /**< Enable Specular Light */
      bool enableAmbient;    /**< Enable Ambient Light */
      bool enableSpot;       /**< Enable Spot Light */
      bool enableAtenuation; /**< Enable Linear Atenuation */

      /*! Constructor */
      mapLight();
      /*! Destructor */
      ~mapLight();
};

/*! The light Distance Class */
class lightDist
{
   public:
      int lightNumber;  /**< Internal Number of the Light */
      GLfloat distance; /**< Current Distance */
};

/*! Group of lights to a map */
class mapLights
{
   public:
      /*! Constructor */
      mapLights(); 
      /*! Destructor */
      ~mapLights();
      /*! Load Map Lights from file
       * \param arq -> filename of lights file */
      void load(std::string arq);
      /*! Actualize Map Lights Position
       * \param posX -> active character X position
       * \param posZ -> active character Z position */
      void update(GLfloat posX, GLfloat posZ);

      /*! Get the Lights File Name
       * \return name of the file */
      std::string getFileName();

   private:
      /*! Set the active lights as the near lights
       * \param posX -> X position
       * \param posZ -> Z position */
      void setNearLights(GLfloat posX, GLfloat posZ);

      int totalLights;                       /**< Total Lights on Map */
      mapLight light[MAX_LIGHTS_PER_MAP];    /**< lights on Map */
      std::string fileName;                       /**< FileName */
      lightDist activeLights[3];             /**< Current Active Lights */
};

#endif

