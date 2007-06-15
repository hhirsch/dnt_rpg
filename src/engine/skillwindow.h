#ifndef _skillwindow_h
#define _skillwindow_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "../gui/farso.h"
#include "../classes/skills.h"
#include "../lang/lang.h"

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
      /*! Skill Window constructor */
      skillWindow(skills* sk, skills* savSkill, interface* inter);

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return 0 on close, 1 otherwise */
      int treat(guiObject* object, int eventInfo, interface* inter);

      janela*      window;           /**< Pointer to the internal window */

   private:
      skills*      externalSkill;    /**< Pointer to the skillList with descriptions */
      skills*      saveSkill;        /**< Pointer to the edited skill list */
      int          avaiblePoints;    /**< Avaible points to distribute */
      int          curSkill;         /**< Actual Skill */
      quadroTexto* desc;             /**< Description Text */
      quadroTexto* txtAvaiblePoints; /**< Avaible Points Text */
      quadroTexto* skillName;        /**< Actual Skill Name */
      quadroTexto* txtPoints;        /**< Actual Points Text */
      quadroTexto* txtCosts;         /**< Actual Cost Text */

      button* buttonSum;
      button* buttonDec;
      button* buttonPrevious;
      button* buttonNext;
      button* buttonConfirm;
      button* buttonCancel;

      figura*      skFig;            /**< Actual skill Image */
      lang language;                 /**< Language internationalization */ 

};

#endif

