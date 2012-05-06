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
#include <stdlib.h>

using namespace Farso;

/***************************************************************
 *                          Constructor                        *
 ***************************************************************/
Colors::Colors()
{
   int i;

   for(i=0; i < 3; i++)
   {
      colorCont[i] = defCont[i];
   }
   colorWindow = defWindow;
   colorText = defText;
   colorSelText = defSelText;
   colorBar = defBar;
   colorButton = defButton;
   colorMenu = defMenu;
   colorFile = defFile;
   colorDirectory = defDir;
   colorHigh = defHigh;
}

/***************************************************************
 *                            init                             *
 ***************************************************************/
void Colors::init()
{
   defCont[0].R = 134;
   defCont[0].G = 134;
   defCont[0].B = 134;
   defCont[0].A = 255;
   defCont[1].R = 70;
   defCont[1].G = 70;
   defCont[1].B = 70;
   defCont[1].A = 255;
   defCont[2].R = 169;
   defCont[2].G = 169;
   defCont[2].B = 169;
   defCont[2].A = 255;
   defWindow.R = 30;
   defWindow.G = 30;
   defWindow.B = 30;
   defWindow.A = 190;
   defText.R = 255;
   defText.G = 255;
   defText.B = 255;
   defText.A = 255;
   defSelText.R = 26;
   defSelText.G = 230;
   defSelText.B = 25;
   defSelText.A = 255;
   defBar.R = 102;
   defBar.G = 0;
   defBar.B = 0;
   defBar.A = 190;
   defButton.R = defWindow.R;
   defButton.G = defWindow.G;
   defButton.B = defWindow.B;
   defButton.A = defWindow.A;
   defMenu.R = defWindow.R;
   defMenu.G = defWindow.G;
   defMenu.B = defWindow.B;
   defMenu.A = defWindow.A;
   defFile.R = 240;
   defFile.B = 240;
   defFile.G = 240;
   defFile.A = 255;
   defDir.R = 255;
   defDir.G = 20;
   defDir.B = 20;
   defDir.A = 255;
   defHigh.R = 240;
   defHigh.G = 120;
   defHigh.B = 0;
   defHigh.A = 255;
}

/***************************************************************
 *                        setDefaultColor                      *
 ***************************************************************/
void Colors::setDefaultColor(int colorId, int R, int G, int B, int A)
{
   Color* c = NULL;
   switch(colorId)
   {
      case COLOUR_CONT_0:
         c = &defCont[0];
      break;
      case COLOUR_CONT_1:
         c = &defCont[1];
      break;
      case COLOUR_CONT_2:
         c = &defCont[2];
      break;
      case COLOUR_WINDOW:
         c = &defWindow;
      break;
      case COLOUR_BUTTON:
         c = &defButton;
      break;
      case COLOUR_MENU:
         c = &defMenu;
      break;
      case COLOUR_TEXT:
         c = &defText;
      break;
      case COLOUR_BAR:
         c = &defBar;
      break;
      case COLOUR_SEL_TEXT:
         c = &defSelText;
      break;
      case COLOUR_FILE:
         c = &defFile;
      break;
      case COLOUR_DIRECTORY:
         c = &defDir;
      break;
      case COLOUR_HIGH:
         c = &defHigh;
      break;
   }

   if(c)
   {
      c->R = R;
      c->G = G;
      c->B = B;
      c->A = A;
   }
}


Color Colors::defCont[3];
Color Colors::defWindow;
Color Colors::defButton;
Color Colors::defMenu;
Color Colors::defText;
Color Colors::defBar;
Color Colors::defSelText;
Color Colors::defFile;
Color Colors::defDir;
Color Colors::defHigh;

