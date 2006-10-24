#ifndef _politic_h
#define _politc_h

#include "../../ia/potentAgent.h"
#include "states.h"
#include "briefcase.h"

#define WORK_INTERVAL   60000
#define WORK_TIME       1000

class politic: public potentAgent
{
   public:
     politic();
     ~politic();

     void actualizeMachineAndPosition();

     briefCase* currentBriefCase();
     void setBriefCase(briefCase* br);

     void setBaseArea(int group, GLfloat x1, GLfloat z1, GLfloat x2, GLfloat z2);

     void setCongress(GLfloat x, GLfloat z);

     int getState();
     
   private:
     int rogueGroup;
     int lastWork;
     static int lastSession;
     stateMachine* state;
     briefCase* brief;

     GLfloat baseX1, baseX2, baseZ1, baseZ2;
     GLfloat congressX, congressZ;

};

#endif

