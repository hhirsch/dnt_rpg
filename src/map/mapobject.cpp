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
object* lObject::insertObject(string arquivo, modelList& mdlList, 
                              weaponTypes& wTypes)
{
   object* novo = NULL;

   string::size_type loc = arquivo.find( ".dcc", 0 );
   if( loc != string::npos )
   {
      /* Is a map Object *.dcc */
      novo = (object*) new mapObject(arquivo, mdlList);
   }
   else
   {
      loc = arquivo.find( ".wcc", 0 );
      if( loc != string::npos )
      {
         /* Is a weapon Object *.wcc */
         novo = (object*) new weapon(arquivo, mdlList, wTypes);
      }
   }

   /* verify if created the pointer */
   if(novo == NULL)
   {
      printf("Error, cannot define the type of %s\n",arquivo.c_str());
      return(NULL);
   }

   /* Insert the pointer at list */
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

   int numObjs = total, i;

   /* Removel all models with usedFlag <= 0 */
   mdl = first;
   for(i = 0; i < numObjs; i++)
   {
      if(mdl->getUsedFlag() <= 0)
      {
         oth = mdl;
         mdl = mdl->next;
         removeObject(oth);
      }
      else
      {
         mdl = mdl->next;
      }
   }
}

/*********************************************************************
 *                           removeMapObject                         *
 *********************************************************************/
void lObject::removeObject(object* obj)
{
   /* Redefine pointers */
   obj->previous->next = obj->next;
   obj->next->previous = obj->previous;

   if(obj == first)
   {
      first = obj->next;
   }

   /* Delete object memory */
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

   /* Decrement the counter */
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

