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

#include "moneywindow.h"
#include "barterwindow.h"
#include "modstate.h"
#include "../gui/farsoopts.h"
using namespace std;

/********************************************************************
 *                              setMap                              *
 ********************************************************************/
void moneyWindow::setMap(guiInterface* interf, Map* m)
{
   /* set map and interface */
   curMap = m;
   inter = interf;
}

/********************************************************************
 *                               open                               *
 ********************************************************************/
void moneyWindow::open(inventory* source, bool sell)
{
   if(isOpen())
   {
      /* Must cancel current one! */
      cancel();
   }

   if(sell)
   {
      type = MONEY_INVENTORY_TO_BUY;
   }
   else
   {
      type = MONEY_INVENTORY_TO_SELL;
   }
   inv = source;
   curQty = inv->getMoneyQuantity();
   maxQty = curQty;

   /* Open the window */
   open();
}

/********************************************************************
 *                               open                               *
 ********************************************************************/
void moneyWindow::open(bool sell, inventory* destiny)
{
   barterWindow bart;
   barter* bInv = bart.getBarterInventory();

   if(isOpen())
   {
      /* Cancel previous */
      cancel();
   }

   if(!bInv)
   {
      cerr << "moneyWindow::open() - ERROR: no barter!!" << endl;
      return;
   }

   if(sell)
   {
      type = MONEY_BUY_TO_INVENTORY;
      curQty = bInv->getSellMoneyQuantity();
   }
   else
   {
      type = MONEY_SELL_TO_INVENTORY;
      curQty = bInv->getBuyMoneyQuantity();
   }
   inv = destiny;
   maxQty = curQty;

   /* Open the window */
   open();
}

/********************************************************************
 *                               open                               *
 ********************************************************************/
void moneyWindow::open(inventory* source, float pX, float pZ)
{
   if(isOpen())
   {
      /* Must cancel */
      cancel();
   }

   /* set type and quantitys */
   type = MONEY_INVENTORY_TO_FLOOR;
   inv = source;
   destinyPosX = pX;
   destinyPosZ = pZ;
   curQty = inv->getMoneyQuantity();
   maxQty = curQty;

   /* open the window */
   open();
}

/********************************************************************
 *                               open                               *
 ********************************************************************/
