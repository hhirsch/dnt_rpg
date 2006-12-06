#include "fuzzy.h"
#include <stdlib.h>

/***************************************************************
 *                           Constructor                       *
 ***************************************************************/
fuzzy::fuzzy()
{
   /* Define Rules */
   rules = NULL;
   totalRules = 0;

   /* Define Functions */
   functions = NULL;
   totalFunctions = 0;

   /* Define Variables */
   variables = NULL;
   totalVariables = 0;
}

/***************************************************************
 *                            Destructor                       *
 ***************************************************************/
fuzzy::~fuzzy()
{
   int i;

   /* Delete All Rules */
   fuzzyRule* fr = rules;
   fuzzyRule* tr;
   for(i=0; i < totalRules; i++)
   {
      tr = fr;
      fr = fr->next;
      delete(tr);
   }

   /* Delete All Variables */
   fuzzyVariable* fv = variables;
   fuzzyVariable* tv;
   for(i=0; i < totalVariables; i++)
   {
      tv = fv;
      fv = fv->next;
      delete(tv);
   }

   /* Delete All Functions */
   fuzzyFunction* ff = functions;
   fuzzyFunction* tf;
   for(i=0; i < totalFunctions; i++)
   {
      tf = ff;
      ff = ff->next;
      delete(tf);
   }
   
}

/***************************************************************
 *                           addVariable                       *
 ***************************************************************/
fuzzyVariable* fuzzy::addVariable()
{
   fuzzyVariable* fv = new(fuzzyVariable);
   if(variables)
   {
      fv->next = variables->next;
      fv->previous = variables;
      fv->previous->next = fv;
      fv->next->previous = fv;
   }
   else
   {
      fv->next = fv;
      fv->previous = fv;
   }
   totalVariables++;
   return(fv);
}

/***************************************************************
 *                             addRule                         *
 ***************************************************************/
fuzzyRule* fuzzy::addRule(int numberOfOperators)
{
   fuzzyRule* fr = new fuzzyRule(numberOfOperators);
   if(rules)
   {
      fr->next = rules->next;
      fr->previous = rules;
      fr->previous->next = fr;
      fr->next->previous = fr;
   }
   else
   {
      fr->next = fr;
      fr->previous = fr;
   }
   totalRules++;
   return(fr);
}

/***************************************************************
 *                           addFunction                       *
 ***************************************************************/
fuzzyFunction* fuzzy::addFunction(int functionType, 
                                  int xa, int xb, int xc, int xd)
{
   fuzzyFunction* ff = new fuzzyFunction(functionType, xa, xb, xc, xd);
   if(functions)
   {
      ff->next = functions->next;
      ff->previous = functions;
      ff->previous->next = ff;
      ff->next->previous = ff;
   }
   else
   {
      ff->next = ff;
      ff->previous = ff;
   }
   totalFunctions++;
   return(ff);

}

/***************************************************************
 *                             evalute                         *
 ***************************************************************/
void fuzzy::evalute()
{
   int i;

   /* Clear All Variables */
   fuzzyVariable* fv = variables;
   for(i=0; i < totalVariables; i++)
   {
      fv->clearValue();
      fv = fv->next;
   }

   /* Evalute all Rules */
   fuzzyRule* fr = rules;
   for(i=0; i < totalRules; i++)
   {
      fr->evalute();
      fr = fr->next;
   }

}

/***************************************************************
 *                         deffuzyfication                     *
 ***************************************************************/
float fuzzy::defuzzyfication()
{
   int i;
   float value = 0;
   
   /* Using the Largest of Max Method to Calculate Value */

   fuzzyVariable* fv = variables;
   for(i=0; i < totalVariables; i++)
   {
      if(fv->getValue() > value)
      {
         value = fv->getValue();
      }
      fv = fv->next;
   }

   return(value);
}

