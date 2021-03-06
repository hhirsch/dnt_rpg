/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_fightGroup_h
#define _dnt_fightGroup_h

#include "../engine/character.h"

#define FIGHT_MAX_CHARACTERS_PER_GROUP 5 /**< Max avaible characters per group*/

/*! A fight group is a group of allied characters in battle.*/
//TODO friendly and enemy groups to a group.
class fightGroup
{
   public:
     /*! Group Constructor */
     fightGroup();
     /*! Group Destructor */
     ~fightGroup();

    /*!
     *************************************************************** 
     * Insert Character on Group. Return true if OK.  
     * \param pers -> Pointer to Character
     * \return true if success.
     ***************************************************************/
     bool insertCharacter(character* pers);
     /*!
     *************************************************************** 
     * Verify if character is in Group  
     * \param pers -> Pointer to Character
     * \return true if character is in group.
     ***************************************************************/
     bool isCharacterIn(character* pers);
     /*!
     *************************************************************** 
     * Verify if there's any character alive and in range 
     * in the Group  
     * \param posX -> current character posX
     * \param posZ -> current character posZ
     * \param onlyHostile -> if true, only accept hostile characters
     * \param range -> max range value
     * \return true if someone is alive and in range.
     ***************************************************************/
     bool anyoneIsAliveAndInRange(bool onlyHostile,
           float posX=-1, float posZ=-1, float range=DNT_BATTLE_RANGE);

     /*! Verify if at lat one character is alive at the group */
     bool isAnyoneAlive();

     /*!
     *************************************************************** 
     * Get nearest character enemy to pers.  
     * \param pers -> character pointer.
     * \return an character pointer, enemy to this one.
     ***************************************************************/
     character* getNearestEnemy(character* pers);

     /*!
     *************************************************************** 
     * Empty the group  
     ***************************************************************/
     void empty();

     /*!
      **************************************************************
      * Get total elements on the group
      * \return number of characters on the group;
      **************************************************************/
      int total();

     /*!
      **************************************************************
      * Get the character on the group position
      * \param pos -> position on the group (0 to (total()-1))
      * \return pointer to the character at the position, or NULL
      **************************************************************/
     character* getAtPosition(int pos);


   private:
     /*! Pointers to Characters on Group */
     character* characters[FIGHT_MAX_CHARACTERS_PER_GROUP]; 
     /*! Total number of Characters on Group */
     int actualCharacters;
};

#endif

