#include "politic.h"


politic::politic():potentAgent(false)
{
   state = new stateMachine(STATE_LOOK_OBJECT);
}

politic::~politic()
{
   delete(state);
}

