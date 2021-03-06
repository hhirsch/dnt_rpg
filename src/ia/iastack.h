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

#ifndef _dnt_iastack_h
#define _dnt_iastack_h

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "../etc/defparser.h"

/*! The jumps positions for if, while, for, etc. */
class iaJumpPos
{
   public:
      std::streampos begin;     /**< The begin of the block */
      std::streampos end;       /**< The end of the block (normally used only on
                                for and whiles). */
      int lineNumber;      /**< The file line number (for errors show) */
      iaJumpPos* previous; /**< Previous jump on the stack */
      std::string command; /**< The token command (if, else, for) */
};

/*! The ia Stack of Jumps. */
class iaStack
{
   public:
      /*! Constructor */
      iaStack();
      /*! Destructor */
      ~iaStack();

      /*! Pop the last
       * \return -> last jumpPos on the stack */
      iaJumpPos* pop();

      /*! Push a jumppos to the stack
       * \param jmp -> jump struct to be put. */
      void push(iaJumpPos* jmp);

      /*! Print the stack to the cerr for debug */
      void print();

      /*! Save the Stack to a file
       * \param file -> ofstream file to save to */
      void save(std::ofstream* file);

      /*! Load the defparser from a file
       * \param def -> defParser to load from */
      void load(defParser* def);

   protected:
      /*! Insert a iaJumpPos at the stack's bottom
       * \param jmp -> jmp to be at bottom 
       * \note -> usually used at stack load from file */
      void insertAtBottom(iaJumpPos* jmp);

   private:
      iaJumpPos* top;      /**< The pointer to the top of the stack */
      iaJumpPos* bottom;   /**< The pointer to the bottom of the stack */
      int total;           /**< The total of the stack */
};

#endif

