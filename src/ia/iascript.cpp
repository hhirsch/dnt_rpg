
#include "iascript.h"
#include "../engine/dccnit.h"

/***********************************************************************
 *                           Constructor                               *
 ***********************************************************************/
iaScript::iaScript(string scriptFile, void* usedEngine)
{
   fileName = scriptFile;
   file.open(scriptFile.c_str(), ios::in | ios::binary);
   if(!file)
   {
      cerr << "Failed to open the script: " << scriptFile << endl;
   }
   objectOwner = NULL;
   characterOwner = NULL;
   actualMap = NULL;
   pendingAction = NULL;

   symbols = new iaSymbolsTable();
   jumpStack = new iaStack();

   actualLine = 0;

   actualEngine = usedEngine;

   context = "";
}


/***********************************************************************
 *                            Destructor                               *
 ***********************************************************************/
iaScript::~iaScript()
{
   if(file)
   {
      file.close();
   }
   objectOwner = NULL;
   characterOwner = NULL;
   actualMap = NULL;
   pendingAction = NULL;
   delete(symbols);
   delete(jumpStack);
}

/***********************************************************************
 *                        defineObjectOwner                            *
 ***********************************************************************/
void iaScript::defineObjectOwner(object* owner)
{
   characterOwner = NULL;
   objectOwner = owner;
}

/***********************************************************************
 *                      defineCharacterOwner                           *
 ***********************************************************************/
void iaScript::defineCharacterOwner(character* owner)
{
   characterOwner = owner;
   objectOwner = NULL;
}

/***********************************************************************
 *                            defineMap                                *
 ***********************************************************************/
void iaScript::defineMap(Map* acMap)
{
   actualMap = acMap;
}

/***********************************************************************
 *                              run                                    *
 ***********************************************************************/
action* iaScript::run()
{
   bool done = false; /* Will be done when finish the file, 
                         or at pending action */
   string strBuffer, token;
   unsigned int pos = 0;

   streampos lastPos;

   iaVariable* iv = NULL;

   if(file)
   {
      while(!done)
      {
         if( (pendingAction) && (pendingAction->done) )
         {
            delete(pendingAction);
            pendingAction = NULL;
         }
         else
         {
            /* Read Line */
            lastPos = file.tellg();
            getline(file, strBuffer);
            actualLine++;
            pos = 0;
            if(file.eof())
            {
               /* is end of file, so the script is done */
               done = true;
            }
            else
            {
               /* get the first token */
               token = nextToken(strBuffer, pos);

               iv = symbols->getSymbol(token);

               if(token[0] == IA_COMMENT_LINE)
               {
                  //ignore the line, since it is commented
               }
               else if(token == IA_SETENCE_SCRIPT)
               {
                  /* It's the script main definition */
                  context = IA_SETENCE_SCRIPT;
                  iaJumpPos* jmp = new iaJumpPos;
                  jmp->begin = lastPos;
                  jmp->end = 0; //Unknow.
                  jmp->command = IA_SETENCE_SCRIPT;
                  jumpStack->push(jmp);
               }
               else if(!context.empty())
               {
                  if(isType(token))
                  {
                     /* If the first token is a type, then the line is
                      * a variable declaration. */
                     declareVariable(strBuffer);
                  }
                  else if(token == IA_SETENCE_IF)
                  {
                    /* It's an if */

                     //TODO
                  }
                  else if(token == IA_SETENCE_ELSE)
                  {
                     /* It's an else */
  
                     //TODO
                  }
                  else if(token == IA_SETENCE_FOR)
                  {
                     /* It's a for */
                     //TODO
                  }
                  else if(token == IA_SETENCE_END)
                  {
                     /* It's an end */
                     //TODO
                  }
                  else if(iv != NULL)
                  {
                     /* The token is a symbol on the table.
                      * So it must be an assign operation */
                      evaluateExpression(iv, strBuffer);
                  }
                  else if(isFunction(token))
                  {
                     /* The token is a function */
                     callFunction(NULL, strBuffer);
                  }
                  else if(!token.empty())
                  {
                     /* Unknow! */
                     cerr << "Unknow token: " << token << " at line " 
                          << actualLine << " of the script file: " 
                          << fileName << endl;
                  }
               }
               else if(!token.empty())
               {
                  /* No Context defined! */
                  cerr << "No context defined at line: " << actualLine 
                       << " of script file: " << fileName 
                       << " did you forget the script() declaration?" << endl;
               }
            }
         }
      }
   }
   return(NULL);
}

/***********************************************************************
 *                         declareVariable                             *
 ***********************************************************************/
