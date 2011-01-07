/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <string>
using namespace std;

#define CURSOR_WALK         0 /**< Walk Mouse Cursor */
#define CURSOR_ATTACK       1 /**< Attack Mouse Cursor */
#define CURSOR_DEFEND       2 /**< Defend Mouse Cursor */
#define CURSOR_MAPTRAVEL    3 /**< Map Travel Mouse Cursor */
#define CURSOR_TALK         4 /**< Talk Mouse Cursor */
#define CURSOR_GET          5 /**< Get Mouse Cursor */
#define CURSOR_INVENTORY    6 /**< Inventory Mouse Cursor */
#define CURSOR_DOOR         7 /**< Door Mouse Cursor */
#define CURSOR_FORBIDDEN    8 /**< Forbidden Mouse Cursor */
#define CURSOR_WALK_CONT    9 /**< Continuous Walk Cursor */
#define CURSOR_USE         10 /**< Object Use */
#define CURSOR_USER_IMAGE  11 /**< Any user image seted as cursor */
#define CURSOR_TOTAL       12 /**< Total number of mouse cursors */

#define DNT_CURSOR_MAX_TEXT_OVER_TIME  220   /**< After 60ms if no update */

/*! A mouse cursor class.*/
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
      void setTextOver(string txt);
      
   private:
      static GLuint texture[CURSOR_TOTAL]; /**< Cursors Textures */
      static float sizeX[CURSOR_TOTAL];    /**< Cursors Widths */
      static float sizeY[CURSOR_TOTAL];    /**< Cursors Heights */
      static float propX[CURSOR_TOTAL];    /**< X Proportion */
      static float propY[CURSOR_TOTAL];    /**< Y Proportion */
      static float hotSpot[CURSOR_TOTAL][2]; /**< HotSpot */
      static float scaleFactor[CURSOR_TOTAL]; /**< Scale Factor */
      static int currentCursor;            /**< Current Cursor Index */
      
      static string textOver;              /**< Text over cursor */
      static GLuint textOverTexture;       /**< Texture of text over */
      static int textOverWidth;            /**< Width of text over texture */
      static int textOverHeight;           /**< Height of text over texture */
      static Uint32 textOverInit;          /**< Time inited the display */

      /*! Load Cursor file 
       * \param fileName -> file name of cursor
       * \param index -> internal cursor index */
      void loadCursor(string fileName, int index);

      
};

#endif

