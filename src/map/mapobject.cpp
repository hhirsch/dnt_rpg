/*************************************************************************
 * DccNiTghtmare is Public Domain - Do whatever you want with this code. *
 *************************************************************************/

#include "mapobject.h"
#include "../classes/weapon.h"

/*********************************************************************
 *                             Constructor                           *
 *********************************************************************/
lObject::lObject()
{
   first = NULL;
   total = 0;
}

/*********************************************************************
 *                              Destructor                           *
 *********************************************************************/
lObject::~lObject()
{
   object* obj = first;
   object* tmp;
   while(total > 0)
   {
      tmp = obj;
      obj = obj->next;
      removeObject(tmp);
   }
}

/*********************************************************************
 *                           insertMapObject                         *
 *********************************************************************/
object* lObject::insertObject(string arquivo, modelList& mdlList, weaponTypes& wTypes)
{
   object* novo = NULL;

   string::size_type loc = arquivo.find( ".dcc", 0 );
   if( loc != string::npos )
   {
      /* Is a map Object *.dcc */
      novo = (object*) new mapObject(arquivo, mdlList);
   }

   loc = arquivo.find( ".wcc", 0 );
   if( loc != string::npos )
   {
      /* Is a weapon Object *.wcc */
      novo = (object*) new weapon(arquivo, mdlList, wTypes);
   }

   if(novo == NULL)
   {
      printf("Error, cannot define the type of %s\n",arquivo.c_str());
      return(NULL);
   }


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
 *                         removeUnusedObjects                       *
 *********************************************************************/
void lObject::removeUnusedObjects()
{
   object* mdl;
   object* oth;

   if(first)
   {
      mdl = first->next;
      while( (first != NULL) && (mdl != first) )
      {
         oth = mdl;
         mdl = mdl->next;
         if(oth->getUsedFlag() <= 0)
         {
            removeObject(oth);
         }
      }
      if((first != NULL) && (first->getUsedFlag() <= 0))
      {
         removeObject(first);
      }
   }
}

/*********************************************************************
 *                           removeMapObject                         *
 *********************************************************************/
void lObject::removeObject(object* obj)
{
   obj->previous->next = obj->next;
   obj->next->previous = obj->previous;

   switch(obj->getType())
   {
      case OBJECT_TYPE_MAPOBJECT:
      {
         mapObject *o = (mapObject*)obj;
         delete(o);
         break;
      }
      case OBJECT_TYPE_WEAPON:
      {
         weapon *w = (weapon*)obj;
         delete(w);
         break;
      }
      default:
      {
         delete(obj);
         break;
      }
   }
   total--;
   if(total == 0)
   {
      first = NULL;
   }
}

/*********************************************************************
 *                             getObject                             *
 *********************************************************************/
object* lObject::getObject(string fileName)
{
   int aux;
   object* obj = first;
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
void mapObject::callDeadAnimation()
{
   //TODO
}

