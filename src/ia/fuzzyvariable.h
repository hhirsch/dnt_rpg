#ifndef _fuzzyvariable_h
#define _fuzzyvariable_h

#include "fuzzyfunction.h"

/*! Fuzzy Variable (usually destiny one of a rule) */
class fuzzyVariable
{
   public:
      /*! Constructor 
       * \param func -> fucntion type
       * \param xa -> first used coordinate 
       * \param xb -> second used coordinate
       * \param xc -> third used coordinate
       * \param xd -> forth used coordinate */
      fuzzyVariable(int func, int xa, int xb, int xc, int xd);
      /*! Destructor */
      ~fuzzyVariable();

      /*! Clear the Value of Variable */
      void clearValue();
      
      /*! Set Value of the Variable */
      void setValue(float val);
      
      /*! Get value of the variable */
      float getValue();

      /*! Evalute the Variable Function at Point X
       * \param x -> point where will evalute the function 
       * \return value of the function at point x */
      float evaluteFunction(float x);

      fuzzyVariable* next;
      fuzzyVariable* previous;
      
   private:
      float value;             /**< value of the variable */
      fuzzyFunction* function; /**< function of the variable */
};

#endif

