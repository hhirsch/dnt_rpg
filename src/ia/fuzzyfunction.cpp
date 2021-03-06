/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "fuzzyfunction.h"
#include <stdlib.h>
#include <stdio.h>


/***************************************************************
 *                           Constructor                       *
 ***************************************************************/
fuzzyFunctionList::fuzzyFunctionList()
{
}

/***************************************************************
 *                            Destructor                       *
 ***************************************************************/
fuzzyFunctionList::~fuzzyFunctionList()
{
   clearList();
}

/***************************************************************
 *                           freeElement                       *
 ***************************************************************/
void fuzzyFunctionList::freeElement(dntListElement* obj)
{
   fuzzyFunction* f = (fuzzyFunction*)obj;
   delete(f);
}

/***************************************************************
 *                           Constructor                       *
 ***************************************************************/
fuzzyFunction::fuzzyFunction(int func, float xa, float xb, float xc, float xd)
{
   function = func;
   x1 = xa;
   x2 = xb;
   x3 = xc;
   x4 = xd;
   crispValue = -1;
   upperValue = 1;
}

/***************************************************************
 *                           Destructor                        *
 ***************************************************************/
fuzzyFunction::~fuzzyFunction()
{
   function = -1;
}

/***************************************************************
 *                          setCrispValue                      *
 ***************************************************************/
void fuzzyFunction::setCrispValue(float crisp)
{
   crispValue = crisp;
}

/***************************************************************
 *                             setLimit                        *
 ***************************************************************/
void fuzzyFunction::setLimit(float value)
{
   upperValue = value;
}


/***************************************************************
 *                             evalute                         *
 ***************************************************************/
float fuzzyFunction::evalute()
{
   float ret = 0;
   switch(function)
   {
      case FUZZY_FUNCTION_STEP:
      {
         if(crispValue <= x1)
         {
            ret = 0;
         }
         else if( (crispValue > x1) && (crispValue < x2))
         {
            float a = (upperValue / (x2 - x1));
            float b = -(a*x1);
            ret = (a * crispValue) + b;
         }
         else if(crispValue >= x2)
         {
            ret = upperValue;
         }
      }
      break;
      case FUZZY_FUNCTION_INVERTED_STEP:
      {
         if(crispValue <= x1)
         {
            ret = upperValue;
         }
         else if( (crispValue > x1) && (crispValue < x2))
         {
            float a = (-upperValue / (x2 - x1));
            float b = -a*x2;
            ret = (a * crispValue) + b;
         }
         else if(crispValue >= x2)
         {
            ret = 0;
         }
      }
      break;
      case FUZZY_FUNCTION_TRIANGULAR:
      {
         if(crispValue <= x1)
         {
            ret = 0;
         }
         else if( (crispValue > x1) && (crispValue < x2))
         {
            float a = (upperValue / (x2 - x1));
            float b = -a*x1;
            ret = (a * crispValue) + b;
         }
         else if(crispValue == x2)
         {
            ret = upperValue;
         }
         else if( (crispValue >= x2) && (crispValue < x3))
         {
            float a = (-upperValue / (x3 - x2));
            float b = -a*x3;
            ret = (a * crispValue) + b;
         }
         else if(crispValue >= x3)
         {
            ret = 0;
         }
      }
      break;
      case FUZZY_FUNCTION_TRAPEZOIDAL:
      {
         if(crispValue <= x1)
         {
            ret = 0;
         }
         else if( (crispValue > x1) && (crispValue < x2))
         {
            float a = (upperValue / (x2 - x1));
            float b = -a*x1;
            ret = (a * crispValue) + b;
         }
         else if( (crispValue >= x2) && (crispValue <= x3))
         {
            ret = upperValue;
         }
         else if( (crispValue > x3) && (crispValue < x4))
         {
            float a = (-upperValue / (x4 - x3));
            float b = -a*x4;
            ret = (a * crispValue) + b;
         }
         else if(crispValue >= x4)
         {
            ret = 0;
         }
      }
      break;
   }
   
   return(ret);
}

