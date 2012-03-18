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

#ifndef _farso_textbar_h
#define _farso_textbar_h

#include <stdlib.h>

#include "color.h"
#include "draw.h"
#include "dntfont.h"
#include "mouse.h"
#include "guiobject.h"

namespace Farso
{

/*! Text Bar Definition */
class TextBar: public GuiObject
{
   public:
      /*! Constructor
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate 
       * \param yb -> y2 coordinate
       * \param text1 -> textBar text
       * \param cripto -> if is crypt, or not
       * \param screen -> surface to draw to */
      TextBar(int xa,int ya,int xb,int yb, std::string text1, bool cripto,
              SDL_Surface* screen);
      /*! Destructor */
      ~TextBar();
      
      /*!
       * Draw the text bar on a surface */
      void draw();

      /*!
       * Make the write thing on text bar
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate
       * \param Mbotao -> SDL mouse buttons state info
       * \param teclas -> SDL keyboard state info */
      int doWrite(int mouseX, int mouseY, Uint8 Mbotao, Uint8* teclas);

      /*! Set the current text at the bar
       * \param txt -> new text to the bar */
      void setText(std::string txt);

      /*! Define current cursor position, based on mouse position
       * (for example at a click inside the bar) 
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate */
      void defineCursorPosition(int mouseX, int mouseY);

   private:
      /*! Put the text on the surface
       * \param pos -> current text position
       * \param marca -> current mark position */
      void putText(unsigned int pos, int marca);

      bool cript;           /**< Pseudo-criptography? */
      int init,             /**< Actual first visible character on string */
          end;              /**< Actual last visible character on string */
      unsigned int pos;     /**< Current cursor position */
      int lastWrite;        /**< Last time when write */
      std::string lastChar; /**< Last Character writed */
      Colors colors;        /**< Internal Colors */   
};

}
 
#endif
