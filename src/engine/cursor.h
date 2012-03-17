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

#ifndef _dnt_cursor_h
#define _dnt_cursor_h

#include <string>

#include "../gui/mouse.h"

enum
{
   CURSOR_WALK=0,     /**< Walk Mouse Cursor */
   CURSOR_ATTACK,     /**< Attack Mouse Cursor */
   CURSOR_DEFEND,     /**< Defend Mouse Cursor */
   CURSOR_MAPTRAVEL,  /**< Map Travel Mouse Cursor */
   CURSOR_TALK,       /**< Talk Mouse Cursor */
   CURSOR_GET,        /**< Get Mouse Cursor */
   CURSOR_INVENTORY,  /**< Inventory Mouse Cursor */
   CURSOR_DOOR,       /**< Door Mouse Cursor */
   CURSOR_FORBIDDEN,  /**< Forbidden Mouse Cursor */
   CURSOR_WALK_CONT,  /**< Continuous Walk Cursor */
   CURSOR_USE,        /**< Object Use */
   CURSOR_TOTAL,      /**< Total number of mouse cursors */
   CURSOR_USER_IMAGE  /**< The special User Image cursor */
};

/*! The DNT mouse cursor class.*/
class cursor
{
   public:
      /*! cursor Constructor */
      cursor();
      /*! cursor Destructor */
      ~cursor();

      /*! init the cursor use 
       * \note musr be called at engine init */
      void init();
      /*! Finish the cursor use
       * \note must be called at engine end*/
      void finish();

      /*! Set current mouse Cursor 
       * \param nCursor -> cursor Number to use.*/
      void set(int nCursor);

      /*! Set the current mouse Cursor to an image
       * \param img -> sdl surface to set as cursor */
      void set(SDL_Surface* img);

      /*! Draw th Cursor to screen */
      void draw(int mouseX, int mouseY, float angle=0.0f,
                float scaleX=1.0f, float scaleY=1.0f, float scaleZ=1.0f);

      /*! Gets the actual mouse cursor
       * \return -> pointer to the surface of the mouse cursor */
      int get();

      /*! Set the text over the cursor
       * \param txt -> text to show or "" for no text */
      void setTextOver(std::string txt);

      /*! Hide the mouse cursor (must reset it to cursor to show again) */
      void hide();

      /*! Set the font used for text over
       * \param s -> font file name  */
      void setTextOverFont(std::string f);
      
   private:
      static int currentCursor;        /**< Current Cursor Index */
      static MouseCursor mCursor;      /**< The Farso Mouse Cursor */
      
};

#endif

