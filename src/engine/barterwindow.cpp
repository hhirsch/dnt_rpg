/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#include "barterwindow.h"
#include "cursor.h"
#include "../etc/dirs.h"

/***************************************************************
 *                         Constructor                         *
 ***************************************************************/ 
barterWindow::barterWindow()
{
}

/***************************************************************
 *                             open                            *
 ***************************************************************/
void barterWindow::open(character* s, character* b,
                        guiInterface* inter, itemWindow* infoW, 
                        engine* usedEngine)
{
   dirs dir;
   /* Init Values */
   gui = inter;
   infoWindow = infoW;
   curEngine = usedEngine;

   int midX = SCREEN_X / 2;

   /* Set characters */
   seller = s;
   buyer = b;

   /* Create Barter */
   barterInventory = new barter(seller, buyer);
   
   /* Create Window */
   intWindow = gui->insertWindow(midX-132,0,midX+132,288,gettext("Barter"));

   /* Totals */
   buyerTotals = intWindow->getObjectsList()->insertTextBox(30,236,128,252,
                                                            0,"0,00");
   sellerTotals = intWindow->getObjectsList()->insertTextBox(162,236,251,252,
                                                             0,"0,00");
   buyerTotals->setColor(255,10,10);
   sellerTotals->setColor(255,10,10);

   /* Actions Buttons */
   offerButton = intWindow->getObjectsList()->insertButton(173,262,251,280,
                                             gettext("Offer"),0);
   imposeButton = intWindow->getObjectsList()->insertButton(91,262,168,280,
                                             gettext("Impose"),0);
   cancelButton = intWindow->getObjectsList()->insertButton(9,262,87,280,
                                             gettext("Cancel"),0);

   /* Money Images */
   intWindow->getObjectsList()->insertPicture(4,234,0,0,
                       dir.getRealFile("texturas/inventory/moeda.png").c_str());
   intWindow->getObjectsList()->insertPicture(136,234,0,0,
                       dir.getRealFile("texturas/inventory/moeda.png").c_str());


   /* Create the TabButton */
   barterTabButton = intWindow->getObjectsList()->insertTabButton(7,41,256,220,
                       dir.getRealFile("texturas/inventory/trade.png").c_str());

   /* Buyer Buttons */
   buyerInv = barterTabButton->insertButton(0,0,114,190);

   /* Seller Buttons */
   sellerInv = barterTabButton->insertButton(133,0,247,190);

   /* Open Window */
   intWindow->setExternPointer(&intWindow);
   gui->openWindow(intWindow);

   /* Verify if the buyer and seller windows are opened. If not open them */
   if(seller->inventories->getOpenedWindow() == NULL)
   {
      sellerWindow = new inventWindow(midX+133,0,gettext("Inventory"),
                                      seller, inter, infoWindow, curEngine);
   }
   else
   {
      sellerWindow = NULL;
   }

   if(buyer->inventories->getOpenedWindow() == NULL)
   {
      buyerWindow = new inventWindow(midX-400,0,gettext("Inventory"),
                                     buyer, inter, infoWindow, curEngine);
   }
   else
   {
      buyerWindow = NULL;
   }
}

/***************************************************************
 *                          Destructor                         *
 ***************************************************************/
barterWindow::~barterWindow()
{
}

/***************************************************************
 *                            close                            *
 ***************************************************************/
void barterWindow::close()
{
   /* Cancel the barter if the window is opened */
   if(isOpen())
   {
      cancel();
      gui->closeWindow(intWindow);
      intWindow = NULL;
   }

   /* Close the Inventory windows, if them are opened here. */
   if(buyerWindow)
   {
      delete(buyerWindow);
      buyerWindow = NULL;
   }
   if(sellerWindow)
   {
      delete(sellerWindow);
      sellerWindow = NULL;
   }

   /* Delete the barter inventory */
   if(barterInventory)
   {
      delete(barterInventory);
      barterInventory = NULL;
   }
}

/**************************************************************
 *                            isOpen                          *
 **************************************************************/
bool barterWindow::isOpen()
{
   return(intWindow != NULL);
}

/**************************************************************
 *                         addSellItem                        *
 **************************************************************/
bool barterWindow::addSellItem(object* obj)
{
   bool result = false;
   if(barterInventory)
   {
      result = barterInventory->addSellItem(obj);
      reDraw();
   }
   return(result);
}

