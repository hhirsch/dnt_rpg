/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_thing_h
#define _dnt_thing_h

#include "skills.h"
#include "align.h"
#include "modifier.h"
#include "../etc/dirs.h"
#include "../etc/scenenode.h"
#include "../etc/scene.h"

#define PSYCHO_HOSTILE     0  /**< Thing is hostile to PCs */
#define PSYCHO_NEUTRAL     1  /**< Thing is neutral to PCs */
#define PSYCHO_FRIENDLY    2  /**< Thing is friendly to PCs */

#define THING_TYPE_NONE       0  /**< Unknow thing */
#define THING_TYPE_OBJECT     1  /**< Thing is an Object */
#define THING_TYPE_CHARACTER  2  /**< Thing is a character */


#define THING_ARMATURE_CLASS    "ARMATURE_CLASS"
#define THING_SIZE_MODIFIER     "SIZE_MODIFIER"
#define THING_INITIATIVE_BONUS  "INITIATIVE"
#define THING_DISPLACEMENT      "DISPLACEMENT"
#define THING_MAX_LIFE_POINTS   "MAX_LIFE_POINTS"

/*! Define what is a thing on game, in other words, what is the base fields
 * for characters and map objects.*/
class thing
{
   public:
      /*! thing Constructor */
      thing();
      /*! thing virtual Destructor */
      virtual ~thing();

      skills sk;             /**< skills without images and descriptions */
      
      bonusAndSaves curBonusAndSaves; /**< Thing's bonus and saves */
      int armatureClass;      /**< Thing's Armature Class (AC) */
      int displacement;       /**< Thing's Displacement (in meters) */
      int sizeModifier;       /**< Thing's Size Modifier */
      int initiativeBonus;    /**< Thing's initiative bonus value */

      sceneNode* scNode;      /**< Scene node related to the thing */
      float initialXPosition; /**< Thing's initial X Position */
      float initialZPosition; /**< Thing's Initial Z Position */
      float bloodPosition;    /**< Blood Z Orign*/
      string bloodFileName;   /**< Blood Particle FileName to use */ 
      int sexType;            /**< Thing's Sex Type */
      int age;                /**< Thing's Age */
      string name;              /**< Character's name */

      thing* currentEnemy;    /**< Pointer to current fight enemy (target) */

      float cr;               /**< The Challenge Rating */
      float xpPercent;        /**< The percentual Left of XP to give */
      float cost;             /**< Cost Value */
      float weight;           /**< Weight Value */
      float walk_interval;    /**< The interval of walking (for multiply per
                                   WALK_ACTUALIZATION value. This is for 
                                   temporize the walk animation of the thing) */

      //////////////////////////////////////////////////////////////////////
      //                                                                  //
      //                        Dialog Functions                          //
      //                                                                  //
      //////////////////////////////////////////////////////////////////////
      /*! Set the conversation file
       * \param file -> name of the conversation file*/
      void setConversationFile(string file);

      /*! Get the conversation file
       * \return -> conversation fileName */
      string getConversationFile();

      /*! Create the conversation (load all things, but not open the dialog)
       * \param curMap -> string with the map character is in */
      void createConversation(string curMap);

      /*! Set the initial dialog to the character conversation
       * \param i -> initial dialog number */
      void setInitialConversation(int i);

      /*! Get the character conversation
       * \return -> pointer to the character conversation (or NULL) */
      void* getConversation();

      //////////////////////////////////////////////////////////////////////
      //                                                                  //
      //                      Level and XP Functions                      //
      //                                                                  //
      //////////////////////////////////////////////////////////////////////

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

      //////////////////////////////////////////////////////////////////////
      //                                                                  //
      //                 Life and LifePoints Functions                    //
      //                                                                  //
      //////////////////////////////////////////////////////////////////////

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


      //////////////////////////////////////////////////////////////////////
      //                                                                  //
      //                 Skills and States Functions                      //
      //                                                                  //
      //////////////////////////////////////////////////////////////////////

      /*! Do a check (skill, saves, etc) against the difficulty.
       * \param difficulty -> Difficulty of the check
       * \return true if succed, false if failed. */
      bool doCheck(string stateToCheck, int difficulty);

