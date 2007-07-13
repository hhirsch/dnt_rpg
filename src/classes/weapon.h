#ifndef _weapon_h
#define _weapon_h

#include "dices.h"
#include "object.h"

/*! The Weapon Class Definition */
class weapon: public object
{
   public:
      /*! Constructor
       * \param fileName -> name of the weapon file to load */
      weapon(string fileName);
      /*! Destructor */
      ~weapon();

      /*! Get the Types Informations of the weapon
       * \param category -> category type of the weapon
       * \param range -> range type of the weapn (meele or ranged)
       * \param size -> size type of the weapon
       * \param style -> style type of the weapon
       * \param damageA -> first damage type
       * \param damageB -> second damage type */
      void getType(int& category, int& range, int& size, int& style,
                   int& damageA, int& damageB);

      /*! Get the weapon's cost
       * \return weapons cost */
      float getCost();

      /*! Get the weapon's damage dice
       * \return the weapons damage dice */
      diceThing getDice();

      /*! Get the range distance of the weapon
       * \return -> range distance */
      int getRange();

      /*! Get the weight of the weapon
       * \return -> weapon's weight in Kg. */
      float getWeight();

      /*! Calls the attack animation of the weapon */
      void attackAnimation();
   protected:
      int categoryType;       /**< Weapon Category (Ex: Exotic)  */
      int rangeType;          /**< Range Type (Ex: Meele) */
      int sizeType;           /**< Size Type (Ex: Light) */
      int styleType;          /**< Style Type (Ex: Two-Handed) */
      float cost;             /**< Cost Value */
      diceThing damageDice;   /**< Damage Dices */
      int rangeValue;         /**< Range Value */
      float weight;           /**< Weight Value */
      int damageType[2];      /**< Damage Type Vector (Ex: Slashing) */
      string attackSound[2];  /**< Damage Type Sounds Files */

      /*! TODO /todo Special things on weapons! It's, for example a 
       * resistance, a special damage, etc.. */

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

   private:
      /*! Read A definition file
       * \param names -> pointer to the string pointer to be loaded
       * \param totals -> pointer to the integer with totals 
       * \param fileName ->  string with the fileName of the file to load */
      void readFile(string** names, int* totals, string fileName);


      int totalCategories;    /**< Total number of categories */
      string* categories;     /**< The categories */
      int totalRanges;        /**< Total Number of ranges */
      string* ranges;         /**< The ranges */
      int totalSizes;         /**< Total Number of Sizes */
      string* sizes;          /**< The sizes */
      int totalWeights;       /**< Total Number of Weights */
      string* weights;        /**< The weights */
      int totalDamages;       /**< Total Number of damages */
      string* damages;        /**< The damages */
};

#endif

