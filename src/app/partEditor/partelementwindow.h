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

#ifndef _dnt_part_editor_partelement_window_aux
#define _dnt_part_editor_partelement_window_aux

#include <string>
using namespace std;

#include "../../gui/farso.h"
#include "../../particle/partelement.h"
#include "../../particle/particle.h"

class partElementWindow
{
   public:
      /*! Constructor 
       * \param interf -> pointer to the current guiInterface */
      partElementWindow(guiInterface* interf);
      /*! Destructor */
      ~partElementWindow();

      /*! Set the element to display, opening the window 
       * if not already opened.
       * \param e -> new element to display
       * \param text -> name of the element to display
       * \param p -> pointer to the particleSystem on edition */
      void setElement(dntPartElement* e, string text, particleSystem* p);

      /*! Treat the window events
       * \param object -> guiObject where event occurred
       * \param eventInfo -> info about the happened event
       * \return true when event is related to partElement window */
      bool treat(guiObject* object, int eventInfo);

      /*! Verify if window is open
       * \return true if open */
      bool isOpen();

   protected:
      /*! Open the window with current element */
      void openWindow();

      /*! Close the window */
      void closeWindow();

      /*! Set things on window related to the current type  */
      void setTypeThings();

      /*! Set all texts */
      void setTexts();

      /*! Reset the element, to update it with new values */
      void resetElement();

      guiInterface* gui;        /**< Gui used */
      window* curWindow;        /**< Current in use window */
      dntPartElement* element;  /**< Current in-edition element */
      string elementName;       /**< Current element's name */
      particleSystem* part;     /**< Current particle system edited */

      button* nextType;         /**< To select next type */
      button* previousType;     /**< To select previous type */
      textBox* type;            /**< Current type text */

      cxSel* useLimits;         /**< If will use the limits or not */
      textBar* lowerLimit;      /**< Lower limit */
      textBar* upperLimit;      /**< Upper limit */

      /* Random */
      textBar* mult;            /**< Multiplier value */
      textBar* sum;             /**< Sum value */

      /* Linear */
      textBar* initialValue;    /**< initial value */
      textBar* finalValue;      /**< final value */

      textBox* texts[7];        /**< Text of each element */
};

#endif

