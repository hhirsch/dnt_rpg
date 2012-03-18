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

#ifndef _dnt_feat_sel_h
#define _dnt_feat_sel_h

#include "../gui/farso.h"
#include "character.h"

#define TALENT_WINDOW_CANCEL     0
#define TALENT_WINDOW_CONFIRM    1
#define TALENT_WINDOW_OTHER      2

#define FEATS_PER_PAGE  5

/*! The featSelWindow defines a window where the user can select new
 * available talents to its character */
class featSelWindow
{
   public:
      /*! Constructor
       * \param interf -> gui interface to open the window */
      featSelWindow(Farso::GuiInterface* interf);

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
      int treat(Farso::GuiObject* object, int eventInfo);

      /*! Verify if the window is openned
       * \return true if opened, false otherwise. */
      bool isOpen();

      /*! Apply all new permanent feat */
      void applyAllNewPermanentFeats();

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
            /*! Get the fSelFeat from list
             * \param featId -> Id of the feat to get
             * \return poiter to the feat got (or NULL) */
            fSelFeat* getFeat(std::string featId);
         protected:
            /*! Free Element
             * \param obj -> pointer to the
             * depFeat to delete */
            void freeElement(dntListElement* obj);
      };

      /*! Define all available feats for the current character */
      void defineAvailableFeats();

      /*! Draw all related gui with feats
       * \param f -> feat to show description */
      void drawThings(fSelFeat* f);
     
      Farso::Window* intWindow;         /**< Pointer to the internal window */
      Farso::GuiInterface* inter;       /**< Current GUI interface */
      character* current;        /**< Current opened character */
      int curTotal;              /**< Current total */

      featsList* allFeats;         /**< List of all feats in game */
      fSelFeatList availableFeats; /**< List of available feats */
      fSelFeatList selectedFeats;  /**< List of selected feats */

      Farso::TextBox* textAvail[FEATS_PER_PAGE];/**< Available feats text */
      Farso::Picture* picAvail[FEATS_PER_PAGE]; /**< Available feats picture */
      Farso::Button* buttonInsert[FEATS_PER_PAGE];/**< Insert Available 
                                                       feat buttons */
      fSelFeat* curAvail[FEATS_PER_PAGE];  /**< Current available feats */
      Farso::Button* nextAvailButton;      /**< Next Page Button on available */
      Farso::TextBox* textAvailPage;       /**< Page of available feats */
      Farso::Button* prevAvailButton; /**< Previous Page Button on available */
      Farso::TextBox* textSel[FEATS_PER_PAGE]; /**< Selected feats text */
      Farso::Picture* picSel[FEATS_PER_PAGE];  /**< Selected feats picture */
      Farso::Button* buttonRemove[FEATS_PER_PAGE];/**< Remove Selected 
                                                       feat buttons */
      fSelFeat* curSel[FEATS_PER_PAGE];    /**< Current selected feats */
      Farso::Button* nextSelButton;        /**< Next Page button on selected */
      Farso::TextBox* textSelPage;         /**< Page of selected feats */
      Farso::Button* prevSelButton;     /**< Previous page button on selected */
      Farso::TextBox* textTotal;          /**< Total remaining text */

      int curSelPage;                      /**< Current selected page */
      int curAvailPage;                    /**< Current available page */

      Farso::TextBox* textTitle;             /**< Feat title */
      Farso::RolBar* textDescription;        /**< Feat description */
      Farso::Picture* picFeat;               /**< Feat image */


      Farso::Button* okButton;          /**< The ok Button */
      Farso::Button* cancelButton;      /**< The cancel button */
};

#endif

