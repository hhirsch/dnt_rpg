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

#include "aspectwindow.h"

#include "../etc/userinfo.h"

#include <iostream>
#include <fstream>
using namespace std;


/**************************************************************
 *                          Constructor                       *
 **************************************************************/
aspectWindow::aspectWindow(character* dude, guiInterface* inter)
{
   dirs dir;
   dntFont fnt;
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;


   curImage = 0;
   images = NULL;
   loadImages();

   usedCharacter = dude;
   intWindow = inter->insertWindow(centerX-129,centerY-55,centerX+129,
                                   centerY+55,
                                   gettext("Aspect"));

   /* Portrait Figure */
   figurePortrait = intWindow->getObjectsList()->insertPicture(10,23,0,0,NULL);
   figurePortrait->set(images[curImage].image);

   /* Previous Image Button */
   buttonPreviousImage = intWindow->getObjectsList()->insertButton(5,83,
                                                                  19,101,
                                                  fnt.createUnicode(0x25C4),0);
   /* Next Image Button */
   buttonNextImage = intWindow->getObjectsList()->insertButton(55,83,69,101,
                                                  fnt.createUnicode(0x25BA),0);
   /* Contorn to the previous and next buttons */
   intWindow->getObjectsList()->insertTextBox(20,83,54,101,1,"");

   /* Name Text */
   userInfo user;
   intWindow->getObjectsList()->insertTextBox(72,20,112,34,1,gettext("Name:"));
   textName = intWindow->getObjectsList()->insertTextBar(113,20,251,34,
                                                         user.getUserName(),0);

   /* Age Text */
   intWindow->getObjectsList()->insertTextBox(72,35,112,49,1,gettext("Age:"));
   textAge = intWindow->getObjectsList()->insertTextBar(113,35,143,49,"20",0);

   /* Sex Selectors */
   intWindow->getObjectsList()->insertTextBox(72,50,112,64,1,gettext("Sex:"));
   cxSelSexF = intWindow->getObjectsList()->insertCxSel(113, 52, false);
   intWindow->getObjectsList()->insertPicture(125,50,0,0,
                         dir.getRealFile("texturas/aspectw/sex_f.png").c_str());
   cxSelSexM = intWindow->getObjectsList()->insertCxSel(143, 52, true);
   intWindow->getObjectsList()->insertPicture(155,50,0,0,
                         dir.getRealFile("texturas/aspectw/sex_m.png").c_str());
   cxSelSexO = intWindow->getObjectsList()->insertCxSel(173, 52, false);
   intWindow->getObjectsList()->insertPicture(185,50,0,0,
                         dir.getRealFile("texturas/aspectw/sex_o.png").c_str());

   /* Contorns */
   intWindow->getObjectsList()->insertTextBox(70,18,254,74,2,"");
   intWindow->getObjectsList()->insertTextBox(70,75,254,101,2,"");

   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(181,79,251,97, 
                                                 gettext("Confirm"),1);
   
   /* Cancel Button */
   buttonCancel = intWindow->getObjectsList()->insertButton(72,79,140,97, 
                                                  gettext("Cancel"),1);

   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(false, true, false, false);
   inter->openWindow(intWindow);
}

/**************************************************************
 *                          Destructor                        *
 **************************************************************/
aspectWindow::~aspectWindow()
{
   /* Delete all images */
   if(images)
   {
      delete[] images;
   }
}

/**************************************************************
 *                          loadImages                        *
 **************************************************************/
void aspectWindow::loadImages()
{
   ifstream f;
   int i;
   string buffer;

   totalImages = 0;

   dirs dir;
   f.open(dir.getRealFile("characters/portraits/portraits.lst").c_str(),
          ios::in | ios::binary);
   
   if(!f)
   {
      cerr << "Can't open the list with portraits: portraits.lst!" << endl;
      return;
   }

   getline(f, buffer);
   sscanf(buffer.c_str(),"%d", &totalImages);
   images = new portraitImage[totalImages];

   for(i = 0; i < totalImages; i++)
   {
      getline(f, images[i].imageFile);
      images[i].image = IMG_Load(dir.getRealFile(images[i].imageFile).c_str());
   }

   f.close();
}

/**************************************************************
 *                             treat                          *
 **************************************************************/
int aspectWindow::treat(guiObject* object, int eventInfo, guiInterface* inter)
{
   if(eventInfo == FARSO_EVENT_PRESSED_BUTTON)
   {
      /* Confirm */
      if(object == (guiObject*) buttonConfirm)
      {
         /* Save Things to the character */
         usedCharacter->definePortrait(images[curImage].imageFile);
         /* Define Sex */
         if(cxSelSexF->isSelected())
         {
            usedCharacter->sexType = SEX_FEMALE;
         }
         else if(cxSelSexM->isSelected())
         {
            usedCharacter->sexType = SEX_MALE;
         }
         else
         {
            usedCharacter->sexType = SEX_OTHER;
         }
         /* Define Name */
         usedCharacter->name = textName->getText();
         /* Define Age */
         sscanf(textAge->getText().c_str(),"%d", &usedCharacter->age);

         /* Close Window */
         figurePortrait->set(NULL);
         inter->closeWindow(intWindow);
         intWindow = NULL;
         return(ASPECTW_CONFIRM);
      }
      /* Cancel */
      else if(object == (guiObject*) buttonCancel) 
      {
         figurePortrait->set(NULL);
         inter->closeWindow(intWindow);
         intWindow = NULL;
         return(ASPECTW_CANCEL);
      }
      /* Previous Image */
      else if(object == (guiObject*) buttonPreviousImage)
      {
         curImage--;
         if(curImage < 0)
         {
            curImage = totalImages-1;
         }
         figurePortrait->set(images[curImage].image);
      }
      /* Next Image */
      else if(object == (guiObject*) buttonNextImage)
      {
         curImage++;
         if(curImage >= totalImages)
         {
            curImage = 0;
         }
         figurePortrait->set(images[curImage].image);
      }
   }
   else if(eventInfo == FARSO_EVENT_MODIFIED_CX_SEL)
   {
      /* Female */
      if(object == (guiObject*) cxSelSexF)
      {
         if(cxSelSexF->isSelected())
         {
            cxSelSexM->setSelection(false);
         }
         else
         {
            cxSelSexM->setSelection(true);
         }
         cxSelSexO->setSelection(false);
      }
      /* Male */
      if(object == (guiObject*) cxSelSexM)
      {
         if(cxSelSexM->isSelected())
         {
            cxSelSexF->setSelection(false);
         }
         else
         {
            cxSelSexF->setSelection(true);
         }
         cxSelSexO->setSelection(false);
      }
      /* Other */
      if(object == (guiObject*) cxSelSexO)
      {
         if(cxSelSexO->isSelected())
         {
            cxSelSexF->setSelection(false);
         }
         else
         {
            cxSelSexF->setSelection(true);
         }
         cxSelSexM->setSelection(false);
      }
   }
   intWindow->draw(0,0);
   return(ASPECTW_OTHER);
}