/**************************************************************
 *                          addBuyItem                        *
 **************************************************************/
bool barterWindow::addBuyItem(object* obj)
{
   bool result = false;
   if(barterInventory)
   {
      result = barterInventory->addBuyItem(obj);
      reDraw();
   }
   return(result);
}

/**************************************************************
 *                       removeSellItem                       *
 **************************************************************/
void barterWindow::removeSellItem(int x, int y, int curSellSlot)
{
   if(barterInventory)
   {
      barterInventory->removeSellItem(x,y, curSellSlot);
      reDraw();
   }
}

/**************************************************************
 *                       removeBuyItem                        *
 **************************************************************/
void barterWindow::removeBuyItem(int x, int y, int curBuySlot)
{
   if(barterInventory)
   {
      barterInventory->removeBuyItem(x,y, curBuySlot);
      reDraw();
   }
}

/**************************************************************
 *                            reDraw                          *
 **************************************************************/
void barterWindow::reDraw()
{
   char value[32];
   if(isOpen())
   {
      /* Draw the barter inventory */
      barterInventory->draw(0,0, barterTabButton->get(), 
                            0, 0);

      /* Draw the Prices Text */

      /* The PC (buyer) is selling to the seller */
      sprintf(value, "%.2f", barterInventory->getTotalSellValue());
      buyerTotals->setText(value);

      /* The PC is buying from the seller */
      sprintf(value, "%.2f", barterInventory->getTotalBuyValue());
      sellerTotals->setText(value);

      intWindow->draw(0,0);
   }
}

/**************************************************************
 *                            cancel                          *
 **************************************************************/
void barterWindow::cancel()
{
   barterInventory->cancelBarter();
}

/**************************************************************
 *                             offer                          *
 **************************************************************/
bool barterWindow::offer()
{
   bool res = (barterInventory->doBarter());
   briefing brief;

   if(res)
   {
      /* The barter was accepted, so close all barter related windows */
      close();
      brief.addText(gettext("The barter was accepted."),67,92,215);
   }
   else
   {
      /* Tell the user it was rejected */
      brief.addText(gettext("The barter was rejected."),190,13,18);
   }

   return(res);
}

/**************************************************************
 *                            impose                          *
 **************************************************************/
bool barterWindow::impose()
{
   briefing brief;
   bool res = barterInventory->imposeBarter();

   //FIXME -> enter battle mode

   if(res)
   {
      /* The imposition was accepted, so close all barter related windows */
      close();
      brief.addText(gettext("The barter imposition was accepted."),67,92,215);
   }
   else
   {
      /* Tell the user it was rejected */
      brief.addText(gettext("The barter imposition was rejected."),190,13,18);
   }

   return(res);
}

/**************************************************************
 *                           openMenu                         *
 **************************************************************/
void barterWindow::openMenu(int mouseX, int mouseY)
{
   int x = mouseX - intWindow->getX1();
   int y = mouseY - intWindow->getY1();
   int xSize = 0;
   dirs dir;
   dntFont fnt;

   /* Create the Menu */
   objectMenu = (menu*) intWindow->getObjectsList()->addMenu();
   objectMenu->insertItem(gettext("Remove"), dir.getRealFile("icons/drop.png"), 
                          true);

   fnt.defineFont(DNT_FONT_ARIAL,12);

   xSize = objectMenu->getMaxCharac()*(fnt.getIncCP()+1)+6;

   /* Make Sure all Menu is in Window */
   if( (y + 20) >= (intWindow->getY2() - intWindow->getY1()) )
   {
      y = intWindow->getY2()-intWindow->getY1()-20;
   }

   if( (x + xSize) >= (intWindow->getX2() - intWindow->getX1()) )
   {
      x = intWindow->getX2() - intWindow->getX1()  - xSize;
   }

   objectMenu->setPosition(x,y);
}

/**************************************************************
 *                            treat                           *
 **************************************************************/
