#include "feats.h" 
#include "actions.h"
#include "../engine/util.h"
#include "../particle/partcontroller.h"
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
   currentWeapon = NULL;
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
                                  thing* target, bool heal)
{
   char texto[255];
   briefing brief;

   /* Verify if the feat is valid */
   if( (featNumber < 0) || (featNumber >= totalFeats) )
   {
      brief.addText(gettext("Invalid Talent"), 255, 0, 0);
      return(false);
   }

   /* Show feature name */
   sprintf(texto,"%s ",m_feats[featNumber].name.c_str());
   brief.addText(texto);

   /* Verify if have the feat points to use it */
   if( (m_feats[featNumber].actualQuantity >= m_feats[featNumber].costToUse)
       || (m_feats[featNumber].costToUse) == 0 )
   {
      /* Try to use the feat */
      if(doHealOrAttack(actor, target, 
                        m_feats[featNumber].diceInfo, 
                        &m_feats[featNumber].conceptBonus,
                        m_feats[featNumber].range, heal))
      {
         /* Yes, we've used it */
         useFeat(featNumber);

         /* Apply Ammo for weapon, if needed */
         if( (featNumber == FEAT_RANGED_ATTACK) || 
             (featNumber == FEAT_MELEE_ATTACK) )
         {
            flushCurrentMunition();
         }

         /* Yes, used the feat! */
         return(true);
      }
      else
      {
         return(false);
      }
   }

   /* Can't use due to points! */
   brief.addText(gettext("Not enought points to use!"), 255, 10, 10);
   return(false);
}

/***************************************************************
 *                   applyHealAndFixFeat                       *
 ***************************************************************/
bool feats::applyHealAndFixFeat(thing& attacker, int featNumber, 
                                thing* target)
{
   return(applyHealOrAttackFeat(attacker, featNumber, target, true));
}

/***************************************************************
 *                     applyPsychoFeat                         *
 ***************************************************************/
bool feats::applyPsychoFeat(thing& attacker, int featNumber, 
                            thing* target)
{
   //TODO
   return(false);
}

/***************************************************************
 *                   applyInvocationFeat                       *
 ***************************************************************/
bool feats::applyInvocationFeat(thing& attacker, int featNumber, 
                                thing* target)
{
   //TODO
   return(false);
}


/***************************************************************
 *                 applyAttackAndBreakFeat                     *
 ***************************************************************/
bool feats::applyAttackAndBreakFeat(thing& attacker, int featNumber, 
                                    thing* target)
{
   return(applyHealOrAttackFeat(attacker, featNumber, target, false));
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
   power = m_feats[ft].diceInfo.baseDice.getType() * 
           m_feats[ft].diceInfo.baseDice.getNumberOfDices() +
           m_feats[ft].diceInfo.baseDice.getSumNumber();

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
         tmpPower = m_feats[i].diceInfo.baseDice.getType() * 
                    m_feats[i].diceInfo.baseDice.getNumberOfDices() +
                    m_feats[i].diceInfo.baseDice.getSumNumber();
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
      int power = m_feats[ft].diceInfo.baseDice.getType() * 
                  m_feats[ft].diceInfo.baseDice.getNumberOfDices() +
                  m_feats[ft].diceInfo.baseDice.getSumNumber();

      /* Run over all feats searching for a powerfull heal one */
      for(i = 0; i < totalFeats; i++)
      {
         /* Verify if is an attack feat and is avaible */
         if( (i != ft) && (m_feats[i].action == ACT_HEAL) && 
             ( (m_feats[i].actualQuantity >= m_feats[i].costToUse)
             || (m_feats[i].costToUse) == 0 ))
         {
            /* Verify if is powerfull */
            tmpPower = m_feats[i].diceInfo.baseDice.getType() * 
                       m_feats[i].diceInfo.baseDice.getNumberOfDices() +
                       m_feats[i].diceInfo.baseDice.getSumNumber();
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
 *                     flushCurrentMunition                    *
 ***************************************************************/
void feats::flushCurrentMunition()
{
   if(currentWeapon)
   {
      cout << "Have Weapon!" << endl;
      /* Get the actual quantity for its type */
      if(currentWeapon->getRangeType()->index == FEAT_MELEE_ATTACK)
      {
         currentWeapon->setCurrentMunition( 
                                     m_feats[FEAT_MELEE_ATTACK].actualQuantity);
      }
      else
      {
         cout << "Will set as: " << m_feats[FEAT_RANGED_ATTACK].actualQuantity
              << endl;
         currentWeapon->setCurrentMunition( 
                                    m_feats[FEAT_RANGED_ATTACK].actualQuantity);
      }
   }
}

/***************************************************************
 *                         defineWeapon                        *
 ***************************************************************/
void feats::defineWeapon(weapon* w)
{ 
   int inUse=0, noUse=0;

   /* Must update the current weapon ammo value */
   flushCurrentMunition();
   currentWeapon = w;

   /* Define if is using a Melee or Ranged Weapon */
   if( (w == NULL) || (w->getRangeType()->index == FEAT_MELEE_ATTACK) )
   {
      inUse = FEAT_MELEE_ATTACK;
      noUse = FEAT_RANGED_ATTACK;
   }
   else
   {
      inUse = FEAT_RANGED_ATTACK;
      noUse = FEAT_MELEE_ATTACK;
   }

   /* Disable "noUse" Attacks */
   m_feats[noUse].diceInfo.initialLevel = 0;
   m_feats[noUse].range = 0;

   /* Enable "inUse" Attacks */
   if(w != NULL)
   {
      m_feats[inUse].diceInfo = w->getDice();
      m_feats[inUse].range = w->getRange();      
   }
   else
   {
      /* Using bare hands */
      diceThing dc;
      dc.baseDice.setType(DICE_D2);
      dc.baseDice.setNumberOfDices(1);
      dc.baseDice.setSumNumber(0);
      dc.baseDice.setCriticalMultiplier(1);
      dc.initialLevel = 1;
      m_feats[inUse].diceInfo = dc;
      m_feats[inUse].range = (int)(WALK_PER_MOVE_ACTION * DNT_TO_METER);
   }

   /* If Ammo not None, must use it */
   if( (w != NULL) && (w->getMunitionType()->index != 0) )
   {
      m_feats[inUse].actualQuantity = w->getCurrentMunition();
      m_feats[inUse].costToUse = 1;
   }
   else
   {
      m_feats[inUse].actualQuantity = 0;
      m_feats[inUse].costToUse = 0;
   }
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
         printf("Can't open talent file: %s \n",arqDescricao.c_str() );
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

      /* Get Dices */
      int numberOfDices=0, diceID=0, sumNumber=0;

      /* Base Dice */
      fscanf(desc,"%d*d%d+%d",&numberOfDices, &diceID, &sumNumber);
      m_feats[aux].diceInfo.baseDice.setType(diceID);
      m_feats[aux].diceInfo.baseDice.setNumberOfDices(numberOfDices);
      m_feats[aux].diceInfo.baseDice.setSumNumber(sumNumber);

      /* Aditional dice */
      fscanf(desc,"%d*d%d+%d", &numberOfDices, &diceID, &sumNumber);
      m_feats[aux].diceInfo.aditionalDice.setType(diceID);
      m_feats[aux].diceInfo.aditionalDice.setNumberOfDices(numberOfDices);
      m_feats[aux].diceInfo.aditionalDice.setSumNumber(sumNumber);

      /* Aditional Quantity */
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