void iaScript::declareVariable(string strLine)
{
   unsigned int pos = 0;
   string type = nextToken(strLine, pos);
   string name = nextToken(strLine, pos);
   cout << "strLine: " << strLine << endl;
   while(name != "")
   {
      cout << "Type: " << type << " name: " << name << endl;
      symbols->addSymbol(type, name);
      name = nextToken(strLine, pos);
   }
}

/***********************************************************************
 *                          callFunction                               *
 ***********************************************************************/
void iaScript::callFunction(iaVariable* var, string strLine)
{
   unsigned int pos = 0;

   string functionName = nextToken(strLine, pos);

   engine* eng = (engine*)actualEngine;

   if(functionName == IA_MOVE_TO_POSITION)
   {
      //TODO
   }
   else if(functionName == IA_MOVE_TO_CHARACTER)
   {
      //TODO
   }
   else if(functionName == IA_MOVE_TO_OBJECT)
   {
      //TODO
   }
   else if(functionName == IA_FEAT_TOTAL)
   {
      //TODO
   }
   else if(functionName == IA_FEAT_COST)
   {
      //TODO
   }
   else if(functionName == IA_FEAT_USE_AT_CHARACTER)
   {
      //TODO
   }
   else if(functionName == IA_FEAT_USE_AT_OBJECT)
   {
      //TODO
   }
   else if(functionName == IA_SKILL_POINTS)
   {
      //TODO
   }
   else if(functionName == IA_SKILL_MODIFIER)
   {
      //TODO
   }
   else if(functionName == IA_FIGHT_ENTER)
   {
      /* Enter the battle mode, with the character owner and
       * the PC's characters. */
      if(!var)
      {
         characterOwner->psychoState = PSYCHO_HOSTILE;
         eng->enterBattleMode(false);
      }
      else
      {
         cerr << "Error: " << IA_FIGHT_ENTER << " don't return any value!" 
              << " at " << strLine << " on script: " << fileName << endl;
      }
   }
   else if(functionName == IA_FIGHT_EXIT)
   {
      //TODO
      //something that will remove the hostile state of all battle characters.
      //eng->exitBattleMode();
   }
   else if(functionName == IA_CHARACTER_GET_PSYCHO)
   {
      //TODO
   }
   else if(functionName == IA_CHARACTER_SET_PSYCHO)
   {
      //TODO
   }
   else if(functionName == IA_SELF_OBJECT)
   {
      /* Define the variable as the object owner */
      if(var)
      {
         if(var->type == IA_TYPE_OBJECT)
         {
            var->value = (void*)objectOwner;
         }
         else
         {
            cerr << "Error: Expected object symbol, but got: " << var->type
                 << " at " << strLine << " on script: " << fileName << endl;
         }
      }
   }
   else if(functionName == IA_SELF_CHARACTER)
   {
      /* Define the variable as the character owner */
      if(var)
      {
         if(var->type == IA_TYPE_CHARACTER)
         {
            var->value = (void*)characterOwner;
         }
         else
         {
            cerr << "Error: Expected character symbol, but got: " << var->type
                 << " at " << strLine << " on script: " << fileName << endl;
         }
      }
   }
   else if(functionName == IA_ACTIVE_CHARACTER)
   {
      /* Define the variable as the character owner */
      if(var)
      {
         if(var->type == IA_TYPE_CHARACTER)
         {
            var->value = (void*)eng->PCs->getActiveCharacter();
         }
         else
         {
            cerr << "Error: Expected character symbol, but got: " << var->type
                 << " at " << strLine << " on script: " << fileName << endl;
         }
      }
   }
   else if(functionName == IA_ACTUAL_MAP)
   {
      /* Define the variable as the actualMap */
      if(var)
      {
         if(var->type == IA_TYPE_MAP)
         {
            var->value = (void*)actualMap;
         }
         else
         {
            cerr << "Error: Expected map symbol, but got: " << var->type 
                 << " at " << strLine << " on script: " << fileName << endl;
         }
      }
   }
   else if(functionName == IA_TRUE)
   {
      /* Define the variable as true */
      if(var)
      {
         if(var->type == IA_TYPE_BOOL)
         {
            bool* v = (bool*)var->value;
            *v = true;
         }
         else
         {
            cerr << "Error: Expected bool symbol, but got: " << var->type
                 << " at " << strLine << " on script: " << fileName << endl;
         }
      }
   }
   else if(functionName == IA_FALSE)
   {
      /* Define the variable as false */
      if(var)
      {
         if(var->type == IA_TYPE_BOOL)
         {
            bool* v = (bool*)var->value;
            *v = false;
         }
         else
         {
            cerr << "Error: Expected bool symbol, but got: " << var->type
                 << " at " << strLine << " on script: " << fileName << endl;
         }
      }
   }

}

/***********************************************************************
 *                       evaluateExpression                            *
 ***********************************************************************/
