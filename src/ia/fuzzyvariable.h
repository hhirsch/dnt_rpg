/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_fuzzyvariable_h
#define _dnt_fuzzyvariable_h

#include "../etc/list.h"

#include "fuzzyfunction.h"

/*! Fuzzy Variable (usually destiny one of a rule) */
class fuzzyVariable: public dntListElement
{
   public:
      /*! Constructor 
       * \param func -> fucntion type
       * \param xa -> first used coordinate 
       * \param xb -> second used coordinate
       * \param xc -> third used coordinate
       * \param xd -> forth used coordinate */
      fuzzyVariable(int func, float xa, float xb, float xc, float xd);
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

   private:
      float value;             /**< value of the variable */
      fuzzyFunction* function; /**< function of the variable */
};

/*! List of fuzzy variables */
class fuzzyVariableList: public dntList
{
   public:
      /*! Constructor */
      fuzzyVariableList();
      /*! Destructor */
      ~fuzzyVariableList();

   protected:
      /*! Free the fuzzy variable
       * \param obj -> pointer to the object to delete */
      void freeElement(dntListElement* obj);
};

#endif

