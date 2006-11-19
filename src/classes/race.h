#ifndef _race_h
#define _race_h

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>
#include "modifier.h"
#include "feats.h"
#include "skills.h"
using namespace std;

/*! Single Race Definition */
class race
{
   public:
      race();
      ~race();

      int intID;              /**< Integer Identifier of the race */
      string strID;           /**< String Identifier of the race */
      SDL_Surface* image;     /**< Image of the Aligment */
      
      string name;            /**< Name of the Aligment */
      string description;     /**< Description of the Aligment */
      string citation;        /**< Race Citation */

      int totalModifiers;     /**< Total Modifiers on Class */
      modifier* raceModifiers;/**< Race Modifiers */

      int totalFeats;         /**< Total Race Feats */
      string* raceFeats;      /**< ID String of Race Feats */

      int totalSkills;           /**< Total Race Skills */
      string* raceSkills;        /**< Race Skills */
      string* raceSkillsJustify; /**< Justify of the Skill */
      
      race* previous;        /**< Previous Aligment on list */
      race* next;            /**< Next Aligment on list */
};

/**! The List of Races Class */
class races
{
   public:
      /*! Constructor
       * \param fileListName -> name of the file containing the list of all
       *         aligments (usually ../data/race/race.lst)  */
      races(string directory, string fileListName);
      /*! Destructor */
      ~races();

      /*! Get an Race from list
       * \param id -> integer ID of the race
       * \return aligment found, if found one. */
      race* getRaceByInteger(int id);
      /*! Get an Aligment from list 
       * \param id -> string ID of the race
       * \return aligment found, if found one. */
      race* getRaceByString(string id);
      
   private:
      int totalRaces;              /**< Total Races on List */
      race* first;                 /**< First Race on List */

      /*! Insert an race on List
       * \param -> fileName of the race to insert */
      void insertRace(string fileName,string imgFile,string idString,int idInt);
};


#endif
