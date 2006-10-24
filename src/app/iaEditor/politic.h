#ifndef _politic_h
#define _politc_h

#include "../../ia/potentAgent.h"
#include "states.h"
#include "briefcase.h"

class politic: public potentAgent
{
   public:
     politic();
     ~politic();

     void actualizeMachineAndPosition();

     briefCase* currentBriefCase();
     void setBriefCase(briefCase* br);

     void setBaseArea(GLfloat x1, GLfloat z1, GLfloat x2, GLfloat z2);

     int getState();
     
   private:
     stateMachine* state;
     briefCase* brief;

     GLfloat baseX1, baseX2, baseZ1, baseZ2;

};

#endif

