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

#ifndef _dnt_partController_h
#define _dnt_partController_h

#include "particle.h"
#include "partlist.h"

#include "../engine/collision.h"

#include <string>
using namespace std;

class Map;

/*! The partController class controls all particles systens, 
    make their updates, renders, deaths, etc. */
class partController
{
   public:
      /*! Init the use of the partController */
      void init();
      /*! Finish the use of the partController  */
      void finish();

      /*! Update and Render all Active Systems
       * \param matriz -> View Frustum Matrix.
       * \param enableGrass -> true if grass is enable. */
      void updateAll(GLfloat** matriz, bool enableGrass);

      /*! Add a Particle System to the System
       * \param X -> X coordinate
       * \param Y -> Y coordinate
       * \param Z -> Z coordinate
       * \param fileName -> name of the file containing the definition
       * \return particle pointer; */
      particleSystem* addParticle(GLfloat X, GLfloat Y, GLfloat Z,
                                  string fileName );

      /*! Remove an particle system from the system
       * \param part -> address of the Particle */
      void removeParticle(particleSystem* part);

      /*! Gets the total numbers of particles
       * \return Total Number of particles on system, from all systems */
      int numParticles();

      /*! Put all particle system on a stable state. */
      void stabilizeAll();

      /*! Load Particles Systems from file.
       * \param fileName -> name of the file to load. */
      void loadFromFile(string fileName);

      /*! Save Particles System To file.
       * \param fileName -> name of the file to be saved. */
      void saveToFile(string fileName);

      /*! Remove all particle systems from the controller 
       * \param keepPCRelated -> true to keep all particles related to the 
       *                         character */
      void deleteAll(bool keepPCRelated=false);

      /*! Remove all PC related particles from the controller  */
      void deleteAllPCRelated();

      /*! Define the opened map
       * \param acMap -> pointer to the opened map
       * \param col -> pointer to the collision system */
      void setActualMap(Map* acMap, collision* col);


   private:
      static particleList* particles;        /**< Waterfall Particles */
      
      static collision* colDetect;           /**< Collision System */
      static Map* currentMap;                /**< Current Map */

};


#endif
