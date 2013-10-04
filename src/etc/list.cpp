/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "list.h"
#include <stdlib.h>


///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                            dntListElement                             //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
dntListElement::dntListElement()
{
   next = NULL;
   previous = NULL;
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
dntListElement::~dntListElement()
{
}

/***********************************************************************
 *                              getNext                                *
 ***********************************************************************/
dntListElement* dntListElement::getNext()
{
   return(next);
}

/***********************************************************************
 *                            getPrevious                              *
 ***********************************************************************/
dntListElement* dntListElement::getPrevious()
{
   return(previous);
}

/***********************************************************************
 *                                setNext                              *
 ***********************************************************************/
void dntListElement::setNext(dntListElement* obj)
{
   next = obj;
}

/***********************************************************************
 *                              setPrevious                            *
 ***********************************************************************/
void dntListElement::setPrevious(dntListElement* obj)
{
   previous = obj;
}

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                               dntList                                 //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
dntList::dntList(int t)
{
   first = NULL;
   total = 0;
   listType = t;
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
dntList::~dntList()
{
}

/***********************************************************************
 *                             clearList                               *
 ***********************************************************************/
void dntList::clearList()
{
   int i;
   dntListElement* e = first;
   dntListElement* paux;

   /* Delete all elements */
   for(i=0; i<total; i++)
   {
      paux = e;
      e = e->getNext();
      freeElement(paux);
   }

   first = NULL;
   total = 0;
}

/***********************************************************************
 *                               insert                                *
 ***********************************************************************/
bool dntList::insert(dntListElement* obj)
{
   if(obj == NULL)
   {
      return(false);
   }

   if(listType == DNT_LIST_TYPE_ADD_AT_BEGIN)
   {
      return(insertAtBegin(obj));
   }
   /* Else */
   return(insertAtEnd(obj));
}

/***********************************************************************
 *                         insertAtBegin                               *
 ***********************************************************************/
bool dntList::insertAtBegin(dntListElement* obj)
{
   if(total == 0)
   {
      /* It's the only element */
      obj->setNext(obj);
      obj->setPrevious(obj);
   }
   else
   {
      /* Insert it before the first */
      obj->setNext(first);
      obj->setPrevious(first->getPrevious());
      obj->getNext()->setPrevious(obj);
      obj->getPrevious()->setNext(obj);
   }

   /* Set as the first */
   first = obj;
   total++;

   return(true);
}

/***********************************************************************
 *                               insert                                *
 ***********************************************************************/
bool dntList::insertAtEnd(dntListElement* obj)
{
   if(total == 0)
   {
      /* It's the only element */
      obj->setNext(obj);
      obj->setPrevious(obj);
      first = obj;
   }
   else
   {
      /* Insert it before the first */
      obj->setNext(first);
      obj->setPrevious(first->getPrevious());
      obj->getNext()->setPrevious(obj);
      obj->getPrevious()->setNext(obj);
   }

   total++;

   return(true);
}


/***********************************************************************
 *                        removeWithoutDelete                          *
 ***********************************************************************/
bool dntList::removeWithoutDelete(dntListElement* obj)
{
   if(obj == NULL)
   {
      return(false);
   }

   /* Update the pointers */
   obj->getNext()->setPrevious(obj->getPrevious());
   obj->getPrevious()->setNext(obj->getNext());
   
   if(first == obj)
   {
      first = first->getNext();
   } 
   
   /* And dec */
   total--;
   if(total == 0)
   {
      first = NULL;
   }

   return(true);
}

/***********************************************************************
 *                               remove                                *
 ***********************************************************************/
bool dntList::remove(dntListElement* obj)
{
   if(removeWithoutDelete(obj))
   {
      /* Really remove it */
      freeElement(obj);
      return(true);
   }

   return(false);
}

/***********************************************************************
 *                               getFirst                              *
 ***********************************************************************/
dntListElement* dntList::getFirst()
{
   return(first);
}

/***********************************************************************
 *                                 get                                 *
 ***********************************************************************/
dntListElement* dntList::get(int i)
{
   int j=0;
   dntListElement* e = first;

   if( (i < 0) && (i >= total) )
   {
      return(NULL);
   }

   while(j < i)
   {
      e = e->getNext();
      j++;
   }

   return(e);
}

/***********************************************************************
 *                               getTotal                              *
 ***********************************************************************/
int dntList::getTotal()
{
   return(total);
}

