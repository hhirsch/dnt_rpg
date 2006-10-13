#include "agents.h"
#include "message.h"
#include "../../etc/distance.h"
#include "../../gui/desenho.h"
#include "../../engine/util.h"
#include "../../etc/glm.h"
#include <GL/glu.h>
#include <math.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include <fstream>

#define AGENTS_STATE_NONE           0
#define AGENTS_STATE_POTENTIAL      1
#define AGENTS_STATE_PATTERN        2
#define AGENTS_STATE_WAYPOINTS      3
#define AGENTS_STATE_GOAL           4
#define AGENTS_STATE_OBSTACLE        5


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

   img = IMG_Load("../data/iaEditor/goal.png");
   if(!img)
   {
      printf("Can't Open Texture!\n");
   }
   carregaTexturaRGBA(img, &goalTexture);
   SDL_FreeSurface(img);


   totalPattAgents = 0;
   pattAgents = NULL;

   obstacleOrientation = 0;
   obstacleX = 0;
   obstacleZ = 0;
   actualObstacle = NULL;

}

/********************************************************************
 *                       Remove All Agents                          *
 ********************************************************************/
void agents::removeAllAgents()
{
   potentAgent* ag;
   while(totalPotentAgents > 0)
   {
      ag = potentAgents;
      potentAgents = potentAgents->next;
      delete(ag);
      totalPotentAgents--;
   }

   pattAgent* patt;
   while(totalPattAgents > 0)
   {
      patt = pattAgents;
      pattAgents = pattAgents->next;
      delete(patt);
      totalPattAgents--;
   }
}

/********************************************************************
 *                           Destructor                             *
 ********************************************************************/
agents::~agents()
{
   removeAllAgents();

   glDeleteTextures(1,&potentialTexture);
   glDeleteTextures(1,&pattTexture);
   glDeleteTextures(1,&goalTexture);
}

/********************************************************************
 *                           actualize                              *
 ********************************************************************/
