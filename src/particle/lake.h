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

#ifndef _dnt_lake_h
#define _dnt_lake_h

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

/*! The lake class */
class lake
{
   public:
      /*! Constructor */
      lake();
      /*! Destructor */
      ~lake();

      /*! Define Lake Position
       * \param x1 -> x first coordinate
       * \param z1 -> z first coordinate
       * \param x2 -> x last coordinate
       * \param z2 -> z last coordinate */
      void definePosition(GLfloat x1, GLfloat z1, GLfloat x2, GLfloat z2); 

      /*! Get Lake Position
       * \param x1 -> x first coordinate
       * \param z1 -> z first coordinate
       * \param x2 -> x last coordinate
       * \param z2 -> z last coordinate */
      void getPosition(GLfloat &x1, GLfloat &z1, GLfloat &x2, GLfloat &z2);

      /*! Define Color
       * \param r -> red
       * \param g -> green 
       * \param b -> blue
       * \param a -> alpha */
      void defineColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

      /* Get Color
       * \param r -> red
       * \param g -> green 
       * \param b -> blue
       * \param a -> alpha */
      void getColor(GLfloat &r, GLfloat &g, GLfloat &b, GLfloat &a);
   
      /*! Draw the lake */
      void draw();

      lake* next;          /**< Next lake on the list */
      lake* previous;      /**< Previous lake on the list */

   protected:
      GLfloat pos[4];      /**< Lake position */
      GLfloat color[4];    /**< Lake Color */

      GLfloat *vertexBuffer;  /**< Lake Render Buffer */
      int numVertex;          /**< Number of vertexes on lake */

};

#endif

