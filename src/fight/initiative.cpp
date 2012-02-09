/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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

#include "initiative.h"
#include <SDL/SDL.h>
#include "../classes/dices.h"
using namespace std;

/***************************************************************
 *                       CONSTRUCTOR                           *
 ***************************************************************/
initiative::initiative()
{
   first = NULL;
   next = NULL;
   actual = NULL;
}

/***************************************************************
 *                        DESTRUCTOR                           *
 ***************************************************************/
initiative::~initiative()
{
   clear();
}

/***************************************************************
 *                     insertCharacter                        *
 ***************************************************************/
void initiative::insertCharacter(character* pers)
{
   char buffer[1024];
   string text;
   briefing brief;
   dice d20;

   initiativeStruct *cmp, *aux, *oth;
   srand(SDL_GetTicks());

   sprintf(buffer, gettext("%s rolls initiative: "),pers->name.c_str()); 
   text = buffer;

   /* Insertion on List, orded by initiativeValue (decrescent order) */
   if(first == NULL)
   {
      first = new(initiativeStruct);
      first->next = NULL;
      first->initiativeValue = d20.roll() + pers->attBonus(ATT_DEXTERITY) +
         pers->initiativeBonus;
      sprintf(buffer, "%d.",first->initiativeValue);
      text += buffer;
      brief.addText(text);
      first->dude = pers;
   }
   else
   {
      aux = new initiativeStruct;
      aux->initiativeValue = d20.roll() + pers->attBonus(ATT_DEXTERITY);
      sprintf(buffer,"%d.",aux->initiativeValue);
      text += buffer;
      brief.addText(text);
      aux->dude = pers;
      cmp = first; 
      oth = NULL;
      while( (cmp != NULL) && 
             (aux->initiativeValue < cmp->initiativeValue) )
      {
         oth = cmp;
         cmp = cmp->next;
      }

      if(oth == NULL) //it will be the first on list
      {
         aux->next = first;
         first = aux;
      }
      else //can be on any place of the list
      {
         aux->next = oth->next;
         oth->next = aux;
      }
   }

}

/***************************************************************
 *                           newRound                          *
 ***************************************************************/
void initiative::newRound()
{
   next = first;
}

/***************************************************************
 *                        nextCharacter                        *
 ***************************************************************/
character* initiative::nextCharacter()
{
   actual = next;
   if(next)
   {
      next = next->next;
   }
   if( actual )
   {
      return(actual->dude);
   }
   else
   {
      return(NULL);
   }
}

/***************************************************************
 *                      actualCharacter                        *
 ***************************************************************/
character* initiative::actualCharacter()
{
   if(actual)
   {
      return(actual->dude);
   }
   else
   {
      return(NULL);
   }
}

/***************************************************************
 *                            clear                            *
 ***************************************************************/
void initiative::clear()
{
   initiativeStruct* aux;
   while(first)
   {
      aux = first;
      first = aux->next;
      delete(aux);
   }
   actual = NULL;
   first = NULL;
   next = NULL;
}

