
#include "iascript.h"

/***********************************************************************
 *                           Constructor                               *
 ***********************************************************************/
iaScript::iaScript(string scriptFile)
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
 *                              run                                    *
 ***********************************************************************/
action* iaScript::run()
{
   bool done = false; /* Will be done when finish the file, 
                         or at pending action */
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
         }
      }
   }
   return(NULL);
}

/***********************************************************************
 *                            defineMap                                *
 ***********************************************************************/
void iaScript::defineMap(Map* acMap)
{
   actualMap = acMap;
}




