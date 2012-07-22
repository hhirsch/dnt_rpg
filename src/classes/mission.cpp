/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mission.h"
#include "../engine/briefing.h"
#include "../engine/dccnit.h"
#include "../sound/sound.h"
#include "../etc/dirs.h"
#include "../etc/message3d.h"
#include "../lang/translate.h"
using namespace std;

#define MISSION_TOKEN_CURRENT_MISSION    "currentMission"
#define MISSION_TOKEN_XP_VALUE           "xpValue"
#define MISSION_TOKEN_TEMP_FLAG          "tempFlag"
#define MISSION_TOKEN_DESCRIPTION        "description"
#define MISSION_TOKEN_AREA               "area"
#define MISSION_TOKEN_COMPLETION_SCRIPT  "completionScript"
#define MISSION_TOKEN_FAILURE_SCRIPT     "failureScript"

#define MISSION_TOKEN_END_MISSION        "endMission"

#define MISSION_TOKEN_COMPLETED_MISSION  "completedMission"
#define MISSION_TOKEN_COMPLETED          "completed"

#define MISSION_TOKEN_ACTUAL_LINE        "actualLine"
#define MISSION_TOKEN_LAST_POS           "lastPos"
#define MISSION_TOKEN_CONTEXT            "context"
#define MISSION_TOKEN_SYMBOLS_TABLE      "symbolsTable"
#define MISSION_TOKEN_STACK              "stack"

#define MISSION_CONTROLLER_TOTAL_TREAT  5

/************************************************************
 *                        Constructor                       *
 ************************************************************/
mission::mission(string missionFile, engine* usedEngine, 
                 bool loadDefinition):
         iaScript(usedEngine)
{
   int i;
   defParser def;
   string key, value;

   /* Clear Values */
   description = "";
   area = "";
   completionScript = "";
   failureScript = "";

   /* Read the information from the definition's file */
   if(loadDefinition)
   {
      def.load(missionFile);
      while(def.getNextTuple(key, value))
      {
         if(key == "script")
         {
            fileName = value;
         }
         else if(key == MISSION_TOKEN_DESCRIPTION)
         {
            description = translateDataString(value);
         }
         else if(key == MISSION_TOKEN_AREA)
         {
            area = value;
         }
         else if(key == MISSION_TOKEN_COMPLETION_SCRIPT)
         {
            completionScript = value;
         }
         else if(key == MISSION_TOKEN_FAILURE_SCRIPT)
         {
            failureScript = value;
         }
      }
   }
   else
   {
      /* Is loading directly from the script,
       * usually at loading a saved game. */
      fileName = missionFile;
   }

   /* Call the init for the iaScript! */
   init();

   /* Ddefine Mission Related things */
   next = NULL;
   previous = NULL;
   type = IASCRIPT_TYPE_MISSION;
   completed = 0;
   for(i=0; i < MISSION_TEMP_FLAGS; i++)
   {
      tempFlag[i] = 0;
   }
}

/************************************************************
 *                         Destructor                       *
 ************************************************************/
mission::~mission()
{
}

/************************************************************
 *                       getCompletion                      *
 ************************************************************/
int mission::getCompletion()
{
   return(completed);
}

/************************************************************
 *                        getTempFlag                       *
 ************************************************************/
int mission::getTempFlag(int flagNumber)
{
   if((flagNumber >= 0) && (flagNumber < MISSION_TEMP_FLAGS))
   {
      return(tempFlag[flagNumber]);
   }
   
   return(0);
}

/************************************************************
 *                        setTempFlag                       *
 ************************************************************/
void mission::setTempFlag(int flagNumber, int flagValue)
{
   if((flagNumber >= 0) && (flagNumber < MISSION_TEMP_FLAGS))
   {
      tempFlag[flagNumber] = flagValue;
   }
}

/************************************************************
 *                           getXp                          *
 ************************************************************/
int mission::getXp()
{
   return(xpValue);
}

/************************************************************
 *                           setXp                          *
 ************************************************************/
void mission::setXp(int xp)
{
   xpValue = xp;
}

