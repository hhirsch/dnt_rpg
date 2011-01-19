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
#include <iostream>
using namespace std;

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

   type = DNT_PART_ELEMENT_CONSTANT;

   used = false;
   isStatic = true;
}

/***********************************************************************
 *                            setConstant                              *
 ***********************************************************************/
void dntPartElement::setConstant(float i)
{
   initialValue = i;
   used = true;
   type = DNT_PART_ELEMENT_CONSTANT;
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
   used = true;
}

/***********************************************************************
 *                         setRandomInitial                            *
 ***********************************************************************/
void dntPartElement::setRandomInitial(float i, float m, float s)
{
   initialValue = i;
   mult = m;
   sum = s;
   type = DNT_PART_ELEMENT_RANDOM_INITIAL;
   used = true;
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
   used = true;
}

/***********************************************************************
 *                             setLimits                               *
 ***********************************************************************/
void dntPartElement::setLimits(float lower, float upper)
{
   lowerLimit = lower;
   upperLimit = upper;
   limitsDefined = true;
   used = true;
}

/***********************************************************************
 *                           updateValue                               *
 ***********************************************************************/
bool dntPartElement::updateValue(float& curValue)
{
   float lastValue = curValue;
   isStatic = false;

   if( (!used) || 
       (type == DNT_PART_ELEMENT_CONSTANT) ||
       (type == DNT_PART_ELEMENT_RANDOM_INITIAL) )
   {
      /* No need to update */
      isStatic = true;
      return(false);
   }

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
 *                       getInitialValue                               *
 ***********************************************************************/
float dntPartElement::getInitialValue()
{
   switch(type)
   {
       /* Return the defined initial */
       case DNT_PART_ELEMENT_CONSTANT:
       case DNT_PART_ELEMENT_LINEAR:
       {
          return(initialValue);
       }
       break;
       /* Return the random generated */
       case DNT_PART_ELEMENT_RANDOM_INITIAL:
       case DNT_PART_ELEMENT_RANDOM:
       {
          return(mult*((rand() / ((double)RAND_MAX+1))) + sum + initialValue);
       }
       break;
   }
   return(0.0f);
}

/***********************************************************************
 *                         getIsStatic                                 *
 ***********************************************************************/
bool dntPartElement::getIsStatic()
{
   return(isStatic);
}

/***********************************************************************
 *                            isUsed                                   *
 ***********************************************************************/
bool dntPartElement::isUsed()
{
   return(used);
}

/***********************************************************************
 *                         fromString                                  *
 ***********************************************************************/
bool dntPartElement::fromString(string s)
{
   char buf[512];
   string typeString="";
   float aux=0.0f;
   int flag=0;

   if(sscanf(s.c_str(), "%s %f %f %f %i %f %f",
                      &buf[0], &initialValue, &aux, &sum, 
                      &flag, &lowerLimit, &upperLimit) == 7)
   {
      typeString = buf;
      if(typeString == "constant")
      {
         type = DNT_PART_ELEMENT_CONSTANT;
      }
      else if(typeString == "linear")
      {
         finalValue = aux;
         type = DNT_PART_ELEMENT_LINEAR;
      }
      else if(typeString == "random")
      {
         mult = aux;
         type = DNT_PART_ELEMENT_RANDOM;
      }
      else if(typeString == "randomInitial")
      {
         mult = aux;
         type = DNT_PART_ELEMENT_RANDOM_INITIAL;
      }
      else
      {
         cerr << "Error: Unknow dntPartElement type: '" 
              << typeString << "'" << endl;
         return(false);
      }
      limitsDefined = (flag != 0);
      return(true);
   }
   
   cerr << "Error: unknow part element string: '" << s << "'" << endl;
   return(false);

}

/***********************************************************************
 *                           toString                                  *
 ***********************************************************************/
string dntPartElement::toString()
{
   char buf[256];
   string res="";

   if(!used)
   {
      return("");
   }

   switch(type)
   {
      case DNT_PART_ELEMENT_CONSTANT:
      case DNT_PART_ELEMENT_LINEAR:
      {
         /* linear initialValue finalValue sum useLimits 
          *        lowerLimit upperLimit */
         if(type == DNT_PART_ELEMENT_LINEAR)
         {
            res = "linear ";
         }
         else
         {
            res = "constant ";
         }
         sprintf(buf, "%.3f %.3f %.3f %i %.3f %.3f", 
                       initialValue, finalValue, sum,
                       limitsDefined, lowerLimit, upperLimit);
         res += buf;
      }
      break;
      case DNT_PART_ELEMENT_RANDOM:
      case DNT_PART_ELEMENT_RANDOM_INITIAL:
      {
         /* random initialValue mult sum useLimits lowerLimit upperLimit */
         if(type == DNT_PART_ELEMENT_RANDOM)
         {
            res = "random ";
         }
         else
         {
            res = "randomInitial ";
         }
         sprintf(buf, "%.3f %.3f %.3f %i %.3f %.3f", 
                      initialValue, mult, sum, 
                      limitsDefined, lowerLimit, upperLimit);
         res += buf;
      }
      break;
   }

   return(res);
}

