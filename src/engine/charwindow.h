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

#ifndef _dnt_charwindow_h
#define _dnt_charwindow_h

#include "../gui/farso.h"
#include "character.h"
#include "skillwindow.h"

/*! The character window displays information about a
 * character current states. */
class charWindow
{
   public:
      /*! Constructor
       * \param interf -> gui interface to open the window */
      charWindow(guiInterface* interf);

      /*! Destructor */
      ~charWindow();

      /*! Open the window with info about the character
       * \param pers -> character to show info */
      void open(character* pers);

      /*! Close the charWindow (if opened) */
      void close();

      /*! Treat Events on Window. 
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return != 0 if event is gathered */
      int treat(guiObject* object, int eventInfo,
                GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! Verify if the window is openned
       * \return true if opened, false otherwise. */
      bool isOpen();

      /*! Verify if it has some children window opened
       * \return -> true if some children window is opened */
      bool hasChildrenWindows();

   protected:

      /*! Write at the infoBar about the weapon from the inventory place
       * \param inventoryPlace -> place where the weapon is (usually 
       *        INVENTORY_LEFT_HAND and INVENTORY_RIGHT_HAND)*/
      void writeAboutWeapon(int inventoryPlace);

      window* intWindow;         /**< Pointer to the internal window */
      skillWindow* skWindow;     /**< Skill Window used */

      character* current;        /**< Current character */

      guiInterface* inter;       /**< Current GUI interface */
      button* okButton;          /**< The ok Button */
      button* levelUpButton;     /**< The Level Up Button */
      button* skillsButton;      /**< The Skills Button */
      rolBar* infoBar;           /**< The Info Bar */
};

#endif

