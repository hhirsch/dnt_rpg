/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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

#ifndef _dnt_iafuncs_h
#define _dnt_iafuncs_h

#include <string>
#include <iostream>
using namespace std;


/* Operators */
#define IA_OPERATOR_ADDITION       "+"
#define IA_OPERATOR_SUBTRACTION    "-"
#define IA_OPERATOR_MULTIPLICATION "*"
#define IA_OPERATOR_DIVISION       "/"
#define IA_OPERATOR_MODULUS        "%"
#define IA_OPERATOR_AND            "&&"
#define IA_OPERATOR_OR             "||"
#define IA_OPERATOR_NOT            "!"

#define IA_OPERATOR_EQUAL          "=="
#define IA_OPERATOR_NOT_EQUAL      "!="
#define IA_OPERATOR_LESSER         "<" 
#define IA_OPERATOR_GREATER        ">"
#define IA_OPERATOR_GEQUAL         ">="
#define IA_OPERATOR_LEQUAL         "<="

#define IA_OPERATOR_ASSIGN         "="

#define IA_OPERATOR_LPARENTHESIS   "("
#define IA_OPERATOR_RPARENTHESIS   ")"

/* Separators */
#define IA_SEPARATOR_COMMA         ","
#define IA_SEPARATOR_SEMI_COLON    ';'
#define IA_SEPARATOR_SPACE         ' '
#define IA_SEPARATOR_TAB           '\t'
#define IA_SEPARATOR_NEW_LINE      '\n'
#define IA_SEPARATOR_NULL          '\0'
#define IA_SEPARATOR_STRING        '"'


/* Comments */
#define IA_COMMENT_LINE            '#'

/* Types */
#define IA_TYPE_VOID "void"
#define IA_TYPE_BOOL "bool"
#define IA_TYPE_INT "int" 
#define IA_TYPE_FLOAT "float"
#define IA_TYPE_STRING "string"
#define IA_TYPE_CHARACTER "character"
#define IA_TYPE_OBJECT "object"

/* Setences */
#define IA_SETENCE_SCRIPT "script"

#define IA_SETENCE_IF "if"
#define IA_SETENCE_ELSE "else"

#define IA_SETENCE_WHILE "while"

#define IA_SETENCE_END "end"

////////////////////////////////////////////////////
//                Quick Pointers                  //
////////////////////////////////////////////////////
#define IA_SELF_OBJECT "SELF_OBJECT"  /**< As the owner object of the script */
#define IA_SELF_CHARACTER "SELF_CHARACTER" /**< As script's owner character */
#define IA_ACTIVE_CHARACTER "ACTIVE_CHARACTER" /**< The Active PC */
#define IA_ACTUAL_MAP "ACTUAL_MAP" /**< The actual openned map */
#define IA_SELF_MISSION "SELF_MISSION" /**< As the script's owner mission */

////////////////////////////////////////////////////
//              Imediate Expressions              //
////////////////////////////////////////////////////
#define IA_TRUE "true"     /**< Boolean TRUE value */
#define IA_FALSE "false"   /**< Boolean FALSE value */

////////////////////////////////////////////////////
//                  Time Functions                //
////////////////////////////////////////////////////
/*! bool wait(int seconds) */
#define IA_WAIT "wait"

////////////////////////////////////////////////////
//               Debug Functions                  //
////////////////////////////////////////////////////
/*! void print(string msg) */
#define IA_DEBUG_PRINT "print"

////////////////////////////////////////////////////
//             Movimentation Functions            //
////////////////////////////////////////////////////
/*! bool moveToPosition(character char, int x, int z) */
#define IA_MOVE_TO_POSITION "moveToPosition"
/*! bool moveToCharacter(character char, character tgt) */
#define IA_MOVE_TO_CHARACTER "moveToCharacter"
/*! bool moveToObject(character char, object obj) */
#define IA_MOVE_TO_OBJECT "moveToObject"
/*! void setIdle(character char) */
#define IA_SET_IDLE "setIdle"

////////////////////////////////////////////////////
//                Mission Functions               //
////////////////////////////////////////////////////
/*! void missionAdd(string missionFile) */
#define IA_MISSION_ADD  "missionAdd"
/*! void missionComplete(string missionFile, int cType) */
#define IA_MISSION_COMPLETE "missionComplete"
/*! void missionAbort(string missionFile) */
#define IA_MISSION_ABORT "missionAbort"
/*! bool missionIsActive(string missionFile) */
#define IA_MISSION_IS_ACTIVE "missionIsActive"
/*! void missionSetTemp(string missionfile, int tmpNumber, int value) */
#define IA_MISSION_SET_TEMP "missionSetTemp"
/*! int missionGetTemp(string missionFile, int tmpNumber) */
#define IA_MISSION_GET_TEMP "missionGetTemp"
/*! void missionSetXp(string missionFile, int xpValue) */
#define IA_MISSION_SET_XP "missionSetXp"

