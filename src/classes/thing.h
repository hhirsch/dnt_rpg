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
      /*! thing virtual Destructor */
      virtual ~thing();

      skills sk;             /**< skills without images and description */

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
      float orientation;      /**< Character's orientation (angle) */
      float xPosition;        /**< Character's X Position */
      float yPosition;        /**< Character's Y Position (UP) */
      float zPosition;        /**< Character's Z Position */
      float bloodPosition;    /**< Blood Z Orign*/
      string bloodFileName;   /**< Blood Particle FileName to use */ 
      int sexType;            /**< Thing's Sex Type */
      int age;                /**< Thing's Age */
      int xp;                 /**< The experience of the thing */
      float cr;               /**< The Challenge Rating */
      int xpPercent;          /**< The percentual Left of XP to give */
      float cost;             /**< Cost Value */
      float walk_interval;    /**< The interval of walking (for multiply per
                                   WALK_ACTUALIZATION value. This is for 
                                   temporize the walk animation of the thing) */

      float min[3];           /**< Min points of static bounding box */
      float max[3];           /**< Max points of static bounding box */

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

      /*!
       **********************************************
       *  Get the battle script of this thing
       *  \return -> pointer to iaScript that 
       *             controlls the battle mode
       *             of this thing.
       **********************************************/
      void* getBattleScript();

      /*!
       **********************************************
       *  Set the battle Script of the thing
       * \param script -> pointer to the battle script
       * \param scriptFileName -> filename of the script
       **********************************************/
      void setBattleScript(void* script, string scriptFileName);

      /*!
       **********************************************
       *  Set the thing as enemy to PC
       **********************************************/
      void setAsEnemy();

      /*!
       **********************************************
       *  Set thing as dead (AKA: call die animation
       *  and then put on state dead)
       **********************************************/
       void kill();

       /*!
       **********************************************
       *  Verify if the thing is alive
       *  \return -> true if continue live
       **********************************************/
       bool isAlive();

       /*!
       **********************************************
       *   Virtual method to call dead animation
       **********************************************/
       virtual void callDeadAnimation()=0;

      /*!
       **********************************************
       *   Virtual method to call attack animation
       **********************************************/
       virtual void callAttackAnimation()=0;

      /*!
       **********************************************
       *   Virtual method to call attack animation
       **********************************************/
       virtual void callIdleAnimation()=0;

   protected:
      bool dead;           /**< Dead state or not (if dead is only a corpse) */

      void* battleScript;          /**< Pointer to the battle iaScript */
      string battleScriptFileName; /**< The Battle Script Filename */

};

#endif

