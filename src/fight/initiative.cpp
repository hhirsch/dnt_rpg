/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "initiative.h"
#include <SDL/SDL.h>
#include "../classes/dices.h"

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
      first->initiativeValue = d20.roll() + pers->attBonus(ATT_DEXTERITY);
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

