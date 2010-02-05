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
   value = v;
   initedTime = SDL_GetTicks();
   missionAction = false;
   done = false;
   toggle = false;
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

#ifdef DNT_DEBUG_PENDING_ACTION
   cerr << "Action: set as ended with result: " << result << endl;
#endif
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
#ifdef DNT_DEBUG_PENDING_ACTION
   cerr << "setToggle: " << b << endl;
#endif
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
//                           pendingActionList                             //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/************************************************************
 *                        Constructor                       *
 ************************************************************/
pendingActionList::pendingActionList()
{
}

/************************************************************
 *                         Destructor                       *
 ************************************************************/
pendingActionList::~pendingActionList()
{
   clearList();
}

/************************************************************
 *                        freeElement                       *
 ************************************************************/
void pendingActionList::freeElement(dntListElement* obj)
{
   pendingAction* pa = (pendingAction*)obj;
   delete(pa);
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
   NPCs = NULL;
   PCs = NULL;
}

/************************************************************
 *                         Destructor                       *
 ************************************************************/
pendingActionController::~pendingActionController()
{
   removeAllActions(true);
   actions.clearList();
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
   pendingAction* ac = (pendingAction*)actions.getFirst();
   for(i = 0; i < actions.getTotal(); i++)
   {
      if(!ac->isMissionAction())
      {
         ac->setAsEnded(false);
         ac = (pendingAction*)ac->getNext();
      }
   }
}

/************************************************************
 *                      removeAllActions                    *
 ************************************************************/
void pendingActionController::removeAllActions(bool mission)
{
   int curTotal = actions.getTotal();
   int i;
   pendingAction* act = (pendingAction*)actions.getFirst();
   pendingAction* ract;

   /* Remove all Actions */
   for(i = 0; i < curTotal; i++)
   {
      ract = act;
      act = (pendingAction*)act->getNext();
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
   /* Remove from list */
   actions.remove(act);
}

/************************************************************
 *                        addAction                         *
 ************************************************************/
pendingAction* pendingActionController::addAction(pendingAction* act, 
                                                  bool mission)
{
   if(act)
   {
      /* Insert on list */
      actions.insert(act);

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
            bool signalX = ((rand()/(RAND_MAX+1.0)) > 0.5);
            GLfloat varX = (int)((10 * (rand()/(RAND_MAX+1.0)))+10);

            bool signalZ = ((rand()/(RAND_MAX+1.0)) > 0.5);
            GLfloat varZ = (int)((10 * (rand()/(RAND_MAX+1.0)))+10);

            /* Apply X variation */
            if(signalX)
            {
               act->targetX = act->target->xPosition + varX;
            }
            else
            {
               act->targetX = act->target->xPosition - varX;
            }

            /* Apply X variation */
            if(signalZ)
            {
               act->targetZ = act->target->zPosition + varZ;
            }
            else
            {
               act->targetZ = act->target->zPosition - varZ;
            }

            /* It's a move to a target */
            act->actor->pathFind.findPath(act->actor,
                                          act->targetX, 
                                          act->targetZ, 
                                          act->actor->walk_interval, 
                                          NPCs, PCs, true);
         }
      }
      else if(act->actionType == ACT_WAIT)
      {
         /* Nothing! */
      }
      else
      {
         /* TODO */
         cerr << "Warning: Pending action of type '" << act->actionType 
              << "' not yet implemented!" << endl;
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
   return(actions.getTotal());
}

/************************************************************
 *                        getFirst                          *
 ************************************************************/
pendingAction* pendingActionController::getFirst()
{
   return((pendingAction*)actions.getFirst());
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

#ifdef DNT_DEBUG_PENDING_ACTION
   cerr << "treatActions: total = " << getTotal() << endl;
#endif

   for(i = 0; i < getTotal(); i++)
   {
      if(!act->isRunning())
      {
         /* do nothing. */ 
#ifdef DNT_DEBUG_PENDING_ACTION
         cerr << "Warning: Actions isn't running!" << endl;
#endif
      }
      else if(act->getType() == ACT_WAIT)
      {
#ifdef DNT_DEBUG_PENDING_ACTION
         cerr << "Pending Actions: ACT_WAIT" << endl;
#endif
         /* Verify if the wait period ended */
         if(curTime - act->initedTime >= (Uint32)act->value)
         {
            act->setAsEnded(true);
         }
      }
      else if(act->getType() == ACT_MOVE)
      {
#ifdef DNT_DEBUG_PENDING_ACTION
         cerr << "Pending Action: ACT_MOVE" << endl;
#endif
         if(act->getTargetThing() != NULL)
         {
            /* TODO Verify if the target moves */
#ifdef DNT_DEBUG_PENDING_ACTION
            cerr << "Move to a target!" << endl;
#endif
         }

         character* actor = act->getActor();
         if( (actor->isAlive()) && 
             (!dlgWindow.isOpened((conversation*)actor->getConversation())))
         {
            int state = actor->pathFind.getState();
#ifdef DNT_DEBUG_PENDING_ACTION
            cerr << "pathFind state: " << state << endl;
#endif
            if(state == ASTAR_STATE_FOUND)
            {
#ifdef DNT_DEBUG_PENDING_ACTION
               cerr << "ASTAR_FOUND!" << endl;
#endif
               actor->setState(STATE_WALK);
               act->setToggle(true);
            }
            else if(state == ASTAR_STATE_NOT_FOUND)
            {
#ifdef DNT_DEBUG_PENDING_ACTION
               cerr << "ASTAR_NOT_FOUND!" << endl;
#endif
               /* The move ended, since not found a path */
               act->setAsEnded(false);
               actor->setState(STATE_IDLE);
            }

            /* If the toggle is seted, the path was found */
            if(act->getToggle())
            {
#ifdef DNT_DEBUG_PENDING_ACTION
               cerr << "Toggle: true!" << endl;
#endif
               if(!actor->pathFind.getNewPosition(actor->xPosition,
                                                  actor->zPosition,
                                                  actor->orientation,
                                                  fightMode, false))
               {
#ifdef DNT_DEBUG_PENDING_ACTION
                  cerr << "No new position. Move ended!" << endl;
#endif
                  /* The move ended */
                  act->setAsEnded(true);
                  actor->setState(STATE_IDLE);
               }
               else
               {
#ifdef DNT_DEBUG_PENDING_ACTION
                  cerr << "New position got." << endl;
#endif
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
#ifdef DNT_DEBUG_PENDING_ACTION
            cerr << "actor is Dead!" << endl;
#endif
            /* Dead Characters can't walk. */
            act->setAsEnded(false);
         }
         else
         {
#ifdef DNT_DEBUG_PENDING_ACTION
            cerr << "Talk is opened!" << endl;
#endif
            /* Talk window is opened, so Idle. */
            actor->setState(STATE_IDLE);
         }
      }
      else
      {
         //TODO
      }
      act = (pendingAction*)act->getNext();
   }
}

