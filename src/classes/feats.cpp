#include "feats.h" 
#include "actions.h"
#include "../engine/util.h"
#include "../particle/partSystem.h"
#include "../lang/translate.h"
#include "../etc/dirs.h"
#include <SDL/SDL_image.h>

/**************************************************************************
 *                                    FEAT                                *
 **************************************************************************/

/***************************************************************
 *                       Constructor                           *
 ***************************************************************/
feats::feats()
{
   totalFeats = 0;
}

/***************************************************************
 *                        Destructor                           *
 ***************************************************************/
feats::~feats()
{
   totalFeats = 0;
}

/***************************************************************
 *                       featByNumber                          *
 ***************************************************************/
feat* feats::featByNumber(int featNumber)
{
   if( (featNumber > 0) && (featNumber < totalFeats) )
   {
      return(&m_feats[featNumber]);
   }
   return(NULL);
}

/***************************************************************
 *                       featByName                            *
 ***************************************************************/
feat* feats::featByString(string featName)
{
   int i;
   for(i=0;i<totalFeats;i++)
   {
      if(featName.compare(m_feats[i].idString) == 0)
      {
         return(&m_feats[i]);
      }
   }
   return(NULL);
}

/***************************************************************
 *                       insertFeat                            *
 ***************************************************************/
bool feats::insertFeat(featDescription featInsert)
{
   int i;
   if(totalFeats < MAX_FEATS)
   {
      m_feats[totalFeats].internalListNumber = featInsert.internalListNumber; 
      m_feats[totalFeats].requeridedLevel = featInsert.requeridedLevel;
      m_feats[totalFeats].quantityPerDay = featInsert.quantityPerDay;
      m_feats[totalFeats].aditionalQuantity = featInsert.aditionalQuantity;
      m_feats[totalFeats].aditionalLevels = featInsert.aditionalLevels;
      m_feats[totalFeats].actualQuantity = featInsert.quantityPerDay;
      m_feats[totalFeats].costToUse = featInsert.costToUse;
      m_feats[totalFeats].actionType = featInsert.actionType;
      m_feats[totalFeats].action = featInsert.action;
      m_feats[totalFeats].range = featInsert.range;
      m_feats[totalFeats].name = featInsert.name;
      m_feats[totalFeats].diceInfo = featInsert.diceInfo;
      m_feats[totalFeats].conceptBonus = featInsert.conceptBonus;
      m_feats[totalFeats].conceptAgainst = featInsert.conceptAgainst;
      m_feats[totalFeats].conceptTarget = featInsert.conceptTarget;
      m_feats[totalFeats].idString = featInsert.idString;
      for(i = 0; i < MAX_DEP_FEATS; i++)
      {
         m_feats[totalFeats].depFeats[i].reason = featInsert.depFeats[i].reason;
         m_feats[totalFeats].depFeats[i].featIDString = 
                                            featInsert.depFeats[i].featIDString;
         m_feats[totalFeats].depFeats[i].used = featInsert.depFeats[i].used;
      }
      totalFeats++;
      return(true);
   }
   return(false);
}

/***************************************************************
 *                          useFeat                            *
 ***************************************************************/
void feats::useFeat(int featNumber)
{
   int i;
   feat* ft;
   m_feats[featNumber].actualQuantity--;
   for(i=0;i<MAX_DEP_FEATS;i++)
   {
       if(m_feats[featNumber].depFeats[i].used)
       {
          ft = featByString(m_feats[featNumber].depFeats[i].featIDString);
          ft->actualQuantity -= 1.0 / m_feats[featNumber].depFeats[i].reason;
       }
   }
}

/***************************************************************
 *                   applyHealOrAttackFeat                     *
 ***************************************************************/
