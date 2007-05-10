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
#define ATT_STRENGHT     1
#define ATT_DEXTERY      2
#define ATT_CONSTITUTION 3
#define ATT_INTELIGENCY  4
#define ATT_WISDOW       5
#define ATT_CHARISM      6
#define ATT_SKILL_FIRST  7
#define ATT_SKILL_LAST   46

#define ATT_STR_LEVEL        "ATT_LEVEL"
#define ATT_STR_STRENGHT     "ATT_STRENGHT"
#define ATT_STR_DEXTERY      "ATT_DEXTERY"
#define ATT_STR_CONSTITUTION "ATT_CONSTITUTION"
#define ATT_STR_INTELIGENCY  "ATT_INTELIGENCY"
#define ATT_STR_WISDOW       "ATT_WISDOW"
#define ATT_STR_CHARISM      "ATT_CHARISM"


#define VAR_AUX1         40
#define VAR_AUX2         41
#define VAR_AUX3         42
#define VAR_AUX4         43
#define VAR_AUX5         44
#define VAR_AUX6         45

/******************************************************************
 *                         TENDENCY CONSTANTS                     *
 ******************************************************************/
#define TEND_SL_LIBERTARIO     200
#define TEND_SL_CENTRISTA      201
#define TEND_SL_CAPITALISTA    202
#define TEND_NEUTRO_FUNCIONAL  203
#define TEND_NEUTRO_CENTRISTA  204
#define TEND_NEUTRO_CAOTICO    205
#define TEND_SP_CAPITALISTA    206
#define TEND_SP_NEUTRO         207
#define TEND_SP_CAOTICO        208

#define TEND_STR_SL_LIBERTARIO     "TEND_SL_LIBERTARIO"
#define TEND_STR_SL_CENTRISTA      "TEND_SL_CENTRISTA"
#define TEND_STR_SL_CAPITALISTA    "TEND_SL_CAPITALISTA"
#define TEND_STR_NEUTRO_FUNCIONAL  "TEND_NEUTRO_FUNCIONAL"
#define TEND_STR_NEUTRO_CENTRISTA  "TEND_NEUTRO_CENTRISTA"
#define TEND_STR_NEUTRO_CAOTICO    "TEND_NEUTRO_CAOTICO"
#define TEND_STR_SP_CAPITALISTA    "TEND_SP_CAPITALISTA"
#define TEND_STR_SP_NEUTRO         "TEND_SP_NEUTRO"
#define TEND_STR_SP_CAOTICO        "TEND_SP_CAOTICO"

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


/*!
 **************************************************************** 
 * Convert the concept with name buffer to its internal number  
 * \param buffer -> string with the name of the concept
 * \return the internal number of the constant.
 ***************************************************************/
int numberConstant(string buffer);

/*!
 **************************************************************** 
 * Verifies if the number represents a skill  
 * \param number -> a number that represents some concept
 * \return \c true if is a Skill, \c false otherwise.
 ***************************************************************/
bool isSkill(int number);

/*!
 ************************************************************** 
 * Verifies if the number represents a class 
 * \param number -> a number that represents some concept
 * \return \c true if is a Class, \c false otherwise.
 ***************************************************************/
bool isClass(int number);

/*!
 ************************************************************** 
 * Verifies if the number represents a tendency.  
 * \param number -> a number that represents some concept
 * \return \c true if is a Tendency, \c false otherwise.
 ***************************************************************/
bool isTendency(int number);

/*!
 ************************************************************** 
 * Verifies if the number represents a race  
 * \param number -> a number that represents some concept
 * \return \c true if is a Race, \c false otherwise.
 ***************************************************************/
bool isRace(int number);

/*!
 ************************************************************** 
 * Verifies if the number represents an attribute  
 * \param number -> a number that represents some concept
 * \return \c true if is an Attribute, \c false otherwise.
 ***************************************************************/
bool isAttribute(int number);

/*!
 ************************************************************** 
 * Verifies if the number represents a variable  
 * \param number -> a number that represents some concept
 * \return \c true if is a Variable, \c false otherwise.
 ***************************************************************/
bool isVariable(int number);


#endif
