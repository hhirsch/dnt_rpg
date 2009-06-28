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

#include "inventory.h"
#include "object.h"
#include "money.h"
#include "../engine/inventwindow.h"

/**************************************************************
 *                          Constructor                       *
 **************************************************************/
inventory::inventory()
{
   int i;
   inventoryImage = NULL;
   openedWindow = NULL;

   /* Create Slots */
   slots = new itemSlot*[INVENTORY_PER_CHARACTER];
   for(i = 0; i<INVENTORY_PER_CHARACTER; i++)
   {
      slots[i] = new itemSlot(INVENTORY_SIZE_X, INVENTORY_SIZE_Y);
   }

   /* Create Equipped */
   equippedSlots = new itemSlot*[INVENTORY_TOTAL_PLACES];
   equippedSlots[INVENTORY_HEAD] = new itemSlot(2,2);
   equippedSlots[INVENTORY_HEAD]->setAsOneItemPerTime();
   equippedSlots[INVENTORY_LEFT_HAND] = new itemSlot(2,4);
   equippedSlots[INVENTORY_LEFT_HAND]->setAsOneItemPerTime();
   equippedSlots[INVENTORY_RIGHT_HAND] = new itemSlot(2,4);
   equippedSlots[INVENTORY_RIGHT_HAND]->setAsOneItemPerTime();
   equippedSlots[INVENTORY_LEFT_FINGER] = new itemSlot(1,1);
   equippedSlots[INVENTORY_LEFT_FINGER]->setAsOneItemPerTime();
   equippedSlots[INVENTORY_RIGHT_FINGER] = new itemSlot(1,1);
   equippedSlots[INVENTORY_RIGHT_FINGER]->setAsOneItemPerTime();
   equippedSlots[INVENTORY_NECK] = new itemSlot(2,1);
   equippedSlots[INVENTORY_NECK]->setAsOneItemPerTime();
   equippedSlots[INVENTORY_FOOT] = new itemSlot(2,2);
   equippedSlots[INVENTORY_FOOT]->setAsOneItemPerTime();
   equippedSlots[INVENTORY_BODY] = new itemSlot(3,4);
   equippedSlots[INVENTORY_BODY]->setAsOneItemPerTime();

   /* Load Images */
   dirs dir;
   inventoryImage = IMG_Load(
                   dir.getRealFile("texturas/inventory/inventory.png").c_str());
   if(!inventoryImage)
   {
      printf("Can't Load Inventory Image!\n");
   }
   equipedImage = IMG_Load(
                       dir.getRealFile("texturas/inventory/equip.png").c_str());
   if(!inventoryImage)
   {
      printf("Can't Load Equiped Inventory Image!\n");
   }
}

/**************************************************************
 *                           Destructor                       *
 **************************************************************/
inventory::~inventory()
{
   int i;
   /* Free Slots */
   for(i=0; i<INVENTORY_PER_CHARACTER;i++)
   {
      delete slots[i];
   }
   delete[] slots;

   for(i = 0; i<INVENTORY_TOTAL_PLACES; i++)
   {
      delete equippedSlots[i];
   }
   delete[] equippedSlots;

   /* Free Images */
   if(inventoryImage)
   {
      SDL_FreeSurface(inventoryImage);
   }
   if(equipedImage)
   {
      SDL_FreeSurface(equipedImage);
   }
}

/**************************************************************
 *                       setOpenedWindow                      *
 **************************************************************/
void inventory::setOpenedWindow(void* window)
{
   openedWindow = window;
}

/**************************************************************
 *                        getOpenedWindow                     *
 **************************************************************/
void* inventory::getOpenedWindow()
{
   return(openedWindow);
}

/**************************************************************
 *                         getTotalItems                      *
 **************************************************************/
int inventory::getTotalItems()
{
   int i;
   int total = 0;
   for(i=0; i<INVENTORY_PER_CHARACTER;i++)
   {
      total += slots[i]->getTotalItems();
   }
   return(total);
}

/**************************************************************
 *                           addObject                        *
 **************************************************************/
bool inventory::addObject(object* obj, int x, int y, int curInv)
{
   bool res = false;

   /* Add the object */
   res = slots[curInv]->addObject(obj, x, y);

   /* Redraw the window if needed */
   if( (res) && (openedWindow != NULL))
   {
      inventWindow* i = (inventWindow*)openedWindow;
      i->reDraw();
   }

   return(res);
}

/**************************************************************
 *                          equipObject                       *
 **************************************************************/
bool inventory::equipObject(object* obj, int where)
{
   if( (obj) && (where >= 0) && (where < INVENTORY_TOTAL_PLACES) )
   {
      //TODO verify if object Use Type is compatible with other places

      if( (obj->getType() == OBJECT_TYPE_WEAPON) && 
          ( (where == INVENTORY_LEFT_HAND) ||
            (where == INVENTORY_RIGHT_HAND)))
      {
         return(equippedSlots[where]->addObject(obj,0,0));
      }
   }
   return(false);
}

/**************************************************************
 *                           addObject                        *
 **************************************************************/
bool inventory::addObject(object* obj)
{
   int inv = 0;
   money* m, *cur;

   if(obj->getType() == OBJECT_TYPE_MONEY)
   {
      cur = (money*)obj;
      /* Search for money already on the inventory */
      for(inv = 0; inv < INVENTORY_PER_CHARACTER; inv++)
      {
         m = (money*)slots[inv]->getItemByFileName(DNT_MONEY_OBJECT);
         if(m != NULL)
         {
            /* Add the money quantity to the current */
            m->addQuantity(cur->quantity());
            /* bye current! */
            delete(cur);

            return(true);
         }
      }
   }

   inv = 0;
   while( (!slots[inv]->addObject(obj) && (inv < INVENTORY_PER_CHARACTER)))
   {
      inv++;
   }

   /* Redraw the window if needed */
   if( (inv < INVENTORY_PER_CHARACTER) && (openedWindow != NULL))
   {
      inventWindow* i = (inventWindow*)openedWindow;
      i->reDraw();
   }

   return(inv < INVENTORY_PER_CHARACTER);
}

