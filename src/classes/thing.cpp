#include "thing.h"
#include "xp.h"
#include "defs.h"

#include "../engine/util.h"
#include "../engine/dialog.h" 
#include "../ia/iascript.h"

#include <math.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

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
   reflex = 0;
   iAmNotAFool = 0;
   armatureClass = 0;
   displacement = 0;
   baseAttackModifier = 0;
   sizeModifier = 0;
   psychoState = PSYCHO_NEUTRAL;
   xPosition = 0;
   yPosition = 0;
   zPosition = 0;
   initialXPosition = 0;
   initialZPosition = 0;
   xp = 0;
   upLevels = 0;
   cr = 1;
   xpPercent = 100;
   dead = false;
   battleScript = NULL;
   battleScriptFileName = "";

   currentEnemy = NULL;

   /* Set Default Values */
   dirs dir;
   bloodFileName = dir.getRealFile("particles/blood3.par");
   bloodPosition = 25;
   walk_interval = 37.5 * WALK_UPDATE; //default value, if not
                                       //defined at the def file.
   thingType = THING_TYPE_NONE;
}

/******************************************************
 *                    Destructor                      *
 ******************************************************/
thing::~thing()
{
   dead = true;
   if(battleScript)
   {
      iaScript* isc = (iaScript*)battleScript;
      delete(isc);
      battleScript = NULL;
   }
}

/******************************************************
 *                   getThingType                     *
 ******************************************************/
int thing::getThingType()
{
   return(thingType);
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
   skill* s = NULL;
   if(something.type == MOD_TYPE_ATT)
   {
      s = sk.getSkillByString(something.id);
      if(s)
      {
         return(attBonus(s));
      }
      else
      {
         cerr << "Unknow Attribute: " << something.id << endl;
      }
   }
   else if(something.type == MOD_TYPE_SKILL)
   {
      s = sk.getSkillByString(something.id);
      if(s)
      {
         return(skillBonus(s));
      }
      else
      {
         cerr << "Unknow Skill: " << something.id << endl;
      }
   }
   
   return(0);
}

/******************************************************
 *                   getBattleScript                  *
 ******************************************************/
void* thing::getBattleScript()
{
   return(battleScript);
}

/******************************************************
 *                   setBattleScript                  *
 ******************************************************/
void thing::setBattleScript(void* script, string scriptFileName)
{
   battleScript = script;
   battleScriptFileName = scriptFileName;
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
/******************************************************
 *                        getXP                       *
 ******************************************************/
int thing::getXP()
{
   return(xp);
}
/******************************************************
 *                        addXP                       *
 ******************************************************/
void thing::addXP(int points)
{
   int prevLevel=0, curLevel=0;

   /* Get the level before new XP added */
   prevLevel = getXPLevel(xp);

   /* add the XP */
   xp += points;

   /* Get the level after new XP added */
   curLevel = getXPLevel(xp);

   /* Define number of levels to Up */
   if(curLevel > prevLevel)
   {
      upLevels += curLevel - prevLevel;
   }
}

/******************************************************
 *                        setXP                       *
 ******************************************************/
void thing::setXP(int points)
{
   xp = points;
}

/******************************************************
 *                     getUpLevels                    *
 ******************************************************/
int thing::getUpLevels()
{
   return(upLevels);
}

/******************************************************
 *                     decUpLevels                    *
 ******************************************************/
void thing::decUpLevels()
{
   upLevels--;

   /* Avoid underflow */
   if(upLevels < 0)
   {
      upLevels = 0;
   }
}

/******************************************************
 *                    getLifePoints                   *
 ******************************************************/
int thing::getLifePoints()
{
   return(lifePoints);
}

/******************************************************
 *                     addLifePoints                  *
 ******************************************************/
void thing::addLifePoints(int points)
{
   lifePoints += points;
   if(lifePoints > maxLifePoints)
   {
      lifePoints = maxLifePoints;
   }
   updateHealthBar();
}

/******************************************************
 *                     setLifePoints                  *
 ******************************************************/
void thing::setLifePoints(int points)
{
   lifePoints = points;
   updateHealthBar();
}

/******************************************************
 *                  getMaxLifePoints                  *
 ******************************************************/
int thing::getMaxLifePoints()
{
   return(maxLifePoints);
}
/******************************************************
 *                  setMaxLifePoints                  *
 ******************************************************/
void thing::setMaxLifePoints(int points)
{
   maxLifePoints = points;
   lifePoints = maxLifePoints;
   updateHealthBar();
}

/******************************************************
 *                   getPsychoState                   *
 ******************************************************/
int thing::getPsychoState()
{
   return(psychoState);
}
/******************************************************
 *                   setPsychoState                   *
 ******************************************************/
void thing::setPsychoState(int state)
{
   psychoState = state;
}

/*********************************************************************
 *                        setConversationFile                        *
 *********************************************************************/
void thing::setConversationFile(string file)
{
   conversationFile = file;
}

/*********************************************************************
 *                        getConversationFile                        *
 *********************************************************************/
string thing::getConversationFile()
{
   return(conversationFile);
}

/*********************************************************************
 *                          getConversation                          *
 *********************************************************************/
void* thing::getConversation()
{
   return(conv);
}

/*********************************************************************
 *                        createConversation                         *
 *********************************************************************/
void thing::createConversation(string curMap)
{
   if(conversationFile != "")
   {
      conv = (void*) new conversation();
      conversation* cs = (conversation*)conv;
      cs->loadFile(conversationFile);
      cs->setOwner(this, curMap);
   }
}

/*********************************************************************
 *                      setInitialConversation                       *
 *********************************************************************/
void thing::setInitialConversation(int i)
{
   if(conv != NULL)
   {
      conversation* cs = (conversation*)conv;
      cs->setInitialDialog(i);
   }
}