bool feats::applyHealOrAttackFeat(thing& actor, int featNumber, 
                                  thing& target, string& brief,
                                  messageController* controller,
                                  void* pSystem, bool heal)
{
   int diceValue;
   int criticalRoll = -1;
   int damage = 0;
   int targetValue; 
   int bonus;
   int i,j;
   bool criticalHit = false;
   bool criticalMiss = false;
   bool miss = false;
   char texto[1024];

   if( (featNumber < 0) || (featNumber >= totalFeats) )
   {
      brief += gettext("Invalid Feat");
      return(false);
   }

   if(!actionInRange(actor.xPosition, actor.zPosition, 
                     target.xPosition, target.zPosition,
                     m_feats[featNumber].range*METER_TO_DNT))
   {
      brief += gettext("Too far away for action!");
      return(false);
   }

   sprintf(texto,"%s ",m_feats[featNumber].name.c_str());
   brief += texto;

   srand(SDL_GetTicks());

   if( (m_feats[featNumber].actualQuantity >= m_feats[featNumber].costToUse)
       || (m_feats[featNumber].costToUse) == 0 )
   {
      /* apply Costs */
      useFeat(featNumber);

      /* Call the animation */
      //TODO call other animation, if is defined
      if(!heal)
      {
         actor.callAttackAnimation();
      }
      //TODO -> create an heal animation!


      //TODO verify if can use or not based on target thing


      //verify Bonus
      bonus = actor.getBonusValue(m_feats[featNumber].conceptBonus) + 
              actor.sizeModifier + actor.baseAttackModifier;

      diceValue = 1 + (int)(DICE_D20*(rand() / (RAND_MAX + 1.0))); 

      //TODO apply reflexes bonus, esquive bonus, etc, to target 
      targetValue = target.armatureClass;
   
      /* verify critical Hit */
      if(diceValue == DICE_D20)
      {
          criticalRoll = 1 + (int)(DICE_D20*(rand() / (RAND_MAX + 1.0)));
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
          criticalRoll = 1 + (int)(DICE_D20*(rand() / (RAND_MAX + 1.0)));
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
      brief += texto;
      brief += "|";

      //apply bonus (skill bonus)
      diceValue += bonus;

      /*TODO apply resistances  */

      if( (diceValue - targetValue <= 0) || (criticalMiss) || (miss) )
      {
         brief += gettext("Miss.");
         if( criticalMiss )
         {
             brief += "|";
             brief += gettext("Critical Miss!");
             controller->addMessage(actor.xPosition,
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
            controller->addMessage(actor.xPosition,
                                   actor.yPosition+actor.max[1],
                                   actor.zPosition,gettext("Miss."),
                                   0.92,0.41,0.14);
         }
         return(true);
      }

      /* Apply Base Damage Dices */
      for(i = 0; i < m_feats[featNumber].diceInfo.baseDice.numberOfDices; i++)
      {
          damage += (1 + (int)(m_feats[featNumber].diceInfo.baseDice.diceID *
                        (rand() / (RAND_MAX + 1.0))));
      }
      /* Sum Dice Damage Number */
      damage += m_feats[featNumber].diceInfo.baseDice.sumNumber;

      /* Apply Critical Hit */
      if(criticalHit)
      {
         /* Throws the dice by the number of critical multipliers */
         for(j = 0; j<m_feats[featNumber].diceInfo.baseDice.criticalMultiplier; 
             j++)
         {
            /* Double Base Damage Dices */
            for(i = 0; i<m_feats[featNumber].diceInfo.baseDice.numberOfDices; 
                i++)
            {
                damage += (1 + 
                           (int)(m_feats[featNumber].diceInfo.baseDice.diceID *
                           (rand() / (RAND_MAX + 1.0))));
            }
            /* Sum Dice Damage Number */
            damage += m_feats[featNumber].diceInfo.baseDice.sumNumber;
         }
      }

      

      /*TODO apply aditional dices */

      if(heal)
      {
         sprintf(texto,gettext("Healed %d points."),damage);
      }
      else
      {
         sprintf(texto,gettext("Hit for %d points."),damage);
      }
      brief += texto;

      if(heal)
      {
         /* apply the heal */
         target.lifePoints += damage;
         /* Verify Limits */
         if(target.lifePoints > target.maxLifePoints)
         {
            target.lifePoints = target.maxLifePoints;
         }
      }
      else
      {
         /* apply damage on thing */
         target.lifePoints -= damage;
      }

      if( criticalHit)
      {
          brief += "|";
          if(heal)
          {
             brief += gettext("Critical Heal!");
             controller->addMessage(actor.xPosition,
                                    actor.yPosition+actor.max[1],
                                    actor.zPosition,gettext("Critical Heal!"),
                                    0.06,0.04,0.56);
          }
          else
          {
             brief += gettext("Critical Hit!");
             /* Show critical hit */
             controller->addMessage(actor.xPosition,
                                    actor.yPosition+actor.max[1],
                                    actor.zPosition,gettext("Critical Hit!"),
                                    0.54,0.0,0.0);
          }
      }
      /* Show Damage */
      sprintf(texto,"%d",damage);
      if(heal)
      {
         controller->addMessage(target.xPosition, 
                                target.yPosition + target.max[1],
                                target.zPosition, texto,
                                0.21, 0.15, 0.7);
      } 
      else
      {
         controller->addMessage(target.xPosition, 
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
         GLfloat cs = cos(deg2Rad(target.orientation));
         GLfloat sn = sin(deg2Rad(target.orientation));
         partSystem* ps = (partSystem*)pSystem;
         ps->addParticle(PART_BLOOD, target.xPosition - (sn*2),
                         target.yPosition + target.bloodPosition,
                         target.zPosition - (cs*2), 
                         target.bloodFileName);
      }

      return(true);
   }
   brief += gettext("Not enought points to use!");
   return(false);
}

/***************************************************************
 *                   applyHealAndFixFeat                       *
 ***************************************************************/
bool feats::applyHealAndFixFeat(thing& attacker, int featNumber, 
                                thing& target, string& brief,
                                messageController* controller,
                                void* pSystem)
{
   return(applyHealOrAttackFeat(attacker, featNumber, target,
                                brief, controller, pSystem,
                                true));
}

/***************************************************************
 *                     applyPsychoFeat                         *
 ***************************************************************/
bool feats::applyPsychoFeat(thing& attacker, int featNumber, 
                            thing& target, string& brief,
                            messageController* controller)
{
   //TODO
   return(false);
}

/***************************************************************
 *                   applyInvocationFeat                       *
 ***************************************************************/
bool feats::applyInvocationFeat(thing& attacker, int featNumber, 
                                thing& target, string& brief,
                                messageController* controller)
{
   //TODO
   return(false);
}


/***************************************************************
 *                 applyAttackAndBreakFeat                     *
 ***************************************************************/
bool feats::applyAttackAndBreakFeat(thing& attacker, int featNumber, 
                                    thing& target, string& brief,
                                    messageController* controller,
                                    void* pSystem)
{
   return(applyHealOrAttackFeat(attacker, featNumber, target,
                                brief, controller, pSystem,
                                false));
}

/***************************************************************
 *                         New Day                             *
 ***************************************************************/
void feats::newDay()
{
   int i;
   for(i=0;i<totalFeats;i++)
   {
      m_feats[i].actualQuantity = m_feats[i].quantityPerDay;
   }
}

/***************************************************************
 *                      getNPCAttackFeat                       *
 ***************************************************************/
int feats::getRandomNPCAttackFeat(thing* pers, thing* target)
{
   int ft;

   if( (target != NULL) && (pers != NULL))
   {
      srand(SDL_GetTicks());
      ft = (int)(totalFeats*(rand() / (RAND_MAX + 1.0)));

      //FIXME verify if the feat is in range to use!

      if( (ft != FEAT_RANGED_ATTACK) && (ft != FEAT_MELEE_ATTACK) &&
          (m_feats[ft].action == ACT_ATTACK)  && 
          ( (m_feats[ft].actualQuantity >= m_feats[ft].costToUse)
          || (m_feats[ft].costToUse) == 0 ))
      {
          /* is avaible */
          return(ft);
      }
      else
      {
          /* otherwise, use base attack (melee or ranged) */
          if(m_feats[FEAT_RANGED_ATTACK].diceInfo.initialLevel == 0)
          {
              return(FEAT_MELEE_ATTACK);
          }
          else
          {
             return(FEAT_RANGED_ATTACK);
          }
      }
   }
   
   return(-1);
}

/***************************************************************
 *                   getPowerFullAttackFeat                    *
 ***************************************************************/
int feats::getPowerfullAttackFeat(thing* pers, thing* target)
{
   int ft = -1;
   int power = 0;
   int tmpPower = 0;
   int i;

   /* FIXME test range of the feats! */

   /* FIXME calculate the power with the level of the user and the
    * aditional levels dices! */

   /* Take the initial feat */
   if(m_feats[FEAT_RANGED_ATTACK].diceInfo.initialLevel == 0)
   {
      ft = FEAT_MELEE_ATTACK;
   }
   else
   {
      ft = FEAT_RANGED_ATTACK;
   }
   power = m_feats[ft].diceInfo.baseDice.diceID * 
           m_feats[ft].diceInfo.baseDice.numberOfDices +
           m_feats[ft].diceInfo.baseDice.sumNumber;

   /* Run over all feats searching for a powerfull one */
   for(i = 0; i < totalFeats; i++)
   {
      /* Verify if is an attack feat and is avaible */
      if( (i != ft) && (i != FEAT_RANGED_ATTACK) && 
          (i != FEAT_MELEE_ATTACK) && (m_feats[i].action == ACT_ATTACK) && 
          ( (m_feats[i].actualQuantity >= m_feats[i].costToUse)
          || (m_feats[i].costToUse) == 0 ))
      {
         /* verify if is powerfull */
         tmpPower = m_feats[i].diceInfo.baseDice.diceID * 
                    m_feats[i].diceInfo.baseDice.numberOfDices +
                    m_feats[i].diceInfo.baseDice.sumNumber;
         if(tmpPower > power)
         {
            /* Is powerfull, take the feat */
            ft = i;
            power = tmpPower;
         }
      }
   }

   return(ft);
}

/***************************************************************
 *                       getFirstHealFeat                      *
 ***************************************************************/
int feats::getFirstHealFeat(thing* pers)
{
   int i = -1;

   if(pers)
   {
      /* Run over all feats searching for a heal one */
      for(i = 0; i < totalFeats; i++)
      {
         /* Verify if is an attack feat and is avaible */
         if( (m_feats[i].action == ACT_HEAL) && 
             ( (m_feats[i].actualQuantity >= m_feats[i].costToUse)
             || (m_feats[i].costToUse) == 0 ))
         {
            return(i);
         }
      }
   }

   return(-1);
}

/***************************************************************
 *                     getRandomHealFeat                       *
 ***************************************************************/
int feats::getRandomHealFeat(thing* pers)
{
   int ft;

   if((pers != NULL))
   {
      srand(SDL_GetTicks());
      ft = (int)(totalFeats*(rand() / (RAND_MAX + 1.0)));

      if( (m_feats[ft].action == ACT_HEAL)  && 
          ( (m_feats[ft].actualQuantity >= m_feats[ft].costToUse)
          || (m_feats[ft].costToUse) == 0 ))
      {
          /* is avaible */
          return(ft);
      }
   }
   return(getFirstHealFeat(pers));
}

/***************************************************************
 *                     getPowerfullHealFeat                    *
 ***************************************************************/
int feats::getPowerfullHealFeat(thing* pers)
{
   int ft = getFirstHealFeat(pers);
   int i = 0;

   if( (pers) && (ft != -1) )
   {
      int tmpPower = 0;
      int power = m_feats[ft].diceInfo.baseDice.diceID * 
                  m_feats[ft].diceInfo.baseDice.numberOfDices +
                  m_feats[ft].diceInfo.baseDice.sumNumber;

      /* Run over all feats searching for a powerfull heal one */
      for(i = 0; i < totalFeats; i++)
      {
         /* Verify if is an attack feat and is avaible */
         if( (i != ft) && (m_feats[i].action == ACT_HEAL) && 
             ( (m_feats[i].actualQuantity >= m_feats[i].costToUse)
             || (m_feats[i].costToUse) == 0 ))
         {
            /* Verify if is powerfull */
            tmpPower = m_feats[i].diceInfo.baseDice.diceID * 
                       m_feats[i].diceInfo.baseDice.numberOfDices +
                       m_feats[i].diceInfo.baseDice.sumNumber;
            if(tmpPower > power)
            {
               power = tmpPower;
               ft = i;
            }
         }
      }
   }

   return(ft);
}

/***************************************************************
 *                   getAttackFeatRangeType                    *
 ***************************************************************/
int feats::getAttackFeatRangeType()
{
   if(m_feats[FEAT_RANGED_ATTACK].diceInfo.initialLevel == 0)
   {
      return(FEAT_MELEE_ATTACK);
   }
   return(FEAT_RANGED_ATTACK);
}

/***************************************************************
 *                     getAttackFeatRange                      *
 ***************************************************************/
int feats::getAttackFeatRange()
{
   int value =  m_feats[FEAT_RANGED_ATTACK].range;
   if(m_feats[FEAT_RANGED_ATTACK].diceInfo.initialLevel == 0)
   {
      value = m_feats[FEAT_MELEE_ATTACK].range;
   }
   return(value);
}

/***************************************************************
 *                      defineMeleeWeapon                      *
 ***************************************************************/
void feats::defineMeleeWeapon(diceThing& weaponDice, int rangeValue)
{ 
   /* Disable Ranged Attacks */
   m_feats[FEAT_RANGED_ATTACK].diceInfo.initialLevel = 0;
   m_feats[FEAT_RANGED_ATTACK].range = 0;
   /* Enable Melee Attacks */
   m_feats[FEAT_MELEE_ATTACK].diceInfo = weaponDice;
   m_feats[FEAT_MELEE_ATTACK].range = rangeValue;
}

/***************************************************************
 *                      defineRangedWeapon                     *
 ***************************************************************/
void feats::defineRangedWeapon(diceThing& weaponDice, int rangeValue)
{
   /* Disable Melee Attacks */
   m_feats[FEAT_MELEE_ATTACK].diceInfo.initialLevel = 0;
   m_feats[FEAT_MELEE_ATTACK].range = 0;
   /* Enable Ranged Attacks */
   m_feats[FEAT_RANGED_ATTACK].diceInfo = weaponDice;
   m_feats[FEAT_RANGED_ATTACK].range = rangeValue;
}

/**************************************************************************
 *                               FEATS LIST                               *
 **************************************************************************/

/***************************************************************
 *                       Constructor                           *
 ***************************************************************/
featsList::featsList(string dir, string arq)
{
   dirs dirInfo;
   FILE* file;
   string arqDescricao;
   string arqImagem;
   char buffer[1024];
   char buf2[256];
   char buf3[256];
   char buf4[256];
   int num;
   int i;
   int aux;

   if(!(file=fopen(arq.c_str(),"r")))
   {
       printf("Error while opening feats list: %s\n",arq.c_str());
       return;
   }

   fgets(buffer, sizeof(buffer), file);
   sscanf(buffer, "%d", &totalFeats);

   if(totalFeats > 0)
   {
      m_feats = new featDescription[totalFeats];
   }
   else
   {
      m_feats = NULL;
   }

   
   for(aux = 0; aux < totalFeats; aux++)
   {
      fgets(buffer, sizeof(buffer), file);
      sscanf(buffer,"%d %s %s %s",&num,&buf2[0],&buf3[0], &buf4[0]);
      arqImagem = buf3;
      arqDescricao = buf2;
      arqDescricao = dir+arqDescricao;
      m_feats[aux].idString = buf4;

      FILE* desc;
      if(! (desc = fopen(arqDescricao.c_str(), "r")))
      {
         printf("Can't open feat file: %s \n",arqDescricao.c_str() );
         return;
      }
      m_feats[aux].internalListNumber = aux;
      fgets(buffer, sizeof(buffer), desc);
      m_feats[aux].name = translateDataString(buffer);
      fgets(buffer, sizeof(buffer), desc);
      m_feats[aux].description = translateDataString(buffer);
      fscanf(desc,"%d",&m_feats[aux].requeridedLevel);
      fscanf(desc,"%s %s",&buffer[0], &buf2[0]);
      m_feats[aux].requeridedFactor.type = buffer;
      m_feats[aux].requeridedFactor.id = buf2;
      fscanf(desc,"%s %s",&buffer[0], &buf2[0]);
      m_feats[aux].conceptBonus.type = buffer;
      m_feats[aux].conceptBonus.id = buf2;
      fscanf(desc,"%s %s",&buffer[0], &buf2[0]);
      m_feats[aux].conceptAgainst.type = buffer;
      m_feats[aux].conceptAgainst.id = buf2;
      fscanf(desc,"%s %s",&buffer[0], &buf2[0]);
      m_feats[aux].conceptTarget.type = buffer;
      m_feats[aux].conceptTarget.id = buf2;
      fscanf(desc,"%d*d%d+%d",&m_feats[aux].diceInfo.baseDice.numberOfDices,
                              &m_feats[aux].diceInfo.baseDice.diceID,
                              &m_feats[aux].diceInfo.baseDice.sumNumber);
      fscanf(desc,"%d*d%d+%d",
                             &m_feats[aux].diceInfo.aditionalDice.numberOfDices,
                             &m_feats[aux].diceInfo.aditionalDice.diceID,
                             &m_feats[aux].diceInfo.aditionalDice.sumNumber);
      fscanf(desc,"%d,%d,%d",&m_feats[aux].quantityPerDay,
                             &m_feats[aux].aditionalQuantity,
                             &m_feats[aux].aditionalLevels);
      fscanf(desc,"%d %s %s",&m_feats[aux].costToUse,
                             &buf2[0],&buf3[0]);
      m_feats[aux].actionType = numberActionType(buf2);
      m_feats[aux].action = numberAction(buf3);
      fscanf(desc,"%d",&m_feats[aux].range);
      //Read Dependent Feats
      for(i=0; i<MAX_DEP_FEATS;i++)
      {
         fscanf(desc,"%s %f,%d",&buf2[0],
                                &m_feats[aux].depFeats[i].reason,
                                &num);
         m_feats[aux].depFeats[i].featIDString = buf2;
         m_feats[aux].depFeats[i].used = num == 1;
      }
      m_feats[aux].image = IMG_Load(dirInfo.getRealFile(arqImagem).c_str());
      fclose(desc);
   }

   fclose(file);
}

/***************************************************************
 *                        Destructor                           *
 ***************************************************************/
featsList::~featsList()
{
   if(m_feats)
   {
      delete[] m_feats;
   }
}

/***************************************************************
 *                        featByName                           *
 ***************************************************************/
featDescription featsList::featByString(string featName)
{
   int i;
   for(i=0; i < totalFeats; i++)
   {
      if(featName == m_feats[i].idString)
      {
         return(m_feats[i]);
      }
   }
   return(m_feats[0]);
}

/***************************************************************
 *                        featByNumber                         *
 ***************************************************************/
featDescription featsList::featByNumber(int featNumber)
{
   if( (featNumber > 0) && (featNumber < totalFeats) )
   {
      return(m_feats[featNumber]);
   }
   return(m_feats[0]);
}

