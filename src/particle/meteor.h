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

#ifndef _dnt_meteor_h
#define _dnt_meteor_h

#include "part2.h"
#include "../engine/collision.h"

/*! The meteor class. It is just a fire particles, 
 * that have their orign translated, and when reach
 * target, stop the motion, for MAX_STATIC_LIVING iterations */
class meteor: public particleSystem
{
   public:
      /*!
       ****************************************************************
       * Contructor
       * \param cX -> center X position
       * \param cY -> center Y position
       * \param cZ -> center Z position
       * \param vX -> variation on X
       * \param vY -> variation on Y
       * \param vZ -> variation on Z
       * \param tX -> target X
       * \param tY -> target Y
       * \param tZ -> target Z
       * \param fileName -> file name of the file to load 
       ****************************************************************/
      meteor(float cX, float cY, float cZ, float vX, float vY, float vZ,
             float tX, float tY, float tZ, string fileName);

      /*! Destructor */
      ~meteor();

      /*!
       ***************************************************************
       * Do things before render (like glBegin)
       ***************************************************************/  
      void initRender();
      /*!
       ***************************************************************
       * Do things after render (like glEnd)
       ***************************************************************/ 
      void endRender();

      /*!
       ****************************************************************
       * Verify if the Meteor is Living (aka, moving or 
       * in the static period) 
       * \return -> true if is living, false if it can be deleted 
       ****************************************************************/
      bool isLiving();

      /*!
       ***************************************************************
       * Do Next Step on System, rendering and actualizing
       * \param matriz -> View Frustum Matrix
       ***************************************************************/
      void nextStep(GLfloat** matriz);

      /*!
       ***************************************************************
       * Gets total living particles.
       * \return number of living particles
       ***************************************************************/
      int numParticles();

      /*!
       **************************************************************
       * Define The Collision System
       * \param col -> pointer to the collision system
       **************************************************************/
      void defineCollision(collision* col);

      /*! Not Used
       * \param part -> not used */
       void render(particle* part);
       /*! Not Used
       * \param part -> not used */
       void update(particle* part);
       /*! Not Used
       * \param part -> not used */
       bool continueLive(particle* part);
       /*! Not Used
       * \return -> not used */
       int  needCreate();
       /*! Not Used
       * \param part -> not used */
       void createParticle(particle* part);

   private:
      int actualLiving;     /**< Living Time after dead */
      GLfloat curPosX,      /**< Current Meteor X Position */
              curPosY,      /**< Current Meteor Y Position */
              curPosZ;      /**< Current Meteor Z Position */
      GLfloat varX,         /**< X variation */ 
              varY,         /**< Y variation */
              varZ;         /**< Z variation */
      GLfloat targX,        /**< Target X Coordinate */
              targY,        /**< Target Y Coordinate */
              targZ;        /**< Target Z Coordinate */
      bool dead;            /**< When reach target, is dead */
      part2* intFire;       /**< Internal Fire Particle */
      collision* colDetect; /**< Actual Collision Detection System */
};

#endif

