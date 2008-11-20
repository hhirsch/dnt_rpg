#ifndef _racewindow_h
#define _racewindow_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "../gui/farso.h"
#include "../classes/race.h"

#define RACEW_CANCEL  0
#define RACEW_CONFIRM 1
#define RACEW_OTHER   2

/*! Races Window */
class raceWindow
{
   public:
      /*! Constructor
       * \param sk -> character's skills pointer
       * \param inter -> GUI interface
       * \param retRace -> pointer where will be the choosed race */
      raceWindow(skills* sk, guiInterface* inter, race** retRace);

      /*! Destructor */
      ~raceWindow();

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return 0 on close, 1 otherwise */
      int treat(guiObject* object, int eventInfo, guiInterface* inter);

      window*      intWindow;      /**< Pointer to the internal window */

   private:
      int         curRace;         /**< Position of current race on ordered */
      int         totalRaces;      /**< total Races */
      race**      choosedRace;     /**< Pointer to the choosed pointer race */
      race**      racesOrder;      /**< Vector with races ordered by name */
      skills*     externalSkills;  /**< Pointer to External Skills List */

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

      picture* raceImage;
      /*picture* prev2RaceImage;
      picture* prevRaceImage;
      picture* nextRaceImage;
      picture* next2RaceImage;*/

      void setCharacteristics();

      void setDescription();

};

#endif
