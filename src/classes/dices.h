#ifndef _dices_h
#define _dices_h

/******************************************************************
 *                        DICE CONSTANTS                          *
 ******************************************************************/
#define DICE_D2    2
#define DICE_D4    4
#define DICE_D6    6
#define DICE_D8    8
#define DICE_D10  10
#define DICE_D12  12
#define DICE_D20  20

/******************************************************************
 *                           DICE STRUCT                          *
 ******************************************************************/
typedef struct _dice
{
   int diceID;           /* ID of Dice */
   int numberOfDices;    /* Number of Dices */
   int sumNumber;        /* Base Number to Sum */
}dice;

typedef struct _diceThing
{
   /* Note: the number is calculated by: 
        baseDice + SUM((level-initialLevel)/aditionalLevels)*aditionalDice
        And Level can be anything that is needed, from level itself to 
        skills and base attributes.
    */
   dice baseDice;         /* Basic Dice */
   dice aditionalDice;    /* Aditional Dice per Level */
   int aditionalLevels;   /* Number of Levels per Aditional Dices */
   int initialLevel;      /* Initial Level to use this */
}diceThing;


#endif

