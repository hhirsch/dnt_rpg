/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
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

#ifndef _dnt_part_editor_part_origin_window
#define _dnt_part_editor_part_origin_window

#include <string>

#include "../../gui/farso.h"
#include "../../particle/partorigin.h"
#include "partaux.h"

class partOriginWindow
{
   public:
      /*! Constructor 
       * \param interf -> pointer to the current Farso::GuiInterface */
      partOriginWindow(Farso::GuiInterface* interf);
      /*! Destructor */
      ~partOriginWindow();

      /*! Set the particle to display origin, opening the window 
       * if not already opened.
       * \param p -> the particle to display */
      void setParticle(partAux* p);

      /*! Treat the window events
       * \param object -> Farso::GuiObject where event occurred
       * \param eventInfo -> info about the happened event
       * \return true when event is related to partElement window */
      bool treat(Farso::GuiObject* object, int eventInfo);

      /*! Verify if window is open
       * \return true if open */
      bool isOpen();

      /*! Close the window */
      void closeWindow();

   protected:
      /*! Open the window with current element */
      void openWindow();

      /*! Set things on window related to the current type  */
      void setTypeThings();

      /*! Set all texts */
      void setTexts();

      /*! Reset the element, to update it with new values */
      void resetElement();

      Farso::GuiInterface* gui;        /**< Gui used */
      Farso::Window* curWindow;        /**< Current in use window */
      partAux* part;            /**< Current particle system edited */

      Farso::Button* nextType;         /**< To select next type */
      Farso::Button* previousType;     /**< To select previous type */
      Farso::TextBox* type;            /**< Current type text */

      Farso::TextBox* text[3];         /**< Desc text */

      Farso::TextBar* pos1[3];   /**< Lower Position edition */
      Farso::TextBar* pos2[3];   /**< Upper Position edition (only for quad) */
      Farso::TextBar* radius;    /**< Radius (only for sphere) */
};

#endif

