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

#ifndef _farso_cxsel_h
#define _farso_cxsel_h

#include "draw.h"
#include "guiobject.h"
#include "color.h"

/*! Selection Box Class */
class cxSel: public guiObject
{
   public:
      /*! Constructor
       * \param x -> x position
       * \param y -> y position */
      cxSel(int x, int y);

      /*! Draws the Selected Box
       * \param screen -> surface */
      void draw(SDL_Surface *screen);

      /*! Verify if the cxSel is selected 
       * \return true if the cxsel is selected */
      bool isSelected();

      /*! Invert the selection of the cxSel */
      void invertSelection();

      /*! Set the state to value
       * \param value -> new value of the selection */
      void setSelection(bool value);

      /*! Do the cxSel press
       * \param mouseButton -> current mouse button state
       * \return true when the pressing is finished */
      bool doPress(Uint8 mouseButton);

   private:
      farso_colors Colors;   /**< Colors of the object */
      bool selected;         /**< True if selected, false otherwise  */

      bool pressing;         /**< mouse is currently pressing it */
};

#endif

