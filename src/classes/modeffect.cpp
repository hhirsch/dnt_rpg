/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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
using namespace std;

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
   applied = obj->applied;
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
   this->applied = false;
   this->time = time;
   this->periodicTime = periodicTime;
   this->cause.id = factorId;
   this->cause.type = factorType;
}

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
modEffect::modEffect(string saveTxt)
{
   fromSaveText(saveTxt);
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
modEffect::~modEffect()
{
}

/***********************************************************************
 *                             toSaveText                              *
 ***********************************************************************/
string modEffect::toSaveText()
{
   char buf[512];

   sprintf(buf, "%d,%hu,%hu,%d,%d,%s %s", 
         mod, init, lastApply, time, periodicTime, 
         cause.id.c_str(), cause.type.c_str());
   return(buf);
}

/***********************************************************************
 *                            fromSaveText                             *
 ***********************************************************************/
void modEffect::fromSaveText(string txt)
{
   char i[64], t[64];
   sscanf(txt.c_str(), "%d,%hu,%hu,%d,%d,%s %s", 
         &mod, &init, &lastApply, &time, &periodicTime, 
         i, t);

   cause.id = i;
   cause.type = t;
   applied = false;
}

/***********************************************************************
 *                          toReadableText                             *
 ***********************************************************************/
string modEffect::toReadableText(character* actor)
{
   char buf[128];

   skill* s;

   if(actor == NULL)
   {
      cerr << "modEffect::toReadableText: NULL actor!" << endl;
      return("");
   }

   /* Attribute and skills */
   if((cause.type == MOD_TYPE_ATT) || (cause.type == MOD_TYPE_SKILL)) 
   {
      s = actor->sk.getSkillByString(cause.id);

      if(s)
      {
         if(mod <= 0)
         {
            sprintf(buf, "%s: %d", s->definition->name.c_str(), mod);
         }
         else
         {
            sprintf(buf, "%s: +%d", s->definition->name.c_str(), mod);
         }
         return(buf);
      }
   }
   else if(cause.type == MOD_TYPE_THING)
   {
      string text = cause.id;
      if(cause.id == THING_ARMATURE_CLASS)
      {
         text = gettext("Armature Class");
      }
      else if(cause.id == THING_SIZE_MODIFIER)
      {
         text = gettext("Size Modifier");
      }
      else if(cause.id == THING_INITIATIVE_BONUS)
      {
         text = gettext("Initiative");
      }
      else if(cause.id == THING_MAX_LIFE_POINTS)
      {
         text = gettext("Life Points");
      }
      else if(cause.id == THING_DISPLACEMENT)
      {
         text = gettext("Displacement");
      }
      else if(cause.id == DNT_BS_FORTITUDE)
      {
         text = gettext("Fortitude");
      }
      else if(cause.id == DNT_BS_REFLEXES)
      {
         text = gettext("Reflexes");
      }
      else if( (cause.id == DNT_BS_I_AM_NOT_A_FOOL) ||
               (cause.id == DNT_BS_WILL) )
      {
         text = gettext("I Am Not A Fool");
      }
      
      if(mod <= 0)
      {
         sprintf(buf, "%s: %d", text.c_str(), mod);
      }
      else
      {
         sprintf(buf, "%s: +%d", text.c_str(), mod);
      }
      return(buf);
   }

   cerr << "Invalid: " << cause.type << " " << cause.id << endl;
   return(gettext("Invalid Modifier!"));
}

/***********************************************************************
 *                              getValue                               *
 ***********************************************************************/
int modEffect::getValue()
{
   return(mod);
}

/***********************************************************************
 *                                 apply                               *
 ***********************************************************************/
void modEffect::apply(character* actor)
{
   /* Apply */
   doApply(actor, mod);
   applied = true;
}

/***********************************************************************
 *                               unApply                               *
 ***********************************************************************/
void modEffect::unApply(character* actor)
{
   /* Apply with inverse value */
   doApply(actor, -mod);
   applied = false;
}

/***********************************************************************
 *                               doApply                               *
 ***********************************************************************/
void modEffect::doApply(character* actor, int value)
{
   int* v = NULL;

   if(actor != NULL)
   {
      v = actor->getFactorValuePointer(cause);
      if(v)
      {
         *v += value;
      }
   }
   else
   { 
      cerr << "modEffect::doApply: NULL character!" << endl;
   }
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
void modEffectList::setOwner(character* actor)
{
   owner = actor;
}

/***********************************************************************
 *                                insert                               *
 ***********************************************************************/
bool modEffectList::insert(modEffect* obj, bool applyEffect)
{
   if(obj)
   {
      /* Insert it on list */
      dntList::insert(obj);
      
      /* Set its initial timer to now */
      obj->init = SDL_GetTicks();

      /* Apply the effect, if owner is defined */
      if( (owner != NULL) && (applyEffect) )
      {
         obj->apply(owner);
      }
      return(true);
   }

   return(false);
}

/***********************************************************************
 *                              insertCopy                             *
 ***********************************************************************/
bool modEffectList::insertCopy(modEffect* obj, bool applyEffect)
{
   modEffect* eff;

   if(obj != NULL)
   {
      /* Create the copy */
      eff = new modEffect(obj);
      
      /* Insert on list */
      return(insert(eff, applyEffect));
   }

   return(false);
}

/***********************************************************************
 *                                  rest                               *
 ***********************************************************************/
void modEffectList::rest()
{
   int i;
   int curTotal = total;
   modEffect* eff = (modEffect*)first;
   modEffect* efaux;

   /* Search the list for expired modEffects */
   for(i = 0; i < curTotal; i++)
   {
      /* Getting the next to not lose the pointer */
      efaux = eff;
      eff = (modEffect*)eff->getNext();

      /* Verify if is timed */
      if(efaux->time != 0)
      {
         /* Must deapply it */
         if(owner != NULL)
         {
            efaux->unApply(owner);
         }

         /* remove it from the list */
         remove(efaux);
      }
   }
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
            efaux->lastApply = curTime;
         }
      }
   }
}


/***********************************************************************
 *                               unApply                               *
 ***********************************************************************/
void modEffectList::unApply()
{
   int i;

   modEffect* eff = (modEffect*)first;

   for(i=0; i < total; i++)
   {
      if(eff->applied)
      {
         eff->unApply(owner);
      }
      eff = (modEffect*)eff->getNext();
   }
}

/***********************************************************************
 *                                apply                                *
 ***********************************************************************/
void modEffectList::apply()
{
   int i;

   modEffect* eff = (modEffect*)first;

   for(i=0; i < total; i++)
   {
      if(!eff->applied)
      {
         eff->apply(owner);
      }
      eff = (modEffect*)eff->getNext();
   }
}

