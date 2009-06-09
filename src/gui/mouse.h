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


#ifndef _farso_mouse_h
#define _farso_mouse_h

/*! Verify if the mouse coordinate is indoor to the are defined
 * \param x1 -> x1 coordinate
 * \param y1 -> y1 coordinate
 * \param x2 -> x2 coordinate
 * \param y2 -> y2 coordinate
 * \param mouseX -> mouse X coordinate
 * \param mouseY -> mouse Y coordinate 
 * \return true id on area, false otherwise */
bool isMouseAt(int x1,int y1,int x2,int y2, int mouseX, int mouseY);

#endif

