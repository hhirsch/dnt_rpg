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

#ifndef _dnt_sky_h
#define _dnt_sky_h

#include "../map/map.h"
#include <SDL/SDL_opengl.h>

/*! Sky Control Class */
class sky
{
   public:
      /*! Constructor */
      sky();
      /*! Destructor */
      ~sky();

      /*! Draw Sky Dome 
       * \param actualMap -> pointer to the opened Map 
       * \param sunRot -> current Sun Rotation */
      void draw(Map* actualMap, GLfloat sunRot);

   private:

      /*! Draw Sky Dome
       * \param lats -> number of latitudes
       * \param longs -> number of longitudes */
      void drawDome(int lats, int longs);
      /*! Draw a sky box */
      void drawBox();

      GLuint skyMap;
      GLfloat solarTime;
      GLfloat color[3];

      GLuint boxTexture[4];
      GLfloat skySize;
};


#endif

