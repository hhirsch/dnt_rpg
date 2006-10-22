#include "states.h"


stateMachine::stateMachine(int st)
{
   state = st;
   initialStateTime = 0;
}

stateMachine::~stateMachine()
{
   state = STATE_NONE;
   initialStateTime = -1;
}

int stateMachine::getActualState()
{
   return(state);
}

int stateMachine::getTime()
{
   return(initialStateTime);
}

void stateMachine::setState(int st, int time)
{
   state = st;
   initialStateTime = time;
}

void stateMachine::nextState(int time)
{
   switch(state)
   {
      case STATE_LOOK_OBJECT:
      break;
      case STATE_GET_OBJECT:
      break;
      case STATE_BRING_OBJECT:
      break;
      case STATE_BUSTED:
      break;
      case STATE_INTERROGATION:
      break;
      case STATE_PRETEND:
      break;
      case STATE_WORK:
      break;

      case STATE_PATROL:
      break;
      case STATE_CATCH:
      break;
      case STATE_SEND_PRISON:
      break; 
   }
}

