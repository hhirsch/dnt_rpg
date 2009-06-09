/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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

#ifndef _dnt_barter_h
#define _dnt_barter_h

#include "object.h"
#include "itemslot.h"
#include "inventory.h"
#include "../etc/dirs.h"
#include "../engine/character.h"
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
       * \param s -> pointer to the seller inventory
       * \param b -> pointer to the buyer inventory */
      barter(character* s, character* b);
      /*! Destructor */
      ~barter();

      /*! Cancel the Current Barter */
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
       * \note You need to remove the item from where it came from
       *       after call this function.
       * \param obj -> pointer to the item to include 
       * \return true if can add for sell, false otherwise */
      bool addSellItem(object* obj);

      /*! Add Item for Buy
       * \note You need to remove the item from where it came from
       *       after call this function.
       * \param obj -> pointer to the item to include 
       * \return true if can add for buy, false otherwise */
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

      /*! Get the current buy goods value
       * \return -> total value of goods on the buy inventory */
      float getTotalBuyValue();
      
      /*! Get the current sell goods value
       * \return -> total value of goods on the sell inventory */
      float getTotalSellValue();

      /*! Get an object (if exists) at inventory position
       * \param posX -> X position
       * \param posY -> Y position
       * \param curInv -> current inventory number
       * \param seller -> true if seller, false if buyer 
       * \return -> pointer to object or NULL */
      object* getFromPosition(int posX, int posY, int curInv, bool seller);

   private:
      /*! Accept the current barter, changing items  */
      void acceptBarter();
     
      character* buyer;             /**< The buyer character */
      character* seller;            /**< The Seller character */

      float totalBuyValue;          /**< The total value at the buy slot */
      float totalSellValue;         /**< The total value at the sell slot */
       
      itemSlot* buySlot[BARTER_BUY_SLOTS];  /**< The itemSlot of Buy things */
      itemSlot* sellSlot[BARTER_SELL_SLOTS];/**< The itemSlot of Sell things */

      SDL_Surface* barterImage;     /**< The Barter Image */
};

#endif

