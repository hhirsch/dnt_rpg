#ifndef _fightGroup_h
#define _fightGroup_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "../engine/personagens.h"

#define FIGHT_MAX_CHARACTERS_PER_GROUP 5 /**< Max avaible characters per group*/

/*! A fight group is a group of allied characters in battle.*/
//TODO friendly and enemy groups to a group.
class fightGroup
{
   public:
     /*! Group Constructor */
     fightGroup();
     /*! Group Destructor */
     ~fightGroup();

    /*!
     *************************************************************** 
     * Insert Character on Group. Return true if OK.  
     * \param pers -> Pointer to Character
     * \return true if success.
     ***************************************************************/
     bool insertCharacter(personagem* pers);
     /*!
     *************************************************************** 
     * Verify if character is in Group  
     * \param pers -> Pointer to Character
     * \return true if character is in group.
     ***************************************************************/
     bool isCharacterIn(personagem* pers);
     /*!
     *************************************************************** 
     * Verify if there's any character alive and in range 
     * in the Group  
     * \return true if someone is alive and in range.
     ***************************************************************/
     bool anyoneIsAliveAndInRange();
     /*!
     *************************************************************** 
     * Get better to attack character enemy to pers.  
     * \param pers -> character pointer.
     * \return an character pointer, enemy to this one.
     ***************************************************************/
     personagem* getNPCEnemy(personagem* pers);

     /*!
     *************************************************************** 
     * Empty the group  
     ***************************************************************/
     void empty();

   private:
     /*! Pointers to Characters on Group */
     personagem* characters[FIGHT_MAX_CHARACTERS_PER_GROUP]; 
     /*! Total number of Characters on Group */
     int actualCharacters;
};

#endif

