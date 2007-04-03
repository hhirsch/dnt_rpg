#ifndef _inventory_h
#define _inventory_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "object.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define INVENTORY_SIZE_X 13 /**< Number of Spaces on X axis */
#define INVENTORY_SIZE_Y 6  /**< Number of Spaces on Y axis */

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

/*! Definition of each inventory space */
class space
{
   public:
      object* obj;    /**< Occupied Object */
      int origX,      /**< X Orign of the Object on Inventory (space) */
          origY;      /**< Y Orign of the Object on Inventory (space) */
      
};

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
       * \return -> true if added to inventory, false if can't add. */
      bool addObject(object* obj, int x, int y);

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
       * \return -> pointer to object on position */
      object* getFromPosition(int x, int y);

      /*! Get object from iventory place
       * \param where -> place ID on inventory
       * \return -> pointer to object on position */
      object* getFromPlace(int where);

      /*! Remove object from inventory
       * \param obj -> pointer to object to be removed */
      void removeFromInventory(object* obj);

      /*! Remove object from inventory
       * \param x -> x space position on inventory
       * \param y -> y space position on inventory */
      void removeFromInventory(int x, int y);

      /*! Remove Object from Equipped Place
       * \param where -> equipped place ID */
      void removeFromPlace(int where);

      /*! Verify if can Add object to inventory position
       * \param obj -> pointer to object to be added
       * \param x -> x inventory position
       * \param y -> y inventory position
       * \return -> true if can add, false otherwise */
      bool canAdd(object* obj, int x, int y);

      /*! Draw Inventory to Surface
       * \param surface -> SDL_Surface to draw the iventory 
       * \param x -> x coordinate on surface
       * \param y -> y coordinate on surface */
      void draw(int x, int y, SDL_Surface* surface);

      /*! Draw Equiped Itens to Surface
       * \param surface -> SDL_Surface to draw the iventory 
       * \param x -> x coordinate on surface
       * \param y -> y coordinate on surface */
      void drawEquiped(int x, int y, SDL_Surface* surface);

   private:
      void print();

      space spaces[INVENTORY_SIZE_X][INVENTORY_SIZE_Y];/**< The inventory */
      object* equippedObject[INVENTORY_TOTAL_PLACES];  /**< Equipped Current 
                                                            Objects */
      SDL_Surface* inventoryImage;      /**< The Inventory Image */

};

#endif

