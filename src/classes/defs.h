#ifndef _defs_h
#define _defs_h

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
        baseDice + ((level-initialLevel)/aditionalLevels)*aditionalDice
    */
   dice baseDice;         /* Basic Dice */
   dice aditionalDice;    /* Aditional Dice per Level */
   int aditionalLevels;   /* Number of Levels per Aditional Dices */
   int initialLevel;      /* Initial Level to use this */
}diceThing;

/******************************************************************
 *                        CLASSES CONSTANTS                       *
 ******************************************************************/
#define CLASS_EADM          0
#define CLASS_EBIOLOGIA     1
#define CLASS_EDCC          2
#define CLASS_EDFISICA      3
#define CLASS_EMECANICA     4
#define CLASS_EFILOSOFIA    5
#define CLASS_EFISIOTERAPIA 6
#define CLASS_EMEDICINA     7
#define CLASS_EMUSICA       8
#define CLASS_ETO           9
#define STR_EADM          "EADM"
#define STR_EBIOLOGIA     "EBIOLOGIA"
#define STR_EDCC          "EDCC"
#define STR_EDFISICA      "EDFISICA"
#define STR_EMECANICA     "EMECANICA"
#define STR_EFILOSOFIA    "EFILOSOFIA"
#define STR_EFISIOTERAPIA "EFISIOTERAPIA"
#define STR_EMEDICINA     "EMEDICINA"
#define STR_EMUSICA       "EMUSICA"
#define STR_ETO           "ETO"

#endif
