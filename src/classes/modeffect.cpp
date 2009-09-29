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

#include "modeffect.h"

#include "../engine/character.h"

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                             modEffect                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
modEffect::modEffect(modEffect* obj)
{
   /* Copy values */
   mod = obj->mod;
   init = obj->init;
   time = obj->time;
   lastApply = obj->lastApply;
   periodicTime = obj->periodicTime;
   cause.type = obj->cause.type;
   cause.id = obj->cause.id;

   /* Nullify pointers */
   next = NULL;
   previous = NULL;
}

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
modEffect::modEffect(int mod, int time, int periodicTime, 
            string factorId, string factorType)
{
   /* set values */
   this->mod = mod;
   this->init = SDL_GetTicks();
   this->lastApply = 0;
   this->time = time;
   this->periodicTime = periodicTime;
   this->cause.id = factorId;
   this->cause.type = factorType;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
modEffect::~modEffect()
{
}

/***********************************************************************
 *                                 apply                               *
 ***********************************************************************/
void modEffect::apply(void* actor)
{
   /* TODO */
}

/***********************************************************************
 *                               unApply                               *
 ***********************************************************************/
void modEffect::unApply(void* actor)
{
   /* TODO */
}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                           modEffectList                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
modEffectList::modEffectList()
{
   owner = NULL;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
modEffectList::~modEffectList()
{
   clearList();
}

/***********************************************************************
 *                             freeElement                             *
 ***********************************************************************/
void modEffectList::freeElement(dntListElement* obj)
{
   modEffect* effect = (modEffect*)obj;
   delete(effect);
}

/***********************************************************************
 *                               setOwner                              *
 ***********************************************************************/
void modEffectList::setOwner(void* actor)
{
   owner = actor;
}

/***********************************************************************
 *                              insertCopy                             *
 ***********************************************************************/
bool modEffectList::insertCopy(modEffect* obj)
{
   modEffect* eff;

   if(obj != NULL)
   {
      /* Create the copy */
      eff = new modEffect(obj);
      
      /* Set its initial timer to now */
      eff->init = SDL_GetTicks();

      /* Insert on list */
      insert(eff);

      /* Apply the effect, if owner is defined */
      if(owner != NULL)
      {
         eff->apply(owner);
      }

      return(true);
   }

   return(false);
}

/***********************************************************************
 *                                 doStep                              *
 ***********************************************************************/
void modEffectList::doStep()
{
   int i;
   int curTotal = total;
   modEffect* eff = (modEffect*)first;
   modEffect* efaux;
   Uint16 curTime = SDL_GetTicks();

   /* Search the list for expired modEffects */
   for(i = 0; i < curTotal; i++)
   {
      /* Getting the next to not lose the pointer */
      efaux = eff;
      eff = (modEffect*)eff->getNext();

      /* Verify if expired */
      if( (efaux->time != 0) && (curTime - efaux->init >= efaux->time))
      {
         /* Must deapply it */
         if(owner != NULL)
         {
            efaux->unApply(owner);
         }

         /* remove it from the list */
         remove(efaux);
      }
      /* Verify if is time to apply again */
      else if( (efaux->periodicTime != 0) && 
            (curTime - efaux->lastApply >= efaux->periodicTime) )
      {
         /* Apply the effect, if owner is defined */
         if(owner != NULL)
         {
            eff->apply(owner);
         }
      }
   }
}


