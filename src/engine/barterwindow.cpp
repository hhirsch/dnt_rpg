#include "barterwindow.h"

/***************************************************************
 *                         Constructor                         *
 ***************************************************************/ 
barterWindow::barterWindow(inventory* inventSeller, inventory* inventBuyer,
                           interface* inter)
{
   /* Init Values */
   curSellSlot = 0;
   curBuySlot = 0;
   gui = inter;

   /* Create Barter */
   barterInventory = new barter(inventSeller, inventBuyer);
   
   /* Create Window */
   intWindow = gui->insertWindow(0,0,263,274,"Barter");

   /* Create the TabButton */
   barterTabButton = intWindow->getObjectsList()->insertTabButton(4,15,256,256,
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

   /* Actions Buttons */
   //offerButton = 
   //imposeButton = 
   //cancelButton = 

   /* Open Window */
   intWindow->setExternPointer(&intWindow);
   gui->openWindow(intWindow);
}

/***************************************************************
 *                          Destructor                         *
 ***************************************************************/
barterWindow::~barterWindow()
{
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


