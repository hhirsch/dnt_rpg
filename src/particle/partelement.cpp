/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#include "partelement.h"
#include <stdlib.h>

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
dntPartElement::dntPartElement()
{
   initialValue = 0.0f;
   finalValue = 0.0f;

   lowerLimit = 0.0f;
   upperLimit = 0.0f;
   limitsDefined = false;

   mult = 0.0f;
   sum = 0.0f;

   type = DNT_PART_ELEMENT_LINEAR;

   isStatic = true;
}

/***********************************************************************
 *                             setLinear                               *
 ***********************************************************************/
void dntPartElement::setLinear(float i, float f, int maxSteps)
{
   initialValue = i;
   finalValue = f;
   sum = ((finalValue - initialValue) / ((float)maxSteps));
   type = DNT_PART_ELEMENT_LINEAR;
}

/***********************************************************************
 *                             setRandom                               *
 ***********************************************************************/
void dntPartElement::setRandom(float i, float m, float s)
{
   initialValue = i;
   mult = m;
   sum = s;
   type = DNT_PART_ELEMENT_RANDOM;
}

/***********************************************************************
 *                             setLimits                               *
 ***********************************************************************/
void dntPartElement::setLimits(float lower, float upper)
{
   lowerLimit = lower;
   upperLimit = upper;
   limitsDefined = true;
}

/***********************************************************************
 *                           updateValue                               *
 ***********************************************************************/
bool dntPartElement::updateValue(float& curValue)
{
   float lastValue = curValue;
   isStatic = false;
   switch(type)
   {
      case DNT_PART_ELEMENT_LINEAR:
      {
         /* Only sum the linear factor */
         curValue += sum;
         if( ( (sum > 0) && (curValue > finalValue) ) ||
             ( (sum < 0) && (curValue < finalValue) ) )
         {
            curValue = finalValue;
         }
      }
      break;
      case DNT_PART_ELEMENT_RANDOM:
      {
         /* Do the random */
         curValue = mult*((rand() / ((double)RAND_MAX+1))) + sum; 
      }
      break;
      default:
      {
         isStatic = true;
         return(false);
      }
      break;
   }

   /* Verify limits, if defined */
   if(limitsDefined)
   {
      if(curValue < lowerLimit)
      {
         curValue = lowerLimit;
      }
      else if(curValue > upperLimit)
      {
         curValue = upperLimit;
      }
   }

   /*! Verify if static  */
   isStatic = (lastValue == curValue);

   return(true);
}

/***********************************************************************
 *                         getIsStatic                                 *
 ***********************************************************************/
bool dntPartElement::getIsStatic()
{
   return(isStatic);
}



