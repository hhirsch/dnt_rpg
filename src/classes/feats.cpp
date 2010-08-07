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
#include "../engine/dccnit.h"
#include "../ia/iascript.h"
#include "../particle/partcontroller.h"
#include "../lang/translate.h"
#include "../etc/dirs.h"
#include "../etc/defparser.h"
#include <SDL/SDL_image.h>

string dntFeatTypeName[] =
{
   "FEAT_TYPE_PERMANENT",
   "FEAT_TYPE_ON_TARGET",
   "FEAT_TYPE_ON_AREA"
};

/***********************************************************************
 *                            ~depFeatList                             *
 ***********************************************************************/
depFeatList::~depFeatList()
{
   clearList();
}

/***********************************************************************
 *                             freeElement                             *
 ***********************************************************************/
void depFeatList::freeElement(dntListElement* obj)
{
   delete((depFeat*)obj);
};

/***********************************************************************
 *                          ~reqFactorList                             *
 ***********************************************************************/
reqFactorList::~reqFactorList()
{
   clearList();
}

/***********************************************************************
 *                             freeElement                             *
 ***********************************************************************/
void reqFactorList::freeElement(dntListElement* obj)
{
   delete((reqFactor*)obj);
};

/**************************************************************************
 *                            FEAT_DESCRIPTION                            *
 **************************************************************************/

/***************************************************************
 *                       Constructor                           *
 ***************************************************************/
featDescription::featDescription()
{
   internalListNumber = 0;
   quantityPerDay = 0;
   aditionalQuantity = 0;
   aditionalLevel = 0;
   costToUse = 0;
   powerLevel = 0;
   type = 0;
   range = 0;
   scriptFile = "";
   name = "undefined";
   idString = "unknow";
   description = "undefined";
   image = NULL;
}

/***************************************************************
 *                        Destructor                           *
 ***************************************************************/
featDescription::~featDescription()
{
   /* Clear the image */
   if(image != NULL)
   {
      SDL_FreeSurface(image);
   }
}

/**************************************************************************
 *                                    FEAT                                *
 **************************************************************************/

/***************************************************************
 *                       Constructor                           *
 ***************************************************************/
feats::feats(void* usedEngine)
{
   totalFeats = 0;
   uEngine = usedEngine;
}

/***************************************************************
 *                        Destructor                           *
 ***************************************************************/
feats::~feats()
{
   clear();
}

/***************************************************************
 *                           clear                             *
 ***************************************************************/
void feats::clear()
{
   totalFeats = 0;
}

/***************************************************************
 *                         getTotal                            *
 ***************************************************************/
int feats::getTotal()
{
   return(totalFeats);
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
      m_feats[totalFeats].actualQuantity = featInsert->quantityPerDay;
      totalFeats++;
      return(true);
   }
   return(false);
}

/***************************************************************
 *                            canUse                           *
 ***************************************************************/
