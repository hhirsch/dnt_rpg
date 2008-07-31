#include "barterwindow.h"
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
                        guiInterface* inter, itemWindow* infoW)
{
   dirs dir;
   /* Init Values */
   curSellSlot = 0;
   curBuySlot = 0;
   gui = inter;
   infoWindow = infoW;

   /* Set characters */
   seller = s;
   buyer = b;

   /* Create Barter */
   barterInventory = new barter(seller, buyer);
   
   /* Create Window */
   intWindow = gui->insertWindow(268,0,531,288,gettext("Barter"));

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
   barterTabButton = intWindow->getObjectsList()->insertTabButton(3,15,256,220,
                       dir.getRealFile("texturas/inventory/trade.png").c_str());

   /* Buyer Buttons */
   buyer1 = barterTabButton->insertButton(4,13,42,26);
   buyer2 = barterTabButton->insertButton(42,13,80,26);
   buyer3 = barterTabButton->insertButton(80,13,118,26);
   buyerInv = barterTabButton->insertButton(4,26,118,216);

   /* Seller Buttons */
   seller1 = barterTabButton->insertButton(137,13,175,26);
   seller2 = barterTabButton->insertButton(175,13,213,26);
   seller3 = barterTabButton->insertButton(213,13,251,26);
   sellerInv = barterTabButton->insertButton(137,26,251,216);

   /* Open Window */
   intWindow->setExternPointer(&intWindow);
   gui->openWindow(intWindow);

   /* Verify if the buyer and seller windows are opened. If not open them */
   if(!seller->inventories->openedWindow)
   {
      sellerWindow = new inventWindow(536,0,gettext("Inventory"),
                                      seller->inventories, inter, 
                                      infoWindow);
   }
   else
   {
      sellerWindow = NULL;
   }

   if(!buyer->inventories->openedWindow)
   {
      buyerWindow = new inventWindow(0,0,gettext("Inventory"),
                                     buyer->inventories, inter, infoWindow);
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
 *                       removeBuyItem                       *
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
                            curSellSlot, curBuySlot);

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
   int x = mouseX;
   int y = mouseY;
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
                         buyer->xPosition, buyer->zPosition);
   }
   if(sellerWindow)
   {
      sellerWindow->treat(guiObj, eventInfo, mouseCursor, actualMap, 
                          seller->xPosition, seller->zPosition, true);
   }

   /* Gather GUI events */
   switch(eventInfo)
   {
      case PRESSED_BUTTON:
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

      case PRESSED_TAB_BUTTON:
      {
         /* Inventory Spaces Selected */
         if( (guiObj == (guiObject*) buyerInv) || 
             (guiObj == (guiObject*) sellerInv) )
         {
            /* Define Inventory */
            sellerObj = (guiObj == (guiObject*) sellerInv);
            int curInv = seller ? curBuySlot : curSellSlot;

            /* Convert Mouse Coordinate to inventory coordinate */
            if(sellerObj)
            {
               posX = (int) floor((mouseX - (140 + intWindow->getX1())) / 19.0);
               posY = (int) floor((mouseY - (28 + intWindow->getY1())) / 19.0);
            }
            else
            {
               posX = (int) floor((mouseX - (7 + intWindow->getX1())) / 19.0);
               posY = (int) floor((mouseY - (28 + intWindow->getY1())) / 19.0);
            }

            /* Open Menu For Object if one is avaible */
            activeObject = barterInventory->getFromPosition(posX, posY, 
                                                            curInv, sellerObj);
            if(activeObject)
            {
               activeObject = barterInventory->getFromPosition(posX,posY,curInv,
                                                               sellerObj);
               objX = posX;
               objY = posY;

               openMenu(mouseX, mouseY);
            }
            res = true;
         }
      }
      break;

      case SELECTED_MENU:
      {
         if( (objectMenu) && (activeObject))
         {
            switch(objectMenu->getActualItem())
            {
               case 1: /* Remove */
               {
                  if(sellerObj)
                  {
                     removeBuyItem(objX, objY, curBuySlot);
                  }
                  else
                  {
                     removeSellItem(objX, objY, curSellSlot);
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

int barterWindow::curSellSlot = 0;
int barterWindow::curBuySlot = 0;

barter* barterWindow::barterInventory = NULL;
guiInterface* barterWindow::gui = NULL;
textBox* barterWindow::sellerTotals = NULL;
textBox* barterWindow::buyerTotals = NULL;
window* barterWindow::intWindow = NULL;
button* barterWindow::imposeButton = NULL;
button* barterWindow::offerButton = NULL;
button* barterWindow::cancelButton = NULL;

tabButton* barterWindow::barterTabButton = NULL;
oneTabButton* barterWindow::seller1 = NULL;
oneTabButton* barterWindow::seller2 = NULL;
oneTabButton* barterWindow::seller3 = NULL;
oneTabButton* barterWindow::sellerInv = NULL;
inventWindow* barterWindow::sellerWindow = NULL;

oneTabButton* barterWindow::buyer1 = NULL;
oneTabButton* barterWindow::buyer2 = NULL;
oneTabButton* barterWindow::buyer3 = NULL;
oneTabButton* barterWindow::buyerInv = NULL;
inventWindow* barterWindow::buyerWindow = NULL;

menu* barterWindow::objectMenu = NULL;

itemWindow* barterWindow::infoWindow = NULL;