/************************************************************
 *                        getDescription                    *
 ************************************************************/
string mission::getDescription()
{
   return(description);
}

/************************************************************
 *                        setDescription                    *
 ************************************************************/
void mission::setDescription(string desc)
{
   description = desc;
}

/************************************************************
 *                        getArea                           *
 ************************************************************/
string mission::getArea()
{
   return(area);
}

/************************************************************
 *                        setArea                           *
 ************************************************************/
void mission::setArea(string a)
{
   area = a;
}

/************************************************************
 *                      saveAsCompleted                     *
 ************************************************************/
void mission::saveAsCompleted(ofstream* file)
{
   /* Filename as completed */
   *file << MISSION_TOKEN_COMPLETED_MISSION << " = " << fileName << endl;

   /* Completed mission things */
   *file << MISSION_TOKEN_COMPLETED << " = " << completed << endl;

   /* Save Description */
   *file << MISSION_TOKEN_DESCRIPTION << " = " << description << endl;

   /* Save area Info */
   *file << MISSION_TOKEN_AREA << " = " << area << endl;

   /* Mark End */
   *file << MISSION_TOKEN_END_MISSION << " = " << fileName << endl;
}

/************************************************************
 *                      loadAsCompleted                     *
 ************************************************************/
void mission::loadAsCompleted(defParser* def)
{
   string key="", value="";

    /* Define type */
   type = IASCRIPT_TYPE_MISSION;

   /* parse until file end or mission definition's end */
   while(def->getNextTuple(key, value))
   {
      if(key == MISSION_TOKEN_END_MISSION)
      {
         /* Done loading this mission */
         return;
      }
      else if(key == MISSION_TOKEN_COMPLETED)
      {
         /* Get the completed state */
         sscanf(value.c_str(),"%d", &completed);
      }
      else if(key == MISSION_TOKEN_DESCRIPTION)
      {
         /* Set the description */
         description = value;
      }
      else if(key == MISSION_TOKEN_AREA)
      {
         /* Set the area */
         area = value;
      }
      else
      {
         cerr << "Unexpected token '" << key <<"' while loading mission: " 
              << fileName << endl;
      }
   }
}


/************************************************************
 *                       saveAsCurrent                      *
 ************************************************************/
void mission::saveAsCurrent(ofstream* file)
{
   int i;

   /* Mission file as Current */
   *file << MISSION_TOKEN_CURRENT_MISSION << " = " << fileName << endl;

   /* Save Mission Related Things */
   *file << MISSION_TOKEN_XP_VALUE << " = " << xpValue << endl;

   for(i = 0; i < MISSION_TEMP_FLAGS; i++)
   {
      *file << MISSION_TOKEN_TEMP_FLAG << " = " << tempFlag[i] << endl;
   }

   /* Save Description */
   *file << MISSION_TOKEN_DESCRIPTION << " = " << description << endl;

   /* Save area Info */
   *file << MISSION_TOKEN_AREA << " = " << area << endl;

   /* Save Script Related Things */

   /* Actual Line */
   *file << MISSION_TOKEN_ACTUAL_LINE << " = " << actualLine << endl;

   /* Last Pos */
   *file << MISSION_TOKEN_LAST_POS << " = " << lastPos << endl;

   /* Current Context */
   *file << MISSION_TOKEN_CONTEXT << " = " << context << endl;

   /* the symbols table */
   *file << MISSION_TOKEN_SYMBOLS_TABLE << " = BEGIN " << endl;
   symbols->save(file);

   /* the stack */
   *file << MISSION_TOKEN_STACK << " = BEGIN " << endl;
   jumpStack->save(file);

   //TODO: FIXME: Pending Action!
   

   /* Mark End */
   *file << MISSION_TOKEN_END_MISSION << " = " << fileName << endl;
}

/************************************************************
 *                       loadAsCurrent                      *
 ************************************************************/
