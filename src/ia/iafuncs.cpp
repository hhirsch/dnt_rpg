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

#include "iafuncs.h"

/***********************************************************************
 *                            isInteger                                *
 ***********************************************************************/
bool isInteger(string s)
{
   int i;
   for(i = 0; i < (int)s.length(); i++)
   {
      if( (s[i] != '0') && (s[i] != '1') && (s[i] != '2') &&
          (s[i] != '3') && (s[i] != '4') && (s[i] != '5') &&
          (s[i] != '6') && (s[i] != '7') && (s[i] != '8') &&
          (s[i] != '9') )
      {
         return(false);
      }
   }
   return(true);
}

/***********************************************************************
 *                             isFloat                                 *
 ***********************************************************************/
bool isFloat(string s)
{
   int i;
   for(i = 0; i < (int)s.length(); i++)
   {
      if( (s[i] != '0') && (s[i] != '1') && (s[i] != '2') &&
          (s[i] != '3') && (s[i] != '4') && (s[i] != '5') &&
          (s[i] != '6') && (s[i] != '7') && (s[i] != '8') &&
          (s[i] != '9') && (s[i] != '.') )
      {
         return(false);
      }
   }
   return(true);
}

/***********************************************************************
 *                             isString                                *
 ***********************************************************************/
bool isString(string s)
{
   return( (s[0] == IA_SEPARATOR_STRING) && 
           (s[s.length()-1] == IA_SEPARATOR_STRING));
}


/***********************************************************************
 *                            isOperator                               *
 ***********************************************************************/
bool isOperator(string c)
{
   return( (c == IA_OPERATOR_ADDITION) || (c == IA_OPERATOR_SUBTRACTION) ||
           (c == IA_OPERATOR_MULTIPLICATION) || (c == IA_OPERATOR_DIVISION) ||
           (c == IA_OPERATOR_AND) || (c == IA_OPERATOR_OR) ||
           (c == IA_OPERATOR_NOT) || (c == IA_OPERATOR_MODULUS) ||
           (c == IA_OPERATOR_EQUAL) || (c == IA_OPERATOR_NOT_EQUAL) ||
           (c == IA_OPERATOR_LESSER) || (c == IA_OPERATOR_GREATER) ||
           (c == IA_OPERATOR_GEQUAL) || (c == IA_OPERATOR_LEQUAL) ||
           (c == IA_OPERATOR_ASSIGN) || (c == IA_OPERATOR_LPARENTHESIS) ||
           (c == IA_OPERATOR_RPARENTHESIS) || (c == IA_SEPARATOR_COMMA) );
}

/***********************************************************************
 *                              isType                                 *
 ***********************************************************************/
bool isType(string s)
{
   return( (s == IA_TYPE_VOID) || (s == IA_TYPE_BOOL) ||
           (s == IA_TYPE_INT) || (s == IA_TYPE_FLOAT) ||
           (s == IA_TYPE_STRING) || (s == IA_TYPE_CHARACTER) ||  
           (s == IA_TYPE_OBJECT) );

}

/***********************************************************************
 *                            isFunction                               *
 ***********************************************************************/
bool isFunction(string s)
{
   return( (s == IA_DEBUG_PRINT) || 
           (s == IA_MOVE_TO_POSITION) || (s == IA_MOVE_TO_CHARACTER) ||
           (s == IA_MOVE_TO_OBJECT) || (s == IA_SET_IDLE) ||
           (s == IA_WAIT) ||
           (s == IA_FEAT_ACTUAL_QUANTITY) || (s == IA_FEAT_QUANTITY_PER_DAY) ||
           (s == IA_FEAT_COST) || (s == IA_FEAT_USE_AT_CHARACTER) ||
           (s == IA_FEAT_USE_AT_OBJECT) || (s == IA_FEAT_GET_RANDOM_ATTACK) ||
           (s == IA_FEAT_GET_POWERFULL_ATTACK) || 
           (s == IA_FEAT_GET_RANDOM_HEAL) ||
           (s == IA_FEAT_GET_POWERFULL_HEAL) || (s == IA_SKILL_POINTS) ||
           (s == IA_ATT_MODIFIER) || (s == IA_FIGHT_ENTER) ||
           (s == IA_FIGHT_EXIT) || (s == IA_FIGHT_CAN_ATTACK) ||
           (s == IA_FIGHT_RUN_AWAY_FROM_BATTLE) ||
           (s == IA_GET_OBJECT) || (s == IA_GET_OBJECT_STATE) ||
           (s == IA_CHARACTER_IS_ALIVE) || (s == IA_CHARACTER_IS_ALL_DEAD) ||
           (s == IA_CHARACTER_GET_PSYCHO) ||
           (s == IA_CHARACTER_SET_PSYCHO) || (s == IA_CHARACTER_AT_RANGE) || 
           (s == IA_CHARACTER_GET_ACTUAL_LIFE) ||
           (s == IA_CHARACTER_GET_MAX_LIFE) || (s == IA_SELF_OBJECT) ||
           (s == IA_SELF_CHARACTER) || (s == IA_SELF_MISSION) ||
           (s == IA_ACTIVE_CHARACTER) ||
           (s == IA_ACTUAL_MAP) || (s == IA_TRUE) || (s == IA_FALSE) ||
           (s == IA_MISSION_ADD) || (s == IA_MISSION_COMPLETE) ||
           (s == IA_MISSION_ABORT) ||
           (s == IA_MISSION_IS_ACTIVE) || (s == IA_MISSION_SET_TEMP) ||
           (s == IA_MISSION_GET_TEMP) || (s == IA_MISSION_SET_XP) ||
           (s == IA_INVENTORY_HAVE) || (s == IA_FIGHT_GET_NEAREST_ENEMY) ||
           (s == IA_DIALOG_SET_INITIAL) || (s == IA_GET_NPC_BY_NAME) ||
           (s == IA_MAP_TRAVEL) );
}

