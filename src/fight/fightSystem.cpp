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
         pers->actualEnemy = NULL;
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
void fightSystem::doRound(string& brief)
{
   string tmp;
   personagem* pers;
   brief = "Round Begins.";
   charsInitiatives.newRound();
   pers = charsInitiatives.nextCharacter();
   while(pers != NULL)
   {
      if(isPC(pers))
      {
         getPCAction(pers,tmp);
         brief += "|" + tmp;
      }
      else
      {
         doNPCAction(pers,tmp);
         brief += "|" + tmp;
      }
      pers = charsInitiatives.nextCharacter();
   }
   brief += "|Round Ends.";
}

/***************************************************************
 *                       doBattleCicle                         *
 ***************************************************************/
bool fightSystem::doBattleCicle(string& brief)
{
   int i;
   bool verifyPCs = false;
   doRound(brief);

   /* Verify if any PC is alive */
   for(i=0; i < FIGHT_MAX_PC_GROUPS; i++)
   {
       if(pcGroups[i].anyoneIsAliveAndInRange())
       {
           verifyPCs = true;
       }
   }

   if(!verifyPCs)
   {
      /* All PCs dies. */
      return(false); 
   }

   /* Verify if some enemy NPC is alive and in range area*/
   for(i=0; i < FIGHT_MAX_NPC_GROUPS; i++)
   {
       if(npcGroups[i].anyoneIsAliveAndInRange())
       {
           //TODO verify if npc group is evil or not to PC.
           /* at last one enemy NPC is alive */
           return(true);
       }
   }

   
   return(false);
}

/***************************************************************
 *                       doNPCAction                           *
 ***************************************************************/
void fightSystem::doNPCAction(personagem* pers, string& brief)
{
   int attackFeat;

   //doMovimentation, if wanted, before
   doNPCMovimentation(pers, FIGHT_MOVIMENTATION_BEFORE);

   //TODO if is badly wounded and can heal, heal (or heal group)

   //TODO else some sing or enchantment

   //else, do an basic attack
      if( (pers->actualEnemy == NULL) || (pers->actualEnemy->dead))
      {
         pers->actualEnemy =  getNPCEnemy(pers);
      }

      attackFeat = getNPCAttackFeat(pers,pers->actualEnemy);

      if( (pers->actualEnemy != NULL) && (attackFeat != -1))
      {
         pers->actualFeats.applyAttackAndBreakFeat(*pers,attackFeat,
                                                   *pers->actualEnemy, brief);
      }

   //TODO call the animations and sound effects.
 
   doNPCMovimentation(pers,FIGHT_MOVIMENTATION_AFTER);

}

/***************************************************************
 *                       getPCAction                           *
 ***************************************************************/
void fightSystem::getPCAction(personagem* pers, string& brief)
{
   //TODO
}

/***************************************************************
 *                       getNPCAttackFeat                      *
 ***************************************************************/
int fightSystem::getNPCAttackFeat(personagem* pers, personagem* target)
{
   if( (target != NULL) && (pers != NULL))
   { 
       return(pers->actualFeats.getNPCAttackFeat(pers,target));
   }
   
   return(-1);
}

/***************************************************************
 *                       getNPCEnemy                           *
 ***************************************************************/
personagem* fightSystem::getNPCEnemy(personagem* pers)
{
   /*FIXME For now, get the enemy on first enemy list. 
     better make this in a more expert way */

   personagem *ch = NULL;
   int group = 0;

   //Take enemy on PCGroups
   while( (ch == NULL) && (group < FIGHT_MAX_PC_GROUPS))
   {
      ch = pcGroups[group].getNPCEnemy(pers);
      group++;
   }
   
   group = 0;
   //OtherWise, take on NPCGroups
   while( (ch == NULL) && (group < FIGHT_MAX_PC_GROUPS))
   {
      if(group != pers->actualFightGroup)
      {
         ch = npcGroups[group].getNPCEnemy(pers);
      }
      group++;
   }

   return(ch);
}

/***************************************************************
 *                   doNPCMovimentation                        *
 ***************************************************************/
void fightSystem::doNPCMovimentation(personagem* pers, int when)
{
   //TODO
}


