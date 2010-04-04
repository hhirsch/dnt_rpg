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
      alignWindow(aligns* alg, guiInterface* inter, align** actual);

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return 0 on close, 1 otherwise */
      int treat(guiObject* object, int eventInfo, guiInterface* inter );

      window*      intWindow;        /**< Pointer to the internal window */

   private:
      aligns*      externalAligns;   /**< Pointer to the Aligns List */
      align*       actualAlign;      /**< Pointer to actual aligned */
      align**      choosedAlign;     /**< Pointer to the pointer of choosed */

      button* buttonConfirm;          /**< Confirm Button */
      button* buttonCancel;           /**< Cancel Button */

      button* buttonPrevious;         /**< Previous Button */
      button* buttonNext;             /**< Next Button */

      textBox* textName;             /**< Text Name */
      rolBar* textDesc;              /**< Text Description */

      picture* alignImage;           /**< Alignment Image */

};

#endif
