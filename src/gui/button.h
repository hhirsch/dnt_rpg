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

#ifndef _farso_button_h
#define _farso_button_h


#include <string>

#include "guiobject.h"
#include "color.h"
#include "draw.h"
#include "mouse.h"

#ifdef FARSO_USE_DNT_SOUND
   #include "../sound/sound.h"
#endif

namespace Farso
{

class Menu;

/*! The button class */
class Button: public GuiObject
{
   public:

      enum ButtonType
      {
         NORMAL,
         KEEP_PRESSING
      };

      /*! Constructor
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate 
       * \param txt -> button text
       * \param isOval -> if the button is oval or rectangular */
      Button(int xa,int ya,int xb,int yb, std::string txt, bool isOval, 
            SDL_Surface* surface);

      /*! Destructor */
      ~Button();

      /*! Set button type
       * \param t -> buttonType constant */
      void setType(int t);

      /*! Draw the Button on wSurface */
      void draw();

      /*! Do the press proccess of the button.
       * \param Xjan -> window x1 coordinate 
       * \param Yjan -> window y1 coordinate
       * \param x -> mouseX coordinate
       * \param y -> mouseY coordinate
       * \param pronto -> will be != 0 when end the proccess
       * \param screen -> surface where draws the button
       * \param Mbotao -> mouse button state
       * \return true when pressed, false if not pressed. */
       bool press(int Xjan, int Yjan, int x, int y, Uint8 Mbotao, int* pronto);


      /*! Define the font to use for the button
       * \param fileName -> fileName of the font to use
       * \param size -> size of the font to use */
      void defineFont(std::string fileName, int size);

      /*! Set the text color of the button
       * \param r -> red
       * \param g -> green
       * \param b -> blue */
      void setTextColor(int r, int g, int b);

      Menu* men;        /**< Button Menu (buttons can have a menu to be 
                                          activated when button presses) */

   protected:
      bool pressed;         /**< If the button is pressed or not */
      bool oval;            /**< Oval Button?  */
      Colors colors;        /**< Colors */
      std::string fontName; /**< Font Name */
      int fontSize;         /**< Font Size */
      int bType;            /**< Button type */
#ifdef FARSO_USE_DNT_SOUND
      sound snd;            /**< Sound engine used */
#endif
};

}

#endif

