#ifndef _skillwindow_h
#define _skillwindow_h

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
      skillWindow(skills* sk, int points, interface* inter);

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \return 0 on close, 1 otherwise */
      int treat(Tobjeto* object, int eventInfo, interface* inter);

      janela*      window;           /**< Pointer to the internal window */

   private:
      skills*      externalSkill;    /**< Pointer to the edited skillList */
      int          avaiblePoints;    /**< Avaible points to distribute */
      int          curSkill;         /**< Actual Skill */
      quadroTexto* desc;             /**< Description Text */
      quadroTexto* txtAvaiblePoints; /**< Avaible Points Text */
      quadroTexto* skillName;        /**< Actual Skill Name */
      quadroTexto* txtPoints;        /**< Actual Points Text */
      quadroTexto* txtCosts;         /**< Actual Cost Text */

      botao* buttonSum;
      botao* buttonDec;
      botao* buttonPrevious;
      botao* buttonNext;
      botao* buttonConfirm;
      botao* buttonCancel;

      figura*      skFig;            /**< Actual skill Image */
      lang language;                 /**< Language internationalization */
};

#endif

