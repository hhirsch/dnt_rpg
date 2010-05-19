/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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

#include "feats.h" 
#include "actions.h"
#include "../engine/util.h"
#include "../particle/partcontroller.h"
#include "../lang/translate.h"
#include "../etc/dirs.h"
#include "../etc/defparser.h"
#include <SDL/SDL_image.h>

/**************************************************************************
 *                            FEAT_DESCRIPTION                            *
 **************************************************************************/

/***************************************************************
 *                       Constructor                           *
 ***************************************************************/
featDescription::featDescription()
{
   int i;

   internalListNumber = 0;
   requeridedLevel = 0;
   quantityPerDay = 0;
   aditionalQuantity = 0;
   aditionalLevels = 0;
   costToUse = 0;
   actionType = ACT_TYPE_NORMAL_ACTION;
   action = ACT_ATTACK;
   range = 0;
   name = "undefined";
   idString = "unknow";
   description = "undefined";
   image = NULL;
   effects = new modEffectList();

   for(i = 0; i < MAX_DEP_FEATS; i++)
   {
      depFeats[i].used = false;
   }      
}

/***************************************************************
 *                        Destructor                           *
 ***************************************************************/
featDescription::~featDescription()
{
   delete(effects);
}

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
   bareHandsDice.baseDice.setType(DICE_D2);
   bareHandsDice.baseDice.setNumberOfDices(1);
   bareHandsDice.baseDice.setSumNumber(0);
   bareHandsDice.baseDice.setCriticalMultiplier(1);
   bareHandsDice.initialLevel = 1;
}

/***************************************************************
 *                      setBareHandsDamage                     *
 ***************************************************************/
void feats::setBareHandsDamage(int dices, int diceId, int sum, int crit)
{
   bareHandsDice.baseDice.setType(diceId);
   bareHandsDice.baseDice.setNumberOfDices(dices);
   bareHandsDice.baseDice.setSumNumber(sum);
   bareHandsDice.baseDice.setCriticalMultiplier(crit);
   bareHandsDice.initialLevel = 1;
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
   if( (featNumber >= 0) && (featNumber < totalFeats) )
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
      if(featName.compare(m_feats[i].info->idString) == 0)
      {
         return(&m_feats[i]);
      }
   }
   return(NULL);
}

/***************************************************************
 *                       insertFeat                            *
 ***************************************************************/
