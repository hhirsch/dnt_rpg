/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#ifndef _barter_window_h
#define _barter_window_h

#include "../lang/translate.h"
#include "../gui/farso.h"
#include "../classes/barter.h"
#include "inventwindow.h"
#include "itemwindow.h"

class engine;

/*! The Barter Window Class. In the barter window, objects to sell and buy 
 * are inserted, and you can offer the barter, impose the barter or cancel. */
class barterWindow
{
   public:
      /*! Constructor */
      barterWindow();

      /*! Destructor */
      ~barterWindow();

      /*! Open the barter window 
       * \param s -> pointer to the seller character
       * \param b -> pointer to the buyer character 
       * \param inter -> GUI interface used 
       * \param usedEngine -> pointer to the engine used */
      void open(character *s, character* b,
                guiInterface* inter, itemWindow* infoW, engine* usedEngine);

      /*! Close / Remove structures from the barter window */
      void close();

      /*! Treat Events on Window. 
       * \param guiObj -> last GUI object
       * \param eventInfo -> last GUI Event
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate
       * \param mouseCursor -> the mouse cursor
       * \param actualMap -> pointer to the opened map
       * \return true if event is threated, false otherwise. */
      bool treat(guiObject* guiObj, int eventInfo, int mouseX, int mouseY,
                 cursor* mouseCursor, Map* actualMap);

      /*! Verify if window is still opened
       * \return true if window is openned, false otherwise */
      bool isOpen();

      /*! Redraw the Inventory Window, to actualize things on it. */
      void reDraw();

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
       * \return true ii can add for buy, false otherwise */
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

      /*! Get the barter inventory */
      barter* getBarterInventory(){return(barterInventory);};

   private:

      /*! Cancel the barter */
      void cancel();

      /*! Offer the Barter
       * \return -> true if is accepted */
      bool offer();

      /*! Impose the Barter
       * \return -> true if the imposition is accepted */
      bool impose();

      /*! Open the object Menu
       * \param mouseX -> current mouse X coordinate
       * \param mouseY -> current mouse Y coordinate */
      void openMenu(int mouseX, int mouseY);

      static character* buyer;          /**< Buyer character */
      static character* seller;         /**< Seller character */

      static object* activeObject;      /**< Current active object */
      static int objX;                  /**< Object X position */
      static int objY;                  /**< Object Y position */

      static bool sellerObj;            /**< True if object is from seller */

      static barter* barterInventory;   /**< The barter inventory */
      static guiInterface* gui;         /**< The used GUI */
      static textBox* sellerTotals;     /**< The Seller Totals */
      static textBox* buyerTotals;      /**< The Buyer Totals */
      static window* intWindow;         /**< The window used */
      static button* imposeButton;      /**< The Impose Button */
      static button* offerButton;       /**< The Offer Button */
      static button* cancelButton;      /**< The Cancel Button */
      static tabButton* barterTabButton;/**< The Barter TabButton */
      static oneTabButton* sellerInv;   /**< The actual Inventory */
      static inventWindow* sellerWindow;/**< Pointer to the seller inventory
                                             window, if it is openned here. */
      static oneTabButton* buyerInv;    /**< The actual Inventory */
      static inventWindow* buyerWindow; /**< Pointer to the buyer inventory 
                                             window, if it is opened here. */

      static menu* objectMenu;          /**< The object menu */

      static itemWindow* infoWindow;    /**< Pointer to the item window used */

      static engine* curEngine;           /**< Pointer to the current engine */
};



#endif