void iaScript::evaluateExpression(iaVariable* var, string strLine)
{
   unsigned int pos = 0;
   string token;
   string type; 

   /* get the variable */
   nextToken(strLine, pos);

   iaVariable* varStack[1024];
   int varPos = 0;

   iaVariable* var1 = NULL;
   iaVariable* var2 = NULL;

   /* get the assign token */
   token = nextToken(strLine, pos);
   if(token == IA_OPERATOR_ASSIGN)
   {
      /* Remove the variable and assign */
      strLine.erase(0, pos);
      /* Translate the string to postfix */
      string postFix = toPostFix(strLine);

      cout << strLine << endl;
      cout << postFix << endl;

      /* Evaluate the expression */
      pos = 0;
      token = nextToken(postFix, pos);
      while(!token.empty())
      {
         if(isOperator(token))
         {
            /* Apply the operator */
            if( (token == IA_OPERATOR_ADDITION) ||
                (token == IA_OPERATOR_SUBTRACTION) ||
                (token == IA_OPERATOR_MULTIPLICATION) ||
                (token == IA_OPERATOR_DIVISION) ||
                (token == IA_OPERATOR_MODULUS) )
            {
               /* Pop the two variables */
               if(varPos < 2)
               {
                  cerr << "Error: operator " << token << " needs two variables"
                       << endl;
               }
               else
               {
                  var1 = varStack[varPos-1];
                  var2 = varStack[varPos-2];
                  varPos -= 2;
                  /* Type must be integer or float */
                  if( ( (var1->type == IA_TYPE_FLOAT) ||
                        (var1->type == IA_TYPE_INT) ) &&
                      ( (var2->type == IA_TYPE_FLOAT) ||
                        (var2->type == IA_TYPE_INT) ) )
                  {
                     if( (var1->type == IA_TYPE_FLOAT) ||
                         (var2->type == IA_TYPE_FLOAT) )
                     {
                        type = IA_TYPE_FLOAT;
                     }
                     else
                     {
                        type = IA_TYPE_INT;
                     }
                     /* Alloc the result variable */
                     varStack[varPos] = new iaVariable(type,"result");
                     varStack[varPos]->receiveOperation(token, var1, var2);
                     cout << "result: " << varStack[varPos]->toString() << endl;
                     varPos++;

                     /* Free the memory */
                     delete(var1);
                     delete(var2);
                  }
                  else
                  {
                     cerr << "Error: Operator " << token << " is only for "
                          << IA_TYPE_INT << " or " << IA_TYPE_FLOAT << endl;
                  }
               }
            }

         }
         else
         {
            /* Put it at the stack */
            iaVariable* iv = symbols->getSymbol(token);
            if(iv != NULL)
            {
               cout << "Add " << token << " type " << iv->type << endl;
               varStack[varPos] = new iaVariable(iv->type, token);
               *(varStack[varPos]) = (*iv);
               varPos++;
            }
            else if(isFunction(token))
            {
               string ftype = functionType(token);
               if(ftype != IA_TYPE_VOID)
               {
                  //TODO
                  //varStack[varPos] = new iaVariable(iv->type, token);
                  //callFunction(varStack[varPos], strFunc);
                  //varPos++;
               }
               else
               {
                  cerr << "Error: void function used as operand at " << strLine
                       << " on script " << fileName << endl;
               }
            }
            else
            {
               /* Must be a value */
               if(isInteger(token))
               {
                  /* Create the integer value at the stack */
                  int ti;
                  sscanf(token.c_str(),"%d",&ti);
                  varStack[varPos] = new iaVariable(IA_TYPE_INT, token);
                  *(int*)varStack[varPos]->value = ti;
                  varPos++;
               }
               else if(isFloat(token))
               {
                  /* Create the float value at the stack */
                  float tf;
                  sscanf(token.c_str(),"%f",&tf);
                  varStack[varPos] = new iaVariable(IA_TYPE_FLOAT, token);
                  *(float*)varStack[varPos]->value = tf;
                  varPos++;
               }
               else
               {
                  cerr << "Error: unknow token " << token << " at " << strLine
                       << " on script " << fileName << " line " << actualLine
                       << endl;
               }
            }
         }
         token = nextToken(postFix, pos);
      }

      /* Take the result to the correct place */
      if(varPos != 1)
      {
         cerr << "Error: The evaluation stack isn't with only the result!" 
              << endl << "Size: " << varPos << " line " << actualLine 
              << " script " << fileName;
      }
      else if(var != NULL)
      {
         *(var) = *(varStack[0]);
         cout << "Final Result: " << var->toString() << endl;
         delete(varStack[0]);
      }

   }
   else
   {
      /* Unknow operation! */
      cerr << "Error: Expected assign operator at " << strLine << " on script "
           << fileName << endl;
   }

}

