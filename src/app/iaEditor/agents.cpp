#include "agents.h"
#include "../../etc/distance.h"
#include "../../gui/desenho.h"
#include <GL/glu.h>
#include <math.h>
#include <SDL/SDL_image.h>

/********************************************************************
 *                          Constructor                             *
 ********************************************************************/
agents::agents()
{
   SDL_Surface* img = IMG_Load("../data/iaEditor/potentialTexture.png");
   if(!img)
   {
      printf("Can't Open Texture!\n");
   }
   carregaTextura(img, &potentialTexture);
   SDL_FreeSurface(img);

   totalPotentAgents = 0;
   potentAgents = NULL;

   img = IMG_Load("../data/iaEditor/pattTexture.png");
   if(!img)
   {
      printf("Can't Open Texture!\n");
   }
   carregaTextura(img, &pattTexture);
   SDL_FreeSurface(img);


   totalPattAgents = 0;
   pattAgents = NULL;

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
   glDeleteTextures(1,&potentialTexture);
   glDeleteTextures(1,&pattTexture);
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
      addVisibleAgents(potAg);
      potAg->actualize();
      potAg = potAg->next;
   }

   pattAgent* patAg = pattAgents;
   /* Pattern Agents */
   for(aux = 0; aux < totalPattAgents; aux++)
   {
      //addVisibleAgents(patAg);
      patAg->actualize();
      patAg = patAg->next;
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
      //printf("Ag: %d Position: %.3f %.3f\n",aux, x, z);
      glPushMatrix();
      //TODO Rotate, based on orientation
      //glRotatef(an);
      glTranslatef(x ,0.0, z);
      drawPotentAgent();
      glPopMatrix();
      potAg = potAg->next;
   }

   pattAgent* patAg = pattAgents;
   /* Pattern Agents */
   for(aux = 0; aux < totalPattAgents; aux++)
   {
      patAg->getPosition(x,z);
      glPushMatrix();
      //TODO Rotate, based on orientation
      //glRotatef(an);
      glTranslatef(x ,0.0, z);
      drawPattAgent();
      glPopMatrix();
      patAg = patAg->next;
   }


}

/********************************************************************
 *                        drawPotentAgent                           *
 ********************************************************************/
void agents::drawPotentAgent()
{
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, potentialTexture);
   
   glBegin(GL_QUADS);
      /* Face de frente */
      glNormal3i(0,0,1);
      glTexCoord2d(0,1);
      glVertex3f(-AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,-AGENT_POTENT_HALF_Z);
      glTexCoord2d(1,1);
      glVertex3f(AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,-AGENT_POTENT_HALF_Z);
      glTexCoord2d(1,0);
      glVertex3f(AGENT_POTENT_HALF_X,-1,-AGENT_POTENT_HALF_Z);
      glTexCoord2d(0,0);
      glVertex3f(-AGENT_POTENT_HALF_X,-1,-AGENT_POTENT_HALF_Z);
      /* Face de tras */
      glNormal3i(0,0,-1);
      glTexCoord2d(0,1);
      glVertex3f(-AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,AGENT_POTENT_HALF_Z);
      glTexCoord2d(1,1);
      glVertex3f(AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,AGENT_POTENT_HALF_Z);
      glTexCoord2d(1,0);
      glVertex3f(AGENT_POTENT_HALF_X,-1,AGENT_POTENT_HALF_Z);
      glTexCoord2d(0,0);
      glVertex3f(-AGENT_POTENT_HALF_X,-1,AGENT_POTENT_HALF_Z);
      /* Face de esquerda */
      glNormal3i(-1,0,0);
      glTexCoord2d(1,0);
      glVertex3f(-AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,-AGENT_POTENT_HALF_Z);
      glTexCoord2d(1,1);
      glVertex3f(-AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,AGENT_POTENT_HALF_Z);
      glTexCoord2d(0,1);
      glVertex3f(-AGENT_POTENT_HALF_X,-1,AGENT_POTENT_HALF_Z);
      glTexCoord2d(0,0);
      glVertex3f(-AGENT_POTENT_HALF_X,-1,-AGENT_POTENT_HALF_Z);
      /* Face de direita */
      glNormal3i(1,0,0);
      glTexCoord2d(1,0);
      glVertex3f(AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,-AGENT_POTENT_HALF_Z);
      glTexCoord2d(1,1);
      glVertex3f(AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,AGENT_POTENT_HALF_Z);
      glTexCoord2d(0,1);
      glVertex3f(AGENT_POTENT_HALF_X,-1,AGENT_POTENT_HALF_Z);
      glTexCoord2d(0,0);
      glVertex3f(AGENT_POTENT_HALF_X,-1,-AGENT_POTENT_HALF_Z);
      /* Face de cima */
      glNormal3i(0,1,0);
      glTexCoord2d(0,0);
      glVertex3f(-AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,-AGENT_POTENT_HALF_Z);
      glTexCoord2d(1,0);
      glVertex3f(AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,-AGENT_POTENT_HALF_Z);
      glTexCoord2d(1,1);
      glVertex3f(AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,AGENT_POTENT_HALF_Z);
      glTexCoord2d(0,1);
      glVertex3f(-AGENT_POTENT_HALF_X,AGENT_POTENT_HIGHT,AGENT_POTENT_HALF_Z);
   glEnd();

   glDisable(GL_COLOR_MATERIAL);
   glDisable(GL_TEXTURE_2D);
}

