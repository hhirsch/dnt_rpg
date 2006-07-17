#ifndef _fightSystem_h
#define _fightSystem_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "fightGroup.h"
#include "initiative.h"


#define FIGHT_MAX_NPC_GROUPS 5 /**< Max groups of NPCs characters */
#define FIGHT_MAX_PC_GROUPS  1 /**< Max groups of PCs characters */

#define FIGHT_MOVIMENTATION_BEFORE 1 /**< Movimentation before action */
#define FIGHT_MOVIMENTATION_AFTER  2 /**< Movimentation after action */

/*! The fight System Implementation */
class fightSystem
{
   public:
     /*! Constructor */
     fightSystem();
     /*! Destructor */
     ~fightSystem();
  
     /*!
     *************************************************************** 
     * Insert Character on PCs List.  
     * \param pers -> Pointer to Character
     * \param group -> PC group Number
     * \param brief -> briefing string
     * \return true if success.
     ***************************************************************/
     bool insertPC(personagem* pers, int group, string& brief);
     /*!
     *************************************************************** 
     * Insert Character on NPCs List.  
     * \param pers -> Pointer to Character
     * \param group -> NPC group Number
     * \param brief -> briefing string
     * \return true if success.
     ***************************************************************/
     bool insertNPC(personagem* pers, int group, string& brief);

     /*!
     *************************************************************** 
     * Do Battle Events. Returns false when battle ends.
     * \param brief -> briefing to print, lines separed by '|' .
     * \return true if battle continue, false otherwise.
     ***************************************************************/
     bool doBattleCicle(string& brief);

   private:
     fightGroup npcGroups[FIGHT_MAX_NPC_GROUPS]; /**< PCs groups */
     fightGroup pcGroups[FIGHT_MAX_PC_GROUPS];   /**< NPCs groups */
     initiative charsInitiatives;     /**< Character's Initiative */

     /*!
     *************************************************************** 
     * Verify if character is PC (and is on pcList).  
     * \param pers -> Pointer to Character
     * \return true if character is a PC.
     ***************************************************************/
     bool isPC(personagem* pers);
     /*!
     *************************************************************** 
     * Do a complete turn on game.  
     * \param brief -> briefing to print, lines separed by '|' .
     * \return true if battle continue, false otherwise.
     ***************************************************************/
     bool doTurn(string& brief);
     /*!
     *************************************************************** 
     * Do a npc action for character.  
     * \param pers -> Pointer to Character that will act
     * \param brief -> briefing to print, lines separed by '|' .
     ***************************************************************/
     void doNPCAction(personagem* pers, string& brief);
     /*!
     *************************************************************** 
     * Get an action for player character.  
     * \param pers -> Pointer to Character that will act
     * \param brief -> briefing to print, lines separed by '|' .
     ***************************************************************/
     void getPCAction(personagem* pers, string& brief);
     /*!
     *************************************************************** 
     * Get the attack feat for NPC to use  
     * \param pers -> Pointer to Character that will use feat
     * \param target -> Pointer to Character that will receive feat
     * \return feat number to use.
     ***************************************************************/
     int getNPCAttackFeat(personagem* pers, personagem* target);
     /*!
     *************************************************************** 
     * Get an ememy character to attack.  
     * \param pers -> Pointer to Character that will act
     * \return enemy character target.
     ***************************************************************/
     personagem* getNPCEnemy(personagem* pers);
     /*!
     *************************************************************** 
     * Do, if wanted, NPC movimentation  
     * \param pers -> Pointer to Character that will act
     * \param when -> time when do movimentation (before or after 
     *                attack) 
     ***************************************************************/
     void doNPCMovimentation(personagem* pers, int when);

     /*!
     ***************************************************************
     * Verifies if character has enemies.
     * \param pers -> pointer to character to verify.
     * \param brief -> briefing to print, lines separed by '|' .
     * \return true if has enemies, false otherwise.
     *************************************************************/
     bool hasEnemies(personagem* pers, string& brief);
};

#endif

