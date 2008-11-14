
#include "iascript.h"
#include "../engine/dccnit.h"
#include "../etc/dirs.h"

/***********************************************************************
 *                           Constructor                               *
 ***********************************************************************/
iaScript::iaScript(string scriptFile, void* usedEngine)
{
   fileName = scriptFile;

   init();

   objectOwner = NULL;
   characterOwner = NULL;
   actualMap = NULL;
   npcs = NULL;
   pendAction = NULL;
   actualEngine = usedEngine;
   type = IASCRIPT_TYPE_DEFAULT;
}

/***********************************************************************
 *                               init                                  *
 ***********************************************************************/
void iaScript::init()
{
   dirs dir;
   file.open(dir.getRealFile(fileName).c_str(), 
             ios::in | ios::binary);
   if(!file)
   {
      cerr << "Failed to open the script: "
           << dir.getRealFile(fileName) << endl;
   }

   symbols = new iaSymbolsTable();
   jumpStack = new iaStack();

   context = "";
   actualLine = 0;
}

/***********************************************************************
 *                             restart                                 *
 ***********************************************************************/
void iaScript::restart()
{
   actualLine = 0;
   close();
   init();
}


/***********************************************************************
 *                            Destructor                               *
 ***********************************************************************/
iaScript::~iaScript()
{
   close(); 
   objectOwner = NULL;
   characterOwner = NULL;
   actualMap = NULL;
   npcs = NULL;
}

/***********************************************************************
 *                              close                                  *
 ***********************************************************************/