void mission::loadAsCurrent(defParser* def)
{
   string key="", value="";
   int flagCount = 0;
   bool done = false;

   /* Define type */
   type = IASCRIPT_TYPE_MISSION;

   /* Load Things from the parser */
   while(!done)
   {
      done |= !def->getNextTuple(key, value);

      if(!done)
      {
         /* Mission Declaration Ended */
         if(key == MISSION_TOKEN_END_MISSION)
         {
            /* So, no more parser here! */
            done = true;
         }
         /* Xp Value */
         else if(key == MISSION_TOKEN_XP_VALUE)
         {
            sscanf(value.c_str(),"%d",&xpValue);
         }
         /* Temporary Flags */
         else if(key == MISSION_TOKEN_TEMP_FLAG)
         {
            if(flagCount < MISSION_TEMP_FLAGS)
            {
               sscanf(value.c_str(),"%d", &tempFlag[flagCount]);
               flagCount++;
            }
            else
            {
               cerr << "Warning: Temporary Flags Overflow for: " << fileName 
                    << endl;
            }
         }
         else if(key == MISSION_TOKEN_DESCRIPTION)
         {
            /* Set the description */
            description = value;
         }
         else if(key == MISSION_TOKEN_AREA)
         {
            /* Set the area */
            area = value;
         }
         /* Actual Line */
         else if(key == MISSION_TOKEN_ACTUAL_LINE)
         {
            sscanf(value.c_str(),"%d",&actualLine);
         }
         /* Last Pos */
         else if(key == MISSION_TOKEN_LAST_POS)
         {
            int lp=0;
            sscanf(value.c_str(),"%d",&lp);
            lastPos = lp;
         }
         /* Context */
         else if(key == MISSION_TOKEN_CONTEXT)
         {
            context = value;
         }
         /* Symbols Table */
         else if(key == MISSION_TOKEN_SYMBOLS_TABLE)
         {
            symbols->load(def, actualEngine);
         }
         /* Stack */
         else if(key == MISSION_TOKEN_STACK)
         {
            jumpStack->load(def);
         }
         /* Unknow! */
         else
         {
            cerr << "Error: Unknow Token: '" << key << "' loading mission save!"
                 << endl;
         }
      }
   }

   /* Now read the script file to the actual line */
   int i;
   string strBuffer;
   streampos pos;
   for(i = 0; i < actualLine; i++)
   {
      pos = file.tellg();
      getline(file, strBuffer);
   }

}

///////////////////////////////////////////////////////////////////////
//                                                                   //
//                            missionList                            //
//                                                                   //
///////////////////////////////////////////////////////////////////////

/************************************************************
 *                        Constructor                       *
 ************************************************************/
missionList::missionList()
{
}

/************************************************************
 *                         Destructor                       *
 ************************************************************/
missionList::~missionList()
{
   clearList();
}

/************************************************************
 *                          freeElement                     *
 ************************************************************/
void missionList::freeElement(dntListElement* obj)
{
   mission* m = (mission*)obj;
   delete(m);
}


///////////////////////////////////////////////////////////////////////
//                                                                   //
//                        MISSIONS CONTROLLER                        //
//                                                                   //
///////////////////////////////////////////////////////////////////////

/************************************************************
 *                        Constructor                       *
 ************************************************************/
missionsController::missionsController()
{
}

/************************************************************
 *                           init                           *
 ************************************************************/
void missionsController::init(engine* usedEngine)
{
   curTreat = NULL;
   pEngine = usedEngine;

   current = new missionList();
   completed = new missionList();
}

/************************************************************
 *                         Destructor                       *
 ************************************************************/
missionsController::~missionsController()
{
}

/************************************************************
 *                           finish                         *
 ************************************************************/
void missionsController::finish()
{
   delete(current);
   delete(completed);
}

/************************************************************
 *                       addNewMission                      *
 ************************************************************/
void missionsController::addNewMission(string missionFile)
{
   mission* m = new mission(missionFile, pEngine, true);

   addCurrent(m);
}

/************************************************************
 *                      completeMission                     *
 ************************************************************/
