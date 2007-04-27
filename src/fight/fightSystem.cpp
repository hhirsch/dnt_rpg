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
 *                           hasEnemies                        *
 ***************************************************************/
bool fightSystem::hasEnemies(personagem* pers, string& brief)
{
   int i;
   bool isNPC = !isPC(pers);

   //TODO friendly groups;

   /* Verify if some enemy PC is alive */
   for(i=0; i < FIGHT_MAX_PC_GROUPS; i++)
   {
       if( ((isNPC) || (pers->actualFightGroup != i) ) &&  
           (pcGroups[i].anyoneIsAliveAndInRange(false)) )
       {
           return(true);
       }
   }

   /* Verify if some enemy NPC is alive and in range area*/
   for(i=0; i < FIGHT_MAX_NPC_GROUPS; i++)
   {
       if( ((!isNPC) || (pers->actualFightGroup != i) ) &&
           (npcGroups[i].anyoneIsAliveAndInRange(true)) )
       {
           //TODO verify if npc group is evil or not to PC.
           /* at last one enemy NPC is alive */
           return(true);
       }
   }

   brief += "|"+ language.FIGHT_END_NOENEMIES;
   return(false);
}


/***************************************************************
 *                           doRound                           *
 ***************************************************************/ 
int fightSystem::doTurn(string& brief)
{
   string tmp;
   personagem* pers;
   brief = "";
 
   pers = charsInitiatives.nextCharacter();

   if(pers == NULL)
   {
      /* Begin new Round */
      brief += language.FIGHT_ROUND_NEW  + "|";
      charsInitiatives.newRound(); 
      pers = charsInitiatives.nextCharacter();
      if(pers == NULL)
      {
         brief += language.FIGHT_ERROR_NO_CHARACTERS;
         return(FIGHT_END);
      }
   }

   /* don't play with dead characters or not Hostile Ones */
   while( (!pers->isAlive()) || 
          ( (!isPC(pers)) && (pers->psychoState != PSYCHO_HOSTILE)) )
   {
      pers = charsInitiatives.nextCharacter();
      if(pers == NULL)
      {
         /* Begin new Round */
         brief += language.FIGHT_ROUND_NEW  + "|";;
         charsInitiatives.newRound(); 
         pers = charsInitiatives.nextCharacter();
         if(pers == NULL)
         {
            brief += language.FIGHT_ERROR_NO_CHARACTERS;
            return(FIGHT_END);
         }
      } 
   }

   tmp = "";
   if(isPC(pers))
   {
       if(!hasEnemies(pers, brief))
       {
          /* There's no more enemies, so no more battle */
          return(FIGHT_END);
       }
       brief += pers->nome + " " + language.FIGHT_TURN;
       return(FIGHT_PC_TURN);
   }
   else
   { 
      doNPCAction(pers,tmp);
      brief += "|" + tmp;
   }

   if(!hasEnemies(pers, brief))
   {
      /* There's no more enemies, so no more battle */
      return(FIGHT_END);
   }

   return(FIGHT_CONTINUE);
}

/***************************************************************
 *                       doBattleCicle                         *
 ***************************************************************/
int fightSystem::doBattleCicle(string& brief)
{
   return(doTurn(brief));
}

/***************************************************************
 *                       doNPCAction                           *
 ***************************************************************/
void fightSystem::doNPCAction(personagem* pers, string& brief)
{
   int attackFeat;

   /* Determine the target of the character */
   if( (pers->actualEnemy == NULL) || (!pers->actualEnemy->isAlive()))
   {
      pers->actualEnemy =  getNPCEnemy(pers);
   }
   
   //doMovimentation, if wanted, before
   doNPCMovimentation(pers, FIGHT_MOVIMENTATION_BEFORE);

   //TODO if is badly wounded and can heal, heal (or heal group)

   //TODO else some sing or enchantment

   //else, do an basic attack
   attackFeat = getNPCAttackFeat(pers,pers->actualEnemy);

   if( (pers->actualEnemy != NULL) && (attackFeat != -1))
   {
      brief += pers->nome + " " + language.FIGHT_ATTACKS + " " + 
               pers->actualEnemy->nome + "|";
      pers->actualFeats.applyAttackAndBreakFeat(*pers,attackFeat,
                                                *pers->actualEnemy, brief);
         
      if(!pers->actualEnemy->isAlive())
      {
         brief += "|" + pers->actualEnemy->nome +" "+ language.FIGHT_DEAD;
      }
      
   }

   //TODO call the animations and sound effects.
   doNPCMovimentation(pers,FIGHT_MOVIMENTATION_AFTER);

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
   while( (ch == NULL) && (group < FIGHT_MAX_NPC_GROUPS))
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

/***************************************************************
 *                          empty                              *
 ***************************************************************/
void fightSystem::empty()
{
   int i;

   for(i=0; i < FIGHT_MAX_PC_GROUPS; i++)
   {
       pcGroups[i].empty();
   }

   for(i=0; i < FIGHT_MAX_NPC_GROUPS; i++)
   {
       npcGroups[i].empty();
   }

   charsInitiatives.clear();
}

/***************************************************************
 *                  actualCharacterTurn                        *
 ***************************************************************/
personagem* fightSystem::actualCharacterTurn()
{
   return(charsInitiatives.actualCharacter());
}

