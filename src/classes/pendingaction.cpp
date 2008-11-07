#include "pendingaction.h"

#include "../engine/dialog.h"

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//                           pendingAction                                 //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


/************************************************************
 *                        Constructor                       *
 ************************************************************/
pendingAction::pendingAction(string strLine, int type, character* act, 
                             thing* tgt)
{
   init(strLine, type, act, tgt, -1, -1, -1);
}

/************************************************************
 *                        Constructor                       *
 ************************************************************/
pendingAction::pendingAction(string strLine, int type, character* act, 
                             thing* tgt, GLfloat tgtX, GLfloat tgtZ)
{
   init(strLine, type, act, tgt, tgtX, tgtZ, -1);
}

/************************************************************
 *                        Constructor                       *
 ************************************************************/
pendingAction::pendingAction(string strLine, int type, character* act, 
                             GLfloat tgtX, GLfloat tgtZ)
{
   init(strLine, type, act, NULL, tgtX, tgtZ, -1);
}

/************************************************************
 *                        Constructor                       *
 ************************************************************/
pendingAction::pendingAction(string strLine, int type, int v)
{
   init(strLine, type, NULL, NULL, -1, -1, v);
}

/************************************************************
 *                           init                           *
 ************************************************************/
void pendingAction::init(string strLine, int type, character* act, thing* tgt,
                         GLfloat tgtX, GLfloat tgtZ, int v)
{
   scriptLine = strLine;
   actionType = type;
   actor = act;
   target = tgt;
   targetX = tgtX;
   targetZ = tgtZ;
   next = NULL;
   previous = NULL;
   value = v;
   initedTime = SDL_GetTicks();
   missionAction = false;
}

/************************************************************
 *                         Destructor                       *
 ************************************************************/
pendingAction::~pendingAction()
{
}

/************************************************************
 *                         isRunning                        *
 ************************************************************/
bool pendingAction::isRunning()
{
   return(!done);
}

/************************************************************
 *                         setAsEnded                       *
 ************************************************************/
void pendingAction::setAsEnded(bool result)
{
   done = true;
   returnValue = result;
}

/************************************************************
 *                           getType                        *
 ************************************************************/
int pendingAction::getType()
{
   return(actionType);
}

/************************************************************
 *                          getActor                        *
 ************************************************************/
character* pendingAction::getActor()
{
   return(actor);
}

/************************************************************
 *                       getTargetThing                     *
 ************************************************************/
thing* pendingAction::getTargetThing()
{
   return(target);
}

/************************************************************
 *                      getTargetPosition                   *
 ************************************************************/
void pendingAction::getTargetPosition(GLfloat& x, GLfloat& z)
{
   x = targetX;
   z = targetZ;
}

/************************************************************
 *                       getReturnValue                     *
 ************************************************************/
bool pendingAction::getReturnValue()
{
   return(returnValue);
}

/************************************************************
 *                         getToggle                        *
 ************************************************************/
bool pendingAction::getToggle()
{
   return(toggle);
}

/************************************************************
 *                         setToggle                        *
 ************************************************************/
void pendingAction::setToggle(bool b)
{
   toggle = b;
}

/************************************************************
 *                    setAsMissionAction                    *
 ************************************************************/
void pendingAction::setAsMissionAction()
{
   missionAction = true;
}

/************************************************************
 *                      isMissionAction                     *
 ************************************************************/
bool pendingAction::isMissionAction()
{
   return(missionAction);
}


/************************************************************
 *                       getScriptLine                      *
 ************************************************************/
