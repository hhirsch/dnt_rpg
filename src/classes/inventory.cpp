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
   int i;
   inventoryImage = NULL;

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
 *                           addObject                        *
 **************************************************************/
bool inventory::addObject(object* obj, int x, int y, int curInv)
{
   return(slots[curInv]->addObject(obj, x, y));
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
   int inv = 0;
   while( (!slots[inv]->addObject(obj) && (inv < INVENTORY_PER_CHARACTER)))
   {
      inv++;
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
   for(i=0; i<INVENTORY_PER_CHARACTER; i++)
   {
      slots[i]->removeObject(obj);
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


