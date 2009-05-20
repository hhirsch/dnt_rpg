#include "defs.h"

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
   reflex = 0;
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
   res.reflex = reflex + b.reflex;
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
   reflex = b.reflex;
   fortitude = b.fortitude;
   baseAttackBonus = b.baseAttackBonus;
   return(*this);
}

