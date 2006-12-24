#ifndef _fuzzy_h
#define _fuzzy_h

#include "fuzzyrule.h"
#include "fuzzyfunction.h"
#include "fuzzyvariable.h"

/*! The Fuzzy Logic Class 
 *  In the fuzzy class, the functions are used as operator values to rules,
 *  the variables are the return values of rules. */
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
                                  float xa, float xb, float xc, float xd);

       /*! Add variable to the fuzzy class
        * \param functionType -> function type
        * \param xa -> first used coordinate 
        * \param xb -> second used coordinate
        * \param xc -> third used coordinate
        * \param xd -> forth used coordinate
        * \return -> pointer to the variable created */
       fuzzyVariable* addVariable(int functionType, 
                                  float xa, float xb, float xc, float xd);

       /*! Add Rule to the fuzzy class 
        * \param numberOfOperators -> numer of Operators in the Rule
        * \return -> pointer to the fuzzy rule created */
       fuzzyRule* addRule(int numberOfOperators);

       /*! Evalute all actual CrispValues in all Functions,
        *  putting the result in the respective variables, 
        *  based on the rules. */
       void evalute();

       /*! Do the Defuzzyfication of Value */
       float defuzzyfication();

   private:
      fuzzyRule* rules;         /**< FuzzyRules List */
      int totalRules;           /**< Total Fuzzy Rules */

      fuzzyFunction* functions; /**< Fuzzy Functions List */
      int totalFunctions;       /**< Total Fuzzy Rules */

      fuzzyVariable* variables; /**< Fuzzy Variables List */
      int totalVariables;       /**< Total Fuzzy Rules */

};


#endif

