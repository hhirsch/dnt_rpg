
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
action* iaScript::run(int maxLines)
{
   bool done = false; /* Will be done when finish the file, 
                         or at pending action */
   string strBuffer, token;
   unsigned int pos = 0;

   int lines = 0;

   streampos lastPos;

   iaVariable* iv = NULL;

   if(file)
   {
      while(!done)
      {
         if( (pendingAction) && (!pendingAction->isRunning()) )
         {
            //TODO go to where stops at the action
            delete(pendingAction);
            pendingAction = NULL;
         }
         else if(pendingAction)
         {
            /* The pending action isn't finished, so do nothing */
            done = true;
         }
         else
         {
            /* Read Line */
            lastPos = file.tellg();
            getline(file, strBuffer);
            actualLine++;
            lines++;
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
                     callFunction(NULL, strBuffer, token, pos);
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
void iaScript::callFunction(iaVariable* var, string strLine, 
                            string functionName, unsigned int& pos)
{
   string token = "";
   iaVariable* iv = NULL;
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

   /* IA_CHARACTER_GET_PSYCHO */
   else if(functionName == IA_CHARACTER_GET_PSYCHO)
   {
      /* Syntax int getPsycho(character c)  */
      token = nextToken(strLine, pos);
      iv = symbols->getSymbol(token);
      if(isFunction(token))
      {
         /* Get the function value */
         iv = new iaVariable(IA_TYPE_CHARACTER,"charac");
         callFunction(iv,strLine,token,pos);
      }

      if( (!iv) && (!isFunction(token)) )
      {
         cerr << "Error: Unknow parameter " << token << " to function "
              << IA_CHARACTER_GET_PSYCHO << " must be a " 
              << IA_TYPE_CHARACTER << " type"
              << " at file " << fileName << " line " << actualLine << endl;
      }
      else if(iv->type == IA_TYPE_CHARACTER)
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
      }
      else
      {
         cerr << "Error: Type for parameter of function " 
              << IA_CHARACTER_GET_PSYCHO << " must be " << IA_TYPE_CHARACTER
              << " at script: " << fileName << endl;
      }

      /* Free the token */
      if(isFunction(token))
      {
         delete(iv);
      }
   }

   /* IA_CHARACTER_SET_PSYCHO */
   else if(functionName == IA_CHARACTER_SET_PSYCHO)
   {
      //TODO
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
                     varStack[varPos]->receiveOperation(token, var2, var1);
                     cout << "result: " << varStack[varPos]->toString() << endl;
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
                  cout << "result: " << varStack[varPos]->toString() << endl;
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
                  cout << "result: " << varStack[varPos]->toString() << endl;
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
                  cout << "result: " << varStack[varPos]->toString() << endl;
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

