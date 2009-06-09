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

#include "fuzzyrule.h"
#include <stdlib.h>
#include <stdio.h>

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
void fuzzyRule::addVariable(fuzzyVariable* variable)
{
   returnVariable = variable;
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
            result = andValue(result,1-(functions[i]->evalute()));
         }
         break;
         case FUZZY_OPERATOR_OR:
         {
            result = orValue(result,functions[i]->evalute());
         }
         break;
         case FUZZY_OPERATOR_OR_NOT:
         {
            result = orValue(result, 1-(functions[i]->evalute()));
         }
         break;
      }
   }

   if(returnVariable)
   {
      returnVariable->setValue(result);
   }

   //printf("result of Rule: %.3f\n",result);
}

