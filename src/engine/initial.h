/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_initial_h
#define _dnt_initial_h

#include "../gui/farso.h"
#include "../sound/sound.h"
#include "cursor.h"

/* initialScreen Returns Values */
#define EXIT_GAME     0   /**< Exit Game Return Value */
#define NEW_GAME      1   /**< New Game Return Value */
#define LOAD_GAME     2   /**< Load Game Return Value */
#define SAVE_GAME     3   /**< Save Return Value */
#define CONTINUE_GAME 4   /**< Continue Return Value */
#define OPTIONS       5   /**< Number of Options */

/* initialScreen Status */
#define ON_INIT 1         /**< When Call Class On Game Init */
#define IN_GAME 2         /**< When Call Class in game execution */

/*! DNT's initial screen */
class initialScreen
{
   public:
     /*! initialScreen Constructor */
     initialScreen();
     /*! initialScreen Destructor */
     ~initialScreen();

     /*! Run the initialScreen.
      * \param Status -> game status when called screen 
      *                  ( \c ON_INIT or \c IN_GAME )
      * \param proj -> projection Matrix
      * \param modl -> modelView matrix
      * \param viewPort -> current view Port
      * \param idTextura -> loaded initial screen texture id.
      * \param snd -> sound engine to use
      * \param interf -> guiInterface to use
      * \return initialScreen Return values correspondent to action made */
     int run(int Status, GLdouble proj[16],
             GLdouble modl[16], GLint viewPort[4],
             GLuint idTextura, sound* snd,
             Farso::GuiInterface* interf);
   private:
      Farso::GuiInterface* gui; /**< GUI for Windows */

      int result;        /**< Result of the initial Screen */
      bool done;
      
      Farso::Window* menuWindow;
      Farso::Button* buttonNew;
      Farso::Button* buttonContinue;
      Farso::Button* buttonSave;
      Farso::Button* buttonLoad;
      Farso::Button* buttonOptions;
      Farso::Button* buttonExit;
};

#endif

