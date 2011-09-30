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

#ifndef _dnt_mapfog_h
#define _dnt_mapfog_h

#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;

/*! Map Fog Class */
class mapFog
{
   public:
      GLfloat color[4]; /**< Fog Color */
      GLfloat density;  /**< Fog Density */
      GLfloat start;    /**< Fog Distant Start */
      GLfloat end;      /**< Fog Distant End */
      bool enabled;     /**< Fog Enabled? */
      string fileName;  /**< File name of the fog */

      /*! Constructor */
      mapFog();
      /*! Destructor */
      ~mapFog();
      /*! Load Fog from File
       * \param arq -> fog filename */
      void load(string arq);
      /*! Save fog to file */
      bool save();

      /*! Apply the fog values */
      void apply(bool outdoor, float ofarview, float ifarview);
     
};

#endif

