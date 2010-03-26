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

#ifndef _farso_picture_h
#define _farso_picture_h

#include "draw.h"
#include "guiobject.h"

/*! The Picture Class. A picture is just a image on the GUI */
class picture: public guiObject
{
   public:
      /*! Destructor */
      ~picture();

      /*! Constructor
       * \param x -> x initial coordinate
       * \param y -> y initial coordinate
       * \param w -> width of the picture
       * \param h -> height of the picture
       * \param surface -> surface to draw to
       * \param arquivo -> fileName to opwn or NULL to not open a file */
      picture(int x,int y,int w,int h,const char* arquivo, 
            SDL_Surface* surface);

      /*! Draw the Picture on the surface */
      void draw();

      /*! Set the picture to the surface
       * \param newPicture -> surface to set the picture */
      void set(SDL_Surface* newPicture);

      /*! Set surface deletion at object's destructor
       * \param enable -> true to delete the surface at the destructor
       * \note Defaut == true. */
      void setSurfaceDeletion(bool enable);

      /*! Get the surface of the picture
       * \return -> surface of the picture */
      SDL_Surface* get();

      /*! Set the picture visible area
       * \param xa -> left
       * \param ya -> up
       * \param xb -> right 
       * \param yb -> bottom */
      void setVisibleArea(int xa, int ya, int xb, int yb);

      /*! Set the picture as all visible */
      void setAllVisible();


   protected:
      SDL_Surface *fig;     /**< A picture surface */
      SDL_Rect area;        /**< The visible area */
      bool deleteSurface;   /**< true will delete the surface at destructor */

};


#endif

