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
   psychoState = PSYCHO_NEUTRAL;
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
int thing::skillBonus(skill* curSkill)
{
   int att;
   att = attBonus(curSkill->habilidadeBase);
   return(att + curSkill->pontos);
}

/******************************************************
 *                     attBonus                       *
 ******************************************************/
int thing::attBonus(skill* curAttribute)
{
   return((int)floor((curAttribute->pontos-10) / 2.0));
}

int thing::attBonus(int curAttribute)
{
   return(attBonus(&sk.m_skills[curAttribute]));
}


/******************************************************
 *                   getBonusValue                    *
 ******************************************************/
int thing::getBonusValue(factor something)
{
   if(something.type == "ATTRIBUTE")
   {
      return(attBonus(sk.getSkillByString(something.id)));
   }
   else if(something.type == "SKILL")
   {
      return(skillBonus(sk.getSkillByString(something.id)));
   }
   
   return(0);
}