bool feats::insertFeat(featDescription* featInsert)
{
   if( (totalFeats < MAX_FEATS) && (featInsert != NULL) )
   {
      m_feats[totalFeats].info = featInsert;
      m_feats[totalFeats].range = featInsert->range;
      m_feats[totalFeats].costToUse = featInsert->costToUse;
      m_feats[totalFeats].actualQuantity = featInsert->quantityPerDay;
      m_feats[totalFeats].diceInfo = featInsert->diceInfo;
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
       if(m_feats[featNumber].info->depFeats[i].used)
       {
          ft = featByString(m_feats[featNumber].info->depFeats[i].featIDString);
          ft->actualQuantity -= 1.0 / 
                                m_feats[featNumber].info->depFeats[i].reason;
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

   /* Get Ammo from weapon, if needed */
   if( (featNumber == FEAT_RANGED_ATTACK) || 
       (featNumber == FEAT_MELEE_ATTACK) )
   {
      defineWeapon(currentWeapon);
   }

   /* Verify if have the feat points to use it */
   if( (m_feats[featNumber].actualQuantity >= 
        m_feats[featNumber].costToUse) ||
       (m_feats[featNumber].costToUse) == 0 )
   {
      /* Show feature name */
      sprintf(texto,"%s ",m_feats[featNumber].info->name.c_str());
      brief.addText(texto);

      /* Try to use the feat */
      if(doHealOrAttack(actor, target, 
                        m_feats[featNumber].diceInfo, 
                        &m_feats[featNumber].info->conceptBonus,
                        m_feats[featNumber].range, heal))
      {
         /* Yes, we've used it */
         useFeat(featNumber);

         /* Apply Ammo for weapon, if needed */
         if( (featNumber == FEAT_RANGED_ATTACK) || 
             (featNumber == FEAT_MELEE_ATTACK) )
         {
            flushCurrentMunition();
            /* Do the sound realted to the weapon, if one */
            if(currentWeapon)
            {
               currentWeapon->playMainAttackSound(actor.xPosition, 
                  actor.yPosition, actor.zPosition);
            }
         }

         /* Yes, used the feat! */
         return(true);
      }
      else
      {
         return(false);
      }
   }

   if((featNumber == FEAT_RANGED_ATTACK) || (featNumber == FEAT_MELEE_ATTACK))
   {
      /* Can't use due to ammo! */
      brief.addText(gettext("Out of ammo!"), 255, 10, 10);
   }
   else
   {
      /* Can't use due to points! */
      brief.addText(gettext("Not enought points to use!"), 255, 10, 10);
   }
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
      m_feats[i].actualQuantity = m_feats[i].info->quantityPerDay;
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
          (m_feats[ft].info->action == ACT_ATTACK)  && 
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
          (i != FEAT_MELEE_ATTACK) && 
          (m_feats[i].info->action == ACT_ATTACK) && 
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
         if( (m_feats[i].info->action == ACT_HEAL) && 
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

      if( (m_feats[ft].info->action == ACT_HEAL)  && 
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
         if( (i != ft) && (m_feats[i].info->action == ACT_HEAL) && 
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
      /* Get the actual quantity for its type */
      if(currentWeapon->getRangeType()->index == FEAT_MELEE_ATTACK)
      {
         currentWeapon->setCurrentMunition( 
                                     m_feats[FEAT_MELEE_ATTACK].actualQuantity);
      }
      else
      {
         currentWeapon->setCurrentMunition( 
                                    m_feats[FEAT_RANGED_ATTACK].actualQuantity);
      }
   }
}

/***************************************************************
 *                      getCurrentWeapon                       *
 ***************************************************************/
weapon* feats::getCurrentWeapon()
{
   return(currentWeapon);
}

/***************************************************************
 *                         defineWeapon                        *
 ***************************************************************/
void feats::defineWeapon(weapon* w)
{ 
   int inUse=0, noUse=0;

   /* Must update the current weapon ammo value */
   if(currentWeapon != w)
   {
      flushCurrentMunition();
      currentWeapon = w;
   }

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
      m_feats[inUse].diceInfo = bareHandsDice;
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
void featsList::init(string dir, string arq)
{
   dirs dirInfo;
   ifstream file;
   string arqDescricao;
   string arqImagem;
   char buf2[256];
   char buf3[256];
   string strBuffer;
   int aux;

   file.open(arq.c_str(), ios::in | ios::binary);
   if(!file)
   {
       cerr << "Error while opening feats list: " << arq << endl;
       return;
   }

   getline(file, strBuffer);
   sscanf(strBuffer.c_str(), "%d", &totalFeats);

   if(totalFeats > 0)
   {
      m_feats = new featDescription[totalFeats];
   }
   else
   {
      m_feats = NULL;
   }

   /* Temp variables */
   string key="", value="";
   int numberOfDices=0, diceID=0, sumNumber=0;
   int curDepFeat=0;

   /* Get all Feats from file */
   for(aux = 0; aux < totalFeats; aux++)
   {
      getline(file, strBuffer);
      sscanf(strBuffer.c_str(),"%s %s",&buf2[0],&buf3[0]);
      arqImagem = buf3;
      arqDescricao = buf2;
      arqDescricao = dir+arqDescricao;

      curDepFeat = 0;

      /* Open the feat's description file */
      defParser desc;
      if(!desc.load(arqDescricao))
      {
         cerr << "Can't open talent file: " << arqDescricao << endl;
         return;
      }
      m_feats[aux].internalListNumber = aux;

      /* Now parse all feat tuples */
      while(desc.getNextTuple(key, value))
      {
         /* Text Keys */
         if(key == "title")
         {
            /* Feat Title */
            m_feats[aux].name = translateDataString(value);
         }
         else if(key == "description")
         {
            /* Feat Description Text */
            m_feats[aux].description = translateDataString(value);
         }

         /* Identifiers */
         else if(key == "id")
         {
            m_feats[aux].idString = value;
         }

         /* Pre-Requistes */
         else if(key == "requerided")
         {
            /* Requerided factor  */
            sscanf(value.c_str(), "%s %s", &buf2[0], &buf3[0]);
            m_feats[aux].requeridedFactor.type = buf2;
            m_feats[aux].requeridedFactor.id = buf3;
         }
         else if(key == "reqValue")
         {
            /* requerided value (level) */
            sscanf(value.c_str(),"%d",&m_feats[aux].requeridedLevel);
         }
         else if(key == "range")
         {
            /* Feat Range */
            sscanf(value.c_str(),"%d",&m_feats[aux].range);
         }

         /* Action definition */
         else if(key == "actionType")
         {
            /* Action Type */
            m_feats[aux].actionType = numberActionType(value);
         }
         else if(key == "action")
         {
            /* Action */
            m_feats[aux].action = numberAction(value);
         }

         /* Concept Bonus */
         else if(key == "bonus")
         {
            sscanf(value.c_str(), "%s %s", &buf2[0], &buf3[0]);
            m_feats[aux].conceptBonus.type = buf2;
            m_feats[aux].conceptBonus.id = buf3;
         }

         /* Concept Target */
         else if(key == "affect")
         {
            /* Affect */
            sscanf(value.c_str(), "%s %s", &buf2[0], &buf3[0]);
            m_feats[aux].conceptTarget.type = buf2;
            m_feats[aux].conceptTarget.id = buf3;
         }

         /* Dices! */
         else if(key == "damage")
         {
            /* Base Dice */
            sscanf(value.c_str(), "%d*d%d+%d",
                   &numberOfDices, &diceID, &sumNumber);
            m_feats[aux].diceInfo.baseDice.setType(diceID);
            m_feats[aux].diceInfo.baseDice.setNumberOfDices(numberOfDices);
            m_feats[aux].diceInfo.baseDice.setSumNumber(sumNumber);
         }
         else if(key == "aditionalDice")
         {
            /* Aditional dice */
            sscanf(value.c_str(), "%d*d%d+%d", 
                   &numberOfDices, &diceID, &sumNumber);
            m_feats[aux].diceInfo.aditionalDice.setType(diceID);
            m_feats[aux].diceInfo.aditionalDice.setNumberOfDices(numberOfDices);
            m_feats[aux].diceInfo.aditionalDice.setSumNumber(sumNumber);
         }
         else if(key == "quantityPerDay")
         {
            /* Quantity Per Day */
            sscanf(value.c_str(),"%d",&m_feats[aux].quantityPerDay);
         }
         else if(key == "aditionalQuantity")
         {
            /* Aditional Quantity */
            sscanf(value.c_str(),"%d",&m_feats[aux].aditionalQuantity);
         }
         else if(key == "aditionalLevels")
         {
            /* Aditional Levels */
            sscanf(value.c_str(),"%d",&m_feats[aux].aditionalLevels);
         }

         /* Feat Modifier Effect */
         else if(key == "effect")
         {
            int time=0, mod=0, periodic=0;

            /* Define its values TODO: get peridicity! */
            sscanf(value.c_str(),"%ds %d %s %s", 
                  &time, &mod, &buf2[0], &buf3[0]);

            /* Create the new modEffect */
            modEffect* effect = new modEffect(mod, time, periodic, buf3, buf2);

            /* Insert it on the list */
            m_feats[aux].effects->insert(effect);
         }

         /* Dependent Feats */
         else if(key == "depFeat")
         {
            if(curDepFeat < MAX_DEP_FEATS)
            {
               sscanf(value.c_str(),"%f %s",
                      &m_feats[aux].depFeats[curDepFeat].reason,
                      &buf2[0]);
               m_feats[aux].depFeats[curDepFeat].featIDString = buf2;
               m_feats[aux].depFeats[curDepFeat].used = true;
               curDepFeat++;
            }
         }

         /* Unknow Key!! */
         else
         {
            cerr << "Warning: Unknow key '" << key << "' at feat file: "
                 << arqDescricao << endl;
         }

      }

      m_feats[aux].image = IMG_Load(dirInfo.getRealFile(arqImagem).c_str());
   }

   file.close();
}

/***************************************************************
 *                        Destructor                           *
 ***************************************************************/
void featsList::finish()
{
   if(m_feats)
   {
      delete[] m_feats;
   }
}

/***************************************************************
 *                        featByName                           *
 ***************************************************************/
featDescription* featsList::featByString(string featName)
{
   int i;
   for(i=0; i < totalFeats; i++)
   {
      if(featName == m_feats[i].idString)
      {
         return(&m_feats[i]);
      }
   }
   return(NULL);
}

/***************************************************************
 *                        featByNumber                         *
 ***************************************************************/
featDescription* featsList::featByNumber(int featNumber)
{
   if( (featNumber >= 0) && (featNumber < totalFeats) )
   {
      return(&m_feats[featNumber]);
   }
   return(NULL);
}

/***************************************************************
 *                       static members                        *
 ***************************************************************/
featDescription* featsList::m_feats=NULL;
int featsList::totalFeats=0;

