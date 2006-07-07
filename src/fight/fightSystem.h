#ifndef _fightSystem_h
#define _fightSystem_h

#include "fightGroup.h"

#define FIGHT_MAX_NPC_GROUPS 5
#define FIGHT_MAX_PC_GROUPS  1


class fightSystem
{
   public:
     fightSystem();
     ~fightSystem();
  
     /*************************************************************** 
     * Reason: Insert Character on PCs List.  
     * Param:
     *        pers -> Pointer to Character
     *        group -> PC group Number
     ***************************************************************/
     bool insertPC(personagem* pers, int group);
     /*************************************************************** 
     * Reason: Insert Character on NPCs List.  
     * Param:
     *        pers -> Pointer to Character
     *        group -> NPC group Number
     ***************************************************************/
     bool insertNPC(personagem* pers, int group);

   private:
     fightGroup npcGroups[FIGHT_MAX_NPC_GROUPS]; /* PCs groups */
     fightGroup pcGroups[FIGHT_MAX_PC_GROUPS];   /* NPCs groups */
};

#endif

