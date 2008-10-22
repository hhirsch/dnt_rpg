#ifndef _thing_h
#define _thing_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "skills.h"
#include "align.h"
#include "modifier.h"
#include "../etc/dirs.h"

#define PSYCHO_HOSTILE     0  /**< Thing is hostile to PCs */
#define PSYCHO_NEUTRAL     1  /**< Thing is neutral to PCs */
#define PSYCHO_FRIENDLY    2  /**< Thing is friendly to PCs */

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

      
      int fortitude;          /**< Thing's Fortitude */
      int reflex;             /**< Thing's Reflexes */
      int iAmNotAFool;        /**< Thing's I'm not a fool */
      int armatureClass;      /**< Thing's Armature Class (AC) */
      int displacement;       /**< Thing's Displacement (in meters) */
      int baseAttackModifier; /**< Thing's Base Attack Modifier */
      int sizeModifier;       /**< Thing's Size Modifier */

      float orientation;      /**< Thing's orientation (angle) */
      float xPosition;        /**< Thing's X Position */
      float yPosition;        /**< Thing's Y Position (UP) */
      float zPosition;        /**< Thing's Z Position */
      float initialXPosition; /**< Thing's initial X Position */
      float initialZPosition; /**< Thing's Initial Z Position */
      float bloodPosition;    /**< Blood Z Orign*/
      string bloodFileName;   /**< Blood Particle FileName to use */ 
      int sexType;            /**< Thing's Sex Type */
      int age;                /**< Thing's Age */
      string name;              /**< Character's name */

      float cr;               /**< The Challenge Rating */
      float xpPercent;        /**< The percentual Left of XP to give */
      float cost;             /**< Cost Value */
      float weight;           /**< Weight Value */
      float walk_interval;    /**< The interval of walking (for multiply per
                                   WALK_ACTUALIZATION value. This is for 
                                   temporize the walk animation of the thing) */

      float min[3];           /**< Min points of static bounding box */
      float max[3];           /**< Max points of static bounding box */


      /*! Get the number of levels pending to up
       * \return -> number of levels the thing can up
       * \note -> 0 means the thing can't advance an levels */
      int getUpLevels();
      /*! Decrement the number of levels the character can advance */
      void decUpLevels();

      /*! Get the number of eXperience Points of the thing
       * \return -> current XP */
      int getXP();
      /*! Add some experience points to the thing
       * \param points -> points to add to current XP */
      void addXP(int points);
      /*! Set the current XP of the thing
       * \param points -> new current experince */
      void setXP(int points);

      /*! Get the current number of life points
       * \return -> current life points */
      int getLifePoints();
      /*! Add some points to the current life
       * \param points -> points to add */
      void addLifePoints(int points);
      /*! Set the current life points
       * \param points -> new life value */
      void setLifePoints(int points);

      /*! Get the current max life points
       * \return -> max life points */
      int getMaxLifePoints();
      /*! Set the current max life points
       * \param points -> new max life points*/
      void setMaxLifePoints(int points);

      /*! Get the current psycho state
       * \return Psycho state constant */
      int getPsychoState();
      /*! Set the crrent psycho state
       * \param state -> Psycho state constant */
      void setPsychoState(int state);
      
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
      int upLevels;        /**< Number of levels the thing can advance */
      int xp;              /**< The experience of the thing */
      
      int lifePoints;         /**< Thing's Life points */
      int maxLifePoints;      /**< Thing's Max Life Points */
      
      int psychoState;        /**< Thing's Psycho State */
      bool dead;           /**< Dead state or not (if dead is only a corpse) */

      void* battleScript;          /**< Pointer to the battle iaScript */
      string battleScriptFileName; /**< The Battle Script Filename */

};

#endif

