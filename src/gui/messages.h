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

#ifndef _farso_messages_h
#define _farso_messages_h

#include "farso.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <string>

namespace Farso
{

/*! The warning class is used to show messages to the user */
class Warning
{
   public:
      /*! Show a message to user
       * \param title -> title of the window
       * \param message -> message to show to user
       * \param gui -> pointer to the current interface */
      void show(std::string title, std::string message, GuiInterface* gui);

      /*! Get the okButton pointer
       * \return -> pointer to the okButton */
      Button* getOkButton();

      /*! Verify if the warnWindow is opened
       * \return -> true, if opened, false otherwise */
      bool isOpened();

      /*! Close the warning window */
      void close();

   protected:
      static Window* warnWindow;         /**< The warning window */
      static GuiInterface* lastGui;      /**< The guiInterface in use */
      static Button* okButton;           /**< The Ok Button */
};

}

#endif