void missionsController::completeMission(mission* m, int type)
{
   sound snd;
   briefing brief;
   messageController msgController;
   char vstr[200];
   int i;

   /* First, remove from the current list, without deleting it. */
   removeFromCurrent(m, false);

   /* So, insert at the completed list */
   addCompleted(m);

   /* Close the script, since it won't run anymore */
   m->close();
   m->completed = type;

   /* Verify if it isn't the curTreat Mission */
   if(m == curTreat)
   {
      curTreat = NULL;
   }

   /* type <= 0 means failed. > 0 success */
   if(type > 0)
   {
      /* Call the completion script, if any */
      if(!m->completionScript.empty())
      {
         iaScript* sc = new iaScript(m->completionScript, pEngine);
         sc->defineMap( pEngine->getCurrentMap(), pEngine->NPCs);
         sc->run(0);
         delete(sc);
      }

      character* dude = (character*)pEngine->PCs->getFirst();
      
      /* Add XP to all PC characters */
      for(i = 0; i < pEngine->PCs->getTotal(); i++)
      {
         dude->addXP(m->xpValue);
         dude = (character*)dude->getNext();
      }

      /* Add 3D Message of success */
      dude = pEngine->PCs->getActiveCharacter();

      sprintf(vstr,gettext("Mission Completed: %d XP!"),m->getXp()); 
      msgController.addMessage(dude->scNode->getPosX(), 
            dude->scNode->getBoundingBox().max.y+dude->scNode->getPosY(),
            dude->scNode->getPosZ(), vstr, 0.94, 0.8, 0.0);

      /* Add Briefing message of success */
      brief.addText(vstr);

      /* Play Completion Sound Effect */
      snd.addSoundEffect(SOUND_NO_LOOP, "sndfx/missions/mission_complete.ogg");
   }
   else 
   {
      /* Call the failure script, if any */
      if(!m->failureScript.empty())
      {
         iaScript* sc = new iaScript(m->failureScript, pEngine);
         sc->defineMap( pEngine->getCurrentMap(), pEngine->NPCs);
         sc->run(0);
         delete(sc);
      }

      /* Add 3D message of failure */
      character* dude = pEngine->PCs->getActiveCharacter();
      msgController.addMessage(dude->scNode->getPosX(), 
            dude->scNode->getBoundingBox().max.y+dude->scNode->getPosY(),
            dude->scNode->getPosZ(), 
            gettext("Mission Failure!"), 0.94, 0.2, 0.1);

      /* Add Briefing message of failure */
      brief.addText(gettext("Mission Failure!"));

      /* Play Failure Sound Effect */
      snd.addSoundEffect(SOUND_NO_LOOP, "sndfx/missions/mission_failed.ogg");
   }
}

/************************************************************
 *                     removeFromCurrent                    *
 ************************************************************/
void missionsController::removeFromCurrent(mission* m, bool del)
{
   if(del)
   {
      current->remove(m);
   }
   else
   {
      current->removeWithoutDelete(m);
   }
}

/************************************************************
 *                        addCurrent                        *
 ************************************************************/
void missionsController::addCurrent(mission* m)
{
   current->insert(m);
}

/************************************************************
 *                       addCompleted                       *
 ************************************************************/
void missionsController::addCompleted(mission* m)
{
   completed->insert(m);
}

/************************************************************
 *                     removeFromCurrent                    *
 ************************************************************/
void missionsController::removeFromCompleted(mission* m)
{
   completed->remove(m);
}

/************************************************************
 *                    getCurrentMission                     *
 ************************************************************/
mission* missionsController::getCurrentMission(string scriptFile)
{
   int i;
   mission* m = (mission*)current->getFirst();
   for(i = 0; i < current->getTotal(); i++)
   {
      if(m->fileName == scriptFile)
      {
         return(m);
      }
      m = (mission*)m->getNext();
   }
   return(NULL);
}

/************************************************************
 *                           Treat                          *
 ************************************************************/
