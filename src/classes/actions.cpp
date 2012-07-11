/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "actions.h"
#include "../engine/dccnit.h"
#include "../engine/briefing.h"
#include "../engine/util.h"
#include "../etc/message3d.h"
#include "../particle/partcontroller.h"

using namespace std;

/********************************************************
 *                    numberAction                      *
 ********************************************************/
int numberAction(string buffer)
{
   int ret = -1;
   if(buffer.compare(ACT_STR_OPEN) == 0)
     ret = ACT_OPEN;
   else if(buffer.compare(ACT_STR_ATTACK) == 0)
     ret = ACT_ATTACK;
   else if(buffer.compare(ACT_STR_DRINK) == 0)
     ret = ACT_DRINK;
   else if(buffer.compare(ACT_STR_EAT) == 0)
     ret = ACT_EAT;
   else if(buffer.compare(ACT_STR_DEFEND) == 0)
     ret = ACT_DEFEND;
   else if(buffer.compare(ACT_STR_TALK) == 0)
     ret = ACT_TALK;
   else if(buffer.compare(ACT_STR_CLOSE) == 0)
     ret = ACT_CLOSE;
   else if(buffer.compare(ACT_STR_MOVE) == 0)
     ret = ACT_MOVE;
   else if(buffer.compare(ACT_STR_TAKE) == 0)
     ret = ACT_TAKE;
   else if(buffer.compare(ACT_STR_FREE) == 0)
     ret = ACT_FREE;
   else if(buffer.compare(ACT_STR_MODIFY_OWN_CONCEPT) == 0)
     ret = ACT_MODIFY_OWN_CONCEPT;
   else if(buffer.compare(ACT_STR_MODIFY_OTHER_CONCEPT) == 0)
     ret = ACT_MODIFY_OTHER_CONCEPT;
   else if(buffer.compare(ACT_STR_CONJURE) == 0)
     ret = ACT_CONJURE;
   else if(buffer.compare(ACT_STR_MODIFY_OBJECT) == 0)
     ret = ACT_MODIFY_OBJECT;
   else if(buffer.compare(ACT_STR_HEAL) == 0)
     ret = ACT_HEAL;
   else if(buffer.compare(ACT_STR_FIX) == 0)
     ret = ACT_FIX;
   else if(buffer.compare(ACT_STR_BREAK) == 0)
     ret = ACT_BREAK;
   else if(buffer.compare(ACT_STR_WAIT) == 0)
     ret = ACT_WAIT;

   return(ret);
}

/********************************************************
 *                  numberActionType                    *
 ********************************************************/
int numberActionType(string buffer)
{
   int ret = -1;

   if(buffer.compare(ACT_TYPE_STR_FREE_ACTION) == 0)
     ret = ACT_TYPE_FREE_ACTION;
   else if(buffer.compare(ACT_TYPE_STR_NORMAL_ACTION) == 0)
     ret = ACT_TYPE_NORMAL_ACTION;
   else if(buffer.compare(ACT_TYPE_STR_SECUNDARY_ACTION) == 0)
     ret = ACT_TYPE_SECUNDARY_ACTION;
   else if(buffer.compare(ACT_TYPE_STR_NO_ACTION) == 0)
     ret = ACT_TYPE_NO_ACTION;

   return(ret);
}


/***************************************************************
 *                       doHealOrAttack                        *
 ***************************************************************/
bool doHealOrAttack(thing* actor, thing* target, 
                    diceThing diceInfo, factor* conceptBonus,
                    int range, bool heal)
{
   factor against;

   against.type = MOD_TYPE_THING;
   against.id = THING_ARMATURE_CLASS;
   return(doHealOrAttack(actor, target, diceInfo, conceptBonus, &against,
                         range, heal));
}

/***************************************************************
 *                       doHealOrAttack                        *
 ***************************************************************/
