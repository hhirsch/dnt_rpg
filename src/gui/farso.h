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

#ifndef _farso_h
#define _farso_h


#include <stdlib.h>
#include <SDL/SDL.h>
#include "draw.h"
#include "button.h"
#include "color.h"
#include "cxsel.h"
#include "window.h"
#include "interface.h"
#include "dntfont.h"
#include "healthBar.h"
#include "textbox.h"
#include "rolbar.h"
#include "seltext.h"
#include "textbar.h"
#include "guiobject.h"
#include "guilist.h"
#include "picture.h"
#include "menu.h"
#include "mouse.h"
#include "tabbutton.h"
#include "messages.h"
#include "../lang/translate.h"

/*! Init the SDL/OpenGL Context
 * \param screen -> pointer to the pointer to the surface used as screen
 * \param title -> title of the screen
 * \param width -> screen width
 * \param height -> screen height
 * \param fullScreen -> true if is fullscreen
 * \param antiAliasingSample -> number of Antialising used (0 == disabled)
 * \param stencilBufferSize -> number of stencil buffers */
void Farso_Init(SDL_Surface **screen, string title, int width, int height,
                bool fullScreen, int antiAliasingSample, int stencilBufferSize);

/*! Define the SDL/OpenGL Context Resolution
 * \param screen -> pointer to the pointer to the surface used as screen
 * \param title -> title of the screen
 * \param width -> screen width
 * \param height -> screen height
 * \param fullScreen -> true if is fullscreen
 * \param antiAliasingSample -> number of Antialising used (0 == disabled) */
void Farso_DefineResolution(SDL_Surface **screen, string title, 
                            int width, int height,
                            bool fullScreen, int antiAliasingSample, 
                            int stencilBufferSize);

/*! End the SDL/OpenGL Context
 * \param screen -> surface used as screen */
void Farso_End(SDL_Surface *screen);


#endif