bool feats::canUse(int featNumber)
{
   char texto[255];
   briefing brief;

   /* Verify if the feat is valid */
   if( (featNumber < 0) || (featNumber >= totalFeats) )
   {
      brief.addText(gettext("Invalid Talent"), 255, 0, 0);
      return(false);
   }

   /* Verify if have the feat points to use it */
   if(m_feats[featNumber].actualQuantity - 
         m_feats[featNumber].info->costToUse >= 0)
   {
      /* Show feature name */
      if(featNumber != FEAT_WEAPON_ATTACK)
      {
         sprintf(texto,"%s ",m_feats[featNumber].info->name.c_str());
         brief.addText(texto); 
      }

      return(true);
   }
   else
   {
      /* Can't use due to points! */
      brief.addText(gettext("Not enought points to use!"), 255, 10, 10);
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
   depFeat* dft;
   
   /* Apply Cost */
   if(m_feats[featNumber].info->costToUse > 0)
   {
      /* Decrement its avaiable quantity */
      m_feats[featNumber].actualQuantity -= m_feats[featNumber].info->costToUse;

      /* Decrement avaiable quantity of dependent feats */
      dft = (depFeat*)m_feats[featNumber].info->depFeats.getFirst();

      for(i=0; i < m_feats[featNumber].info->depFeats.getTotal(); i++)
      {
         ft = featByString(dft->featId);
         ft->actualQuantity -= m_feats[featNumber].info->costToUse / 
                               dft->reason;

         /* next dependency */
         dft = (depFeat*)dft->getNext();
      }
   }
}

/***************************************************************
 *                       useFeatAtTarget                       *
 ***************************************************************/
bool feats::useFeatAtTarget(thing* actor, int featNumber, thing* target)
{
   briefing brief;
   iaScript* sc;
   engine* eng = (engine*)uEngine;

   if( (canUse(featNumber)) && (!m_feats[featNumber].info->scriptFile.empty()))
   {
      /* Verify range */
      if(!actionInRange(actor->xPosition, actor->zPosition, 
               target->xPosition, target->zPosition,
               m_feats[featNumber].info->range*METER_TO_DNT))
      {
         brief.addText(gettext("Too far away for action!"), 225, 20, 20);
         return(false);
      }

      /* Init the script to use */
      sc = new iaScript(m_feats[featNumber].info->scriptFile, uEngine);

      /* Set infos */
      sc->defineCharacterOwner((character*)actor);
      sc->defineMap(eng->getCurrentMap(), eng->NPCs);

      /* Set parameters */
      sc->setParameter("target", target);

      /* Run it! */
      sc->run(0);

      /* Finish with the script */
      delete(sc);

      return(true);
   }

   return(false);
}

/***************************************************************
 *                       useFeatAtArea                         *
 ***************************************************************/
bool feats::useFeatAtArea(thing* actor, int featNumber, 
             float x, float y, float z)
{
   /* TODO */
}

/***************************************************************
 *                      applyPermanentFeat                     *
 ***************************************************************/
bool feats::applyPermanentFeat(thing* actor, int featNumber)
{
   /* TODO */
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

      if( (ft != FEAT_WEAPON_ATTACK) &&
          (m_feats[ft].info->action == ACT_ATTACK)  && 
          (m_feats[ft].actualQuantity - m_feats[ft].info->costToUse >= 0) )
      {
          /* is avaible */
          return(ft);
      }
      else
      {
          /* otherwise, use base weapon attack (melee or ranged) */
         return(FEAT_WEAPON_ATTACK);
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
   int i;

   /* FIXME test range of the feats! */

   /* Take the initial feat */
   ft = FEAT_WEAPON_ATTACK;
   power = m_feats[ft].info->powerLevel;

   /* Run over all feats searching for a powerfull one */
   for(i = 0; i < totalFeats; i++)
   {
      /* Verify if is an attack feat and is avaible */
      if( (i != ft) && (i != FEAT_WEAPON_ATTACK) && 
          (m_feats[i].info->action == ACT_ATTACK) && 
          (m_feats[i].actualQuantity - m_feats[i].info->costToUse >= 0) )
      {
         /* verify if is powerfull */
         if(m_feats[i].info->powerLevel > power)
         {
            /* Is powerfull, take the feat */
            ft = i;
            power = m_feats[i].info->powerLevel;
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
         /* Verify if is an heal feat and is available */
         if( (m_feats[i].info->action == ACT_HEAL) && 
             (m_feats[i].actualQuantity-m_feats[i].info->costToUse >= 0))
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
          (m_feats[ft].actualQuantity-m_feats[ft].info->costToUse >= 0))
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
      /* Run over all feats searching for a powerfull heal one */
      for(i = 0; i < totalFeats; i++)
      {
         /* Verify if is an heal feat and is available */
         if( (i != ft) && (m_feats[i].info->action == ACT_HEAL) && 
             (m_feats[i].actualQuantity - m_feats[i].info->costToUse >= 0))
         {
            /* Verify if is powerfull */
            if(m_feats[i].info->powerLevel > m_feats[ft].info->powerLevel)
            {
               ft = i;
            }
         }
      }
   }

   return(ft);
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
   char buf2[256];
   char buf3[256];
   reqFactor* req;
   string strBuffer;
   int aux, i;

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

   /* Get all Feats from file */
   for(aux = 0; aux < totalFeats; aux++)
   {
      getline(file, strBuffer);
      sscanf(strBuffer.c_str(),"%s",&buf2[0]);
      arqDescricao = buf2;
      arqDescricao = dir+arqDescricao;

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
         /* Type */
         else if(key == "type")
         {
            for(i = 0; i < FEAT_TYPE_TOTAL; i++)
            {
               if(value == dntFeatTypeName[i])
               {
                  m_feats[aux].type = i;
                  break;
               }
            }
         }
         /* Action */
         else if(key == "action")
         {
            m_feats[aux].action = numberAction(value);
         }
         /* Icon */
         else if(key == "icon")
         {
            m_feats[aux].image = IMG_Load(dirInfo.getRealFile(value).c_str());
         }

         /* Power Level */
         else if(key == "powerLevel")
         {
            sscanf(value.c_str(), "%d", &m_feats[aux].powerLevel);
         }

         /* Pre-Requistes */
         else if(key == "required")
         {
            req = new(reqFactor);
            sscanf(value.c_str(), "%s %s %d", 
                  &buf2[0], &buf3[0], &req->requiredLevel);
            req->requiredFactor.type = buf2;
            req->requiredFactor.id = buf3;
            m_feats[aux].reqFactors.insertAtEnd(req);
         }

         /* Range */
         else if(key == "range")
         {
            sscanf(value.c_str(),"%d",&m_feats[aux].range);
         }

         /* Cost To Use */
         else if(key == "costToUse")
         {
            sscanf(value.c_str(), "%d", &m_feats[aux].costToUse);
         }

         /* Quantity Per Day */
         else if( (key == "quantityPerDay") || (key == "quantity") )
         {
            sscanf(value.c_str(),"%d",&m_feats[aux].quantityPerDay);
         }
         /* Aditional Quantity */
         else if(key == "aditionalQuantity")
         {
            sscanf("%s %d %d %d", 
                   &buf2[0], &m_feats[aux].aditionalQuantity,
                   &m_feats[aux].aditionalLevel,
                   &m_feats[aux].aditionalDiv);
            m_feats[aux].aditionalFactor.id = buf2;
            m_feats[aux].aditionalFactor.type = "CLASS";
         }

         /* Script */
         else if(key == "script")
         {
            m_feats[aux].scriptFile = value;
         }

         /* Dependent Feats */
         else if(key == "depFeat")
         {
            depFeat* dp = new depFeat();

            sscanf(value.c_str(),"%f %s", &dp->reason, &buf2[0]);
            dp->featId = buf2;
            m_feats[aux].depFeats.insert(dp);
         }

         /* Unknow Key!! */
         else
         {
            cerr << "Warning: Unknow key '" << key << "' at feat file: "
                 << arqDescricao << endl;
         }

      }
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
 *                          getTotal                           *
 ***************************************************************/
int featsList::getTotal()
{
   return(totalFeats);
}

/***************************************************************
 *                       static members                        *
 ***************************************************************/
featDescription* featsList::m_feats=NULL;
int featsList::totalFeats=0;

