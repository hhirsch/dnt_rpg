#include "states.h"


stateMachine::stateMachine(int st)
{
   state = st;
   initialStateTime = 0;
}

stateMachine::~stateMachine()
{
   state = STATE_NONE;
   initialStateTime = 0;
}

int stateMachine::getActualState()
{
   return(state);
}

GLuint stateMachine::getTime()
{
   return(initialStateTime);
}

void stateMachine::setTime(GLuint time)
{
   initialStateTime = time;
}


void stateMachine::setState(int st, GLuint time)
{
   state = st;
   initialStateTime = time;
}

void stateMachine::nextState(GLuint time)
{
   switch(state)
   {
      case STATE_LOOK_OBJECT:
         state = STATE_GET_OBJECT;
      break;
      case STATE_GET_OBJECT:
         state = STATE_BRING_OBJECT;
      break;
      case STATE_BRING_OBJECT:
         state = STATE_LOOK_OBJECT;
      break;
      case STATE_BUSTED:
         state = STATE_INTERROGATION;
      break;
      case STATE_INTERROGATION:
         state = STATE_PRETEND;
      break;
      case STATE_PRETEND:
         state = STATE_LOOK_OBJECT;
      break;
      case STATE_WORK:
         state = STATE_LOOK_OBJECT;
      break;

      case STATE_PATROL:
         state = STATE_PATROL;
      break;
      case STATE_CATCH:
         state = STATE_SEND_PRISON;
      break;
      case STATE_SEND_PRISON:
         state = STATE_GO_PATROL;
      break; 
      case STATE_GO_PATROL:
         state = STATE_PATROL;
      break;
   }
}

