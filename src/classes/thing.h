#ifndef _thing_h
#define _thing_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "skills.h"
#include "defs.h"

/*! Define what is a thing on game, in other words, what is the base fields
 * for characters and map objects.*/
class thing
{
   public:
      /*! thing Constructor */
      thing();
      /*! thing Destructor */
      ~thing();

      int lifePoints;         /**< Thing's Life points */
      int maxLifePoints;      /**< Thing's Max Life Points */
      int fortitude;          /**< Thing's Fortitude */
      int reflexes;           /**< Thing's Reflexes */
      int will;               /**< Thing's Will */
      int armatureClass;      /**< Thing's Armature Class (AC) */
      int displacement;       /**< Thing's Displacement (in meters) */
      int level;              /**< Thing's Level */
      int baseAttackModifier; /**< Thing's Base Attack Modifier */
      int sizeModifier;       /**< Thing's Size Modifier */

      bool dead;             /**< Dead state or not.(if dead is only a corpse)*/

      skills sk;             /**< skills without images and description */

      /*!
       **********************************************
       *  Get the skill bonus (modifier or not).
       *  \param skillNumber -> number of the skill.
       *  \return bonus.
       **********************************************/
      int skillBonus(int skillNumber);
      /*!
       **********************************************
       *  Get the attribute bonus: modifier.
       *  \param attNumber -> number of the attribute.
       *  \return bonus.
       **********************************************/
      int attBonus(int attNumber);
      /*!
       **********************************************
       *  Get the bonus Value (modifier or not).
       *  \param something -> number of the definition.
       *  \return bonus.
       **********************************************/
      int getBonusValue(int something);
};

#endif