bool doHealOrAttack(thing* actor, thing* target, 
                    diceThing diceInfo, factor* conceptBonus,
                    factor* conceptAgainst, int range, bool heal)
{
   partController pSystem;
   int diceValue;
   int criticalRoll = -1;
   int damage = 0;
   int targetValue; 
   int bonus;
   bool criticalHit = false;
   bool criticalMiss = false;
   bool miss = false;
   char texto[1024];
   string diceText;
   briefing brief;
   messageController controller;
   collision colDet;
   particleSystem* bloodPart;

   /* Define Actor orientation
    * FIXME -> call rotate animation! */
   actor->scNode->setAngle(0.0f, getAngle(actor->scNode->getPosX(), 
                                         actor->scNode->getPosZ(),
                                         target->scNode->getPosX(),
                                         target->scNode->getPosZ()), 0.0f);

   /* Must set some target when attacking */
   if(!heal)
   {
      /* Define Actor target to the current */
      actor->currentEnemy = target;

      /* Define Target Enemy to actor, if no enemy defined */
      if(!target->currentEnemy)
      {
         target->currentEnemy = actor;
      }

      /* define its status to hostile */
      target->setPsychoState(PSYCHO_HOSTILE);
   }
   
   /* Show try brief */
   if(!heal)
   {
      sprintf(texto, gettext("%s tries to attack %s"), 
                     actor->name.c_str(), target->name.c_str());
   }
   else
   {
      sprintf(texto, gettext("%s tries to heal %s"), 
                     actor->name.c_str(), target->name.c_str());
   }
   brief.addText(texto);

   /* Verify Action Range */
   if( (range != 0) && 
       (!actionInRange(actor->scNode->getPosX(), actor->scNode->getPosZ(), 
                     target->scNode->getPosX(), target->scNode->getPosZ(),
                     range*METER_TO_DNT)))
   {
      brief.addText(gettext("Too far away for action!"), 225, 20, 20);
      return(false);
   }

   /* Verify action sight */
   if(target->getThingType() == THING_TYPE_CHARACTER)
   {
      if(!colDet.characterAtSight((character*)actor, (character*)target))
      {
         brief.addText(gettext("Enemy out of sight!"), 255, 20, 20);
         return(false);
      }
   }

   srand(SDL_GetTicks());

   /* Call the animation */
   //TODO call other animation, if is defined
   if(!heal)
   {
      actor->callAttackAnimation();
   }
   //TODO -> create an heal animation!


   //TODO verify if can use or not based on target thing


   /* If no concept test, always hit! */
   if(conceptAgainst)
   {
      /* Apply Bonuses */
      //FIXME get fromm the relative attack, not always of the first!
      bonus = actor->sizeModifier + 
         actor->curBonusAndSaves.baseAttackBonus.getBonus(1);
      if(conceptBonus)
      { 
         bonus += actor->getBonusValue(*conceptBonus);
      }

      dice d20;
      diceValue = d20.roll();

      //TODO apply reflexes bonus, esquive bonus, etc, to target,
      //depending of the attack type!
      targetValue = target->getBonusValue(*conceptAgainst);

      /* Defined heal agains't as half difficult */
      if(heal)
      {
         targetValue /= 2.0f;
      }

      /* verify critical Hit */
      if(diceValue == DICE_D20)
      {
         criticalRoll = d20.roll();
         if( (criticalRoll + bonus - targetValue) > 0)
         {
            criticalHit = true;
         }
      }
      else
      {
         /* verify critical Miss */
         if( diceValue == 1)  
         {
            miss = true;
            criticalRoll = d20.roll();
            if( (criticalRoll + bonus - targetValue) <= 0 )
            {
               criticalMiss = true;
            }
         }
      }

      /* Put Dice Values on Briefing */
      char txtBonus[32];
      if(bonus >= 0)
      {
         sprintf(txtBonus,"+%d",bonus);
      }
      else
      {
         sprintf(txtBonus,"%d",bonus);
      }

      if(criticalRoll != -1)
      {
         sprintf(texto,"%d(%s) & (%d%s) x %d : ",diceValue,txtBonus,
               criticalRoll,
               txtBonus,targetValue);
      }
      else
      {
         sprintf(texto,"%d(%s) x %d : ",diceValue,txtBonus,targetValue);
      }
      diceText = texto;

      //apply bonus (skill bonus)
      diceValue += bonus;

      /*TODO apply resistances  */

      if( ( (diceValue - targetValue <= 0) || (criticalMiss) || (miss) ) &&
          (!criticalHit) )
      {
         brief.addText(diceText + gettext("Miss."));
         if( criticalMiss )
         {
            brief.addText(gettext("Critical Miss!"), 220, 0, 0);
            controller.addMessage(actor->scNode->getPosX(),
               actor->scNode->getPosY() + actor->scNode->getBoundingBox().max.y,
               actor->scNode->getPosZ(),
               gettext("Critical Miss!"), 0.92,0.41,0.14);
            if(heal)
            {
               /* Damage the target with ??? ! */
               //TODO
            }
            else
            {
               //TODO lose weapon;
            }
         }
         else
         {
            controller.addMessage(actor->scNode->getPosX(),
               actor->scNode->getPosY()+actor->scNode->getBoundingBox().max.y,
               actor->scNode->getPosZ(),gettext("Miss."),
                  0.92,0.41,0.14);
         }
         return(true);
      }

   }

   /* Apply Base Damage Dices */
   damage += diceInfo.baseDice.roll(criticalHit);

   /* Apply Damage/Heal modifiers bonus */
   if(conceptBonus)
   {
      damage += actor->getBonusValue(*conceptBonus);
   }

   /* Make sure damage value is at last 1 */
   if(damage <= 0)
   {
      damage = 1;
   }

   /*TODO apply aditional dices */

   if(heal)
   {
      sprintf(texto,gettext("Healed %d points."),damage);
   }
   else
   {
      /* Verify instant killing the target */
      int instantKill = diceInfo.instantKill /* + target->getInstantKilled() */;
      dice d100(100);
      if(d100.roll() < instantKill)
      {
         /* Got an instantaneous death here! */
         damage = target->getMaxLifePoints();
         sprintf(texto, gettext("Hit at a vital area, inflicting %d points "
                               "of damage."), damage);
      }
      else
      {
         sprintf(texto,gettext("Hit for %d points."),damage);
      }
   }
   brief.addText(diceText + texto);

   if(heal)
   {
      /* apply the heal */
      target->addLifePoints(damage);
   }
   else
   {
      /* apply damage on thing */
      target->addLifePoints(-damage);
   }

   if(criticalHit)
   {
      if(heal)
      {
         brief.addText(gettext("Critical Heal!"), 12, 10, 128);
         controller.addMessage(actor->scNode->getPosX(),
               actor->scNode->getPosY()+actor->scNode->getBoundingBox().max.y,
               actor->scNode->getPosZ(),gettext("Critical Heal!"),
               0.06,0.24,0.86);
      }
      else
      {
         brief.addText(gettext("Critical Hit!"), 12, 10, 128);
         /* Show critical hit */
         controller.addMessage(actor->scNode->getPosX(),
               actor->scNode->getPosY()+actor->scNode->getBoundingBox().max.y,
               actor->scNode->getPosZ(),gettext("Critical Hit!"),
               0.84,0.2,0.01);
      }
   }
   /* Show Damage */
   sprintf(texto,"%d",damage);
   if(heal)
   {
      controller.addMessage(target->scNode->getPosX(), 
            target->scNode->getPosY() + target->scNode->getBoundingBox().max.y,
            target->scNode->getPosZ(), texto,
            0.21, 0.15, 0.7);
   } 
   else
   {
      controller.addMessage(target->scNode->getPosX(), 
            target->scNode->getPosY() + target->scNode->getBoundingBox().max.y,
            target->scNode->getPosZ(), texto,
            0.4, 0.01, 0.03);
   }

   if(heal)
   {
      /* Add Some Effect to the healed creature! */
      //TODO!
   }
   else
   {
      /* Add Blood */
      GLfloat cs = cos(deg2Rad(target->scNode->getAngleY()));
      GLfloat sn = sin(deg2Rad(target->scNode->getAngleY()));

      bloodPart = pSystem.addParticle(target->scNode->getPosX() - (sn*2),
            target->scNode->getPosY() + target->bloodPosition,
            target->scNode->getPosZ() - (cs*2), target->bloodFileName);

      /* Set the blood follow character if needed */
      if(target->getThingType() == THING_TYPE_CHARACTER)
      {
         bloodPart->setFollowCharacter(target, DNT_PARTICLE_SYSTEM_FOLLOW_HEAD);
      }
   }

   return(true);
}

