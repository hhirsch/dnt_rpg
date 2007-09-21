#ifndef _xp_h
#define _xp_h

#include "../engine/character.h"

/*! Get the XP value for the actor doing something to target
 * the something can be a kill, a success on using skill, etc. 
 * \param actualActor -> pointer to the actor character 
 * \param targetCR -> Challenge Rate of the target 
 * \return total amount of XP. */
int getXP(character* actualActor, float targetCR);


#endif

