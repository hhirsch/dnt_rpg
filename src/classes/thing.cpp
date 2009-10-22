/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
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

/******************************************************
 *                   Constructor                      *
 ******************************************************/
thing::thing()
{
   age = 0;
   sexType = SEX_MALE;
   lifePoints = 0;
   maxLifePoints = 0;
   armatureClass = 0;
   displacement = 0;
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

   canMove = true;
   canAttack = true;

   currentEnemy = NULL;

   /* Set Default Values */
   bloodFileName = "particles/blood3.par";
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
   int att=0;

   if(curSkill)
   {
      /* define the attribute bonus, if exists */
      if(curSkill->definition)
      {
         att = attBonus(curSkill->definition->baseAttribute);
      }

      return(att + curSkill->points);
   }

   cerr << "Warning: Tried to define bonus of undefined skill! " << endl;
   return(0);
}

/******************************************************
 *                     attBonus                       *
 ******************************************************/
int thing::attBonus(skill* curAttribute)
{
   return((int)floor((curAttribute->points-10) / 2.0));
}

/******************************************************
 *                     attBonus                       *
 ******************************************************/
int thing::attBonus(int curAttribute)
{
   return(attBonus(sk.getSkillByInt(curAttribute)));
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

/***********************************************************************
 *                                doCheck                              *
 ***********************************************************************/
bool thing::doCheck(string stateToCheck, int difficulty)
{
   skill* skl;
   bool couldCheck = false;
   bool res;

   /* First, let's test as skill */
   skl = sk.getSkillByString(stateToCheck);
   if(skl != NULL)
   {
      return(sk.doSkillCheck(skl, difficulty));
   }

   /* Nope, so let's test as a bonus and Save */
   res = curBonusAndSaves.doCheck(stateToCheck, difficulty, &couldCheck);
   if(couldCheck)
   {
      return(res);
   }

   /* Unknow! */
   cerr << "thing::doCheck - Unknown state to check: '" << stateToCheck 
        << "'" << endl;
   return(false);
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
 *                           getCanAttack                            *
 *********************************************************************/
bool thing::getCanAttack()
{
   return(canAttack);
}

/*********************************************************************
 *                           setCanAttack                            *
 *********************************************************************/
void thing::setCanAttack(bool can)
{
   canAttack = can;
}

/*********************************************************************
 *                            getCanMove                            *
 *********************************************************************/
bool thing::getCanMove()
{
   return(canMove);
}

/*********************************************************************
 *                            setCanMove                             *
 *********************************************************************/
void thing::setCanMove(bool can)
{
   canMove = can;
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

