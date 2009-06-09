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

#ifndef _dnt_part1_h
#define _dnt_part1_h

#include "particle.h"

/*!
 ******************************************
 *            Max Planes Number           *
 ******************************************/
#define PLANE_NO_INCLINATION 0 /**< No Inclination on Plane */
#define PLANE_INCLINATION_X  1 /**< Inclination on X axis */
#define PLANE_INCLINATION_Z  2 /**< Inclination on Z axis */

/*! Intersection Plane */
class interPlane
{
   public:
      float x1,        /**< X coordinate of plane */
            x2;        /**< X coordinate of plane */
      float y1,        /**< Y coordinate of plane */
            y2;        /**< Y coordinate of plane */
      float z1,        /**< Z coordinate of plane */
            z2;        /**< Z coordinate of plane */
      float dX,        /**< Variation X of water on plane */
            dZ;        /**< Variation Z of water on plane */
      int inclination; /**< Inclination Type of plane */

      interPlane* next;       /**< Next Plane on List */
      interPlane* previous;   /**< Previous Plane on List */
}; 

/*! WaterFalls Particle */
class part1: public particleSystem
{
   public:
      /*!
       ***************************************************************
       * Contructor
       * \param cX -> center X position
       * \param cY -> center Y position
       * \param cZ -> center Z position 
       * \param fileName -> file name of the file to load 
       ***************************************************************/
      part1(float cX,float cY,float cZ, string fileName); 
      /*! 
       ***************************************************************
       * Destructor 
       ***************************************************************/
      ~part1();

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
       ***************************************************************
       * Update particles attributes (with global independent
       *         forces and influentions).
       * \param part -> particle to actualize
       ***************************************************************/
      void update(particle* part);
      /*!
       ***************************************************************
       * Verifies if the particle continue live or not.
       * \param part -> particle to verify
       * \return true if particle continue live, false, otherwise.
       ***************************************************************/
      bool continueLive(particle* part);
      /*!
       ***************************************************************
       * Total Particles needed to create on this step
       * \return Number of particles to create
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
       * Do a step on system after sec seconds
       * \param matriz -> View Frustum Matrix
       ***************************************************************/
      void nextStep(GLfloat** matriz);
      /*!
       ***************************************************************
       * Gets the number of actual active particles
       * \return number of actual living particles. 
       ***************************************************************/
      int numParticles();
      /*!
       ***************************************************************
       * Add one plane to internal Planes
       * \param x1 -> initial x plane coordinate 
       * \param y1 -> initial y plane coordinate
       * \param z1 -> initial z plane coordinate
       * \param x2 -> final x plane coordinate 
       * \param y2 -> final y plane coordinate
       * \param z2 -> final z plane coordinate
       * \param dX -> particle X variation when plane contact
       * \param dZ -> particle Z variation when plane contact
       * \param inclination -> plane inclination type
       ***************************************************************/
      interPlane* addPlane(float x1, float y1, float z1, 
                           float x2, float y2, float z2, 
                           float dX, float dZ, int inclination);

      /*!
       ***************************************************************
       * Removes a Plane
       * \param ip -> pointer to the interPlane to remove
       ***************************************************************/
      void removePlane(interPlane* ip);

      /*!
       ***************************************************************
       * Removes The Character's Planes
       ***************************************************************/
      void removeCharacterPlanes();
      /*!
       ***************************************************************
       * Verify if Particle intersects one internal Plane
       * \param part -> particle struct;
       * \param dX -> Variation on X particle axys when intersects
       * \param dZ -> Variation on Z particle axys when intersects
       * \return true if intersects.
       ***************************************************************/
      bool intersectPlanes(particle* part, float* dX, float* dZ); 

      /*!
       ***************************************************************
       * Get the total number of planes on the waterfall
       * \return number of planes
       ***************************************************************/
      int getTotalPlanes();

      /*!
       ***************************************************************
       * Get the last plane on the waterfall
       * \return -> pointer to the last plane
       ***************************************************************/
      interPlane* getLastPlane();

      /*!
       ***************************************************************
       * Remove the last added plane of the waterfall
       ***************************************************************/
      void removeLastPlane();


   private:
      float seconds;         /**< Actual Time on particle */

      interPlane* intersections; /**< The internal Planes */
      int actualPlanes;      /**< actual number of planes */

      
};


#endif

