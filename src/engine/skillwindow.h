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
      skillWindow(skills* savSkill, Farso::GuiInterface* inter,
                  int actualLevel, bool readOnlyMode=false);

      /*! Destructor */
      ~skillWindow();

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return 0 on close, 1 otherwise */
      int treat(Farso::GuiObject* object, int eventInfo, 
           Farso::GuiInterface* inter);

      /*! Verify if the window is opened
       * \return true if is opened, false otherwise */
      bool isOpened();

      /*! Close the window, if is opened
       * \param inter -> guiInterface where the window is */
      void close(Farso::GuiInterface* inter);

   private:
 
      /*! Set point colors, due to the changes */
      void setColors();
      /*! Update the info show of the current skill */
      void updateSkillInfo();
      /*! Set to the previous skill */
      void previousSkill();
      /*! Set to the next skill */
      void nextSkill();

      Farso::Window* intWindow;   /**< Pointer to the internal window */
      skills*      saveSkill;     /**< Pointer to the edited skill list */
      skill**      skillsOrder;   /**< Sorted skills */
      int          totalSkills;   /**< Total avaible skills */
      int          avaiblePoints; /**< Avaible points to distribute */
      int          curSkill;      /**< Actual Skill */
      int          charLevel;     /**< Character Level */
      bool         readOnly;      /**< True if read only window (no changes) */

      Farso::RolBar* desc;               /**< Description Text */
      Farso::TextBox* txtAvaiblePoints;  /**< Avaible Points Text */
      Farso::TextBox* skillName;         /**< Actual Skill Name */
      Farso::TextBox* txtPoints;         /**< Actual Points Text */
      Farso::TextBox* txtCosts;          /**< Actual Cost Text */
      Farso::CxSel* cxOnlyClass;         /**< Only Race/Class skills */

      Farso::Button* buttonSum;          /**< Inc Points Button */
      Farso::Button* buttonDec;          /**< Dec Points Button */
      Farso::Button* buttonPrevious;     /**< Previous Skill Button */
      Farso::Button* buttonNext;         /**< Next Skill Button */
      Farso::Button* buttonConfirm;      /**< Confirm Changes Button */
      Farso::Button* buttonCancel;       /**< Cancel Changes Button */

      Farso::Picture*     skFig;         /**< Actual skill Image */

};

#endif

