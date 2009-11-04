/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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

#ifndef _farso_seltext_h
#define _farso_seltext_h

#include <string>
using namespace std;

#include "draw.h"
#include "dntfont.h"
#include "color.h"
#include "mouse.h"
#include "guiobject.h"

#define MAX_OPTIONS 5  /**< Max Options on Sel Text */

/*! Select Text Class (from 1 to 5 texts) */
class selText: public guiObject
{
   public:
      /*! Constructor
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate
       * \param text0 -> option 0 text
       * \param text1 -> option 1 text
       * \param text2 -> option 2 text
       * \param text3 -> option 3 text
       * \param text4 -> option 4 text
       * \param screen -> the window screen used */
      selText(int xa,int ya,int xb,int yb, string text0, string text1,
              string text2, string text3, string text4, SDL_Surface* screen);

      /*! Destructor */
      ~selText();

      /*! Set the SelText coordinates
       * \param xa -> x1 coordinate
       * \param xb -> x2 coordinate
       * \param ya -> y1 coordinate
       * \param yb -> y2 coordinate */
      void setCoordinate(int xa,int ya,int xb,int yb);

      /*! Get the selText coordinates
       * \param xa -> x1 coordinate
       * \param xb -> x2 coordinate
       * \param ya -> y1 coordinate
       * \param yb -> y2 coordinate */
      void getCoordinate(int& xa, int& ya, int& xb, int& yb);


      /*! Draw the seltext on a surface
       * \param screen -> surface to draw */
      void draw(SDL_Surface *screen);

      /*! Treat the selText
       * \param screen  -> surface where the selText is
       * \param xa -> mouse X coordinate
       * \param ya -> mouse Y coordinate
       * \param Mbotao -> mouse button state*/
      int treat(int xa,int ya, Uint8 Mbotao, SDL_Surface *screen);

      /*! Get the last selected item 
       * \param info -> pointer to a integer to get the info or NULL
       * \return -> last selected item */
      int getLastSelectedItem(int* info = NULL);

      /*! Set the text of an option
       * \param opt -> option number
       * \param txt -> option txt 
       * \param info -> option info to store
       * \note -> at info you can store, for example, external vector 
       *          position of the element */
      void setText(int opt, string txt, int info=0);

      /*! Clear the selText options text */
      void clearText();

   private:
      /*! Write to the surface the selected item on differente color.
       * \param selectItem -> number of the selected text
       * \param screen -> surface to draw */
      void writeSelected(int selectItem, SDL_Surface *screen);

      /*! based on y, get the seleted item
       * \param ya -> mouse y coordinate
       * \return -1 if no item selected */
      int getSelectedItem(int ya);

   protected:
      int y[MAX_OPTIONS];          /**< Final Y of each option */
      string optText[MAX_OPTIONS]; /**< Text Options */
      int optInfo[MAX_OPTIONS];    /**< Info Options */
      int selec;                   /**< last item selected */
      farso_colors Cores;          /**< colors */
      bool pressed;                /**< while pressed == true */
      SDL_Surface* windowScreen;   /**< Pointer to the window screen */


};

#endif


