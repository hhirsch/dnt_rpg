#ifndef _classwindow_h
#define _classwindow_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "../gui/farso.h"
#include "../lang/lang.h"
#include "../classes/classes.h"

#define CLASSW_CANCEL  0
#define CLASSW_CONFIRM 1
#define CLASSW_OTHER   2

/*! Races Window */
class classWindow
{
   public:
      /*! Constructor */
      classWindow(classes* cls, skills* sk, interface* inter);

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \param actual -> pointer to current choosed class
       * \return 0 on close, 1 otherwise */
      int treat(Tobjeto* object, int eventInfo,interface* inter, 
                classe** actual);

      janela*      window;           /**< Pointer to the internal window */

   private:
      classes*      externalClasses;  /**< Pointer to the Classes List */
      classe*       actualClass;      /**< Pointer to actual Class */
      skills*       externalSkills;   /**< Pointer to actual Skill List */

      botao* buttonConfirm;
      botao* buttonCancel;

      botao* buttonPrevious;
      botao* buttonNext;

      quadroTexto* textName;
      quadroTexto* textDescTitle;
      rolBar* textDesc;
      rolBar* textCharac;
      quadroTexto* textCharacTitle;
      quadroTexto* textCitation;

      figura* classImage;

      lang language;                 /**< Language internationalization */

      string getCharacteristics();

};

#endif
