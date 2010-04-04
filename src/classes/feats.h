/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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

#ifndef _dnt_feats_h
#define _dnt_feats_h

#include <SDL/SDL.h>
#include "defs.h"
#include <string>
using namespace std;

#include "thing.h"
#include "modifier.h"
#include "modeffect.h"
#include "weapon.h"

#include "../etc/message3d.h"
#include "../engine/briefing.h"
#include "../lang/translate.h"

#define MAX_FEATS          30 /**< Max number of Feats per Character */
#define MAX_DEP_FEATS       5 /**< Max number of cost this, cost that feats */
#define MAX_FEAT_EFFECTS   10 /**< Max number of effects per feats */

#define FEAT_MELEE_ATTACK   0 /**< Melee Attack */
#define FEAT_RANGED_ATTACK  1 /**< Ranged Attack */

/*!
 ************************************************************************
 * Dependence Feats are feats that has number of uses affected  
 *  when use actual feat. \par
 * For Example, when use \e ameivasII you'll can do -2 \e ameivaI. (1/2 reason).
 *************************************************************************/
class depFeat
{
   public:
      float  reason;       /**< Dependence Reason (1/1, 1/2, 1/3, 2/1, etc) */
      string featIDString; /**< ID String of the Feat */
      bool   used;         /**< Dependence used or not? */
};

/*!
 ******************************************************************************
 * This is a definition based on an attack feature. If the feature is
 * an aditional concept feature, the numbers in quantityPerDay, 
 * aditionalQuantity and aditionalLevels refers to, respectively, the concepts
 * described on conceptBonus, conceptAgainst and conceptTarget.               
 ******************************************************************************/
class featDescription
{
   public:
      /*! Constructor */
      featDescription();
      /*! Destructor */
      ~featDescription();

      int internalListNumber;      /**< Number on List */
      int requeridedLevel;         /**< Requerided Character class level */
      factor requeridedFactor;     /**< Requerided Factor (class, race, etc) */
      int quantityPerDay;          /**< Quantity avaible to use per day*/
      int aditionalQuantity;       /**< Quantity Added per AditionalLevel */
      int aditionalLevels;         /**< Number of Levels to AditionalQuantity */
      int costToUse;               /**< Cost, in PP to use the feat */
      int actionType;              /**< Action Type of the feat */
      int action;                  /**< Defined Action of the feat */
      int range;                   /**< Range action of the feat */
      factor conceptBonus;         /**< The concept that bonus the feat */
      factor conceptAgainst;       /**< Define the concept against the feat */
      factor conceptTarget;        /**< Define the valid target of feat */
      diceThing diceInfo;          /**< Defined Dice*/
      string name;                 /**< Feat Name */
      string idString;             /**< Feat ID String */
      string description;             /**< Feat Description */
      depFeat depFeats[MAX_DEP_FEATS];/**< Feat Dependency */
      
      modEffectList* effects;      /**< List of effects */
      SDL_Surface* image;          /**< Feat Image */
};

/*!
 *************************************************************
 * The feat is the internal feat definition per characters,
 * with only the requerided fields to avoid loss memory with
 * unnecessary data.
 *************************************************************/
class feat
{
   public:
      featDescription* info;       /**< The feat info */
      float actualQuantity;        /**< Actual quantity to use */
      float range;                 /**< Actual Range */
      int costToUse;               /**< Cost, in PP to use the feat */
      diceThing diceInfo;          /**< Defined Dice*/
};


/*!
 *************************************************
 * the class feats defines the Character's Feats,
 * and its functions to actualize, use, etc. 
 *************************************************/
class feats               
{
   public:
      /*! feats Constuctor */
      feats();
      /*! feats Destructior */
      ~feats();
 
