/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2010 DNTeam <dnt@dnteam.org>
 
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

#ifndef _dnt_shortcutswindow_h
#define _dnt_shortcutswindow_h

#include "../gui/farso.h"
#include "../classes/feats.h"

#include "featswindow.h"

#define SHORTCUTS_WINDOW_NONE              -1
#define SHORTCUTS_WINDOW_OTHER              0
#define SHORTCUTS_WINDOW_MENU               1
#define SHORTCUTS_WINDOW_LOAD               2
#define SHORTCUTS_WINDOW_SAVE               3
#define SHORTCUTS_WINDOW_ATTACK_MODE        4
#define SHORTCUTS_WINDOW_JOURNAL            5
#define SHORTCUTS_WINDOW_MAP                6
#define SHORTCUTS_WINDOW_INVENTORY          7
#define SHORTCUTS_WINDOW_REST               8
#define SHORTCUTS_WINDOW_GROUP              9
#define SHORTCUTS_WINDOW_CHARACTER         10
#define SHORTCUTS_WINDOW_END_TURN          11
#define SHORTCUTS_WINDOW_SELECTED_TALENT   12

#define QUICK_FEATS    10

/*! The shortcuts window, if quick access buttons and some
 * text infos. */
class shortcutsWindow
{
   public:
      /*! Constructor */
      shortcutsWindow();
      /*! Destructor */
      ~shortcutsWindow();

      /*! Open the window 
       * \param gui -> interface used */
      void open(guiInterface* gui);
      /*! Close the window 
       * \param gui -> interface used */
      void close(guiInterface* gui);
      /*! Verify if the window is opened
       * \return -> true if the window is opened */
      bool isOpened();
      /*! Reopen the Window (if is opened) 
       * \param gui -> pointer to the used guiInterface */ 
      void reOpen(guiInterface* gui);

      /*! Treat the window events
       * \param object -> current active gui object
       * \param eventInfo -> current event 
       * \param engineMode -> current engine mode
       * \param activeCharacter -> pointer to the current active Character
       * \return -> shortcutsWindow return value constant */
      int treat(guiObject* object, int eventInfo, int engineMode,
            character* activeCharacter);

      /*! Set the thing bellow mouse
       * \param thingName -> the name of the thing */
      void setThing(string thingName);
      /*! Set the total number of particles active
       * \param total -> number of particles */
      void setParticlesNumber(int total);
      /*! Set the current FPS value
       * \param fps -> current FPS value */
      void setFPS(float fps);
      /*! Set the hour text
       * \param hour -> new hour text */
      void setHour(string hour);

      /*! Get the current selected quick talent (if any)
       * \return pointer to the selected talent or NULL */
      feat* getSelectedTalent();
      /*! Clear th current selected talent */
      void clearSelectedTalent();

   protected:
      static guiInterface* guiUsed;          /**< the used guiInterface */

      static textBox* fpsTxt;                /**< shows FPS */
      static textBox* partTxt;               /**< shows Particles count */
      static textBox* thingTxt;              /**< shows thing at mouse */
      static textBox* hourTxt;               /**< shows Hour */

      static button* buttonMenu;             /**< call Menu */
      static button* buttonSave;             /**< call Save Function */
      static button* buttonLoad;             /**< call Load Function */
      static oneTabButton* buttonAttackMode; /**< Enter on Attack Mode */
      static oneTabButton* buttonJournal;    /**< Open Journal Window */
      static oneTabButton* buttonMap;        /**< Call MiniMap */
      static oneTabButton* buttonInventory;  /**< Call Inventory */
      static oneTabButton* buttonRest;       /**< Call Rest Function */
      static oneTabButton* buttonGroup;      /**< open group window */
      static oneTabButton* buttonCharacter;  /**< open Character Window */
      static oneTabButton* buttonEndTurn;    /**< End Turn Button */

      static oneTabButton* buttonQuickFeat[QUICK_FEATS];  /**< Feats buttons */
      static picture* picQuickFeat[QUICK_FEATS];  /**< Feats icons */
      static feat* quickFeat[QUICK_FEATS];   /**< Quick access feats */

      static featsWindow* talentWindow;      /**< Window to select talent */
      static int curDefinedTalent;           /**< Current quick talent to 
                                                  define at talentWindow */
      static int curSelectedTalent;          /**< Current talent 
                                                  selected to use */

      static window* shortCutsWindow;        /**< ShortCuts Window */
};

#endif

