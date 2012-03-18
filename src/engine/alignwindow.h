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

#ifndef _dnt_alignwindow_h
#define _dnt_alignwindow_h

#include "../lang/translate.h"
#include "../gui/farso.h"
#include "../classes/align.h"

#define ALIGNW_CANCEL  0
#define ALIGNW_CONFIRM 1
#define ALIGNW_OTHER   2

/*! Alignments Window */
class alignWindow
{
   public:
      /*! Constructor
       * \param alg -> alignmnet list
       * \param inter -> GUI interface to use
       * \param actual -> pointer to the actual alignent */
      alignWindow(aligns* alg, Farso::GuiInterface* inter, align** actual);

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return 0 on close, 1 otherwise */
      int treat(Farso::GuiObject* object, int eventInfo, 
            Farso::GuiInterface* inter );

      Farso::Window* intWindow;        /**< Pointer to the internal window */

   private:
      aligns*      externalAligns;   /**< Pointer to the Aligns List */
      align*       actualAlign;      /**< Pointer to actual aligned */
      align**      choosedAlign;     /**< Pointer to the pointer of choosed */

      Farso::Button* buttonConfirm;          /**< Confirm Button */
      Farso::Button* buttonCancel;           /**< Cancel Button */

      Farso::Button* buttonPrevious;         /**< Previous Button */
      Farso::Button* buttonNext;             /**< Next Button */

      Farso::TextBox* textName;             /**< Text Name */
      Farso::RolBar* textDesc;              /**< Text Description */

      Farso::Picture* alignImage;           /**< Alignment Image */

};

#endif
