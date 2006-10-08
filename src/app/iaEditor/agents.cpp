#include "agents.h"
#include "message.h"
#include "../../etc/distance.h"
#include "../../gui/desenho.h"
#include "../../engine/util.h"
#include <GL/glu.h>
#include <math.h>
#include <SDL/SDL_image.h>

#define AGENTS_STATE_NONE           0
#define AGENTS_STATE_POTENTIAL      1
#define AGENTS_STATE_PATTERN        2
#define AGENTS_STATE_WAYPOINTS      3
#define AGENTS_STATE_GOAL           4


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
   actualAgent = NULL;

   state = AGENTS_STATE_NONE;
   goalX = 200;
   goalZ = 200;

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
      removeColliders(patAg);
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
   float color = 0.0;
   GLfloat x,z;
   potentAgent* potAg = potentAgents;
   /* Pontential Function Agents */
   for(aux = 0; aux < totalPotentAgents; aux++)
   {
      potAg->getPosition(x,z);
      //printf("Ag: %d Position: %.3f %.3f\n",aux, x, z);
      glPushMatrix();
      if(potAg->oriented())
      {
         glRotatef(potAg->orientationValue(),0,1,0);
      }
      glTranslatef(x ,0.0, z);
      drawPotentAgent();
      glPopMatrix();
      potAg = potAg->next;
   }

   pattAgent* patAg = pattAgents;
   /* Pattern Agents */
   for(aux = 0; aux < totalPattAgents; aux++)
   {
      glColor3f(color,0.5,0.5);
      patAg->getPosition(x,z);
      glPushMatrix();
      glTranslatef(x ,0.0, z);
      if(patAg->oriented())
      {
         glRotatef(patAg->orientationValue(),0,1,0);
      }
      drawPattAgent();
      glPopMatrix();
      patAg->drawWayPoints();
      patAg = patAg->next;
      color += 0.1;
   }

   glColor3f(1.0,1.0,1.0);

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
   actualAgent = ag;
}



/********************************************************************
 *                      addVisibleAgents                            *
 ********************************************************************/
void agents::addVisibleAgents(agent* ag)
{
   int aux;
   
   potentAgent* potAg = potentAgents;

   ag->clearObstacles();
   
   /* Pontential Function Agents */
   for(aux = 0; aux < totalPotentAgents; aux++)
   {
      if(potAg != ag)
      {
         ag->addIfVisible(potAg);
      }
      potAg = potAg->next;
   }
}

/******************************************************************
 *                        remove Colliders                        *
 ******************************************************************/
void agents::removeColliders(pattAgent* patAg)
{
   int aux;
   int initialTotalAgents = totalPotentAgents; //Total agents before remove
   GLfloat agX, agZ;
   
   potentAgent* potAg = potentAgents;

   patAg->clearObstacles();
   patAg->getPosition(agX, agZ);

   GLfloat min1[3]; GLfloat max1[3];
   GLfloat min2[3]; GLfloat max2[3];

   GLfloat x[4];
   GLfloat z[4];

   x[0] = -AGENT_PATT_HALF_X;
   z[0] = -AGENT_PATT_HALF_Z;
   x[1] = -AGENT_PATT_HALF_X;
   z[1] = +AGENT_PATT_HALF_Z;
   x[2] = +AGENT_PATT_HALF_X;
   z[2] = +AGENT_PATT_HALF_Z;
   x[3] = +AGENT_PATT_HALF_X;
   z[3] = -AGENT_PATT_HALF_Z;

   if(patAg->oriented())
   {
      rotTransBoundingBox(patAg->orientationValue(), x, z,
                          agX, 0.0, 0.0, agZ,
                          min1, max1);
   }
   else
   {
      rotTransBoundingBox(0.0, x, z,
                          agX, 0.0, 0.0, agZ,
                          min1, max1);
   }
   
   
   /* Pontential Function Agents */
   for(aux = 0; aux < initialTotalAgents; aux++)
   {
      if((agent*)potAg != (agent*)patAg)
      {
         potAg->getPosition(agX, agZ);
         x[0] = -AGENT_POTENT_HALF_X;
         z[0] = -AGENT_POTENT_HALF_Z;
         x[1] = -AGENT_POTENT_HALF_X;
         z[1] = +AGENT_POTENT_HALF_Z;
         x[2] = +AGENT_POTENT_HALF_X;
         z[2] = +AGENT_POTENT_HALF_Z;
         x[3] = +AGENT_POTENT_HALF_X;
         z[3] = -AGENT_POTENT_HALF_Z;

         if(potAg->oriented())
         {
            rotTransBoundingBox(potAg->orientationValue(), x, z,
                                agX, 0.0, 0.0, agZ,
                                min2, max2);
         }
         else
         {  
            rotTransBoundingBox(0.0, x, z,
                                agX, 0.0, 0.0, agZ,
                                min2, max2);
         }
         
         if(estaDentro(min1, max1, min2, max2, 1))
         {
            //Remove the potential agents in range!
            /*GLfloat x,z;
            potAg->getPosition(x, z);
            printf("Removed: %.3f %.3f\n",x,z);
            patAg->getPosition(x, z);
            printf("From: %.3f %.3f\n",x,z);*/
            potAg = removePotentAgent(potAg);
         }
         else
         {
            potAg = potAg->next;
         }
      }
      else if(potAg)
      {
         potAg = potAg->next;
      }
   }
}

