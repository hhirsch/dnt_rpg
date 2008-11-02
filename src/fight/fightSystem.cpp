/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "fightSystem.h"
#include "../engine/modstate.h"

/***************************************************************
 *                       CONSTRUCTOR                           *
 ***************************************************************/
fightSystem::fightSystem(partController* pSystem)
{
   lastTime = 0;
   actualActor = NULL;
   pendingAnimation = true;
   particleSystem = pSystem;
   mapFileName = "";
   brief = new briefing();
}

/***************************************************************
 *                        DESTRUCTOR                           *
 ***************************************************************/
fightSystem::~fightSystem()
{
   delete(brief);
}

/***************************************************************
 *                          setMap                             *
 ***************************************************************/
void fightSystem::setMap(string fileName)
{
   mapFileName = fileName;
}

/***************************************************************
 *                         insertPC                            *
 ***************************************************************/
bool fightSystem::insertPC(character* pers, int group)
{
   if( (group >= 0) && (group < FIGHT_MAX_PC_GROUPS))
   {
      if(pcGroups[group].insertCharacter(pers))
      {
          pers->actualFightGroup = group;
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
bool fightSystem::insertNPC(character* pers, int group)
{
   if( (group >= 0) && (group < FIGHT_MAX_NPC_GROUPS))
   {
      if(npcGroups[group].insertCharacter(pers))
      {
         pers->actualFightGroup = group;
         charsInitiatives.insertCharacter(pers);
         pers->currentEnemy = NULL;
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
bool fightSystem::hasEnemies(character* pers)
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

   brief->addText(gettext("No more enemies."), 27, 208, 23);
   return(false);
}

/***************************************************************
 *                        setActualActor                       *
 ***************************************************************/
void fightSystem::setActualActor(character* actor)
{
   actualActor = actor;
}

/***************************************************************
 *                         verifyDeads                         *
 ***************************************************************/ 
void fightSystem::verifyDeads()
{
   char buf[512];
   //TODO verify other deads than the target one (for example for a feat
   // that affects an area instead of a target)!

   /* Kill the target, if it is dead and not marked as dead */
   if( (actualActor->currentEnemy->getLifePoints() <= 0) && 
       (actualActor->currentEnemy->isAlive()) )
   {

      //FIXME Other states, like partial death to be implemented
      actualActor->currentEnemy->kill();

      sprintf(buf, gettext("%s is dead!"), 
              actualActor->currentEnemy->name.c_str());
      brief->addText(buf, 255, 144, 0);

      /* Add to the modstate the 'dead character' */
      //FIXME always using as character. Must check if it is an object!
      character* currentEnemy = (character*)actualActor->currentEnemy;
      modState modif;
      modif.mapCharacterAddAction(MODSTATE_ACTION_CHARACTER_DEAD,
                                  currentEnemy->getCharacterFile(),
                                  mapFileName, 
                                  currentEnemy->xPosition,
                                  currentEnemy->yPosition,
                                  currentEnemy->zPosition,
                                  currentEnemy->orientation,
                                  currentEnemy->initialXPosition,
                                  currentEnemy->initialZPosition);

      if(isPC(actualActor))
      {
         int i;
         int pcg = 0;
         character* p = NULL;
         int xp = 0;
         /* Get the PC group */
         for(i=0;i<FIGHT_MAX_PC_GROUPS;i++)
         {
            if(pcGroups[i].isCharacterIn(actualActor))
            {
               pcg = i;
            }
         }

         /* Apply the XP for all PC Characters on the group */
         for(i=0; i < pcGroups[pcg].total(); i++)
         {
            p = pcGroups[pcg].getAtPosition(i);
            if(p)
            {
               /* Calculate and apply the number of XP points 
                * for killing the target */
               xp = (int)(actualActor->currentEnemy->xpPercent/100.0 * 
                     getXP(actualActor, actualActor->currentEnemy->cr)) / 
                     pcGroups[pcg].total();
               p->addXP(xp);
               sprintf(buf, gettext("%s receive %d XP for killing"), 
                       p->name.c_str(), xp);
               brief->addText(buf, 255, 144, 0);
            }
         }

         /* Since is dead, the xpPercent avaible to give is now 0 */
         actualActor->currentEnemy->xpPercent = 0;
      }
   }
   else
   {
      //TODO call defend or receive animation
   }

}

/***************************************************************
 *                           doRound                           *
 ***************************************************************/ 
int fightSystem::doTurn()
{
   char buffer[512];
   Uint32 time = SDL_GetTicks();

   if(time - lastTime >= FIGHT_ANIMATION_DELAY)
   {
      lastTime = time;

      if( (actualActor != NULL) && (actualActor->currentEnemy != NULL) && 
          (pendingAnimation))
      {
         /* Put the actor at idle state */
         pendingAnimation = false;
         actualActor->setState(STATE_IDLE);

         verifyDeads();

      }
      else
      {
         actualActor = charsInitiatives.nextCharacter();

         /* Verify new round! */
         if(actualActor == NULL)
         {
            /* Begin new Round */
            brief->addText(gettext("New Round"));
            charsInitiatives.newRound(); 
            actualActor = charsInitiatives.nextCharacter();
            if(actualActor == NULL)
            {
               brief->addText(gettext("Error: No Characters!"), 255,0,0);
               return(FIGHT_END);
            }
         }

         /* don't play with dead characters or not Hostile Ones */
         while( (!actualActor->isAlive()) || 
                ( (!isPC(actualActor)) && 
                  (actualActor->getPsychoState() != PSYCHO_HOSTILE)) )
         {
            actualActor = charsInitiatives.nextCharacter();
            if(actualActor == NULL)
            {
               /* Begin new Round */
               brief->addText(gettext("New Round"));
               charsInitiatives.newRound(); 
               actualActor = charsInitiatives.nextCharacter();
               if(actualActor == NULL)
               {
                  brief->addText(gettext("Error: No Characters!"), 255,0,0);
                  return(FIGHT_END);
               }
            } 
         }

         /* Verify if the hostile creature have enemies */
         if( (actualActor->getPsychoState() == PSYCHO_HOSTILE) &&
             (!hasEnemies(actualActor)) )
         {
            /* There's no more enemies, so no more battle */
            return(FIGHT_END);
         }

         /* Set its state to IDLE */
         actualActor->setState(STATE_IDLE);

         if(isPC(actualActor))
         {
             if(!hasEnemies(actualActor))
             {
                /* There's no more enemies, so no more battle */
                return(FIGHT_END);
             }
             sprintf(buffer, gettext("%s's turn."), actualActor->name.c_str());
             brief->addText(buffer);
             return(FIGHT_PC_TURN);
         }
         else
         { 
            sprintf(buffer, gettext("%s's turn."), actualActor->name.c_str());
            brief->addText(buffer);
            
            /* If NPC hasn't script, run from here */
            if(actualActor->getBattleScript() == NULL)
            {
               doNPCAction(actualActor);
               pendingAnimation = true;
            }
            else
            {
               /* Run the script from the engine */
               return(FIGHT_NPC_TURN);
            }
         }
      }
   }

   return(FIGHT_CONTINUE);
}

/***************************************************************
 *                       doBattleCicle                         *
 ***************************************************************/
int fightSystem::doBattleCicle()
{
   return(doTurn());
}

/***************************************************************
 *                       doNPCAction                           *
 ***************************************************************/
void fightSystem::doNPCAction(character* pers)
{
   int attackFeat;

   /* Determine the target of the character */
   if( (pers->currentEnemy == NULL) || (!pers->currentEnemy->isAlive()))
   {
      pers->currentEnemy =  getNearestEnemy(pers);
   }
   
   //doMovimentation, if wanted, before
   doNPCMovimentation(pers, FIGHT_MOVIMENTATION_BEFORE);

   //TODO if is badly wounded and can heal, heal (or heal group)

   //TODO else some sing or enchantment

   //else, do an basic attack
   attackFeat = getNPCAttackFeat(pers,pers->currentEnemy);

   if( (pers->currentEnemy != NULL) && (attackFeat != -1))
   {
      pers->actualFeats.applyAttackAndBreakFeat(*pers,attackFeat,
                                                pers->currentEnemy,
                                                particleSystem);
      verifyDeads();   
   }

   //TODO call the animations and sound effects.
   doNPCMovimentation(pers,FIGHT_MOVIMENTATION_AFTER);

}

/***************************************************************
 *                       getNPCAttackFeat                      *
 ***************************************************************/
int fightSystem::getNPCAttackFeat(character* pers, thing* target)
{
   if( (target != NULL) && (pers != NULL))
   { 
       return(pers->actualFeats.getRandomNPCAttackFeat(pers,
                                                       (character*)target));
   }
   
   return(-1);
}

/***************************************************************
 *                      getNearestEnemy                        *
 ***************************************************************/
character* fightSystem::getNearestEnemy(character* pers)
{
   character *ch = NULL;
   int group = 0;
   character* enemy = NULL;
   float dist = 0, 
         acDist = 0;

   //Take enemy on PCGroups
   if(!isPC(pers))
   {
      while(group < FIGHT_MAX_PC_GROUPS)
      {
         ch = pcGroups[group].getNearestEnemy(pers);
         if( (enemy == NULL) && (ch != NULL) )
         {
            acDist = sqrt( (ch->xPosition - pers->xPosition) *
                           (ch->xPosition - pers->xPosition) +
                           (ch->zPosition - pers->zPosition) *
                           (ch->zPosition - pers->zPosition) );
            enemy = ch;
         }
         else if(ch != NULL)
         {
            dist = sqrt( (ch->xPosition - pers->xPosition) *
                         (ch->xPosition - pers->xPosition) +
                         (ch->zPosition - pers->zPosition) *
                         (ch->zPosition - pers->zPosition) );
            if(dist < acDist)
            {
               acDist = dist;
               enemy = ch;
            }
         }
         group++;
      }
   }
   
   group = 0;
   //OtherWise, take on NPCGroups
   while(group < FIGHT_MAX_NPC_GROUPS)
   {
      if(group != pers->actualFightGroup)
      {
         ch = npcGroups[group].getNearestEnemy(pers);
         if( (enemy == NULL) && (ch != NULL) )
         {
            acDist = sqrt( (ch->xPosition - pers->xPosition) *
                           (ch->xPosition - pers->xPosition) +
                           (ch->zPosition - pers->zPosition) *
                           (ch->zPosition - pers->zPosition) );
            enemy = ch;
         }
         else if(ch != NULL)
         {
            dist = sqrt( (ch->xPosition - pers->xPosition) *
                         (ch->xPosition - pers->xPosition) +
                         (ch->zPosition - pers->zPosition) *
                         (ch->zPosition - pers->zPosition) );
            if(dist < acDist)
            {
               acDist = dist;
              enemy = ch;
            }
         }
      }
      group++;
   }

   return(enemy);
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