      /*!
       *************************************************************** 
       * Return the feat with number featNumber  
       * \param featNumber -> internal number of feat to return
       * \return the feat struct with internal number.
       ***************************************************************/
      feat* featByNumber(int featNumber);
      /*!
       **************************************************************** 
       * Return the feat with name featName
       * \param featName -> name of feat to return
       * \return return the feat struct that has the name.
       ***************************************************************/
      feat* featByString(string featName);
      /*!
       **************************************************************** 
       * Insert a feat on Character's Feats.
       * \param featInsert -> featDescription of feat to insert
       * \return \c true if sucefully inserted.
       ***************************************************************/
      bool insertFeat(featDescription* featInsert);
      /*!
       ************************************************************** 
       * Refresh Quantities to use on a new day to all feats.
       ***************************************************************/
      void newDay();
      /*!
       **************************************************************** 
       * Apply an attack or break feat.
       * \param attacker -> thing that will attack
       * \param featNumber -> Number of Feat on List
       * \param target -> target to use the Feat
       * \return \c true if the feat was used.
       ***************************************************************/
      bool applyAttackAndBreakFeat(thing& attacker, int featNumber, 
                                   thing* target);
      /*!
       **************************************************************** 
       * Apply a heal or fix feat.
       * \param attacker -> thing that will attack
       * \param featNumber -> Number of Feat on List
       * \param target -> target to use the Feat
       * \return \c true if the feat was used.
       ***************************************************************/
      bool applyHealAndFixFeat(thing& attacker, int featNumber, 
                               thing* target);
      /*!
       **************************************************************** 
       * Apply a psycho feat.
       * \param attacker -> thing that will attack
       * \param featNumber -> Number of Feat on List
       * \param target -> target to use the Feat
       * \return \c true if the feat was used.
       ***************************************************************/
      bool applyPsychoFeat(thing& attacker, int featNumber, 
                           thing* target);
      /*!
       **************************************************************** 
       * Apply an invocation feat.
       * \param attacker -> thing that will attack
       * \param featNumber -> Number of Feat on List
       * \param target -> target to use the Feat
       * \return \c true if the feat was used.
       ***************************************************************/
      bool applyInvocationFeat(thing& attacker, int featNumber, 
                               thing* target);

      /*!
       **************************************************************** 
       * Get a random attack feat that can be used.
       * \param pers -> character that will use the feat
       * \param target -> thing that will receive the feat.
       * \return \c number of the feat got
       ***************************************************************/
      int getRandomNPCAttackFeat(thing* pers, thing* target);

      /*!
       **************************************************************** 
       * Get the powerfull attack feat that can be used.
       * \param pers -> character that will use the feat
       * \param target -> thing that will receive the feat.
       * \return \c number of the feat got
       ***************************************************************/
      int getPowerfullAttackFeat(thing* pers, thing* target);


      int getFirstHealFeat(thing* pers);

      int getRandomHealFeat(thing* pers);

      int getPowerfullHealFeat(thing* feat);

      /*!
       **************************************************************** 
       * Define the active character's base attack to a weapon.
       * \param w -> the weapon used.
       ***************************************************************/
      void defineWeapon(weapon* w);
      /*!
       **************************************************************** 
       * Flush the Current munition to the current defined weapon
       ***************************************************************/
      void flushCurrentMunition();

      /*!
       **************************************************************** 
       * Get the range type of the attack feat
       * \return range Type of the attack feat
       ***************************************************************/
      int getAttackFeatRangeType();

      /*!
       **************************************************************** 
       * Get the range of the attack feat
       * \return range of the attack feat
       ***************************************************************/
      int getAttackFeatRange();

      /*! Set the bare hands damage dice
       * \param dices -> number of dices
       * \param diceId -> number of faces
       * \param sum -> integer to sum to the result
       * \param crit -> critical multiplier */
      void setBareHandsDamage(int dices, int diceId, int sum, int crit);

   private:

      weapon* currentWeapon;    /**< The current weapon */
      feat m_feats[MAX_FEATS];  /**< Internal Feats Struct */
      int  totalFeats;          /**< Actual Number of Feats */
      diceThing bareHandsDice;  /**< Damage by bare hands */

      /*!
       **************************************************************** 
       * Use a quantity of the feat and propagate to other dependent 
       * feats.
       * \param featNumber -> feat to use
       ***************************************************************/
       void useFeat(int featNumber);

       /*!
       **************************************************************** 
       * Apply feats that will afect the Life Points of the Target
       * \param attacker -> thing that will attack
       * \param featNumber -> Number of Feat on List
       * \param target -> target to use the Feat
       * \param heal -> true if is a heal/fix feat, 
       *                false if is an attack/break one.
       * \return \c true if the feat was used.
       ***************************************************************/
       bool applyHealOrAttackFeat(thing& actor, int featNumber, 
                                  thing* target, bool heal);
};

/*! List of All Feats on Game */
class featsList           
{
   public:
      /*!
       ***************************************************************
       * featsList Constructor
       * \param dir -> string with the directory where all feats are
       * \param arq -> file name with the list of avaible feats.
       ***************************************************************/
      void init(string dir, string arq);
      /*!
       ***************************************************************
       * featsList Destructor
       ***************************************************************/
      void finish();

      /*!
       ************************************************************** 
       * Return the featDescription with idString featName
       * \param featName -> name of feat to return
       * \return the featDescription struct.
       ***************************************************************/
      featDescription* featByString(string featName);

      /*!
       **************************************************************** 
       * Return the feat with number featNumber  
       * \param featNumber -> number of feat to return
       * \return the featDescription struct.
       ***************************************************************/
      featDescription* featByNumber(int featNumber);

   private:
      static featDescription* m_feats; /**< Internal Desc Struct */
      static int totalFeats;           /**< Total Declared Feats */
};

#endif

