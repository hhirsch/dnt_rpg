
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
 *                            finished                                 *
 ***********************************************************************/
bool iaScript::finished()
{
   return(file.eof());
}

/***********************************************************************
 *                           changeFrom                                *
 ***********************************************************************/
string iaScript::changeFrom(string postFix, string token, int num,
                            string newToken)
{
   unsigned int pos = 0;
   string result = "";

   /* Get all previous tokens */
   string actual = nextToken(postFix, pos);
   while( (actual != token) && (!actual.empty()) )
   {
      result += " " + actual;
      actual = nextToken(postFix, pos);
   }

   /* Ignore all desirable tokens */
   int i;
   for(i = 0; i < num; i++)
   {
      actual = nextToken(postFix, pos);
   }

   /* Change to the newToken */
   result += " " + newToken;

   /* Copy all remaining tokens  */
   while(actual != "")
   {
      actual = nextToken(postFix, pos);
      result += " " + actual;
   }

   return(result);
}

/***********************************************************************
 *                              run                                    *
 ***********************************************************************/
void iaScript::run(int maxLines)
{
   bool done = false; /* Will be done when finish the file, 
                         or at pending action */
   string strBuffer, token;
   unsigned int pos = 0;

   int lines = 0;

   bool interpret = false;

   iaVariable* iv = NULL;

   if(file)
   {
      while(!done)
      {
         if( (pendingAction) && (!pendingAction->isRunning()) )
         {
            /* Go to where stops at the action */
            strBuffer = pendingAction->getScriptLine();
            interpret = true;

            /* Remove the action from the controller */
            engine* eng = (engine*)actualEngine;
            eng->actionControl->removeAction(pendingAction);
            pendingAction = NULL;
         }
         else if(pendingAction)
         {
            /* The pending action isn't finished, so do nothing */
            done = true;
            interpret = false;
         }
         else
         {
            /* Read Line */
            lastPos = file.tellg();
            getline(file, strBuffer);
            actualLine++;
            lines++;
            interpret = true;
         }

         if(interpret)
         {
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
                  lines--;
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
                    iaVariable* ifCond = new iaVariable(IA_TYPE_BOOL,"ifcond");
                    evaluateExpression(ifCond, strBuffer, false);

                    /* If false, need to get to an else or end.
                     * If true, just put it at the stack and continue */
                    if( (*(bool*)ifCond->value) == false)
                    {
                      while( (token != IA_SETENCE_END) )
                      {
                         lastPos = file.tellg();
                         getline(file, strBuffer);
                         actualLine++;
                         pos = 0;
                         if(file.eof())
                         {
                           /* is end of file, so the script is done */
                           done = true;
                           cerr << "if without end or else on script " << 
                                   fileName << endl;
                           token = "end";
                         }
                         else
                         {
                           token = nextToken(strBuffer, pos);
                           if(token == IA_SETENCE_ELSE)
                           {
                              token = nextToken(strBuffer, pos);
                              if(token.empty())
                              {
                                 /* It's an pure else thing, so run it! */
                                 token = "end";
                                 /* Put the else at the stack */
                                 iaJumpPos* jmp = new iaJumpPos;
                                 jmp->begin = lastPos;
                                 jmp->end = 0; //Unknow.
                                 jmp->command = IA_SETENCE_ELSE;
                                 jumpStack->push(jmp);
                              }
                              else if(token == IA_SETENCE_IF)
                              {
                                 evaluateExpression(ifCond, strBuffer, false);
                                 if( (*(bool*)ifCond->value) == true)
                                 {
                                    /* It's an valid else if, so run it! */
                                    token = "end";
                                    /* Put the else if at the stack */
                                    iaJumpPos* jmp = new iaJumpPos;
                                    jmp->begin = lastPos;
                                    jmp->end = 0; //Unknow.
                                    jmp->command = IA_SETENCE_IF;
                                    jumpStack->push(jmp);
                                 }
                              }
                              else
                              {
                                 cerr << "Error: Unkown statment " << strBuffer
                                      << " at line " << actualLine << 
                                         " of script " << fileName << endl;
                              }
                           }
                         }
                      }
                    }
                    else
                    {
                       /* Put the if at the stack */
                       iaJumpPos* jmp = new iaJumpPos;
                       jmp->begin = lastPos;
                       jmp->end = 0; //Unknow.
                       jmp->command = IA_SETENCE_IF;
                       jumpStack->push(jmp);
                    }
                    delete(ifCond);
                  }
                  else if(token == IA_SETENCE_ELSE)
                  {
                     /* It's an else */
                     iaJumpPos* jmp = jumpStack->pop();
                     if( (jmp) && (jmp->command == IA_SETENCE_IF) )
                     {
                        //ignore the else and all elses after it, to an end
                        while( (token != IA_SETENCE_END) )
                        {
                           lastPos = file.tellg();
                           getline(file, strBuffer);
                           actualLine++;
                           pos = 0;
                           if(file.eof())
                           {
                              /* is end of file, so the script is done */
                              done = true;
                              cerr << "if without end or else on script " << 
                                      fileName << endl;
                              token = "end";
                           }
                           else
                           {
                              token = nextToken(strBuffer, pos);
                           }
                        }
                        /* free the jump */
                        delete(jmp);
                     }
                     else
                     {
                        cerr << "Got else, without ifs, at line " << actualLine
                             << " of script " << fileName << endl;
                        if(jmp)
                        {
                           /* Push Back the jump  */
                           jumpStack->push(jmp);
                        }
                     }
                  }
                  else if(token == IA_SETENCE_WHILE)
                  {
                     /* It's a for or an while */
                     iaVariable* iv = new iaVariable(IA_TYPE_BOOL, "result");
                     evaluateExpression(iv, strBuffer, false);
                     if( (*(bool*)iv->value) == true )
                     {
                        /* Put the if at the stack */
                       iaJumpPos* jmp = new iaJumpPos;
                       jmp->begin = lastPos;
                       jmp->end = 0; //Unknow.
                       jmp->command = IA_SETENCE_WHILE;
                       jumpStack->push(jmp);
                     }
                     else
                     {
                        /* Get to the end of the while */
                        while( (token != IA_SETENCE_END) )
                        {
                           lastPos = file.tellg();
                           getline(file, strBuffer);
                           actualLine++;
                           pos = 0;
                           if(file.eof())
                           {
                              /* is end of file, so the script is done */
                              done = true;
                              cerr << "while without end on script "
                                   << fileName << endl;
                              token = "end";
                           }
                           else
                           {
                              token = nextToken(strBuffer, pos);
                           }
                        }
                     }
                     delete(iv);
                  }
                  /* Treat the END statment */
                  else if(token == IA_SETENCE_END)
                  {
                     /* It's an end */
                     iaJumpPos* jmp = jumpStack->pop();
                     if(!jmp)
                     {
                        cerr << "Got an end without any initial statement at "
                             << fileName << " line " << actualLine << endl;
                     }
                     else
                     {
                        if(jmp->command == IA_SETENCE_WHILE)
                        {
                           file.seekg(jmp->begin);
                        }
                        delete(jmp);
                     }
                  }
                  else if(iv != NULL)
                  {
                     /* The token is a symbol on the table.
                      * So it must be an assign operation */
                      evaluateExpression(iv, strBuffer, true);
                  }
                  else if(isFunction(token))
                  {
                     /* The token is a function */
                     string postFix = toPostFix(strBuffer);
                     /* Get the position after the function token */
                     unsigned int p = 0;
                     nextToken(postFix,p);
                     callFunction(NULL, postFix, token, p);
                  }
                  else if(!token.empty())
                  {
                     /* Unknow! */
                     cerr << "Unknow token: " << token << " at line " 
                          << actualLine << " of the script file: " 
                          << fileName << endl;
                  }
                  else
                  {
                     //ignore line
                     lines--;
                  }
               }
               else if(!token.empty())
               {
                  /* No Context defined! */
                  cerr << "No context defined at line: " << actualLine 
                       << " of script file: " << fileName 
                       << " did you forget the script() declaration?" << endl;
               }
               else
               {
                  //ignore line
                  lines--;
               }
            }
         }
         if( (maxLines != 0) && (lines >= maxLines) )
         {
            done = true;
         }

         if(!pendingAction)
         {
            /* Remove all temporary symbols used */
            symbols->removeTempSymbols();
         }
      }
   }
   return;
}

