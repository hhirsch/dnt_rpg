/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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



#ifndef _dnt_money_window_h
#define _dnt_money_window_h

#include "../gui/farso.h"
#include "../classes/money.h"
#include "../classes/inventory.h"
#include "../map/map.h"

enum
{
   MONEY_INVENTORY_TO_SELL,
   MONEY_INVENTORY_TO_BUY,
   MONEY_SELL_TO_INVENTORY,
   MONEY_BUY_TO_INVENTORY,
   MONEY_INVENTORY_TO_FLOOR
};

/*! The money window is used to select money quantity from 
 * a source (inventory) to destiny (inventory or floor).  */
class moneyWindow
{
   public:
      /*! Set the map and interface used for moneyWindow */
      static void setMap(Farso::GuiInterface* interf, Map* m);

      /*! Open the money window for inventory to sell/buy transaction */
      static void open(inventory* source, bool sell);

      /*! Open the money window for sell/buy to inventory transaction */
      static void open(bool sell, inventory* destiny);

      /*! Open the money window for inventory to floor position transaction */
      static void open(inventory* source, float pX, float pZ);

      /*! Cancel the money window, closing it */
      static void cancel();

      /*! Accept the money window, doing transaction and closeing it */
      static void accept();

      /*! Treat Events on Window. 
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return true if event is gathered */
      static bool treat(Farso::GuiObject* object, int eventInfo);

      /*! Verify if the window is openned
       * \return true if opened, false otherwise. */
      static bool isOpen();

   protected:

      /*! Open the window */
      static void open();

      /*! Close the window */
      static void close();

      static Farso::Window* intWindow;  /**< Pointer to the internal window */

      static Farso::GuiInterface* inter;  /**< Current GUI interface */
      
      static Farso::TextBox* qtyText;    /**< Text with quantity */
      static int curQty;          /**< Current quantity selected */
      static int maxQty;          /**< Max quantity to select */
      static Farso::Button* incButton;   /**< Inc quantity button  */
      static Farso::Button* decButton;   /**< Dec quantity button */
      static Farso::Button* allButton;   /**< Set quantity to max button */

      static Farso::Button* cancelButton; /**< cancel button */
      static Farso::Button* okButton;     /**< The ok Button */

      
      static int type;             /**< type of money transaction */
      static inventory* inv;       /**< inventory used at transaction */
      static float destinyPosX;    /**< floor X position for transaction */
      static float destinyPosZ;    /**< floor Z position for transaction */

      static Map* curMap;          /**< current opened map */
};



#endif

