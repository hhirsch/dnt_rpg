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
bool fightSystem::insertPC(personagem* pers, int group)
{
   if( (group >= 0) && (group < FIGHT_MAX_PC_GROUPS))
   {
      return(pcGroups[group].insertCharacter(pers));
   }
   else
   {
      return(false); 
   }
}

/***************************************************************
 *                         insertNPC                           *
 ***************************************************************/
bool fightSystem::insertNPC(personagem* pers, int group)
{
   if( (group >= 0) && (group < FIGHT_MAX_NPC_GROUPS))
   {
      return(npcGroups[group].insertCharacter(pers));
   }
   else
   {
      return(false); 
   }
}
