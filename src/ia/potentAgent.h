#ifndef _potentAgent_h
#define _potentAgent_h

#include "agent.h"

#define AGENT_POTENT_HALF_X 5.0
#define AGENT_POTENT_HALF_Z 5.0
#define AGENT_POTENT_HIGHT  6.0


class potentAgent: public agent
{
   public:
      potentAgent(bool oriented);
      ~potentAgent();

      /*! Defines Next Agent position. Virtual. 
       * \return true if position was defined */
      bool defineNextPosition();

      potentAgent* next;
};
                   


#endif

