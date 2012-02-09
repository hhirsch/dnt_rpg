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

#ifndef _dnt_comic_page_h
#define _dnt_comic_page_h

#include "../etc/list.h"

#include "comicbox.h"

/*! The Comic Page is the representation of a page,
 * including all comic boxes and its effects */
class comicPage: public dntListElement
{
   public:
      /*! Constructor */
      comicPage();
      /*! Destructor */
      ~comicPage();

      /*! Insert a comic box at the page
       * \param box -> comicBox to insert */
      void insertBox(comicBox* box);

      /*! Insert (draw) a text to the page texture
       * \param x1 -> x first coordinate 
       * \param y1 -> y first coordinate
       * \param x2 -> x last coordinate
       * \param y2 -> y last coordinate
       * \param text -> string with text to insert */
      void insertText(int x1, int y1, int x2, int y2, std::string text); 
      
      /*! Insert (draw) a text to the page texture
       * \param x1 -> x first coordinate 
       * \param y1 -> y first coordinate
       * \param x2 -> x last coordinate
       * \param y2 -> y last coordinate
       * \param text -> string with text to insert
       * \param fontSize -> font size to use 
       * \param fontStyle -> font style to use 
       * \param R -> red color
       * \param G -> green color
       * \param B -. blue color */
      void insertText(int x1, int y1, int x2, int y2, std::string text,
                      int fontSize, int fontStyle, int R, int G, int B);

      /*! Set the texture
       * \textureFile -> image file to use as texture
       * \return -> true if can load the image */
      bool defineTexture(std::string textureFile);

      /*! Flush the SDL texture to an openGL texture */
      void flushTexture();

      /*! Get the first box on the list
       * \return -> pointer to the first box */
      comicBox* getFirstBox();
      /*! Get total boxes
       * \return -> total number of boxes on page */
      int getTotalBoxes();

      /*! Render the page (updating the active box) */
      void render();

      /*! Get the current page texture width
       * \return -> page width */
      int getWidth();

      /*! Get the current page texture height
       * \return -> page height */
      int getHeight();

   protected:

      SDL_Surface* texture; /**< Page SDL Texture */
      GLuint tex;           /**< Page OpenGL Texture */

      comicBoxList boxes;   /**< Boxes List */
};

/*! The list of comic pages */
class comicPageList: public dntList
{
   public:
      /*! Constructor */
      comicPageList();
      /*! Destructor */
      ~comicPageList();

   protected:
      /*! Free element
       * \param obj -> pointer to the comicPage to delete */
      void freeElement(dntListElement* obj);
};

#endif


