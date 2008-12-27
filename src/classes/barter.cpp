/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "barter.h"
#include "../engine/inventwindow.h"


#define SELL_SIZE_X   6  /**< Size of Sell Slots */
#define SELL_SIZE_Y  10  /**< Size of Sell Slots */
#define BUY_SIZE_X    6  /**< Size of Buy Slots */
#define BUY_SIZE_Y   10  /**< Size of Buy Slots */

/*******************************************************************
 *                           Constructor                           *
 *******************************************************************/
barter::barter(character* s, character* b)
{
   int i;

   /* Empty the Values */
   totalBuyValue = 0;
   totalSellValue = 0;

   /* Define the Inventories */
   seller = s;
   buyer = b;

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
   dirs dir;
   barterImage = IMG_Load(
                       dir.getRealFile("texturas/inventory/trade.png").c_str());
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
   seller = NULL;
   buyer = NULL;
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
         seller->inventories->addObject(obj);
         buySlot[i]->removeObject(oX, oY);
         obj = buySlot[i]->getFirstItem(oX, oY);
      }
   }
   totalBuyValue = 0;

   /* Next, reput all sell items to the buyer */
   for(i=0; i < BARTER_SELL_SLOTS; i++)
   {
      obj = sellSlot[i]->getFirstItem(oX, oY);
      while(obj != NULL)
      {
         buyer->inventories->addObject(obj);
         sellSlot[i]->removeObject(oX, oY);
         obj = sellSlot[i]->getFirstItem(oX, oY);
      }
   }
   totalSellValue = 0;
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
   bool accept = false;

   // FIXME -> implement the appraise skill here for both
   // as the DNT Master Book describes!

   if(totalSellValue > totalBuyValue)
   {
      /* The trade is good to the seller, so always accept */
      accept = true;
   }
   else
   {
      /* Isn't clearly good. Must do an accept check */
      dice d20;
      float roll = d20.roll() / 30.0;
      float sellDiff = (totalSellValue - totalBuyValue);
      float eqn = 1.0 / (seller->curBonusAndSaves.iAmNotAFool+fabs(sellDiff));

      accept = eqn > roll;
   }

   if(accept)
   {
      /* First, put all buy items to the buyer */
      for(i=0; i<BARTER_BUY_SLOTS; i++)
      {
         obj=buySlot[i]->getFirstItem(oX, oY);
         while(obj != NULL)
         {
            buyer->inventories->addObject(obj);
            buySlot[i]->removeObject(oX, oY);
            obj = buySlot[i]->getFirstItem(oX, oY);
         }
      }

      /* Next, put all sell items to the seller */
      for(i=0; i<BARTER_SELL_SLOTS; i++)
      {
         obj = sellSlot[i]->getFirstItem(oX, oY);
         while(obj != NULL)
         {
            seller->inventories->addObject(obj);
            sellSlot[i]->removeObject(oX, oY);
            obj = sellSlot[i]->getFirstItem(oX, oY);
         }
      }
   }

   return(accept);
}

/*******************************************************************
 *                           imposeBarter                          *
 *******************************************************************/
bool barter::imposeBarter()
{
   dice d20;

   /******************************* 
    * The Test is: 
    *    (mod(ATT) * 100 ) / dP
    * Versus
    *    (IANAF * dP) / 100 
    *******************************/
   float dP = (totalBuyValue - totalSellValue);
   float invDP = 1.0f / dP;

   float stTest = invDP*100*(d20.roll()+buyer->attBonus(ATT_STRENGTH)) - 
                  dP*0.01f*(d20.roll()+seller->curBonusAndSaves.iAmNotAFool);
   float coTest = invDP*100*(d20.roll()+buyer->attBonus(ATT_CONSTITUTION)) - 
                  dP*0.01f*(d20.roll()+seller->curBonusAndSaves.iAmNotAFool);

   /*cout << "dP: " << dP << endl;
   cout << "St: " << stTest << endl;
   cout << "Co: " << coTest << endl;*/

   if( (stTest > 0) && (coTest > 0) )
   {
      /* Both Tests Pass, so accept */
      doBarter();
      return(true);
   }
   else if( (stTest <= 0) && (coTest <= 0) )
   {
      /* Both Tests Fails, so battle */
      //TODO
   }

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

   /* Draw the SellSlot */
   sellSlot[curSellSlot]->draw(x,y, surface);

   /* Draw the BuySlot */
   buySlot[curBuySlot]->draw(x+133,y,surface);

}

/*******************************************************************
 *                         getFromPosition                         *
 *******************************************************************/
object* barter::getFromPosition(int posX, int posY, int curInv, bool seller)
{
   if( (seller) && (curInv < BARTER_SELL_SLOTS))
   {
      return(buySlot[curInv]->getFromPosition(posX, posY));
   }
   else if( (!seller) && (curInv < BARTER_BUY_SLOTS))
   {
      return(sellSlot[curInv]->getFromPosition(posX, posY));
   }

   return(NULL);
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
         totalSellValue += obj->cost;
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
         totalBuyValue += obj->cost;
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
   /* Get the desired object */
   object* obj = buySlot[curBuySlot]->getFromPosition(x,y);
   if(obj)
   {
      /* Put it back at the sellers inventory */
      seller->inventories->addObject(obj);
      if(seller->inventories->getOpenedWindow() != NULL)
      {
         inventWindow* i=(inventWindow*)seller->inventories->getOpenedWindow();
         i->reDraw();
      }

      /* Dec the total value and remove from inventory */
      totalBuyValue -= obj->cost;
      buySlot[curBuySlot]->removeObject(x,y);
   }
}

/*******************************************************************
 *                         removeSellItem                          *
 *******************************************************************/
void barter::removeSellItem(int x, int y, int curSellSlot)
{
   /* Get the desired object */
   object* obj = sellSlot[curSellSlot]->getFromPosition(x,y);
   if(obj)
   {
      /* Put it back at the buyer's inventory */
      buyer->inventories->addObject(obj);
      if(buyer->inventories->getOpenedWindow() != NULL)
      {
         inventWindow* i = (inventWindow*)buyer->inventories->getOpenedWindow();
         i->reDraw();
      }
      
      /* Dec from the total value and remove from inventory */
      totalSellValue -= obj->cost;
      sellSlot[curSellSlot]->removeObject(x,y);
   }
}

/*******************************************************************
 *                         getTotalBuyValue                        *
 *******************************************************************/
float barter::getTotalBuyValue()
{
   return(totalBuyValue);
}

/*******************************************************************
 *                        getTotalSellValue                        *
 *******************************************************************/
float barter::getTotalSellValue()
{
   return(totalSellValue);
}

