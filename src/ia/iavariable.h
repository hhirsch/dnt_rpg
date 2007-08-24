#ifndef _iavariable_h
#define _iavariable_h

#include "iafuncs.h"
#include <string>
#include <iostream>
using namespace std;


/*! iaVariable Class. It's just a variable to use at ia scripts */
class iaVariable
{
   public:
      /*! Constructor */
      iaVariable(string varType, string varName);
      /*! Destructor */
      ~iaVariable();

      /*! Equal Operator */
      bool operator==(const iaVariable& v);

      /*! Difference Operator */
      bool operator!=(const iaVariable& v);

      /*! Gequal Operator */
      bool operator>=(const iaVariable& v);

      /*! Greater Operator */
      bool operator>(const iaVariable& v);

      /*! Lesser Operator */
      bool operator<(const iaVariable& v);

      /*! Lequal Operator */
      bool operator<=(const iaVariable& v);

      /*! Assign Operator */
      void operator=(const iaVariable& v);

      /*! Define the variable value to be the result of the operation on
       * the two variables.
       * \param operation -> operation to do
       * \param v1 -> pointer to the first variable
       * \param v2 -> pointer to the second variable (NULL on NOT !)*/
      void receiveOperation(string operation, iaVariable* v1, iaVariable* v2);

      /*!  Convert the variable value to a string one. */
      string toString();

      void* value;   /**< The value of the variable. */
      string type;   /**< The type string id of the variable */
      string name;   /**< The name of the variable */

      iaVariable* next;       /**< Next variable on the list */
      iaVariable* previous;   /**< Previous variable on the list */
};

/*! The symbols table of the language */
class iaSymbolsTable
{
   public:
      /*! Constructor */
      iaSymbolsTable();
      /*! Destructor */
      ~iaSymbolsTable();

      /*! Add the symbol to the table.
       * \param type -> type of the symbol;
       * \param name -> name of the symbol; */
      void addSymbol(string type, string name);

      /*! Add the symbol to the table.
       * \param type -> type of the symbol;
       * \return -> name of the symbol; */
      string addTempSymbol(string type);

      /*! Remove all temporary symbols from the table */
      void removeTempSymbols();

      /*! Remove the Symbol from the table
       * \param name -> symbol's name */
      void removeSymbol(string name);
      /*! Remove the Symbol from the table
       * \param symbol -> the symbol's pointer */
      void removeSymbol(iaVariable* symbol);

      /*! Get the symbol from the table
       * \param name -/. name of the symbol to get.
       * \return pointer to the symbol, or NULL if not found any. */
      iaVariable* getSymbol(string name);
   protected:
      iaVariable* first;   /**< First element on the list */
      int total;           /**< Total symbols on the list */
      int tempSymbol;      /**< The next temporary symbol number */
};

#endif

