#include "actions.h"
#include "../engine/briefing.h"
#include "../engine/util.h"
#include "../etc/message3d.h"
#include "../particle/partcontroller.h"

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
bool doHealOrAttack(thing& actor, thing& target, 
                    diceThing diceInfo, factor* conceptBonus,
                    int range, void* pSystem, bool heal)
{
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

   /* Show try brief */
   if(!heal)
   {
      sprintf(texto, gettext("%s try to attack %s"), 
                     actor.name.c_str(), target.name.c_str());
   }
   else
   {
      sprintf(texto, gettext("%s try to heal %s"), 
                     actor.name.c_str(), target.name.c_str());
   }
   brief.addText(texto);

   /* Verify Action Range */
   if(!actionInRange(actor.xPosition, actor.zPosition, 
                     target.xPosition, target.zPosition,
                     range*METER_TO_DNT))
   {
      brief.addText(gettext("Too far away for action!"), 225, 20, 20);
      return(false);
   }

   srand(SDL_GetTicks());

   /* Call the animation */
   //TODO call other animation, if is defined
   if(!heal)
   {
      actor.callAttackAnimation();
   }
   //TODO -> create an heal animation!


   //TODO verify if can use or not based on target thing


   /* Apply Bonuses */
   bonus = actor.sizeModifier + actor.baseAttackModifier;
   if(conceptBonus)
   { 
      bonus += actor.getBonusValue(*conceptBonus);
   }

   dice d20;
   diceValue = d20.roll();

   //TODO apply reflexes bonus, esquive bonus, etc, to target 
   targetValue = target.armatureClass;

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

   if( (diceValue - targetValue <= 0) || (criticalMiss) || (miss) )
   {
      brief.addText(diceText + gettext("Miss."));
      if( criticalMiss )
      {
         brief.addText(gettext("Critical Miss!"), 220, 0, 0);
         controller.addMessage(actor.xPosition,
               actor.yPosition+actor.max[1],
               actor.zPosition,
               gettext("Critical Miss!"),
               0.92,0.41,0.14);
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
         controller.addMessage(actor.xPosition,
               actor.yPosition+actor.max[1],
               actor.zPosition,gettext("Miss."),
               0.92,0.41,0.14);
      }
      return(true);
   }

   /* Apply Base Damage Dices */
   damage += diceInfo.baseDice.roll(criticalHit);

   /*TODO apply aditional dices */

   if(heal)
   {
      sprintf(texto,gettext("Healed %d points."),damage);
   }
   else
   {
      sprintf(texto,gettext("Hit for %d points."),damage);
   }
   brief.addText(diceText + texto);

   if(heal)
   {
      /* apply the heal */
      target.addLifePoints(damage);
   }
   else
   {
      /* apply damage on thing */
      target.addLifePoints(-damage);
   }

   if( criticalHit)
   {
      if(heal)
      {
         brief.addText(gettext("Critical Heal!"), 12, 10, 128);
         controller.addMessage(actor.xPosition,
               actor.yPosition+actor.max[1],
               actor.zPosition,gettext("Critical Heal!"),
               0.06,0.04,0.56);
      }
      else
      {
         brief.addText(gettext("Critical Hit!"), 12, 10, 128);
         /* Show critical hit */
         controller.addMessage(actor.xPosition,
               actor.yPosition+actor.max[1],
               actor.zPosition,gettext("Critical Hit!"),
               0.54,0.0,0.0);
      }
   }
   /* Show Damage */
   sprintf(texto,"%d",damage);
   if(heal)
   {
      controller.addMessage(target.xPosition, 
            target.yPosition + target.max[1],
            target.zPosition, texto,
            0.21, 0.15, 0.7);
   } 
   else
   {
      controller.addMessage(target.xPosition, 
            target.yPosition + target.max[1],
            target.zPosition, texto,
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
      if(pSystem)
      {
         GLfloat cs = cos(deg2Rad(target.orientation));
         GLfloat sn = sin(deg2Rad(target.orientation));
         partController* ps = (partController*)pSystem;
         ps->addParticle(PART_BLOOD, target.xPosition - (sn*2),
                         target.yPosition + target.bloodPosition,
                         target.zPosition - (cs*2), 
                         target.bloodFileName);
      }
   }

   return(true);
}


