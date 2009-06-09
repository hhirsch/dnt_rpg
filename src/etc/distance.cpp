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

#include "distance.h"

GLfloat distancePointLine(GLfloat x0, GLfloat z0, GLfloat x1, GLfloat z1,
                          GLfloat x2, GLfloat z2)
{
   /* Take the distance, preserving the signal, to know the direction,
    * formula deducted from GAAL book (!) */
   GLfloat num = /*fabs*/( ((x1 - x2)*(z1 - z0)) - ((x1 - x0)*(z1 - z2)) );
   GLfloat den = sqrt( ((x1-x2)*(x1-x2)) + ((z1-z2)*(z1-z2)) );

   return(num / den);
}
