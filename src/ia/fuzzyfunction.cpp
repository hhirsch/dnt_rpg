#include "fuzzyfunction.h"
#include <stdlib.h>

/***************************************************************
 *                           Constructor                       *
 ***************************************************************/
fuzzyFunction::fuzzyFunction(int func, int xa, int xb, int xc, int xd)
{
   function = func;
   x1 = xa;
   x2 = xb;
   x3 = xc;
   x4 = xd;
   next = NULL;
   previous = NULL;
}

/***************************************************************
 *                           Destructor                        *
 ***************************************************************/
fuzzyFunction::~fuzzyFunction()
{
   function = -1;
}

/***************************************************************
 *                           Constructor                       *
 ***************************************************************/
float fuzzyFunction::evalute(float crispValue)
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
            float a = (1 / (x2 - x1));
            float b = a*x1;
            ret = (a * crispValue) + b;
         }
         else if(crispValue >= x2)
         {
            ret = 1;
         }
      }
      break;
      case FUZZY_FUNCTION_INVERTED_STEP:
      {
         if(crispValue <= x1)
         {
            ret = 0;
         }
         else if( (crispValue > x1) && (crispValue < x2))
         {
            float a = (-1 / (x2 - x1));
            float b = a*x2;
            ret = (a * crispValue) + b;
         }
         else if(crispValue >= x2)
         {
            ret = 1;
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
            float a = (1 / (x2 - x1));
            float b = a*x1;
            ret = (a * crispValue) + b;
         }
         else if(crispValue == x2)
         {
            ret = 1;
         }
         else if( (crispValue >= x2) && (crispValue < x3))
         {
            float a = (-1 / (x3 - x2));
            float b = a*x3;
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
            float a = (1 / (x2 - x1));
            float b = a*x1;
            ret = (a * crispValue) + b;
         }
         else if( (crispValue >= x2) && (crispValue <= x3))
         {
            ret = 1;
         }
         else if( (crispValue > x3) && (crispValue < x4))
         {
            float a = (-1 / (x4 - x3));
            float b = a*x4;
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

