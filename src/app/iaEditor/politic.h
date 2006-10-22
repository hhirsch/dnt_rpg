#ifndef _politic_h
#define _politc_h

#include "../../ia/potentAgent.h"
#include "states.h"

class politic: potentAgent
{
   public:
     politic();
     ~politic();

     stateMachine* state;

};

#endif

