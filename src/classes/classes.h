#ifndef _classes_h
#define _classes_h

#include <string>
using namespace std;
#include "skills.h"
#include "feats.h"
#include "defs.h"

#define MAX_DEFINED_LEVEL 20

/*! Single Character Class Definition */
class classe
{
   public:
      /*! Constructor */
      classe();
      /*! Destructor */
      ~classe();

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
      /*! Constructor
       * \param fileListName -> name of the file containing the list of all
       *         classes (usually ../data/classes/classes.lst)  
       * \param directory -> directory of classes descriptions */
      classes(string directory, string fileListName);
      /*! Destructor */
      ~classes();

      /*! Get a Class from list
       * \param id -> integer ID of the class
       * \return class found, if found one. */
      classe* getClassByInteger(int id);
      /*! Get a Class from list 
       * \param id -> string ID of the class
       * \return class found, if found one. */
      classe* getClassByString(string id);
      
   private:
      int totalClasses;        /**< Total Classes on List */
      classe* first;           /**< First Class on List */

      /*! Insert a class on List
       * \param fileName -> file name of the class to insert 
       * \param imgFile -> image file of the class
       * \param idString -> String Identifier of the class
       * \param idInt -> Integer Identifier of the class */
      void insertClass(string fileName, string imgFile, string idString,
                       int idInt);
};

#endif

