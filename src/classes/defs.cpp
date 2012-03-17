/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

#include "defs.h"
#include "../engine/briefing.h"
#include "../lang/translate.h"
using namespace std;

#include <stdio.h>

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                             attackBonus                               //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
attackBonus::attackBonus()
{
   clear();
}

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
attackBonus::attackBonus(int total)
{
   int i;
   int curVal = total;

   /* Distribute the total between each base bonus. 
    * If the current reachs 6, a new possible attack is achieved,
    * up to MAX_ATTACKS. */
   for(i = 0; (i < MAX_ATTACKS); i++)
   {
      if(curVal > 0)
      {
         bonus[i] = curVal;
      }
      else
      {
         bonus[i] = 0;
      }

      /* For the next base attack the bonus will be 5 lesser */
      curVal -= 5;
   }

}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
attackBonus::~attackBonus()
{
}

/***********************************************************************
 *                                clear                                *
 ***********************************************************************/
void attackBonus::clear()
{
   int i;
   for(i=0; i <MAX_ATTACKS; i++)
   {
      bonus[i] = 0;
   }
}

/***********************************************************************
 *                                toString                             *
 ***********************************************************************/
string attackBonus::toString()
{
   string res = "";
   char val[16];
   int i;

   for(i=0; i < MAX_ATTACKS; i++)
   {
      /* Only Convert non 0 bonus */
      if(bonus[i] != 0)
      {
         /* Put a separator, if needed */
         if(i > 0)
         {
            res += "/";
         }
         sprintf(val,"%d", bonus[i]);
         res += "+";
         res += val;
      }
   }

   /* If empty, no bonus, so 0 is defined */
   if(res.empty())
   {
      res = "0";
   }

   return(res);
}

/***********************************************************************
 *                                toInt                                *
 ***********************************************************************/
int attackBonus::toInt()
{
   return(bonus[0]);
}

/***********************************************************************
 *                               getBonus                              *
 ***********************************************************************/
int attackBonus::getBonus(int attackNumber)
{
   if( (attackNumber > 0) && (attackNumber <= MAX_ATTACKS))
   {
      return(bonus[attackNumber-1]);
   }

   return(0);
}

/***********************************************************************
 *                               setBonus                              *
 ***********************************************************************/
void attackBonus::setBonus(int attackNumber, int value)
{
   if( (attackNumber > 0) && (attackNumber <= MAX_ATTACKS))
   {
      bonus[attackNumber-1] = value;
   }
}

/***********************************************************************
 *                               operator+                             *
 ***********************************************************************/
attackBonus attackBonus::operator+(const attackBonus& atBonus)
{
   return(attackBonus(toInt()+atBonus.bonus[0]));
}

/***********************************************************************
 *                               operator=                             *
 ***********************************************************************/
attackBonus& attackBonus::operator=(const attackBonus& atBonus)
{
   int i;
   for(i=0; i < MAX_ATTACKS; i++)
   {
      bonus[i] = atBonus.bonus[i];
   }

   return(*this);
}

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                            bonusAndSaves                              //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
bonusAndSaves::bonusAndSaves()
{
   clear();
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
bonusAndSaves::~bonusAndSaves()
{
}

/***********************************************************************
 *                                 clear                               *
 ***********************************************************************/
void bonusAndSaves::clear()
{
   level = 0;
   iAmNotAFool = 0;
   reflexes = 0;
   fortitude = 0;
   baseAttackBonus.clear();
}

/***********************************************************************
 *                               operator+                             *
 ***********************************************************************/
bonusAndSaves bonusAndSaves::operator+(const bonusAndSaves& b)
{
   bonusAndSaves res;
   res.level = level + b.level;
   res.iAmNotAFool = iAmNotAFool + b.iAmNotAFool;
   res.reflexes = reflexes + b.reflexes;
   res.fortitude = fortitude + b.fortitude;
   res.baseAttackBonus = baseAttackBonus + b.baseAttackBonus;

   return(res);
}

/***********************************************************************
 *                               operator=                             *
 ***********************************************************************/
bonusAndSaves& bonusAndSaves::operator=(const bonusAndSaves& b)
{
   level = b.level;
   iAmNotAFool = b.iAmNotAFool;
   reflexes = b.reflexes;
   fortitude = b.fortitude;
   baseAttackBonus = b.baseAttackBonus;
   return(*this);
}

/***********************************************************************
 *                               doCheck                               *
 ***********************************************************************/
bool bonusAndSaves::doCheck(string stateToCheck, int difficulty,
      bool* couldCheck)
{
   bool canCheck = false;
   bool checkRes = false;
   int value = 0;
   int checkType = DNT_CHECK_ROLL;
   string stateName = "";
   briefing brief;
   dice d20(DICE_D20);

   /* Let's get the state (if defined) */
   if(stateToCheck == DNT_BS_LEVEL)
   {
      checkType = DNT_CHECK_COMPARE;
      canCheck = true;
      value = level;
      stateName = gettext("Level");
   }
   else if(stateToCheck == DNT_BS_FORTITUDE)
   {
      value = fortitude;
      canCheck = true;
      stateName = gettext("Fortitude");
   }
   else if(stateToCheck == DNT_BS_REFLEXES)
   {
      value = reflexes;
      canCheck = true;
      stateName = gettext("Reflexes");
   }
   else if( (stateToCheck == DNT_BS_I_AM_NOT_A_FOOL) ||
            (stateToCheck == DNT_BS_WILL))
   {
      value = iAmNotAFool;
      canCheck = true;
      stateName = gettext("I am Not a Fool");
   }

   /* Do the check, if defined */
   if(canCheck)
   {
      if(checkType == DNT_CHECK_ROLL)
      {
         value += d20.roll();
      }
      brief.addCheckText(stateName, value, difficulty);
      checkRes = (value >= difficulty);
   }

   /* Set the flag */
   if(couldCheck != NULL)
   {
      *couldCheck = canCheck;
   }
   return(checkRes);
}

/***********************************************************************
 *                            setIAmNotAFool                           *
 ***********************************************************************/
void bonusAndSaves::setIAmNotAFool(int i)
{
   iAmNotAFool = i;
}

/***********************************************************************
 *                              setReflexes                            *
 ***********************************************************************/
void bonusAndSaves::setReflexes(int i)
{
   reflexes = i;
}

/***********************************************************************
*                              setFortitude                            *
 ***********************************************************************/
void bonusAndSaves::setFortitude(int i)
{
   fortitude = i;
}

/***********************************************************************
 *                             setBaseAttack                           *
 ***********************************************************************/
void bonusAndSaves::setBaseAttack(int i)
{
   int aux;

   for(aux = 1; aux <= MAX_ATTACKS; aux++)
   {
      baseAttackBonus.setBonus(aux, i);
   }
}


