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
bool fightSystem::insertPC(personagem* pers, int group)
{
   if( (group >= 0) && (group < FIGHT_MAX_PC_GROUPS))
   {
      if(pcGroups[group].insertCharacter(pers))
      {
          charsInitiatives.insertCharacter(pers);
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
         //TODO get user action to character
      }
      else
      {
         //TODO get AI action to character
      }
      pers = charsInitiatives.nextCharacter();
   }
}