string pendingAction::getScriptLine()
{
   return(scriptLine);
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//                       pendingActionController                           //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


/************************************************************
 *                        Constructor                       *
 ************************************************************/
pendingActionController::pendingActionController()
{
   first = NULL;
   total = 0;
   NPCs = NULL;
   PCs = NULL;
}

/************************************************************
 *                         Destructor                       *
 ************************************************************/
pendingActionController::~pendingActionController()
{
   removeAllActions(true);
}

/************************************************************
 *                    setCharactersLists                    *
 ************************************************************/
void pendingActionController::setCharacterLists(characterList* npcs, 
                                                characterList* pcs)
{
   NPCs = npcs;
   PCs = pcs;
}

/************************************************************
 *                       abortAllActions                    *
 ************************************************************/
void pendingActionController::abortAllActions()
{
   int i;
   pendingAction* ac = first;
   for(i = 0; i < total; i++)
   {
      if(!ac->isMissionAction())
      {
         ac->setAsEnded(false);
         ac = ac->next;
      }
   }
}

/************************************************************
 *                      removeAllActions                    *
 ************************************************************/
void pendingActionController::removeAllActions(bool mission)
{
   int curTotal = total;
   int i;
   pendingAction* act = first;
   pendingAction* ract;

   /* Remove all Actions */
   for(i = 0; i < curTotal; i++)
   {
      ract = act;
      act = act->next;
      /* Only if is not a missionAction or 
       * if is to delete missions actions too */
      if( (mission) || (!ract->isMissionAction()))
      {
         removeAction(ract);
      }
   }
}

/************************************************************
 *                        removeAction                      *
 ************************************************************/
void pendingActionController::removeAction(pendingAction* act)
{
   act->next->previous = act->previous;
   act->previous->next = act->next;
   if(act == first)
   {
      first = act->next;
   }
   delete(act);
   total--;
   if(total <= 0)
   {
      first = NULL;
   }
}

/************************************************************
 *                        addAction                         *
 ************************************************************/
pendingAction* pendingActionController::addAction(pendingAction* act, 
                                                  bool mission)
{
   if(act)
   {
      if(first != NULL)
      {
         act->next = first;
         act->previous = first->previous;
      }
      else
      {
         act->next = act;
         act->previous = act;
      }
      act->next->previous = act;
      act->previous->next = act;
      act->done = false;
      act->toggle = false;
      first = act;
      total++;

      /* Do the related trigger pendingAction to the one defined */
      if(act->actionType == ACT_MOVE )
      {
         if(act->target == NULL)
         {
            /* It's a move to position */
            act->actor->pathFind.findPath(act->actor,
                                          act->targetX, act->targetZ, 
                                          act->actor->walk_interval, 
                                          NPCs, PCs, true);
         }
         else
         {
            //FIXME: When can't reach the random, try another near position!
            
            /* Do some randomize (current ticks + random number [0,1000) */
            srand(SDL_GetTicks() + (int)(1 + 1000 * 
                                         (rand() / (RAND_MAX + 1.0))));

            /* Save the target value to move (to get changes of 
             *                                target position ) */
            act->targetX = act->target->xPosition - 
                                           ((int)(20*rand()/(RAND_MAX+1.0))+30);
            act->targetZ = act->target->zPosition - 
                                           ((int)(20*rand()/(RAND_MAX+1.0))+30);
            
            /* It's a move to a target */
            act->actor->pathFind.findPath(act->actor,
                                          act->target->xPosition - 
                                          ACT_MOVE_DELTA, 
                                          act->target->zPosition - 
                                          ACT_MOVE_DELTA, 
                                          act->actor->walk_interval, 
                                          NPCs, PCs, true);
         }
      }
      else
      {
         //TODO
      }

   }

   /* Set as mission action, if needed */
   if(mission)
   {
      act->setAsMissionAction();
   }

   return(act);
}

/************************************************************
 *                        addAction                         *
 ************************************************************/
pendingAction* pendingActionController::addAction(string strLine, int type, 
                                                  character* act, thing* tgt,
                                                  bool mission)
{
   pendingAction* a = new pendingAction(strLine, type, act, tgt);
   return(addAction(a, mission));
}

/************************************************************
 *                        addAction                         *
 ************************************************************/
pendingAction* pendingActionController::addAction(string strLine, int type, 
                                                  character* act, 
                                                  GLfloat tgtX, GLfloat tgtZ,
                                                  bool mission)
{
   pendingAction* a = new pendingAction(strLine, type, act, tgtX, tgtZ);
   return(addAction(a, mission));
}

/************************************************************
 *                        addAction                         *
 ************************************************************/
pendingAction* pendingActionController::addAction(string strLine, int type, 
                                                  character* act, thing* tgt, 
                                                  GLfloat tgtX, GLfloat tgtZ,
                                                  bool mission)
{
   pendingAction* a = new pendingAction(strLine, type, act, tgt, tgtX, tgtZ);
   return(addAction(a, mission));
}

/************************************************************
 *                        addAction                         *
 ************************************************************/
pendingAction* pendingActionController::addAction(string strLine, int type, 
                                                  int v, bool mission)
{
   pendingAction* a = new pendingAction(strLine, type, v);
   return(addAction(a, mission));
}

/************************************************************
 *                        getTotal                          *
 ************************************************************/
int pendingActionController::getTotal()
{
   return(total);
}

/************************************************************
 *                        getFirst                          *
 ************************************************************/
pendingAction* pendingActionController::getFirst()
{
   return(first);
}

/************************************************************
 *                      treatActions                        *
 ************************************************************/
void pendingActionController::treatActions(Map* actualMap, bool fightMode)
{
   int i;
   pendingAction* act = getFirst();
   Uint32 curTime = SDL_GetTicks();
   dialogWindow dlgWindow;

   for(i = 0; i < getTotal(); i++)
   {
      if(!act->isRunning())
      {
         //do nothing.
      }
      else if(act->getType() == ACT_WAIT)
      {
         /* Verify if the wait period ended */
         if(curTime - act->initedTime >= (Uint32)act->value)
         {
            act->setAsEnded(true);
         }
      }
      else if(act->getType() == ACT_MOVE)
      {
         if(act->getTargetThing() != NULL)
         {
            /* Verify if the target moves */
            thing* tgt = act->getTargetThing();
            GLfloat tX = 0, tZ = 0;
            act->getTargetPosition(tX, tZ);
            if( ( (tgt->xPosition <= tX - ACT_MOVE_DELTA) || 
                  (tgt->xPosition >= tX + ACT_MOVE_DELTA) ) || 
                ( (tgt->zPosition <= tZ - ACT_MOVE_DELTA) || 
                  (tgt->zPosition >= tZ + ACT_MOVE_DELTA) ) )
            {
               /* Rerun the pathFind */
               act->targetX = tgt->xPosition;
               act->targetZ = tgt->zPosition;

               act->setToggle(false);
               act->actor->pathFind.forceNextCall();

               //FIXME Move not ON character but TO character
               act->actor->pathFind.findPath(act->actor,
                                             act->target->xPosition - 
                                             ACT_MOVE_DELTA, 
                                             act->target->zPosition - 
                                             ACT_MOVE_DELTA, 
                                             act->actor->walk_interval, 
                                             NPCs, PCs, fightMode);
            }
         }
         character* actor = act->getActor();
         if( (actor->isAlive()) && 
             (!dlgWindow.isOpened((conversation*)actor->getConversation())))
         {
            int state = actor->pathFind.getState();
            if(state == ASTAR_STATE_FOUND)
            {
               actor->setState(STATE_WALK);
               act->setToggle(true);
            }
            else if(state == ASTAR_STATE_NOT_FOUND)
            {
               /* The move ended, since not found a path */
               act->setAsEnded(false);
               actor->setState(STATE_IDLE);
            }

            /* If the toggle is seted, the path was found */
            if(act->getToggle())
            {
               if(!actor->pathFind.getNewPosition(actor->xPosition,
                                                  actor->zPosition,
                                                  actor->orientation,
                                                  fightMode))
               {  
                  /* The move ended */
                  act->setAsEnded(true);
                  actor->setState(STATE_IDLE);
               }
               else
               {
                  /* Define New Occuped Square */
                  int posX =(int)floor(actor->xPosition / 
                                       actualMap->squareSize());
                  int posZ =(int)floor(actor->zPosition / 
                                       actualMap->squareSize());
                  actor->ocSquare = actualMap->relativeSquare(posX,posZ);

                  /* Define New Height */
                  actualMap->defineThingHeight(actor, actor->xPosition,
                                               actor->zPosition);
               }
            }
         }
         else if(!actor->isAlive())
         {
            /* Dead Characters can't walk. */
            act->setAsEnded(false);
         }
         else
         {
            /* Talk window is opened, so Idle. */
            actor->setState(STATE_IDLE);
         }
      }
      else
      {
         //TODO
      }
      act = act->next;
   }
}

