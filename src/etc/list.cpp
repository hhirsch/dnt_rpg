/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
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
dntList::dntList()
{
   first = NULL;
   total = 0;
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
dntList::~dntList()
{
   int i;
   dntListElement* e = first;

   /* Delete all elements */
   for(i=0; i<total; i++)
   {
      e = e->getNext();
      freeElement(e->getPrevious());
   }
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

   /* Be the first */
   first = obj;
   total++;

   return(true);
}

/***********************************************************************
 *                               remove                                *
 ***********************************************************************/
bool dntList::remove(dntListElement* obj)
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

   /* Really remove it */
   freeElement(obj);

   /* And dec */
   total--;
   if(total == 0)
   {
      first = NULL;
   }

   return(true);
}

/***********************************************************************
 *                               getTotal                              *
 ***********************************************************************/
int dntList::getTotal()
{
   return(total);
}

