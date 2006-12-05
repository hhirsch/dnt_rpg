#ifndef _fuzzyrule_h
#define _fuzzyrule_h

#include "fuzzyfunction.h"
#include "fuzzyvariable.h"

/*! Fuzzy Operators */
#define FUZZY_OPERATOR_AND     1
#define FUZZY_OPERATOR_AND_NOT 2
#define FUZZY_OPERATOR_OR      3
#define FUZZY_OPERATOR_OR_NOT  4

/*! A Rule in the Fuzzy Class */
class fuzzyRule
{
   public:
      /*! Constructor
       * \param total -> total number of members */
      fuzzyRule(int total);
      /*! Destructor */
      ~fuzzyRule();

      /*! Add Member and operator to Rule
       * \param op -> operator to be applied
       * \param function -> function to be avaliated */
      bool addMember(int op, fuzzyFunction* function);

      /*! Add the result variable to the Rule
       * \param variable -> the result variable */
      void addVariable(fuzzyVariable* variable);

      /*! Evalute The Rule, setting the return Variable */
      void evalute();

      fuzzyRule* next;
      fuzzyRule* previous;
      
   private:
      int totalMembers;              /**< Total Members that rule will need */
      int numberOfMembers;           /**< Number of Members in Rule */
      int* operators;                /**< Operators of Rule */
      fuzzyFunction** functions;     /**< Vector of Functions in Rule */
      fuzzyVariable* returnVariable; /**< Return variable of the Rule */

      /*! Calculate the Fuzzy And Value
       * \param op1 -> first operator
       * \param op2 -> second operator
       * \return -> fuzzy And Value */
      float andValue(float op1, float op2);
      
      /*! Calculate the Fuzzy Or Value
       * \param op1 -> first operator
       * \param op2 -> second operator
       * \return -> fuzzy Or Value */
      float orValue(float op1, float op2);

      
};


#endif

