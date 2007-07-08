#ifndef _weapon_h
#define _weapon_h

#include "../etc/animodel.h"
#include "dices.h"

/*! The Weapon Class Definition */
class weapon: public aniModel
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
      int categoryType;       /**< Weapon Category (Ex: Exothic)  */
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

#endif

