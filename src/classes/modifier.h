#ifndef _modifier_h
#define _modifier_h

#include <string>
using namespace std;


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

#endif