/**************************************************************
 *                            canAdd                          *
 **************************************************************/
bool inventory::canAdd(object* obj, int x, int y, int curInv)
{
   return(slots[curInv]->canAdd(obj, x, y));
}

/**************************************************************
 *                         getFromPlace                       *
 **************************************************************/
object* inventory::getFromPlace(int where)
{
   if( (where >= 0) && (where < INVENTORY_TOTAL_PLACES) )
   {
      return(equippedSlots[where]->getFromPosition(0,0));
   }
   return(NULL);
}

/**************************************************************
 *                       getFromPosition                      *
 **************************************************************/
object* inventory::getFromPosition(int x, int y, int curInv)
{
   if( (x >= 0) && (y >=0) && (x < INVENTORY_SIZE_X) &&
       (y < INVENTORY_SIZE_Y))
   {
      return(slots[curInv]->getFromPosition(x,y));
   }
   return(NULL);
}

/**************************************************************
 *                        getFirstObject                      *
 **************************************************************/
object* inventory::getFirstObject(int& x, int& y, int curInv)
{
   return(slots[curInv]->getFirstItem(x,y));
}

/**************************************************************
 *                         getNextObject                      *
 **************************************************************/
object* inventory::getNextObject(int& x, int& y, int curInv)
{
   return(slots[curInv]->getNextItem(x,y));
}

/**************************************************************
 *                       removeFromPlace                      *
 **************************************************************/
void inventory::removeFromPlace(int where)
{
   if( (where >= 0) && (where < INVENTORY_TOTAL_PLACES) )
   {
      equippedSlots[where]->removeObject(0,0);
   }
}

/**************************************************************
 *                     removeFromInventory                    *
 **************************************************************/
void inventory::removeFromInventory(object* obj)
{
   int i;
   bool removed = false;

   /* Remove the first object equal to this one */
   for(i=0; ((i < INVENTORY_PER_CHARACTER) && (!removed)); i++)
   {
      removed = slots[i]->removeObject(obj);
   }
}

/**************************************************************
 *                             print                          *
 **************************************************************/
void inventory::print()
{
   /* Printf, for debug, when needed, uncomment */
   /*int x,y;
   printf("\n****************************************\n");
   for(y=0; y < INVENTORY_SIZE_Y; y++)
   {
      for(x=0; x < INVENTORY_SIZE_X; x++)
      {
         printf("%p\t", spaces[x][y].obj);
      }
      printf("\n");
   }*/
}


/**************************************************************
 *                     removeFromInventory                    *
 **************************************************************/
void inventory::removeFromInventory(int x, int y, int curInv)
{
   slots[curInv]->removeObject(x,y);
}

/**************************************************************
 *                             draw                           *
 **************************************************************/
void inventory::draw(int x, int y, SDL_Surface* surface, int curInv)
{
   SDL_Rect ret;

   /* First, blit the inventory image with the surface at desired position */
   ret.x = x;
   ret.y = y;
   ret.w = inventoryImage->w;
   ret.h = inventoryImage->h;
   SDL_BlitSurface(inventoryImage, NULL, surface, &ret);

   /* Next, Blit all inventory objects image on surface */
   slots[curInv]->draw(x,y,surface);
}

/**************************************************************
 *                        drawEquiped                         *
 **************************************************************/
void inventory::drawEquiped(int x, int y, SDL_Surface* surface)
{
   SDL_Rect ret;

   /* First, blit the equipped image with the surface at desired position */
   ret.x = x;
   ret.y = y;
   ret.w = inventoryImage->w;
   ret.h = inventoryImage->h;
   SDL_BlitSurface(equipedImage, NULL, surface, &ret);

   /* Next, draw all the slots */
   equippedSlots[INVENTORY_HEAD]->draw(x+109,y+3, surface);
   equippedSlots[INVENTORY_LEFT_HAND]->draw(x+159,y+112, surface);
   equippedSlots[INVENTORY_RIGHT_HAND]->draw(x+58,y+112, surface);
   equippedSlots[INVENTORY_LEFT_FINGER]->draw(x+200,y+169, surface);
   equippedSlots[INVENTORY_RIGHT_FINGER]->draw(x+37,y+169, surface);
   equippedSlots[INVENTORY_NECK]->draw(x+159,y+77, surface);
   equippedSlots[INVENTORY_FOOT]->draw(x+109,y+214, surface);
   equippedSlots[INVENTORY_BODY]->draw(x+95,y+100, surface);
}

/**************************************************************
 *                     getItemByFileName                      *
 **************************************************************/
object* inventory::getItemByFileName(string fileName)
{
   object* res = NULL;

   /* First, search at the normal slots */
   int i;
   for(i=0; i<INVENTORY_PER_CHARACTER; i++)
   {
      res = slots[i]->getItemByFileName(fileName);
      if(res != NULL)
      {
         return(res);
      }
   }

   /* Next search at the equipped ones */
   for(i = 0; i < INVENTORY_TOTAL_PLACES; i++)
   {
      res = equippedSlots[i]->getItemByFileName(fileName);
      if(res != NULL)
      {
         return(res);
      }
   }

   return(NULL);
}


