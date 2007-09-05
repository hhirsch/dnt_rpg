/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "fightSystem.h"

/***************************************************************
 *                       CONSTRUCTOR                           *
 ***************************************************************/
fightSystem::fightSystem(messageController* controller)
{
   lastTime = 0;
   actualActor = NULL;
   pendingAnimation = true;
   msgController = controller;
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
bool fightSystem::insertPC(character* pers, int group,  string& brief)
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
bool fightSystem::insertNPC(character* pers, int group, string& brief)
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
bool fightSystem::isPC(character* pers)
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
bool fightSystem::hasEnemies(character* pers, string& brief)
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
   Uint32 time = SDL_GetTicks();
   brief = "";

   if(time - lastTime >= FIGHT_ANIMATION_DELAY)
   {
      lastTime = time;

      if( (actualActor != NULL) && (actualActor->actualEnemy != NULL) && 
          (pendingAnimation))
      {
         /* Put the actor at idle state */
         pendingAnimation = false;
         actualActor->setState(STATE_IDLE);

         /* Kill the target, if it is dead */
         if(actualActor->actualEnemy->lifePoints <= 0)
         {
            //FIXME Other states, like partial death to be implemented
            actualActor->actualEnemy->kill();
         }
         else
         {
            //TODO call defend or receive animation
         }
      }
      else
      {
         actualActor = charsInitiatives.nextCharacter();

         if(actualActor == NULL)
         {
            /* Begin new Round */
            brief += language.FIGHT_ROUND_NEW  + "|";
            charsInitiatives.newRound(); 
            actualActor = charsInitiatives.nextCharacter();
            if(actualActor == NULL)
            {
               brief += language.FIGHT_ERROR_NO_CHARACTERS;
               return(FIGHT_END);
            }
         }

         /* don't play with dead characters or not Hostile Ones */
         while( (!actualActor->isAlive()) || 
                ( (!isPC(actualActor)) && 
                  (actualActor->psychoState != PSYCHO_HOSTILE)) )
         {
            actualActor = charsInitiatives.nextCharacter();
            if(actualActor == NULL)
            {
               /* Begin new Round */
               brief += language.FIGHT_ROUND_NEW  + "|";;
               charsInitiatives.newRound(); 
               actualActor = charsInitiatives.nextCharacter();
               if(actualActor == NULL)
               {
                  brief += language.FIGHT_ERROR_NO_CHARACTERS;
                  return(FIGHT_END);
               }
            } 
         }

         tmp = "";
         if(isPC(actualActor))
         {
             if(!hasEnemies(actualActor, brief))
             {
                /* There's no more enemies, so no more battle */
                return(FIGHT_END);
             }
             brief += actualActor->nome + " " + language.FIGHT_TURN;
             return(FIGHT_PC_TURN);
         }
         else
         { 
            doNPCAction(actualActor,tmp);
            pendingAnimation = true;
            brief += "|" + tmp;
         }

         if(!hasEnemies(actualActor, brief))
         {
            /* There's no more enemies, so no more battle */
            return(FIGHT_END);
         }
         
      }
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
void fightSystem::doNPCAction(character* pers, string& brief)
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
                                                *pers->actualEnemy, brief,
                                                msgController);
         
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
int fightSystem::getNPCAttackFeat(character* pers, character* target)
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
character* fightSystem::getNPCEnemy(character* pers)
{
   /*FIXME For now, get the enemy on first enemy list. 
     better make this in a more expert way */

   character *ch = NULL;
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
void fightSystem::doNPCMovimentation(character* pers, int when)
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
   lastTime = 0;
   actualActor = NULL;
   pendingAnimation = true;
}

/***************************************************************
 *                  actualCharacterTurn                        *
 ***************************************************************/
character* fightSystem::actualCharacterTurn()
{
   return(charsInitiatives.actualCharacter());
}

