#ifndef _defs_h
#define _defs_h

/** \file defs.h
 *  Here is where all game rules definitions resides. Again, some of them are define two times, one with the internal number, other with the definition string.
 */

#include "dices.h"
#include <string>
using namespace std;

/******************************************************************
 *                        ATTRIBUTES CONSTANTS                    *
 ******************************************************************/
#define ATT_LEVEL        0
#define ATT_STRENGTH     1
#define ATT_DEXTERITY    2
#define ATT_CONSTITUTION 3
#define ATT_INTELLIGENCE 4
#define ATT_WISDOM       5
#define ATT_CHARISM      6
#define ATT_SKILL_FIRST  7
#define ATT_SKILL_LAST   46

#define VAR_AUX1         40
#define VAR_AUX2         41
#define VAR_AUX3         42
#define VAR_AUX4         43
#define VAR_AUX5         44
#define VAR_AUX6         45

#define WORK_ON_ALL            10000
#define STR_WORK_ON_ALL        "WORK_ON_ALL"
#define NONE                   10001
#define STR_NONE               "NONE" 

/******************************************************************
 *                            SEX CONSTANTS                       *
 ******************************************************************/
#define SEX_MALE        0
#define SEX_FEMALE      1
#define SEX_OTHER       2


#endif
