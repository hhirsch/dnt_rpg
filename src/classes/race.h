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

#ifndef _dnt_race_h
#define _dnt_race_h

#include <SDL/SDL.h>
#ifdef __APPLE__
   #include <SDL_image/SDL_image.h>
#else
   #include <SDL/SDL_image.h>
#endif

#include <string>

#include "../etc/list.h"

#include "modifier.h"
#include "feats.h"
#include "skills.h"
using namespace std;

/*! Single Race Definition */
class race: public dntListElement
{
   public:
      race();
      ~race();

      /*! Apply the Skill Costs, based on the race (in other words,
       * put the cost = 1 to all race skills)
       * \param sk -> character skill list */
      void applySkillCosts(skills* sk);

      /*! Get the attributes modifiers based on the race
       * \param mods -> the modifiers
       * \param sum -> if is to sum to the mods or just set the mods as
       *               the modifiers. (the sum = true is util when calling
       *               this function after a call to the similar one at
       *               the classes)
       * \param sk -> character skill list */
      void getAttModifiers(int mods[6], bool sum, skills* sk);


      int intID;              /**< Integer Identifier of the race */
      string strID;           /**< String Identifier of the race */
      SDL_Surface* image;     /**< Image of the Race */
      
      string name;            /**< Name of the Race */
      string description;     /**< Description of the Race */
      string citation;        /**< Race Citation */

      int totalModifiers;     /**< Total Modifiers on Class */
      modifier* raceModifiers;/**< Race Modifiers */

      int totalFeats;         /**< Total Race Feats */
      string* raceFeats;      /**< ID String of Race Feats */

      int totalSkills;           /**< Total Race Skills */
      string* raceSkills;        /**< Race Skills */
      string* raceSkillsJustify; /**< Justify of the Skill */
};

/**! List of races */
class raceList: public dntList
{
   public:
      /*! Constructor */
      raceList();
      /*! Destructor */
      ~raceList();

   protected:
      /*! Delete race memory
       * \param obj -> pointer to the race to delete */
      void freeElement(dntListElement* obj);
};

/**! The List of Races Class */
class races
{
   public:
      /*! Static Constructor */
      void init();
      /*! Static Destructor */
      void finish();

      /*! Get an Race from list
       * \param id -> integer ID of the race
       * \return race found, if found one. */
      race* getRaceByInteger(int id);
      /*! Get an Race from list 
       * \param id -> string ID of the race
       * \return race found, if found one. */
      race* getRaceByString(string id);

      /*! Get total races avaible
       * \return number of races avaible */
      int getTotalRaces();

   private:
      static raceList* list;        /**< List of races */

      /*! Insert an race on List
       * \param fileName -> file name of the race to insert 
       * \param imgFile -> image file of the race
       * \param idString -> String Identifier of the race
       * \param idInt -> Integer Identifier of the race */
      void insertRace(string fileName,string imgFile,string idString,int idInt);
};


#endif
