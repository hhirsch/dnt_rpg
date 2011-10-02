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

#ifndef _dnt_inventory_h
#define _dnt_inventory_h

#include "object.h"
#include "itemslot.h"
#include "money.h"
#include "../etc/dirs.h"
#include "../map/map.h"

#include <SDL/SDL.h>
#ifdef __APPLE__
   #include <SDL_image/SDL_image.h>
#else
   #include <SDL/SDL_image.h>
#endif

#define INVENTORY_SIZE_X 13 /**< Number of Spaces on X axis */
#define INVENTORY_SIZE_Y 6  /**< Number of Spaces on Y axis */

#define TRADE_SIZE_X     6  /**< Number of Spaces on X axis for Trades */
#define TRADE_SIZE_Y    10  /**< Number of Spaces on Y axis for Trades */

#define INVENTORY_TOTAL_PLACES  8  /**< Total Eqquiped Places */
#define INVENTORY_HEAD          0  /**< Head Place */
#define INVENTORY_LEFT_HAND     1  /**< Left Hand Place */
#define INVENTORY_RIGHT_HAND    2  /**< Right Hand Place */
#define INVENTORY_LEFT_FINGER   3  /**< Left Finger Place */
#define INVENTORY_RIGHT_FINGER  4  /**< Right Finger Place */
#define INVENTORY_NECK          5  /**< Neck Place */
#define INVENTORY_FOOT          6  /**< Foot Place */
#define INVENTORY_BODY          7  /**< Body Place (Torso) */

#define INVENTORY_INVENTORY     8  /**< is in inventory */

#define INVENTORY_PER_CHARACTER 4  /**< Number of Inventories per Character */

class inventWindow;

/*! Character Inventory Definition */
class inventory
{
   public:
      /*! Constructor */
      inventory();
      /*! Destructor */
      ~inventory();

      /*! Add Money to the inventory
       * \param qty -> money quantity (value) 
       * \return true if could add, false otherwise */
      bool addMoney(int qty);

      /*! Get money quantity */
      int getMoneyQuantity();

      /*! Get money object at inventory, if any */
      money* getMoney();

      /*! Dec money value from current money object
       * \note -> if remaining quantity is == 0, object is deleted. 
       * \return true if could subtract, false if quantity insuficient */
      bool decMoney(int qty);

      /*! Add object to inventory space
       * \param obj -> pointer to object to be added
       * \param x -> x space position to be the orign on inventory
       * \param y -> y space position to be the orign on inventory
       * \param curInv -> current Inventory
       * \return -> true if added to inventory, false if can't add. */
      bool addObject(object* obj, int x, int y, int curInv);

      /*! Add object to first free inventory space
       * \param obj -> pointer to object to be added
       * \return -> true if added to inventory, false if can't add. */
      bool addObject(object* obj);

      /*! Equip Object to the Place defined.
       * \param obj -> pointer to object to be equipped
       * \param where -> ID of the place to equip
       * \return -> true if equipped, false, if can't equip */
      bool equipObject(object* obj, int where);

      /* Drop object from the inventory (puting it on the floor X,Z)
       * \param obj -> object to drop from the inventory
       * \param x -> x position of the object on inventory
       * \param y -> y position of the object at inventory
       * \param inv -> inventory where object lies
       * \param actualMap -> pointer to the current map
       * \param X -> x position to drop object to
       * \param Z -> z position to drop object to */
      void dropObject(object* obj, int x, int y, int inv,
            Map* actualMap, GLfloat X, GLfloat Z);

      /*! Get object from iventory position
       * \param x -> x space position on inventory
       * \param y -> y space position on inventory
       * \param curInv -> current Inventory
       * \return -> pointer to object on position */
      object* getFromPosition(int x, int y, int curInv);

      /*! Get object from inventory place
       * \param where -> place ID on inventory
       * \return -> pointer to object on position */
      object* getFromPlace(int where);

      /*! Get the first object founded on the inventory. 
       * \param x -> x position of the item got
       * \param y -> y position of the item got
       * \param curInv -> inventory number to get object from
       * \return -> pointer to the first founded item, or NULL. */
      object* getFirstObject(int& x, int& y, int curInv);

      /*! Get next object on inventory.
       * \note -> this function is usually called after a getFirstObject,
       *          with the result x,y coordinate got from that 
       * \param x -> x coordinate of the previous item got (will have 
       *             the x coordinate of the current after the function)
       * \param y -> y coordinate of the previos item got (will have the
       *             y coordinate of the current after the function)
       * \param curInv -> inventory number to get object from
       * \return -> pointer to the object found or NULL, if no more objects */
      object* getNextObject(int& x, int& y, int curInv);

      /*! Remove object from inventory
       * \param obj -> pointer to object to be removed */
      void removeFromInventory(object* obj);

      /*! Remove object from inventory
       * \param x -> x space position on inventory
       * \param y -> y space position on inventory 
       * \param curInv -> current Inventory */
      void removeFromInventory(int x, int y, int curInv);

      /*! Remove Object from Equipped Place
       * \param where -> equipped place ID */
      void removeFromPlace(int where);

      /*! Verify if can Add object to inventory position
       * \param obj -> pointer to object to be added
       * \param x -> x inventory position
       * \param y -> y inventory position
       * \param curInv -> current Inventory
       * \return -> true if can add, false otherwise */
      bool canAdd(object* obj, int x, int y, int curInv);

      /*! Draw Inventory to Surface
       * \param surface -> SDL_Surface to draw the iventory 
       * \param x -> x coordinate on surface
       * \param y -> y coordinate on surface
       * \param curInv -> current Inventory */
      void draw(int x, int y, SDL_Surface* surface, int curInv);

      /*! Draw Equiped Itens to Surface
       * \param surface -> SDL_Surface to draw the iventory 
       * \param x -> x coordinate on surface
       * \param y -> y coordinate on surface */
      void drawEquiped(int x, int y, SDL_Surface* surface);

      /*! Get a item from the inventory with the desired fileName, if exists.
       * \param fileName -> fileName of the item
       * \return -> pointer to the found object of NULL */
      object* getItemByFileName(string fileName);

      /*! Set the inventory Opened Window
       * \param w -> pointer to an inventWindow */
      void setOpenedWindow(inventWindow* w);

      /*! Get the current inventory opened window
       * \return -> pointer to the current inventory window or NULL */
      inventWindow* getOpenedWindow();

      /*! Get total number of items at the inventory
       * \return -> total number of items */
      int getTotalItems();

   private:
      /*! For debug: print all names of inventory itens on terminal */
      void print();

      inventWindow* openedWindow;          /**< Pointer to the opened window */

      itemSlot** slots;            /**< The Inventory */
      itemSlot** equippedSlots;    /**< The Equipped Slots */

      SDL_Surface* inventoryImage; /**< The Inventory Image */
      SDL_Surface* equipedImage;   /**< The Equiped Inventory Image */

};

#endif

