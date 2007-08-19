#ifndef _iavariable_h
#define _iavariable_h


/* Types */
#define IA_VOID "void"
#define IA_BOOL "bool"
#define IA_INT "int" 
#define IA_FLOAT "float"
#define IA_STRING "string"
#define IA_CHARACTER "character"
#define IA_OBJECT "object"
#define IA_FEAT "feat"
#define IA_SKILL "skill"


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


#endif

