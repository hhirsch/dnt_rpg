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
   private:
      pattAgent* patAg;
      potentAgent* potAg;
};


#endif

