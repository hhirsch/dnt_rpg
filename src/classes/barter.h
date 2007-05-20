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

   private:
      inventory* sellerInventory;   /**< Pointer to the Seller Inventory */
      inventory* buyerInventory;    /**< Pointer to the Buyer Inventory */

      itemSlot* buySlot;            /**< The itemSlot of Buy things */
      itemSlot* sellSlot;           /**< The itemSlot of Sell things */
};

#endif

