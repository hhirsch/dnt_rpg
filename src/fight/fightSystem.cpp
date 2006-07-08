/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "fightSystem.h"

/***************************************************************
 *                       CONSTRUCTOR                           *
 ***************************************************************/
fightSystem::fightSystem()
{
}

/***************************************************************
 *                        DESTRUCTOR                           *
 ***************************************************************/
fightSystem::~fightSystem()
{
}

/***************************************************************
 *                         insertPC                            *
 ***************************************************************/
bool fightSystem::insertPC(personagem* pers, int group,  string& brief)
{
   if( (group >= 0) && (group < FIGHT_MAX_PC_GROUPS))
   {
      if(pcGroups[group].insertCharacter(pers))
      {
          pers->actualFightGroup = group;
          charsInitiatives.insertCharacter(pers,brief);
          return(true);
      }
      else
      {
          return(false);
      }
   }
   else
   {
      return(false); 
   }
}

/***************************************************************
 *                         insertNPC                           *
 ***************************************************************/
bool fightSystem::insertNPC(personagem* pers, int group, string& brief)
{
   if( (group >= 0) && (group < FIGHT_MAX_NPC_GROUPS))
   {
      if(npcGroups[group].insertCharacter(pers))
      {
         pers->actualFightGroup = group;
         charsInitiatives.insertCharacter(pers,brief);
         return(true);
      }
      else
      {
         return(false);
      }
   }
   else
   {
      return(false); 
   }
}

/***************************************************************
 *                             isPC                            *
 ***************************************************************/
bool fightSystem::isPC(personagem* pers)
{
   int i;
   for(i=0;i<FIGHT_MAX_PC_GROUPS;i++)
   {
      if(pcGroups[i].isCharacterIn(pers))
      {
          return(true);
      }
   }
   return(false);
}

/***************************************************************
 *                           doRound                           *
 ***************************************************************/ 
void fightSystem::doRound()
{
   personagem* pers;
   charsInitiatives.newRound();
   pers = charsInitiatives.nextCharacter();
   while(pers != NULL)
   {
      if(isPC(pers))
      {
         getPCAction(pers);
      }
      else
      {
         doNPCAction(pers);
      }
      pers = charsInitiatives.nextCharacter();
   }
}


bool fightSystem::doBattleCicle()
{
   doRound();
   //TODO Verify battle end conditions
   //Condition: No more enemies.
   //First condition: enemies dies
   //Second condition: enemies out of range
   //Third condition: all PCs dies, game over
   return(true);
}

void fightSystem::doNPCAction(personagem* pers)
{
   personagem* target;
   int attackFeat;

   //doMovimentation, if wanted, before
   doNPCMovimentation(pers, FIGHT_MOVIMENTATION_BEFORE);

   //TODO if is badly wounded and can heal, heal (or heal group)

   //TODO some song or enchantment

   //else, do an basic attack
   target =  getNPCEnemy(pers);
   attackFeat = getNPCAttackFeat(pers,target);
 
   doNPCMovimentation(pers,FIGHT_MOVIMENTATION_AFTER);

}

void fightSystem::getPCAction(personagem* pers)
{
   //TODO
}


int fightSystem::getNPCAttackFeat(personagem* pers, personagem* target)
{
   //TODO
   return(0);
}

personagem* fightSystem::getNPCEnemy(personagem* pers)
{
   //TODO
   return(NULL);
}

void fightSystem::doNPCMovimentation(personagem* pers, int when)
{
   //TODO
}


