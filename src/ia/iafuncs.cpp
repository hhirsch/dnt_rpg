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
           (s == IA_TYPE_OBJECT) || (s == IA_TYPE_FEAT) ||
           (s == IA_TYPE_SKILL) || (s == IA_TYPE_MAP) );

}

/***********************************************************************
 *                            isFunction                               *
 ***********************************************************************/
bool isFunction(string s)
{
   return( (s == IA_MOVE_TO_POSITION) || (s == IA_MOVE_TO_CHARACTER) ||
           (s == IA_MOVE_TO_OBJECT) || (s == IA_FEAT_TOTAL) ||
           (s == IA_FEAT_COST) || (s == IA_FEAT_USE_AT_CHARACTER) ||
           (s == IA_FEAT_USE_AT_OBJECT) || (s == IA_SKILL_POINTS) ||
           (s == IA_SKILL_MODIFIER) || (s == IA_FIGHT_ENTER) ||
           (s == IA_FIGHT_EXIT) || (s == IA_CHARACTER_GET_PSYCHO) ||
           (s == IA_CHARACTER_SET_PSYCHO) || (s == IA_SELF_OBJECT) ||
           (s == IA_SELF_CHARACTER) || (s == IA_ACTIVE_CHARACTER) ||
           (s == IA_ACTUAL_MAP) || (s == IA_TRUE) || (s == IA_FALSE) ||
           (s == IA_MISSION_ADD) || (s == IA_MISSION_COMPLETE) ||
           (s == IA_MISSION_IS_ACTIVE) || (s == IA_MISSION_SET_TEMP) ||
           (s == IA_MISSION_GET_TEMP) || (s == IA_MISSION_SET_XP) ||
           (s == IA_INVENTORY_HAVE) );
}

/***********************************************************************
 *                           functionType                              *
 ***********************************************************************/
string functionType(string s)
{
   if( (s == IA_MOVE_TO_POSITION) || (s == IA_MOVE_TO_CHARACTER) ||
       (s == IA_MOVE_TO_OBJECT) || (s == IA_FEAT_USE_AT_CHARACTER) ||
       (s == IA_FEAT_USE_AT_OBJECT) || (s == IA_TRUE) || (s == IA_FALSE) ||
       (s == IA_MISSION_IS_ACTIVE) || (s == IA_INVENTORY_HAVE) )
   {
      return(IA_TYPE_BOOL);
   }
   else if( (s == IA_FEAT_TOTAL) || (s == IA_FEAT_COST) || 
            (s == IA_SKILL_POINTS) || (s == IA_SKILL_MODIFIER) ||
            (s == IA_CHARACTER_GET_PSYCHO) || (s == IA_MISSION_GET_TEMP) )
   {
      return(IA_TYPE_INT);
   }
   else if(s == IA_SELF_OBJECT)
   {
      return(IA_TYPE_OBJECT);
   }
   else if((s == IA_SELF_CHARACTER) || (s == IA_ACTIVE_CHARACTER) )
   {
      return(IA_TYPE_CHARACTER);
   }
   else if(s == IA_ACTUAL_MAP)
   {
      return(IA_TYPE_MAP);
   }
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
          (s[pos] == IA_SEPARATOR_NULL) || (s[pos] == IA_SEPARATOR_COMMA[0]) ))
   {
      pos++;
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
                (s[pos] != '\t') && (!isOperator(cmp)) )
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




