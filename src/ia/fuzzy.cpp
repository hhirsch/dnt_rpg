/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
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

#include "fuzzy.h"
#include <stdlib.h>

/***************************************************************
 *                           Constructor                       *
 ***************************************************************/
fuzzy::fuzzy()
{
}

/***************************************************************
 *                            Destructor                       *
 ***************************************************************/
fuzzy::~fuzzy()
{
   /* Delete All Rules */
   ruleList.clearList();

   /* Delete All Variables */
   varList.clearList();

   /* Delete All Functions */
   funcList.clearList();
}

/***************************************************************
 *                           addVariable                       *
 ***************************************************************/
fuzzyVariable* fuzzy::addVariable(int functionType, 
                                  float xa, float xb, float xc, float xd)
{
   fuzzyVariable* fv = new fuzzyVariable(functionType, xa, xb, xc, xd);
   varList.insert(fv);

   return(fv);
}

/***************************************************************
 *                             addRule                         *
 ***************************************************************/
fuzzyRule* fuzzy::addRule(int numberOfOperators)
{
   fuzzyRule* fr = new fuzzyRule(numberOfOperators);
   ruleList.insert(fr);

   return(fr);
}

/***************************************************************
 *                           addFunction                       *
 ***************************************************************/
fuzzyFunction* fuzzy::addFunction(int functionType, 
                                  float xa, float xb, float xc, float xd)
{
   fuzzyFunction* ff = new fuzzyFunction(functionType, xa, xb, xc, xd);
   funcList.insert(ff);
   
   return(ff);

}

/***************************************************************
 *                             evalute                         *
 ***************************************************************/
void fuzzy::evalute()
{
   int i;

   /* Clear All Variables */
   fuzzyVariable* fv = (fuzzyVariable*)varList.getFirst();
   for(i=0; i < varList.getTotal(); i++)
   {
      fv->clearValue();
      fv = (fuzzyVariable*)fv->getNext();
   }

   /* Evalute all Rules */
   fuzzyRule* fr = (fuzzyRule*)ruleList.getFirst();
   for(i=0; i < ruleList.getTotal(); i++)
   {
      fr->evalute();
      fr = (fuzzyRule*)fr->getNext();
   }

}

/***************************************************************
 *                         deffuzyfication                     *
 ***************************************************************/
float fuzzy::defuzzyfication()
{
   int i;
   float k;
   float funcValue;
   float a = 0;
   float b = 0;
   
   /* Using the Centroid Method to Calculate Value */
   fuzzyVariable* fv = (fuzzyVariable*)varList.getFirst();
   for(i=0; i < varList.getTotal(); i++)
   {
      for(k=0; k<=1; k+= 0.1)
      {
         funcValue = fv->evaluteFunction(k);
         a += k*funcValue;
         b += funcValue;
      }
      fv = (fuzzyVariable*)fv->getNext();
   }

   return((a / b));
}

