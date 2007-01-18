#ifndef _thing_h
#define _thing_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "skills.h"
#include "align.h"
#include "modifier.h"

#define PSYCHO_HOSTILE 0
#define PSYCHO_NEUTRAL 1
#define PSYCHO_FRIENDLY 2

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
      int baseAttackModifier; /**< Thing's Base Attack Modifier */
      int sizeModifier;       /**< Thing's Size Modifier */
      int psychoState;        /**< Thing's Psycho State */

      bool dead;             /**< Dead state or not.(if dead is only a corpse)*/

      skills sk;             /**< skills without images and description */

      /*!
       **********************************************
       *  Get the skill bonus (modifier or not).
       *  \param curSkill -> current Skill
       *  \return bonus.
       **********************************************/
      int skillBonus(skill* curSkill);
      /*!
       **********************************************
       *  Get the attribute bonus: modifier.
       *  \param curAttribute -> current attribute pointer.
       *  \return bonus.
       **********************************************/
      int attBonus(skill* curAttribute);
      /*!
       **********************************************
       *  Get the attribute bonus: modifier.
       *  \param curAttribute -> current attribute ID.
       *  \return bonus.
       **********************************************/
      int attBonus(int curAttribute);

      /*!
       **********************************************
       *  Get the bonus Value (modifier or not).
       *  \param something -> number of the definition.
       *  \return bonus.
       **********************************************/
      int getBonusValue(factor something);
};

#endif

