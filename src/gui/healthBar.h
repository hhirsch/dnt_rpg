/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _farso_healthBar_h
#define _farso_healthBar_h

#include "guiobject.h"
#include <SDL/SDL.h>

namespace Farso
{

/*! The Health Bar Class */
class HealthBar: public GuiObject
{
   public:
  
      /*! Contructor
       * \param x1 -> coordinate on surface 
       * \param y1 -> coordinate on surface
       * \param x2 -> coordinate on surface
       * \param y2 -> coordinate on surface
       * \param surface -> window where the bar is */
      HealthBar(int x1, int y1, int x2, int y2, SDL_Surface* surface);
      /*! Destructor */
      ~HealthBar();

      /*! Defines the Max Health of the Bar
       * \param health -> health value to be the max */
      void defineMaxHealth(int health);

      /*! Defines the Actual Health of the Bar
       * \param health -> health value to be the actual */
      void defineActualHealth(int health);

      /*! Draws the healthBar to Surface. */
      void draw();

   private:

      int actualHealth; /**< The Actual life on bar */
      int maxHealth;    /**< The Maximun life on bar */

};

}

#endif

