#ifndef _fightGroup_h
#define _fightGroup_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "../engine/personagens.h"

#define FIGHT_MAX_CHARACTERS_PER_GROUP 5

/* A fight group is a group of allied characters in battle.*/

//TODO friendly and enemy groups to a group.

class fightGroup
{
   public:
     fightGroup();
     ~fightGroup();

    /*************************************************************** 
     * Reason: Insert Character on Group. Return true if OK.  
     * Param:
     *        pers -> Pointer to Character
     ***************************************************************/
     bool insertCharacter(personagem* pers);
     /*************************************************************** 
     * Reason: Verify if character is in Group  
     * Param:
     *        pers -> Pointer to Character
     ***************************************************************/
     bool isCharacterIn(personagem* pers);

     personagem* getNPCEnemy(personagem* pers);

   private:
     /* Pointers to Characters on Group */
     personagem* characters[FIGHT_MAX_CHARACTERS_PER_GROUP]; 
     /* Total number of Characters on Group */
     int actualCharacters;
};

#endif