/***********************************************************************
 *                           functionType                              *
 ***********************************************************************/
string functionType(string s)
{
   /* Boolean Functions */
   if( (s == IA_MOVE_TO_POSITION) || (s == IA_MOVE_TO_CHARACTER) ||
       (s == IA_MOVE_TO_OBJECT) || (s == IA_FEAT_USE_AT_CHARACTER) ||
       (s == IA_FEAT_USE_AT_OBJECT) || (s == IA_TRUE) || (s == IA_FALSE) ||
       (s == IA_MISSION_IS_ACTIVE) || (s == IA_INVENTORY_HAVE) ||
       (s == IA_CHARACTER_IS_ALIVE) || (s == IA_CHARACTER_IS_ALL_DEAD) ||
       (s == IA_CHARACTER_AT_RANGE) || (s == IA_WAIT) || 
       (s == IA_FIGHT_CAN_ATTACK) )
   {
      return(IA_TYPE_BOOL);
   }
   /* Integer Functions */
   else if( (s == IA_FEAT_ACTUAL_QUANTITY) || (s == IA_FEAT_COST) || 
            (s == IA_SKILL_POINTS) || (s == IA_ATT_MODIFIER) ||
            (s == IA_CHARACTER_GET_PSYCHO) || (s == IA_MISSION_GET_TEMP) ||
            (s == IA_CHARACTER_GET_ACTUAL_LIFE) || 
            (s == IA_FEAT_QUANTITY_PER_DAY) ||
            (s == IA_CHARACTER_GET_MAX_LIFE ) || 
            (s == IA_FEAT_GET_RANDOM_ATTACK) ||
            (s == IA_FEAT_GET_POWERFULL_ATTACK) ||
            (s == IA_FEAT_GET_RANDOM_HEAL) ||
            (s == IA_FEAT_GET_POWERFULL_HEAL) ||
            (s == IA_GET_OBJECT_STATE) )
   {
      return(IA_TYPE_INT);
   }
   /* String Functions */
   else if( (s == IA_SELF_MISSION) || (s == IA_ACTUAL_MAP) )
   {
      return(IA_TYPE_STRING);
   }
   /* Character Functions */
   else if( (s == IA_SELF_CHARACTER) || (s == IA_ACTIVE_CHARACTER) ||
            (s == IA_FIGHT_GET_NEAREST_ENEMY) || (s == IA_GET_NPC_BY_NAME) )
   {
      return(IA_TYPE_CHARACTER);
   }
   /* Object Function */
   else if( (s == IA_SELF_OBJECT) || (s == IA_GET_OBJECT) )
   {
      return(IA_TYPE_OBJECT);
   }
   /* Void Functions */
   return(IA_TYPE_VOID);
}


/***********************************************************************
 *                           precedence                                *
 ***********************************************************************/
int precedence(string s)
{
   int value = -1;

   /* Logical Negation */
   if(s == IA_OPERATOR_NOT)
   {
      value = 1;
   }
   /* Multiply Operators */
   else if( (s == IA_OPERATOR_MULTIPLICATION) || (s == IA_OPERATOR_DIVISION) ||
            (s == IA_OPERATOR_MODULUS) )
   {
      value = 2;
   }
   /* Addition Operators */
   else if( (s == IA_OPERATOR_ADDITION) || (s == IA_OPERATOR_SUBTRACTION))
   {
      value = 3;
   }
   /* Dimension Comparator */
   else if( (s == IA_OPERATOR_LESSER) || (s == IA_OPERATOR_GREATER) ||
            (s == IA_OPERATOR_LEQUAL) || (s == IA_OPERATOR_GEQUAL) )
   {
      value = 4;
   }
   /* Equal Comparations */
   else if( (s == IA_OPERATOR_EQUAL) || (s == IA_OPERATOR_NOT_EQUAL) )
   {
      value = 5;
   }
   /* AND Operators */
   else if(s == IA_OPERATOR_AND)
   {
      value = 6;
   }
   /* OR Operator */
   else if(s == IA_OPERATOR_OR)
   {
      value = 7;
   }
   /* Assign Operator */
   else if(s == IA_OPERATOR_ASSIGN)
   {
      value = 8;
   }
   /* Parenthesis */
   if( (s == IA_OPERATOR_LPARENTHESIS) || (s == IA_OPERATOR_RPARENTHESIS))
   {
      value = 9;
   }

   return(value);
}

