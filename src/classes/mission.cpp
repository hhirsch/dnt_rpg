#include "mission.h"

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

///////////////////////////////////////////////////////////////////////
//                                                                   //
//                        MISSIONS CONTROLLER                        //
//                                                                   //
///////////////////////////////////////////////////////////////////////

/************************************************************
 *                        Constructor                       *
 ************************************************************/
missionsController::missionsController(void* usedEngine)
{
   totalCompleted = 0;
   completed = NULL;
   totalCurrent = 0;
   current = NULL;
   pEngine = usedEngine;
}

/************************************************************
 *                         Destructor                       *
 ************************************************************/
missionsController::~missionsController()
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
void missionsController::completeMission(mission* m)
{
   /* First, remove from the current list, without deleting it. */
   removeFromCurrent(m, false);

   /* So, insert at the completed list */
   addCompleted(m);
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
   totalCompleted--;
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
void missionsController::treat(Map* acMap)
{
   int i;
   int total = totalCurrent;
   mission* m = current;
   /* Only have to treat current missions */
   for(i=0; i < total; i++)
   {
      /* Run Script */
      m->defineMap(acMap);
      m->run(MAX_SCRIPT_LINES);
      m = m->next;

      /* Verify if finished */
      if(m->previous->finished())
      {
         m->previous->completed = MISSION_COMPLETION_FINISHED;
      }

      /* Verify completion */
      if(m->previous->completed != MISSION_COMPLETION_FALSE)
      {
         completeMission(m->previous);
      }
   }
}


