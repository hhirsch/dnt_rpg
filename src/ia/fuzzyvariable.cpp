#include "fuzzyvariable.h"
#include <stdlib.h>

/***************************************************************
 *                           Constructor                       *
 ***************************************************************/
fuzzyVariable::fuzzyVariable(int func, float xa, float xb, float xc, float xd)
{
   function = new fuzzyFunction(func, xa, xb, xc, xd);
   clearValue();
   next = NULL;
   previous = NULL;
}

/***************************************************************
 *                           Destructor                        *
 ***************************************************************/
fuzzyVariable::~fuzzyVariable()
{
   delete(function);
   clearValue();
}

/***************************************************************
 *                            clearValue                       *
 ***************************************************************/
void fuzzyVariable::clearValue()
{
   value = -1;
   function->setLimit(1.0);
}

/***************************************************************
 *                             getValue                        *
 ***************************************************************/
float fuzzyVariable::getValue()
{
   return(value);
}

/***************************************************************
 *                             setValue                        *
 ***************************************************************/
void fuzzyVariable::setValue(float val)
{
   if(value == -1)
   {
      /* Not Setted, so start */
      value = val;
   }
   else
   {
      /* Already started, so calculate */
      value = ((value + val) / 2.0);
      /* Or another Formula */
      //value = (value+val) - (value*val);
   }
   function->setLimit(1.0);
}

/***************************************************************
 *                          evaluteFunction                    *
 ***************************************************************/
float fuzzyVariable::evaluteFunction(float x)
{
   function->setCrispValue(x);
   return(function->evalute());
}


