#include "feats.h" 
#include "actions.h"
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
 *                 applyAttackAndBreakFeat                     *
 ***************************************************************/
bool feats::applyAttackAndBreakFeat(thing& attacker, int featNumber, 
                                    thing& target, string& brief)
{
   int diceValue;
   int criticalRoll = -1;
   int damage = 0;
   int targetValue; 
   int bonus;
   int i;
   bool criticalHit = false;
   bool criticalMiss = false;
   bool miss = false;
   char texto[50];

   if( (featNumber < 0) || (featNumber >= totalFeats) )
   {
      brief += language.FEAT_INVALID;
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

      //TODO verify if can use or not based on target thing and range

      //verify Bonus
      bonus = attacker.getBonusValue(m_feats[featNumber].conceptBonus) + 
              attacker.sizeModifier + attacker.baseAttackModifier;

      /*printf("Bonus: %d = %d + %d + %d\n", bonus,attacker.getBonusValue(m_feats[featNumber].conceptBonus),attacker.sizeModifier,attacker.baseAttackModifier);
*/
      diceValue = ((rand() % DICE_D20)+1); 

      //TODO apply reflexes bonus, esquive bonus, etc, to target 
      targetValue = target.armatureClass;
   
      /* verify critical Hit */
      if(diceValue == DICE_D20)
      {
          criticalRoll = rand() % DICE_D20 +1;
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
          criticalRoll = (rand() % DICE_D20 +1);
          if( (criticalRoll + bonus - targetValue) <= 0 )
          {
             criticalMiss = true;
          }
      }

      /* Put Dice Values on Briefing */
      if(criticalRoll != -1)
      {
         sprintf(texto,"%d(+%d) & (%d+%d) x %d : ",diceValue,bonus,criticalRoll,
                                         bonus,targetValue);
      }
      else
      {
         sprintf(texto,"%d(+%d) x %d : ",diceValue,bonus,targetValue);
      }
      brief += texto;

      //apply bonus (skill bonus)
      diceValue += bonus;

      /*TODO apply resistances  */

      if( (diceValue - targetValue < 0) || (criticalMiss) || (miss) )
      {
         brief += language.FEAT_MISS;
         if( criticalMiss )
         {
             brief += "|"+language.FEAT_CRITICAL_MISS ;
             //TODO lose weapon;
         }
         return(true);
      }
      else
      {
         brief += "|"+language.FEAT_HIT_FOR+" ";
      }

      /* Apply Base Damage Dices */
      for(i = 0; i < m_feats[featNumber].diceInfo.baseDice.numberOfDices; i++)
      {
          damage+=(rand() % m_feats[featNumber].diceInfo.baseDice.diceID)+1;
      }
      /* Sum Dice Damage Number */
      damage += m_feats[featNumber].diceInfo.baseDice.sumNumber;

      /* Apply Critical Hit */
      if(criticalHit)
      {
         /* Double Base Damage Dices */
         for(i = 0; i<m_feats[featNumber].diceInfo.baseDice.numberOfDices; i++)
         {
             damage+=(rand()%m_feats[featNumber].diceInfo.baseDice.diceID)+1;
         }
         /* Sum Dice Damage Number */
         damage += m_feats[featNumber].diceInfo.baseDice.sumNumber;
      }

      

      /*TODO apply aditional dices */

      sprintf(texto,"%d",damage);
      brief += texto;
      brief += " " + language.FEAT_POINTS;

      /* apply damage on thing */
      target.lifePoints -= damage;

      if( criticalHit)
      {
          brief += "|"+ language.FEAT_CRITICAL_HIT ;
      }

      if(target.lifePoints <= 0)
      {
         //FIXME Other states, like partial death to be implemented
         target.dead = true;
      }

      return(true);
   }
   brief += language.FEAT_NO_MORE_POINTS;
   return(false);
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
int feats::getNPCAttackFeat(thing* pers, thing* target)
{
   int ft;
   //FIXME do in a better way than that random stuff;

   if( (target != NULL) && (pers != NULL))
   {
      srand(SDL_GetTicks());
      ft = (rand() % totalFeats);

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
 *                      defineMeleeWeapon                      *
 ***************************************************************/
void feats::defineMeleeWeapon(diceThing& weaponDice)
{ 
   /* Disable Ranged Attacks */
   m_feats[FEAT_RANGED_ATTACK].diceInfo.initialLevel = 0;
   /* Enable Melee Attacks */
   m_feats[FEAT_MELEE_ATTACK].diceInfo = weaponDice;
}

/***************************************************************
 *                      defineRangedWeapon                     *
 ***************************************************************/
void feats::defineRangedWeapon(diceThing& weaponDice)
{
   /* Disable Melee Attacks */
   m_feats[FEAT_MELEE_ATTACK].diceInfo.initialLevel = 0;
   /* Enable Ranged Attacks */
   m_feats[FEAT_RANGED_ATTACK].diceInfo = weaponDice;

}

/**************************************************************************
 *                               FEATS LIST                               *
 **************************************************************************/

/***************************************************************
 *                       Constructor                           *
 ***************************************************************/
featsList::featsList(string dir, string arq)
{
   FILE* file;
   string arqDescricao;
   string arqImagem;
   char buffer[1024];
   char buf2[128];
   char buf3[128];
   char buf4[128];
   int num;
   int i;
   int aux;

//   srand(SDL_GetTicks());

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
      m_feats[aux].name = buffer;
      fgets(buffer, sizeof(buffer), desc);
      m_feats[aux].description = buffer;
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
      //Read Dependent Feats
      for(i=0; i<MAX_DEP_FEATS;i++)
      {
         fscanf(desc,"%s %f,%d",&buf2[0],
                                &m_feats[aux].depFeats[i].reason,
                                &num);
         m_feats[aux].depFeats[i].featIDString = buf2;
         m_feats[aux].depFeats[i].used = num == 1;
      }
      m_feats[aux].image = IMG_Load(arqImagem.c_str());
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

