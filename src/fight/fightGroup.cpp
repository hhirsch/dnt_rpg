/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "fightGroup.h"
#include <SDL/SDL.h>
#include <stdlib.h>

/***************************************************************
 *                        CONSTRUCTOR                          *
 ***************************************************************/
fightGroup::fightGroup()
{
   int i;

   /* mark with NULL all pointers */
   for(i=0;i<FIGHT_MAX_CHARACTERS_PER_GROUP;i++)
   {
      characters[i] = NULL;
   }
   actualCharacters = 0;
}

/***************************************************************
 *                        DESTRUCTOR                           *
 ***************************************************************/
fightGroup::~fightGroup()
{
   actualCharacters = 0;
}

/***************************************************************
 *                      insertCharacter                        *
 ***************************************************************/
bool fightGroup::insertCharacter(personagem* pers)
{
   if(actualCharacters < FIGHT_MAX_CHARACTERS_PER_GROUP)
   {
      /* can insert character */
      characters[actualCharacters] = pers;
      actualCharacters++;
      return(true);
   }
   else
   {
      /* cannot insert character on this group */
      return(false);
   }
}

/***************************************************************
 *                       isCharacterIn                         *
 ***************************************************************/
bool fightGroup::isCharacterIn(personagem* pers)
{
   int i;
   for(i=0;i<actualCharacters;i++)
   {
      if(characters[i] == pers)
      {
          return(true);
      }
   }
   return(false);
}

/***************************************************************
 *                        getNPCEnemy                          *
 ***************************************************************/
personagem* fightGroup::getNPCEnemy(personagem* pers)
{
   /* FIXME For now, get a random enemy on list, better 
      make this in a more expert way */
   if(actualCharacters > 1)
   {
      srand48(SDL_GetTicks());
      int ch = ((lrand48() % actualCharacters));
      if(!characters[ch]->dead)
      {
         return(characters[ch]);
      }
      else
      {
         for(ch=0;ch<actualCharacters;ch++)
         {
            if(!characters[ch]->dead)
            {
               return(characters[ch]);
            }
         }
         return(NULL);
      }
   }
   else if(actualCharacters == 1)
   {
      if(!characters[0]->dead)
      {
         return(characters[0]);
      }
      else
      {
         return(NULL);
      }
   }
   else
   {
      return(NULL);
   }
}

/***************************************************************
 *                  anyoneIsAliveAndInRange                    *
 ***************************************************************/
bool fightGroup::anyoneIsAliveAndInRange()
{ 
   int i;
   for(i=0;i<actualCharacters;i++)
   {
      if(!characters[i]->dead)
      {
         return(true);
      }
   }
   return(false);
}
