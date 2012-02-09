/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

#ifndef _dnt_iascript_h
#define _dnt_iascript_h

#include "iafuncs.h"
#include "iavariable.h"
#include "iastack.h"

#include "../engine/character.h"
#include "../classes/object.h"
#include "../classes/dices.h"
#include "../classes/feats.h"
#include "../classes/skills.h"
#include "../map/map.h"
#include "../classes/actions.h"
#include "../classes/pendingaction.h"

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#define MAX_SCRIPT_LINES 2 /**< Max interpreted script lines per cycle */

#define IASCRIPT_TYPE_DEFAULT   0  /**< Default type */
#define IASCRIPT_TYPE_MISSION   1  /**< Mission type */

class engine;

/*! The ia Script is the script that controls the behaviour
 * of characters, objects and events on DNT. */
class iaScript
{
   public:
      /*! Constructor
       * \param usedEngine -> the pointer to the engine used
       * \note -> if you use this contstructor, you must define latter the
       * script's fileName and call the init() function. */
      iaScript(engine* usedEngine);
      /*! Constructor
       * \param scriptFile -> fileName of the script
       * \param usedEngine -> the pointer to the engine used */
      iaScript(std::string scriptFile, engine* usedEngine);
      /*! Destructor */
      ~iaScript();

      /*! Init the script for use (open file, create structs, etc) */
      void init();

      /*! Close the iaScript. A closed iaScript can't run. */
      void close();

      /*! Set a script parameter value
       * \param paramName -> the parameter name (defined at the script's file)
       * \param value -> pointer to the parameter value or 
       *                 the value if already is a pointer type. 
       * \return true if parameter was defined.
       * \note: to define a character parameter:
       *    character *c = some_pointer_to_the_character;
       *    setParameter("name", c);
       * \note : to define an integer parameter: 
       *    int val = 50;
       *    setParameter("name", &val); */
      bool setParameter(std::string paramName, void* value);
   
      /*! Define the owner of the script as a object
       * \param owner -> pointer to the object */
      void defineObjectOwner(object* owner);
      /*! Define the owner of the script as a Character
       * \param owner -> pointer to the character */
      void defineCharacterOwner(character* owner);
      /*! Get the character owner of the script (if any) */
      character* getCharacterOwner();

      /*! Define the actual openned Map
       * \param acMap -> pointer to the opened map
       * \param NPCs -> pointer to the current npcs list */
      void defineMap(Map* acMap, characterList* NPCs);

      /*! run the Script to the point of a more than one cicle action,
       * that will be marked as pending or at the end of the script.
       * \param maxLines -> maxLines that will be interpreted at this
       *                    cicle. 0 for no line limit. */
      void run(int maxLines);

      /*! Verify if the script is finished or not
       * \return -> if the end of the script is found. False otherwise. */
      bool finished();

      /*! restart the script to the initial status (for example, to run 
       *  another time)  */
      void restart();

   protected:
      std::ifstream file;        /**< The script's file */
      std::string fileName;      /**< FileName of the script */
      object* objectOwner;       /**< Object Owner */
      character* characterOwner; /**< Character Owner */

      engine* actualEngine;      /**< The actual Engine */

      Map* actualMap;            /**< Actual Opened Map */
      characterList* npcs;       /**< Current NPCs List */

      int actualLine;            /**< The actual Line */

      std::streampos lastPos;    /**< Last pos before the read */

      std::string context;       /**< The context */

      int type;                  /**< Define the type if is a child */

      pendingAction* pendAction; /**< Pending Action on the script. Only
                                      will advance on script when receive
                                      that this actions was done. */

      iaSymbolsTable* symbols;   /**< The script's table of symbols */
      iaStack* jumpStack;        /**< The jump stack (if, else, for) */

      /*! Create all the parameters variables 
       * \note parameters are defined at script(params) */
      void createParameters();

      /*! Declare a variable at the symbol's table, with the declaration
       * line, if it is with right syntax. The syntax for declaration is
       * type name1 name2 name3 
       * where type is the type of the variable and name is how many
       * new variable of the type will be declared. 
       * \param strLine -> the line of declaration read from the file */
      void declareVariable(std::string strLine);

      /*! Call a function.
       * \param var -> variable that will receive the result of the function
       *               (can be NULL, if no variable will receive, or with the
       *                function is without return) 
       * \param strLine -> the line that contains the function call.
       * \param functionName -> the name of the function to call
       * \param pos -> the position on the strLine after the function name */
      void callFunction(iaVariable* var, std::string strLine,
                        std::string functionName, unsigned int &pos);

      /*! Get next parameter of the function
       * \param token -> token got
       * \param strLine -> string line to got parameter
       * \param paramType -> the type of the parameter to got 
       * \param pos -> position of after the rpevious parameter got
       * \return -> the iaVariable with the parameter got, or NULL if an
       *            error occurs. */
      iaVariable* getParameter(std::string& token, std::string strLine, 
                               std::string paramType, unsigned int& pos);
      int getParameteri(std::string& token, std::string strLine, 
                        unsigned int& pos);
      float getParameterf(std::string& token, std::string strLine,
                          unsigned int& pos);
      character* getParameterc(std::string& token, std::string strLine,
                               unsigned int& pos);
      object* getParametero(std::string& token, std::string strLine, 
                            unsigned int& pos);
      std::string getParameters(std::string& token, std::string strLine, 
                                unsigned int& pos);
      diceThing* getParameterd(std::string& token, std::string strLine,
                               unsigned int& pos);
      weapon* getParameterw(std::string& token, std::string strLine,
                            unsigned int& pos);
  
      /*! Assign the value to the variable
       * \param var -> pointer to a variable
       * \param value -> pointer to the value to assign
       * \param type -> the type of parameter AND value */
      void assignValue(iaVariable* var, void* value, std::string type);

      /*! Evaluate a expression
       * \param var -> variable that will contain the expression result
       * \param strLine -> the line that contains the expression
       * \param assignExpression -> true if is an assign expression,
       *                            false otherwise (false usually if
       *                            only a parameter or conditional one) */
      void evaluateExpression(iaVariable* var, std::string strLine,
                              bool assignExpression);


      /*! change from the postFix string the token string and
       * num tokens after the token string, for the newToken string
       * \param postFix -> the postFix string
       * \param token -> the token string to remove
       * \param num -> the number of tokens to remove after the token 
       * \param newToken -> the token that will take place of removed ones. */
      std::string changeFrom(std::string postFix, std::string token, int num, 
                             std::string newToken);
};

#endif

