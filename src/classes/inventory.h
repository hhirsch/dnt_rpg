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

      /*! Get object from iventory position
       * \param x -> x space position on inventory
       * \param y -> y space position on inventory
       * \return -> pointer to object on position */
      object* getFromPosition(int x, int y);

      /*! Remove object from inventory
       * \param obj -> pointer to object to be removed */
      void removeFromInventory(object* obj);

      /*! Remove object from inventory
       * \param x -> x space position on inventory
       * \param y -> y space position on inventory */
      void removeFromInventory(int x, int y);

      /*! Verify if can Add object to inventory position
       * \param obj -> pointer to object to be added
       * \param x -> x inventory position
       * \param y -> y inventory position
       * \return -> true if can add, false otherwise */
      bool canAdd(object* obj, int x, int y);

      /*! Draw Iventory to Surface
       * \param surface -> SDL_Surface to draw the iventory 
       * \param x -> x coordinate on surface
       * \param y -> y coordinate on surface */
      void draw(int x, int y, SDL_Surface* surface);


   private:
      space spaces[INVENTORY_SIZE_X][INVENTORY_SIZE_Y];/**< The inventory */
      SDL_Surface* inventoryImage;                   /**< The Inventory Image */

};

#endif

