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

#include "itemwindow.h"
#include "../classes/weapon.h"
using namespace std;

/********************************************************************
 *                           Constructor                            *
 ********************************************************************/
itemWindow::itemWindow(guiInterface* interf)
{
   inter = interf;
   intWindow = NULL;
}

/********************************************************************
 *                            Destructor                            *
 ********************************************************************/
itemWindow::~itemWindow()
{
   close();
}

/********************************************************************
 *                              open                                *
 ********************************************************************/
void itemWindow::open(object* item)
{
   /* Set open position */
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;

   int curY = 0;
   int prvY = 0;

   char buf[256];
   string aux;

   /* Close the window, if it is openned */
   if(isOpen())
   {
      centerX = intWindow->getX1()+128;
      centerY = intWindow->getY1()+144;
      close();
   }

   /* Create Window */
   intWindow = inter->insertWindow(centerX-128,centerY-144,
                                   centerX+128,centerY+144,
                                   item->getName());

   /***********************
    * Common Object Things 
    ***********************/
   
   curY = 15;
   prvY = curY;

   /* Name */
   sprintf(buf,gettext("Name: %s"), item->getName().c_str());
   intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,buf);
   curY += 18;

   /* Description */
   if(!item->getDescription().empty())
   {
      intWindow->getObjectsList()->insertRolBar(70, curY, 250, curY+51, 
         item->getDescription());
      curY += 52;
   }

   /* Cost */
   sprintf(buf,gettext("Cost: %.2f"), item->cost);
   intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,buf);
   curY += 18;

   /* Weight */
   sprintf(buf,gettext("Weight: %.2f Kg"), item->weight);
   intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,buf);
   curY += 18;

   /* General Contorn */
   intWindow->getObjectsList()->insertTextBox(70,prvY,250,curY-1,1,"");

   /***********************
    * Heal Items Things
    ***********************/
   if(item->getType() == OBJECT_TYPE_HEAL)
   {
      diceThing dInfo = item->getDiceInfo();

      /* Base Dice */
      sprintf(buf,"%s %dd%d+%d", gettext("Heal Factor: "), 
              dInfo.baseDice.getNumberOfDices(), 
              dInfo.baseDice.getType(),
              dInfo.baseDice.getSumNumber());
      intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,1,buf);
      curY += 18;

      /* Aditional Dice */
      if(dInfo.aditionalDice.getNumberOfDices() > 0)
      {
         sprintf(buf,"%s %dd%d+%d", gettext("Aditional Factor: "), 
                 dInfo.aditionalDice.getNumberOfDices(), 
                 dInfo.aditionalDice.getType(),
                 dInfo.aditionalDice.getSumNumber());
         intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,1,buf);
         curY += 18;
      }
   }

   /***********************
    * Weapons Things 
    ***********************/
   else if(item->getType() == OBJECT_TYPE_WEAPON)
   {
      /* Get All Info from the weapon */
      wInfo category, range, size, weight,
            damageA, damageB, *ammo;
      weapon* wp = (weapon*)item;
      wp->getType(category, range, size, weight, damageA, damageB);
      diceThing dam = wp->getDice();

      /* Category */
      sprintf(buf,"%s %s", gettext("Category: "), category.title.c_str());
      intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,1,buf);
      curY += 18;

      /* Damage Dice */
      prvY = curY;
      sprintf(buf,"%s %dd%d+%d (x%d)", gettext("Damage Dice: "),  
              dam.baseDice.getNumberOfDices(), dam.baseDice.getType(), 
              dam.baseDice.getSumNumber(), 
              dam.baseDice.getCriticalMultiplier());
      intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,buf);
      curY += 18;

      /* Primary Damage */
      sprintf(buf,"%s %s", gettext("Primary Damage: "), damageA.title.c_str());
      intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,buf);
      curY += 18;

      /* Secondary Damage */
      if(damageB.index != -1)
      {
         sprintf(buf,"%s %s", gettext("Secondary Damage: "), 
                 damageB.title.c_str());
         intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,buf);
         curY += 18;
      }

      /* Damage Contorn */
      intWindow->getObjectsList()->insertTextBox(70,prvY,250,curY-1,1,"");


      prvY = curY;
      /* Range */
      sprintf(buf,"%s %s", gettext("Range: "), range.title.c_str());
      intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,buf);
      curY += 18;

      /* Size */
      sprintf(buf,"%s %s", gettext("Size: "), size.title.c_str());
      intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,buf);
      curY += 18;

      /* Contorn to range and size */
      intWindow->getObjectsList()->insertTextBox(70,prvY,250,curY-1,1,"");

      prvY = curY;
      
      /* Munition Type */
      ammo = wp->getMunitionType();
      if(ammo->index != 0)
      {
         aux = gettext("Ammo Type:");
         aux += " " + ammo->title;
         intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,aux);
         curY += 18;

         /* Quantity */
         sprintf(buf,"%s %d/%d", gettext("Quantity:"), wp->getCurrentMunition(),
                                 wp->getMunitionCapacity());
         intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,buf);
         curY += 18;
         
         /* Contorn */
         intWindow->getObjectsList()->insertTextBox(70,prvY,250,curY-1,1,"");
      }
   }

   /***********************
    * Ammmo Things 
    ***********************/
   else if(item->getType() == OBJECT_TYPE_AMMO)
   {
      prvY = curY;

      weaponTypes wTypes;
      /* Get Ammo Type */
      wInfo* weaponInfo = wTypes.getMunition(item->getRelatedInfo());
      /* Display Type */
      aux = gettext("Ammo Type:");
      aux += " " + weaponInfo->title;
      intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,aux);
      curY += 18;
      
      /* Quantity */
      sprintf(buf, "%s %d", gettext("Quantity:"), item->getState());
      intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,buf);
      curY += 18;

      /* Contorn */
      intWindow->getObjectsList()->insertTextBox(70,prvY,250,curY-1,1,"");
   }

   /* Item Image */
   int remainX = (64 - item->get2dModel()->w) / 2;
   int remainY = ((curY-15) - item->get2dModel()->h) / 2;
   fig = intWindow->getObjectsList()->insertPicture(8+remainX,16+remainY,
                                                    0,0,NULL);
   fig->setSurfaceDeletion(false);
   fig->set(item->get2dModel());
   intWindow->getObjectsList()->insertTextBox(5,15,69,curY-1,1,"");

   /* Ok Button */
   okButton = intWindow->getObjectsList()->insertButton(88,260,158,279,
                                                        gettext("Ok"),1);

   /* Remaining Contorn */
   intWindow->getObjectsList()->insertTextBox(5,curY,250,281,1,"");
   

   /* Open Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(true,true,false,false);
   inter->openWindow(intWindow);
}

/********************************************************************
 *                             close                                *
 ********************************************************************/
void itemWindow::close()
{
   if(intWindow)
   {
      inter->closeWindow(intWindow);
      intWindow = NULL;
   }
}

/********************************************************************
 *                             isOpen                               *
 ********************************************************************/
bool itemWindow::isOpen()
{
   return(intWindow != NULL);
}

/********************************************************************
 *                              treat                               *
 ********************************************************************/
int itemWindow::treat(guiObject* object, int eventInfo)
{
   if(eventInfo == FARSO_EVENT_PRESSED_BUTTON)
   {
      if(object == (guiObject*) okButton)
      {
         close();
         return(1);
      }
   }
   return(0);
}

