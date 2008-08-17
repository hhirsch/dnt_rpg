#ifndef _xp_h
#define _xp_h

#include "../engine/character.h"

/*! Get the XP value for the actor doing something to target
 * the something can be a kill, a success on using skill, etc. 
 * \param actualActor -> pointer to the actor character 
 * \param targetCR -> Challenge Rate of the target 
 * \return total amount of XP. */
int getXP(character* actualActor, float targetCR);

/*! Get the next number of Experience Points needed to advance
 * a level, based on the current XP level.
 * \param curXP -> current number of experience points
 * \return -> needed experience points to advance to next level */
int nextLevelXP(int curXP);

/*! Get the character level respective to the XP amount
 * \param curXP -> current number of experience points 
 * \return -> character level relative to this XP quantity*/
int getXPLevel(int curXP);

#endif

