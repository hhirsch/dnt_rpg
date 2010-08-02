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

#ifndef _dnt_feat_sel_h
#define _dnt_feat_sel_h

#include "../gui/farso.h"
#include "character.h"

/*! The featSelWindow defines a window where the user can select new
 * available talents to its character */
class featSelWindow
{
   public:
      /*! Constructor
       * \param interf -> gui interface to open the window */
      featSelWindow(guiInterface* interf);

      /*! Destructor */
      ~featSelWindow();

      /*! Open the window of available feats for a character
       * \param pers -> character to put feats to
       * \param fList -> lis of all game talents 
       * \param total -> number of new talents the user can select
       *                 (dispite race/class talents) */
      void open(character* pers, featsList* fList, int total);

      /*! Close the window (if opened) */
      void close();

      /*! Treat Events on Window. 
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return != 0 if event is gathered */
      int treat(guiObject* object, int eventInfo,
                GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! Verify if the window is openned
       * \return true if opened, false otherwise. */
      bool isOpen();

   protected:

      /*! The feat at selection */
      class fSelFeat: public dntListElement
      {
         public:
            featDescription* desc; /*< pointer to the feat */
            bool classFeat;        /*< true if is a race/class feat */
      };

      /*! The list of feats in selection */
      class fSelFeatList: public dntList
      {
         public:
            /*! Destructor */
            ~fSelFeatList();
         protected:
            /*! Free Element
             * \param obj -> pointer to the
             * depFeat to delete */
            void freeElement(dntListElement* obj);
      };
     
      window* intWindow;         /**< Pointer to the internal window */
      guiInterface* inter;       /**< Current GUI interface */
      character* current;        /**< Current opened character */

      fSelFeatList availableFeats; /**< List of available feats */
      fSelFeatList selectedFeats;  /**< List of selected feats */


      button* okButton;          /**< The ok Button */
      button* cancelButton;      /**< The cancel button */
};

#endif