/***********************************************************************
 *                         declareVariable                             *
 ***********************************************************************/
void iaScript::declareVariable(string strLine)
{
   unsigned int pos = 0;
   string type = nextToken(strLine, pos);
   string name = nextToken(strLine, pos);
   while(name != "")
   {
      symbols->addSymbol(type, name);
      name = nextToken(strLine, pos);
   }
}

/***********************************************************************
 *                          getParameter                               *
 ***********************************************************************/
iaVariable* iaScript::getParameter(string& token, string strLine, 
                                   string paramType, unsigned int& pos)
{
   token = nextToken(strLine, pos);
   iaVariable* iv = symbols->getSymbol(token);

   if(isFunction(token))
   {
      /* Get the function value */
      iv = new iaVariable(paramType,"param");
      callFunction(iv,strLine,token,pos);
   }

   /* Verify type compatibility */
   if( ( (!iv) && (!isFunction(token)) ) ||
       ( (iv) && 
         ( (iv->type != paramType) && 
           (! (paramType == IA_TYPE_FLOAT) &&
              (iv->type == IA_TYPE_INT) 
           )
         ) 
       )
     )
   {
      cerr << "Error: Unknow parameter " << token << " must be a " 
           << paramType << " type"
           << " at file " << fileName << " line " << actualLine << endl;
      return(NULL);
   }
   return(iv);
}

