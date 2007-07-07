#ifndef _dices_h
#define _dices_h

/******************************************************************
 *                        DICE CONSTANTS                          *
 ******************************************************************/
#define DICE_D2    2     /**< Two Faces Dice */
#define DICE_D4    4     /**< Four Faces Dice */
#define DICE_D6    6     /**< Six Faces Dice */
#define DICE_D8    8     /**< Eight Faces Dice */
#define DICE_D10  10     /**< Ten Faces Dice */
#define DICE_D12  12     /**< Twelve Faces Dice */
#define DICE_D20  20     /**< Twenty Faces Dice */

/******************************************************************
 *                           DICE STRUCT                          *
 ******************************************************************/
/*! This Struct defines the internal dice, with value
       correspondent to \e numberOfDices*random(diceID)+sumNumber.*/
typedef struct _dice
{
   int diceID;             /**< ID of Dice. Represents the number of faces.*/
   int numberOfDices;      /**< Number of Dices to roll. */
   int sumNumber;          /**< Base Number to Sum to value.*/
   int criticalMultiplier; /**< The critical Multiplier Value */
}dice;

/*! 
  This is the dice Thing struct, with all things to know about one dice. 
  \note the number on dice is calculated by: \par
    \e baseDice+SUM((level-initialLevel)/aditionalLevels)*aditionalDice \par
    And Level can be anything that is needed, from level itself to 
    skills and base attributes.
*/
typedef struct _diceThing
{
   dice baseDice;         /**< Basic Dice. */
   dice aditionalDice;    /**< Aditional Dice per Level. */
   int aditionalLevels;   /**< Number of Levels per Aditional Dices. */
   int initialLevel;      /**< Initial Level to use this diceThing. */
}diceThing;


#endif

