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

#ifndef _dnt_xp_h
#define _dnt_xp_h

#include "../engine/character.h"

/*! Get the XP value for the actor doing something to target
 * the something can be a kill, a success on using skill, etc. 
 * \param actualActor -> pointer to the actor character 
 * \param targetCR -> Challenge Rate of the target 
 * \return total amount of XP. */
int getXP(character* actualActor, float targetCR);

/*! Get the next number of Experience Points needed to advance
 * a level, based on the current XP level.
 * \param curXP -> current number of experience points
 * \return -> needed experience points to advance to next level */
int nextLevelXP(int curXP);

/*! Get the character level respective to the XP amount
 * \param curXP -> current number of experience points 
 * \return -> character level relative to this XP quantity*/
int getXPLevel(int curXP);

#endif

