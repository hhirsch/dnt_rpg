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

#ifndef _dnt_level_up_h
#define _dnt_level_up_h

#include "character.h"

/*! The levelUp class is used to level up characters when its
 * experience points reached enought level. */
class levelUp
{
   public:
      /*! Constructor
       * \param c -> pointer to character to level up */
      levelUp(character* c);
      /*! Destructor */
      ~levelUp();

      /*! Do the level Up process (calling the needed windows, etc) */
      void doLevelUp(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

   protected:
      character* current;
      int state;
};

#endif

