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

#include "color.h"

using namespace Farso;

/***************************************************************
 *                          Constructor                        *
 ***************************************************************/
Colors::Colors()
{
   colorCont[0].R = 134;
   colorCont[0].G = 134;
   colorCont[0].B = 134;
   colorCont[0].A = 255;
   colorCont[1].R = 70;
   colorCont[1].G = 70;
   colorCont[1].B = 70;
   colorCont[1].A = 255;
   colorCont[2].R = 169;
   colorCont[2].G = 169;
   colorCont[2].B = 169;
   colorCont[2].A = 255;
   colorWindow.R = 30;
   colorWindow.G = 30;
   colorWindow.B = 30;
   colorWindow.A = 190;
   colorText.R = 255;
   colorText.G = 255;
   colorText.B = 255;
   colorText.A = 255;
   colorSelText.R = 26;
   colorSelText.G = 230;
   colorSelText.B = 25;
   colorSelText.A = 255;
   colorBar.R = 102;
   colorBar.G = 0;
   colorBar.B = 0;
   colorBar.A = 190;
   colorButton.R = colorWindow.R;
   colorButton.G = colorWindow.G;
   colorButton.B = colorWindow.B;
   colorButton.A = colorWindow.A;
   colorMenu.R = colorWindow.R;
   colorMenu.G = colorWindow.G;
   colorMenu.B = colorWindow.B;
   colorMenu.A = colorWindow.A;
} 

