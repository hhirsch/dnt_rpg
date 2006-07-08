#include "thing.h"
#include <math.h>

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

/******************************************************
 *                   Constructor                      *
 ******************************************************/
thing::thing()
{
   lifePoints = 0;
   fortitude = 0;
   reflexes = 0;
   will = 0;
   armatureClass = 0;
   displacement = 0;
   level = 0;
}

/******************************************************
 *                    Destructor                      *
 ******************************************************/
thing::~thing()
{
}

/******************************************************
 *                   skillBonus                       *
 ******************************************************/
int thing::skillBonus(int skillNumber)
{
   int att;
   if( (skillNumber >= ATT_SKILL_FIRST) && (ATT_SKILL_LAST))
   {
      att = attBonus(sk.m_skills[skillNumber].habilidadeBase);
      return(att + sk.m_skills[skillNumber].pontos);
   }
   else
   {
      return(0);
   }
}

/******************************************************
 *                     attBonus                       *
 ******************************************************/
int thing::attBonus(int attNumber)
{
   if( (attNumber >= ATT_STRENGHT) || (attNumber <= ATT_CHARISM))
   { 
       return((int)floor((sk.m_skills[attNumber].pontos-10) / 2.0));
   } 
   else
   {
      return(0);
   }
}


int thing::getBonusValue(int something)
{
   if(isSkill(something))
   {
     return(skillBonus(something));
   }
   else
   if(isAttribute(something))
   {
     return(attBonus(something));
   }
   return(0);
}

