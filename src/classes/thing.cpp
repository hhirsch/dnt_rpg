#include "thing.h"
#include "defs.h"
#include "../engine/util.h"
#include <math.h>

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

/******************************************************
 *                   Constructor                      *
 ******************************************************/
thing::thing()
{
   age = 0;
   sexType = SEX_MALE;
   lifePoints = 0;
   maxLifePoints = 0;
   fortitude = 0;
   reflexes = 0;
   will = 0;
   armatureClass = 0;
   displacement = 0;
   baseAttackModifier = 0;
   sizeModifier = 0;
   psychoState = PSYCHO_NEUTRAL;
   xPosition = 0;
   yPosition = 0;
   zPosition = 0;
   xp = 0;
   cr = 1;
   xpPercent = 100;
   dead = false;
   walk_interval = 37.5 * WALK_ACTUALIZATION; //default value, if not
                                              //defined at the def file.
}

/******************************************************
 *                    Destructor                      *
 ******************************************************/
thing::~thing()
{
   dead = true;
}

/******************************************************
 *                   skillBonus                       *
 ******************************************************/
int thing::skillBonus(skill* curSkill)
{
   int att;
   att = attBonus(curSkill->baseAttribute);
   return(att + curSkill->points);
}

/******************************************************
 *                     attBonus                       *
 ******************************************************/
int thing::attBonus(skill* curAttribute)
{
   return((int)floor((curAttribute->points-10) / 2.0));
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

/******************************************************
 *                     setAsEnemy                     *
 ******************************************************/
void thing::setAsEnemy()
{
   psychoState = PSYCHO_HOSTILE;
}

/******************************************************
 *                        kill                        *
 ******************************************************/
void thing::kill()
{
   dead = true;
   callDeadAnimation();
}

/******************************************************
 *                      isAlive                       *
 ******************************************************/
bool thing::isAlive()
{
   return(!dead);
}

