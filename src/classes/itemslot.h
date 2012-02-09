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

#ifndef _dnt_itemslot_h
#define _dnt_itemslot_h

#include "object.h"
#include <SDL/SDL.h>
#ifdef __APPLE__
   #include <SDL_image/SDL_image.h>
#else
   #include <SDL/SDL_image.h>
#endif

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

      /*! Get next item on the itemSlot.
       * \note -> this function is usually called after a getFirstItem,
       *          with the result x,y coordinate got from that 
       * \param x -> x coordinate of the previous item got (will have 
       *             the x coordinate of the current after the function)
       * \param y -> y coordinate of the previos item got (will have the
       *             y coordinate of the current after the function)
       * \return -> pointer to the object found or NULL, if no more objects */
      object* getNextItem(int& x, int& y);

      /*! Remove All references to the object from the itemSlot
       * \param obj -> pointer to object to remove
       * \return -> true if can remove (object existed there) */
      bool removeObject(object* obj);
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

      /*! Get a item from the slots with the desired fileName, if exists.
       * \param fileName -> fileName of the item
       * \return -> pointer to the found object of NULL */
      object* getItemByFileName(std::string fileName);

      /*! Get the number of items at the itemSlot
       * \return -> number of items at the itemSlot */
      int getTotalItems();

   private:
      int sizeX,  /**< Size X of the slot */
          sizeY;  /**< Size Y of the slot */

      int totalItems;  /**< Number of Items at the itemSlot */

      space** spaces;  /**< Each Space of the Slot */
      bool oneItemPerTime; /**< If only accepts one item at the slot per time,
                                usually used at the equipped objects slots.
                                When only one item per time, it will ALLWAYS
                                at the 0,0 position of the slot. */

};


#endif

