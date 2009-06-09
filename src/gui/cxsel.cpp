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

#include "cxsel.h"

/**********************************************************
 *                       Constructor                      *
 **********************************************************/
cxSel::cxSel(int x, int y):guiObject()
{
   x1 = x;
   y1 = y;
   x2 = x+10;
   y2 = y+10;
   available = true;
   selected = false;
   lastChangeTime = SDL_GetTicks();
   type = FARSO_OBJECT_SEL_BOX;
}

/**********************************************************
 *                           draw                         *
 **********************************************************/
void cxSel::draw(SDL_Surface *screen)
{
   color_Set(Colors.colorCont[0].R, Colors.colorCont[0].G, 
             Colors.colorCont[0].B, Colors.colorCont[0].A);
   rectangle_2Colors(screen, x1, y1, x1+10, y1+10, Colors.colorCont[1].R,
                     Colors.colorCont[1].G, Colors.colorCont[1].B,
                     Colors.colorCont[1].A);
   color_Set(Colors.colorCont[2].R,Colors.colorCont[2].G,Colors.colorCont[2].B,
             Colors.colorButton.A);
   rectangle_Fill(screen, x1+1, y1+1, x1+9, y1+9);
   if( (available) && (selected) )
   {
      color_Set(Colors.colorBar.R, Colors.colorBar.G, 
                Colors.colorBar.B, Colors.colorCont[0].A);
      line_Draw(screen, x1+2, y1+2, x1+8, y1+8);
      line_Draw(screen, x1+1, y1+2, x1+7, y1+8);
      line_Draw(screen, x1+3, y1+2, x1+9, y1+8);
      line_Draw(screen, x1+8, y1+2, x1+2, y1+8);
      line_Draw(screen, x1+7, y1+2, x1+1, y1+8);
      line_Draw(screen, x1+9, y1+2, x1+3, y1+8);
   }
}

/**********************************************************
 *                       isSelected                       *
 **********************************************************/
bool cxSel::isSelected()
{
   return(selected);
}

/**********************************************************
 *                     invertSelection                    *
 **********************************************************/
void cxSel::invertSelection()
{
   GLuint actualTime = SDL_GetTicks();
   if((actualTime - lastChangeTime) >= 100)
   {
      setChanged();
      selected = !selected;
      lastChangeTime = actualTime;
   }
}

/**********************************************************
 *                       setSelection                     *
 **********************************************************/
void cxSel::setSelection(bool value)
{
   setChanged();
   selected = value;
}

