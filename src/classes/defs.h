#ifndef _defs_h
#define _defs_h

/** \file defs.h
 *  Here is where all game rules definitions resides. Again, some of them are define two times, one with the internal number, other with the definition string.
 */

#include "dices.h"
#include <string>
using namespace std;


#define MAX_ATTACKS 6

/******************************************************************
 *                        ATTRIBUTES CONSTANTS                    *
 ******************************************************************/
#define ATT_LEVEL        0
#define ATT_STRENGTH     1
#define ATT_DEXTERITY    2
#define ATT_CONSTITUTION 3
#define ATT_INTELLIGENCE 4
#define ATT_WISDOM       5
#define ATT_CHARISM      6
#define ATT_SKILL_FIRST  7
#define ATT_SKILL_LAST   46

#define VAR_AUX1         40
#define VAR_AUX2         41
#define VAR_AUX3         42
#define VAR_AUX4         43
#define VAR_AUX5         44
#define VAR_AUX6         45

#define WORK_ON_ALL            10000
#define STR_WORK_ON_ALL        "WORK_ON_ALL"
#define NONE                   10001
#define STR_NONE               "NONE" 

/******************************************************************
 *                            SEX CONSTANTS                       *
 ******************************************************************/
#define SEX_MALE        0
#define SEX_FEMALE      1
#define SEX_OTHER       2

/*! The attackBonus class represents a single base attack bonus */
class attackBonus
{
   public:
      /*! Constructor */
      attackBonus();

      /*! Constructor
       * \total -> total bonus value
       * \note -> usually, the total is relative to the 
       *          first attack bonus.
       * \note -> you can get the total value if the toInt() function */
      attackBonus(int total);

      /*! Destructor */
      ~attackBonus();

      /*! Clear the bonus values */
      void clear();

      /*! + Overloaded operator -> will sum attackBonus
       * \param atBonus -> attackBonus to sum
       * \return -> sum result */
      attackBonus operator+(const attackBonus& atBonus);

      /*! = Overloaded operator
       * \param atBonus -> attackBonus to use as base */
      attackBonus& operator=(const attackBonus& atBonus);

      /*! Convert the attack bonus to a string 
       * \return -> string representing the attack bonus */
      string toString();

      /*! Convert the attack bonus to a representativ integer
       * \return -> int with the attack bonus */
      int toInt();

      /*! Get bonus for an attack
       * \param attackNumber -> number of the attack (1 for first) 
       * \return -> base attack bonus for the attack */
      int getBonus(int attackNumber);

      /*! Set bonus for an attack
       * \param attackNumbner -> number of the attack (1 for first)
       * \param value -> bonus value for the attack */
      void setBonus(int attackNumber, int value);

    protected:
      int bonus[MAX_ATTACKS];  /**< The Bonuses itself */
};


/*! Per Level Bonus and Saves */
class bonusAndSaves
{
   public:
      /*! Constructor */
      bonusAndSaves();
      /*! Destructor */
      ~bonusAndSaves();

      /*! Clear Values */
      void clear();

      /*! = Operator */
      bonusAndSaves& operator=(const bonusAndSaves& b);

      /*! + Operator */
      bonusAndSaves operator+(const bonusAndSaves& b);

      int level;                          /**< Class Level */
      attackBonus baseAttackBonus;        /**< Base Attack Bonus */
      int fortitude;                      /**< Fortitude Save Bonus */
      int reflex;                         /**< Reflex Save Bonus */
      int iAmNotAFool;                    /**< I Am Not A Fool Bonus */
};



#endif