void agents::actualize(Map* actualMap)
{
   int aux;
   potentAgent* potAg = potentAgents;
   /* Pontential Function Agents */
   for(aux = 0; aux < totalPotentAgents; aux++)
   {
      addVisibleAgents(potAg, actualMap);
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


   /* Temporary Obstacles */
   if( (state == AGENTS_STATE_OBSTACLE) && (actualObstacle != NULL))
   {
      actualObstacle->Desenhar(obstacleX, obstacleZ, 0, obstacleOrientation);
   }

   
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

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   
   glEnable(GL_TEXTURE_2D);     
   glBindTexture(GL_TEXTURE_2D, goalTexture);
   glBegin(GL_QUADS);
     glTexCoord2f(0.0,0.0);
     glNormal3i(0,1,0);
     glVertex3f( goalX-2 , 
                 0.2 , 
                 goalZ-2 );
     glTexCoord2f(0.0,1.0);
     glNormal3i(0,1,0);
     glVertex3f( goalX-2 , 
                 0.0 , 
                 goalZ+2);
     glTexCoord2f(1.0,1.0);
     glNormal3i(0,1,0);
     glVertex3f( goalX+2, 
                 0.0 , 
                 goalZ+2 );
     glTexCoord2f(1.0,0.0);
     glNormal3i(0,1,0);
     glVertex3f( goalX+2, 
                 0.0, 
                 goalZ-2 );
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);

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
      aux->defineBoundingBox(-AGENT_POTENT_HALF_X, -AGENT_POTENT_HALF_Z, 
                              AGENT_POTENT_HALF_X,  AGENT_POTENT_HALF_Z);
      ag = (agent*) potentAgents;
      totalPotentAgents++;
   }
   else if(type == AGENT_TYPE_PATTERN)
   {
      pattAgent* aux = new pattAgent(oriented);
      aux -> next = pattAgents;
      pattAgents = aux;
      aux->defineBoundingBox(-AGENT_PATT_HALF_X, -AGENT_PATT_HALF_Z, 
                              AGENT_PATT_HALF_X,  AGENT_PATT_HALF_Z);

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
 *                      addSquareObstacles                          *
 ********************************************************************/
void agents::addSquareObstacles(agent* ag, Square* saux)
{
   GLfloat posX, posZ;
   GLfloat min1[3];
   GLfloat max1[3];
   GLfloat min2[3];
   GLfloat max2[3];
   GLfloat sightD, sightA;
   
   if(!saux)
   {
      return;
   }

   ag->getPosition(posX, posZ);
   ag->getSight(sightD, sightA);
   
   /* Define Position */
   ag->getBoundingBox(min1[0],min1[2], max1[0], max1[2]);

   min1[0] += posX-sightD; 
   min1[1] = 0; 
   min1[2] += posZ-sightD;
   max1[0] += posX+sightD; 
   max1[1] = 0; 
   max1[2] += posZ+sightD;

   /* Verify Walls */
   int mur = 0;
   while((mur < MAXMUROS ) && (saux->muros[mur] != NULL))
   {
      min2[0] = saux->muros[mur]->x1; 
      min2[1] = 0; 
      min2[2] = saux->muros[mur]->z1;
      max2[0] = saux->muros[mur]->x2; 
      max2[1] = MUROALTURA; 
      max2[2] = saux->muros[mur]->z2;
      if(estaDentro(min1,max1,min2,max2,1))
      {
         ag->addObstacle(((min2[0]+max2[0]) / 2), ((min2[2]+max2[2])/2), 
                           saux->muros[mur]->x1, saux->muros[mur]->z1, 
                           saux->muros[mur]->x2, saux->muros[mur]->z2);
      }
      mur++;
   }

   /* Verify Objects */
   int ob = 0;
   GLMmodel* modelo3d;
   GLfloat X[4], Z[4];
   while( (saux->objects[ob] != NULL)) 
   {
      modelo3d = (GLMmodel*)saux->objects[ob]->modelo3d;
      X[0] = modelo3d->x1;
      Z[0] = modelo3d->z1;
      X[1] = modelo3d->x1;
      Z[1] = modelo3d->z2;
      X[2] = modelo3d->x2;
      Z[2] = modelo3d->z2;
      X[3] = modelo3d->x2;
      Z[3] = modelo3d->z1;
      rotTransBoundingBox(saux->objectsOrientation[ob], X, Z,
                          saux->Xobjects[ob], modelo3d->y1, 
                          modelo3d->y2, saux->Zobjects[ob], 
                          min2, max2);
           
      if(estaDentro(min1,max1,min2,max2,1))
      {
         ag->addObstacle(saux->Xobjects[ob], saux->Zobjects[ob], 
                         min2[0], min2[2], 
                         max2[0], max2[2]);
      }
      ob++;
   }
}
      


/********************************************************************
 *                      addVisibleAgents                            *
 ********************************************************************/
void agents::addVisibleAgents(agent* ag, Map* actualMap)
{
   int aux;
   GLfloat posX, posZ;

   if(!ag)
   {
      return;
   }

   ag->getPosition(posX, posZ);

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

   pattAgent* patAg = pattAgents;

   /* Pattern Functions Agents */
   for(aux = 0; aux < totalPattAgents; aux++)
   {
      if(patAg != ag)
      {
         ag->addIfVisible(patAg);
      }
      patAg = patAg->next;
   }
   
   if(!actualMap)
   {
      return;
   }
   
   /* Only Verify actual Occuped Square. */
   ag->getPosition(posX, posZ);
   Square* saux = actualMap->quadradoRelativo( (int)(posX / SQUARESIZE),
                                               (int)(posZ / SQUARESIZE));

   addSquareObstacles(ag,saux);

/*   saux = actualMap->quadradoRelativo( (int)(posX / SQUARESIZE)-1,
                                       (int)(posZ / SQUARESIZE));
   if(saux)
   {
      addSquareObstacles(ag, saux);
      saux = actualMap->quadradoRelativo( (int)(posX / SQUARESIZE)-1,
                                          (int)(posZ / SQUARESIZE)-1);
      addSquareObstacles(ag, saux);
      saux = actualMap->quadradoRelativo( (int)(posX / SQUARESIZE)-1,
                                          (int)(posZ / SQUARESIZE)+1);
      addSquareObstacles(ag, saux);
   }

   saux = actualMap->quadradoRelativo( (int)(posX / SQUARESIZE)+1,
                                       (int)(posZ / SQUARESIZE));
   addSquareObstacles(ag, saux);
   if(saux)
   {
      saux = actualMap->quadradoRelativo( (int)(posX / SQUARESIZE)+1,
                                       (int)(posZ / SQUARESIZE)+1);
      addSquareObstacles(ag, saux);
      saux = actualMap->quadradoRelativo( (int)(posX / SQUARESIZE)+1,
                                          (int)(posZ / SQUARESIZE)-1);
      addSquareObstacles(ag, saux);
   }

   saux = actualMap->quadradoRelativo( (int)(posX / SQUARESIZE),
                                       (int)(posZ / SQUARESIZE)+1);
   addSquareObstacles(ag, saux);
   saux = actualMap->quadradoRelativo( (int)(posX / SQUARESIZE),
                                       (int)(posZ / SQUARESIZE)-1);
   addSquareObstacles(ag, saux);*/

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
 *                        Remove Pattern Agent                    *
 ******************************************************************/
pattAgent* agents::removePattAgent(pattAgent* patAg)
{
   pattAgent* aux;

   if(patAg == pattAgents)
   {
      pattAgents = patAg->next;
      aux = pattAgents;
   }
   else
   {
      aux = pattAgents;
      while(aux->next != patAg)
      {
         aux = aux->next;
      }
      aux->next = patAg->next;
      aux = patAg->next;
   }
   
   delete(patAg);
   totalPattAgents--;
   
   if(totalPattAgents == 0)
   {
      pattAgents = NULL;
   }

   return(aux);
}


/******************************************************************
 *                        redefineGoal                            *
 ******************************************************************/
void agents::redefineGoal(GLfloat x, GLfloat z)
{
   goalX = x;
   goalZ = z;

   int aux;
   
   potentAgent* potAg = potentAgents;

   /* Pontential Function Agents */
   for(aux = 0; aux < totalPotentAgents; aux++)
   {
      potAg->defineDestiny(goalX, goalZ);
      potAg = potAg->next;
   }

}

/******************************************************************
 *                        Verify Action                           *
 ******************************************************************/
void agents::verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                          Uint8 mButton, int tool, Map* actualMap, 
                          mapObjeto** actualObject)
{
   if( (state == AGENTS_STATE_POTENTIAL) && (tool != TOOL_POTENTIAL_ADD))
   {
      if(actualAgent != NULL)
      {
         removePotentAgent( (potentAgent*) actualAgent);
         actualAgent = NULL;
      }
   }

   if( (state == AGENTS_STATE_PATTERN) && (tool != TOOL_PATTERN_ADD))
   {
      if(actualAgent != NULL)
      {
         removePattAgent( (pattAgent*) actualAgent);
         actualAgent = NULL;
      }
   }

      
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
      if(mButton & SDL_BUTTON(1))
      {
         redefineGoal(mouseX, mouseZ);
         while(mButton & SDL_BUTTON(1))
         {
            //Wait for Mouse Button Release
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }

      }
   }
   else if(tool == TOOL_OBSTACLE_ADD)
   {
      state = AGENTS_STATE_OBSTACLE;
      actualObstacle = *actualObject;
      obstacleX = mouseX;
      obstacleZ = mouseZ;
      
      if( (mButton & SDL_BUTTON(1)) && (actualObstacle != NULL) )
      {
         insertObject(mouseX, mouseZ, obstacleOrientation, actualMap, 
                      actualObstacle, 
                      (int)(mouseX / SQUARESIZE), (int)(mouseZ / SQUARESIZE));
         while(mButton & SDL_BUTTON(1))
         {
            //Wait for Mouse Button Release
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }
      }
      else if((mButton & SDL_BUTTON(2)) && (actualObstacle != NULL))
      {
         obstacleOrientation += 1;
      }
      else if((mButton & SDL_BUTTON(3)) && (actualObstacle != NULL))
      {
         actualObstacle = (mapObjeto*)actualObstacle->proximo;
         /* Verify Head Node */
         if(actualObstacle == actualMap->Objetos->primeiro)
         {
            actualObstacle = (mapObjeto*)actualObstacle->proximo;
         }

         *actualObject = actualObstacle;

         while(mButton & SDL_BUTTON(3))
         {
            //Wait for Mouse Button Release
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }
      }
   }
   else
   {
      state = AGENTS_STATE_NONE;
      actualAgent = NULL;
   }
}


