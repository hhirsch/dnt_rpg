#include "barterwindow.h"
#include "../etc/dirs.h"

/***************************************************************
 *                         Constructor                         *
 ***************************************************************/ 
barterWindow::barterWindow(inventory* inventSeller, inventory* inventBuyer,
                           guiInterface* inter, itemWindow* infoW)
{
   dirs dir;
   /* Init Values */
   curSellSlot = 0;
   curBuySlot = 0;
   gui = inter;
   infoWindow = infoW;

   /* Create Barter */
   barterInventory = new barter(inventSeller, inventBuyer);
   
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
   if(!inventSeller->openedWindow)
   {
      sellerWindow = new inventWindow(536,0,gettext("Inventory"),
                                      inventSeller, inter, infoWindow);
   }
   else
   {
      sellerWindow = false;
   }

   if(!inventBuyer->openedWindow)
   {
      buyerWindow = new inventWindow(0,0,gettext("Inventory"),
                                     inventBuyer, inter, infoWindow);
   }
   else
   {
      buyerWindow = false;
   }
}

/***************************************************************
 *                          Destructor                         *
 ***************************************************************/
barterWindow::~barterWindow()
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
   }
   if(sellerWindow)
   {
      delete(sellerWindow);
   }

   /* Delete the barter inventory */
   delete(barterInventory);
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
bool barterWindow::treat(guiObject* guiObj, int eventInfo)
{
   //TODO
   return(false);
}

