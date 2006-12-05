#ifndef _fuzzyfunction_h
#define _fuzzyfunction_h

/*! Fuzzy Membership Functions */
#define FUZZY_FUNCTION_STEP           1
#define FUZZY_FUNCTION_INVERTED_STEP 2
#define FUZZY_FUNCTION_TRIANGULAR    3
#define FUZZY_FUNCTION_TRAPEZOIDAL   4

/*! A Function in the Fuzzy Class */
class fuzzyFunction
{
   public:
      /*! Constructor
       * \param func -> fucntion type
       * \param xa -> first used coordinate 
       * \param xb -> second used coordinate
       * \param xc -> third used coordinate
       * \param xd -> forth used coordinate*/
      fuzzyFunction(int func, int xa, int xb, int xc, int xd);
      
      /*! Destructor */
      ~fuzzyFunction();

      /*! Evalute The Function
       * \param crispValue -> Crisp Value to be evaluted
       * \return value of the function */
      float evalute(float crispValue);

      fuzzyFunction* next;      /**< Next Fuzzy Rule on List */
      fuzzyFunction* previous;  /**< Previous Fuzzy on List */

   private:
      int function;         /**< Membership Function of The Rule */
      float x1,             /**< First Used Coordinate */
            x2,             /**< Second Used Coordinate */
            x3,             /**< Third Used Coordinate */
            x4;             /**< Forth Used Coordinate */   
};


#endif

