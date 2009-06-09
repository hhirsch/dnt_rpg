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

#ifndef _dnt_part4_h
#define _dnt_part4_h

#include "particle.h"

/*! Smoke Particle */
class part4: public particleSystem
{
   public:
      /*! Contructor
       * \param cX -> center X position
       * \param cY -> center Y position
       * \param cZ -> center Z position 
       * \param fileName -> file name of the file to load */
      part4(float cX,float cY,float cZ, string fileName);
      ~part4();

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
       * Update particles attributes (with global independent
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
       * \param matriz -> view Frustum Matrix 
       ***************************************************************/
      void nextStep(GLfloat** matriz);
      /*!
       ***************************************************************
       * Gets total living particles.
       * \return number of living particles
       ***************************************************************/
      int numParticles();

   private:
      float seconds;     /**< Seconds, not used. */
};


#endif

