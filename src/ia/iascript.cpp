
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
   
                  else
                  {
                     /* Unknow! */
                     cerr << "Unknow token: " << token << "at line " 
                          << actualLine << "of the script file: " 
                          << fileName << endl;
                  }
               }
               else
               {
                  /* No Context defined! */
                  cerr << "No context defined at line: " << actualLine 
                       << "of script file: " << fileName 
                       << "did you forget the script() declaration?" << endl;
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
   while(name != "")
   {
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
                 << " at " << strLine << " on script:" << fileName << endl;
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
                 << " at " << strLine << " on script:" << fileName << endl;
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
                 << " at " << strLine << " on script:" << fileName << endl;
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
                 << " at " << strLine << " on script:" << fileName << endl;
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
                 << " at " << strLine << " on script:" << fileName << endl;
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
                 << " at " << strLine << " on script:" << fileName << endl;
         }
      }
   }

}

/***********************************************************************
 *                       evaluateExpression                            *
 ***********************************************************************/
void iaScript::evaluateExpression(iaVariable* var, string strLine)
{
   //TODO
}

