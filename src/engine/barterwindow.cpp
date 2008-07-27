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
 *                            reDraw                          *
 **************************************************************/
void barterWindow::reDraw()
{
   if(isOpen())
   {
      barterInventory->draw(0,0, barterTabButton->get(), 
                            curSellSlot, curBuySlot);
      intWindow->draw(0,0);
   }
}

/**************************************************************
 *                            cancel                          *
 **************************************************************/
void barterWindow::cancel()
{
   barterInventory->cancelBarter();
   gui->closeWindow(intWindow);
   intWindow = NULL;
}

/**************************************************************
 *                             offer                          *
 **************************************************************/
bool barterWindow::offer()
{
   return(barterInventory->doBarter());
}

/**************************************************************
 *                            impose                          *
 **************************************************************/
bool barterWindow::impose()
{
   return(barterInventory->imposeBarter());
}

/**************************************************************
 *                            treat                           *
 **************************************************************/
bool barterWindow::treat(guiObject* guiObj, int eventInfo, cursor* mouseCursor,
                         Map* actualMap)
{
   if(!isOpen() && (barterInventory != NULL))
   {
      /* No more window, so free the structures! */
      close();
      return(true);
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
                          seller->xPosition, seller->zPosition);
   }

   //TODO
   return(false);
}



/**********************************************************************
 *                            Static Members                          *
 **********************************************************************/
character* barterWindow::buyer = NULL;
character* barterWindow::seller = NULL;

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

itemWindow* barterWindow::infoWindow = NULL;