/******************************************************************
 *                            Save File                           *
 ******************************************************************/
string agents::saveState(string fileName)
{
   string ret = "";
   int i;

   std::ofstream file;
   file.open(fileName.c_str(), ios::out | ios::binary);


   if(!file)
   {
      ret = "Error while saving file: ";
      ret += fileName;
      return(ret);
   }

   /* Save Number of Total Agents */
   file << "Agents: " << totalPotentAgents << " " << totalPattAgents << "\n";

   /* Save Goal Position */
   file << "Goal: " << goalX << " " << goalZ << "\n";

   GLfloat x, z, sightDist, sightAngle;
   int oriented;
   potentAgent* ag = potentAgents;
   
   /* Save All Potential Agents */
   for(i=0; i< totalPotentAgents; i++)
   {
      ag->getPosition(x,z);
      ag->getSight(sightDist, sightAngle);
      if(ag->oriented())
      {
         oriented = 1;
      }
      else
      {
         oriented = 0;
      }
      file << "Pot: " << x << " " << z << " " << oriented << " " <<
            ag->getStepSize() << " " << sightDist << " " << sightAngle << "\n";
      ag = ag->next;
   }

   /* Save All Pattern Agents */
   pattAgent* a = pattAgents;
   int w, numWayPoints;
   for(i=0; i< totalPattAgents; i++)
   {
      a->getPosition(x,z);
      a->getSight(sightDist, sightAngle);
      if(a->oriented())
      {
         oriented = 1;
      }
      else
      {
         oriented = 0;
      }
      numWayPoints = a->getTotalWayPoints();

      file << "Pat: " << x << " " << z << " " << oriented << " " << 
              a->getStepSize() << " " << sightDist << " " << sightAngle <<
              " " << numWayPoints << "\n";
      
      wayPoint* wp = a->getWayPoints();
      for(w = 0; w < numWayPoints; w++)
      {
         file << "Way: " << wp->x << " " << wp->z << "\n";
         wp = wp->next;
      }
      a = a->next;
   }


   ret = "Saved File: ";
   ret += fileName;
   return(ret);
}