/***********************************************************************
 *                          callFunction                               *
 ***********************************************************************/
void iaScript::callFunction(iaVariable* var, string strLine, 
                            string functionName, unsigned int& pos)
{
   string token = "";
   iaVariable* iv = NULL;
   engine* eng = (engine*)actualEngine;

   string varName = "";

   /* Move to Position */
   if(functionName == IA_MOVE_TO_POSITION)
   {
      /* Syntax bool moveToPosition(character* char, int x, int z) */
      varName = symbols->addTempSymbol(IA_TYPE_BOOL);

      string line = changeFrom(strLine, functionName, 2, varName);
      character* dude = NULL;
      GLfloat X = 0;
      GLfloat Z = 0;

      /* Get the character */
      iv = getParameter(token, strLine, IA_TYPE_CHARACTER, pos);
      if(iv != NULL)
      {
         dude = (character*)iv->value;
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      /* Get the X */
      iv = getParameter(token, strLine, IA_TYPE_FLOAT, pos);
      if(iv != NULL)
      {
         if(iv->type == IA_TYPE_FLOAT)
         {
            X = *(float*)iv->value;
         }
         else
         {
            X = *(int*)iv->value;
         }
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      /* Get the Z */
      iv = getParameter(token, strLine, IA_TYPE_FLOAT, pos);
      if(iv != NULL)
      {
         if(iv->type == IA_TYPE_FLOAT)
         {
            Z = *(float*)iv->value;
         }
         else
         {
            Z = *(int*)iv->value;
         }
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      if(dude)
      {
         dude->pathFind.defineMap(actualMap);
         pendingAction = eng->actionControl->addAction(line, ACT_MOVE, 
                                                       dude, X, Z);
      }
   }


   /* Move to Character */
   else if(functionName == IA_MOVE_TO_CHARACTER)
   {
      //TODO
   }


   /* Move to Object */
   else if(functionName == IA_MOVE_TO_OBJECT)
   {
      //TODO
   }


   /* Feat Total */
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


   /* fight enter */
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


   /* fight_exit */
   else if(functionName == IA_FIGHT_EXIT)
   {
      //TODO
      //something that will remove the hostile state of all battle characters.
      //eng->exitBattleMode();
   }


   /* IA_CHARACTER_GET_PSYCHO */
   else if(functionName == IA_CHARACTER_GET_PSYCHO)
   {
      /* Syntax int getPsycho(character c)  */
      iv = getParameter(token, strLine, IA_TYPE_CHARACTER, pos);
      if(iv != NULL)
      {
         character* c = (character*)iv->value;
         if(c != NULL)
         {
            if( (var) && (var->value == IA_TYPE_INT))
            {
               (*(int*)var->value) = c->psychoState;
            }
            else if(var)
            {
               cerr << "Error: Expected " << IA_TYPE_INT
                    << " variable, but got " << var->type 
                    << " at line " << actualLine << " file "
                    << fileName << endl;
            }
         }
         else
         {
            cerr << "Error: Tried to access a NULL character at line " 
                 << actualLine << " of the script: " << fileName << endl;
         }
         if(isFunction(token))
         {
            delete(iv);
         }
      }
   }



   /* IA_CHARACTER_SET_PSYCHO */
   else if(functionName == IA_CHARACTER_SET_PSYCHO)
   {
      /* Syntax void setPsycho(character c, int psycho)  */
      character* dude = NULL;
      int psy = 0;

      /* Get character */
      iv = getParameter(token, strLine, IA_TYPE_CHARACTER, pos);
      if(iv != NULL)
      {
         dude = (character*)iv->value;
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      /* Get psycho */
      iv = getParameter(token, strLine, IA_TYPE_INT, pos);
      if(iv != NULL)
      {
         psy = (*(int*)iv->value);
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      /* Set the things */
      if(dude != NULL)
      {
         dude->psychoState = psy;
      }
      else
      {
         cerr << "Error: Tried to access a NULL character at line " 
                 << actualLine << " of the script: " << fileName << endl;
      }
   }


   /* IA_SELF_OBJECT */
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

   /* IA_SELF_CHARACTER */
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


   /* Active Character */
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


   /* Actual Map */
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


   /* true */
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


   /* false */
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
void iaScript::evaluateExpression(iaVariable* var, string strLine,
                                  bool assignExpression)
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
   if(assignExpression)
   {
      token = nextToken(strLine, pos);
   }

   /* If var is null, the expression is not an assign operation,
    * so just evaluate it. */
   if( (!assignExpression) || (token == IA_OPERATOR_ASSIGN) )
   {
      /* Remove the variable and assign */
      strLine.erase(0, pos);
      /* Translate the string to postfix */
      string postFix = toPostFix(strLine);

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
                     varStack[varPos]->receiveOperation(token, var2, var1);
                     varPos++;
                  }
                  else
                  {
                     cerr << "Error: Operator " << token << " is only for "
                          << IA_TYPE_INT << " or " << IA_TYPE_FLOAT << endl;
                  }
                  /* Free the memory */
                  delete(var1);
                  delete(var2);
               }
            }
            /* Comparations operations */
            else if( (token == IA_OPERATOR_EQUAL) ||
                     (token == IA_OPERATOR_NOT_EQUAL) ||
                     (token == IA_OPERATOR_LESSER) || 
                     (token == IA_OPERATOR_GREATER) ||
                     (token == IA_OPERATOR_GEQUAL) ||
                     (token == IA_OPERATOR_LEQUAL) )
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
                  /* Alloc the result variable */
                  varStack[varPos] = new iaVariable(IA_TYPE_BOOL,"result");
                  varStack[varPos]->receiveOperation(token, var2, var1);
                  varPos++;
                  /* Free the memory */
                  delete(var1);
                  delete(var2);
               }
            }
            /* and && or || operations */
            else if( (token == IA_OPERATOR_AND) ||
                     (token == IA_OPERATOR_OR) )
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
                  /* Alloc the result variable */
                  varStack[varPos] = new iaVariable(IA_TYPE_BOOL,"result");
                  varStack[varPos]->receiveOperation(token, var2, var1);
                  varPos++;
                  /* Free the memory */
                  delete(var1);
                  delete(var2);
               }
            }
            /* ! not operations */
            else if((token == IA_OPERATOR_NOT))
            {
               /* Pop the variable */
               if(varPos < 1)
               {
                  cerr << "Error: operator " << token << " needs two variables"
                       << endl;
               }
               else
               {
                  var1 = varStack[varPos-1];
                  varPos --;
                  /* Alloc the result variable */
                  varStack[varPos] = new iaVariable(IA_TYPE_BOOL,"result");
                  varStack[varPos]->receiveOperation(token, var1, NULL);
                  varPos++;
                  /* Free the memory */
                  delete(var1);
               }
            }
            else
            {
               cerr << "Unimplemented operator: " << token << " at " 
                    << fileName << " line " << actualLine << endl;
            }

         }
         else
         {
            /* Put it at the stack */
            iaVariable* iv = symbols->getSymbol(token);
            if(iv != NULL)
            {
               varStack[varPos] = new iaVariable(iv->type, token);
               *(varStack[varPos]) = (*iv);
               varPos++;
            }
            else if(isFunction(token))
            {
               string ftype = functionType(token);
               if(ftype != IA_TYPE_VOID)
               {
                  varStack[varPos] = new iaVariable(ftype, token);
                  callFunction(varStack[varPos], strLine, token, pos);
                  varPos++;
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
              << " script " << fileName << endl;
         /* Delete the stack, to avoid leaks */
         varPos--;
         while(varPos >= 0)
         {
            delete(varStack[varPos]);
            varPos--;
         }
      }
      else if(var != NULL)
      {
         *(var) = *(varStack[0]);
         delete(varStack[0]);
      }

   }
   /* If not is temporary, we expected an assign (on temps, no assign is
    * nedded, since they are functions results). */
   else if(!symbols->isTemp(var))
   {
      /* Unknow operation! */
      cerr << "Error: Expected assign operator at " << strLine << " on script "
           << fileName << endl;
   }

}

