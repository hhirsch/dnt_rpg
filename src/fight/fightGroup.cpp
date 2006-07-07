/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "fightGroup.h"

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

