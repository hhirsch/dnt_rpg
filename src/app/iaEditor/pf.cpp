#include "pf.h"

pf::pf()
{
   patAg = new pattAgent(true);
   potAg = new potentAgent(false);
   state = new stateMachine(STATE_PATROL);
}

pf::~pf()
{
   delete(patAg);
   delete(potAg);
   delete(state);
}

