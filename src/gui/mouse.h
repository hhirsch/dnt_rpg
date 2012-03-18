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


#ifndef _farso_mouse_h
#define _farso_mouse_h

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#ifdef __APPLE__
   #include <SDL_image/SDL_image.h>
#else
   #include <SDL/SDL_image.h>
#endif

#include <string>
#include <list>

namespace Farso
{

#define MOUSE_CURSOR_NONE         "None"
#define MOUSE_CURSOR_USER_IMAGE   "User"

#define MOUSE_CURSOR_MAX_TEXT_OVER_TIME  220   /**< After 60ms if no update */

/*! A single mouse cursor information */
class SingleMouseCursor
{
   public:
      /*! Constructor */
      SingleMouseCursor();
      /*! Destructor */
      ~SingleMouseCursor();

      GLuint texture;          /**< Cursor texture */
      GLfloat size[2];         /**< Cursor Size */
      GLfloat prop[2];         /**< Cursor Proportion */
      GLfloat hotSpot[2];      /**< Cursor Active Position ("HotSpot") */
      GLfloat scaleFactor;     /**< Cursor Scale Factor */
      std::string fileName;    /**< Cursor FileName ("none" for using image) */
};

class MouseCursor
{
   public:
      /*! cursor Constructor */
      MouseCursor();
      /*! cursor Destructor */
      ~MouseCursor();

      /*! init the cursor use 
       * \note musr be called at engine init */
      void init();
      /*! Finish the cursor use
       * \note must be called at engine end*/
      void finish();

      /*! Set the current cursor to one defined by the image file name
       * \param imageFile -> cursor image file name
       * \param spotX -> cursor active X 
       * \param spotY -> cursor active Y
       * \note -> if not yet loaded, will load it to current cursors list */
      void set(std::string imageFile, int spotX, int spotY);

      /*! Set the current mouse Cursor to an image
       * \param img -> sdl surface to set as cursor */
      void set(SDL_Surface* img);

      /*! Draw the Cursor to screen */
      void draw(int mouseX, int mouseY, float angle=0.0f,
                float scaleX=1.0f, float scaleY=1.0f, float scaleZ=1.0f);

      /*! Set the text over the cursor
       * \param txt -> text to show or "" for no text */
      void setTextOver(std::string txt);

      /*! Hide the mouse cursor (must reset it to cursor to show again) */
      void hide();

      /*! Get the current cursor */
      std::string get();

      /*! Set the font used for text over
       * \param s -> font file name  */
      void setTextOverFont(std::string f);
      
   private:
      static SingleMouseCursor* current;   /**< Active cursor */
      static std::list<SingleMouseCursor*> cursors; /**< Loaded Cursors list */

      static std::string textOver;         /**< Text over cursor */
      static GLuint textOverTexture;       /**< Texture of text over */
      static int textOverWidth;            /**< Width of text over texture */
      static int textOverHeight;           /**< Height of text over texture */
      static Uint32 textOverInit;          /**< Time inited the display */
      static std::string font;             /**< Font used to display */ 

      /*! Load Cursor file 
       * \param fileName -> file name of cursor */
      SingleMouseCursor* loadCursor(std::string fileName, int hX, int hY);

      /*! Search for a loaded cursor of fileName image */
      SingleMouseCursor* search(std::string fileName);
 
};

/*! Verify if the mouse coordinate is indoor to the are defined
 * \param x1 -> x1 coordinate
 * \param y1 -> y1 coordinate
 * \param x2 -> x2 coordinate
 * \param y2 -> y2 coordinate
 * \param mouseX -> mouse X coordinate
 * \param mouseY -> mouse Y coordinate 
 * \return true id on area, false otherwise */
bool isMouseAt(int x1,int y1,int x2,int y2, int mouseX, int mouseY);

}

#endif

