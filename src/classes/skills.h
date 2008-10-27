#ifndef _skills_h
#define _skills_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <string>
using namespace std;

#include "../etc/dirs.h"

/*! The individual skill definition.
 */
class skill
{
   public:
      int points;             /**< Total skill points */
      int prevPoints;         /**< Previous points, when adding and cancel */
      int mod;                /**< Needed points to add +1 on points */
      string name;            /**< Skill Name */
      string description;     /**< Skill Description */
      string idString;        /**< String Identificator of the Skill */
      int baseAttribute;      /**< Base Attibute */
      SDL_Surface* image;     /**< Skill Image */
};

/*! The group skill definitions. All game skills are here.
  */
class skills
{
   public:
      /*! skills contructor. Read all skills from file.
       *  \param dir -> directory of the skills files
       *  \param arq -> filename of the skills list. */
      skills(string dir, string arq); 
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

      /*! Return the wanted skill number
       * \param idString -> String identifier of the skill 
       * \return -> skill index */
      int getSkillIntByString(string idString);

      /*! Do a skill check (getting the value got at the check,
       *  to compare latter with another skill check or a fixed value) 
       *  \param idString -> String identifier of the skill
       *  \return -> skill check result value */
      int doSkillCheck(string idString);

      /*! Do a skill check (getting the value got at the check,
       *  to compare latter with another skill check or a fixed value) 
       *  \param sk -> pointer to the skill to check 
       *  \return -> skill check result value */
      int doSkillCheck(skill* sk);

      /*! set avaible points
       * \param points -> avaiblePoints */
      void setAvaiblePoints(int points);

      /*! get avaible points
       * \return avaiblePoints */
      int getAvaiblePoints();

      /*! clear all skills points */
      void clear();


     skill* m_skills; /**< the internal skills */

   private:
     int totalSkills;         /**< Total Number of Skills on List */
     int avaiblePoints;       /**< Total Avaible Points on List */
};

#endif
