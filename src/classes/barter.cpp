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
   int i;

   /* Define the Inventories */
   sellerInventory = seller;
   buyerInventory = buyer;

   /* Create Buy Slots */
   for(i=0; i < BARTER_BUY_SLOTS; i++)
   {
      buySlot[i] = new itemSlot(BUY_SIZE_X, BUY_SIZE_Y);
   }

   /* Create Sell Slots */
   for(i=0; i < BARTER_SELL_SLOTS; i++)
   {
      sellSlot[i] = new itemSlot(SELL_SIZE_X, SELL_SIZE_Y);
   }

   /* Load Images */
   barterImage = IMG_Load("../data/texturas/inventory/trade.png");
   if(!barterImage)
   {
      printf("Can't Load Barter Image!\n");
   }
}

/*******************************************************************
 *                            Destructor                           *
 *******************************************************************/
barter::~barter()
{
   int i;
   /* Cancel the barter of all itens actually on slots */
   cancelBarter();

   /* Remove Buy Slots */
   for(i=0; i < BARTER_BUY_SLOTS; i++)
   {
      if(buySlot[i])
      {
         delete(buySlot[i]);
      }
   }

   /* Remove Sell Slots */
   for(i=0; i < BARTER_SELL_SLOTS; i++)
   {
      if(sellSlot[i])
      {
         delete(sellSlot[i]);
      }
   }
   sellerInventory = NULL;
   buyerInventory = NULL;
}

/*******************************************************************
 *                           cancelBarter                          *
 *******************************************************************/
void barter::cancelBarter()
{
   int i;
   object* obj;
   int oX = 0, 
       oY = 0;
   /* First, reput all buy items to the seller */
   for(i=0; i < BARTER_BUY_SLOTS; i++)
   {
      obj=buySlot[i]->getFirstItem(oX, oY);
      while(obj != NULL)
      {
         sellerInventory->addObject(obj);
         buySlot[i]->removeObject(oX, oY);
         obj=buySlot[i]->getFirstItem(oX, oY);
      }
   }

   /* Next, reput all sell items to the buyer */
   for(i=0; i < BARTER_SELL_SLOTS; i++)
   {
      obj = sellSlot[i]->getFirstItem(oX, oY);
      while(obj != NULL)
      {
         buyerInventory->addObject(obj);
         sellSlot[i]->removeObject(oX, oY);
         obj = sellSlot[i]->getFirstItem(oX, oY);
      }
   }
}

/*******************************************************************
 *                             doBarter                            *
 *******************************************************************/
bool barter::doBarter()
{
   int i;
   object* obj;
   int oX = 0, 
       oY = 0;

   //TODO verify if the transaction is good for the seller
   
   /* First, put all buy items to the buyer */
   for(i=0; i<BARTER_BUY_SLOTS; i++)
   {
      obj=buySlot[i]->getFirstItem(oX, oY);
      while(obj != NULL)
      {
         buyerInventory->addObject(obj);
         buySlot[i]->removeObject(oX, oY);
         obj=buySlot[i]->getFirstItem(oX, oY);
      }
   }

   /* Next, put all sell items to the seller */
   for(i=0; i<BARTER_SELL_SLOTS; i++)
   {
      obj = sellSlot[i]->getFirstItem(oX, oY);
      while(obj != NULL)
      {
         sellerInventory->addObject(obj);
         sellSlot[i]->removeObject(oX, oY);
         obj = sellSlot[i]->getFirstItem(oX, oY);
      }
   }

   return(true);
}

/*******************************************************************
 *                           imposeBarter                          *
 *******************************************************************/
bool barter::imposeBarter()
{
   //TODO, verify if the seller will accept the imposition
   //if(accept)
   //{
   //   doBarter();
   //   return(true);
   //}
   return(false);
}

/*******************************************************************
 *                               draw                              *
 *******************************************************************/
void barter::draw(int x, int y, SDL_Surface* surface,
                  int curSellSlot, int curBuySlot)
{
   SDL_Rect ret;
   /* Blit the Barter Image */
   ret.x = x;
   ret.y = y;
   ret.w = barterImage->w;
   ret.h = barterImage->h;
   SDL_BlitSurface(barterImage, NULL, surface, &ret);

   //FIXME draw at correct positions

   /* Draw the SellSlot */
   sellSlot[curSellSlot]->draw(x,y, surface);

   /* Draw the BuySlot */
   buySlot[curBuySlot]->draw(x,y,surface);

}

/*******************************************************************
 *                          addSellItem                            *
 *******************************************************************/
bool barter::addSellItem(object* obj)
{
   int i;
   for(i=0; i < BARTER_SELL_SLOTS; i++)
   {
      if(sellSlot[i]->addObject(obj))
      {
         return(true);
      }
   }
   return(false);
}

/*******************************************************************
 *                           addBuyItem                            *
 *******************************************************************/
bool barter::addBuyItem(object* obj)
{
   int i;
   for(i=0; i < BARTER_BUY_SLOTS; i++)
   {
      if(buySlot[i]->addObject(obj))
      {
         return(true);
      }
   }
   return(false);
}

/*******************************************************************
 *                          removeBuyItem                          *
 *******************************************************************/
void barter::removeBuyItem(int x, int y, int curBuySlot)
{
   buySlot[curBuySlot]->removeObject(x,y);
}

/*******************************************************************
 *                         removeSellItem                          *
 *******************************************************************/
void barter::removeSellItem(int x, int y, int curSellSlot)
{
   sellSlot[curSellSlot]->removeObject(x,y);
}


