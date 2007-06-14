/*************************************************************************
 * DccNiTghtmare is Public Domain - Do whatever you want with this code. *
 *************************************************************************/

#include "mapobject.h"
#include "../etc/glm.h"

/*********************************************************************
 *                             Constructor                           *
 *********************************************************************/
lMapObject::lMapObject()
{
   first = NULL;
   total = 0;
}

/*********************************************************************
 *                              Destructor                           *
 *********************************************************************/
lMapObject::~lMapObject()
{
   mapObject* obj = first;
   mapObject* tmp;
   while(total > 0)
   {
      tmp = obj;
      obj = obj->next;
      delete(tmp);
      total--;
   }
}

/*********************************************************************
 *                           insertMapObject                         *
 *********************************************************************/
mapObject* lMapObject::insertMapObject(string arquivo, modelList& mdlList)
{
   mapObject* novo;
   novo = new mapObject(arquivo, mdlList);
   if(first == NULL)
   {
      first = novo;
      novo->next = first;
      novo->previous = first;
   }
   else
   {
      novo->next = first;
      novo->previous = first->previous;
      novo->previous->next = novo;
      novo->next->previous = novo;
      first = novo;
   }
   
   total++;
   return(novo);
}

/*********************************************************************
 *                           removeMapObject                         *
 *********************************************************************/
void lMapObject::removeMapObject(mapObject* obj)
{
   obj->previous->next = obj->next;
   obj->next->previous = obj->previous;
   delete(obj);
   total--;
   if(total == 0)
   {
      first = NULL;
   }
}

/*********************************************************************
 *                           getMapObject                            *
 *********************************************************************/
mapObject* lMapObject::getMapObject(string fileName)
{
   int aux;
   mapObject* obj = first;
   for(aux=0;aux<total;aux++)
   {
      if(obj->getFileName() == fileName)
      {
         return(obj);
      }
      obj = obj->next;
   }
   return(NULL);
}

/*********************************************************************
 *                              canGet                               *
 *********************************************************************/
bool mapObject::canGet()
{
   int x, y;
   getInventorySize(x,y);
   return( (x != 0) && (y != 0));
}

/*********************************************************************
 *                              canGet                               *
 *********************************************************************/
void mapObject::callDeadAnimation()
{
   //TODO
}

