#include "feats.h"

int numberConstant(string buffer)
{
   int ret = -1;

   //FIXME NO More!
   return(ret);
}


bool isSkill(int number)
{
   return( (number >= ATT_SKILL_FIRST) && (number <= ATT_SKILL_LAST));
}

bool isClass(int number)
{
   //FIXME
   return(false);
   //return( (number >= CLASS_EADM) && (number <= CLASS_ETO) );
}

bool isTendency(int number)
{
   //FIXME
   //return( (number >= TEND_SL_LIBERTARIO) && (number <= TEND_SP_CAOTICO) );
   return(false);
}

bool isRace(int number)
{
   //FIXME
   return(false);
   //return( (number >= RACE_AUTISTA_ESTRANHO) && (number <= RACE_AMEIVA) );
}

bool isAttribute(int number)
{
   //FIXME
   return(false);
   //return( (number >= ATT_LEVEL) && (number <= ATT_CHARISM) );
}

bool isVariable(int number)
{
   //FIXME
   return(false);
   //return( (number >= VAR_AUX1) && (number <= VAR_AUX6) );
}
