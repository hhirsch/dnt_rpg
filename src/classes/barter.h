#ifndef _barter_h
#define _barter_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/
#include "object.h"
#include "itemslot.h"
#include "inventory.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define BARTER_BUY_SLOTS   3  /**< Number of Buy Slots */
#define BARTER_SELL_SLOTS  3  /**< Number of Sell Slots */

/*! The barter class. It will use the appraise skill to 
 * define the item value, for each one bartering. */
class barter
{
   public:
      /*! Constructor
       * \param seller -> pointer to the seller inventory
       * \param buyer -> pointer to the buyer inventory */
      barter(inventory* seller, inventory* buyer);
      /*! Destructor */
      ~barter();

      /*! Cancel the Actual Barter */
      void cancelBarter();

      /*! Impose the Actual Barter
       * \return -> true if imposition was accept, false otherwise. 
       * \note Usually, when refuse to impose, enter on fight mode. */
      bool imposeBarter();

      /*! Do the actual Barter
       * \return -> true if the barter was accept, false otherwise. */
      bool doBarter();

      /*! Draw the actual buySlot and SellSlot
       * \param x -> beginning x coordinate 
       * \param y -> beginning y coordinate
       * \param surface -> surface where will draw
       * \param curSellSlot -> current sell slot
       * \param curBuySlot -> current buy slot */
      void draw(int x, int y, SDL_Surface* surface,
                 int curSellSlot, int curBuySlot);

      /*! Add Item for Sell
       * \note You need to remove the item from where it comes from
       *       after call this function.
       * \param obj -> pointer to the item to include 
       * \return true fi can add for sell, false otherwise */
      bool addSellItem(object* obj);

      /*! Add Item for Buy
       * \note You need to remove the item from where it comes from
       *       after call this function.
       * \param obj -> pointer to the item to include 
       * \return true fi can add for buy, false otherwise */
      bool addBuyItem(object* obj);

      /*! Remove a Item of Buy Slots
       * \param x -> x coordinate
       * \param y -> y coordinate
       * \param curBuySlot -> current buy Slot */
      void removeBuyItem(int x, int y, int curBuySlot);

      /*! Remove a Item of Sell Slots
       * \param x -> x coordinate
       * \param y -> y coordinate
       * \param curSellSlot -> current sell Slot */
      void removeSellItem(int x, int y, int curSellSlot);

   private:
      inventory* sellerInventory;   /**< Pointer to the Seller Inventory */
      inventory* buyerInventory;    /**< Pointer to the Buyer Inventory */

      itemSlot* buySlot[BARTER_BUY_SLOTS];  /**< The itemSlot of Buy things */
      itemSlot* sellSlot[BARTER_SELL_SLOTS];/**< The itemSlot of Sell things */

      SDL_Surface* barterImage;     /**< The Barter Image */
};

#endif

