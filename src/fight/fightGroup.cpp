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
 *                         Empty                               *
 ***************************************************************/
void fightGroup::empty()
{
   int i;

   /* mark with NULL all pointers */
   for(i=0;i<FIGHT_MAX_CHARACTERS_PER_GROUP;i++)
   {
      if(characters[i] != NULL)
      {
         /* Remove the enemy status and put as neutral */
         characters[i]->psychoState = PSYCHO_NEUTRAL;
      }
      characters[i] = NULL;
   }
   actualCharacters = 0;
}

/***************************************************************
 *                        DESTRUCTOR                           *
 ***************************************************************/
fightGroup::~fightGroup()
{
   empty();
   actualCharacters = 0;
}

/***************************************************************
 *                      insertCharacter                        *
 ***************************************************************/
bool fightGroup::insertCharacter(character* pers)
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
bool fightGroup::isCharacterIn(character* pers)
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
character* fightGroup::getNPCEnemy(character* pers)
{
   /* FIXME For now, get a random enemy on list, better 
      make this in a more expert way */
   if(actualCharacters > 1)
   {
      srand(SDL_GetTicks());
      int ch = ((rand() % actualCharacters));
      if( (characters[ch]->isAlive()) && (characters[ch] != pers) )
      {
         return(characters[ch]);
      }
      else
      {
         for(ch=0;ch<actualCharacters;ch++)
         {
            if( (characters[ch]->isAlive()) && (characters[ch] != pers))
            {
               return(characters[ch]);
            }
         }
         return(NULL);
      }
   }
   else if(actualCharacters == 1)
   {
      if( (characters[0]->isAlive()) && (characters[0] != pers))
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
bool fightGroup::anyoneIsAliveAndInRange(bool onlyHostile)
{ 
   //TODO verify range!
   int i;
   for(i=0;i<actualCharacters;i++)
   {
      if( (characters[i]->isAlive()) && 
          ( (!onlyHostile) || (characters[i]->psychoState == PSYCHO_HOSTILE)) )
      {
         return(true);
      }
   }
   return(false);
}

