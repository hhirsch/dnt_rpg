/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
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

#ifndef _dnt_racewindow_h
#define _dnt_racewindow_h

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
       * \param inter -> GUI interface
       * \param retRace -> pointer where will be the choosed race */
      raceWindow(guiInterface* inter, race** retRace);

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

      void setCharacteristics();

      void setDescription();

};

#endif
