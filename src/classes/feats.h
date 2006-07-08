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

#define NUMBER_OF_FEATS     2 /* Total Number of Feats */
#define MAX_FEATS          30 /* Max Number of Know Feats for character */
#define MAX_DEP_FEATS       5 /* Max number of cost this, cost that feats */

/*************************************************************************
 * Note: Dependence Feats  are feats  that  has number of uses affected  *
 *  when use actual feat. For Example, when use ameivas-II you'll can do *
 *  -2 ameivaI (1/2 reason).                                             *
 *************************************************************************/
typedef struct _depfeat
{
   float  reason;      /* Dependence Reason (1/1, 1/2, 1/3, 2/1, etc) */
   string featName;    /* Name of the Feat */
   bool   used;        /* Dependence used or not? */
}depFeat;

/******************************************************************************
 * Note: this  is a  definition based  on an attack feature. If the feature is*
 * an   aditional    concept   feature,   the   numbers   in   quantityPerDay,* 
 * aditionalQuantity and aditionalLevels refers to, respectively, the concepts*
 * described on conceptBonus, conceptAgainst and conceptTarget.               *
 ******************************************************************************/
typedef struct _featDesc
{
   int internalListNumber;         /* Number on List */
   int requeridedLevel;            /* Requerided Character class level to use */
   int requeridedClass;            /* Requerided Character Class */
   int quantityPerDay;             /* Quantity avaible to use per day*/
   int aditionalQuantity;          /* Quantity Added per Aditional Level */
   int aditionalLevels;            /* Number of Levels to Aditional Quantity */
   int costToUse;                  /* Cost, in PP to use */
   int actionType;                 /* Action Type of the feat */
   int action;                     /* Defined Action of the feat */
   int conceptBonus;               /* Define the concept that bonus the feat */
   int conceptAgainst;             /* Define the concept against the feat */
   int conceptTarget;              /* Define the valid target of feat */
   diceThing diceInfo;             /* Defined Dice*/
   string name;                    /* Feat Name */
   string description;             /* Feat Description */
   depFeat depFeats[MAX_DEP_FEATS];/* Feat Dependency */
   SDL_Surface* image;             /* Feat Image */
}featDescription;

typedef struct _feat
{
   int internalListNumber;          /* Number on List */
   int requeridedLevel;             /* Requerided Level to use */
   int quantityPerDay;              /* Quantity avaible to use per day*/
   int aditionalQuantity;           /* Quantity Added per Aditional Level */
   int aditionalLevels;             /* Number of Levels to Aditional Quantity */
   int costToUse;                   /* Cost, in PP to use */
   int actionType;                  /* Action Type of the feat */
   int action;                      /* Defined Action of the feat */
   int conceptBonus;                /* Define the concept that bonus the feat */
   int conceptAgainst;              /* Define the concept against the feat */
   int conceptTarget;               /* Define the valid target of feat */
   float actualQuantity;            /* Actual quantity to use */
   diceThing diceInfo;              /* Defined Dice*/
   string name;                     /* Feat Name */
   depFeat depFeats[MAX_DEP_FEATS]; /* Feat Dependency */
}feat;                              /* Feat Struct */

class feats               /* Character's Feats */
{
   public:
      feats();
      ~feats();
 
      /*************************************************************** 
       * Reason: Return the feat with number featNumber  
       * Param:
       *        featNumber -> number of feat to return
       ***************************************************************/
      feat* featByNumber(int featNumber);
      /*************************************************************** 
       * Reason: Return the feat with name featName
       * Param:
       *        featName -> name of feat to return
       ***************************************************************/
      feat* featByName(string featName);
      /*************************************************************** 
       * Reason: Insert a feat on Character's Feats. Return true if ok. 
       * Param:
       *        featInsert -> featDescription of feat to insert
       ***************************************************************/
      bool insertFeat(featDescription featInsert);
      /*************************************************************** 
       * Reason: Refresh Quantities to use on a new day to all feats.
       * Param:
       ***************************************************************/
      void newDay();
      /*************************************************************** 
       * Reason: Apply an attack or break feat. Returns true if used.
       * Param:
       *        featNumber -> Number of Feat on List
       *        target -> target to use the Feat
       *        brief -> string that will have briefing
       ***************************************************************/
      bool applyAttackAndBreakFeat(thing& attacker, int featNumber, 
                                   thing& target, string& brief);

      /*int applyCureAndFixFeat(int featNumber);
      int applyModifyObjectFeat(int featNumber);
      int applyConjureFeat(int featNumber);
      int applyInvocationFeat(int featNumber);*/

   private:
      feat m_feats[MAX_FEATS]; /* Internal Feats Struct */
      int  totalFeats;         /* Actual Number of Feats */

      /*************************************************************** 
       * Reason: Use a quantity of the feat and propagate to other
       *         dependent feats.
       * Param:
       *        featNumber -> feat to use
       ***************************************************************/
       void useFeat(int featNumber);
};


class featsList           /* List of All Feats on Game */
{
   public:
      featsList(string dir, string arq);
      ~featsList();

      /*************************************************************** 
       * Reason: Return the featDescription with name featName
       * Param:
       *        featName -> name of feat to return
       ***************************************************************/
      featDescription featByName(string featName);

      /*************************************************************** 
       * Reason: Return the feat with number featNumber  
       * Param:
       *        featNumber -> number of feat to return
       ***************************************************************/
      featDescription featByNumber(int featNumber);

   private:
      featDescription m_feats[NUMBER_OF_FEATS]; /* Internal Desc Struct */
};

#endif

