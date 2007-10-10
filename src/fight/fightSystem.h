#ifndef _fightSystem_h
#define _fightSystem_h

#include "../classes/xp.h"

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "fightGroup.h"
#include "initiative.h"
#include <libintl.h>
#include "../etc/message3d.h"
#include "../particle/partSystem.h"
#include <SDL/SDL.h>


#define FIGHT_MAX_NPC_GROUPS 5 /**< Max groups of NPCs characters */
#define FIGHT_MAX_PC_GROUPS  1 /**< Max groups of PCs characters */

#define FIGHT_MOVIMENTATION_BEFORE 1 /**< Movimentation before action */
#define FIGHT_MOVIMENTATION_AFTER  2 /**< Movimentation after action */

#define FIGHT_END 0
#define FIGHT_PC_TURN 1
#define FIGHT_NPC_TURN 2
#define FIGHT_CONTINUE 3

#define FIGHT_ANIMATION_DELAY  2000  /**< Delay for animations */

/*! The fight System Implementation */
class fightSystem
{
   public:
     /*! Constructor 
      * \param controller -> the world messages controller
      * \param pSystem -> the particle system controller */
     fightSystem(messageController* controller, partSystem* pSystem);
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
     bool insertPC(character* pers, int group, string& brief);
     /*!
     *************************************************************** 
     * Insert Character on NPCs List.  
     * \param pers -> Pointer to Character
     * \param group -> NPC group Number
     * \param brief -> briefing string
     * \return true if success.
     ***************************************************************/
     bool insertNPC(character* pers, int group, string& brief);

     /*!
     *************************************************************** 
     * Do Battle Events. Returns false when battle ends.
     * \param brief -> briefing to print, lines separed by '|' .
     * \return true if battle continue, false otherwise.
     ***************************************************************/
     int doBattleCicle(string& brief);
 
     /*!
     *************************************************************** 
     * Empty All the Battle System
     ***************************************************************/
     void empty();

     /*!
     *************************************************************** 
     * Gets actual character's that is in turn
     * \return character that is in turn.
     ***************************************************************/
     character* actualCharacterTurn();

    /*!
     *************************************************************** 
     * Sets actual character's that is in turn
     * \param actor -> new character that is in turn
     ***************************************************************/
     void setActualActor(character* actor);

     /*!
      **************************************************************
      * Verify all Deads that occurs on the turn.
      * \param brief -> briefing to print
      **************************************************************/
      void verifyDeads(string& brief);

    /*!
     ***************************************************************
     * Verify if character is PC (and is on pcList).  
     * \param pers -> Pointer to Character
     * \return true if character is a PC.
     ***************************************************************/
     bool isPC(character* pers);

    /*!
     ***************************************************************
     * Verifies if character has enemies.
     * \param pers -> pointer to character to verify.
     * \param brief -> briefing to print, lines separed by '|' .
     * \return true if has enemies, false otherwise.
     *************************************************************/
     bool hasEnemies(character* pers, string& brief);

     /*!
     *************************************************************** 
     * Get the nearest ememy character to attack.  
     * \param pers -> Pointer to Character that will act
     * \return enemy character target.
     ***************************************************************/
     character* getNearestEnemy(character* pers);

   private:
     fightGroup npcGroups[FIGHT_MAX_NPC_GROUPS];  /**< PCs groups */
     fightGroup pcGroups[FIGHT_MAX_PC_GROUPS];   /**< NPCs groups */
     initiative charsInitiatives;     /**< Character's Initiative */
     Uint32 lastTime;            /**< Last time of some animation */
     character* actualActor;         /**< Actual Character Acting */
     bool pendingAnimation;     /**< If some animation is pending */
     messageController* msgController; /**< World message control */
     partSystem* particleSystem; /**< The particle system control */
   
     /*!
     *************************************************************** 
     * Do a complete turn on game.  
     * \param brief -> briefing to print, lines separed by '|' .
     * \return true if battle continue, false otherwise.
     ***************************************************************/
     int doTurn(string& brief);
     /*!
     *************************************************************** 
     * Do a npc action for character.  
     * \param pers -> Pointer to Character that will act
     * \param brief -> briefing to print, lines separed by '|' .
     ***************************************************************/
     void doNPCAction(character* pers, string& brief);
     /*!
     *************************************************************** 
     * Get the attack feat for NPC to use  
     * \param pers -> Pointer to Character that will use feat
     * \param target -> Pointer to Character that will receive feat
     * \return feat number to use.
     ***************************************************************/
     int getNPCAttackFeat(character* pers, character* target);
     /*!
     *************************************************************** 
     * Do, if wanted, NPC movimentation  
     * \param pers -> Pointer to Character that will act
     * \param when -> time when do movimentation (before or after 
     *                attack) 
     ***************************************************************/
     void doNPCMovimentation(character* pers, int when);

};

#endif

