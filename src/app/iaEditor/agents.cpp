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
   totalPotentAgents = 0;
   potentAgents = NULL;

   carregaTextura(img, &potentialTexture);
      
   SDL_FreeSurface(img);
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

}

/********************************************************************
 *                        drawPotentAgent                           *
 ********************************************************************/
void agents::drawPotentAgent()
{
   GLfloat materialColor[4] = {0.7, 0.7, 0.7, 1.0};
   float shininess;
   shininess = 50.0f;

   glEnable(GL_COLOR_MATERIAL);

   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
   glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
   glMaterialfv(GL_FRONT, GL_SPECULAR, materialColor);
   glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);

        
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
