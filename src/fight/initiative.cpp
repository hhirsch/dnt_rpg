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
   initiativeStruct* aux;
   while(first)
   {
      aux = first;
      first = aux->next;
      delete(first);
   }
   first = NULL;
   next = NULL;
}

/***************************************************************
 *                     insertCharacter                        *
 ***************************************************************/
void initiative::insertCharacter(personagem* pers, string& brief)
{
   char text[20];
   initiativeStruct *cmp, *aux, *oth;
   srand(SDL_GetTicks());

   brief = pers->nome + " " + language.FIGHT_ROLLS_INITIATIVE + " ";

   /* Insertion on List, orded by initiativeValue (decrescent order) */
   if(first == NULL)
   {
      first = new(initiativeStruct);
      first->next = NULL;
      first->initiativeValue = (rand() % DICE_D20 +1) +  
                               pers->attBonus(ATT_DEXTERY);
      sprintf(text,"%d.",first->initiativeValue);
      brief += text;
      first->character = pers;
   }
   else
   {
      aux = new initiativeStruct;
      aux->initiativeValue = (rand() % DICE_D20 +1) +
                             pers->attBonus(ATT_DEXTERY);
      sprintf(text,"%d.",aux->initiativeValue);
      brief += text;
      aux->character = pers;
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
personagem* initiative::nextCharacter()
{
   actual = next;
   if(next)
   {
      next = next->next;
   }
   if( actual )
   {
      return(actual->character);
   }
   else
   {
      return(NULL);
   }
}

/***************************************************************
 *                       nextCharacter                       *
 ***************************************************************/
personagem* initiative::actualCharacter()
{
   if(actual)
   {
      return(actual->character);
   }
   else
   {
      return(NULL);
   }
}
