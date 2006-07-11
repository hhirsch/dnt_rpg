#ifndef _fightSystem_h
#define _fightSystem_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "fightGroup.h"
#include "initiative.h"


#define FIGHT_MAX_NPC_GROUPS 5
#define FIGHT_MAX_PC_GROUPS  1

#define FIGHT_MOVIMENTATION_BEFORE 1
#define FIGHT_MOVIMENTATION_AFTER  2

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
     *        brief -> briefing string
     ***************************************************************/
     bool insertNPC(personagem* pers, int group, string& brief);

     /*************************************************************** 
     * Reason: do Battle Events. Returns false when battle ends.
     * Param:
     *        brief -> briefing to print, lines separed by '|' .
     ***************************************************************/
     bool doBattleCicle(string& brief);

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
     *        brief -> briefing to print, lines separed by '|' .
     ***************************************************************/
     void doRound(string& brief);
     /*************************************************************** 
     * Reason: Do a npc action for character.  
     * Param:
     *        pers -> Pointer to Character that will act
     ***************************************************************/
     void doNPCAction(personagem* pers, string& brief);
     /*************************************************************** 
     * Reason: Get an action for player character.  
     * Param:
     *        pers -> Pointer to Character that will act
     ***************************************************************/
     void getPCAction(personagem* pers, string& brief);
     /*************************************************************** 
     * Reason: Returns the attack feat for NPC to use  
     * Param:
     *        pers -> Pointer to Character that will use feat
     *        target -> Pointer to Character that will receive feat
     ***************************************************************/
     int getNPCAttackFeat(personagem* pers, personagem* target);
     /*************************************************************** 
     * Reason: Returns an ememy character to attack.  
     * Param:
     *        pers -> Pointer to Character that will act
     ***************************************************************/
     personagem* getNPCEnemy(personagem* pers);
     /*************************************************************** 
     * Reason: Do, if wanted, NPC movimentation  
     * Param:
     *        pers -> Pointer to Character that will act
     *        when -> time when do movimentation (before or after 
     *                attack) 
     ***************************************************************/
     void doNPCMovimentation(personagem* pers, int when);
};

#endif

