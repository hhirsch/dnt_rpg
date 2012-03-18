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

#ifndef _dnt_charwindow_h
#define _dnt_charwindow_h

#include "../gui/farso.h"
#include "character.h"
#include "skillwindow.h"
#include "featswindow.h"

/*! The character window displays information about a
 * character current states. */
class charWindow
{
   public:
      /*! Constructor
       * \param interf -> gui interface to open the window */
      charWindow(Farso::GuiInterface* interf);

      /*! Destructor */
      ~charWindow();

      /*! Open the window with info about the character
       * \param pers -> character to show info */
      void open(character* pers);

      /*! Close the charWindow (if opened) */
      void close();

      /*! Treat Events on Window. 
       * \param object -> last GUI object
       * \param features -> current talents on game
       * \param eventInfo -> last GUI Event
       * \return != 0 if event is gathered */
      int treat(Farso::GuiObject* object, int eventInfo, featsList* features,
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

      Farso::Window* intWindow;         /**< Pointer to the internal window */
      skillWindow* skWindow;     /**< Skill Window used */
      featsWindow* talentsWindow;/**< Talents window */

      character* current;        /**< Current character */

      Farso::GuiInterface* inter;       /**< Current GUI interface */
      Farso::Button* okButton;          /**< The ok Button */
      Farso::Button* levelUpButton;     /**< The Level Up Button */
      Farso::Button* skillsButton;      /**< The Skills Button */
      Farso::Button* talentsButton;     /**< The talents button */
      Farso::RolBar* infoBar;           /**< The Info Bar */
};

#endif