/***************************************************************
 *                         doAreaAttack                        *
 ***************************************************************/
bool doAreaAttack(thing* actor, float targetX, float targetZ, int radius,
                  diceThing diceInfo, factor* conceptBonus,
                  factor* conceptAgainst, int range, engine* eng)
{
   briefing brief;
   partController pSystem;
   messageController controller;
   collision colDet;
   particleSystem* bloodPart;
   string diceText;
   int diceValue;
   int criticalRoll = -1;
   int damage = 0;
   int targetValue; 
   int bonus;
   bool miss = false;
   bool criticalHit = false;
   char texto[1024];

   /* Define Actor orientation
    * FIXME -> call rotate animation! */
   actor->scNode->setAngle(0.0f, getAngle(actor->scNode->getPosX(), 
                                         actor->scNode->getPosZ(),
                                         targetX, targetZ), 0.0f);
   /* Show try brief */
   sprintf(texto, gettext("%s tries to attack an area."), actor->name.c_str());
   brief.addText(texto);

   /* Verify Action Range */
   if( (range != 0) && 
       (!actionInRange(actor->scNode->getPosX(), actor->scNode->getPosZ(), 
                     targetX, targetZ, range*METER_TO_DNT)))
   {
      brief.addText(gettext("Too far away for action!"), 225, 20, 20);
      return(false);
   }

   srand(SDL_GetTicks());

   /* Call the animation */
   actor->callAttackAnimation();

   /* Now let's check action and damage for each character in the area */
   /* TODO: check and apply damage to objects too! */
   int i,c;
   characterList* l = eng->PCs;
   character* targ;
   for(i=0; i < 2; i++)
   {
      targ = (character*)l->getFirst();
      for(c=0; c < l->getTotal(); l++)
      {
         /* Verify if target in area */
         float dX = (targ->scNode->getPosX() - targetX);
         float dZ = (targ->scNode->getPosZ() - targetZ);
         float dist = sqrt( (dX*dX) + (dZ*dZ) );
         if(dist <= radius)
         {
            miss = false;
            criticalHit = false;
            criticalRoll = -1;
            damage = 0;
            /* Let's apply action to the target */
            if(conceptAgainst)
            {
               /* Apply Bonuses */
               bonus = 0;
               if(conceptBonus)
               { 
                  bonus += actor->getBonusValue(*conceptBonus);
               }

               dice d20;
               diceValue = d20.roll();

               //TODO apply reflexes bonus, esquive bonus, etc, to target,
               //depending of the attack type!
               targetValue = targ->getBonusValue(*conceptAgainst);

               /* verify critical Hit */
               if(diceValue == DICE_D20)
               {
                  criticalRoll = d20.roll();
                  if( (criticalRoll + bonus - targetValue) > 0)
                  {
                     criticalHit = true;
                  }
               }
               else if(diceValue == 1)
               {
                 miss = true; 
               }

               /* Put Dice Values on Briefing */
               char txtBonus[32];
               if(bonus >= 0)
               {
                  sprintf(txtBonus,"+%d",bonus);
               }
               else
               {
                  sprintf(txtBonus,"%d",bonus);
               }

               if(criticalRoll != -1)
               {
                  sprintf(texto,"%d(%s) & (%d%s) x %d : ",diceValue,txtBonus,
                        criticalRoll,txtBonus,targetValue);
               }
               else
               {
                  sprintf(texto,"%d(%s) x %d : ",diceValue,txtBonus,
                        targetValue);
               }
               diceText = texto;

               //apply bonus (skill bonus)
               diceValue += bonus;

               /*TODO apply resistances  */

               if( ( (diceValue - targetValue <= 0 || (miss))) && 
                   (!criticalHit) )
               {
                  brief.addText(diceText + gettext("Miss."));
                  controller.addMessage(actor->scNode->getPosX(),
                        targ->scNode->getPosY()+
                        targ->scNode->getBoundingBox().max.y,
                        targ->scNode->getPosZ(),gettext("Miss."),
                        0.92,0.41,0.14);
                  return(true);
               }

            }

            /* Apply Base Damage Dices */
            damage += diceInfo.baseDice.roll(criticalHit);

            /* Apply Damage/Heal modifiers bonus */
            if(conceptBonus)
            {
               damage += actor->getBonusValue(*conceptBonus);
            }

            /* Make sure damage value is at last 1 */
            if(damage <= 0)
            {
               damage = 1;
            }

            /*TODO apply aditional dices */

            /* Verify instant killing the target */
            int instantKill = diceInfo.instantKill;
            
            dice d100(100);
            if(d100.roll() < instantKill)
            {
               /* Got an instantaneous death here! */
               damage = targ->getMaxLifePoints();
               sprintf(texto, 
                     gettext("Hit at a vital area, inflicting %d points "
                        "of damage."), damage);
            }
            else
            {
               sprintf(texto,gettext("Hit for %d points."),damage);
            }
            brief.addText(diceText + texto);

            /* apply damage on thing */
            targ->addLifePoints(-damage);

            if(criticalHit)
            {
               brief.addText(gettext("Critical Hit!"), 12, 10, 128);
               /* Show critical hit */
               controller.addMessage(targ->scNode->getPosX(),
                     targ->scNode->getPosY()+
                     targ->scNode->getBoundingBox().max.y,
                     targ->scNode->getPosZ(),gettext("Critical Hit!"),
                     0.84,0.2,0.01);
            }
            /* Show Damage */
            sprintf(texto,"%d",damage);
            controller.addMessage(targ->scNode->getPosX(), 
                  targ->scNode->getPosY() + 
                  targ->scNode->getBoundingBox().max.y,
                  targ->scNode->getPosZ(), texto,
                  0.4, 0.01, 0.03);

            /* Add Blood */
            GLfloat cs = cos(deg2Rad(targ->scNode->getAngleY()));
            GLfloat sn = sin(deg2Rad(targ->scNode->getAngleY()));

            bloodPart = pSystem.addParticle(targ->scNode->getPosX() - (sn*2),
                  targ->scNode->getPosY() + targ->bloodPosition,
                  targ->scNode->getPosZ() - (cs*2), targ->bloodFileName);

            /* Set the blood follow character if needed */
            if(targ->getThingType() == THING_TYPE_CHARACTER)
            {
               bloodPart->setFollowCharacter(targ, 
                     DNT_PARTICLE_SYSTEM_FOLLOW_HEAD);
            }

            /* Verify if character is dead */
            if(targ->getLifePoints() <= 0)
            {
               /* Character is dead! */
               targ->kill();
               sprintf(texto, gettext("%s is dead!"), targ->name.c_str());
               brief.addText(texto, 255, 144, 0);
            }

         }

         targ = (character*)targ->getNext();
      }
      l = eng->NPCs;
   }

   return(true);
}

