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

#ifndef _dnt_sun_h
#define _dnt_sun_h

#include <SDL/SDL_opengl.h>

/*! The class sun represents the global light simulating the SUN on game */
class sun
{
   public:
      /*! Constructor 
       * \param hour -> current hour of day 
       * \param farViewX -> far view X position
       * \param farViewZ -> far view Z position */
      sun(float hour, float farViewX, float farViewZ);
      /*! Destructor*/
      ~sun();

      /*! Update Sun Itensity, based on hour of the day 
       * \param hour -> current hour of day
       * \param posX -> character X position
       * \param posZ -> character Z position */
      void updateHourOfDay(float hour, float posX, float posZ);

      /*! Draw the Sun to its position */
      void drawSun();

      /*! Get The Sun rotation
       * \return sun rotation value */
      GLfloat getRotation();

      /*! Get The Sun Position
       * \param pos -> variable to get the sun position */
      void getPosition(GLfloat pos[4]);

      /*! Set the sun light to the current state */
      void setLight();

      /*! Disable the Sun Light. Usually at indoor maps */
      void disableLight();

      /*! Multiply the Matrix by the currenct Shadow Matrix. */
      void mulShadowMatrix();

      /*! Get the current Shadow Matrix related to the sun
       * \return -> sun's shadow matrix */
      GLfloat* getShadowMatrix();

      /*! Verifies if the sun is on visible time (born -> death)
       *  \return true if is in visible time, false otherwise.*/
      bool visibleTime();

      /*! Verifies if the sun is on shadow time (if false, no shadows)
       *  \return true if is in shadow time, false otherwise.*/
      bool shadowTime();
      
      /*! Verifies if the sun is on shadow time (if false, no shadows)
       *  \return true if is in shadow time, false otherwise.*/
      float getShadowAlpha();

   private:
      GLfloat quadricAttenuation;   /**< Quadric Attenuation Light Value */
      GLfloat constantAttenuation;  /**< Constant Attenuation Light Value */
      GLfloat linearAttenuation;    /**< Linear Attenuation Light Value */

      GLfloat actualColor[4];       /**< Actual Sun Emiting Color */
      GLfloat where[4];             /**< Actual Sun Sky Position */
      GLfloat rotation;             /**< Actual Sun Spin to surface */

      GLfloat shadowMat[4][4];      /**< Actual Shadow Matrix */

      GLuint  sunTexture;           /**< Current Sun Texture */
      GLuint  moonTexture;          /**< Current Moon Texture */

      float curHour;                /**< Current Hour of Day to Sun */

      /*! Set the current Postion of the sun, based on hour of the day
       * \param posX -> character X position
       * \param posZ -> character Z position*/
      void positionOnHour(float posX, float posZ);

      /*! Set the current Color of the sun, based on the rotation */
      void colorOnHour();

      /*! Define the shadow matrix */
      void defineShadowMatrix();
};

#endif

