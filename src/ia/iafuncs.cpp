#include "iafuncs.h"


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
           (c == IA_OPERATOR_RPARENTHESIS) );
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

   /* Ignore all previous spaces */
   while( (pos < s.length()) && ( (s[pos] == ' ') || (s[pos] == '\t') ||
          (s[pos] == '\n') || (s[pos] == '\0')) )
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
      while( (pos < s.length()) && (s[pos] != ';') && (s[pos] != ',') &&
             (s[pos] != '\n') && (s[pos] != '\0') && (s[pos] != ' ') &&
             (s[pos] != '\t') && (!isOperator(cmp)) )
       {
         ret += s[pos];
         pos++;
         cmp = s[pos];
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




