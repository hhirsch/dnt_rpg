/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "campaign.h"
#include "dccnit.h"
#include "../ia/iascript.h"
#include "../etc/defparser.h"

#include <iostream>

using namespace DNT;

/******************************************************************
 *                           Constructor                          *
 ******************************************************************/
Campaign::Campaign(engine* eng, std::string fileName)
{
   curEngine = eng;
   curScript = NULL;
   name = "";
   description = "";
   initialScript = "";
   initialMap = "";
   campaignFile = fileName;
   introBook = "";
   load(fileName);
}

/******************************************************************
 *                            Destructor                          *
 ******************************************************************/
Campaign::~Campaign()
{
   if(curScript)
   {
      delete(curScript);
      curScript = NULL;
   }
}

/******************************************************************
 *                              load                              *
 ******************************************************************/
void Campaign::load(std::string fileName)
{
   defParser def;
   std::string key, value;
   
   campaignFile = fileName;

   if(def.load(fileName))
   {
      while(def.getNextTuple(key, value))
      {
         if(key == "name")
         {
            name = value;
         }
         else if(key == "description")
         {
            description = value;
         }
         else if(key == "initialScript")
         {
            initialScript = value;
         }
         else if(key == "initialMap")
         {
            initialMap = value;
         }
         else if(key == "intro")
         {
            introBook = value;
         }
         else
         {
            std::cerr << "Unknown key '" << key << "'"
               << " for campaign file '" << fileName << "'" << std::endl;
         }
      }
   }
}

/******************************************************************
 *                         runInitialScript                       *
 ******************************************************************/
bool Campaign::runInitialScript()
{
   if(initialScript.empty())
   {
      /* No initial script defined, no run! */
      return(false);
   }

   if(!curScript)
   {
      /* Script not yet defined, create it */
      curScript = new iaScript(initialScript, curEngine);
      curScript->defineMap(curEngine->getCurrentMap(), curEngine->NPCs);
   }
   
   /* Run for 10 lines */
   curScript->run(10);
   if(curScript->finished())
   {
      /* Script is finished */
      delete(curScript);
      curScript = NULL;
      return(false);
   }

   return(true);
}


