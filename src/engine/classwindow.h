#ifndef _classwindow_h
#define _classwindow_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "../gui/farso.h"
#include "../classes/classes.h"

#define CLASSW_CANCEL  0
#define CLASSW_CONFIRM 1
#define CLASSW_OTHER   2

/*! Races Window */
class classWindow
{
   public:
      /*! Constructor 
       * \param cls -> list of classes
       * \param sk -> list of skills 
       * \param inter -> pointer to the GUI
       * \param retClass -> pointer to the choosed class */
      classWindow(classes* cls, skills* sk, interface* inter, 
                  classe** retClass);

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return 0 on close, 1 otherwise */
      int treat(guiObject* object, int eventInfo,interface* inter); 

      window*      intWindow;           /**< Pointer to the internal window */

   private:
      classes*      externalClasses;  /**< Pointer to the Classes List */
      classe*       actualClass;      /**< Pointer to actual Class */
      classe**      choosedClass;     /**< Pointer to choosed pointer class */
      skills*       externalSkills;   /**< Pointer to actual Skill List */

      button* buttonConfirm;
      button* buttonCancel;

      button* buttonPrevious;
      button* buttonNext;

      textBox* textName;
      textBox* textDescTitle;
      rolBar* textDesc;
      rolBar* textCharac;
      textBox* textCharacTitle;
      textBox* textCitation;

      picture* classImage;

      /*! Draw the characteritics */
      void setCharacteristics();
      /*! Draw the descriptions */
      void setDescription();

};

#endif
