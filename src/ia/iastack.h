#ifndef _iastack_h
#define _iastack_h

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#include "../etc/defparser.h"

/*! The jumps positions for if, while, for, etc. */
class iaJumpPos
{
   public:
      streampos begin;     /**< The begin of the block */
      streampos end;       /**< The end of the block (normally used only on
                                for and whiles). */
      int lineNumber;      /**< The file line number (for errors show) */
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

      /*! Print the stack to the cerr for debug */
      void print();

      /*! Save the Stack to a file
       * \param file -> ofstream file to save to */
      void save(ofstream* file);

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

