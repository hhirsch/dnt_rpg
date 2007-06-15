#ifndef _itemslot_h
#define _itemslot_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "object.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

/*! Definition of each slot space */
class space
{
   public:
      space();
      object* obj;    /**< Occupied Object */
      int origX,      /**< X Orign of the Object on itemSlot */
          origY;      /**< Y Orign of the Object on itemSlot */
      
};

/*! itemSlot Class. A itemSlot is just some spaces to have itens into.
 * It's used at the inventory and the trade Buy/Sell spaces */
class itemSlot
{
   public:
      /*! Constructor
       * \param xSize -> x size of the slot
       * \param ySize -> y size of the slot */
      itemSlot(int xSize, int ySize);
      /*! Destruc */
      ~itemSlot();

      /*! Add, if can, the object to the itemSlot at position x,y.
       * \param obj -> pointer to the object to add
       * \param x -> x position on itemSlot
       * \param y -> y position on itemSlot
       * \return -> true if added, false otherwise. */
      bool addObject(object* obj, int x, int y);
      /*! Add the item to the first avaible slot
       * \param obj -> pointer to object to add
       * \return -> true if addes, false otherwise. */
      bool addObject(object* obj);
      /*! Verify if can add the item to the position x,y
       * \param obj -> item to verify
       * \param x -> x position on slot
       * \param y -> y position on slot 
       * \return true if can add, false otherwise. */
      bool canAdd(object* obj, int x, int y);

      /*! Get object that is on the position x,y. The function will not remove
       * it from the position. 
       * \param x -> x slot coordinate
       * \param y -> y slot coordinate 
       * \return -> pointer to the item, or NULL. */
      object* getFromPosition(int x, int y);

      /*! Get the first item founded on the slot. Usefull when removing all
       * itens from the slot, like case of death or cancelled barter.
       * \param x -> x position of the item got
       * \param y -> y position of the item got
       * \return -> pointer to the first founded item, or NULL. */
      object* getFirstItem(int& x, int& y);

      /*! Remove All references to the object from the itemSlot
       * \param obj -> pointer to object to remove */
      void removeObject(object* obj);
      /*! Remove Object from the itemSlot position x,y
       * \param x -> x itemSlot position
       * \param y -> y itemSlot position */
      void removeObject(int x, int y);

      /*! Draw all objectsfrom the itemSlot to the surface, 
       *  begining at the x,y surface coordinate.
       *  \param x -> initial surface x coordinate
       *  \param y -> initial surface y coordinate
       *  \param surface -> surface where will draw*/
      void draw(int x, int y, SDL_Surface* surface);

      /*! Set the slot to only accepts one item on it per time. Usefully fot
       *  equipped objects slots. */
      void setAsOneItemPerTime();
   private:
      int sizeX,  /**< Size X of the slot */
          sizeY;  /**< Size Y of the slot */

      space** spaces;  /**< Each Space of the Slot */
      bool oneItemPerTime; /**< If only accepts one item at the slot per time,
                                usually used at the equipped objects slots.
                                When only one item per time, it will ALWAYS
                                at the 0,0 position of the slot. */

};


#endif

