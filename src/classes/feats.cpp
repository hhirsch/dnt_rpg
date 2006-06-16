#include "feats.h" 


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

featsList::featsList(string dir, string arq)
{
   //TODO
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


