/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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

#include "partlist.h"


/***************************************************************************
 *                             Constructor                                 *
 ***************************************************************************/
particleList::particleList()
{
   total = 0;
   first = NULL;
}

/***************************************************************************
 *                              Destructor                                 *
 ***************************************************************************/
particleList::~particleList()
{
   int i;
   particleSystem* part = first;
   /* Remove the remaining systens. Usually, no systems are at the
    * destructor time, cause the remove here can make a memory leack. */
   for(i = 0; i < total; i++)
   {
      first = part;
      part = part->next;
      delete(first);
   }
}


/***************************************************************************
 *                               getFirst                                  *
 ***************************************************************************/
particleSystem* particleList::getFirst()
{
   return(first);
}


/***************************************************************************
 *                              getTotal                                   *
 ***************************************************************************/
int particleList::getTotal()
{
   return(total);
}

/***************************************************************************
 *                              addSystem                                  *
 ***************************************************************************/
void particleList::addSystem(particleSystem* part)
{
   if(first == NULL)
   {
      part->next = part;
      part->previous = part;
   }
   else
   {
      part->next = first;
      part->previous = first->previous;
      part->next->previous = part;
      part->previous->next = part;
   }
   first = part;
   total++;
}


/***************************************************************************
 *                            removeSystem                                 *
 ***************************************************************************/
void particleList::removeSystem(particleSystem* part)
{
   if(part == first)
   {
      first = part->next;
   }
   part->previous->next = part->next;
   part->next->previous = part->previous;
   total--;
   if(total == 0)
   {
      first = NULL;
   }
}

