#include "mission.h"
#include "../engine/briefing.h"
#include "../engine/dccnit.h"
#include "../sound/sound.h"
#include "../etc/dirs.h"
#include "../etc/message3d.h"

#define MISSION_CONTROLLER_TOTAL_TREAT  5

/************************************************************
 *                        Constructor                       *
 ************************************************************/
mission::mission(string scriptFile, void* usedEngine):
         iaScript(scriptFile, usedEngine)
{
   int i;
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
void missionsController::init(void* usedEngine)
{
   totalCompleted = 0;
   completed = NULL;
   totalCurrent = 0;
   current = NULL;
   curTreat = NULL;
   pEngine = usedEngine;
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
   /* Empty current list */
   while(current)
   {
      removeFromCurrent(current, true);
   }

   /* Empty completed list */
   while(completed)
   {
      removeFromCompleted(completed);
   }

}

/************************************************************
 *                       addNewMission                      *
 ************************************************************/
void missionsController::addNewMission(string scriptFile)
{
   mission* m = new mission(scriptFile, pEngine);

   if(current)
   {
      m->next = current;
      m->previous = current->previous;
      m->next->previous = m;
      m->previous->next = m;
   }
   else
   {
      m->next = m;
      m->previous = m;
   }
   current = m;
   totalCurrent++;
}

/************************************************************
 *                      completeMission                     *
 ************************************************************/
void missionsController::completeMission(mission* m, int type)
{
   sound snd;
   briefing brief;
   messageController msgController;

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
      engine* eng = (engine*)pEngine;
      character* dude = eng->PCs->first->next;
      /* Add XP to all PC characters */
      while(dude != eng->PCs->first)
      {
         dude->xp += m->xpValue;
         dude = dude->next;
      }

      /* Add 3D Message of success */
      dude = eng->PCs->getActiveCharacter();
      char vstr[200];
      sprintf(vstr,gettext("Mission Completed: %d XP!"),m->getXp()); 
      msgController.addMessage(dude->xPosition, dude->max[1]+dude->yPosition,
                               dude->zPosition, vstr, 0.94, 0.8, 0.0);

      /* Add Briefing message of success */
      brief.addText(vstr);

      /* Play Completion Sound Effect */
      snd.addSoundEffect(false, "sndfx/missions/mission_complete.ogg");
   }
   else 
   {
      /*Add Briefing message of failure */
      brief.addText(gettext("Mission Failure!"));

      /* Play Failure Sound Effect */
      snd.addSoundEffect(false, "sndfx/missions/mission_failed.ogg");
   }
}

/************************************************************
 *                     removeFromCurrent                    *
 ************************************************************/
void missionsController::removeFromCurrent(mission* m, bool del)
{
   if(m)
   {
      if(current == m)
      {
         current = m->next;
      }

      m->next->previous = m->previous;
      m->previous->next = m->next;

      if(del)
      {
         delete(m);
      }

      totalCurrent--;
      if(totalCurrent <= 0)
      {
         current = NULL;
      }
   }
}


/************************************************************
 *                       addCompleted                       *
 ************************************************************/
void missionsController::addCompleted(mission* m)
{
   if(completed)
   {
      m->next = completed;
      m->previous = completed->previous;
      m->previous->next = m;
      m->next->previous = m;
   }
   else
   {
      m->next = m;
      m->previous = m;
   }
   completed  = m;
   totalCompleted++;
}

/************************************************************
 *                     removeFromCurrent                    *
 ************************************************************/
void missionsController::removeFromCompleted(mission* m)
{
   if(m)
   {
      if(completed == m)
      {
         completed = m->next;
      }

      m->next->previous = m->previous;
      m->previous->next = m->next;

      delete(m);

      totalCompleted--;
      if(totalCompleted <= 0)
      {
         completed = NULL;
      }
   }
}

/************************************************************
 *                    getCurrentMission                     *
 ************************************************************/
mission* missionsController::getCurrentMission(string scriptFile)
{
   int i;
   mission* m = current;
   for(i = 0; i < totalCurrent; i++)
   {
      if(m->fileName == scriptFile)
      {
         return(m);
      }
      m = m->next;
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
         curTreat = current;
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
         curTreat = curTreat->next;

         /* Verify if finished the script file. */
         if(curTreat->previous->finished())
         {
            completeMission(curTreat->previous, MISSION_COMPLETION_FINISHED);
         }
      }
   }
}

/*************************************************************************
 *                            Static Members                             *
 *************************************************************************/
mission* missionsController::completed = NULL;
mission* missionsController::curTreat = NULL;
int missionsController::totalCompleted = 0;
mission* missionsController::current = NULL;
int missionsController::totalCurrent = 0;   
void* missionsController::pEngine = NULL;

