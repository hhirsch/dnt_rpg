/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2010 DNTeam <dnt@dnteam.org>
 
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

#include "featswindow.h"

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
featsWindow::featsWindow(guiInterface* interf)
{
   usedGui = interf;
   intWindow = NULL;
   curCharacter = NULL;
   curFeat = 0;
   selectFeatMode = false;
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
featsWindow::~featsWindow()
{
   close();
}

/***********************************************************************
 *                               open                                  *
 ***********************************************************************/
void featsWindow::open(character* pers, bool selectFeat)
{
   dntFont fnt;
   /* Set open position */
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;

   selectFeatMode = selectFeat;
   curCharacter = pers;
   curFeat = 0;


   /* Close, if already opened */
   if(isOpen())
   {
      centerX = intWindow->getX1()+128;
      centerY = intWindow->getY1()+124;
      close();
   }

   /* Create the window */
   intWindow = usedGui->insertWindow(centerX-128,centerY-124,
         centerX+124,centerY+118, gettext("Talents"));

   /* Icon and Description */
   featIcon = intWindow->getObjectsList()->insertPicture(10, 18, 0,0, NULL);
   featIcon->setSurfaceDeletion(false);
   featDescription = intWindow->getObjectsList()->insertRolBar(44,18,245,190,
         "");

   /* title and selectors */
   featTitle = intWindow->getObjectsList()->insertTextBox(59,191,230,209,
         1, gettext("No Talents."));
   featTitle->setFont(DNT_FONT_ARIAL,10,DNT_FONT_ALIGN_CENTER,
         DNT_FONT_STYLE_BOLD);
   buttonPrevious = intWindow->getObjectsList()->insertButton(44,191,58,209,
         fnt.createUnicode(0x25C4),0);
   buttonNext = intWindow->getObjectsList()->insertButton(231,191,245,209,
         fnt.createUnicode(0x25BA),0);

   /* Close (or Select) Button */
   buttonClose = intWindow->getObjectsList()->insertButton(88,215,158,234,
       gettext("Close"), 1);
   if(selectFeatMode)
   {
      buttonClose->setText(gettext("Select"));
   }

   /* Open the window */
   intWindow->setExternPointer(&intWindow);
   usedGui->openWindow(intWindow);

   /* Write feats info */
   writeAboutFeat();
}

/***********************************************************************
 *                            writeAboutFeat                           *
 ***********************************************************************/
void featsWindow::writeAboutFeat()
{
   feat* f = (feat*)curCharacter->actualFeats->featByNumber(curFeat);
   if(f != NULL)
   {
      featIcon->set(f->info->image);
      featTitle->setText(f->info->name);
      featDescription->setText(f->info->description);
      intWindow->draw(0,0);
   }
}

/***********************************************************************
 *                               close                                 *
 ***********************************************************************/
void featsWindow::close()
{
   if(intWindow)
   {
      usedGui->closeWindow(intWindow);
      intWindow = NULL;
   }
}

/***********************************************************************
 *                               treat                                 *
 ***********************************************************************/
int featsWindow::treat(guiObject* object, int eventInfo)
{
   if(!isOpen())
   {
      curFeat = -1;
      return(TALENT_WINDOW_CANCEL);
   }

   if(eventInfo == FARSO_EVENT_PRESSED_BUTTON)
   {
      /* Close/Select */
      if(object == buttonClose)
      {
         close();
         return(TALENT_WINDOW_CONFIRM);
      }
      /* Next */
      else if(object == buttonNext)
      {
         curFeat++;
         if(curFeat >= curCharacter->actualFeats->getTotal())
         {
            curFeat = 0;
         }
         writeAboutFeat();
      }
      /* Previous */
      else if(object == buttonPrevious)
      {
         curFeat--;
         if(curFeat < 0)
         {
            curFeat = curCharacter->actualFeats->getTotal()-1;
         }
         writeAboutFeat();
      }
   }

   return(TALENT_WINDOW_OTHER);
}

/***********************************************************************
 *                              isOpen                                 *
 ***********************************************************************/
bool featsWindow::isOpen()
{
   return(intWindow != NULL);
}

/***********************************************************************
 *                        getLastSelectedFeat                          *
 ***********************************************************************/
feat* featsWindow::getLastSelectedFeat()
{
   return((feat*)curCharacter->actualFeats->featByNumber(curFeat));
}

