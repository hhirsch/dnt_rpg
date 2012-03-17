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

#include "messages.h"

#include <stdio.h>
#include <stdlib.h>
using namespace std;

///////////////////////////////////////////////////////////////////////////
//
//                                warning
//
//////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                               Show                                  *
 ***********************************************************************/
void warning::show(string title, string message, guiInterface* gui)
{ 
   dntFont fnt;
   farsoOptions opt;

   /* Close the window if it is opened */
   close();

   /* Define the gui */
   lastGui = gui;

   /* Calculate Window Position And size */
   fnt.defineFont(opt.getDefaultFont(), 10);
   int sizeX = fnt.getStringWidth(message);
   int sX = SCREEN_X / 2;
   int sY = SCREEN_Y / 2;
   int med = sizeX / 2;

   warnWindow = gui->insertWindow(sX-med-10,sY-31,sX+med+10,sY+31,title);
   okButton = warnWindow->getObjectsList()->insertButton(med-28+10,37,
                                                         med+28+10,55,
                                                         "Ok",1);
   textBox* quadText = warnWindow->getObjectsList()->insertTextBox(5,17,
                                                      sizeX+10,33,0,
                                                      message.c_str());
   quadText->setFont(opt.getDefaultFont(), 10, DNT_FONT_ALIGN_CENTER);
   warnWindow->setModal();
   warnWindow->setExternPointer(&warnWindow);
   gui->openWindow(warnWindow);
}

/***********************************************************************
 *                            getOkButton                              *
 ***********************************************************************/
button* warning::getOkButton()
{
   return(okButton);
}

/***********************************************************************
 *                             isOpened                                *
 ***********************************************************************/
bool warning::isOpened()
{
   return(warnWindow != NULL);
}

/***********************************************************************
 *                               close                                 *
 ***********************************************************************/
void warning::close()
{
   if(warnWindow != NULL)
   {
      lastGui->closeWindow(warnWindow);
   }
}

/***********************************************************************
 *                           static members                            *
 ***********************************************************************/
window* warning::warnWindow = NULL;
guiInterface* warning::lastGui = NULL;
button* warning::okButton = NULL;