/******************************************************************
 *                            Load File                           *
 ******************************************************************/
string agents::loadState(string fileName)
{
   string ret = "";
   string aux = "";
   int numPotAgents;
   int numPatAgents;
   int i;

   GLfloat x,z, step;
   GLfloat sightDist, sightAngle;
   int oriented; 

   std::ifstream file;
   file.open(fileName.c_str(), ios::in | ios::binary);


   if(!file)
   {
      ret = "Error while opening file: ";
      ret += fileName;
      return(ret);
   }

   /* Remove All Agents */
   removeAllAgents();

   /* Get Number of Total Agents */
   getline(file, aux);
   sscanf(aux.c_str(),"Agents: %d %d",&numPotAgents, &numPatAgents);

   /* Get Goal Position */
   getline(file, aux);
   sscanf(aux.c_str(),"Goal: %f %f",&goalX, &goalZ);
   
   /* Gets All Potential Agents */
   for(i=0; i< numPotAgents; i++)
   {
      getline(file, aux);
      sscanf(aux.c_str(),"Pot: %f %f %d %f %f %f", &x, &z, &oriented, &step,
                        &sightDist, &sightAngle);
      addAgent(AGENT_TYPE_POTENT, x, z, oriented == 1, 
               step, goalX, goalZ, sightDist, sightAngle);
   }

   /* Gets All Pattern Agents */
   int w, numWayPoints;
   for(i=0; i< numPatAgents; i++)
   {
      getline(file, aux);
      sscanf(aux.c_str(),"Pat: %f %f %d %f %f %f %d", &x, &z, &oriented, &step,
                        &sightDist, &sightAngle, &numWayPoints);
      addAgent(AGENT_TYPE_PATTERN, x, z, oriented == 1, 
               step, goalX, goalZ, sightDist, sightAngle);
      
      pattAgent* a = (pattAgent*) actualAgent;
      for(w = 0; w < numWayPoints; w++)
      {
         getline(file, aux);
         sscanf(aux.c_str(),"Way: %f %f", &x, &z);
         a->addWayPoint(x, z);
      }
   }

   
   file.close();
   ret = "Loaded file: ";
   ret += fileName;

   actualAgent = NULL;
   
   return(ret);
}

