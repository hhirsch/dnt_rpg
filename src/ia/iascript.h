#ifndef _iascript_h
#define _iascript_h

#include "iafuncs.h"
#include "iavariable.h"
#include "iastack.h"

#include "../engine/character.h"
#include "../classes/object.h"
#include "../classes/feats.h"
#include "../classes/skills.h"
#include "../map/map.h"

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

/*! FIXME, not here, temporaly */
class action
{
   public:
      bool done;
};

/*! The ia Script is the script that controls the behaviour
 * of characters, objects and events on DNT. */
class iaScript
{
   public:
      /*! Constructor
       * \param scriptFile -> fileName of the script
       * \param usedEngine -> the pointer to the engine used */
      iaScript(string scriptFile, void* usedEngine);
      /*! Destructor */
      ~iaScript();
   
      /*! Define the owner of the script as a object
       * \param owner -> pointer to the object */
      void defineObjectOwner(object* owner);
      /*! Define the owner of the script as a Character
       * \param owner -> pointer to the character */
      void defineCharacterOwner(character* owner);

      /*! Define the actual openned Map
       * \param acMap -> pointer to the opened map */
      void defineMap(Map* acMap);

      /*! run the Script to the point of a more than one cicle action,
       * that will be marked as pending or at the end of the script.
       * if the scripts ends, a NULL is retorned, and the script can
       * be removed from the memory.
       * \return action pointer of a new pending action */
      action* run();

   protected:
      ifstream file;             /**< The script's file */
      string fileName;           /**< FileName of the script */
      object* objectOwner;       /**< Object Owner */
      character* characterOwner; /**< Character Owner */

      void* actualEngine;        /**< The actual Engine */

      Map* actualMap;            /**< Actual Opened Map */

      int actualLine;            /**< The actual Line */

      string context;            /**< The context */

      action* pendingAction;     /**< Pending Action on the script. Only
                                      will advance on script when receive
                                      that this actions was done. */

      iaSymbolsTable* symbols;   /**< The script's table of symbols */
      iaStack* jumpStack;        /**< The jump stack (if, else, for) */

      /*! Declare a variable at the symbol's table, with the declaration
       * line, if it is with right syntax. The syntax for declaration is
       * type name1 name2 name3 
       * where type is the type of the variable and name is how many
       * new variable of the type will be declared. 
       * \param strLine -> the line of declaration read from the file */
      void declareVariable(string strLine);

      /*! Call a function.
       * \param var -> variable that will receive the result of the function
       *               (can be NULL, if no variable will receive, or with the
       *                function is without return) 
       * \param strLine -> the line that contains the function call.
       * \param functionName -> the name of the function to call
       * \param acPos -> the position on the strLine after the function name */
      void callFunction(iaVariable* var, string strLine,
                        string functionName, unsigned int &pos);

      /*! Evaluate a expression
       * \param var -> variable that will contain the expression result
       * \param strLine -> the line that contains the expression
       * \param assignExpression -> true if is an assign expression,
       *                            false otherwise (false usually if
       *                            only a parameter or conditional one) */
      void evaluateExpression(iaVariable* var, string strLine,
                              bool assignExpression);
};

#endif

