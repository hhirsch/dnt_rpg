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
   else if(buffer.compare(ACT_STR_CURE) == 0)
     ret = ACT_CURE;
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
      first = act;
      total++;
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


