#ifndef _weapon_h
#define _weapon_h

#include "dices.h"
#include "object.h"

/* A single weapon info type */
class wInfo
{
   public:
      string name;   /**< Name of the info */
      int index;     /**< Index of the info */
      string title;  /**< Title of the info (text) */
};

/*! The Weapon Types Class. This class receives all informations about
 * all the types of weapons, readed at the ../data/weapons/types files. */
class weaponTypes
{
   public:
      /*! Constructor */
      weaponTypes();
      /*! Destructor */
      ~weaponTypes();

      /*! Get the category number
       * \param name -> name of the category
       * \return -> the category internal number. -1 on error. */
      int getCategory(string name);

      /*! Get the size number
       * \param name -> name of the size
       * \return -> the size internal number. -1 on error. */
      int getSize(string name);

      /*! Get the weight number
       * \param name -> name of the weight
       * \return -> the weight internal number. -1 on error. */
      int getWeight(string name);

      /*! Get the damage number
       * \param name -> name of the damage
       * \return -> the damage internal number. -1 on error. */
      int getDamage(string name);

      /*! Get the range number
       * \param name -> name of the range
       * \return -> the range internal number. -1 on error. */
      int getRange(string name);

      /*! Get the munition type number
       * \param name -> name of the munition
       * \return -> the munition internal number. -1 on error. */
      int getMunition(string name);

   private:
      /*! Read A definition file
       * \param fileName ->  string with the fileName of the file to load */
      void readFile(string fileName);

      /*! Get a thing from a wInfo
       * \param thing -> the pointer to the desired wInfo 
       * \param total -> the total elements on the thing
       * \param name -> thing name to get
       * \return -> the thing internal number . -1 on error. */
      int getThing(wInfo* thing, int total, string name);


      int totalCategories;    /**< Total number of categories */
      wInfo* categories;      /**< The categories */
      int totalRanges;        /**< Total Number of ranges */
      wInfo* ranges;          /**< The ranges */
      int totalSizes;         /**< Total Number of Sizes */
      wInfo* sizes;           /**< The sizes */
      int totalWeights;       /**< Total Number of Weights */
      wInfo* weights;         /**< The weights */
      int totalDamages;       /**< Total Number of damages */
      wInfo* damages;         /**< The damages */
      int totalMunitions;     /**< Total Number of Munitions */
      wInfo* munitions;       /**< The munitions */
};


/*! The Weapon Class Definition */
class weapon: public object
{
   public:
      /*! Constructor
       * \param path -> name of the weapon file to load
       * \param mdlList -> list of models
       * \param wTypes -> the weapons types */
      weapon(string path, modelList& mdlList, weaponTypes& wTypes);
      /*! Destructor */
      ~weapon();

      /*! Get the Types Informations of the weapon
       * \param category -> category type of the weapon
       * \param range -> range type of the weapn (meele or ranged)
       * \param size -> size type of the weapon
       * \param weight -> weight type of the weapon
       * \param damageA -> first damage type
       * \param damageB -> second damage type */
      void getType(int& category, int& range, int& size, int& weight,
                   int& damageA, int& damageB);

      /*! Gets the range type of the weapon (usually, meele or ranged) */
      int getRangeType();

      /*! Get the weapon's damage dice
       * \return the weapons damage dice */
      diceThing getDice();

      /*! Get the range distance of the weapon
       * \return -> range distance */
      int getRange();

      /*! Get the weight of the weapon
       * \return -> weapon's weight in Kg. */
      float getWeight();

   protected:
      int munitionType;       /**< Weapon Munition Type (ex: 9mm) */
      int munitionCapacity;   /**< Weapon Max Munition */
      int actualMunition;     /**< Weapon actual munition */
      int categoryType;       /**< Weapon Category (Ex: Exotic)  */
      int rangeType;          /**< Range Type (Ex: Meele) */
      int sizeType;           /**< Size Type (Ex: Small) */
      int weightType;         /**< Weight Type (Ex: Light) */
      diceThing damageDice;   /**< Damage Dices */
      int rangeValue;         /**< Range Value */
      float weightValue;      /**< Weight Value */
      int damageType[2];      /**< Damage Type Vector (Ex: Slashing) */
      string attackSound[2];  /**< Damage Type Sounds Files */

      /*! TODO /todo Special things on weapons! It's, for example a 
       * resistance, a special damage, etc.. */

};


#endif

