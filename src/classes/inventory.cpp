/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "inventory.h"
#include "object.h"


/**************************************************************
 *                          Constructor                       *
 **************************************************************/
inventory::inventory()
{
   inventoryImage = NULL;

   /* Create Slots */
   slots = new itemSlot(INVENTORY_SIZE_X, INVENTORY_SIZE_Y);

   /* Create Equipped */
   equippedSlots = new itemSlot*[INVENTORY_TOTAL_PLACES];
   equippedSlots[INVENTORY_HEAD] = new itemSlot(2,2);
   equippedSlots[INVENTORY_LEFT_HAND] = new itemSlot(2,4);
   equippedSlots[INVENTORY_RIGHT_HAND] = new itemSlot(2,4);
   equippedSlots[INVENTORY_LEFT_FINGER] = new itemSlot(1,1);
   equippedSlots[INVENTORY_RIGHT_FINGER] = new itemSlot(1,1);
   equippedSlots[INVENTORY_NECK] = new itemSlot(2,1);
   equippedSlots[INVENTORY_FOOT] = new itemSlot(2,2);
   equippedSlots[INVENTORY_BODY] = new itemSlot(3,4);

   /* Load Images */
   inventoryImage = IMG_Load("../data/texturas/inventory/inventory.png");
   if(!inventoryImage)
   {
      printf("Can't Load Inventory Image!\n");
   }
   equipedImage = IMG_Load("../data/texturas/inventory/equip.png");
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
   delete(slots);

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
 *                           addObject                        *
 **************************************************************/
bool inventory::addObject(object* obj, int x, int y)
{
   return(slots->addObject(obj, x, y));
}

/**************************************************************
 *                          equipObject                       *
 **************************************************************/
bool inventory::equipObject(object* obj, int where)
{
   if( (obj) && (where >= 0) && (where < INVENTORY_TOTAL_PLACES) )
   {
      //TODO verify if object Use Type is compatible with the place
      return(equippedSlots[where]->addObject(obj,0,0));
   }
   return(false);
}

/**************************************************************
 *                           addObject                        *
 **************************************************************/
bool inventory::addObject(object* obj)
{
   return(slots->addObject(obj));
}

/**************************************************************
 *                            canAdd                          *
 **************************************************************/
bool inventory::canAdd(object* obj, int x, int y)
{
   return(slots->canAdd(obj, x, y));
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
object* inventory::getFromPosition(int x, int y)
{
   if( (x >= 0) && (y >=0) && (x < INVENTORY_SIZE_X) &&
       (y < INVENTORY_SIZE_Y))
   {
      return(slots->getFromPosition(x,y));
   }
   return(NULL);
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
   slots->removeObject(obj);
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
void inventory::removeFromInventory(int x, int y)
{
   slots->removeObject(x,y);
}

/**************************************************************
 *                             draw                           *
 **************************************************************/
void inventory::draw(int x, int y, SDL_Surface* surface)
{
   SDL_Rect ret;

   /* First, blit the inventory image with the surface at desired position */
   ret.x = x;
   ret.y = y;
   ret.w = inventoryImage->w;
   ret.h = inventoryImage->h;
   SDL_BlitSurface(inventoryImage, NULL, surface, &ret);

   /* Next, Blit all inventory objects image on surface */
   slots->draw(x,y,surface);
}

/**************************************************************
 *                        drawEquiped                         *
 **************************************************************/
void inventory::drawEquiped(int x, int y, SDL_Surface* surface)
{
   SDL_Rect ret;

   /* First, blit the inventory image with the surface at desired position */
   ret.x = x;
   ret.y = y;
   ret.w = inventoryImage->w;
   ret.h = inventoryImage->h;
   SDL_BlitSurface(equipedImage, NULL, surface, &ret);

   equippedSlots[INVENTORY_HEAD]->draw(x+105,y-10, surface);
   equippedSlots[INVENTORY_LEFT_HAND]->draw(x+155,y+99, surface);
   equippedSlots[INVENTORY_RIGHT_HAND]->draw(x+54,y+99, surface);
   equippedSlots[INVENTORY_LEFT_FINGER]->draw(x+196,y+156, surface);
   equippedSlots[INVENTORY_RIGHT_FINGER]->draw(x+33,y+156, surface);
   equippedSlots[INVENTORY_NECK]->draw(x+155,y+64, surface);
   equippedSlots[INVENTORY_FOOT]->draw(x+105,y+201, surface);
   equippedSlots[INVENTORY_BODY]->draw(x+95,y+87, surface);
}


