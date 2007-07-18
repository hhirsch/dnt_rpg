#ifndef _feats_h
#define _feats_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include <SDL/SDL.h>
#include "defs.h"
#include <string>
using namespace std;

#include "thing.h"
#include "modifier.h"
#include "../lang/lang.h"

#define MAX_FEATS          30 /**< Max number of Feats per Character */
#define MAX_DEP_FEATS       5 /**< Max number of cost this, cost that feats */

#define FEAT_MELEE_ATTACK   0 /**< Melee Attack */
#define FEAT_RANGED_ATTACK  1 /**< Ranged Attack */

/*!
 ************************************************************************
 * Dependence Feats are feats that has number of uses affected  
 *  when use actual feat. \par
 * For Example, when use \e ameivasII you'll can do -2 \e ameivaI. (1/2 reason).
 *************************************************************************/
typedef struct _depfeat
{
   float  reason;       /**< Dependence Reason (1/1, 1/2, 1/3, 2/1, etc) */
   string featIDString; /**< ID String of the Feat */
   bool   used;         /**< Dependence used or not? */
}depFeat;

/*!
 ******************************************************************************
 * This is a definition based on an attack feature. If the feature is
 * an aditional concept feature, the numbers in quantityPerDay, 
 * aditionalQuantity and aditionalLevels refers to, respectively, the concepts
 * described on conceptBonus, conceptAgainst and conceptTarget.               
 ******************************************************************************/
typedef struct _featDesc
{
   int internalListNumber;         /**< Number on List */
   int requeridedLevel;            /**< Requerided Character class level */
   factor requeridedFactor;        /**< Requerided Factor (class, race, etc) */
   int quantityPerDay;             /**< Quantity avaible to use per day*/
   int aditionalQuantity;          /**< Quantity Added per AditionalLevel */
   int aditionalLevels;            /**< Number of Levels to AditionalQuantity */
   int costToUse;                  /**< Cost, in PP to use the feat */
   int actionType;                 /**< Action Type of the feat */
   int action;                     /**< Defined Action of the feat */
   int range;                      /**< Range action of the feat */
   factor conceptBonus;            /**< The concept that bonus the feat */
   factor conceptAgainst;          /**< Define the concept against the feat */
   factor conceptTarget;           /**< Define the valid target of feat */
   diceThing diceInfo;             /**< Defined Dice*/
   string name;                    /**< Feat Name */
   string idString;                /**< Feat ID String */
   string description;             /**< Feat Description */
   depFeat depFeats[MAX_DEP_FEATS];/**< Feat Dependency */
   SDL_Surface* image;             /**< Feat Image */
}featDescription;

/*!
 *************************************************************
 * The feat is the internal feat definition per characters,
 * with only the requerided fields to avoid loss memory with
 * unnecessary data.
 *************************************************************/
typedef struct _feat
{
   int internalListNumber;          /**< Number on List */
   int requeridedLevel;             /**< Requerided Level to use */
   int quantityPerDay;              /**< Quantity avaible to use per day*/
   int aditionalQuantity;           /**< Quantity Added per Aditional Level */
   int aditionalLevels;             /**< Number of Levels to AditionalQuantity*/
   int costToUse;                   /**< Cost, in PP to use */
   int actionType;                  /**< Action Type of the feat */
   int action;                      /**< Action of the feat */
   int range;                       /**< Range of Action of the feat */
   factor conceptBonus;             /**< Concept that bonus the feat */
   factor conceptAgainst;           /**< Define the concept against the feat */
   factor conceptTarget;            /**< Define the valid target of feat */
   float actualQuantity;            /**< Actual quantity to use */
   diceThing diceInfo;              /**< Defined Dice*/
   string name;                     /**< Feat Name */
   string idString;                 /**< Feat ID String */
   depFeat depFeats[MAX_DEP_FEATS]; /**< Feat Dependency */
}feat;                              /**< Feat Struct */


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
      bool insertFeat(featDescription featInsert);
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
       * \param brief -> string that will have briefing
       * \return \c true if the feat was used.
       ***************************************************************/
      bool applyAttackAndBreakFeat(thing& attacker, int featNumber, 
                                   thing& target, string& brief);
      /*!
       **************************************************************** 
       * Apply a heal or fix feat.
       * \param attacker -> thing that will attack
       * \param featNumber -> Number of Feat on List
       * \param target -> target to use the Feat
       * \param brief -> string that will have briefing
       * \return \c true if the feat was used.
       ***************************************************************/
      bool applyHealAndFixFeat(thing& attacker, int featNumber, 
                               thing& target, string& brief);
      /*!
       **************************************************************** 
       * Apply a psycho feat.
       * \param attacker -> thing that will attack
       * \param featNumber -> Number of Feat on List
       * \param target -> target to use the Feat
       * \param brief -> string that will have briefing
       * \return \c true if the feat was used.
       ***************************************************************/
      bool applyPsychoFeat(thing& attacker, int featNumber, 
                           thing& target, string& brief);
      /*!
       **************************************************************** 
       * Apply an invocation feat.
       * \param attacker -> thing that will attack
       * \param featNumber -> Number of Feat on List
       * \param target -> target to use the Feat
       * \param brief -> string that will have briefing
       * \return \c true if the feat was used.
       ***************************************************************/
      bool applyInvocationFeat(thing& attacker, int featNumber, 
                               thing& target, string& brief);

      /*!
       **************************************************************** 
       * Test if an attack feat can be used.
       * \param pers -> character that will use the feat
       * \param target -> thing that will receive the feat.
       * \return \c true if can use the feat.
       ***************************************************************/
      int getNPCAttackFeat(thing* pers, thing* target);

      /*!
       **************************************************************** 
       * Define the active character's base attack to be a melee one.
       * \param weaponDice -> the dice of the weapon used.
       * \param rangeValue -> range of action
       ***************************************************************/
      void defineMeleeWeapon(diceThing& weaponDice, int rangeValue);
      /*!
       **************************************************************** 
       * Define the active character's base attack to be a ranged one.
       * \param weaponDice -> the dice of the weapon used.
       * \param rangeValue -> range of the weapon
       ***************************************************************/
      void defineRangedWeapon(diceThing& weaponDice, int rangeValue);

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


   private:
      feat m_feats[MAX_FEATS];  /**< Internal Feats Struct */
      int  totalFeats;          /**< Actual Number of Feats */
      lang language;            /**< Language Internationalization */

      /*!
       **************************************************************** 
       * Use a quantity of the feat and propagate to other dependent 
       * feats.
       * \param featNumber -> feat to use
       ***************************************************************/
       void useFeat(int featNumber);
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
      featsList(string dir, string arq);
      /*!
       ***************************************************************
       * featsList Destructor
       ***************************************************************/
      ~featsList();

      /*!
       ************************************************************** 
       * Return the featDescription with idString featName
       * \param featName -> name of feat to return
       * \return the featDescription struct.
       ***************************************************************/
      featDescription featByString(string featName);

      /*!
       **************************************************************** 
       * Return the feat with number featNumber  
       * \param featNumber -> number of feat to return
       * \return the featDescription struct.
       ***************************************************************/
      featDescription featByNumber(int featNumber);

   private:
      featDescription* m_feats; /**< Internal Desc Struct */
      int totalFeats;           /**< Total Declared Feats */
};

#endif

