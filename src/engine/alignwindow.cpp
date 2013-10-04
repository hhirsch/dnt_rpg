/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "alignwindow.h"
#include "../etc/dirs.h"
#include "util.h"

/************************************************************************
 *                           Constructor                                *
 ************************************************************************/
alignWindow::alignWindow(aligns* alg, Farso::GuiInterface* inter, 
      align** actual)
{
   dirs dir;
   Farso::Font fnt;
   int centerY = Farso::SCREEN_Y / 2;
   int centerX = Farso::SCREEN_X / 2;

   externalAligns = alg;
   choosedAlign = actual;
   if(*choosedAlign != NULL)
   {
      actualAlign = *choosedAlign;
   }
   else
   {
      actualAlign = externalAligns->getAlignByInteger(0);
   }
   
   /* create intWindow */
   intWindow = inter->insertWindow(centerX-144,centerY-129,
                                   centerX+144,centerY+129,
                                   gettext("Tendency & Alignment"));

   /* Align Image */
   alignImage = intWindow->getObjectsList()->insertPicture(113,185,0,0,NULL);   
   alignImage->setSurfaceDeletion(false);
   alignImage->set(actualAlign->image);

   /* Align Description */
   textDesc = intWindow->getObjectsList()->insertRolBar(5,38,282,180,"");
   textDesc->addText(actualAlign->description.c_str(),
         dir.getRealFile(DNT_FONT_ARIAL), 10, Farso::Font::ALIGN_LEFT,
         Farso::Font::STYLE_NORMAL, 86, 161, 132); 
   textDesc->setFirstLine(0);
      
   /* Name and Selectors */
   buttonPrevious = intWindow->getObjectsList()->insertButton(5,19,19,37,
                                                 fnt.createUnicode(0x25C4),0);
   buttonNext = intWindow->getObjectsList()->insertButton(268,19,282,37,
                                                 fnt.createUnicode(0x25BA),0);
   textName = intWindow->getObjectsList()->insertTextBox(20,19,267,37,1, 
                                                  actualAlign->name.c_str());
   textName->setFont(dir.getRealFile(DNT_FONT_ARIAL), 10, 
         Farso::Font::ALIGN_CENTER, Farso::Font::STYLE_BOLD);

   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(203,229,273,248, 
                                              gettext("Confirm"),1);
   
   /* Cancel Button */
   buttonCancel = intWindow->getObjectsList()->insertButton(16,229,86,248,
                                               gettext("Cancel"),1);

   /* Contorns */
   intWindow->getObjectsList()->insertTextBox(5,181,97,224,2,"");
   intWindow->getObjectsList()->insertTextBox(5,225,97,252,2,"");
   intWindow->getObjectsList()->insertTextBox(98,181,191,252,2,"");
   intWindow->getObjectsList()->insertTextBox(192,181,282,224,2,"");
   intWindow->getObjectsList()->insertTextBox(192,225,282,252,2,"");

   /* Open Skill Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(false, true, false, false);
   inter->openWindow(intWindow);
}

/************************************************************************
 *                              treat                                   *
 ************************************************************************/
int alignWindow::treat(Farso::GuiObject* object, int eventInfo, 
      Farso::GuiInterface* inter)
{
   if(eventInfo == Farso::EVENT_PRESSED_BUTTON)
   {
      if( (object == (Farso::GuiObject*) buttonNext) || 
          (object == (Farso::GuiObject*) buttonPrevious))
      {
         if(object == (Farso::GuiObject*) buttonNext)
         {
            actualAlign = (align*)actualAlign->getNext();
         }
         else
         {
            actualAlign = (align*)actualAlign->getPrevious();
         }
         dirs dir;
         textName->setText(actualAlign->name);
         textDesc->setText("");
         textDesc->addText(actualAlign->description.c_str(),
               dir.getRealFile(DNT_FONT_ARIAL), 10, Farso::Font::ALIGN_LEFT,
               Farso::Font::STYLE_NORMAL, 86, 161, 132);
         textDesc->setFirstLine(0);
         alignImage->set(actualAlign->image);
         intWindow->draw(0,0);
      }
      else if(object == (Farso::GuiObject*) buttonConfirm)
      {
         *choosedAlign = actualAlign;
         inter->closeWindow(intWindow);
         return(ALIGNW_CONFIRM);
      }
      else if(object == (Farso::GuiObject*) buttonCancel) 
      {
         inter->closeWindow(intWindow);
         *choosedAlign = NULL;
         intWindow = NULL;
         return(ALIGNW_CANCEL);
      }
   }
   return(ALIGNW_OTHER);
}

