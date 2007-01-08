#ifndef _modifier_h
#define _modifier_h

#include <string>
using namespace std;

#define SIGNAL_DEC 0
#define SIGNAL_SUM 1

#define MAX_ATTACKS 6

/**! Actuation Factor */
class factor
{
   public:
      string type;    /**< Type of the Factor (ex: Skill, Feat, Class, etc) */
      string id;      /**< ID of the Factor (ex: Intimidation, etc) */
};

/**! Default Modifier */
class modifier
{
   public:
      int mod;            /**< Value of the Modifier */
      factor cause;       /**< Cause of the Modifier */
      factor target;      /**< Target when modifer is valid */
      string description; /**< Modifier Description */
};

/*! Number of Points, based on ATTRIBUTES 
 * the formula is (sum SIGNAL ATTModifier)*mult */
class points
{
   public:
      int sum;          /**< Value of Sum */
      int signal;       /**< Signal Used (- or +) */
      string attID;     /**< ID of the Attribute to use modifier */
      int mult;         /**< Value of Mult */
};

/*! Per Level Bonus and Saves */
class bonusAndSaves
{
   public:
      int level;                          /**< Class Level */
      int baseAttackBonus[MAX_ATTACKS];   /**< Base Attack Bonus */
      int fortSave;                       /**< Fortitude Save Bonus */
      int refSave;                        /**< Reflex Save Bonus */
      int willSave;                       /**< Will Save Bonus */
};


#endif

