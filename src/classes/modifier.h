#ifndef _modifier_h
#define _modifier_h

#include <string>
using namespace std;

#include "defs.h"

#define SIGNAL_DEC 0
#define SIGNAL_SUM 1

#define MOD_TYPE_ATT   "ATTRIBUTE"
#define MOD_TYPE_SKILL "SKILL"

/**! Actuation Factor */
class factor
{
   public:
      /* Default Constructor */
      factor(){type = "ALL"; id = "ALL";};

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


#endif

