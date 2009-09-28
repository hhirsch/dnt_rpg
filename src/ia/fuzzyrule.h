/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_fuzzyrule_h
#define _dnt_fuzzyrule_h

#include "../etc/list.h"

#include "fuzzyfunction.h"
#include "fuzzyvariable.h"

/*! Fuzzy Operators */
#define FUZZY_OPERATOR_AND     1
#define FUZZY_OPERATOR_AND_NOT 2
#define FUZZY_OPERATOR_OR      3
#define FUZZY_OPERATOR_OR_NOT  4

/*! A Rule in the Fuzzy Class */
class fuzzyRule: public dntListElement
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

/*! List of fuzzy rules */
class fuzzyRuleList: public dntList
{
   public:
      /*! Constructor */
      fuzzyRuleList();
      /*! Destructor */
      ~fuzzyRuleList();

   protected:
      /*! Free the fuzzy variable
       * \param obj -> pointer to the object to delete */
      void freeElement(dntListElement* obj);
};



#endif