bool barterWindow::treat(guiObject* guiObj, int eventInfo, int mouseX, 
                         int mouseY, cursor* mouseCursor, Map* actualMap)
{
   bool res = false;
   int posX = -1;
   int posY = -1;
   cursor curMouse;

   if(!isOpen() && (barterInventory != NULL))
   {
      /* No more window, so free the structures! */
      close();
      return(true);
   }
   else if(!isOpen())
   {
      /* Nothing to do here */
      return(false);
   }

   /* Treat Inventory windows openned here */
   if(buyerWindow)
   {
      buyerWindow->treat(guiObj, eventInfo, mouseCursor, actualMap, 
                         buyer->scNode->getPosX(), buyer->scNode->getPosZ());
   }
   if(sellerWindow)
   {
      sellerWindow->treat(guiObj, eventInfo, mouseCursor, actualMap, 
                          seller->scNode->getPosX(), seller->scNode->getPosZ(),
                          true);
   }

   /* Gather GUI events */
   switch(eventInfo)
   {
      case FARSO_EVENT_PRESSED_BUTTON:
      {
         /* Cancel the Barter */
         if(guiObj == (guiObject*) cancelButton)
         {
            cancel();
            close();
            res = true;
         }
         /* Impose Barter */
         else if(guiObj == (guiObject*) imposeButton)
         {
            impose();
            res = true;
         }
         /* Offer Barter */
         else if(guiObj == (guiObject*) offerButton)
         {
            offer();
            res = true;
         }
      }
      break;

      case FARSO_EVENT_PRESSED_TAB_BUTTON:
      case FARSO_EVENT_ON_FOCUS_TAB_BUTTON:
      {
         /* Inventory Spaces Selected */
         if( (guiObj == (guiObject*) buyerInv) || 
             (guiObj == (guiObject*) sellerInv) )
         {
            /* Define Inventory */
            sellerObj = (guiObj == (guiObject*) sellerInv);
            int curInv = 0;

            /* Convert Mouse Coordinate to inventory coordinate */
            if(sellerObj)
            {
               posX = (int) floor((mouseX - (140 + intWindow->getX1())) / 19.0);
               posY = (int) floor((mouseY - (41 + intWindow->getY1())) / 19.0);
            }
            else
            {
               posX = (int) floor((mouseX - (7 + intWindow->getX1())) / 19.0);
               posY = (int) floor((mouseY - (41 + intWindow->getY1())) / 19.0);
            }

            /* Open Menu For Object if one is avaible */
            activeObject = barterInventory->getFromPosition(posX, posY, 
                                                            curInv, sellerObj);
            if(activeObject)
            {
               activeObject = barterInventory->getFromPosition(posX,posY,curInv,
                                                               sellerObj);

               if(eventInfo == FARSO_EVENT_PRESSED_TAB_BUTTON)
               {
                  objX = posX;
                  objY = posY;

                  openMenu(mouseX, mouseY);
               }
               else
               {
                  curMouse.setTextOver(activeObject->name);
               }
            }
            res = true;
         }
      }
      break;

      case FARSO_EVENT_SELECTED_MENU:
      {
         if( (objectMenu) && (activeObject))
         {
            switch(objectMenu->getActualItem())
            {
               case 1: /* Remove */
               {
                  if(sellerObj)
                  {
                     removeBuyItem(objX, objY, 0);
                  }
                  else
                  {
                     removeSellItem(objX, objY, 0);
                  }
                  reDraw();
               }
               break;
            }
            intWindow->getObjectsList()->removeMenu();
            objectMenu = NULL;
            res = true;
         }
      }
      break;
   }

   return(res);
}



/**********************************************************************
 *                            Static Members                          *
 **********************************************************************/
character* barterWindow::buyer = NULL;
character* barterWindow::seller = NULL;

object* barterWindow::activeObject = NULL;
int barterWindow::objX = 0;
int barterWindow::objY = 0;
bool barterWindow::sellerObj = false;

barter* barterWindow::barterInventory = NULL;
guiInterface* barterWindow::gui = NULL;
textBox* barterWindow::sellerTotals = NULL;
textBox* barterWindow::buyerTotals = NULL;
window* barterWindow::intWindow = NULL;
button* barterWindow::imposeButton = NULL;
button* barterWindow::offerButton = NULL;
button* barterWindow::cancelButton = NULL;

tabButton* barterWindow::barterTabButton = NULL;
oneTabButton* barterWindow::sellerInv = NULL;
inventWindow* barterWindow::sellerWindow = NULL;

oneTabButton* barterWindow::buyerInv = NULL;
inventWindow* barterWindow::buyerWindow = NULL;

menu* barterWindow::objectMenu = NULL;

itemWindow* barterWindow::infoWindow = NULL;
engine* barterWindow::curEngine = NULL;

