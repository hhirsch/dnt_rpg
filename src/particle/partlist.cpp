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

#include "partlist.h"

#include "part1.h"
#include "part2.h"
#include "part3.h"
#include "part4.h"
#include "part5.h"
#include "part6.h"
#include "part7.h"
#include "grass.h"
#include "meteor.h"

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
      if(!aux->followPC)
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
      if(aux->followPC)
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

   /* let's delete by type */
   switch(part->type)
   {
      case DNT_PARTICLE_TYPE_NONE:
      default:
      {
         delete(part);
      }
      break;
      case DNT_PARTICLE_TYPE_WATERFALL:
      {
         part1* p1 = (part1*)part;
         delete(p1);
      }
      break;
      case DNT_PARTICLE_TYPE_FIRE:
      {
         part2* p2 = (part2*)part;
         delete(p2);
      }
      break;
      case DNT_PARTICLE_TYPE_WATER_SURFACE:
      {
         part3* p3 = (part3*)part;
         delete(p3);
      }
      break;
      case DNT_PARTICLE_TYPE_SMOKE:
      {
         part4* p4 = (part4*)part;
         delete(p4);
      }
      break;
      case DNT_PARTICLE_TYPE_BLOOD:
      {
         part5* p5 = (part5*)part;
         delete(p5);
      }
      break;
      case DNT_PARTICLE_TYPE_LIGHTNING:
      {
         part6* p6 = (part6*)part;
         delete(p6);
      }
      break;
      case DNT_PARTICLE_TYPE_SNOW:
      {
         part7* p7 = (part7*)part;
         delete(p7);
      }
      break;
      case DNT_PARTICLE_TYPE_GRASS:
      {
         grass* gr = (grass*)part;
         delete(gr);
      }
      break;
      case DNT_PARTICLE_TYPE_METEOR:
      {
         meteor* m = (meteor*)part;
         delete(m);
      }
      break;
   }
}