      /*! Get the thing type 
       * \return -> type of the thing */
      int getThingType();
 
      /*! Get the current psycho state
       * \return Psycho state constant */
      int getPsychoState();
      /*! Set the crrent psycho state
       * \param state -> Psycho state constant */
      void setPsychoState(int state);

      /*!  Get the skill bonus (modifier or not).
       *  \param curSkill -> current Skill
       *  \return bonus. */
      int skillBonus(skill* curSkill);
      /*! Get the attribute bonus: modifier.
       *  \param curAttribute -> current attribute pointer.
       *  \return bonus. */
      int attBonus(skill* curAttribute);
      /*! Get the attribute bonus: modifier.
       *  \param curAttribute -> current attribute ID.
       *  \return bonus. */
      int attBonus(int curAttribute);

      /*! Get the bonus Value (modifier or not).
       *  \param something -> number of the definition.
       *  \return bonus. */
      int getBonusValue(factor something);

      /*! Get the pointer to a factor value
       * \param something -> factor info
       * \return pointer to the factor value */
      int* getFactorValuePointer(factor something);


      //////////////////////////////////////////////////////////////////////
      //                                                                  //
      //                       Fight Functions                            //
      //                                                                  //
      //////////////////////////////////////////////////////////////////////

      /*! Get the battle script of this thing
       *  \return -> pointer to iaScript that 
       *             controlls the battle mode
       *             of this thing. */
      void* getBattleScript();

      /*! Set the battle Script of the thing
       * \param script -> pointer to the battle script
       * \param scriptFileName -> filename of the script */
      void setBattleScript(void* script, string scriptFileName);

      /*! Set the thing as enemy to PC */
      void setAsEnemy();

      /*! Set thing as dead (ie. call die animation
       *  and set state as dead) */
      void kill();

      /*! Verify if the thing is alive
       *  \return -> true if continue live */
      bool isAlive();

      /*! Get if the thing still have attack actions at fight's turn
       * \return -> true if have attack actions */
      bool getCanAttack();
      /*! Set if the thing have attack actions at a fight's turn
       * \param can -> true if can, false if can't*/
      void setCanAttack(bool can);
      /*! Get if the thing still have move actions at fight's turn
       * \return -> true if have move actions */
      bool getCanMove();
      /*! Set if the thing have move actions at a fight's turn
       * \param can -> true if can, false if can't*/
      void setCanMove(bool can);

      /*! Get the probability of been killed with a single strike
       * \return probability value [0, 100] */
      int getInstantKilled();

      /* Verify if the point posX,posZ is at range
       * \param posX -> x coordinate
       * \param posZ -> z coordinate 
       * \param range -> max range 
       * return true if at range */
      bool atRange(float posX, float posZ, float range);

      //////////////////////////////////////////////////////////////////////
      //                                                                  //
      //                     Virtual Functions                            //
      //                                                                  //
      //////////////////////////////////////////////////////////////////////

       /*! Virtual Method to update the health bar draw */
       virtual void updateHealthBar()=0;

       /*! Virtual method to call dead animation */
       virtual void callDeadAnimation()=0;

       /*! Virtual method to call attack animation */
       virtual void callAttackAnimation()=0;

       /*! Virtual method to call attack animation */
       virtual void callIdleAnimation()=0;

   protected:
      int upLevels;        /**< Number of levels the thing can advance */
      int xp;              /**< The experience of the thing */
      
      int lifePoints;         /**< Thing's Life points */
      int maxLifePoints;      /**< Thing's Max Life Points */

      int instantKilled;      /**< Probability of been killed 
                                   with a single shoot */
      
      int psychoState;        /**< Thing's Psycho State */
      bool dead;           /**< Dead state or not (if dead is only a corpse) */

      void* battleScript;          /**< Pointer to the battle iaScript */
      string battleScriptFileName; /**< The Battle Script Filename */

      int thingType;              /**< The type of the thing */

      bool canMove;               /**< If still have movements at fight */
      bool canAttack;             /**< If still have actions at fight */

      string conversationFile;    /**< Name of the Conversation File */
      void* conv;                 /**< Pointer to the conversation */

};

#endif