/********************************************************************
 *                          drawPattAgent                           *
 ********************************************************************/
void agents::drawPattAgent()
{
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, pattTexture);
   
   glBegin(GL_QUADS);
      /* Face de frente */
      glNormal3i(0,0,1);
      glTexCoord2d(0,1);
      glVertex3f(-AGENT_PATT_HALF_X,AGENT_PATT_HIGHT,-AGENT_PATT_HALF_Z);
      glTexCoord2d(1,1);
      glVertex3f(AGENT_PATT_HALF_X,AGENT_PATT_HIGHT,-AGENT_PATT_HALF_Z);
      glTexCoord2d(1,0);
      glVertex3f(AGENT_PATT_HALF_X,-1,-AGENT_PATT_HALF_Z);
      glTexCoord2d(0,0);
      glVertex3f(-AGENT_PATT_HALF_X,-1,-AGENT_PATT_HALF_Z);
      /* Face de tras */
      glNormal3i(0,0,-1);
      glTexCoord2d(0,1);
      glVertex3f(-AGENT_PATT_HALF_X,AGENT_PATT_HIGHT,AGENT_PATT_HALF_Z);
      glTexCoord2d(1,1);
      glVertex3f(AGENT_PATT_HALF_X,AGENT_PATT_HIGHT,AGENT_PATT_HALF_Z);
      glTexCoord2d(1,0);
      glVertex3f(AGENT_PATT_HALF_X,-1,AGENT_PATT_HALF_Z);
      glTexCoord2d(0,0);
      glVertex3f(-AGENT_PATT_HALF_X,-1,AGENT_PATT_HALF_Z);
      /* Face de esquerda */
      glNormal3i(-1,0,0);
      glTexCoord2d(1,0);
      glVertex3f(-AGENT_PATT_HALF_X,AGENT_PATT_HIGHT,-AGENT_PATT_HALF_Z);
      glTexCoord2d(1,1);
      glVertex3f(-AGENT_PATT_HALF_X,AGENT_PATT_HIGHT,AGENT_PATT_HALF_Z);
      glTexCoord2d(0,1);
      glVertex3f(-AGENT_PATT_HALF_X,-1,AGENT_PATT_HALF_Z);
      glTexCoord2d(0,0);
      glVertex3f(-AGENT_PATT_HALF_X,-1,-AGENT_PATT_HALF_Z);
      /* Face de direita */
      glNormal3i(1,0,0);
      glTexCoord2d(1,0);
      glVertex3f(AGENT_PATT_HALF_X,AGENT_PATT_HIGHT,-AGENT_PATT_HALF_Z);
      glTexCoord2d(1,1);
      glVertex3f(AGENT_PATT_HALF_X,AGENT_PATT_HIGHT,AGENT_PATT_HALF_Z);
      glTexCoord2d(0,1);
      glVertex3f(AGENT_PATT_HALF_X,-1,AGENT_PATT_HALF_Z);
      glTexCoord2d(0,0);
      glVertex3f(AGENT_PATT_HALF_X,-1,-AGENT_PATT_HALF_Z);
      /* Face de cima */
      glNormal3i(0,1,0);
      glTexCoord2d(0,0);
      glVertex3f(-AGENT_PATT_HALF_X,AGENT_PATT_HIGHT,-AGENT_PATT_HALF_Z);
      glTexCoord2d(1,0);
      glVertex3f(AGENT_PATT_HALF_X,AGENT_PATT_HIGHT,-AGENT_PATT_HALF_Z);
      glTexCoord2d(1,1);
      glVertex3f(AGENT_PATT_HALF_X,AGENT_PATT_HIGHT,AGENT_PATT_HALF_Z);
      glTexCoord2d(0,1);
      glVertex3f(-AGENT_PATT_HALF_X,AGENT_PATT_HIGHT,AGENT_PATT_HALF_Z);
   glEnd();

   glDisable(GL_COLOR_MATERIAL);
   glDisable(GL_TEXTURE_2D);
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
   else if(type == AGENT_TYPE_PATTERN)
   {
      pattAgent* aux = new pattAgent(oriented);
      aux -> next = pattAgents;
      pattAgents = aux;
      ag = (agent*) pattAgents;
      totalPattAgents++;
      //TODO Get Pattern!!!
      aux->addWayPoint(x, z);
      aux->addWayPoint(x+10, z);
      aux->addWayPoint(x+10, z+10);
      aux->addWayPoint(x, z+10);
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
      if(ag == potAg)
      {
         //does nothing if is the same 
      }
      //TODO Verify oriented agent and Angle!
      else if(fabs(dist) <= agSightDistance)
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
