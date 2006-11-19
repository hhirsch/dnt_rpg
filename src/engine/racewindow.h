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
      /*! Constructor */
      raceWindow(races* rc, interface* inter);

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return 0 on close, 1 otherwise */
      int treat(Tobjeto* object, int eventInfo,interface* inter, race** actual);

      janela*      window;           /**< Pointer to the internal window */

   private:
      races*      externalRaces;   /**< Pointer to the Aligns List */
      race*       actualRace;      /**< Pointer to actual aligned */

      botao* buttonConfirm;
      botao* buttonCancel;

      botao* buttonPrevious;
      botao* buttonNext;

      quadroTexto* textName;
      quadroTexto* textDescTitle;
      quadroTexto* textDesc;
      quadroTexto* textCharac;
      quadroTexto* textCharacTitle;
      quadroTexto* textCitation;

      figura* raceImage;

      lang language;                 /**< Language internationalization */

      string getCharacteristics();

};

#endif
