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

#ifndef _dnt_wind_h
#define _dnt_wind_h

#include <SDL/SDL_opengl.h>

/*! The wind controller class  */
class wind
{
   public:
      /*! The wind constructor */
      wind();
      /*! The window destructor */
      ~wind();

      /*! Set the wind velocity
       * \param vX -> wind X velocity 
       * \param vY -> wind Y velocity
       * \param vZ -> wind Z velocity */
      void setVelocity(GLfloat vX, GLfloat vY, GLfloat vZ);

      /*! Get the wind velocity
       * \param vX -> wind X velocity 
       * \param vY -> wind Y velocity
       * \param vZ -> wind Z velocity */
      void getVelocity(GLfloat& vX, GLfloat& vY, GLfloat& vZ);

   protected:
      GLfloat velX,           /**< Current wind X velocity */
              velY,           /**< Current wind Y velocity */
              velZ;           /**< Current wind Z velocity */
};

#endif

