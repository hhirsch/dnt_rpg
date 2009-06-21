/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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
 *                     getNearestEnemy                         *
 ***************************************************************/
character* fightGroup::getNearestEnemy(character* pers)
{
   character* enemy = NULL;
   float dist = 0;
   float acDist = 0;
   int ch;
   for(ch=0;ch<actualCharacters;ch++)
   {
      if( (characters[ch]->isAlive()) && (characters[ch] != pers))
      {
         if(enemy == NULL)
         {
            enemy = characters[ch];
            acDist = sqrt( (characters[ch]->xPosition - pers->xPosition) *
                           (characters[ch]->xPosition - pers->xPosition) +
                           (characters[ch]->zPosition - pers->zPosition) *
                           (characters[ch]->zPosition - pers->zPosition) );
         }
         else
         {
            dist = sqrt( (characters[ch]->xPosition - pers->xPosition) *
                         (characters[ch]->xPosition - pers->xPosition) +
                         (characters[ch]->zPosition - pers->zPosition) *
                         (characters[ch]->zPosition - pers->zPosition) );
            if(dist < acDist)
            {
               acDist = dist;
               enemy = characters[ch];
            }
         }
      }
   }
   return(enemy);
}

/***************************************************************
 *                  anyoneIsAliveAndInRange                    *
 ***************************************************************/
bool fightGroup::anyoneIsAliveAndInRange(float posX, float posZ, 
                                         bool onlyHostile)
{ 
   float dist;
   int i;
   for(i=0;i<actualCharacters;i++)
   {
      dist = sqrt( (characters[i]->xPosition - posX)*
                   (characters[i]->xPosition - posX) +
                   (characters[i]->zPosition - posZ)*
                   (characters[i]->zPosition - posZ));

      if( (characters[i]->isAlive()) && 
          (dist < DNT_BATTLE_RANGE) &&
          ( (!onlyHostile) || 
            (characters[i]->getPsychoState() == PSYCHO_HOSTILE)) )
      {
         return(true);
      }
   }
   return(false);
}

/***************************************************************
 *                             total                           *
 ***************************************************************/
int fightGroup::total()
{
   return(actualCharacters);
}

/***************************************************************
 *                          getAtPosition                      *
 ***************************************************************/
character* fightGroup::getAtPosition(int pos)
{
   if( (pos >= 0) && (pos < actualCharacters) )
   {
      return(characters[pos]);
   }
   return(NULL);
}

