/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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

#ifndef _dnt_fuzzy_h
#define _dnt_fuzzy_h

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

