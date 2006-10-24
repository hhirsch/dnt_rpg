#ifndef _pf_h
#define _pf_h

#include "../../ia/potentAgent.h"
#include "../../ia/pattAgent.h"
#include "states.h"

class pf
{
   public:
      pf();
      ~pf();

      stateMachine* state;
      pattAgent* patAg;
      potentAgent* potAg;
      pf* next;
};


#endif

