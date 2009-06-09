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

#ifndef _dnt_comicbook_h
#define _dnt_comicbook_h

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "comicpage.h"
#include "comicbox.h"

/*! The comic book class is the DNT cutscene abstraction */
class comicBook
{
   public:
      /*! Constructor */
      comicBook();
      /*! Destructor */
      ~comicBook();

      /*! Load a comicBook from file
       * \param bookFile -> comicBook file name 
       * \return -> true if the load was successfull */
      bool load(string bookFile);

      /*! Run the comic book */
      void run();

   protected:

      /*! Render the current page (with or without a scale effect)
       * \param curPage -> current page to render
       * \param scale -> scale factor to use */
      void render(comicPage* curPage, float scale=1.0);

      /*! Insert a page at the book pages list
       * \param t -> page title 
       * \return -> pointer to the comic page created */
      comicPage* insertPage(string t);

      /*! Empty the book, deleting all its pages */
      void empty();

      /*! Verify User Input to the comic (skip comic or box for example)
       * \return -> true if is to skip current comicBox */
      bool verifyInput();

      Uint8 mButton;      /**< Mouse Buttons State */
      int mouseX,         /**< Current MouseX */
          mouseY;         /**< Current MouseY */
      Uint8* keys;        /**< Current Keyboard state */

      GLuint skipTexture; /**< The Skip Texture */
      float skipScale;    /**< The skip scale factor */
      float skipSum;      /**< The sum to the skip scale */

      bool exit;          /**< True if pressed skip button o ESC key */
      bool changeColor;   /**< True to change skip texture color */

      comicPage* pages;   /**< Each comic book page  */
      int totalPages;     /**< Total book pages */
      string title;       /**< Book Title */
};

#endif

