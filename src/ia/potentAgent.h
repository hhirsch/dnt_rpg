#ifndef _potentAgent_h
#define _potentAgent_h

#include "agent.h"

class potentAgent: public agent
{
   public:
      potentAgent(bool oriented);
      ~potentAgent();

      /*! Defines Next Agent position. Virtual. 
       * \return true if position was defined */
      bool defineNextPosition();

      potentAgent* next;

      /*! Defines the agent Constants
       * \param a -> Atraction constant
       * \param c -> Collision constant
       * \param r -> Repulsion constant */
      void defineConstants(GLfloat a, GLfloat c, GLfloat r);
      
   private:
      GLfloat ka, kc, kr;
};
                   


#endif

