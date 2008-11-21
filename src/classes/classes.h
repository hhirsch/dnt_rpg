#ifndef _classes_h
#define _classes_h

#include <string>
using namespace std;
#include "skills.h"
#include "feats.h"
#include "defs.h"
#include "modifier.h"

#define MAX_DEFINED_LEVEL 20

/*! Single Character Class Definition */
class classe
{
   public:
      /*! Constructor */
      classe();
      /*! Destructor */
      ~classe();

      /*! Apply the Skill Costs, based on the classe (in other words,
       * put the cost = 1 to all race skills)
       * \param sk -> character skill list */
      void applySkillCosts(skills* sk);

      /*! Get the attributes modifiers based on the classe
       * \param mods -> the modifiers
       * \param sum -> if is to sum to the mods or just set the mods as
       *               the modifiers. (the sum = true is util when calling
       *               this function after a call to the similar one at
       *               the races)
       * \param sk -> character skill list */
      void getAttModifiers(int mods[6], bool sum, skills* sk);

      int intID;                 /**< Integer Identifier of the class */
      string strID;              /**< String Identifier of the class */
      SDL_Surface* image;        /**< Image of the Class */
      
      string name;               /**< Name of the Class */
      string description;        /**< Description of the Class */
      string citation;           /**< Class Citation */

      int totalModifiers;        /**< Total Modifiers on Class */
      int* modifierLevels;       /**< Level when each Modifier will be avaible*/
      modifier* classModifiers;  /**< Class Modifiers */

      int totalFeats;            /**< Total Class Feats */
      string* classFeats;        /**< ID String of Class Feats */
      int* reqLevel;             /**< Requerided Level for Feat */

      int totalSkills;           /**< Total Class Skills */
      string* classSkills;       /**< Class Skills */

      int lifeDiceID;            /**< Life Dice ID */
      points firstLevelSP;       /**< First Level Skill Points */
      points otherLevelsSP;      /**< Other Levels Skill Points */

      bonusAndSaves bonus[MAX_DEFINED_LEVEL]; /**< Bonus per Level */
      
      classe* previous;          /**< Previous Class on list */
      classe* next;              /**< Next Class on list */
};

/**! The List of Classes */
class classes
{
   public:
      /*! Init the classes list to use 
       * \param fileListName -> name of the file containing the list of all
       *         classes (usually ../data/classes/classes.lst)  
       * \param directory -> directory of classes descriptions */
      void init(string directory, string fileListName);

      /*! Finish the classes list  */
      void finish();

      /*! Get a Class from list
       * \param id -> integer ID of the class
       * \return class found, if found one. */
      classe* getClassByInteger(int id);
      /*! Get a Class from list 
       * \param id -> string ID of the class
       * \return class found, if found one. */
      classe* getClassByString(string id);

      /*! Get total classes avaible */
      int getTotalClasses();
      
   private:
      static int totalClasses;        /**< Total Classes on List */
      static classe* first;           /**< First Class on List */

      /*! Insert a class on List
       * \param fileName -> file name of the class to insert 
       * \param imgFile -> image file of the class
       * \param idString -> String Identifier of the class
       * \param idInt -> Integer Identifier of the class */
      void insertClass(string fileName, string imgFile, string idString,
                       int idInt);
};

#endif

