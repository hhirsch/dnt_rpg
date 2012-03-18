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

#ifndef _dnt_featswindow_h
#define _dnt_featswindow_h

#include "../gui/farso.h"
#include "../classes/classes.h"
#include "../classes/feats.h"

#include "character.h"
#include "featsel.h"

/*! The feats selector window/class */
class featsWindow
{
   public:
      /*! Constructor
       * \param interf -> gui interface to open the window */
      featsWindow(Farso::GuiInterface* interf);

      /*! Destructor */
      ~featsWindow();

      /*! Open the window of available feats for a character
       * \param pers -> character to put feats to
       * \param selectFeat -> if the window will be opened to select a feat */
      void open(character* pers, bool selectFeat=false);

      /*! Close the window (if opened) */
      void close();

      /*! Treat Events on Window. 
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return != 0 if event is gathered */
      int treat(Farso::GuiObject* object, int eventInfo);

      /*! Verify if the window is openned
       * \return true if opened, false otherwise. */
      bool isOpen();

      /*! Return the last selected feat */
      feat* getLastSelectedFeat();

   protected:
      /*! Write about the feat */
      void writeAboutFeat();

      /*! Get an usable feat (ON_TARGET or ON_AREA)
       * \param prev -> previous usable feat got
       * \return usable feat number or -1 */
      int getUsableFeat(int prev);

      /*! Same as getUsableFeat, but search backwards
       * \param prev -> previous usable feat got
       * \return feat number or -1 */
      int getPreviousUsableFeat(int prev);

      Farso::GuiInterface* usedGui;   /**< Pointer to the gui used */
      character* curCharacter; /**< Current character to show feats */
      int curFeat;             /**< Current displayed feat */
      bool selectFeatMode;     /**< If is at select feat mode */

      Farso::Window* intWindow;        /**< Window used */
      Farso::Picture* featIcon;        /**< The feature Image */
      Farso::TextBox* featTitle;       /**< Feat title text */
      Farso::RolBar* featDescription;  /**< Feat desciprion text */
      Farso::Button* buttonNext;       /**< Next Feat button */
      Farso::Button* buttonPrevious;   /**< Prevous Feat Button */
      Farso::Button* buttonClose;      /**< Close Window button */
};


#endif

