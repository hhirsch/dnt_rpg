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

#include "dices.h"
#include <SDL/SDL.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                                dice                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
dice::dice()
{
   init(DICE_D20);
}
/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
dice::dice(int type)
{
   init(type);
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
dice::~dice()
{
}

/***********************************************************************
 *                                  init                               *
 ***********************************************************************/
void dice::init(int type)
{
   diceID = type;
   numberOfDices = 1;
   sumNumber = 0;
   criticalMultiplier = 1;
}

/***********************************************************************
 *                               setType                               *
 ***********************************************************************/
void dice::setType(int type)
{
   diceID = type;
}

/***********************************************************************
 *                               getType                               *
 ***********************************************************************/
int dice::getType()
{
   return(diceID);
}

/***********************************************************************
 *                           setNumberOfDices                          *
 ***********************************************************************/
void dice::setNumberOfDices(int num)
{
   numberOfDices = num;
}

/***********************************************************************
 *                           getNumberOfDices                          *
 ***********************************************************************/
int dice::getNumberOfDices()
{
   return(numberOfDices);
}

/***********************************************************************
 *                             setSumNumber                            *
 ***********************************************************************/
void dice::setSumNumber(int num)
{
   sumNumber = num;
}

/***********************************************************************
 *                             getSumNumber                            *
 ***********************************************************************/
int dice::getSumNumber()
{
   return(sumNumber);
}

/***********************************************************************
 *                        setCriticalMultiplier                         *
 ***********************************************************************/
void dice::setCriticalMultiplier(int num)
{
   criticalMultiplier = num;
}

/***********************************************************************
 *                        getCriticalMultiplier                        *
 ***********************************************************************/
int dice::getCriticalMultiplier()
{
   return(criticalMultiplier);
}

/***********************************************************************
 *                                  roll                               *
 ***********************************************************************/
int dice::roll(bool critical)
{
   int d; /* dice counter */
   int value = 0; /* the value got */

   /* Do some randomize (current ticks + random number [0,1000) */
   srand(SDL_GetTicks() + (int)(1 + 1000 * (rand() / (RAND_MAX + 1.0))));

   /* Throw the dices */
   for(d = 0; d < numberOfDices; d++)
   {
      value += 1 + (int)(diceID * (rand() / (RAND_MAX + 1.0)));
   }

   /* Apply Sum Factor */
   value += sumNumber;

   /* If critical, reroll as many times defined the dices */
   if(critical)
   {
      for(d = 0; d < criticalMultiplier; d++)
      {
         value += roll(false);
      }
   }

   /* Finally, the value */
   return(value);
}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                            diceThing                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
diceThing::diceThing()
{
   /* Default is no aditional dices */
   aditionalDice.setNumberOfDices(0);
}


