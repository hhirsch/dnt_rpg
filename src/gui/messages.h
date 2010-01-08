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

#ifndef _farso_messages_h
#define _farso_messages_h

#include "farso.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;

/*! The warning class is used to show messages to the user */
class warning
{
   public:
      /*! Show a message to user
       * \param title -> title of the window
       * \param message -> message to show to user
       * \param gui -> pointer to the current interface */
      void show(string title, string message, guiInterface* gui);

      /*! Get the okButton pointer
       * \return -> pointer to the okButton */
      button* getOkButton();

      /*! Verify if the warnWindow is opened
       * \return -> true, if opened, false otherwise */
      bool isOpened();

      /*! Close the warning window */
      void close();

   protected:
      static window* warnWindow;         /**< The warning window */
      static guiInterface* lastGui;      /**< The guiInterface in use */
      static button* okButton;           /**< The Ok Button */
};

#endif