/******************************************************************
 *                          insertObject                          *
 ******************************************************************/
void agents::insertObject(GLfloat xReal, GLfloat zReal, int orObj,
                          Map* map, mapObjeto* obj, int qx, int qz)
{
   Square* saux = map->quadradoRelativo(qx,qz);
   int ob=0;
   if(saux)
   {
     while( (ob < MAXOBJETOS ) && (saux->objects[ob] != NULL))
     {
        ob++;
     }
     if(ob<MAXOBJETOS)
     {
        saux->objects[ob] = obj;
        saux->Xobjects[ob] = xReal;
        saux->Zobjects[ob] = zReal;
        saux->objectsOrientation[ob] = orObj;
        saux->objectsDesenha[ob] = 1;
        //printf("%d° Object Inserted on %d %d\n",ob,qx+1,qz+1);
                  
        GLMmodel* modelo = (GLMmodel*)obj->modelo3d; 

        float X[2], Z[2];
        X[0] = modelo->x1;
        X[1] = modelo->x2;
        Z[0] = modelo->z1;
        Z[1] = modelo->z2;
        if(orObj!=0)
        {
           GLfloat oldX, oldZ;
           GLfloat cosseno = cos(deg2Rad(orObj));
           GLfloat seno = sin(deg2Rad(orObj));
           int aux;
           for(aux = 0;aux<=1;aux++)
           {
              oldX = X[aux];
              oldZ = Z[aux];
              X[aux] = (oldZ*seno) + (oldX*cosseno);
              Z[aux] = (oldZ*cosseno) - (oldX*seno);
           }
           if(X[0]>X[1])
           {
              oldX = X[0];
              X[0] = X[1];
              X[1] = oldX;
           }
           if(Z[0]>Z[1])
           {
              oldZ = Z[0];
              Z[0] = Z[1];
              Z[1] = oldZ;
           }
       }

       int minqx, minqz, maxqx, maxqz;
       minqx = (int)(X[0] + xReal) / SQUARESIZE;
       minqz = (int)(Z[0] + zReal) / SQUARESIZE;
       maxqx = (int)(X[1] + xReal) / SQUARESIZE;
       maxqz = (int)(Z[1] + zReal) / SQUARESIZE; 
       int X1, Z1;
       Square* qaux;
       for(X1 = minqx; X1<=maxqx; X1++)
       {
          for(Z1 = minqz; Z1 <=maxqz; Z1++) 
          {
             qaux = map->quadradoRelativo(X1,Z1);
             if((qaux) && (qaux != saux))
             {
                ob =0;
                while( (ob < MAXOBJETOS ) && 
                       (qaux->objects[ob] != NULL))
                {
                   ob++;
                }
                if(ob < MAXOBJETOS)
                {
                   qaux->objects[ob] = obj;
                   qaux->Xobjects[ob] = xReal;
                   qaux->Zobjects[ob] = zReal;
                   qaux->objectsDesenha[ob] = 0;
                   //printf("%d° Object Inserted on %d %d\n",ob,X1+1,Z1+1);
                }
             }
          }
       }
     }
     else
       printf("Objects Overflow on Square %d %d\n",qx+1,qz+1);
   }
   else
     printf("Out of Map's Limits!\n");
}

