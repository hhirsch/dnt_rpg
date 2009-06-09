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

#ifndef _dnt_fuzzyfunction_h
#define _dnt_fuzzyfunction_h

/*! Fuzzy Membership Functions */
#define FUZZY_FUNCTION_STEP          1
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
      fuzzyFunction(int func, float xa, float xb, float xc, float xd);
      
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

