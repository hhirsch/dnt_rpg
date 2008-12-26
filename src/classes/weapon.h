#ifndef _weapon_h
#define _weapon_h

#include "dices.h"
#include "object.h"

/* A single weapon info type */
class wInfo
{
   public:
      /*! Constructor */
      wInfo();

      /*! Assign Operator */
      void operator=(wInfo& v);

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
       * \return -> the category pointer. NULL on error.. */
      wInfo* getCategory(string name);

      /*! Get the size number
       * \param name -> name of the size
       * \return -> the size pointer. NULL on error.. */
      wInfo* getSize(string name);

      /*! Get the weight number
       * \param name -> name of the weight
       * \return -> the weight pointer. NULL on error.. */
      wInfo* getWeight(string name);

      /*! Get the damage number
       * \param name -> name of the damage
       * \return -> the damage pointer. NULL on error.. */
      wInfo* getDamage(string name);

      /*! Get the range number
       * \param name -> name of the range
       * \return -> the range pointer. NULL on error.. */
      wInfo* getRange(string name);

      /*! Get the munition type number
       * \param name -> name of the munition
       * \return -> the munition pointer. NULL on error.. */
      wInfo* getMunition(string name);

   private:
      /*! Read A definition file
       * \param fileName ->  string with the fileName of the file to load */
      void readFile(string fileName);

      /*! Get a thing from a wInfo
       * \param thing -> the pointer to the desired wInfo 
       * \param total -> the total elements on the thing
       * \param name -> thing name to get
       * \return -> the thing internal number . -1 on error. */
      wInfo* getThing(wInfo* thing, int total, string name);


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
      void getType(wInfo& category, wInfo& range, wInfo& size, wInfo& weight,
                   wInfo& damageA, wInfo& damageB);

      /*! Get the range type of the weapon (usually, meele or ranged)
       * \return -> wInfo representing the range type */
      wInfo* getRangeType();
      
      /*! Get the ammo type of the weapon
       * \return -> wInfo representing the munition type */
      wInfo* getMunitionType();

      /*! Get the weapon's damage dice
       * \return the weapons damage dice */
      diceThing getDice();

      /*! Get the range distance of the weapon
       * \return -> range distance */
      int getRange();

      /*! Get the weight of the weapon
       * \return -> weapon's weight in Kg. */
      float getWeight();

      /*! Get the current munition value
       * \return -> current munition on the weapon */
      int getCurrentMunition();
      /*! Set the current munition value
       * \param v -> new current munition value */
      void setCurrentMunition(int v);

   protected:
      wInfo* munitionType;    /**< Weapon Munition Type (ex: 9mm) */
      int munitionCapacity;   /**< Weapon Max Munition */
      int actualMunition;     /**< Weapon actual munition */
      wInfo* categoryType;    /**< Weapon Category (Ex: Exotic)  */
      wInfo* rangeType;       /**< Range Type (Ex: Meele) */
      wInfo* sizeType;        /**< Size Type (Ex: Small) */
      wInfo* weightType;      /**< Weight Type (Ex: Light) */
      diceThing damageDice;   /**< Damage Dices */
      int rangeValue;         /**< Range Value */
      wInfo* damageType[2];   /**< Damage Type Vector (Ex: Slashing) */
      string attackSound[2];  /**< Damage Type Sounds Files */

      /*! TODO /todo Special things on weapons! It's, for example a 
       * resistance, a special damage, etc.. */

};


#endif