void missionsController::treat(Map* acMap, characterList* NPCs)
{
   int i;

   /* Only have to treat current missions */
   for(i=0; i < MISSION_CONTROLLER_TOTAL_TREAT; i++)
   {
      /* Set the curTreat, if needed */
      if(!curTreat)
      {
         curTreat = (mission*)current->getFirst();
         if(!curTreat)
         {
            /* No current missions -> nothing to do! */
            return;
         }
      }

      /* Run Script */
      curTreat->defineMap(acMap, NPCs);
      curTreat->run(MAX_SCRIPT_LINES);
   
      /* Since it can be completed at run with complete mission function */
      if(curTreat)
      {
         curTreat = (mission*)curTreat->getNext();

         /* Verify if finished the script file. */
         if(((mission*)curTreat->getPrevious())->finished())
         {
            completeMission((mission*)curTreat->getPrevious(),
                  MISSION_COMPLETION_FINISHED);
         }
      }
   }
}

/************************************************************
 *                            save                          *
 ************************************************************/
bool missionsController::save(string fName)
{
   ofstream f;
   int i;
   mission* m;

   /* Create the file */
   f.open(fName.c_str(), ios::out | ios::binary);
   if(!f)
   {
      cerr << "Can't save file: " << fName << endl;
      return(false);
   }

   /* Call to save each completed mission */
   m = (mission*)completed->getFirst();
   for(i = 0; i < completed->getTotal(); i++)
   {
      m->saveAsCompleted(&f);
      m = (mission*)m->getNext();
   }

   /* And Call to save each current mission */
   m = (mission*)current->getFirst();
   for(i = 0; i < current->getTotal(); i++)
   {
      m->saveAsCurrent(&f);
      m = (mission*)m->getNext();
   }

   /* Close the file and done! */
   f.close();
   return(true);
}

/************************************************************
 *                            load                          *
 ************************************************************/
bool missionsController::load(string fName)
{
   defParser def;
   string key, value;
   mission* m = NULL;

   /* Clear Current */
   finish();
   init(pEngine); 

   /* Try to open the missions file */
   if(!def.load(fName))
   {
      cerr << "Can't open missions file: " << fName << endl;
      return(false);
   }

   /* Now parse the file */
   while(def.getNextTuple(key, value))
   {
      if(key == MISSION_TOKEN_CURRENT_MISSION)
      {
         /* Create the new mission */
         m = new mission(value, pEngine, false);

         /* Insert as current */
         addCurrent(m);

         /* Load as current */
         m->loadAsCurrent(&def);
      }
      else if(key == MISSION_TOKEN_COMPLETED_MISSION)
      {
         /* Create new mission */
         m = new mission(value, pEngine, false);

         /* Insert as completed */
         addCompleted(m);

         /* Load as completed */
         m->loadAsCompleted(&def);
      }
      else
      {
         cerr << "Unexpected token '" << key << "' at file " << fName << endl;
      }
   }

   return(true);
}

/************************************************************
 *                    getFirstCurrentMission                *
 ************************************************************/
mission* missionsController::getFirstCurrentMission()
{
   curCur = (mission*)current->getFirst();
   return(curCur);
}

/************************************************************
 *                    getNextCurrentMission                 *
 ************************************************************/
mission* missionsController::getNextCurrentMission()
{
   if(curCur)
   {
      curCur = (mission*)curCur->getNext();
      if(curCur != (mission*)current->getFirst())
      {
         return(curCur);
      }
   }

   curCur = NULL;
   return(NULL);
}

/************************************************************
 *                   getFirstCompletedMission               *
 ************************************************************/
mission* missionsController::getFirstCompletedMission()
{
   curComp = (mission*)completed->getFirst();
   return(curComp);
}

/************************************************************
 *                    getNextCompletedMission               *
 ************************************************************/
mission* missionsController::getNextCompletedMission()
{
   if(curComp)
   {
      curComp = (mission*)curComp->getNext();
      if(curComp != (mission*)completed->getFirst())
      {
         return(curComp);
      }
   }

   curComp = NULL;
   return(NULL);
}


/*************************************************************************
 *                            Static Members                             *
 *************************************************************************/
missionList* missionsController::completed = NULL;
mission* missionsController::curTreat = NULL;
missionList* missionsController::current = NULL;
mission* missionsController::curComp = NULL;
mission* missionsController::curCur = NULL;
engine* missionsController::pEngine = NULL;

