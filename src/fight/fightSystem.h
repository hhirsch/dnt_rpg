#ifndef _fightSystem_h
#define _fightSystem_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "fightGroup.h"
#include "initiative.h"


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
     bool insertPC(personagem* pers, int group, string& brief);
     /*************************************************************** 
     * Reason: Insert Character on NPCs List.  
     * Param:
     *        pers -> Pointer to Character
     *        group -> NPC group Number
     ***************************************************************/
     bool insertNPC(personagem* pers, int group, string& brief);

     /*************************************************************** 
     * Reason: doBattle Events. Returns false when battle ends.
     * Param:
     ***************************************************************/
     bool doBattle();

   private:
     fightGroup npcGroups[FIGHT_MAX_NPC_GROUPS]; /* PCs groups */
     fightGroup pcGroups[FIGHT_MAX_PC_GROUPS];   /* NPCs groups */
     initiative charsInitiatives;     /* Character's Initiative */

     /*************************************************************** 
     * Reason: Verify if character is PC (and is on pcList).  
     * Param:
     *        pers -> Pointer to Character
     ***************************************************************/
     bool isPC(personagem* pers);
     /*************************************************************** 
     * Reason: Do a complete round on game.  
     * Param:
     ***************************************************************/
     void doRound();
};

#endif

