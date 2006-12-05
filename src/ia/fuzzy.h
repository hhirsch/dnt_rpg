#ifndef _fuzzy_h
#define _fuzzy_h

#include "fuzzyrule.h"
#include "fuzzyfunction.h"
#include "fuzzyvariable.h"

/*! The Fuzzy Logic Class */
class fuzzy
{
   public:
      /*! Constructor */
      fuzzy();
      /*! Destructor */
      ~fuzzy();

       /* Add Function to the fuzzy class
        * \param func -> function type
        * \param xa -> first used coordinate 
        * \param xb -> second used coordinate
        * \param xc -> third used coordinate
        * \param xd -> forth used coordinate
        * \return -> pointer to the fuzzyFunctionCreated */
       fuzzyFunction* addFunction(int functionType, 
                                  int xa, int xb, int xc, int xd);

       /*! Add variable to the fuzzy class
        * \return -> pointer to the variable created */
       fuzzyVariable* addVariable();

       /*! Add Rule to the fuzzy class 
        * \param numberOfOperators -> numer of Operators in the Rule
        * \return -> pointer to the fuzzy rule created */
       fuzzyRule* addRule(int numberOfOperators);

   private:
      fuzzyRule* rules;         /**< FuzzyRules List */
      int totalRules;           /**< Total Fuzzy Rules */

      fuzzyFunction* functions; /**< Fuzzy Functions List */
      int totalFunctions;       /**< Total Fuzzy Rules */

      fuzzyVariable* variables; /**< Fuzzy Variables List */
      int totalVariables;       /**< Total Fuzzy Rules */

};


#endif

