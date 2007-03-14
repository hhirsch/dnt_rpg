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
   int x,y;
   inventoryImage = NULL;

   for(x=0; x < INVENTORY_SIZE_X; x++)
   {
      for(y=0; y < INVENTORY_SIZE_Y; y++)
      {
         spaces[x][y].obj = NULL;
         spaces[x][y].origX = x;
         spaces[x][y].origY = y;
      }
   }
   inventoryImage = IMG_Load("../data/texturas/inventory/inventory.png");
   if(!inventoryImage)
   {
      printf("Can't Load Inventory Image!\n");
   }

   /* Nullify equiped pointers */
   for(x = 0; x < INVENTORY_TOTAL_PLACES; x++)
   {
      equippedObject[x] = NULL;
   }
}

/**************************************************************
 *                           Destructor                       *
 **************************************************************/
inventory::~inventory()
{
   int x,y;
   for(x=0; x < INVENTORY_SIZE_X; x++)
   {
      for(y=0; y < INVENTORY_SIZE_Y; y++)
      {
         if( (spaces[x][y].obj != NULL) &&
             (spaces[x][y].origX == x) &&
             (spaces[x][y].origY == y))
         {
            delete(spaces[x][y].obj);
         }
         spaces[x][y].obj = NULL;
      }
   }

   if(inventoryImage)
   {
      SDL_FreeSurface(inventoryImage);
   }
}

/**************************************************************
 *                           addObject                        *
 **************************************************************/
bool inventory::addObject(object* obj, int x, int y)
{
   if(!obj)
   {
      return(false);
   }
   int sizeX, sizeY;
   int j,k;
   obj->getInventorySize(sizeX, sizeY);

   object* nObj;

   if(canAdd(obj, x, y))
   {
      /* Alloc Intern Object */
      nObj = new object(obj);
      
      /* Occup all needed spaces */
      for(j=x; j<x+sizeX; j++)
      {
         for(k=y; k<y+sizeY; k++)
         {
            spaces[j][k].obj = nObj;
            spaces[j][k].origX = x;
            spaces[j][k].origY = y;
         }
      }
      return(true);
   }
   return(false); 
}

/**************************************************************
 *                          equipObject                       *
 **************************************************************/
bool inventory::equipObject(object* obj, int where)
{
   if( (obj) && (where > 0) && (where < INVENTORY_TOTAL_PLACES) )
   {
      if(equippedObject[where] == NULL)
      {
         //TODO verify if object Use Type is compatible with the place
         equippedObject[where] = obj;
      }
   }
   return(false);
}

/**************************************************************
 *                           addObject                        *
 **************************************************************/
bool inventory::addObject(object* obj)
{
   int x,y;

   if(!obj)
   {
      return(false);
   }

   for(x=0; x < INVENTORY_SIZE_X; x++)
   {
      for(y=0; y < INVENTORY_SIZE_Y; y++)
      {
         if(addObject(obj, x, y))
         {
            return(true);
         }
      }
   }
   return(false);
}

/**************************************************************
 *                            canAdd                          *
 **************************************************************/
bool inventory::canAdd(object* obj, int x, int y)
{
   int sizeX, sizeY;
   int j,k;
   obj->getInventorySize(sizeX, sizeY);
   if( (x<0) || (y<0) || (x+sizeX > INVENTORY_SIZE_X) ||
       (y+sizeY > INVENTORY_SIZE_Y) || (obj == NULL))
   {
      return(false);
   }

   for(j=x; j<x+sizeX; j++)
   {
      for(k=y; k<y+sizeY; k++)
      {
         if(spaces[j][k].obj != NULL)
         {
            return(false);
         }
      }
   }

   return(true);
}

/**************************************************************
 *                         getFromPlace                       *
 **************************************************************/
object* inventory::getFromPlace(int where)
{
   if( (where > 0) && (where < INVENTORY_TOTAL_PLACES) )
   {
      return(equippedObject[where]);
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
      return(spaces[x][y].obj);
   }
   return(NULL);
}

/**************************************************************
 *                       removeFromPlace                      *
 **************************************************************/
void inventory::removeFromPlace(int where)
{
   if( (where > 0) && (where < INVENTORY_TOTAL_PLACES) )
   {
      equippedObject[where] = NULL;
   }
}

/**************************************************************
 *                     removeFromInventory                    *
 **************************************************************/
void inventory::removeFromInventory(object* obj)
{
   int x,y;
   
   if(obj == NULL)
   {
      return;
   }
   
   for(x=0; x < INVENTORY_SIZE_X; x++)
   {
      for(y=0; y < INVENTORY_SIZE_Y; y++)
      {
         if(spaces[x][y].obj == obj)
         {
            if( (spaces[x][y].origX == x) && 
                (spaces[x][y].origY == y))
            {
               /* Free use of the Object */
               delete(spaces[x][y].obj);
            }
            spaces[x][y].obj = NULL;
            spaces[x][y].origX = x;
            spaces[x][y].origY = y;
         }
      }
   }

   print();
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
   object* obj;
   int j,k, sizeX, sizeY, oX, oY;

   if( (x >= 0) && (y >= 0) && (x < INVENTORY_SIZE_X) &&
       (y < INVENTORY_SIZE_Y))
   {
      obj = spaces[x][y].obj;
      if(obj)
      {
         obj->getInventorySize(sizeX, sizeY);
         oX = spaces[x][y].origX;
         oY = spaces[x][y].origY;
         /* Empty all occupied spaces */
         for(j= oX; j < oX + sizeX; j++)
         {
            for(k = oY; k < oY + sizeY; k++)
            {
               spaces[j][k].obj = NULL;
               spaces[j][k].origX = j;
               spaces[j][k].origY = k;
               printf("Sumido com: %d,%d\n",j,k);
            }
         }
         /* Free obj from use here */
         delete(obj);
      }
   }

   print();
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
   int j,k;
   for(j=0; j < INVENTORY_SIZE_X; j++)
   {
      for(k=0; k < INVENTORY_SIZE_Y; k++)
      {
         if( (spaces[j][k].obj != NULL) && (spaces[j][k].origX == j) &&
             (spaces[j][k].origY == k))
         {
            spaces[j][k].obj->draw2D((x+4 + (j*19)), (y+13 + (k*19)), surface);
         }
      }
   }
}

