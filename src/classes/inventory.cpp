/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "inventory.h"


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
 *                           addObject                        *
 **************************************************************/
bool inventory::addObject(object* obj)
{
   int x,y;

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
       (y+sizeY > INVENTORY_SIZE_Y))
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
}


/**************************************************************
 *                     removeFromInventory                    *
 **************************************************************/
void inventory::removeFromInventory(int x, int y)
{
   object* obj;
   int j,k, sizeX, sizeY;
   if( (x >= 0) && (y >=0) && (x < INVENTORY_SIZE_X) &&
       (y < INVENTORY_SIZE_Y))
   {
      obj = spaces[x][y].obj;
      if(obj)
      {
         obj->getInventorySize(sizeX, sizeY);
         /* Empty all occupied spaces */
         for(j= spaces[x][y].origX; j < spaces[x][y].origX + sizeX; j++)
         {
            for(k = spaces[x][y].origY; k < spaces[x][y].origY+ sizeY; k++)
            {
               spaces[j][k].obj = NULL;
               spaces[j][k].origX = j;
               spaces[j][k].origY = k;
            }
         }
         /* Free obj from use here */
         delete(obj);
      }
   }
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
            spaces[j][k].obj->draw2D((x+4 + (j*20)), (y+13 + (k*20)), surface);
         }
      }
   }
}

