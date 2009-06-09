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

#ifndef _dnt_fuzzyvariable_h
#define _dnt_fuzzyvariable_h

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

      fuzzyVariable* next;
      fuzzyVariable* previous;
      
   private:
      float value;             /**< value of the variable */
      fuzzyFunction* function; /**< function of the variable */
};

#endif

