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

#ifndef _dnt_attwindow_h
#define _dnt_attwindow_h

#include "../lang/translate.h"
#include "../gui/farso.h"
#include "../classes/skills.h"

#define ATTW_CANCEL  0
#define ATTW_CONFIRM 1
#define ATTW_OTHER   2

/*! Attributes Roll Window */
class attWindow
{
   public:
      /*! Constructor 
       * \param savSkill -> skill to be edited
       * \param inter -> GUI interface to be used
       * \param modifiers -> race and class Modifiers
       * \param usePreviousValues -> if will use previous values or new ones */
      attWindow(skills* savSkill, Farso::GuiInterface* inter, 
                int modifiers[6], bool usePreviousValues);

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \param proj -> projection matrix
       * \param modl -> Model Viewn matrix
       * \param viewPort -> ViewPort Vector
       * \return 0 on close, 1 otherwise */
      int treat(Farso::GuiObject* object, int eventInfo, 
            Farso::GuiInterface* inter,
            GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      Farso::Window*      intWindow;     /**< Pointer to the internal window */

   private:
      int points[6];              /**< Rolled Points */
      int attPointsIndex[6];      /**< Index of the selected points for att */
      bool used[6];               /**< Mark Used Rolled Points */
      int rcModifiers[6];         /**< Race And class modifiers */

      skills* saveSkill;          /**< Pointer to the edited skill List */

      Farso::TextBox* rolledPoints;  /**< Rolled Points Text */
      Farso::TextBox* attPoints[6];  /**< Attributes Points Texts */
      Farso::TextBox* attTotals[6];  /**< Attributes Totals Texts */
      Farso::TextBox* attMods[6];    /**< Attributes Totals Modifiers */
      Farso::RolBar* textDesc;       /**< Description Text */
      Farso::TextBox* textDescTitle; /**< Description Title Text */

      Farso::Button* attButtonNext[6];    /**< Next Buttons */
      Farso::Button* attButtonPrev[6];    /**< Previous Buttons */
      Farso::Button* rerollButton;        /**< ReRoll Button */
      Farso::Button* clearButton;         /**< Clear Button */
      Farso::Button* autoButton;          /**< Auto Assign Button */
      Farso::Button* buttonConfirm;       /**< Confirm Button */
      Farso::Button* buttonCancel;        /**< Cancel Button */

      /*! Set the description text
       * \param sk -> skill number */
      void setDescription(int sk);

      /*! Roll one Att Dices */
      int rollDices();
      /*! Roll All Att Dices */
      void rollAllDices();

      /*! Gets next (circular) not used rolled point 
       *  \param att -> attribute-1 number
       *  \return -> integer with the index of the point */
      int nextAvaiblePoints(int att);
      /*! Gets previous (circular) not used rolled point 
       *  \param att -> attribute-1 number
       *  \return -> integer with the index of the point */
      int previousAvaiblePoints(int att);
      /*! Clears the selected list of points. */
      void clear();
      /*! Gets the att bonus modifier and puts on screen 
       * \param att -> att-1 number
       * \return attribute bonus modifier */
      int assignAttMod(int att);
      /*! Verifies if all attributes are assigned
       * \return true if all are assigned, false otherwise*/
      bool allAssigned();
      /*! Auto Assign all values to Attributes */
      void autoAssign();
      /*! Assign Previous values to the dices */
      void assignPreviousToDices();
      /*! Assign Previous values to the attributes */
      void assignPreviousValues();

};

#endif

