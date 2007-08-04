#include "barterwindow.h"

/***************************************************************
 *                         Constructor                         *
 ***************************************************************/ 
barterWindow::barterWindow(inventory* inventSeller, inventory* inventBuyer,
                           interface* inter)
{
   lang language;
   /* Init Values */
   curSellSlot = 0;
   curBuySlot = 0;
   gui = inter;

   /* Create Barter */
   barterInventory = new barter(inventSeller, inventBuyer);
   
   /* Create Window */
   intWindow = gui->insertWindow(268,0,531,274,language.BARTERW_TITLE.c_str());

   /* Actions Buttons */
   offerButton = intWindow->getObjectsList()->insertButton(173,242,251,262,
                                             language.BARTERW_OFFER.c_str(),0);
   imposeButton = intWindow->getObjectsList()->insertButton(91,242,168,262,
                                             language.BARTERW_IMPOSE.c_str(),0);
   cancelButton = intWindow->getObjectsList()->insertButton(9,242,87,262,
                                             language.BARTERW_CANCEL.c_str(),0);

   /* Create the low figure */
   intWindow->getObjectsList()->insertPicture(4,232,256,39,
                              "../data/texturas/inventory/trade_inf.png");

   /* Create the TabButton */
   barterTabButton = intWindow->getObjectsList()->insertTabButton(4,15,256,217,
                                        "../data/texturas/inventory/trade.png");

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
      sellerWindow = new inventWindow(536,0,language.INVENTW_TITLE.c_str(),
                                      inventSeller, inter);
   }
   else
   {
      sellerWindow = false;
   }

   if(!inventBuyer->openedWindow)
   {
      buyerWindow = new inventWindow(0,0,language.INVENTW_TITLE.c_str(),
                                     inventBuyer, inter);
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

