#include "fuzzyrule.h"
#include <stdlib.h>

/***************************************************************
 *                           Constructor                       *
 ***************************************************************/
fuzzyRule::fuzzyRule(int total)
{
   operators = (int*) malloc(total*sizeof(int));
   functions = (fuzzyFunction**) malloc(total*sizeof(fuzzyFunction*));
   returnVariable = NULL;
   totalMembers = total;
   numberOfMembers = 0;
   next = NULL;
   previous = NULL;
}

/***************************************************************
 *                            Destructor                       *
 ***************************************************************/
fuzzyRule::~fuzzyRule()
{
   free(operators);
   free(functions);
}

/***************************************************************
 *                             addMember                       *
 ***************************************************************/
bool fuzzyRule::addMember(int op, fuzzyFunction* function)
{
   if(numberOfMembers < totalMembers)
   {
      functions[numberOfMembers] = function;
      operators[numberOfMembers] = op;
      numberOfMembers++;
      return(true);
   }
   return(false);
}

/***************************************************************
 *                            addVariable                      *
 ***************************************************************/
void fuzzyRule::addVariable(fuzzyVariable* returnVariable)
{
   returnVariable = returnVariable;
}

/***************************************************************
 *                              andValue                       *
 ***************************************************************/
float fuzzyRule::andValue(float op1, float op2)
{
   if(op1 > op2)
   {
      return(op2);
   }
   else
   {
      return(op1);
   }
}

/***************************************************************
 *                              orValue                        *
 ***************************************************************/
float fuzzyRule::orValue(float op1, float op2)
{
   if(op1 > op2)
   {
      return(op1);
   }
   else
   {
      return(op2);
   }
}


/***************************************************************
 *                            addVariable                      *
 ***************************************************************/
void fuzzyRule::evalute()
{
   if(numberOfMembers < 1)
   {
      return;
   }
   
   int i;
   float result = functions[0]->evalute();
   
   for(i=1; i < numberOfMembers; i++)
   {
      switch(operators[i])
      {
         case FUZZY_OPERATOR_AND:
         {
            result = andValue(result,functions[i]->evalute());
         }
         break;
         case FUZZY_OPERATOR_AND_NOT:
         {
            result = andValue(result,!(functions[i]->evalute()));
         }
         break;
         case FUZZY_OPERATOR_OR:
         {
            result = orValue(result,functions[i]->evalute());
         }
         break;
         case FUZZY_OPERATOR_OR_NOT:
         {
            result = orValue(result, !(functions[i]->evalute()));
         }
         break;
      }
   }

   if(returnVariable)
   {
      returnVariable->setValue(result);
   }
}

