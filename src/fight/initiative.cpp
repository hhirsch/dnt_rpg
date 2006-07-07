#include "initiative.h"
#include <SDL/SDL.h>
#include "../classes/dices.h"

/***************************************************************
 *                       CONSTRUCTOR                           *
 ***************************************************************/
initiative::initiative()
{
   first = NULL;
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
   actual = NULL;
}

/***************************************************************
 *                     includeCharacter                        *
 ***************************************************************/
void initiative::includeCharacter(personagem* pers)
{
   //TODO Apply DEXTERY BONUS

   initiativeStruct *cmp, *aux, *oth;
   srand48(SDL_GetTicks());

   /* Insertion on List, orded by initiativeValue (crescent order) */
   if(first == NULL)
   {
      first = new(initiativeStruct);
      first->next = NULL;
      first->initiativeValue = (lrand48() % DICE_D20 +1);
      actual = first;
   }
   else
   {
      aux = new initiativeStruct;
      aux->initiativeValue = (lrand48() % DICE_D20 +1);
      cmp = first; 
      oth = NULL;
      while( (cmp != NULL) && 
             (aux->initiativeValue >= cmp->initiativeValue) )
      {
         oth = cmp;
         cmp = cmp->next;
      }

      if(oth == NULL) //it will be the first on list
      {
         aux->next = first;
         aux = first;
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
   actual = first;
}

/***************************************************************
 *                        nextCharacter                        *
 ***************************************************************/
personagem* initiative::nextCharacter()
{
   initiativeStruct* ret = actual;
   if(actual)
   {
      actual = actual->next;
   }
   if(ret)
   {
      return(ret->character);
   }
   else
   {
      return(NULL);
   }
}
