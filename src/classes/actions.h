#ifndef _dnt_actions_h
#define _dnt_actions_h

#include <string>
using namespace std;

#define ACT_OPEN                  0
#define ACT_ATTACK                1
#define ACT_DRINK                 2
#define ACT_EAT                   3
#define ACT_DEFEND                4
#define ACT_TALK                  5
#define ACT_CLOSE                 6
#define ACT_MOVE                  7
#define ACT_TAKE                  8
#define ACT_FREE                  9
#define ACT_MODIFY_OWN_CONCEPT   10
#define ACT_MODIFY_OTHER_CONCEPT 11
#define ACT_CONJURE              12
#define ACT_MODIFY_OBJECT        13
#define ACT_HEAL                 14
#define ACT_FIX                  15
#define ACT_BREAK                16
#define ACT_WAIT                 17

#define ACT_STR_OPEN                 "ACT_OPEN"
#define ACT_STR_ATTACK               "ACT_ATTACK"
#define ACT_STR_DRINK                "ACT_DRINK"
#define ACT_STR_EAT                  "ACT_EAT"
#define ACT_STR_DEFEND               "ACT_DEFEND"
#define ACT_STR_TALK                 "ACT_TALK"
#define ACT_STR_CLOSE                "ACT_CLOSE"
#define ACT_STR_MOVE                 "ACT_MOVE"
#define ACT_STR_TAKE                 "ACT_TAKE"
#define ACT_STR_FREE                 "ACT_FREE"
#define ACT_STR_MODIFY_OWN_CONCEPT   "ACT_MODIFY_OWN_CONCEPT"
#define ACT_STR_MODIFY_OTHER_CONCEPT "ACT_MODIFY_OTHER_CONCEPT"
#define ACT_STR_CONJURE              "ACT_CONJURE"
#define ACT_STR_MODIFY_OBJECT        "ACT_MODIFY_OBJECT"
#define ACT_STR_HEAL                 "ACT_HEAL"
#define ACT_STR_FIX                  "ACT_FIX"
#define ACT_STR_BREAK                "ACT_BREAK"
#define ACT_STR_WAIT                 "ACT_WAIT"


#define ACT_TYPE_FREE_ACTION       0
#define ACT_TYPE_NORMAL_ACTION     1
#define ACT_TYPE_SECUNDARY_ACTION  2
#define ACT_TYPE_NO_ACTION         3

#define ACT_TYPE_STR_FREE_ACTION       "ACT_TYPE_FREE_ACTION"
#define ACT_TYPE_STR_NORMAL_ACTION     "ACT_TYPE_NORMAL_ACTION"
#define ACT_TYPE_STR_SECUNDARY_ACTION  "ACT_TYPE_SECUNDARY_ACTION"
#define ACT_TYPE_STR_NO_ACTION         "ACT_TYPE_NO_ACTION"

#define ACT_MOVE_DELTA 5

/*!
 ************************************************************* 
 * Return the number of the action with name buffer  
 * \param buffer -> string with the name of the concept
 * \return correspondent action Type number.
 ***************************************************************/
int numberAction(string buffer);

/*!
 ************************************************************** 
 * Return the number of the Action Type with name buffer  
 * \param buffer -> string with the name of the concept
 * \return correspondent action Type number.
 ***************************************************************/
int numberActionType(string buffer);

#endif

