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

#ifndef _farso_menu_h
#define _farso_menu_h

#include <stdlib.h>
#include <string>

#include "color.h"
#include "draw.h"
#include "dntfont.h"
#include "mouse.h"
#include "guiobject.h"
#include "guilist.h"

#ifdef FARSO_USE_DNT_SOUND
   #include "../sound/sound.h"
#endif



namespace Farso
{

/*! The Menu GUI class */
class Menu: public GuiList
{
   public:

      static const int ITEM_HEIGHT=12;

      /*! Constructor
       * \param xa -> x coordinate
       * \param ya -> y coordinate
       * \param sWidth -> width usefull of the surface 
       * \param sHeight -> height usefull of the surface
       * \param surface -> surface where the menu is
       * \note: the usefull width and height are due that an window surface
       *        is greater of the window itself (as OpenGL powerOfTwo 
       *        textures restriction) */
      Menu(int xa, int ya, int sWidth, int sHeight, SDL_Surface* surface);

      /*! Destructor */
      ~Menu();

      /*! Insert a Item on the Menu
       * \param text -> item Text
       * \param avaible -> if the item is avaible or not */
      void insertItem(std::string text, bool avaible);

      /*! Insert a Item on the Menu
       * \param text -> item Text
       * \param imageFile -> item Image
       * \note imageFile must be of Size: 10x10 pixels
       * \param avaible -> if the item is avaible or not */
      void insertItem(std::string text, std::string imageFile, bool avaible);

      /*! Set the Position of the Menu
       * \param xa -> x coordinate
       * \param ya -> y coordinate */
      void setPosition(int xa, int ya);

      /*! Do the execution of the menu. Return selected item or -1,
       * \param mouseX -> X mouse coordinate
       * \param mouseY -> Y mouse coordinate
       * \param Mbotao -> mouse button state
       * \param teclado -> keyboard state
       * \param pronto -> it will be 1 when the execution ends 
       * \param Xjan -> x coordinate of the window
       * \param Yjan -> y coordinate of the window */
      int run(int mouseX, int mouseY, Uint8 Mbotao, Uint8* teclado,
              int* pronto, int Xjan, int Yjan);

      /*! Get a item
       * \param i -> position of the item on menu
       * \return pointer to desired item, or NULL, if not exists. */
      GuiObject* getItem(unsigned i);

      /*! Get the actual Item */
      int getActualItem();

      /*! Get the actual max character */
      int getMaxCharac();

      /*! Verify if the item is available
       * \param item -> item number on the menu
       * \return -> true if is available, false otherwise */
      bool itemAvailable(int item);

      /*! Set the item avaible status
       *  \param item -> item number on the menu
       *  \param av -> if available or not */
      void setItemAvailable(int item, bool av);

   protected:
      /*! Draw the Menu
       * \param pos -> menu position selected  */
      void draw(int pos);

      int x,                   /**< Menu X Coordinate */
          y;                   /**< Menu Y Coordinate */   
      unsigned int maxCharac;  /**< Size of the bigger string on menu */
      Colors colors;           /**< Colors */
      int numPictures;         /**< Number of pictures at the menu */
      int actualItem;          /**< Actual Selected item */
      bool pressed;            /**< Control to take the release button press */
      SDL_Surface* wSurface;   /**< Surface where the menu is */
      int wWidth;              /**< Usefull Surface Width */
      int wHeight;             /**< Usefull Surface Height */
#ifdef FARSO_USE_DNT_SOUND
      sound snd;            /**< Sound engine used */
#endif

};

}

#endif

