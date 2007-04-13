#ifndef _racewindow_h
#define _racewindow_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "../gui/farso.h"
#include "../lang/lang.h"
#include "../classes/race.h"

#define RACEW_CANCEL  0
#define RACEW_CONFIRM 1
#define RACEW_OTHER   2

/*! Races Window */
class raceWindow
{
   public:
      /*! Constructor
       * \param rc -> races list
       * \param inter -> GUI interface
       * \param retRace -> pointer where will be the choosed race */
      raceWindow(races* rc, skills* sk, interface* inter, race** retRace);

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return 0 on close, 1 otherwise */
      int treat(Tobjeto* object, int eventInfo,interface* inter);

      janela*      window;           /**< Pointer to the internal window */

   private:
      races*      externalRaces;   /**< Pointer to the Aligns List */
      race*       actualRace;      /**< Pointer to actual aligned */
      race**      choosedRace;     /**< Pointer to the choosed pointer race */
      skills*     externalSkills;  /**< Pointer to External Skills List */

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

      figura* raceImage;

      lang language;                 /**< Language internationalization */

      string getCharacteristics();

};

#endif