/******************************************************************
 *                         Remove Potent Agent                    *
 ******************************************************************/
potentAgent* agents::removePotentAgent(potentAgent* potAg)
{
   potentAgent* aux;

   if(potAg == potentAgents)
   {
      potentAgents = potAg->next;
      aux = potentAgents;
   }
   else
   {
      aux = potentAgents;
      while(aux->next != potAg)
      {
         aux = aux->next;
      }
      aux->next = potAg->next;
      aux = potAg->next;
   }
   
   delete(potAg);
   totalPotentAgents--;
   
   if(totalPotentAgents == 0)
   {
      potentAgents = NULL;
   }

   return(aux);
}

/******************************************************************
 *                        Verify Action                           *
 ******************************************************************/
void agents::verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                          Uint8 mButton, int tool)
{
   if(tool == TOOL_POTENTIAL_ADD)
   {
      if(state != AGENTS_STATE_POTENTIAL)
      {
         addAgent(AGENT_TYPE_POTENT, mouseX, mouseZ, false, 
                                     0.75, goalX, goalZ, 30, 360);
         state = AGENTS_STATE_POTENTIAL;
      }
      else if( mButton & SDL_BUTTON(1))
      {
         actualAgent = NULL;
         state = AGENTS_STATE_NONE;
         while(mButton & SDL_BUTTON(1))
         {
            //Wait for Mouse Button Release
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }
      }
      else if(state == AGENTS_STATE_POTENTIAL)
      {
         actualAgent->definePosition(mouseX, mouseZ);
      }
   }
   else if(tool == TOOL_PATTERN_ADD)
   {
      if( (state != AGENTS_STATE_PATTERN) && (state != AGENTS_STATE_WAYPOINTS) )
      {
         addAgent(AGENT_TYPE_PATTERN, mouseX, mouseZ, true, 
                                     0.75, goalX, goalZ, 0.75, 360);
         state = AGENTS_STATE_PATTERN;
      }
      else if( mButton & SDL_BUTTON(1) )
      {
         if(state == AGENTS_STATE_PATTERN)
         {
            state = AGENTS_STATE_WAYPOINTS;
            pattAgent* aux = (pattAgent*) actualAgent;
            /* Define Initial Position */
            aux->definePosition(mouseX, mouseZ);
            aux->addWayPoint(mouseX, mouseZ);
         }
         else if(state == AGENTS_STATE_WAYPOINTS)
         {
            pattAgent* aux = (pattAgent*) actualAgent;
            aux->addWayPoint(mouseX, mouseZ);
         }
         
         while(mButton & SDL_BUTTON(1))
         {
            //Wait for Mouse Button Release
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }

      }
      else if( (mButton & SDL_BUTTON(3)) && (state == AGENTS_STATE_WAYPOINTS))
      {
         state = AGENTS_STATE_NONE;
         actualAgent = NULL;
      }
      else if(state == AGENTS_STATE_PATTERN)
      {
         actualAgent->definePosition(mouseX, mouseZ);
      }
   }
   else if(tool == TOOL_GOAL_ADD)
   {
      state = AGENTS_STATE_NONE;
   }
   else
   {
      state = AGENTS_STATE_NONE;
      actualAgent = NULL;
   }
}
