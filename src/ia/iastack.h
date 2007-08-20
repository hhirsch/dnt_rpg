#ifndef _iastack_h
#define _iastack_h

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

/*! The jumps positions for if, while, for, etc. */
class iaJumpPos
{
   public:
      streampos begin;     /**< The begin of the block */
      streampos end;       /**< The end of the block (normally used only on
                                for and whiles). */
      iaJumpPos* previous; /**< Previous jump on the stack */
      string command;      /**< The token command (if, else, for) */
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

   private:
      iaJumpPos* top;      /**< The pointer to the top of the stack */
      int total;           /**< The total of the stack */
};

#endif

