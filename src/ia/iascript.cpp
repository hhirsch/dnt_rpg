/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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

#include "iascript.h"
#include "../engine/dccnit.h"
#include "../etc/dirs.h"

/***********************************************************************
 *                           Constructor                               *
 ***********************************************************************/
iaScript::iaScript(void* usedEngine)
{
   objectOwner = NULL;
   characterOwner = NULL;
   actualMap = NULL;
   npcs = NULL;
   pendAction = NULL;
   actualEngine = usedEngine;
   type = IASCRIPT_TYPE_DEFAULT;
}

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
             ifstream::in);
   if(!file)
   {
      cerr << "Failed to open the script: "
           << dir.getRealFile(fileName) << endl;
   }

   symbols = new iaSymbolsTable();
   jumpStack = new iaStack();

   context = "";
   actualLine = 0;

   createParameters();
}

/***********************************************************************
 *                         createParameters                            *
 ***********************************************************************/
void iaScript::createParameters()
{
   bool done = false;
   string type, name, token, strBuffer;
   unsigned int pos = 0;

   if(file)
   {

      /* Let's search for the script main definition */
      while( (!done) || (!file.eof()) )
      {
         getline(file, strBuffer);
         pos = 0;
         token = nextToken(strBuffer, pos);

         if(token == IA_SETENCE_SCRIPT)
         {
            /* It's the script */
            done = true;

            /* Get the '(' */
            nextToken(strBuffer, pos);

            /* Let's create each parameter */
            type = nextToken(strBuffer, pos);
            name = nextToken(strBuffer, pos);
            while( (!type.empty()) && (!name.empty()) &&
                  (type != IA_OPERATOR_RPARENTHESIS) )
            {
               symbols->addSymbol(type, name);
               type = nextToken(strBuffer, pos);
               name = nextToken(strBuffer, pos);
            }
         }
      }

      /* Back to the file's init */
      file.clear();
      file.seekg(0, ios::beg);
   }
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
 *                           setParameter                              *
 ***********************************************************************/
bool iaScript::setParameter(string paramName, void* value)
{
   iaVariable* iv = symbols->getSymbol(paramName);

   if(iv != NULL)
   {
      (*iv) = value;
      return(true);
   }

   cerr << "Error: Unknown script parameter '" << paramName 
        << "' at " << fileName << endl;
   return(false);
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
 *                        getCharacterOwner                            *
 ***********************************************************************/
character* iaScript::getCharacterOwner()
{
   return(characterOwner);
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

#ifdef DNT_DEBUG_SCRIPTS
   cerr << "Interpreting: " << fileName << endl;
#endif

   if(file)
   {
      while(!done)
      {
         if( (pendAction) && (!pendAction->isRunning()) )
         {
            /* Go to where stops at the action */
            strBuffer = pendAction->getScriptLine();
            interpret = true;

            /* Remove the action from the controller */
            engine* eng = (engine*)actualEngine;
            eng->actionControl->removeAction(pendAction);
            pendAction = NULL;

#ifdef DNT_DEBUG_SCRIPTS
            cerr << fileName << ": " << "Done Pending Action..." << endl;
#endif
         }
         else if(pendAction)
         {
            /* The pending action isn't finished, so do nothing */
            done = true;
            interpret = false;

#ifdef DNT_DEBUG_SCRIPTS
            cerr << fileName << ": " << "Waiting Pending Action..." << endl;
#endif
         }
         else
         {
            /* Read Line */
            lastPos = file.tellg();
            getline(file, strBuffer);
            actualLine++;
            lines++;
            interpret = true;

#ifdef DNT_DEBUG_SCRIPTS
            cerr << fileName << ": " << strBuffer << endl;
#endif
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
                           if(token == IA_SETENCE_END)
                           {
                              /* An end is just a block less */
                              numBegins--;
                           }
                           else if( (token == IA_SETENCE_ELSE) && 
                                    (numBegins != 1) )
                           {
                              /* The number of begins must be kept the same,
                               * because is an else (or else if) for another
                               * if (so the block must be kept opened). */
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
                                 else
                                 {
                                    /* Is false, so the "if begin" must be 
                                     * still opened */
                                    numBegins = 1;
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
   else
   {
      cerr << "Error: Unknow script file to interpret!" << endl;
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
      iv = new iaVariable(paramType, "__param__");
      callFunction(iv,strLine,token,pos);
   }
   else if(!iv)
   {
      /* no variable found, must be a literal constant. 
       * let's create a temporary variable to it */
      if( (paramType == IA_TYPE_INT) && (isInteger(token)) )
      {
         /* Create the integer value at the stack */
         int ti;
         sscanf(token.c_str(),"%d",&ti);
         iv = new iaVariable(IA_TYPE_INT, "__param__");
         *(int*)iv->value = ti;
      }
      else if( (paramType == IA_TYPE_FLOAT) && 
               (isInteger(token)) || (isFloat(token)) )
      {
         /* Create the float value at the stack */
         float tf;
         sscanf(token.c_str(),"%f",&tf);
         iv = new iaVariable(IA_TYPE_FLOAT, "__param__");
         *(float*)iv->value = tf;
      }
      else if( (paramType == IA_TYPE_STRING) && (isString(token)) )
      {
         /* Create the string value on the stack */
         iv = new iaVariable(IA_TYPE_STRING, "__param__");

         /* remove the first and the last " */
         token.erase(0,1);
         token.erase(token.length()-1,1);

         *(string*)iv->value = token;
      }
      else
      {
         cerr << "Error: unknow token '" << token << "' at " << strLine
            << " on script " << fileName << " line " << actualLine
            << endl;
         return(NULL);
      }
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
 *                          getParameteri                              *
 ***********************************************************************/
int iaScript::getParameteri(string& token, string strLine, unsigned int& pos)
{
   iaVariable* iv = NULL;
   int val = 0;

   /* Try to get the parameter */
   iv = getParameter(token, strLine, IA_TYPE_INT, pos);
   if(iv)
   {
      /* Copy the value */
      val = (*(int*)iv->value);

      /* Delete temporary variable (if is temporary) */
      if(iv->name == "__param__")
      {
         delete(iv);
      }
   }

   return(val);
}

/***********************************************************************
 *                          getParameterf                              *
 ***********************************************************************/
float iaScript::getParameterf(string& token, string strLine, unsigned int& pos)
{
   iaVariable* iv = NULL;
   float val = 0.0f;

   /* Try to get the parameter */
   iv = getParameter(token, strLine, IA_TYPE_FLOAT, pos);
   if(iv)
   {
      /* Copy the value */
      if(iv->type == IA_TYPE_FLOAT)
      {
         val = *(float*)iv->value;
      }
      else
      {
         val = *(int*)iv->value;
      }

      /* Delete temporary variable (if is temporary) */
      if(iv->name == "__param__")
      {
         delete(iv);
      }
   }

   return(val);
}

/***********************************************************************
 *                          getParameters                              *
 ***********************************************************************/
string iaScript::getParameters(string& token, string strLine, unsigned int& pos)
{
   iaVariable* iv = NULL;
   string st = "";

   /* Try to get the parameter */
   iv = getParameter(token, strLine, IA_TYPE_STRING, pos);
   if(iv)
   {
      /* Copy the string to return */
      st = *(string*)iv->value;

      /* Delete the temporary variable, if any */
      if(iv->name == "__param__")
      {
         delete(iv);
      }
   }

   /* Return the string got */
   return(st);
}

/***********************************************************************
 *                          getParameterc                              *
 ***********************************************************************/
character* iaScript::getParameterc(string& token, string strLine, 
      unsigned int& pos)
{
   iaVariable* iv = NULL;
   character* dude = NULL;

   /* Get character */
   iv = getParameter(token, strLine, IA_TYPE_CHARACTER, pos);
   if(iv != NULL)
   {
      dude = (character*)iv->value;
      if(iv->name == "__param__")
      {
         delete(iv);
      }
   }

   return(dude);
}

/***********************************************************************
 *                          getParametero                              *
 ***********************************************************************/
object* iaScript::getParametero(string& token, string strLine, 
      unsigned int& pos)
{
   iaVariable* iv = NULL;
   object* obj = NULL;

   /* Get character */
   iv = getParameter(token, strLine, IA_TYPE_CHARACTER, pos);
   if(iv != NULL)
   {
      obj = (object*)iv->value;
      if(iv->name == "__param__")
      {
         delete(iv);
      }
   }

   return(obj);
}

/***********************************************************************
 *                          getParameterd                              *
 ***********************************************************************/
diceThing* iaScript::getParameterd(string& token, string strLine, 
      unsigned int& pos)
{
   iaVariable* iv = NULL;
   diceThing* dice = NULL;

   /* Get character */
   iv = getParameter(token, strLine, IA_TYPE_CHARACTER, pos);
   if(iv != NULL)
   {
      dice = (diceThing*)iv->value;
      if(iv->name == "__param__")
      {
         delete(iv);
      }
   }

   return(dice);
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
      else if(type == IA_TYPE_DICE)
      {
         cerr << "Error: Invalid dice assign on script!" << endl;
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
   engine* eng = (engine*)actualEngine;

   string varName = "";


   ////////////////////////////////////////////////////
   //                   Debug Functions              //
   ////////////////////////////////////////////////////
   
   /* Syntax: void print(string message)  */
   if(functionName == IA_DEBUG_PRINT)
   {
      string st = getParameters(token, strLine, pos);
      
      /* Print the message */
      cout << st << endl;
   }

   ////////////////////////////////////////////////////
   //               Briefing Functions               //
   ////////////////////////////////////////////////////
   
   /* Syntax: void briefing(string message)  */
   if(functionName == IA_BRIEFING)
   {
      briefing brief;
      string st = getParameters(token, strLine, pos);
      
      /* Print the message */
      brief.addText(st);
   }

   ////////////////////////////////////////////////////
   //              Random (Dice) Function            //
   ////////////////////////////////////////////////////

   /* Syntax: int rollDice(int diceFaces) */ 
   else if(functionName == IA_ROLL_DICE)
   {
      dice dX;
      int diceFaces;
      int res = 0;

      /* Get the dice faces */
      diceFaces = getParameteri(token, strLine, pos);
      dX.setType(diceFaces);

      /* And Roll the dice */
      res = dX.roll();
      assignValue(var, (void*)&res, IA_TYPE_INT);
   }

   /* Syntax: bool roll(character actor, string skill, int difficulty)  */
   else if(functionName == IA_DICE_ROLL)
   {
      character* actor = NULL;
      string param = "";
      int difficulty = 0;
      bool res = false;

      /* Get the character */
      actor = getParameterc(token, strLine, pos);
      
      /* Get the state to check */
      param = getParameters(token, strLine, pos);
      
      /* Get the roll difficulty */
      difficulty = getParameteri(token, strLine, pos);

      /* Finally, let's try the check */
      if(actor != NULL)
      {
         res = actor->doCheck(param, difficulty);
      }
      else
      {
         cerr << "Error: Tried to access a NULL character at line " 
              << actualLine << " of the script: " << fileName << endl;

      }

      /* And set the result */
      assignValue(var, (void*)&res, IA_TYPE_BOOL);
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
      dude = getParameterc(token, strLine, pos);

      /* Get the X */
      X = getParameterf(token, strLine, pos);

      /* Get the Z */
      Z = getParameterf(token, strLine, pos);

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
      dude = getParameterc(token, strLine, pos);

      /* Get the Target Character */
      tgt = getParameterc(token, strLine, pos);

      /* Do it */
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
      dude = getParameterc(token, strLine, pos);

      /* Get the Object */
      obj = getParametero(token, strLine, pos);

      /* Do the move */
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
      character* c = getParameterc(token, strLine, pos);
      if(c != NULL)
      {
         c->setState(STATE_IDLE);
      }
      else
      {
         cerr << "Error: Tried to access a NULL character at line " 
            << actualLine << " of the script: " << fileName << endl;
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

      int seconds = getParameteri(token, strLine, pos);
      /* Set the pending action (note: time is in mseconds) */
      pendAction = eng->actionControl->addAction(line, ACT_WAIT, seconds*1000,
            (type == IASCRIPT_TYPE_MISSION));
   }


   ////////////////////////////////////////////////////
   //                Mission Functions               //
   ////////////////////////////////////////////////////

   /* Mission Add */
   else if(functionName == IA_MISSION_ADD)
   {
      /*! void missionAdd(string missionFile) */
      missionsController missions;
      string st = getParameters(token, strLine, pos);
         
      /* Add the mission to the engine */
      if(!st.empty())
      {
         missions.addNewMission(st);
      }
   }

   /* Mission Abort */
   else if(functionName == IA_MISSION_ABORT)
   {
      missionsController missions;

      /*! void missionAbort(string missionFile) */
      string st = getParameters(token, strLine, pos);
      /* Search for the mission to the engine */
      mission* m = missions.getCurrentMission(st);
      if(m)
      {
         /*! Do the Abort */
         missions.completeMission(m, -1);
      }
      else
      {
         cerr << "Error: No current mission '" << st 
            << "' at " << strLine << " on script: " << fileName << endl;
      }
   }


   /* Mission Complete */
   else if(functionName == IA_MISSION_COMPLETE)
   {
      /*! void missionComplete(string missionFile, int ctype) */
      string st = getParameters(token, strLine, pos);
      /* Add the mission to the engine */
      missionsController missions;
      mission* m = missions.getCurrentMission(st);
      if(m)
      {
         int cType;

         /* Get the cType */
         string nt = "";
         cType = getParameteri(nt, strLine, pos);

         /*! Do the Completion */
         missions.completeMission(m, cType);
      }
      else
      {
         cerr << "Error: No current mission '" << st 
            << "' at " << strLine << " on script: " << fileName << endl;
      }
   }


   /* Mission is Active */
   else if(functionName == IA_MISSION_IS_ACTIVE)
   {
      /*! bool missionIsActive(string missionFile) */
      string st = getParameters(token, strLine, pos);

      /* Add the mission to the engine */
      missionsController missions;
      mission* m = missions.getCurrentMission(st);
      bool bl = (m != NULL);

      assignValue(var, (void*)&bl, IA_TYPE_BOOL);

   }


   /* mission set temp */
   else if(functionName == IA_MISSION_SET_TEMP)
   {
      mission* m = NULL;
      int tmpNumber = 0;
      int newValue = 0;
      missionsController missions;
      string missionFile = "";

      /*! void missionSetTemp(string missionfile, int tmpNumber, int value) */

      /* Get mission */
      missionFile = getParameters(token, strLine, pos);
      /* get the mission */
      m = missions.getCurrentMission(missionFile);
      
      /* get temp number */
      tmpNumber = getParameteri(token, strLine, pos);

      /* get value */
      newValue = getParameteri(token, strLine, pos);

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
      missionsController missions;

      /* int missionGetTemp(string missionFile, int tmpNumber) */

      /* Get mission */
      missionFile = getParameters(token, strLine, pos);
      m = missions.getCurrentMission(missionFile);

      /* get temp number */
      tmpNumber = getParameteri(token, strLine, pos);

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
      missionsController missions;
      /* void missionSetXp(string missionFile, int xpValue) */

      /* Get mission */
      missionFile = getParameters(token, strLine, pos);
      m = missions.getCurrentMission(missionFile);

      /* get xp value */
      xpValue = getParameteri(token, strLine, pos);

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
      featID = getParameteri(token, strLine, pos);
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
            i = ft->info->quantityPerDay;
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
         dude = getParameterc(token, strLine, pos);

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
      featId = getParameteri(token, strLine, pos);

      /* Get character */
      dude = getParameterc(token, strLine, pos);

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
           (functionName == IA_ATT_MODIFIER) ||
           (functionName == IA_CLASS_LEVEL) )
   {
      character* c = NULL;
      string tmpName = "";

      /* Get Character */
      c = getParameterc(token, strLine, pos);
      if(c == NULL)
      {
         cerr << "Error: Tried to access a NULL character at line " 
            << actualLine << " of the script: " << fileName << endl;
      }

      /* Get Name */
      tmpName = getParameters(token, strLine, pos);

      /* Run Function */
      if(c != NULL)
      {
         /* Syntax int classLevel(character c, string className)  */
         if(functionName == IA_CLASS_LEVEL)
         {
            classes clList;
            classe* cl = clList.getClassByString(tmpName);
            if(cl != NULL)
            {
               int i = c->getLevel(cl);
               assignValue(var, (void*)&i, IA_TYPE_INT);
            }
         }
         /* Syntax int skillPoints(character c, string skillName)  */
         else if(functionName == IA_SKILL_POINTS)
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
   //                   Map Functions                //
   ////////////////////////////////////////////////////
   /* void function(string s)*/
   else if(functionName == IA_MAP_TRAVEL)
   {
      /*! void mapTravel(string mapFile) */
      string st = getParameters(token, strLine, pos);
      if(eng)
      {
         eng->loadMap(st);
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
      dude = getParameterc(token, strLine, pos);

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

   /* Syntax:
      bool doAttack(character actor, character target, dice d, string base) */
   else if(functionName == IA_FIGHT_DO_ATTACK)
   {
      character* actor = getParameterc(token, strLine, pos);
      character* target = getParameterc(token, strLine, pos);
      diceThing* d = getParameterd(token, strLine, pos);
      string baseBonus = getParameters(token, strLine, pos);
      bool res = false;

      if( (actor) && (target) && (d) )
      {
         // TODO doHealOrAttack(actor, target, *d, 
      }

      assignValue(var, (void*)res, IA_TYPE_BOOL);
   }

   
   ////////////////////////////////////////////////////
   //                 Object Functions               //
   ////////////////////////////////////////////////////

   /* Syntax object function(string, float, float) */
   else if(functionName == IA_GET_OBJECT)
   {
      string objName = "";
      float posX=0, posY=0, posZ=0;

      /* Get the name */
      objName = getParameters(token, strLine, pos);

      /* Get the X position */
      posX = getParameterf(token, strLine, pos);

      /* Get the Y position */
      posY = getParameterf(token, strLine, pos);

      /* Get the Z position */
      posZ = getParameterf(token, strLine, pos);

      /* object getObject(string fileName, float posX, float posZ) */
      object* obj = NULL;
      obj = objectsList::search(objName, posX, posY, posZ);
      assignValue(var, (void*)obj, IA_TYPE_OBJECT);
   }

   /* Syntax int function(object) */
   else if(functionName == IA_GET_OBJECT_STATE)
   {
      object* obj = NULL;

      /* Get object */
      obj = getParametero(token, strLine, pos);

      /* Set the result */
      int res = -1;
      
      if(obj != NULL)
      {
         /* Syntax: int getObjectState(object o) */
         if(functionName == IA_GET_OBJECT_STATE)
         {
            res = obj->getState();
         }
      }
      else
      {
         cout << "NULL Object!" << endl;
      }
      assignValue(var, (void*)&res, IA_TYPE_INT);
   }

   ////////////////////////////////////////////////////
   //                Character Functions             //
   ////////////////////////////////////////////////////

   /* Syntax character function(s)  */
   else if(functionName == IA_GET_NPC_BY_NAME)
   {
      string charName = "";

      /* Get the name */
      charName = getParameters(token, strLine, pos);

      /* character getNPCByName(string s) */
      character* dude = NULL;
      if(eng->NPCs != NULL)
      {
         dude = eng->NPCs->getCharacter(charName);
      }
      assignValue(var, (void*)dude, IA_TYPE_CHARACTER);
   }

   /* Syntax bool function(character c) */
   else if(functionName == IA_CHARACTER_IS_ALIVE)
   {
      character* dude = NULL;

      /* Get character */
      dude = getParameterc(token, strLine, pos);

      /* Set the result */
      bool bl = true;
      
      if(dude != NULL)
      {
         /* Syntax: bool isAlive(character c) */
         if(functionName == IA_CHARACTER_IS_ALIVE)
         {
            bl = dude->isAlive();
         }
      }
      assignValue(var, (void*)&bl, IA_TYPE_BOOL);
   }

   /* Syntax bool function(string s) */
   else if(functionName == IA_CHARACTER_IS_ALL_DEAD)
   {
      string s = "";

      /* Get string */
      s = getParameters(token, strLine, pos);

      /* Set the result */
      bool bl = false;
      
      if(!s.empty())
      {
         /* Syntax: bool isAllDead(string map) */
         if(functionName == IA_CHARACTER_IS_ALL_DEAD)
         {
            modState modif;
            /* Make sure have npc file defined */
            if(s.find(".npc") == string::npos)
            {
               s += ".npc";
            }
            bl = modif.allCharactersDead(s);
         }
      }
      assignValue(var, (void*)&bl, IA_TYPE_BOOL);
   }

   /* Syntax: addEffect(character c, int mod, int time, int periodicTime,
    *                   string factorId, string factorType) */
   else if(functionName == IA_CHARACTER_ADD_EFFECT)
   {
      character* dude = NULL;
      int mod=0, time=0, periodicTime=0;
      string factorId="", factorType="";
      
      /* Get character */
      dude = getParameterc(token, strLine, pos);

      /* Get Values */
      mod = getParameteri(token, strLine, pos);
      time = getParameteri(token, strLine, pos);
      periodicTime = getParameteri(token, strLine, pos);
      factorId = getParameters(token, strLine, pos);
      factorType = getParameters(token, strLine, pos);

      /* Add the effect to the character */
      if(dude)
      {
         dude->addModEffect(mod, time, periodicTime, factorId, factorType);
      }
      else
      {
         cerr << "Error: Tried to access a NULL character at line " 
            << actualLine << " of the script: " << fileName << endl;
      }
   }

   /* Syntax void function(character c, int i) */
   else if(functionName == IA_CHARACTER_SET_PSYCHO)
   {
      character* dude = NULL;
      int psy = 0;

      /* Get character */
      dude = getParameterc(token, strLine, pos);

      /* Get psycho */
      psy = getParameteri(token, strLine, pos);

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
      character* c = getParameterc(token, strLine, pos);
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
   }

   /* syntax bool function(character ref, character target) */
   else if(functionName == IA_CHARACTER_AT_RANGE)
   {
      character* ref = NULL;
      character* tgt = NULL;

      /* get ref character */
      ref = getParameterc(token, strLine, pos);

      /* get target character */
      tgt = getParameterc(token, strLine, pos);

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
      dude = getParameterc(token, strLine, pos);

      /* Get Object FileName */
      objectFile = getParameters(token, strLine, pos);

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
      string charFile = getParameters(token, strLine, pos);

      /* Get Map FileName */
      string mapFile = getParameters(token, strLine, pos);

      /* Get Dialog Number */
      int dialogNum = getParameteri(token, strLine, pos);

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
   //                Particle Functions              //
   ////////////////////////////////////////////////////
   /* particleAddToCharacter */
   else if(functionName == IA_PARTICLE_ADD_TO_CHARACTER)
   {
      /* Syntax: void particleAddToCharacter(int partType, string fileName,
       *                                     character* c, int duration) */
      int partType=0;
      character* c;
      float posX=0, posY=0, posZ=0;
      string partFile="";
      int duration = 0;
      particleSystem* ps = NULL;
      partController particles;

      /* Get parameters */
      partType = getParameteri(token, strLine, pos);
      partFile = getParameters(token, strLine, pos);
      c = getParameterc(token, strLine, pos);
      if(c)
      {
         posX = c->xPosition;
         posY = c->max[1];
         posZ = c->zPosition;
      }
      duration = getParameteri(token, strLine, pos);

      /* Add system */
      ps = particles.addParticle(partType, posX, posY, posZ, partFile);
      if(ps != NULL)
      {
         ps->setFollowPC(true);
         ps->setDurationTime(duration);
      }
   }

   ////////////////////////////////////////////////////
   //                  Dice Functions                //
   ////////////////////////////////////////////////////
   /* void setBaseDice */
   else if(functionName == IA_SET_BASE_DICE)
   {
      diceThing* d = getParameterd(token, strLine, pos);
      string diceInfo = getParameters(token, strLine, pos);
      int dType = DICE_D2;
      int numDices = 0;
      int sum = 0;
      if(d)
      {
         sscanf(diceInfo.c_str(), "%d*d%d+%d", &numDices, &dType, &sum);
         d->baseDice.setType(dType);
         d->baseDice.setNumberOfDices(numDices);
         d->baseDice.setSumNumber(sum);
      }
   }
   /* void setAditionalDice */
   else if(functionName == IA_SET_ADITIONAL_DICE)
   {
      diceThing* d = getParameterd(token, strLine, pos);
      int multDices = getParameteri(token, strLine, pos);
      string diceInfo = getParameters(token, strLine, pos);
      int dType = DICE_D2;
      int numDices = 0;
      int sum = 0;
      if(d)
      {
         sscanf(diceInfo.c_str(), "%d*d%d+%d", &numDices, &dType, &sum);
         d->aditionalDice.setType(dType);
         d->aditionalDice.setNumberOfDices(numDices*multDices);
         d->aditionalDice.setSumNumber(sum);
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

   /* float function */
   else if( (functionName == IA_OWNER_HEIGHT) || 
            (functionName == IA_OWNER_POSX) ||
            (functionName == IA_OWNER_POSZ) )
   {
      if(var)
      {
         float v = 0.0;
         thing* t = NULL;

         /* Get the right pointer */
         if(objectOwner != NULL)
         {
            t = objectOwner;
         }
         else if(characterOwner != NULL)
         {
            t = characterOwner;
         }

         /* Get the desired value */
         if(t != NULL)
         {
            if(functionName == IA_OWNER_HEIGHT)
            {
               v = t->max[1];
            }
            else if(functionName == IA_OWNER_POSX)
            {
               v = t->xPosition;
            }
            else if(functionName == IA_OWNER_POSZ)
            {
               v = t->zPosition;
            }
         }

         /* Return it */
         assignValue(var, (void*)&v, IA_TYPE_FLOAT);
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
   unsigned int pos2 = 0;
   string token;
   string type; 

   /* get the variable */
   token = nextToken(strLine, pos);

   /* Verify else ifs */
   if(token == IA_SETENCE_ELSE)
   {
      pos2 = pos;
      token = nextToken(strLine, pos2);
      if(token == IA_SETENCE_IF)
      {
         /* So, must ignore the "else" AND the "if" */
         pos = pos2;
      }
   }


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

               if( (varPos == 1) && (token == IA_OPERATOR_SUBTRACTION) )
               {
                  /* Just change the variable signal */
                  if( (varStack[0]->type == IA_TYPE_FLOAT) ||
                      (varStack[0]->type == IA_TYPE_INT) )
                  {
                     varStack[0]->changeSignal();
                  }
                  else
                  {
                     cerr << "Error: operator '-' must be int or float" << endl;
                  }
               }
               else if(varPos < 2)
               {
                  cerr << "Error: operator '" << token 
                       << "' needs two variables" << endl;
               }
               else
               {
                  /* Pop the two variables */
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

