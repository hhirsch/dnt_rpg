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

#ifndef _dnt_skills_h
#define _dnt_skills_h

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <string>
using namespace std;

#include "../etc/dirs.h"

/*! The individual skill definition. */
class skillDefinition
{
   public:
      string name;          /**< The Skill's Name */
      string description;   /**< The Skill's Description */
      SDL_Surface* image;   /**< The Skill's Image */
      string idString;      /**< String Identificator of the Skill */
      int baseAttribute;    /**< Base Attibute */
      bool isAttribute;     /**< True if the skill is an attribute */
};

/*! The individual skill */
class skill
{
   public:
      int points;             /**< Total skill points */
      int prevPoints;         /**< Previous points, when adding and cancel */
      int mod;                /**< Needed points to add +1 on points */
      skillDefinition* definition; /**< Pointer to definition of this skill */
};

/*! The group skill definitions. All game skills are here. */
class skillsDefinitions
{
   public:
      /*! Init the the skills Definitions, loading them all from the
       * definition file skills/skills.lst */
      void init();

      /*! Finish the use of the skills definitions,
       * freeing the memory of them all */
      void finish();

      /*! Get total skills */
      int getTotalSkills();

      /*! Return the wanted skill definition
       * \param idString -> String Identifier of the skill 
       * \return -> pointer to the skillDefinition */
      skillDefinition* getSkillDefinition(string idString);

      /*! Get an specific skill definition
       * \param i -> index of the skill
       * \return -> pointer to the skill description or NULL */
      skillDefinition* getSkillDefinition(int i);

   protected:
      static skillDefinition* skillsDefs;
      static int totalSkills;
  
};

/*! The skills of a character or thing. It has the pointers to the
 * definitions needed. */
class skills
{
   public:
      /*! skills contructor. Read all skills from file.
       *  \param dir -> directory of the skills files
       *  \param arq -> filename of the skills list. */
      //skills(string dir, string arq); 
      /*! skills contructor. Read all skills from a skills. Do not load 
       *  descriptions. 
       *  \param sk -> other skills pointer.
       */
      skills(skills* sk);
      /*! skills contructor. Read all skills from a skills. Do not load 
       *  descriptions and images.
       */
      skills();
      /*! skills destructor. */
      ~skills();

      /*! Return the wanted skill
       * \param idString -> String Identifier of the skill 
       * \return -> pointer to the skill */
      skill* getSkillByString(string idString);
      
      /*! Return the wanted skill
       * \param i -> integer representing the skill 
       * \return -> pointer to the skill */
      skill* getSkillByInt(int i);

      /*! Return the wanted skill number
       * \param idString -> String identifier of the skill 
       * \return -> skill index */
      int getSkillIntByString(string idString);

      /*! Do a skill check (getting the value got at the check,
       *  to compare latter with another skill check or a fixed value) 
       *  \param idString -> String identifier of the skill
       *  \param difficulty -> check difficulty
       *  \return -> true if success, false if fail */
      bool doSkillCheck(string idString, int difficulty);

      /*! Do a skill check (getting the value got at the check,
       *  to compare latter with another skill check or a fixed value) 
       *  \param sk -> pointer to the skill to check 
       *  \param difficulty -> check difficulty
       *  \return -> true if succes, false if fail */
      bool doSkillCheck(skill* sk, int difficulty);

      /*! set avaible points
       * \param points -> avaiblePoints */
      void setAvaiblePoints(int points);

      /*! get avaible points
       * \return avaiblePoints */
      int getAvaiblePoints();

      /*! clear all skills points */
      void clear();

      /*! Get Total Skills on the list
       * \return -> number of skills */
      int getTotalSkills();

   private:
     skill* skillsVector;     /**< the internal skills */

     int totalSkills;         /**< Total Number of Skills on List */
     int avaiblePoints;       /**< Total Avaible Points on List */
};

#endif
