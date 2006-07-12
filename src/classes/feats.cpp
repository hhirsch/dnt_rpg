#include "feats.h" 
#include "defs.h"
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
feat* feats::featByName(string featName)
{
   int i;
   for(i=0;i<totalFeats;i++)
   {
      if(featName.compare(m_feats[i].name) == 0)
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
      for(i = 0; i < MAX_DEP_FEATS; i++)
      {
         m_feats[totalFeats].depFeats[i].reason = featInsert.depFeats[i].reason;
         m_feats[totalFeats].depFeats[i].featName = 
                                                featInsert.depFeats[i].featName;
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
          ft = featByName(m_feats[featNumber].depFeats[i].featName);
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
   char texto[50];

   if( (featNumber < 0) || (featNumber >= totalFeats) )
   {
      brief += "Invalid Feat!";
      return(false);
   }

   sprintf(texto,"%s ",m_feats[featNumber].name.c_str());
   brief += texto;

   srand48(SDL_GetTicks());

   if( (m_feats[featNumber].actualQuantity >= m_feats[featNumber].costToUse)
       || (m_feats[featNumber].costToUse) == 0 )
   {
      /* apply Costs */
      useFeat(featNumber);

      //TODO verify if can use or not based on target thing and range

      //verify Bonus
      bonus = attacker.getBonusValue(m_feats[featNumber].conceptBonus) + 
              attacker.sizeModifier + attacker.baseAttackModifier;

      diceValue = ((lrand48() % DICE_D20)+1); 

      //TODO apply reflexes bonus, esquive bonus, etc, to target 
      targetValue = target.armatureClass;
   
      /* verify critical Hit */
      if(diceValue == DICE_D20)
      {
          criticalRoll = lrand48() % DICE_D20 +1;
          if( (criticalRoll + bonus - targetValue) > 0)
          {
              criticalHit = true;
          }
      }
      else
      /* verify critical Miss */
      if( diceValue == 1)  
      {
          criticalRoll = (lrand48() % DICE_D20 +1);
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

      if(diceValue - targetValue <= 0)
      {
         brief += "Miss.";
         if( criticalMiss )
         {
             brief += " Critical Miss!" ;
             //TODO lose weapon;
         }
         return(true);
      }
      else
      {
         brief += "Hit for ";
      }

      /* Apply Base Damage Dices */
      for(i = 0; i < m_feats[featNumber].diceInfo.baseDice.numberOfDices; i++)
      {
          damage+=(lrand48() % m_feats[featNumber].diceInfo.baseDice.diceID)+1;
      }
      /* Sum Dice Damage Number */
      damage += m_feats[featNumber].diceInfo.baseDice.sumNumber;

      /* Apply Critical Hit */
      if(criticalHit)
      {
         /* Double Base Damage Dices */
         for(i = 0; i<m_feats[featNumber].diceInfo.baseDice.numberOfDices; i++)
         {
             damage+=(lrand48()%m_feats[featNumber].diceInfo.baseDice.diceID)+1;
         }
         /* Sum Dice Damage Number */
         damage += m_feats[featNumber].diceInfo.baseDice.sumNumber;
      }

      

      /*TODO apply aditional dices */

      sprintf(texto,"%d points.",damage);
      brief += texto;

      /* apply damage on thing */
      target.lifePoints -= damage;

      if( criticalHit)
      {
          brief += " Critical Hit!" ;
      }

      if(target.lifePoints <= 0)
      {
         //FIXME Other states, like partial death to be implemented
         target.dead = true;
      }

      return(true);
   }
   brief += "Not Enough Points to Use!";
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
      srand48(SDL_GetTicks());
      ft = (lrand48() % totalFeats);

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
   int num;
   int i;
   int aux;

//   srand(SDL_GetTicks());

   if(!(file=fopen(arq.c_str(),"r")))
   {
       printf("Error while opening feats list: %s\n",arq.c_str());
       return;
   }
   
   for(aux = 0; aux < NUMBER_OF_FEATS; aux++)
   {
      //fscanf(file,"%d %s",&num,&buffer[0]);
      fgets(buffer, sizeof(buffer), file);
      sscanf(buffer,"%d %s %s",&num,&buf2[0],&buf3[0]);
      arqImagem = buf3;
      arqDescricao = buf2;
      arqDescricao = dir+arqDescricao;

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
      fscanf(desc,"%s",&buffer[0]);
      m_feats[aux].requeridedClass = numberConstant(buffer);
      fscanf(desc,"%s",&buffer[0]);
      m_feats[aux].conceptBonus = numberConstant(buffer);
      fscanf(desc,"%s",&buffer[0]);
      m_feats[aux].conceptAgainst = numberConstant(buffer);
      fscanf(desc,"%s",&buffer[0]);
      m_feats[aux].conceptTarget = numberConstant(buffer);
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
         m_feats[aux].depFeats[i].featName = buf2;
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
}

/***************************************************************
 *                        featByName                           *
 ***************************************************************/
featDescription featsList::featByName(string featName)
{
   int i;
   for(i=0; i < NUMBER_OF_FEATS; i++)
   {
      if(featName.compare(m_feats[i].name) == 0)
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
   if( (featNumber > 0) && (featNumber < NUMBER_OF_FEATS) )
   {
      return(m_feats[featNumber]);
   }
   return(m_feats[0]);
}

