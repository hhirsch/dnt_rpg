#include "actions.h"

/********************************************************
 *                    numberAction                      *
 ********************************************************/
int numberAction(string buffer)
{
   int ret = -1;
   if(buffer.compare(ACT_STR_OPEN) == 0)
     ret = ACT_OPEN;
   else if(buffer.compare(ACT_STR_ATTACK) == 0)
     ret = ACT_ATTACK;
   else if(buffer.compare(ACT_STR_DRINK) == 0)
     ret = ACT_DRINK;
   else if(buffer.compare(ACT_STR_EAT) == 0)
     ret = ACT_EAT;
   else if(buffer.compare(ACT_STR_DEFEND) == 0)
     ret = ACT_DEFEND;
   else if(buffer.compare(ACT_STR_TALK) == 0)
     ret = ACT_TALK;
   else if(buffer.compare(ACT_STR_CLOSE) == 0)
     ret = ACT_CLOSE;
   else if(buffer.compare(ACT_STR_MOVE) == 0)
     ret = ACT_MOVE;
   else if(buffer.compare(ACT_STR_TAKE) == 0)
     ret = ACT_TAKE;
   else if(buffer.compare(ACT_STR_FREE) == 0)
     ret = ACT_FREE;
   else if(buffer.compare(ACT_STR_MODIFY_OWN_CONCEPT) == 0)
     ret = ACT_MODIFY_OWN_CONCEPT;
   else if(buffer.compare(ACT_STR_MODIFY_OTHER_CONCEPT) == 0)
     ret = ACT_MODIFY_OTHER_CONCEPT;
   else if(buffer.compare(ACT_STR_CONJURE) == 0)
     ret = ACT_CONJURE;
   else if(buffer.compare(ACT_STR_MODIFY_OBJECT) == 0)
     ret = ACT_MODIFY_OBJECT;
   else if(buffer.compare(ACT_STR_HEAL) == 0)
     ret = ACT_HEAL;
   else if(buffer.compare(ACT_STR_FIX) == 0)
     ret = ACT_FIX;
   else if(buffer.compare(ACT_STR_BREAK) == 0)
     ret = ACT_BREAK;

   return(ret);
}

/********************************************************
 *                  numberActionType                    *
 ********************************************************/
int numberActionType(string buffer)
{
   int ret = -1;

   if(buffer.compare(ACT_TYPE_STR_FREE_ACTION) == 0)
     ret = ACT_TYPE_FREE_ACTION;
   else if(buffer.compare(ACT_TYPE_STR_NORMAL_ACTION) == 0)
     ret = ACT_TYPE_NORMAL_ACTION;
   else if(buffer.compare(ACT_TYPE_STR_SECUNDARY_ACTION) == 0)
     ret = ACT_TYPE_SECUNDARY_ACTION;
   else if(buffer.compare(ACT_TYPE_STR_NO_ACTION) == 0)
     ret = ACT_TYPE_NO_ACTION;

   return(ret);
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//                                action                                   //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


/************************************************************
 *                        Constructor                       *
 ************************************************************/
action::action(string strLine, int type, character* act, thing* tgt)
{
   init(strLine, type, act, tgt, -1, -1);
}

/************************************************************
 *                        Constructor                       *
 ************************************************************/
action::action(string strLine, int type, character* act, thing* tgt,
               GLfloat tgtX, GLfloat tgtZ)
{
   init(strLine, type, act, tgt, tgtX, tgtZ);
}

/************************************************************
 *                        Constructor                       *
 ************************************************************/
action::action(string strLine, int type, character* act, 
               GLfloat tgtX, GLfloat tgtZ)
{
   init(strLine, type, act, NULL, tgtX, tgtZ);
}

/************************************************************
 *                           init                           *
 ************************************************************/
void action::init(string strLine, int type, character* act, thing* tgt,
                  GLfloat tgtX, GLfloat tgtZ)
{
   scriptLine = strLine;
   actionType = type;
   actor = act;
   target = tgt;
   targetX = tgtX;
   targetZ = tgtZ;
   next = NULL;
   previous = NULL;
}

/************************************************************
 *                         Destructor                       *
 ************************************************************/
action::~action()
{
}

/************************************************************
 *                         isRunning                        *
 ************************************************************/
bool action::isRunning()
{
   return(!done);
}

/************************************************************
 *                         setAsEnded                       *
 ************************************************************/
void action::setAsEnded(bool result)
{
   done = true;
   returnValue = result;
}

/************************************************************
 *                           getType                        *
 ************************************************************/
int action::getType()
{
   return(actionType);
}

/************************************************************
 *                          getActor                        *
 ************************************************************/
character* action::getActor()
{
   return(actor);
}

/************************************************************
 *                       getTargetThing                     *
 ************************************************************/
thing* action::getTargetThing()
{
   return(target);
}

/************************************************************
 *                      getTargetPosition                   *
 ************************************************************/
void action::getTargetPosition(GLfloat& x, GLfloat& z)
{
   x = targetX;
   z = targetZ;
}

/************************************************************
 *                       getReturnValue                     *
 ************************************************************/
bool action::getReturnValue()
{
   return(returnValue);
}

/************************************************************
 *                         getToggle                        *
 ************************************************************/
bool action::getToggle()
{
   return(toggle);
}

/************************************************************
 *                         setToggle                        *
 ************************************************************/
void action::setToggle(bool b)
{
   toggle = b;
}

/************************************************************
 *                       getScriptLine                      *
 ************************************************************/
string action::getScriptLine()
{
   return(scriptLine);
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//                           actionController                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


/************************************************************
 *                        Constructor                       *
 ************************************************************/
actionController::actionController()
{
   first = NULL;
   total = 0;
}

/************************************************************
 *                         Destructor                       *
 ************************************************************/
actionController::~actionController()
{
   removeAllActions();
}

/************************************************************
 *                       abortAllActions                    *
 ************************************************************/
void actionController::abortAllActions()
{
   int i;
   action* ac = first;
   for(i = 0; i < total; i++)
   {
      ac->setAsEnded(false);
      ac = ac->next;
   }
}

/************************************************************
 *                      removeAllActions                    *
 ************************************************************/
void actionController::removeAllActions()
{
   while(total > 0)
   {
      removeAction(first);
   }
}

/************************************************************
 *                        removeAction                      *
 ************************************************************/
void actionController::removeAction(action* act)
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
action* actionController::addAction(action* act)
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

      /* Do the related trigger action to the one defined */
      if(act->actionType == ACT_MOVE )
      {
         if(act->target == NULL)
         {
            /* It's a move to position */
            act->actor->pathFind.findPath(act->actor->xPosition, 
                                          act->actor->zPosition,
                                          act->targetX, act->targetZ, 
                                          act->actor->walk_interval, 
                                          act->actor->orientation,
                                          act->actor->min[0],
                                          act->actor->min[1],
                                          act->actor->min[2],
                                          act->actor->max[0],
                                          act->actor->max[1],
                                          act->actor->max[2]);
         }
         else
         {
            //FIXME: Move to a position not ON the target!

            /* Save the target value to move (to get changes of 
             *                                target position ) */
            act->targetX = act->target->xPosition;
            act->targetZ = act->target->zPosition;

            /* It's a move to a target */
            act->actor->pathFind.findPath(act->actor->xPosition, 
                                          act->actor->zPosition,
                                          act->target->xPosition - 
                                          ACT_MOVE_DELTA, 
                                          act->target->zPosition - 
                                          ACT_MOVE_DELTA, 
                                          act->actor->walk_interval, 
                                          act->actor->orientation,
                                          act->actor->min[0],
                                          act->actor->min[1],
                                          act->actor->min[2],
                                          act->actor->max[0],
                                          act->actor->max[1],
                                          act->actor->max[2]);
         }
      }
      else
      {
         //TODO
      }

   }
   return(act);
}

