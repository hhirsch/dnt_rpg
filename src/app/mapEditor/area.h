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

#ifndef _dnt_map_editor_area_h
#define _dnt_map_editor_area_h

#include <SDL/SDL_opengl.h>
#include <string>

namespace dntMapEditor
{

/*! Simple Definition of Map Area */
class Area
{
   public:
      GLfloat x1, y1, x2, y2; /**< Coordinates */
      std::string whereToGo;  /**< Destiny */
      Area* next;             /**< Next Node */
};

/*! List of Map Areas */
class AreaList
{
   public:
      /*! Constructor */
      AreaList();
      /*! Destructor */
      ~AreaList();

      /*! Search and Return an Area on the list */
      Area* getArea(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
      Area* getArea(GLfloat x, GLfloat y);

      /*! Add an area to the list */
      void addArea(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, 
                   std::string whereToGo);

      /*! Draw on screen all portals */
      void drawAll();

   private:
      Area* first; /**< First Node of the list */
      int total;   /**< Total Nodes of the list */
};

}

#endif

