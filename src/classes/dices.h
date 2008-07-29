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
class dice
{
   public:
      /*! Constructor: Create a dice of the type
       * \param type -> the dice type constant */
      dice(int type);
      /*! Constructor: Create a d20 dice */
      dice();
      /*! Destructor */
      ~dice();

      /*! Roll the dices
       * \param critical -> true if is a critical roll
       * \note -> at critical rolls, the dices are 
       *          rerolled criticalMultiplier times */
      int roll(bool critical=false);

      /*! Set the dice type
       * \param type -> dice type constant */
      void setType(int type);
      /*! Get the dice type
       * \return -> dice type */
      int getType();

      /*! Set the number of dices (default: 1)
       * \param num -> number of dices value */
      void setNumberOfDices(int num);
      /*! Get the current number of dices
       * \return -> number of dices */
      int getNumberOfDices();

      /*! Set the sum number (default: 0)
       * \param num -> sum number value */
      void setSumNumber(int num);
      /*! Get the current sum number
       * \return -> sum number */
      int getSumNumber();

      /*! Set the critical multiplier (default: 1)
       * \param num -> critical multiplier value */
      void setCriticalMultiplier(int num);
      /*! Get the current critical multiplier
       * \return -> critical multiplier */
      int getCriticalMultiplier();

   protected:
 
      /*! Set initial dice values
       * \param type -> dice type constant */
      void init(int type);

      int diceID;             /**< ID of Dice. Represents the number of faces.*/
      int numberOfDices;      /**< Number of Dices to roll. */
      int sumNumber;          /**< Base Number to Sum to value.*/
      int criticalMultiplier; /**< The critical Multiplier Value */
};

/*! 
  This is the dice Thing struct, with all things to know about one dice. 
  \note the number on dice is calculated by: \par
    \e baseDice+SUM((level-initialLevel)/aditionalLevels)*aditionalDice \par
    And Level can be anything that is needed, from level itself to 
    skills and base attributes.
*/
class diceThing
{
   public:
      dice baseDice;         /**< Basic Dice. */
      dice aditionalDice;    /**< Aditional Dice per Level. */
      int aditionalLevels;   /**< Number of Levels per Aditional Dices. */
      int initialLevel;      /**< Initial Level to use this diceThing. */
};


#endif

