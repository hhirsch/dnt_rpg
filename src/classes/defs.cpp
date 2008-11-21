#include "defs.h"


/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
attackBonus::attackBonus()
{
   int i;
   for(i=0; i <MAX_ATTACKS; i++)
   {
      bonus[i] = 0;
   }
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
attackBonus attackBonus::operator+(attackBonus& atBonus)
{
   return(attackBonus(toInt()+atBonus.toInt()));
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
