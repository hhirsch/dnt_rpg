/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "barter.h"

#define SELL_SIZE_X   6  /**< Size of Sell Slots */
#define SELL_SIZE_Y  10  /**< Size of Sell Slots */
#define BUY_SIZE_X    6  /**< Size of Buy Slots */
#define BUY_SIZE_Y   10  /**< Size of Buy Slots */

/*******************************************************************
 *                           Constructor                           *
 *******************************************************************/
barter::barter(inventory* seller, inventory* buyer)
{
   sellerInventory = seller;
   buyerInventory = buyer;
   buySlot = new itemSlot(BUY_SIZE_X, BUY_SIZE_Y);
   sellSlot = new itemSlot(SELL_SIZE_X, SELL_SIZE_Y);
}

/*******************************************************************
 *                            Destructor                           *
 *******************************************************************/
barter::~barter()
{
   /* Cancel the barter of all itens actually on slots */
   cancelBarter();
   if(buySlot)
   {
      delete(buySlot);
   }
   if(sellSlot)
   {
      delete(sellSlot);
   }
   sellerInventory = NULL;
   buyerInventory = NULL;
}

/*******************************************************************
 *                           cancelBarter                          *
 *******************************************************************/
void barter::cancelBarter()
{
   object* obj;
   int oX = 0, 
       oY = 0;
   /* First, reput all buy items to the seller */
   obj=buySlot->getFirstItem(oX, oY);
   while(obj != NULL)
   {
      sellerInventory->addObject(obj);
      buySlot->removeObject(oX, oY);
      obj=buySlot->getFirstItem(oX, oY);
   }

   /* Next, reput all sell items to the buyer */
   obj = sellSlot->getFirstItem(oX, oY);
   while(obj != NULL)
   {
      buyerInventory->addObject(obj);
      buySlot->removeObject(oX, oY);
      obj = sellSlot->getFirstItem(oX, oY);
   }
}

/*******************************************************************
 *                             doBarter                            *
 *******************************************************************/
bool barter::doBarter()
{
   object* obj;
   int oX = 0, 
       oY = 0;

   //TODO verify if the transaction is good for the seller
   
   /* First, put all buy items to the buyer */
   obj=buySlot->getFirstItem(oX, oY);
   while(obj != NULL)
   {
      buyerInventory->addObject(obj);
      buySlot->removeObject(oX, oY);
      obj=buySlot->getFirstItem(oX, oY);
   }

   /* Next, put all sell items to the seller */
   obj = sellSlot->getFirstItem(oX, oY);
   while(obj != NULL)
   {
      sellerInventory->addObject(obj);
      buySlot->removeObject(oX, oY);
      obj = sellSlot->getFirstItem(oX, oY);
   }

   return(true);
}

/*******************************************************************
 *                           imposeBarter                          *
 *******************************************************************/
bool barter::imposeBarter()
{
   //TODO, verify if the seller will accept the imposition
   return(false);
}

