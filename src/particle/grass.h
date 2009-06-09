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

#ifndef _dnt_grass_h
#define _dnt_grass_h

#include "particle.h"
#include "wind.h"
#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <string>
using namespace std;


#define GRASS_INFLUENCE_AREA    10   /**< The Influence area */

class quadPos
{
   public:
      GLfloat x1, y1, z1,
              x2, y2, z2,
              x3, y3, z3,
              x4, y4, z4;
};

/*! The Grass Particle System */
class grass:public particleSystem
{
public:
      /*! Contructor
       * \param cX1 -> center X1 position
       * \param cZ1 -> center Z1 position
       * \param cX2 -> center X2 position
       * \param cZ2 -> center Z2 position
       * \param total -> total number of particles
       * \param scale -> scale of the grass
       * \param fileName -> file name of the file to load */
      grass(float cX1,float cZ1, float cX2, float cZ2, int total,
            float scale, string fileName);
      ~grass();

      /*!
       ***************************************************************
       * Render one particle on screen
       * \param part -> particle to render
       ***************************************************************/
      void render(particle* part);
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
       * Actualize particles attributes (with global independent
       *         forces and influentions).
       * \param part -> particle to actualize
       ***************************************************************/
      void update(particle* part);
      /*!
       ***************************************************************
       * Verifies if a particle continue live
       * \param part -> particle to verify
       * \return true if particle continue live, false, otherwise.
       ***************************************************************/
      bool continueLive(particle* part);
      /*!
       ***************************************************************
       * Total Particles needed to create on this step
       * \return number of particles to create. 
       ***************************************************************/
      int needCreate();
      /*!
       ***************************************************************
       * Create a particle (its position, color, etc);
       * \param part -> particle struct that will have the new created;
       ***************************************************************/
      void createParticle(particle* part);
      /*!
       ***************************************************************
       * Do Next Step on System, rendering and actualizing
       * \param matriz -> view frustum Matrix
       * \param pcPosX -> PC X position
       * \param pcPosY -> PC Y position
       * \param pcPosZ -> PC Z position
       * \param affectWind ->the wind that affect the system
       ***************************************************************/
      void nextStep(GLfloat** matriz,
                    GLfloat pcPosX, GLfloat pcPosY, GLfloat pcPosZ,
                    wind* affectWind);
      /*!
       ***************************************************************
       * Gets total living particles.
       * \return number of living particles
       ***************************************************************/
      int numParticles();
      /*!
       ***************************************************************
       * Define the opened map
       * \param actualMap -> pointer to the opened map
       ***************************************************************/
      void defineMap(void* actualMap);

      /*!
       ***************************************************************
       * Get The Area of the grass
       * \param cX1 -> x1 coordinate
       * \param cZ1 -> z1 coordinate
       * \param cX2 -> x2 coordinate
       * \param cZ2 -> z2 coordinate
       ***************************************************************/
      void getPosition(GLfloat& cX1, GLfloat& cZ1, GLfloat& cX2, GLfloat& cZ2 );

      /*!
       ***************************************************************
       * Get the filename of the Grass texture used
       * \return name of the grass texture file
       ***************************************************************/
      string getGrassFileName();

      /*!
       ***************************************************************
       * Get the scale factor of the grass
       * \return the scale factor of the grass
       ***************************************************************/
      GLfloat getScaleFactor();

      /*!
       ***************************************************************
       * Set the scale factor of the grass
       * \param scale -> the scale factor of the grass
       ***************************************************************/
      void setScaleFactor(GLfloat scale);

      /*!
       ***************************************************************
       * Set the total number of grass
       * \param total -> the total number of grass
       ***************************************************************/
      void setTotal(int total);

   private:
      /*!
       ***************************************************************
       * Set the size and Y position of the grass particle
       * \param part -> the particle to set
       ***************************************************************/
      void defineSize(particle* part);

      GLfloat seconds;      /**< Seconds, not used anymore. */
      GLfloat centerX1,     /**< Grass Area X1 */
            centerX2,       /**< Grass Area X2 */
            centerZ1,       /**< Grass Area Z1 */
            centerZ2;       /**< Grass Area Z2 */
      string grassFileName; /**< Name of the grass file texture */
      GLuint grassTexture;  /**< Load Texture used to grass */
      GLfloat scaleFactor;  /**< Scale factor of the grass */

      wind* internalWind;   /**< Internal wind */


      GLfloat resX,         /**< Result X position after first rotation */
              resY,         /**< Result Y Position after first rotation */
              resZ;         /**< Result Z position after first rotation */

      GLfloat pcX,          /**< PC X position */
              pcY,          /**< PC Y Position */
              pcZ;          /**< PC Z Position */

      quadPos* partPosition; /**< Each particle position after the
                                  initial rotation around the Y axys */
      float* textureArray;   /**< The texture coordinates array */ 
      int vArrayPos;         /**< Actual Vertex Array Position */
      int tArrayPos;         /**< Actual Texture Array Position */

      void* usedMap;        /**< Actual Openned Map */
};


#endif

