/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/
#include "itemslot.h"

/****************************************************************
 *                       spaceConstructor                       *
 ****************************************************************/
space::space()
{
   obj = NULL;
   origX = 0;
   origY = 0;
}

/****************************************************************
 *                         Constructor                          *
 ****************************************************************/
itemSlot::itemSlot(int xSize, int ySize)
{
   int i;
   /* Set initial values */
   sizeX = xSize;
   sizeY = ySize;
   oneItemPerTime = false;
   totalItems = 0;

   /* Alloc Spaces */
   spaces = new space*[sizeX];
   for(i=0; i < sizeX; i++)
   {
      spaces[i] = new space[sizeY];
   }
}

/****************************************************************
 *                          Destructor                          *
 ****************************************************************/
itemSlot::~itemSlot()
{
   int i, x, y;

   /* Mark as Unused Objects */
   for(x=0; x < sizeX; x++)
   {
      for(y=0; y < sizeY; y++)
      {
         if( (spaces[x][y].obj != NULL) &&
             (spaces[x][y].origX == x) &&
             (spaces[x][y].origY == y))
         {
            /* Delete the object */
            delete(spaces[x][y].obj);
         }
         spaces[x][y].obj = NULL;
      }
   }

   /* Free Spaces */
   for(i=0; i < sizeX; i++)
   {
      delete[] spaces[i];
   }
   delete[] spaces;
}

/**************************************************************
 *                           addObject                        *
 **************************************************************/
bool itemSlot::addObject(object* obj, int x, int y)
{
   if(!obj)
   {
      return(false);
   }
   int sX, sY;
   int j,k;
   obj->getInventorySize(sX, sY);

   if(canAdd(obj, x, y))
   {
      /* Occup all needed spaces */
      for(j=x; j<x+sX; j++)
      {
         for(k=y; k<y+sY; k++)
         {
            spaces[j][k].obj = obj;
            spaces[j][k].origX = x;
            spaces[j][k].origY = y;
         }
      }
      /* Change Object Position (to avoid being deleted when back 
       * to the map where got it) */
      obj->xPosition = -x;
      obj->yPosition = -y;
      obj->zPosition = -1;

      totalItems++;
      return(true);
   }
   return(false); 
}

/**************************************************************
 *                           addObject                        *
 **************************************************************/
bool itemSlot::addObject(object* obj)
{
   int x,y;

   if(!obj)
   {
      return(false);
   }

   for(x=0; x < sizeX; x++)
   {
      for(y=0; y < sizeY; y++)
      {
         if(addObject(obj, x, y))
         {
            totalItems++;
            return(true);
         }
      }
   }
   return(false);
}

/**************************************************************
 *                            canAdd                          *
 **************************************************************/
bool itemSlot::canAdd(object* obj, int x, int y)
{
   int sX, sY;
   int j,k;
   obj->getInventorySize(sX, sY);
   if( (x<0) || (y<0) || (x+sX > sizeX) ||
       (y+sY > sizeY) || (obj == NULL))
   {
      return(false);
   }

   if( (oneItemPerTime) && (spaces[0][0].obj != NULL))
   {
      return(false);
   }

   for(j=x; j<x+sX; j++)
   {
      for(k=y; k<y+sY; k++)
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
object* itemSlot::getFromPosition(int x, int y)
{
   if( (x >= 0) && (y >=0) && (x < sizeX) && (y < sizeY))
   {
      return(spaces[x][y].obj);
   }
   return(NULL);
}

/**************************************************************
 *                         removeObject                       *
 **************************************************************/
bool itemSlot::removeObject(object* obj)
{
   int x,y;
   
   if(obj == NULL)
   {
      return(true);
   }
  
   /* Search for the object */
   for(x=0; x < sizeX; x++)
   {
      for(y=0; y < sizeY; y++)
      {
         /* Found it */
         if(spaces[x][y].obj == obj)
         {
            /* Remove only it from the inventory */
            removeObject(spaces[x][y].origX, spaces[x][y].origY);
            totalItems--;
            return(true);
         }
      }
   }

   /* Not Found */
   return(false);
}

/**************************************************************
 *                         removeObject                       *
 **************************************************************/
void itemSlot::removeObject(int x, int y)
{
   object* obj;
   int j,k, sX, sY, oX, oY;

   if( (x >= 0) && (y >= 0) && (x < sizeX) && (y < sizeY))
   {
      obj = spaces[x][y].obj;
      if(obj)
      {
         obj->getInventorySize(sX, sY);
         oX = spaces[x][y].origX;
         oY = spaces[x][y].origY;
         /* Empty all occupied spaces */
         for(j= oX; j < oX + sX; j++)
         {
            for(k = oY; k < oY + sY; k++)
            {
               spaces[j][k].obj = NULL;
               spaces[j][k].origX = j;
               spaces[j][k].origY = k;
            }
         }
         /* Free obj from use here */
         totalItems--;
      }
   }
}

/**************************************************************
 *                             draw                           *
 **************************************************************/
void itemSlot::draw(int x, int y, SDL_Surface* surface)
{
   /* Blit all inventory objects image on surface */
   int j,k;
   for(j=0; j < sizeX; j++)
   {
      for(k=0; k < sizeY; k++)
      {
         if( (spaces[j][k].obj != NULL) && (spaces[j][k].origX == j) &&
             (spaces[j][k].origY == k))
         {
            spaces[j][k].obj->draw2D((x + (j*19)), (y + (k*19)), surface);
         }
      }
   }
}

/**************************************************************
 *                     setAsOneItemPerTime                    *
 **************************************************************/
void itemSlot::setAsOneItemPerTime()
{
   oneItemPerTime = true;
}

/**************************************************************
 *                         getFirstItem                       *
 **************************************************************/
object* itemSlot::getFirstItem(int& x, int& y)
{
   x = 0;
   y = -1;
   return(getNextItem(x,y));
}

/**************************************************************
 *                          getNextItem                       *
 **************************************************************/
object* itemSlot::getNextItem(int& x, int& y)
{
   int j,k;
 
   k = y+1; /* To get next element from previous */
   for(j = x; j < sizeX; j++)
   {
      for(;k < sizeY; k++)
      {
         if( (spaces[j][k].obj != NULL) && 
             (spaces[j][k].origX == j) && (spaces[j][k].origY == k) )
         {
            x = j;
            y = k;
            return(spaces[j][k].obj);
         }
      }
      k = 0; /* To continue the search at first y position */
   }
   x = -1;
   y = -2;
   return(NULL);
}

/**************************************************************
 *                       getItemByFileName                    *
 **************************************************************/
object* itemSlot::getItemByFileName(string fileName)
{
   int j,k;
   for(j=0; j < sizeX; j++)
   {
      for(k=0; k < sizeY; k++)
      {
         if( (spaces[j][k].obj != NULL) &&
             (spaces[j][k].obj->getFileName() == fileName) )
         {
            /* Found One! */
            return(spaces[j][k].obj);
         }
      }
   }
   return(NULL);
}

/**************************************************************
 *                         getTotalItems                      *
 **************************************************************/
int itemSlot::getTotalItems()
{
   return(totalItems);
}

