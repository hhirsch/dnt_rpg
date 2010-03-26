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

#include "cxsel.h"

/**********************************************************
 *                       Constructor                      *
 **********************************************************/
cxSel::cxSel(int x, int y, SDL_Surface* surface):guiObject(surface)
{
   x1 = x;
   y1 = y;
   x2 = x+10;
   y2 = y+10;
   available = true;
   selected = false;
   pressing = false;
   type = FARSO_OBJECT_SEL_BOX;
}

/**********************************************************
 *                           draw                         *
 **********************************************************/
void cxSel::draw()
{
   color_Set(Colors.colorCont[0].R, Colors.colorCont[0].G, 
             Colors.colorCont[0].B, Colors.colorCont[0].A);
   rectangle_2Colors(wSurface, x1, y1, x1+10, y1+10, Colors.colorCont[1].R,
                     Colors.colorCont[1].G, Colors.colorCont[1].B,
                     Colors.colorCont[1].A);
   color_Set(Colors.colorCont[2].R,Colors.colorCont[2].G,Colors.colorCont[2].B,
             Colors.colorButton.A);
   rectangle_Fill(wSurface, x1+1, y1+1, x1+9, y1+9);
   if( (available) && (selected) )
   {
      color_Set(Colors.colorBar.R, Colors.colorBar.G, 
                Colors.colorBar.B, Colors.colorCont[0].A);
      line_Draw(wSurface, x1+2, y1+2, x1+8, y1+8);
      line_Draw(wSurface, x1+1, y1+2, x1+7, y1+8);
      line_Draw(wSurface, x1+3, y1+2, x1+9, y1+8);
      line_Draw(wSurface, x1+8, y1+2, x1+2, y1+8);
      line_Draw(wSurface, x1+7, y1+2, x1+1, y1+8);
      line_Draw(wSurface, x1+9, y1+2, x1+3, y1+8);
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
   setSelection(!selected);
}

/**********************************************************
 *                       setSelection                     *
 **********************************************************/
void cxSel::setSelection(bool value)
{
   setChanged();
   selected = value;
}

/**********************************************************
 *                         doPress                        *
 **********************************************************/
bool cxSel::doPress(Uint8 mouseButton)
{
   if(mouseButton & SDL_BUTTON(1))
   {
      pressing = true;
      return(false);
   }

   if(pressing)
   {
      if(!(mouseButton & SDL_BUTTON(1)))
      {
         /* finished pressing, must invert selection and done */
         invertSelection();
         return(true);
      }
   }

   return(false);
}