void iaScript::close()
{
   if(pendAction)
   {
      engine* eng = (engine*)actualEngine;
      eng->actionControl->removeAction(pendAction);
      pendAction = NULL;
   }
   if(file.is_open())
   {
      file.close();
   }   
   if(symbols)
   {
      delete(symbols);
      symbols = NULL;
   }
   if(jumpStack)
   {
      delete(jumpStack);
      jumpStack = NULL;
   }
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
void iaScript::defineMap(Map* acMap, characterList* NPCs)
{
   actualMap = acMap;
   npcs = NPCs;
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
   string postFix;
   unsigned int pos = 0;
   unsigned int p = 0;

   int numBegins = 0;
   int lines = 0;

   bool interpret = false;

   iaVariable* iv = NULL;

   if(file)
   {
      while(!done)
      {
         if( (pendAction) && (!pendAction->isRunning()) )
         {
            /* Go to where stops at the action */
            strBuffer = pendAction->getScriptLine();
            interpret = true;

            //printf("Action Type ""%d"" ended: %s\n", 
            //       pendAction->getType(), strBuffer.c_str());

            /* Remove the action from the controller */
            engine* eng = (engine*)actualEngine;
            eng->actionControl->removeAction(pendAction);
            pendAction = NULL;
         }
         else if(pendAction)
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
               //printf("Interpreting: %s\n", strBuffer.c_str());

               /* get the first token */
               token = nextToken(strBuffer, pos);

               iv = symbols->getSymbol(token);

               if(token[0] == IA_COMMENT_LINE)
               {
                  /* ignore the line (it is commented) */
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
                  jmp->lineNumber = actualLine;
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
                      numBegins = 1;
                      while( (numBegins > 0) )
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
                           token = IA_SETENCE_END;
                         }
                         else
                         {
                           token = nextToken(strBuffer, pos);

                           /* The numBegins is to avoid ENDs not related to the
                            * IF itself */
                           if( (token == IA_SETENCE_END) || 
                               ((token == IA_SETENCE_ELSE) && (numBegins != 1)))
                           {
                              numBegins--;
                           }
                           else if( (token == IA_SETENCE_IF) ||
                                    (token == IA_SETENCE_WHILE) )
                           {
                              numBegins++;
                           }
                           else if( (token == IA_SETENCE_ELSE) &&
                                    (numBegins == 1) )
                           {
                              numBegins = 0; // To exit the while
                              /* It's an else for its if! */
                              token = nextToken(strBuffer, pos);
                              if(token.empty())
                              {
                                 /* It's an pure else thing, so run it! */
                                 token = IA_SETENCE_END;
                                 /* Put the else at the stack */
                                 iaJumpPos* jmp = new iaJumpPos;
                                 jmp->begin = lastPos;
                                 jmp->end = 0; //Unknow.
                                 jmp->command = IA_SETENCE_ELSE;
                                 jmp->lineNumber = actualLine;
                                 jumpStack->push(jmp);
                              }
                              else if(token == IA_SETENCE_IF)
                              {
                                 /* It's an else if */
                                 evaluateExpression(ifCond, strBuffer, false);
                                 if( (*(bool*)ifCond->value) == true)
                                 {
                                    /* It's an valid else if, so run it! */
                                    token = IA_SETENCE_END;
                                    /* Put the else if at the stack */
                                    iaJumpPos* jmp = new iaJumpPos;
                                    jmp->begin = lastPos;
                                    jmp->end = 0; //Unknow.
                                    jmp->command = IA_SETENCE_IF;
                                    jmp->lineNumber = actualLine;
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
                       jmp->lineNumber = actualLine;
                       jumpStack->push(jmp);
                    }
                    delete(ifCond);
                  }
                  else if(token == IA_SETENCE_ELSE)
                  {
                     /* It's an else (an else at this state means
                      * just to be ignored: it's treated only when the 'if'
                      * condition fails) */
                     iaJumpPos* jmp = jumpStack->pop();
                     if( (jmp) && (jmp->command == IA_SETENCE_IF) )
                     {
                        /* Search for the else end */
                        numBegins = 1; // The else is a begin block
                        while(numBegins > 0)
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
                              token = IA_SETENCE_END;
                           }
                           else
                           {
                              token = nextToken(strBuffer, pos);
                              /* The numBegins is to avoid ENDs not related 
                               * to the ELSE itself */
                              if(token == IA_SETENCE_END)
                              {
                                 numBegins--;
                              }
                              else if(token == IA_SETENCE_ELSE)
                              {
                                 if(numBegins == 1)
                                 {
                                    /* It's an else to the current one, so
                                     * is the end of block! */
                                    numBegins = 0;
                                 }
                                 /* Otherwise, It's an else for another if, so
                                  * it's a block open. But it is also a block
                                  * close, for the if, so no numBegins change
                                  * here! (trickery, no? =^P) */
                              }
                              else if( (token == IA_SETENCE_IF) ||
                                    (token == IA_SETENCE_WHILE) )
                              {
                                 numBegins++;
                              }
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

                           jumpStack->print();
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
                       jmp->lineNumber = actualLine;
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
                              token = IA_SETENCE_END;
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
                           /* Redefine current line (to -1, cause will inc) */
                           actualLine = jmp->lineNumber - 1;
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
                     postFix = toPostFix(strBuffer);
                     /* Get the position after the function token */
                     p = 0;
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

         if( (!pendAction) && (symbols) )
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
           (! ( (paramType == IA_TYPE_FLOAT) &&
                (iv->type == IA_TYPE_INT) )
           )
         ) 
       )
     )
   {
      cerr << "Error: Unknow parameter " << token << " must be a " 
           << paramType << " variable type"
           << " at file " << fileName << " line " << actualLine << endl;
      return(NULL);
   }

   return(iv);
}

/***********************************************************************
 *                           assignValue                               *
 ***********************************************************************/
void iaScript::assignValue(iaVariable* var, void* value, string type)
{
   if( (var) && (var->type == type))
   {
      if(type == IA_TYPE_INT)
      {
         (*(int*)var->value) = *(int*)value;
      }
      else if(type == IA_TYPE_BOOL)
      {
         (*(bool*)var->value) = *(bool*)value;
      }
      else if(type == IA_TYPE_FLOAT)
      {
         (*(float*)var->value) = *(float*)value;
      }
      else if(type == IA_TYPE_STRING)
      {
         (*(string*)var->value) = *(string*)value;
      }
      else
      {
         /* It's a pointer only */
         var->value = value;
      }
   }
   else if(var)
   {
      cerr << "Error: Expected " << type
           << " variable, but got " << var->type 
           << " at line " << actualLine << " file "
           << fileName << endl;
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

   string varName = "";


   ////////////////////////////////////////////////////
   //                   Debug Functions              //
   ////////////////////////////////////////////////////
   
   /* Syntax: void print(string message)  */
   if(functionName == IA_DEBUG_PRINT)
   {
      iv = getParameter(token, strLine, IA_TYPE_STRING, pos);
      if(iv)
      {
         string st = *(string*)iv->value;
         /* Print the message */
         cout << st << endl;
         if(isFunction(token))
         {
            delete(iv);
         }
      }
   }

   ////////////////////////////////////////////////////
   //             Movimentation Functions            //
   ////////////////////////////////////////////////////

   /* Move to Position */
   else if(functionName == IA_MOVE_TO_POSITION)
   {
      /* Syntax bool moveToPosition(character char, int x, int z) */
      varName = symbols->addTempSymbol(IA_TYPE_BOOL);

      string line = changeFrom(strLine, functionName, 3, varName);
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
         pendAction = eng->actionControl->addAction(line, ACT_MOVE, 
                                                    dude, X, Z,
                                               (type == IASCRIPT_TYPE_MISSION));
      }
   }


   /* Move to Character */
   else if(functionName == IA_MOVE_TO_CHARACTER)
   {
      /* Syntax bool moveToCharacter(character char, character tgt) */
      varName = symbols->addTempSymbol(IA_TYPE_BOOL);

      string line = changeFrom(strLine, functionName, 2, varName);
      character* dude = NULL;
      character* tgt = NULL;

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

      /* Get the Target Character */
      iv = getParameter(token, strLine, IA_TYPE_CHARACTER, pos);
      if(iv != NULL)
      {
         tgt = (character*)iv->value;
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      if( (dude) && (tgt) )
      {
         dude->pathFind.defineMap(actualMap);
         pendAction = eng->actionControl->addAction(line, ACT_MOVE, 
                                                    dude, tgt, 
                                               (type == IASCRIPT_TYPE_MISSION));
      }
   }


   /* Move to Object */
   else if(functionName == IA_MOVE_TO_OBJECT)
   {
      /* Syntax bool moveToObject(character char, object obj) */
      varName = symbols->addTempSymbol(IA_TYPE_BOOL);

      string line = changeFrom(strLine, functionName, 2, varName);
      character* dude = NULL;
      object* obj = NULL;

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

      /* Get the Object */
      iv = getParameter(token, strLine, IA_TYPE_OBJECT, pos);
      if(iv != NULL)
      {
         obj = (object*)iv->value;
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      if( (dude) && (obj) )
      {
         dude->pathFind.defineMap(actualMap);
         pendAction = eng->actionControl->addAction(line, ACT_MOVE, 
                                                    dude, obj,
                                               (type == IASCRIPT_TYPE_MISSION));
      }
   }


   /* Set Idle */
   if(functionName == IA_SET_IDLE)
   {
      /* Syntax void setIdle(character char) */
      iv = getParameter(token, strLine, IA_TYPE_CHARACTER, pos);
      if(iv != NULL)
      {
         character* c = (character*)iv->value;
         if(c != NULL)
         {
            c->setState(STATE_IDLE);
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

   ////////////////////////////////////////////////////
   //                  Time Functions                //
   ////////////////////////////////////////////////////
   /* wait */
   else if(functionName == IA_WAIT)
   {
      /* Syntax bool wait(int seconds) */
      varName = symbols->addTempSymbol(IA_TYPE_BOOL);
      string line = changeFrom(strLine, functionName, 1, varName);

      int seconds = 0;
      iv = getParameter(token, strLine, IA_TYPE_INT, pos);
      if(iv != NULL)
      {
         seconds = *(int*)iv->value;
         if(isFunction(token))
         {
            delete(iv);
         }
         /* Set the pending action (note: time is in mseconds) */
         pendAction = eng->actionControl->addAction(line, ACT_WAIT, 
                                                    seconds*1000,
                                               (type == IASCRIPT_TYPE_MISSION));
      }
   }


   ////////////////////////////////////////////////////
   //                Mission Functions               //
   ////////////////////////////////////////////////////

   /* Mission Add */
   else if(functionName == IA_MISSION_ADD)
   {
      /*! void missionAdd(string missionFile) */
      iv = getParameter(token, strLine, IA_TYPE_STRING, pos);
      if(iv)
      {
         string st = *(string*)iv->value;
         /* Add the mission to the engine */
         missionsController missions;
         missions.addNewMission(st);
         if(isFunction(token))
         {
            delete(iv);
         }
      }
   }


   /* Mission Complete */
   else if(functionName == IA_MISSION_COMPLETE)
   {
      /*! void missionComplete(string missionFile, int ctype) */
      iv = getParameter(token, strLine, IA_TYPE_STRING, pos);
      if(iv)
      {
         string st = *(string*)iv->value;
         /* Add the mission to the engine */
         missionsController missions;
         mission* m = missions.getCurrentMission(st);
         if(m)
         {
            int cType = 1;
            /* Get the cType */
            string nt = "";
            iaVariable* iav = getParameter(nt, strLine, IA_TYPE_INT, pos);
            if(iav)
            {
               cType = *(int*)iav->value;
               if(isFunction(nt))
               {
                  delete(iav);
               }
            }
            
            /*! Do the Completion */
            missions.completeMission(m, cType);
         }
         else
         {
            cerr << "Error: No current mission " << st 
                 << " at " << strLine << " on script: " << fileName << endl;
         }
         if(isFunction(token))
         {
            delete(iv);
         }
      }
   }


   /* Mission is Active */
   else if(functionName == IA_MISSION_IS_ACTIVE)
   {
      /*! bool missionIsActive(string missionFile) */
      iv = getParameter(token, strLine, IA_TYPE_STRING, pos);
      if(iv)
      {
         string st = *(string*)iv->value;
         /* Add the mission to the engine */
         missionsController missions;
         mission* m = missions.getCurrentMission(st);
         bool bl = (m != NULL);

         assignValue(var, (void*)&bl, IA_TYPE_BOOL);

         if(isFunction(token))
         {
            delete(iv);
         }
      }
   }


   /* mission set temp */
   else if(functionName == IA_MISSION_SET_TEMP)
   {
      mission* m = NULL;
      int tmpNumber = 0;
      int newValue = 0;
      string missionFile = "";
      /*! void missionSetTemp(string missionfile, int tmpNumber, int value) */

      /* Get mission */
      iv = getParameter(token, strLine, IA_TYPE_STRING, pos);
      if(iv)
      {
         missionFile = *(string*)iv->value;
         /* get the mission */
         missionsController missions;
         m = missions.getCurrentMission(missionFile);
         if(isFunction(token))
         {
            delete(iv);
         }
      }
      
      /* get temp number */
      iv = getParameter(token, strLine, IA_TYPE_INT, pos);
      if(iv)
      {
         tmpNumber = *(int*)iv->value;
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      /* get value */
      iv = getParameter(token, strLine, IA_TYPE_INT, pos);
      if(iv)
      {
         newValue = *(int*)iv->value;
         if(isFunction(token))
         {
           delete(iv);
         }
      }

      /* Run the function */
      if(m)
      {
         m->setTempFlag(tmpNumber, newValue);
      }
      else
      {
         cerr << "Error: No current mission " << missionFile 
              << " at " << strLine << " on script: " << fileName << endl;
      }
   }

   /* mission get temp */
   else if(functionName == IA_MISSION_GET_TEMP)
   {
      mission* m = NULL;
      int tmpNumber = 0;
      string missionFile = "";
      /* int missionGetTemp(string missionFile, int tmpNumber) */

      /* Get mission */
      iv = getParameter(token, strLine, IA_TYPE_STRING, pos);
      if(iv)
      {
         missionFile = *(string*)iv->value;
         /* get the mission */
         missionsController missions;
         m = missions.getCurrentMission(missionFile);
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      /* get temp number */
      iv = getParameter(token, strLine, IA_TYPE_INT, pos);
      if(iv)
      {
         tmpNumber = *(int*)iv->value;
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      /* Run the function */
      if(m)
      {
         int i = m->getTempFlag(tmpNumber);
         assignValue(var, (void*)&i, IA_TYPE_INT);
      }
      else
      {
         cerr << "Error: No current mission " << missionFile 
              << " at " << strLine << " on script: " << fileName << endl;
      }
   }


   /* mission set XP */
   else if(functionName == IA_MISSION_SET_XP)
   {
      mission* m = NULL;
      int xpValue = 0;
      string missionFile = "";
      /* void missionSetXp(string missionFile, int xpValue) */

      /* Get mission */
      iv = getParameter(token, strLine, IA_TYPE_STRING, pos);
      if(iv)
      {
         missionFile = *(string*)iv->value;
         /* get the mission */
         missionsController missions;
         m = missions.getCurrentMission(missionFile);
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      /* get xp value */
      iv = getParameter(token, strLine, IA_TYPE_INT, pos);
      if(iv)
      {
         xpValue = *(int*)iv->value;
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      /* Run the function */
      if(m)
      {
         m->setXp(xpValue);
      }
      else
      {
         cerr << "Error: No current mission " << missionFile 
              << " at " << strLine << " on script: " << fileName << endl;
      }
   }


   ////////////////////////////////////////////////////
   //                 Feats Functions                //
   ////////////////////////////////////////////////////

   /* int function(int featID) */
   else if( (functionName == IA_FEAT_ACTUAL_QUANTITY) ||
            (functionName == IA_FEAT_COST) ||
            (functionName == IA_FEAT_QUANTITY_PER_DAY) )
   {
      int featID = -1;
      feat* ft = NULL;

      /* Get feat */
      iv = getParameter(token, strLine, IA_TYPE_INT, pos);
      if(iv)
      {
         featID = *(int*)iv->value;
         if(characterOwner)
         {
            /* Get really the feat */
            ft = characterOwner->actualFeats.featByNumber(featID);
         }
         else
         {
            cerr << "Error: No character owner defined for function: " << 
                    functionName << " at " << strLine << " on Script " << 
                    fileName << endl;
         }
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      if(ft)
      {
         int i = 0;
         if(functionName == IA_FEAT_ACTUAL_QUANTITY)
         {
            i = (int)ft->actualQuantity;
         }
         else if(functionName == IA_FEAT_COST)
         {
            i = ft->costToUse;
         }
         else if(functionName == IA_FEAT_QUANTITY_PER_DAY)
         {
            i = ft->quantityPerDay;
         }
         /* Assign the result value */
         assignValue(var, (void*)&i, IA_TYPE_INT);
      }
      else
      {
         cerr << "Error: Unknow feat " << featID << " at " << strLine 
              << " on script " << fileName << endl;
      }
   }

   /* int function(character target) */
   else if( (functionName == IA_FEAT_GET_RANDOM_ATTACK) ||
            (functionName == IA_FEAT_GET_POWERFULL_ATTACK) ||
            (functionName == IA_FEAT_GET_RANDOM_HEAL) ||
            (functionName == IA_FEAT_GET_POWERFULL_HEAL) )
   {
      if(characterOwner)
      {
         character* dude = NULL;
         int ft = -1;

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

         if(dude != NULL)
         {
            /* Syntax: int getRandomAttackFeat() */
            if(functionName == IA_FEAT_GET_RANDOM_ATTACK)
            {
               ft = characterOwner->actualFeats.getRandomNPCAttackFeat(
                                                           characterOwner,dude);
            }
            /* Syntax: int getPowerfullAttackFeat() */
            else if(functionName == IA_FEAT_GET_POWERFULL_ATTACK)
            {
               ft = characterOwner->actualFeats.getPowerfullAttackFeat(
                                                           characterOwner,dude);
            }
            /* Syntax: int getRandomHealFeat() */
            else if(functionName == IA_FEAT_GET_RANDOM_HEAL)
            {
               ft = characterOwner->actualFeats.getRandomHealFeat(
                                                                characterOwner);
            }
            /* Syntax: int getPowerfullHealFeat() */
            else if(functionName == IA_FEAT_GET_POWERFULL_HEAL)
            {
               ft=characterOwner->actualFeats.getPowerfullHealFeat(
                                                                characterOwner);
            }

            /* Assign value */
            if(ft != -1)
            {
               assignValue(var, (void*)&ft, IA_TYPE_INT);
            }
         }
         else
         {
            cerr << "Error: Tried to access a NULL character at line " 
                 << actualLine << " of the script: " << fileName << endl;
         }
      }
   }

   /* void function(id, character) */
   else if(functionName == IA_FEAT_USE_AT_CHARACTER)
   {
      character* dude = NULL;
      int featId = -1;

      /* Get feat */
      iv = getParameter(token, strLine, IA_TYPE_INT, pos);
      if(iv != NULL)
      {
         featId = (*(int*)iv->value);
         if(isFunction(token))
         {
            delete(iv);
         }
      }

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

      /* call the function */
      if(dude != NULL)
      {
         /* Get the feat number  */
         if(!featId != -1)
         {
            if( (characterOwner) && (characterOwner->getCanAttack()) )
            {
               /* Verify if at range */
               if(actionInRange(characterOwner->xPosition, 
                                characterOwner->zPosition,  
                                dude->xPosition, dude->zPosition,
                                characterOwner->getActiveFeatRange() * 
                                METER_TO_DNT))
               {
                  //TODO verify if is an attack and break before call!
                  characterOwner->actualFeats.applyAttackAndBreakFeat(
                                                           *characterOwner,
                                                           featId, dude);
               }
            }
         }
      }
      else
      {
         cerr << "Error: Tried to access a NULL character at line " 
                 << actualLine << " of the script: " << fileName << endl;
      }
   }

   /* void function(string, object) */
   else if(functionName == IA_FEAT_USE_AT_OBJECT)
   {
      //TODO
   }

   
   ////////////////////////////////////////////////////
   //                Skills Functions                //
   ////////////////////////////////////////////////////

   /* Syntax int function(character c, string s) */
   else if((functionName == IA_SKILL_POINTS) || 
           (functionName == IA_ATT_MODIFIER) )
   {
      character* c = NULL;
      string tmpName = "";

      /* Get Character */
      iv = getParameter(token, strLine, IA_TYPE_CHARACTER, pos);
      if(iv != NULL)
      {
         character* c = (character*)iv->value;
         if(c == NULL)
         {
            cerr << "Error: Tried to access a NULL character at line " 
                 << actualLine << " of the script: " << fileName << endl;
         }
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      /* Get Name */
      iv = getParameter(token, strLine, IA_TYPE_STRING, pos);
      if(iv)
      {
         tmpName = *(string*)iv->value;
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      /* Run Function */
      if(c != NULL)
      {
         /* Syntax int skillPoints(character c, string skillName)  */
         if(functionName == IA_SKILL_POINTS)
         {
            skill* skl = c->sk.getSkillByString(tmpName);
            if(skl != NULL)
            {
               int i = skl->points;
               assignValue(var, (void*)&i, IA_TYPE_INT);
            }
         }
         /* Syntax int attModifier(character c, string attName) */
         else if(functionName == IA_ATT_MODIFIER)
         {
            skill* skl = c->sk.getSkillByString(tmpName);
            if(skl != NULL)
            {
               int i = c->attBonus(skl);
               assignValue(var, (void*)&i, IA_TYPE_INT);
            }
         }
      }
   }


   ////////////////////////////////////////////////////
   //                  Fight Functions               //
   ////////////////////////////////////////////////////


   /* void function() */
   else if( (functionName == IA_FIGHT_ENTER) ||
            (functionName == IA_FIGHT_EXIT) ||
            (functionName == IA_FIGHT_RUN_AWAY_FROM_BATTLE) )
   {
     
      if(!var)
      {
         /* syntax: void fightEnter() */
         if(functionName == IA_FIGHT_ENTER)
         {
            characterOwner->setPsychoState(PSYCHO_HOSTILE);
            eng->enterBattleMode(false);
         }
         /* syntax void fightExit() */
         else if(functionName == IA_FIGHT_EXIT)
         {
            eng->exitBattleMode();
         }
         /* syntax void runAwayFromBattle() */
         else if(functionName == IA_FIGHT_RUN_AWAY_FROM_BATTLE)
         {
            float posX = 0;
            float posZ = 0;
            if(characterOwner->currentEnemy != NULL)
            {
               /* Define a position away from the current target */
               //FIXME... wrong!
               float angle = getAngle(characterOwner->xPosition,
                                      characterOwner->zPosition,
                                      characterOwner->currentEnemy->xPosition,
                                      characterOwner->currentEnemy->zPosition);
               posX = characterOwner->xPosition - 
                      (cos(deg2Rad(angle))*2*WALK_PER_MOVE_ACTION);
               posZ = characterOwner->zPosition - 
                      (sin(deg2Rad(angle))*2*WALK_PER_MOVE_ACTION);

               //cout << "Angle: " << angle << " X: " << posX << " Z: " << posZ 
               //     << endl;

               /* Make the owner go to the position */
               string line = changeFrom(strLine, functionName, 0, varName);
               characterOwner->pathFind.defineMap(actualMap);
               pendAction = eng->actionControl->addAction(line, ACT_MOVE, 
                                                          characterOwner, 
                                                          posX, posZ,
                                               (type == IASCRIPT_TYPE_MISSION));
            }
         }
      }
      else
      {
         cerr << "Error: " << functionName << " don't return any value!" 
              << " at " << strLine << " on script: " << fileName << endl;
      }
   }

   /* bool function() */
   else if(functionName == IA_FIGHT_CAN_ATTACK)
   {
      /* syntax bool canAttack() */
      bool res = characterOwner->getCanAttack();
      assignValue(var, (bool*)&res, IA_TYPE_BOOL);
   }

   /* getNearestEnemy */
   else if(functionName == IA_FIGHT_GET_NEAREST_ENEMY)
   {
      character* dude = NULL;

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

      if(dude != NULL)
      {
         /* syntax character getNearestEnemy(character dude) */
         character* enemy = eng->getFightSystem()->getNearestEnemy(dude);
         assignValue(var, (void*)enemy, IA_TYPE_CHARACTER);
      }
      else
      {
         cerr << "Error: Tried to access a NULL character at line " 
                 << actualLine << " of the script: " << fileName << endl;
      }
   }


   ////////////////////////////////////////////////////
   //                Character Functions             //
   ////////////////////////////////////////////////////

   /* Syntax character function(s)  */
   else if(functionName == IA_GET_NPC_BY_NAME)
   {
      string charName = "";

      /* Get the name */
      iv = getParameter(token, strLine, IA_TYPE_STRING, pos);
      if(iv != NULL)
      {
         charName = *(string*)iv->value;
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      /* character getNPCByName(string s) */
      character* dude = NULL;
      if(eng->NPCs != NULL)
      {
         dude = eng->NPCs->getCharacter(charName);
      }
      assignValue(var, (void*)dude, IA_TYPE_CHARACTER);
   }

   /* Syntax void function(character c, int i) */
   else if(functionName == IA_CHARACTER_SET_PSYCHO)
   {
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
         /* Syntax void setPsycho(character c, int psycho)  */
         dude->setPsychoState(psy);
      }
      else
      {
         cerr << "Error: Tried to access a NULL character at line " 
                 << actualLine << " of the script: " << fileName << endl;
      }
   }

   /* Syntax int function(character c)  */
   else if( (functionName == IA_CHARACTER_GET_ACTUAL_LIFE) ||
            (functionName == IA_CHARACTER_GET_MAX_LIFE) ||
            (functionName == IA_CHARACTER_GET_PSYCHO) )
   {
      iv = getParameter(token, strLine, IA_TYPE_CHARACTER, pos);
      if(iv != NULL)
      {
         character* c = (character*)iv->value;
         if(c != NULL)
         {
            int i = 0;
            /* Syntax int getActualLife(character c)  */
            if(functionName == IA_CHARACTER_GET_ACTUAL_LIFE)
            {
               i = c->getLifePoints();
            }
            /* Syntax int getMaxLife(character c)  */
            else if(functionName == IA_CHARACTER_GET_MAX_LIFE)
            {
               i = c->getMaxLifePoints();
            }
            /* Syntax int getPsycho(character c)  */
            else if(functionName == IA_CHARACTER_GET_PSYCHO)
            {
               i = c->getPsychoState();
            }
            assignValue(var, (void*)&i, IA_TYPE_INT);
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

   /* syntax bool function(character ref, character target) */
   else if(functionName == IA_CHARACTER_AT_RANGE)
   {
      character* ref = NULL;
      character* tgt = NULL;

      /* get ref character */
      iv = getParameter(token, strLine, IA_TYPE_CHARACTER, pos);
      if(iv != NULL)
      {
         ref = (character*)iv->value;
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      /* get target character */
      iv = getParameter(token, strLine, IA_TYPE_CHARACTER, pos);
      if(iv != NULL)
      {
         tgt = (character*)iv->value;
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      if( (tgt != NULL) && (ref != NULL) )
      {
         /* syntax characterArRange(character ref, character tgt) */
         bool atRange = actionInRange(ref->xPosition, ref->zPosition,  
                                      tgt->xPosition, tgt->zPosition,
                                      ref->getActiveFeatRange() * METER_TO_DNT);
         assignValue(var, (void*)&atRange, IA_TYPE_BOOL);
      }
      else
      {
         cerr << "Error: Tried to access a NULL character at line " 
              << actualLine << " of the script: " << fileName << endl;
      }

   }
   
   ////////////////////////////////////////////////////
   //                Inventory Functions             //
   ////////////////////////////////////////////////////


   /* inventoryHave */
   else if(functionName == IA_INVENTORY_HAVE)
   {
      /* Syntax: bool inventoryHave(character* owner, string objectFile) */
      character* dude = NULL;
      string objectFile = "";

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

      /* Get Object FileName */
      iv = getParameter(token, strLine, IA_TYPE_STRING, pos);
      if(iv != NULL)
      {
         objectFile = *(string*)iv->value;
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      /* Set the result */
      bool bl = false;
      
      if(dude != NULL)
      {         
         bl = (dude->inventories->getItemByFileName(objectFile) != NULL);
      }
      assignValue(var, (void*)&bl, IA_TYPE_BOOL);
   }

   ////////////////////////////////////////////////////
   //                  Dialog Functions              //
   ////////////////////////////////////////////////////

   /* dialogSetInitial */
   else if(functionName == IA_DIALOG_SET_INITIAL)
   {
      /* Get Character FileName */
      string charFile = "";
      iv = getParameter(token, strLine, IA_TYPE_STRING, pos);
      if(iv != NULL)
      {
         charFile = *(string*)iv->value;
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      /* Get Map FileName */
      string mapFile = "";
      iv = getParameter(token, strLine, IA_TYPE_STRING, pos);
      if(iv != NULL)
      {
         mapFile = *(string*)iv->value;
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      /* Get Dialog Number */
      int dialogNum = -1;
      iv = getParameter(token, strLine, IA_TYPE_INT, pos);
      if(iv != NULL)
      {
         dialogNum = *(int*)iv->value;
         if(isFunction(token))
         {
            delete(iv);
         }
      }

      if( (dialogNum != -1) && (!charFile.empty()) &&
          (!mapFile.empty()) )
      {
         modState modif;
         modif.mapTalkAddAction(MODSTATE_TALK_ENTER_VALUE,
                                charFile, mapFile, dialogNum);
         /* Directly apply, if at the same map  */
         if( (actualMap) && (npcs) && (actualMap->getFileName() == mapFile))
         {
            /* apply it!  */
            character* ch = npcs->getCharacter(charFile);
            if(ch)
            {
               ch->setInitialConversation(dialogNum);
            }
         }

      }
      else
      {
         cerr << "Error: Undefined Parameter at line " 
              << actualLine << " of the script: " << fileName << endl;
      }

   }


   ////////////////////////////////////////////////////
   //                Quick Pointers                  //
   ////////////////////////////////////////////////////

   /* IA_SELF_OBJECT */
   else if(functionName == IA_SELF_OBJECT)
   {
      /* Define the variable as the object owner */
      if(var)
      {
         assignValue(var, (void*)objectOwner, IA_TYPE_OBJECT);
      }
   }

   /* IA_SELF_CHARACTER */
   else if(functionName == IA_SELF_CHARACTER)
   {
      /* Define the variable as the character owner */
      if(var)
      {
         assignValue(var, (void*)characterOwner, IA_TYPE_CHARACTER);
      }
   }

   /* IA_SELF_MISSION */
   else if(functionName == IA_SELF_MISSION)
   {
      if(var)
      {
         assignValue(var, (void*)&fileName, IA_TYPE_STRING);
      }
   }

   /* Active Character */
   else if(functionName == IA_ACTIVE_CHARACTER)
   {
      /* Define the variable as the character owner */
      if(var)
      {
         character* dude = eng->PCs->getActiveCharacter();
         assignValue(var, (void*)dude, IA_TYPE_CHARACTER);
      }
   }


   /* Actual Map */
   else if(functionName == IA_ACTUAL_MAP)
   {
      /* Define the variable as the actualMap */
      if(var)
      {
         string mapFileName = actualMap->getFileName();
         assignValue(var, (void*)&mapFileName, IA_TYPE_STRING);
      }
   }


   ////////////////////////////////////////////////////
   //              Imediate Expressions              //
   ////////////////////////////////////////////////////

   /* true */
   else if(functionName == IA_TRUE)
   {
      /* Define the variable as true */
      if(var)
      {
         bool v = true;
         assignValue(var, (void*)&v, IA_TYPE_BOOL);
      }
   }


   /* false */
   else if(functionName == IA_FALSE)
   {
      /* Define the variable as false */
      if(var)
      {
         bool v = false;
         assignValue(var, (void*)&v, IA_TYPE_BOOL);
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
                  callFunction(varStack[varPos], postFix, token, pos);
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
               else if(isString(token))
               {
                  /* Create the string value on the stack */
                  varStack[varPos] = new iaVariable(IA_TYPE_STRING, "name");

                  /* remove the first and the last " */
                  token.erase(0,1);
                  token.erase(token.length()-1,1);

                  *(string*)varStack[varPos]->value = token;
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

