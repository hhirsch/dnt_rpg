#ifndef _skillwindow_h
#define _skillwindow_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

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
       * \param sk -> skills list (with images and description) 
       * \param savSkill -> skills list to save/modify 
       * \param inter -> interface used
       * \actualLevel -> current character level
       * \readOnlyMode -> if the windows is read only, or not */
      skillWindow(skills* sk, skills* savSkill, guiInterface* inter,
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
      skill**      skillsDesc;    /**< Skill Descriptions Sorted */
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

