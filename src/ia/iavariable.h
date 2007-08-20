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

      /*! Assign Operator */
      void operator=(const iaVariable& v);

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
};

#endif

