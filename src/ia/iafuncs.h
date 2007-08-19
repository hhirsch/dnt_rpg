#ifndef _iafuncs_h
#define _iafuncs_h

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


/* Quick Pointers */
#define IA_SELF_OBJECT "SELF_OBJECT"  /**< As the owner object of the script */
#define IA_SELF_CHARACTER "SELF_CHARACTER" /**< As script's owner character */
#define IA_ACTIVE_CHARACTER "ACTIVE_CHARACTER" /**< The Active PC */
#define IA_ACTUAL_MAP "ACTUAL_MAP" /**< The actual openned map */

/* Imediate Expressions */
#define IA_TRUE "true"
#define IA_FALSE "false"

/* Movimentation Functions */
#define IA_MOVE_TO_POSITION "void moveToPosition"
#define IA_MOVE_TO_CHARACTER "void moveToCharacter"
#define IA_MOVE_TO_OBJECT "void moveToObject"

/* Feats Functions */
#define IA_FEAT_TOTAL "int featTotal"
#define IA_FEAT_COST  "int featCost"
#define IA_FEAT_USE_AT_CHARACTER "void featUseAtCharacter"
#define IA_FEAT_USE_AT_OBJECT "void featUseAtObject"

/* Skills Functions */
#define IA_SKILL_POINTS "int skillPoints"
#define IA_SKILL_MODIFIER "int skillModifier"

/* Fight Functions */
#define IA_FIGHT_ENTER "void combatEnter"
#define IA_FIGHT_ExIT "void combartExit"

/* Character Functions */
#define IA_CHARACTER_GET_PSYCHO "string getPsycho"
#define IA_CHARACTER_SET_PSYCHO "string setPsycho"


/*! Convert the infix string to a postfix one
 * \param infix -> infix expression 
 * \return -> postfix expression */
 string toPostFix(string infix);

 bool isOperator(string c);
 int precedence(string s);
 string nextToken(string s, unsigned int& pos);


#endif