/************************************************************
 *                        addAction                         *
 ************************************************************/
action* actionController::addAction(string strLine, int type, character* act, 
                                    thing* tgt)
{
   action* a = new action(strLine, type, act, tgt);
   return(addAction(a));
}

/************************************************************
 *                        addAction                         *
 ************************************************************/
action* actionController::addAction(string strLine, int type, character* act, 
                                    GLfloat tgtX, GLfloat tgtZ)
{
   action* a = new action(strLine, type, act, tgtX, tgtZ);
   return(addAction(a));
}

/************************************************************
 *                        addAction                         *
 ************************************************************/
action* actionController::addAction(string strLine, int type, character* act, 
                                    thing* tgt, GLfloat tgtX, GLfloat tgtZ)
{
   action* a = new action(strLine, type, act, tgt, tgtX, tgtZ);
   return(addAction(a));
}

/************************************************************
 *                        getTotal                          *
 ************************************************************/
int actionController::getTotal()
{
   return(total);
}

/************************************************************
 *                        getFirst                          *
 ************************************************************/
action* actionController::getFirst()
{
   return(first);
}

/************************************************************
 *                      treatActions                        *
 ************************************************************/
void actionController::treatActions(Map* actualMap)
{
   int i;
   action* act = getFirst();

   for(i = 0; i < getTotal(); i++)
   {
      if(!act->isRunning())
      {
         //do nothing.
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
               act->actor->pathFind.findPath(act->actor->xPosition, 
                                          act->actor->zPosition,
                                          act->target->xPosition - 
                                          ACT_MOVE_DELTA, 
                                          act->target->zPosition - 
                                          ACT_MOVE_DELTA, 
                                          act->actor->walk_interval, 
                                          act->actor->orientation,
                                          act->actor->min[0],
                                          act->actor->min[1],
                                          act->actor->min[2],
                                          act->actor->max[0],
                                          act->actor->max[1],
                                          act->actor->max[2]);
            }
         }
         character* actor = act->getActor();
         if( (actor->isAlive()) && (!actor->isConversationOpened()) )
         {
            actor->setState(STATE_WALK);
            if(actor->pathFind.getState() == ASTAR_STATE_FOUND)
            {
               act->setToggle(true);
            }
            else if(actor->pathFind.getState() == ASTAR_STATE_NOT_FOUND)
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
                                                  actor->orientation))
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

