#include "agents.h"
#include "../../etc/distance.h"
#include <GL/glu.h>
#include <math.h>

#define AGENT_POTENT_HALF_X 5.0
#define AGENT_POTENT_HALF_Z 7.0
#define AGENT_POTENT_HIGHT  6.0

/********************************************************************
 *                          Constructor                             *
 ********************************************************************/
agents::agents()
{
   totalPotentAgents = 0;
   potentAgents = NULL;
}

/********************************************************************
 *                           Destructor                             *
 ********************************************************************/
agents::~agents()
{
   potentAgent* ag;
   while(totalPotentAgents > 0)
   {
      ag = potentAgents;
      potentAgents = potentAgents->next;
      delete(ag);
      totalPotentAgents--;
   }
}

/********************************************************************
 *                           actualize                              *
 ********************************************************************/
void agents::actualize()
{
   int aux;
   potentAgent* potAg = potentAgents;
   /* Pontential Function Agents */
   for(aux = 0; aux < totalPotentAgents; aux++)
   {
      potAg->actualize();
      potAg = potAg->next;
   }
}

/********************************************************************
 *                               draw                               *
 ********************************************************************/
void agents::draw()
{
   
   int aux;
   GLfloat x,z;
   potentAgent* potAg = potentAgents;
   /* Pontential Function Agents */
   for(aux = 0; aux < totalPotentAgents; aux++)
   {
      potAg->getPosition(x,z);
      glPushMatrix();
      //TODO rotate, based on orientation
      glTranslatef(x ,0.0, z);
      drawPotentAgent();
      glPopMatrix();
      potAg = potAg->next;
   }

}

/********************************************************************
 *                        drawPotentAgent                           *
 ********************************************************************/
void agents::drawPotentAgent()
{
   glBegin(GL_QUADS);
      /* Face de frente */
      glNormal3i(0,0,1);
      glVertex3f(-AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,-AGENT_POTENT_HALF_Z);
      glVertex3f(AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,-AGENT_POTENT_HALF_Z);
      glVertex3f(AGENT_POTENT_HALF_X,-1,-AGENT_POTENT_HALF_Z);
      glVertex3f(-AGENT_POTENT_HALF_X,-1,-AGENT_POTENT_HALF_Z);
      /* Face de tras */
      glNormal3i(0,0,-1);
      glVertex3f(-AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,AGENT_POTENT_HALF_Z);
      glVertex3f(AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,AGENT_POTENT_HALF_Z);
      glVertex3f(AGENT_POTENT_HALF_X,-1,AGENT_POTENT_HALF_Z);
      glVertex3f(-AGENT_POTENT_HALF_X,-1,AGENT_POTENT_HALF_Z);
      /* Face de esquerda */
      glNormal3i(-1,0,0);
      glVertex3f(-AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,-AGENT_POTENT_HALF_Z);
      glVertex3f(-AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,AGENT_POTENT_HALF_Z);
      glVertex3f(-AGENT_POTENT_HALF_X,-1,AGENT_POTENT_HALF_Z);
      glVertex3f(-AGENT_POTENT_HALF_X,-1,-AGENT_POTENT_HALF_Z);
      /* Face de direita */
      glNormal3i(1,0,0);
      glVertex3f(AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,-AGENT_POTENT_HALF_Z);
      glVertex3f(AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,AGENT_POTENT_HALF_Z);
      glVertex3f(AGENT_POTENT_HALF_X,-1,AGENT_POTENT_HALF_Z);
      glVertex3f(AGENT_POTENT_HALF_X,-1,-AGENT_POTENT_HALF_Z);
      /* Face de cima */
      glNormal3i(0,1,0);
      glVertex3f(-AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,-AGENT_POTENT_HALF_Z);
      glVertex3f(AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,-AGENT_POTENT_HALF_Z);
      glVertex3f(AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,AGENT_POTENT_HALF_Z);
      glVertex3f(-AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,AGENT_POTENT_HALF_Z);
   glEnd();
}

/********************************************************************
 *                             addAgent                             *
 ********************************************************************/
void agents::addAgent(int type, GLfloat x, GLfloat z, bool oriented, 
                      GLfloat stepSize, GLfloat goalX, GLfloat goalZ, 
                      GLfloat sightDist, GLfloat sightAng)
{
   agent* ag;
   
   if(type == AGENT_TYPE_POTENT)
   {
      potentAgent* aux = new potentAgent(oriented);
      aux -> next = potentAgents;
      potentAgents = aux;
      ag = (agent*) potentAgents;
      totalPotentAgents++;
   }
   else
   {
      /* Do not insert agent of unknow type */
      return;
   }
   ag->definePosition(x,z);
   ag->defineSight(sightDist, sightAng);
   ag->defineDestiny(goalX, goalZ);
   ag->defineStepSize(stepSize);
}

/********************************************************************
 *                      addVisibleAgents                            *
 ********************************************************************/
void agents::addVisibleAgents(agent* ag)
{
   GLfloat dist;
   int aux;
   GLfloat x,z;
   GLfloat agX, agZ, agSightDistance, agSightAngle;
   potentAgent* potAg = potentAgents;

   /* Take Current Agent Position */
   ag->getPosition( agX, agZ );
   ag->getSight(agSightDistance, agSightAngle);
   ag->clearObstacles();
   
   /* Pontential Function Agents */
   for(aux = 0; aux < totalPotentAgents; aux++)
   {
      potAg->getPosition(x,z);
      /* Test All 4 Lines Equations of The Agent */
      dist = distancePointLine(agX, agZ, 
                              x - AGENT_POTENT_HALF_X, z - AGENT_POTENT_HALF_Z,
                              x + AGENT_POTENT_HALF_X, z - AGENT_POTENT_HALF_Z);
      //TODO Verify oriented agent and Angle!
      if(fabs(dist) <= agSightDistance)
      {
         ag->addObstacle(x,z);
      }
      else
      {

         dist = distancePointLine(agX, agZ, 
                              x - AGENT_POTENT_HALF_X, z - AGENT_POTENT_HALF_Z,
                              x - AGENT_POTENT_HALF_X, z + AGENT_POTENT_HALF_Z);
         if(fabs(dist) <= agSightDistance)
         {
            ag->addObstacle(x,z);
         }
         else
         {
            dist = distancePointLine(agX, agZ, 
                              x - AGENT_POTENT_HALF_X, z + AGENT_POTENT_HALF_Z,
                              x + AGENT_POTENT_HALF_X, z + AGENT_POTENT_HALF_Z);
            if(fabs(dist) <= agSightDistance)
            {
               ag->addObstacle(x,z);
            }
            else
            {
               dist = distancePointLine(agX, agZ, 
                              x + AGENT_POTENT_HALF_X, z - AGENT_POTENT_HALF_Z,
                              x + AGENT_POTENT_HALF_X, z + AGENT_POTENT_HALF_Z);
            }
         }
      }
      potAg = potAg->next;
   }
   
}
