#include "fuzzyvariable.h"
#include <stdlib.h>

/***************************************************************
 *                           Constructor                       *
 ***************************************************************/
fuzzyVariable::fuzzyVariable()
{
   clearValue();
   next = NULL;
   previous = NULL;
}

/***************************************************************
 *                           Destructor                        *
 ***************************************************************/
fuzzyVariable::~fuzzyVariable()
{
   clearValue();
}

/***************************************************************
 *                            clearValue                       *
 ***************************************************************/
void fuzzyVariable::clearValue()
{
   value = -1;
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
   }
}

