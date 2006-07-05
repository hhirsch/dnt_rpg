#include "feats.h" 
#include "defs.h"
#include "actions.h"
#include <SDL/SDL_image.h>

/**************************************************************************
 *                                    FEAT                                *
 **************************************************************************/

feats::feats()
{
   totalFeats = 0;
}

feats::~feats()
{
   totalFeats = 0;
}

feat* feats::featByNumber(int featNumber)
{
   if( (featNumber > 0) && (featNumber < totalFeats) )
   {
      return(&m_feats[featNumber]);
   }
   return(NULL);
}

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


bool feats::applyAttackAndBreakFeat(int featNumber, thing& target, 
                                      string& brief)
{
   int diceValue;
   int damage = 0;
   int i;
   char texto[15];

   if( (featNumber < 0) || (featNumber >= totalFeats) )
   {
      brief = "Invalid Feat!";
      return(false);
   }

   srand(SDL_GetTicks());

   if( (m_feats[featNumber].actualQuantity >= m_feats[featNumber].costToUse)
       || (m_feats[featNumber].costToUse) > 0 )
   {
      sprintf(texto,"%s ",m_feats[featNumber].name.c_str());
      brief = texto;
      /* apply Costs */
      useFeat(featNumber);
      //TODO verify if can use or not based on target thing

      diceValue = (rand() % DICE_D20); 
      sprintf(texto,"%d x %d : ",diceValue,target.armatureClass);
      brief += texto;
      //TODO apply reflexes bonus, esquive bonus, etc 
      /*TODO apply resistances  */

      //TODO apply bonus (skill bonus)

      if(diceValue - target.armatureClass <= 0)
      {
         brief += "Miss.";
         return(true);
      }
      else
      {
         brief += "Hit for ";
      }

      /* Apply Base Damage Dices */
      for(i = 0; i < m_feats[featNumber].diceInfo.baseDice.numberOfDices; i++)
      {
          damage += rand() % m_feats[featNumber].diceInfo.baseDice.diceID;
      }
      damage += m_feats[featNumber].diceInfo.baseDice.sumNumber;

      sprintf(texto,"%d points.",damage);
      brief += texto;

      /*TODO apply aditional dices */

      /* apply damage on thing */
      target.lifePoints -= damage;

      if( diceValue >= 18)
      {
          brief += " Critical Hit!" ;
      }

      return(true);
   }
   brief = "Not Enough Points to Use!";
   return(false);
}

void feats::newDay()
{
   int i;
   for(i=0;i<totalFeats;i++)
   {
      m_feats[i].actualQuantity = m_feats[i].quantityPerDay;
   }
}

/**************************************************************************
 *                           FEAT DESCRIPTION                             *
 **************************************************************************/
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

featsList::~featsList()
{
}

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

featDescription featsList::featByNumber(int featNumber)
{
   if( (featNumber > 0) && (featNumber < NUMBER_OF_FEATS) )
   {
      return(m_feats[featNumber]);
   }
   return(m_feats[0]);
}

