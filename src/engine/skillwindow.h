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

#ifndef _dnt_skillwindow_h
#define _dnt_skillwindow_h

#include "../gui/farso.h"
#include "../classes/skills.h"
#include "../etc/dirs.h"

#define CHAR_CANCEL  0 /**< Cancel Character Edition Return TODO not here*/
#define CHAR_CONFIRM 1 /**< Confirm Character Edition Return TODO not here */
#define CHAR_OTHER   2 /**< Other Character Edition Return TODO not here */

#define SKILLW_CANCEL  0 /**< Cancel return from Skill Window */
#define SKILLW_CONFIRM 1 /**< Confirm return from skill window */
#define SKILLW_OTHER   2 /**< Other return from Skill Window */

/*! Skills Edition Window */
class skillWindow
{
   public:
      /*! Skill Window constructor 
       * \param savSkill -> skills list to save/modify 
       * \param inter -> interface used
       * \actualLevel -> current character level
       * \readOnlyMode -> if the windows is read only, or not */
      skillWindow(skills* savSkill, guiInterface* inter,
                  int actualLevel, bool readOnlyMode=false);

      /*! Destructor */
      ~skillWindow();

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return 0 on close, 1 otherwise */
      int treat(guiObject* object, int eventInfo, guiInterface* inter);

      /*! Verify if the window is opened
       * \return true if is opened, false otherwise */
      bool isOpened();

      /*! Close the window, if is opened
       * \param inter -> guiInterface where the window is */
      void close(guiInterface* inter);

   private:
 
      /*! Set point colors, due to the changes */
      void setColors();
      /*! Update the info show of the current skill */
      void updateSkillInfo();
      
      window*      intWindow;     /**< Pointer to the internal window */
      skills*      saveSkill;     /**< Pointer to the edited skill list */
      skill**      skillsOrder;   /**< Sorted skills */
      int          totalSkills;   /**< Total avaible skills */
      int          avaiblePoints; /**< Avaible points to distribute */
      int          curSkill;      /**< Actual Skill */
      int          charLevel;     /**< Character Level */
      bool         readOnly;      /**< True if read only window (no changes) */
      
      rolBar* desc;               /**< Description Text */
      textBox* txtAvaiblePoints;  /**< Avaible Points Text */
      textBox* skillName;         /**< Actual Skill Name */
      textBox* txtPoints;         /**< Actual Points Text */
      textBox* txtCosts;          /**< Actual Cost Text */

      button* buttonSum;          /**< Inc Points Button */
      button* buttonDec;          /**< Dec Points Button */
      button* buttonPrevious;     /**< Previous Skill Button */
      button* buttonNext;         /**< Next Skill Button */
      button* buttonConfirm;      /**< Confirm Changes Button */
      button* buttonCancel;       /**< Cancel Changes Button */

      picture*     skFig;         /**< Actual skill Image */

};

#endif

