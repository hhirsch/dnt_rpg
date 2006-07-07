#ifndef _fightGroup_h
#define _fightGroup_h

#include "../engine/personagens.h"

#define FIGHT_MAX_CHARACTERS_PER_GROUP 5


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

   private:
     /* Pointers to Characters on Group */
     personagem* characters[FIGHT_MAX_CHARACTERS_PER_GROUP]; 
     /* Total number of Characters on Group */
     int actualCharacters;
};

#endif