void moneyWindow::open()
{
   farsoOptions fopts;
   dntFont fnt;
   /* Set open position */
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;

   char buf[16];
   sprintf(&buf[0], "%d", curQty);

   if(isOpen())
   {
     close();
   }

   /* Create Window */
   intWindow = inter->insertWindow(centerX-105,centerY-36,
                                   centerX+105,centerY+36,
                                   gettext("Quantity"));

   decButton = intWindow->getObjectsList()->insertButton(10, 20,
               24, 38, fnt.createUnicode(0x25C4),0);
   decButton->defineFont(fopts.getDefaultFont(), 9);
   qtyText = intWindow->getObjectsList()->insertTextBox(25,20,95,38,1,buf);
   incButton = intWindow->getObjectsList()->insertButton(96,20,110,38,
         fnt.createUnicode(0x25BA),0);
   incButton->defineFont(fopts.getDefaultFont(), 9);
   allButton = intWindow->getObjectsList()->insertButton(114,20,164,38,
         gettext("All"),true);

   cancelButton = intWindow->getObjectsList()->insertButton(40,45,100,63,
         gettext("Cancel"),true);
   okButton = intWindow->getObjectsList()->insertButton(110,45,170,63,
         gettext("Confirm"),true);

   /* Open Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(true,true,false,false);
   inter->openWindow(intWindow);

}

/********************************************************************
 *                               close                              *
 ********************************************************************/
void moneyWindow::close()
{
   if(intWindow)
   {
      inter->closeWindow(intWindow);
      intWindow = NULL;
   }
}

/********************************************************************
 *                               cancel                             *
 ********************************************************************/
void moneyWindow::cancel()
{
   inv = NULL;
   close();
}

/********************************************************************
 *                              accept                               *
 ********************************************************************/
void moneyWindow::accept()
{
   barterWindow bart;
   barter* bInv = bart.getBarterInventory();

   if( (type != MONEY_INVENTORY_TO_FLOOR) &&
       (bInv == NULL) )
   {
      /* No more at barter! */
      cancel();
      return;
   }

   switch(type)
   {
      case MONEY_SELL_TO_INVENTORY:
      {
         inv->addMoney(curQty);
         bInv->decSellMoney(curQty);
         bart.reDraw();
      }
      break;
      case MONEY_BUY_TO_INVENTORY:
      {
         inv->addMoney(curQty);
         bInv->decBuyMoney(curQty);
         bart.reDraw();
      }
      break;
      case MONEY_INVENTORY_TO_BUY:
      {
         bInv->addBuyMoney(curQty);
         inv->decMoney(curQty);
         bart.reDraw();
      }
      break;
      case MONEY_INVENTORY_TO_SELL:
      {
         bInv->addSellMoney(curQty);
         inv->decMoney(curQty);
         bart.reDraw();
      }
      break;
      case MONEY_INVENTORY_TO_FLOOR:
      {
         /* Change inventory */
         inv->decMoney(curQty);
         money* m = new money();
         float h = curMap->getHeight(destinyPosX, destinyPosZ);
         m->setQuantity(curQty);

         /* Put it on map */
         modState modifState;
         curMap->insertObject(destinyPosX,h,destinyPosZ,0.0f,0.0f,0.0f,m,false);
         modifState.mapObjectAddAction(MODSTATE_ACTION_OBJECT_ADD,
               m->getFileName(), curMap->getFileName(),
               destinyPosX, h, destinyPosZ);

         /* Save its state (to avoid, for example, ammo reload
          * after dropping-leaving-return to the map) */
         modifState.mapObjectAddAction(MODSTATE_ACTION_OBJECT_CHANGE_STATE,
               m->getFileName(), curMap->getFileName(),
               destinyPosX, h, destinyPosZ, m->getState());
      }
      break;
   }

   close();
}

/********************************************************************
 *                               treat                              *
 ********************************************************************/
bool moneyWindow::treat(guiObject* object, int eventInfo)
{
   char buf[16];
   barterWindow bart;
   barter* bInv = bart.getBarterInventory();

   if(isOpen())
   {

      /* Verify if active barter at barter transaction types */
      if( (type != MONEY_INVENTORY_TO_FLOOR) &&
            (bInv == NULL) )
      {
         /* No more at barter! */
         cancel();
         return(true);
      }

      /* Verify farso events */
      if(eventInfo == FARSO_EVENT_PRESSED_BUTTON)
      {
         if(object == okButton)
         {
            accept();
            return(true);
         }
         else if(object == cancelButton)
         {
            cancel();
            return(true);
         }
         else if(object == allButton)
         {
            curQty = maxQty;
            sprintf(&buf[0], "%d", curQty);
            qtyText->setText(buf);
            return(true);
         }
      }
      else if(eventInfo == FARSO_EVENT_ON_PRESS_BUTTON)
      {
         if(object == incButton)
         {
            curQty++;
            if(curQty > maxQty)
            {
               curQty = maxQty;
            }
            sprintf(&buf[0], "%d", curQty);
            qtyText->setText(buf);
            return(true);
         }
         else if(object == decButton)
         {
            curQty--;
            if(curQty < 0)
            {
               curQty = 0;
            }
            sprintf(&buf[0], "%d", curQty);
            qtyText->setText(buf);
            return(true);
         }
      }
   }

   return(false);
}

/********************************************************************
 *                              isOpen                              *
 ********************************************************************/
bool moneyWindow::isOpen()
{
   return(intWindow != NULL);
}

/********************************************************************
 *                          static members                          *
 ********************************************************************/
window* moneyWindow::intWindow=NULL;
Map* moneyWindow::curMap = NULL;
guiInterface* moneyWindow::inter=NULL;
textBox* moneyWindow::qtyText=NULL;
int moneyWindow::curQty=0;
int moneyWindow::maxQty=0;
button* moneyWindow::incButton=NULL;
button* moneyWindow::decButton=NULL;
button* moneyWindow::allButton=NULL;
button* moneyWindow::okButton=NULL;
button* moneyWindow::cancelButton=NULL;
int moneyWindow::type=0;
inventory* moneyWindow::inv=NULL;
float moneyWindow::destinyPosX=0.0f;
float moneyWindow::destinyPosZ=0.0f;

