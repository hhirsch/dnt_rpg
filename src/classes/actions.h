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

#ifndef _dnt_actions_h
#define _dnt_actions_h

#include "thing.h"
#include "dices.h"
#include <string>

#define ACT_OPEN                  0
#define ACT_ATTACK                1
#define ACT_DRINK                 2
#define ACT_EAT                   3
#define ACT_DEFEND                4
#define ACT_TALK                  5
#define ACT_CLOSE                 6
#define ACT_MOVE                  7
#define ACT_TAKE                  8
#define ACT_FREE                  9
#define ACT_MODIFY_OWN_CONCEPT   10
#define ACT_MODIFY_OTHER_CONCEPT 11
#define ACT_CONJURE              12
#define ACT_MODIFY_OBJECT        13
#define ACT_HEAL                 14
#define ACT_FIX                  15
#define ACT_BREAK                16
#define ACT_WAIT                 17

#define ACT_STR_OPEN                 "ACT_OPEN"
#define ACT_STR_ATTACK               "ACT_ATTACK"
#define ACT_STR_DRINK                "ACT_DRINK"
#define ACT_STR_EAT                  "ACT_EAT"
#define ACT_STR_DEFEND               "ACT_DEFEND"
#define ACT_STR_TALK                 "ACT_TALK"
#define ACT_STR_CLOSE                "ACT_CLOSE"
#define ACT_STR_MOVE                 "ACT_MOVE"
#define ACT_STR_TAKE                 "ACT_TAKE"
#define ACT_STR_FREE                 "ACT_FREE"
#define ACT_STR_MODIFY_OWN_CONCEPT   "ACT_MODIFY_OWN_CONCEPT"
#define ACT_STR_MODIFY_OTHER_CONCEPT "ACT_MODIFY_OTHER_CONCEPT"
#define ACT_STR_CONJURE              "ACT_CONJURE"
#define ACT_STR_MODIFY_OBJECT        "ACT_MODIFY_OBJECT"
#define ACT_STR_HEAL                 "ACT_HEAL"
#define ACT_STR_FIX                  "ACT_FIX"
#define ACT_STR_BREAK                "ACT_BREAK"
#define ACT_STR_WAIT                 "ACT_WAIT"


#define ACT_TYPE_FREE_ACTION       0
#define ACT_TYPE_NORMAL_ACTION     1
#define ACT_TYPE_SECUNDARY_ACTION  2
#define ACT_TYPE_NO_ACTION         3

#define ACT_TYPE_STR_FREE_ACTION       "ACT_TYPE_FREE_ACTION"
#define ACT_TYPE_STR_NORMAL_ACTION     "ACT_TYPE_NORMAL_ACTION"
#define ACT_TYPE_STR_SECUNDARY_ACTION  "ACT_TYPE_SECUNDARY_ACTION"
#define ACT_TYPE_STR_NO_ACTION         "ACT_TYPE_NO_ACTION"

#define ACT_MOVE_DELTA 5

/* Forward declaration */
class engine;

/*!
 ************************************************************* 
 * Return the number of the action with name buffer  
 * \param buffer -> string with the name of the concept
 * \return correspondent action Type number.
 ***************************************************************/
int numberAction(std::string buffer);

/*!
 ************************************************************** 
 * Return the number of the Action Type with name buffer  
 * \param buffer -> string with the name of the concept
 * \return correspondent action Type number.
 ***************************************************************/
int numberActionType(std::string buffer);

/*!
 **************************************************************** 
 * Do an action that will affect the Life Points of the Target
 * \param actor -> thing that will act
 * \param target -> target of the action
 * \param diceInfo -> the action dice
 * \param conceptBonus -> pointer to the concept bonus (if one), or NULL
 * \param range -> action range (==0 to not verify range: usually when already
 *                               verified)
 * \param heal -> true if is a heal/fix feat, 
 *                false if is an attack/break one.
 * \note -> this wil use armatureClass as against concept
 * \return \c true if could attack
 ***************************************************************/
bool doHealOrAttack(thing* actor, thing* target, 
                    diceThing diceInfo, factor* conceptBonus,
                    int range, bool heal);

/*! Do an action that will affect the Life Points of the Target
 * \param actor -> thing that will act
 * \param target -> target of the action
 * \param diceInfo -> the action dice
 * \param conceptBonus -> pointer to the concept bonus (if one), or NULL
 * \param conceptAgainst -> the target's concept to roll against (NULL for no
 * resistence test)
 * \param range -> action range (==0 to not verify range: usually when already
 *                               verified)
 * \param heal -> true if is a heal/fix feat, 
 *                false if is an attack/break one.
 * \return \c true if could attack */
bool doHealOrAttack(thing* actor, thing* target, 
                    diceThing diceInfo, factor* conceptBonus,
                    factor* conceptAgainst, int range, bool heal);

/*! Do an action that will negatively affect the life points of
 * characters  in the area.
 * \note : TODO: afect objects too.
 * \param actor -> thing that will act 
 * \param targetX -> central X target
 * \param targetZ -> central Z target
 * \param diceInfo -> the action dice
 * \param conceptBonus -> pointer to the concept bonus (if one), or NULL
 * \param conceptAgainst -> the target's concept to roll against (NULL for no
 *  resistance test)
 * \param radius -> radius of target circle
 * \param range -> action range 
 * \param eng -> pointer to current DNT engine */
bool doAreaAttack(thing* actor, float targetX, float targetZ, int radius,
                  diceThing diceInfo, factor* conceptBonus,
                  factor* conceptAgainst, int range, engine* eng);

#endif

