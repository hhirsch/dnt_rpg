#ifndef _inventory_h
#define _inventory_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "object.h"
#include "itemslot.h"
#include "../etc/dirs.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

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

/*! Character Inventory Definition */
class inventory
{
   public:
      /*! Constructor */
      inventory();
      /*! Destructor */
      ~inventory();

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
       * \param window -> pointer to an inventWindow */
      void setOpenedWindow(void* window);

      /*! Get the current inventory opened window
       * \return -> pointer to the current inventory window or NULL */
      void* getOpenedWindow();

   private:
      /*! For debug: print all names of inventory itens on terminal */
      void print();

      void* openedWindow;          /**< Pointer to the opened window */

      itemSlot** slots;            /**< The Inventory */
      itemSlot** equippedSlots;    /**< The Equipped Slots */

      SDL_Surface* inventoryImage; /**< The Inventory Image */
      SDL_Surface* equipedImage;   /**< The Equiped Inventory Image */

};

#endif