////////////////////////////////////////////////////
//                 Feats Functions                //
////////////////////////////////////////////////////
/*! int featActualQuantity(int featID) */
#define IA_FEAT_ACTUAL_QUANTITY "featActualQuantity"
/*! int featCost(int featID) */
#define IA_FEAT_COST  "featCost"
/*! int featQuantityPerDay(int featID) */
#define IA_FEAT_QUANTITY_PER_DAY "featQuantityPerDay"
/*! void featUseAtCharacter(int featID, character target) */
#define IA_FEAT_USE_AT_CHARACTER "featUseAtCharacter"
#define IA_FEAT_USE_AT_OBJECT "featUseAtObject"
/*! int getRandomAttackFeat(character target)
 * This function get a random attack feat that can be used by
 * the characterOwner of the script on its defined target. */
#define IA_FEAT_GET_RANDOM_ATTACK "getRandomAttackFeat"
/*! int getPowerfullAttackFeat(character target)
 * This function get the powerfull attack feat that the characterOwner
 * of the script can use at its defined target */
#define IA_FEAT_GET_POWERFULL_ATTACK "getPowerfullAttackFeat"
/*! int getRandomHealFeat(character target) */
#define IA_FEAT_GET_RANDOM_HEAL "getRandomHealFeat"
/*! int getPowerfullHealFeat(character target) */
#define IA_FEAT_GET_POWERFULL_HEAL "getPowerfullHealFeat"

////////////////////////////////////////////////////
//                Skills Functions                //
////////////////////////////////////////////////////
/*! int skillPoints(character c, string skillID) */
#define IA_SKILL_POINTS "skillPoints"
/*! int attModifier(character c, string attID) */
#define IA_ATT_MODIFIER "attModifier"

////////////////////////////////////////////////////
//                  Fight Functions               //
////////////////////////////////////////////////////
/*! void combatEnter() */
#define IA_FIGHT_ENTER "combatEnter"
#define IA_FIGHT_EXIT "combatExit"
/*! void runAwayFromBattle() */
#define IA_FIGHT_RUN_AWAY_FROM_BATTLE "runAwayFromBattle"
/*! character getNearestEnemy(character dude) */
#define IA_FIGHT_GET_NEAREST_ENEMY "getNearestEnemy"
/*! bool canAttack() */
#define IA_FIGHT_CAN_ATTACK "canAttack"

////////////////////////////////////////////////////
//                 Object Functions               //
////////////////////////////////////////////////////
/*! object getObject(string fileName, float posX, float posY, float posZ) */
#define IA_GET_OBJECT "getObject"

/*! int getObjectState(object obj) */
#define IA_GET_OBJECT_STATE "getObjectState"


////////////////////////////////////////////////////
//                Character Functions             //
////////////////////////////////////////////////////
/*! character getNPCBytName(string c) */
#define IA_GET_NPC_BY_NAME "getNPCByName"

/*! bool isAlive(character c) */
#define IA_CHARACTER_IS_ALIVE "isAlive"

/*! int getPsycho(character c)  */
#define IA_CHARACTER_GET_PSYCHO "getPsycho"
/*! void setPsycho(character c, int psychoState)  */
#define IA_CHARACTER_SET_PSYCHO "setPsycho"
/*! int getActualLife(character c) */
#define IA_CHARACTER_GET_ACTUAL_LIFE "getActualLife"
/*! int getMaxLife(character c) */
#define IA_CHARACTER_GET_MAX_LIFE "getMaxLife"
#define IA_CHARACTER_HAVE_HEAL_FUNCTION "haveHealFunction"
#define IA_CHARACTER_HEAL_CHARACTER "healCharacter"
/*! bool characterAtRange(character ref, character target) */
#define IA_CHARACTER_AT_RANGE "characterAtRange"


////////////////////////////////////////////////////
//                Inventory Functions             //
////////////////////////////////////////////////////
/*! bool inventoryHave(character owner, string objectFile) */
#define IA_INVENTORY_HAVE "inventoryHave"

////////////////////////////////////////////////////
//                  Dialog Functions              //
////////////////////////////////////////////////////
/*! void dialogSetInitial(character owner, string map, int dialogNumber) */
#define IA_DIALOG_SET_INITIAL "dialogSetInitial"

/*! Convert the infix string to a postfix one
 * \param infix -> infix expression 
 * \return -> postfix expression */
 string toPostFix(string infix);

/*! Get the precedence number of the operator
 * \param s -> operator string
 * \return -> precedence number of the operator */
 int precedence(string s);

/*! Get the next token on the string from the position pos.
 * \param s -> string to get the token
 * \param pos -> position where the last token was got.
 * \return -> the token string got. "" if no more tokens exists.*/
 string nextToken(string s, unsigned int& pos);

/*! Verify if the string is a operator
 * \param c -> token to verify
 * \return true if is operator. */
 bool isOperator(string c);

/*! Verify if the string is a type
 * \param s -> token to verify
 * \return -> true if the string is a type. */
 bool isType(string s);

/*! Verify if the string is a function name or not.
 * \param s -> token to verify.
 * \return true if is a function name*/
 bool isFunction(string s);

/*! Verify if the string is a float literal
 * \param s -> string to verify 
 * \return true if is float. */
 bool isFloat(string s);
/*! Verify if the string is an integer literal 
 * \param s -> string to verify
 * \return true if is integer */
 bool isInteger(string s);
/*! Verify if the string is an string literal
 * \param s -> string to verify
 * \return true if is an string literal */
 bool isString(string s);

/*! Get the function type
 * \param s -> function name
 * \return -> the type of the function. default is void. */
 string functionType(string s);


#endif

