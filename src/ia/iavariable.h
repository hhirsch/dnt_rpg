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

#ifndef _dnt_iavariable_h
#define _dnt_iavariable_h

#include "iafuncs.h"
#include "../etc/defparser.h"
#include "../etc/list.h"

#include <string>
#include <iostream>
#include <fstream>

/* \todo IA_TYPE_DICE toString and from string to save!*/

class engine;

/*! iaVariable Class. It's just a variable to use at ia scripts */
class iaVariable: public dntListElement
{
   public:
      /*! Constructor */
      iaVariable(std::string varType, std::string varName);
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

      /*! Assign Operator */
      void operator=(void* v);

      /*! Define the variable value to be the result of the operation on
       * the two variables.
       * \param operation -> operation to do
       * \param v1 -> pointer to the first variable
       * \param v2 -> pointer to the second variable (NULL on NOT !)*/
      void receiveOperation(std::string operation, 
                            iaVariable* v1, iaVariable* v2);

      /*! Change the signal of the current variable value */
      void changeSignal();

      /*!  Convert the variable value to a string one
       * \return -> string representing the variable value */
      std::string toString();

      /*! Define the variable value from a string
       * \param s -> string with variable value
       * \param curEngine -> pointer to the current engine */
      void fromString(std::string s, engine* curEngine);

      void* value;   /**< The value of the variable. */
      std::string type;   /**< The type string id of the variable */
      std::string name;   /**< The name of the variable */

};

/*! The symbols table of the language */
class iaSymbolsTable: public dntList
{
   public:
      /*! Constructor */
      iaSymbolsTable();
      /*! Destructor */
      ~iaSymbolsTable();

      /*! Add the symbol to the table.
       * \param type -> type of the symbol;
       * \param name -> name of the symbol; */
      iaVariable* addSymbol(std::string type, std::string name);

      /*! Add the symbol to the table.
       * \param type -> type of the symbol;
       * \return -> name of the symbol; */
      std::string addTempSymbol(std::string type);

      /*! Remove all temporary symbols from the table */
      void removeTempSymbols();

      /*! Called when map changed, to clear pointers */
      void mapChanged();

      /*! Verify if the variable is a temp one
       * \param var -> pointer to the variable
       * \return true if is a tem variable. */
      bool isTemp(iaVariable* var);

      /*! Remove the Symbol from the table
       * \param name -> symbol's name */
      void removeSymbol(std::string name);
      /*! Remove the Symbol from the table
       * \param symbol -> the symbol's pointer */
      void removeSymbol(iaVariable* symbol);

      /*! Get the symbol from the table
       * \param name -/. name of the symbol to get.
       * \return pointer to the symbol, or NULL if not found any. */
      iaVariable* getSymbol(std::string name);

      /*! Save the Symbols Table to a file
       * \param file -> ofstream file to save to */
      void save(std::ofstream* file);

      /*! Load the Symbols Table from a file
       * \param def -> defParser to load from
       * \param curEngine -> pointer to the engine used */
      void load(defParser* def, engine* curEngine);

   protected:
      /*! Delete the variable memory
       * \param obj -> iaVariable to delete */
      void freeElement(dntListElement* obj);

      int tempSymbol;      /**< The next temporary symbol number */
};

#endif

