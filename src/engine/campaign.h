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

#ifndef _dnt_campaign_h
#define _dnt_campaign_h

#include <string>

/* foward declarations */
class engine;
class iaScript;

namespace DNT
{

/*! The campaign defines the initial game state,
 * as a map to load and an initial script to call 
 * (for example a script that will populate the
 *  PC inventory and se some triggers). */
class Campaign
{
   public:
      /*! Constructor
       * \param eng -> pointer to the current used engine 
       * \param fileName -> campaign file to load */
      Campaign(engine* eng, std::string fileName);
      /*! Destructor */
      ~Campaign();

      /*! Get campaign fileName */
      std::string getFileName(){return(campaignFile);};
      /*! Get campaign name */
      std::string getName(){return(name);};
      /*! Get campaign description */
      std::string getDescription(){return(description);};
      /*! Get campaign initial map filename */
      std::string getInitialMap(){return(initialMap);};
      /*! Get the introduction book name (if any) */
      std::string getIntroductionBook(){return(introBook);};

      /*! Call initial script
       * \note -> usually called after the initial map first load by engine
       * \return true when script is still running, false if ended. */
      bool runInitialScript();

   protected:
      /*! Load the campaign */
      void load(std::string fileName);

      std::string campaignFile;  /**< File of the campaign */
      std::string initialMap;    /**< Initial Map of the campaign */
      std::string name;          /**< Campaign name */
      std::string description;   /**< Campaign description */
      std::string initialScript; /**< Initial Script */
      std::string introBook;     /**< Introduction comic book */
      engine* curEngine;         /**< Current engine */
      iaScript* curScript;       /**< Current running script */
};

}

#endif