/***********************************************************************
 *                            nextToken                                *
 ***********************************************************************/
string nextToken(string s, unsigned int& pos)
{
   string ret = "";
   string cmp = "";

   char first = ' ';

   /* Ignore all previous spaces (and commas) */
   while( (pos < s.length()) && ( (s[pos] == IA_SEPARATOR_SPACE) || 
          (s[pos] == IA_SEPARATOR_TAB) || (s[pos] == IA_SEPARATOR_NEW_LINE) || 
          (s[pos] == IA_SEPARATOR_NULL) || (s[pos] == IA_SEPARATOR_COMMA[0]) ||
		  (s[pos] == 13)) )
   {
      pos++;
   }

   if(pos >= s.length())
   {  
      return("");
   }

   cmp = s[pos];

   if(isOperator(cmp))
   {
      /* If the character is a operator (or part of one), read until
       * it isn't anymore a operator. */
      ret = s[pos];
      while(isOperator(ret))
      {
         pos++;
         ret += s[pos];
      }
      /* Remove the character that make the string became a non operator */
      ret.erase(ret.length()-1);
   }
   else
   {
      /* If the character isn't a operator, read until
       * find separator or a operator part */
      first = s[pos];
      if(first == IA_SEPARATOR_STRING)
      {
         ret += s[pos];
         pos++;
      }

      if( first == IA_SEPARATOR_STRING )
      {
         /* Treat as a literal string */
         while( (pos < s.length()) && (s[pos] != IA_SEPARATOR_STRING) )
         {
            ret += s[pos];
            pos++;
         }
         /* Get the last " */
         if(pos < s.length())
         {
            ret += s[pos];
            pos++;
         }
      }
      else
      {
         /* Treat as an normal input */
         while( (pos < s.length()) && (s[pos] != ';') && (s[pos] != ',') &&
                (s[pos] != '\n') && (s[pos] != '\0') && (s[pos] != ' ') &&
                (s[pos] != '\t') && (s[pos] != 13) && (!isOperator(cmp)) )
          {
            ret += s[pos];
            pos++;
            cmp = s[pos];
         }
      }
   }

   return(ret);
}

/***********************************************************************
 *                            toPostFix                                *
 ***********************************************************************/
string toPostFix(string infix)
{
   string postfix = "";
   string stack[1024];
   int stackPos = 0;
   unsigned int l = 0;
   string token = nextToken(infix, l);
   while(!token.empty())
   {
      if(isOperator(token))
      {
         if(stackPos == 0)
         {
            if(token == IA_OPERATOR_RPARENTHESIS)
            {
               /* Received a parenthesis with empty stack, Error! */
               cerr << "Unexpected Right Parenthesis at " << infix << endl;
            }
            else
            {
               /* Stack is Empty, so add the operator */
               stack[stackPos] = token;
               stackPos++;
            }
         }
         else
         {
            /* Stack isn't empty */
            if(token == IA_OPERATOR_LPARENTHESIS)
            {
               /* Left parenthesis, only put it at the stack */
               stack[stackPos] = token;
               stackPos++;
            }
            else if(token == IA_OPERATOR_RPARENTHESIS)
            {
               /* Empty all stack to the left parenthesis */
               while( (stackPos > 0) && 
                      (stack[stackPos-1] != IA_OPERATOR_LPARENTHESIS) )
               {
                  stackPos--;
                  postfix += " " + stack[stackPos];
               }

               if( (stackPos <= 0) || 
                   (stack[stackPos-1] != IA_OPERATOR_LPARENTHESIS) )
               {
                  cerr << "Not found left parenthesis at " << infix << endl;
               }
               else
               {
                  /* Ignore the Left Parenthesis too. */
                  stackPos--;
               }
            }
            else if(precedence(token) < precedence(stack[stackPos-1]))
            {
               /* The precedence is higher, so push it to the stack */
               stack[stackPos] = token;
               stackPos++;
            }
            else
            {
               /* Pop Until Precedence is greater or no more elements */
               while( (stackPos > 0) && 
                      (precedence(token) > precedence(stack[stackPos-1])) )
               {
                  /* Remove from stack the top */
                  postfix += " " + stack[stackPos-1];
                  stackPos--;
               }

               /* Put the operator to the stack */
               stack[stackPos] = token;
               stackPos++;
            }
         }
      }
      else /* is Operand */
      { 
         postfix += " " + token;
      }
      token = nextToken(infix, l);
   }

   /* Empty the stack */
   while(stackPos > 0)
   {
      stackPos--;
      postfix += " " + stack[stackPos];
   }

   return(postfix);
}




