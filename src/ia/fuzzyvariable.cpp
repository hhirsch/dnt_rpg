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

#include "fuzzyvariable.h"
#include <stdlib.h>
#include <stdio.h>

/***************************************************************
 *                           Constructor                       *
 ***************************************************************/
fuzzyVariableList::fuzzyVariableList()
{
}

/***************************************************************
 *                            Destructor                       *
 ***************************************************************/
fuzzyVariableList::~fuzzyVariableList()
{
   clearList();
}

/***************************************************************
 *                           freeElement                       *
 ***************************************************************/
void fuzzyVariableList::freeElement(dntListElement* obj)
{
   fuzzyVariable* var = (fuzzyVariable*)obj;
   delete(var);
}

/***************************************************************
 *                           Constructor                       *
 ***************************************************************/
fuzzyVariable::fuzzyVariable(int func, float xa, float xb, float xc, float xd)
{
   function = new fuzzyFunction(func, xa, xb, xc, xd);
   clearValue();
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
      /* value = ((value + val) / 2.0); */
      /* Or another Formula */
      value = (value+val) - (value*val);
   }
   function->setLimit(value);
}

/***************************************************************
 *                          evaluteFunction                    *
 ***************************************************************/
float fuzzyVariable::evaluteFunction(float x)
{
   function->setCrispValue(x);
   return(function->evalute());
}


