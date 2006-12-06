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

      /*! Set the Crisp Value of the Function. 
       * Must be called before evalute.
       * \param crisp -> Crisp Value to be evaluted */
      void setCrispValue(float crisp);

      /*! Evalute The Function
       * \return value of the function */
      float evalute();

      /*! Set the Upper Value of the function
       * \param value -> limit value to set */
      void setLimit(float value);

      fuzzyFunction* next;      /**< Next Fuzzy Rule on List */
      fuzzyFunction* previous;  /**< Previous Fuzzy on List */

   private:
      int function;         /**< Membership Function of The Rule */
      float x1,             /**< First Used Coordinate */
            x2,             /**< Second Used Coordinate */
            x3,             /**< Third Used Coordinate */
            x4;             /**< Forth Used Coordinate */   
      float crispValue;     /**< Actual Crisp Value */
      float upperValue;     /**< Actual Upper Value */
};


#endif

