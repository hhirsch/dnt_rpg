/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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
particleList::particleList(): dntList(DNT_LIST_TYPE_ADD_AT_END)
{
}

/***************************************************************************
 *                              Destructor                                 *
 ***************************************************************************/
particleList::~particleList()
{
   clearList();
}

/***************************************************************************
 *                            removeNonPCs                                 *
 ***************************************************************************/
void particleList::removeNonPCs()
{
   int i, t = total;
   particleSystem* part = (particleSystem*)first;
   particleSystem* aux;

   for(i = 0; i < t; i++)
   {
      aux = part;
      part = (particleSystem*)part->next;
      if( (aux->followCharacter == NULL) || 
          (aux->followType != DNT_PARTICLE_SYSTEM_FOLLOW_PC) )
      {
         removeSystem(aux);
      }
   }
}

/***************************************************************************
 *                              removePCs                                  *
 ***************************************************************************/
void particleList::removePCs()
{
   int i, t = total;
   particleSystem* part = (particleSystem*)first;
   particleSystem* aux;

   for(i = 0; i < t; i++)
   {
      aux = part;
      part = (particleSystem*)part->next;
      if( (aux->followCharacter != NULL) && 
          (aux->followType == DNT_PARTICLE_SYSTEM_FOLLOW_PC))
      {
         removeSystem(aux);
      }
   }
}

/***************************************************************************
 *                              addSystem                                  *
 ***************************************************************************/
void particleList::addSystem(particleSystem* part)
{
   insert(part);
}


/***************************************************************************
 *                            removeSystem                                 *
 ***************************************************************************/
void particleList::removeSystem(particleSystem* part)
{
   remove(part);
}

/***************************************************************************
 *                             freeElement                                 *
 ***************************************************************************/
void particleList::freeElement(dntListElement* obj)
{
   particleSystem* part = (particleSystem*)obj;
   delete(part);
}

