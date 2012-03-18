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

#ifndef _dnt_part_editor_partelement_window_aux
#define _dnt_part_editor_partelement_window_aux

#include <string>

#include "../../gui/farso.h"
#include "../../particle/partelement.h"
#include "../../particle/particle.h"

class partElementWindow
{
   public:
      /*! Constructor 
       * \param interf -> pointer to the current Farso::GuiInterface */
      partElementWindow(Farso::GuiInterface* interf);
      /*! Destructor */
      ~partElementWindow();

      /*! Set the element to display, opening the window 
       * if not already opened.
       * \param e -> new element to display
       * \param text -> name of the element to display
       * \param p -> pointer to the particleSystem on edition */
      void setElement(dntPartElement* e, std::string text, particleSystem* p);

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
      dntPartElement* element;  /**< Current in-edition element */
      std::string elementName;  /**< Current element's name */
      particleSystem* part;     /**< Current particle system edited */

      Farso::Button* nextType;         /**< To select next type */
      Farso::Button* previousType;     /**< To select previous type */
      Farso::TextBox* type;            /**< Current type text */

      Farso::CxSel* useLimits;         /**< If will use the limits or not */
      Farso::TextBar* lowerLimit;      /**< Lower limit */
      Farso::TextBar* upperLimit;      /**< Upper limit */

      /* Random */
      Farso::TextBar* mult;            /**< Multiplier value */
      Farso::TextBar* sum;             /**< Sum value */

      /* Linear */
      Farso::TextBar* initialValue;    /**< initial value */
      Farso::TextBar* finalValue;      /**< final value */

      Farso::TextBox* texts[7];        /**< Text of each element */
};

#endif

