#include "feats.h" 
#include "defs.h"
#include <SDL/SDL_image.h>


feats::feats()
{
   totalFeats = 0;
}

feats::~feats()
{
   totalFeats = 0;
}

feat feats::featByNumber(int featNumber)
{
   if( (featNumber > 0) && (featNumber < totalFeats) )
   {
      return(m_feats[featNumber]);
   }
   return(m_feats[0]);
}

feat feats::featByName(string featName)
{
   int i;
   for(i=0;i<totalFeats;i++)
   {
      if(featName.compare(m_feats[i].name) == 0)
      {
         return(m_feats[i]);
      }
   }
   return(m_feats[0]);
}

bool feats::insertFeat(featDescription featInsert)
{
   int i;
   if(totalFeats < MAX_FEATS)
   {
      m_feats[totalFeats].internalListNumber = featInsert.internalListNumber;
      m_feats[totalFeats].quantityPerDay = featInsert.quantityPerDay;
      m_feats[totalFeats].actualQuantity = featInsert.quantityPerDay;
      m_feats[totalFeats].costToUse = featInsert.costToUse;
      m_feats[totalFeats].actionType = featInsert.actionType;
      m_feats[totalFeats].action = featInsert.action;
      m_feats[totalFeats].name = featInsert.name;
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

int numberClass(string buffer)
{
   int ret = -1;
   if(buffer.compare(STR_EADM) == 0)
     ret = CLASS_EADM;
   else if(buffer.compare(STR_EBIOLOGIA) == 0)
     ret = CLASS_EBIOLOGIA;
   else if(buffer.compare(STR_EDCC) == 0)
     ret = CLASS_EDCC;
   else if(buffer.compare(STR_EDFISICA) == 0)
     ret = CLASS_EDFISICA;
   else if(buffer.compare(STR_EMECANICA) == 0)
     ret = CLASS_EMECANICA;
   else if(buffer.compare(STR_EFILOSOFIA) == 0)
     ret = CLASS_EFILOSOFIA;
   else if(buffer.compare(STR_EFISIOTERAPIA) == 0)
     ret = CLASS_EFISIOTERAPIA;
   else if(buffer.compare(STR_EMEDICINA) == 0)
     ret = CLASS_EMEDICINA;
   else if(buffer.compare(STR_EMUSICA) == 0)
     ret = CLASS_EMUSICA;
   else if(buffer.compare(STR_ETO) == 0)
     ret = CLASS_ETO;

   return(ret);
}

featsList::featsList(string dir, string arq)
{
   FILE* file;
   string arqDescricao;
   string arqImagem;
   char buffer[1024];
   char buf2[128];
   char buf3[128];
   int num;
   if(!(file=fopen(arq.c_str(),"r")))
   {
       printf("Error while opening feats list: %s\n",arq.c_str());
       return;
   }
   
   int aux;
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
      m_feats[aux].requeridedClass = numberClass(buffer);
      fscanf(desc,"%d*d%d+%d",&m_feats[aux].diceInfo.baseDice.numberOfDices,
                              &m_feats[aux].diceInfo.baseDice.diceID,
                              &m_feats[aux].diceInfo.baseDice.sumNumber);
      fscanf(desc,"%d*d%d+%d",
                             &m_feats[aux].diceInfo.aditionalDice.numberOfDices,
                             &m_feats[aux].diceInfo.aditionalDice.diceID,
                             &m_feats[aux].diceInfo.aditionalDice.sumNumber);
      fscanf(desc,"%d",&m_feats[aux].quantityPerDay);
      fscanf(desc,"%d",&m_feats[aux].costToUse);
      fscanf(desc,"%d",&m_feats[aux].actionType);
      fscanf(desc,"%d",&m_feats[aux].action);
      //TODO DEPENDENT FEATS READ
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


