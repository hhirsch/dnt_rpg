#include "agents.h"
#include "message.h"
#include "../../etc/distance.h"
#include "../../gui/desenho.h"
#include "../../engine/util.h"
#include <GL/glu.h>
#include <math.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include <fstream>

#define AGENTS_STATE_NONE                 0
#define AGENTS_STATE_POTENTIAL            1
#define AGENTS_STATE_PATTERN              2
#define AGENTS_STATE_WAYPOINTS            3
#define AGENTS_STATE_GOAL                 4
#define AGENTS_STATE_OBSTACLE             5
#define AGENTS_STATE_BRIEFCASE            6
#define AGENTS_STATE_ROGUE                7
#define AGENTS_STATE_POLICE               8
#define AGENTS_STATE_POLICE_WAYPOINTS     9



/********************************************************************
 *                          Constructor                             *
 ********************************************************************/
agents::agents()
{
   int i;
   groupAdd = 0;
   totalPotentAgents = 0;
   potentAgents = NULL;
   actualAgent = NULL;

   state = AGENTS_STATE_NONE;
   goalX = 200;
   goalZ = 200;

   loadTexture(&goalTexture,"../data/iaEditor/goal.png",true); 
   loadTexture(&tp3Textures[4],"../data/iaEditor/federal.png",false);
   loadTexture(&tp3Textures[3],"../data/iaEditor/congress.png",false);
   loadTexture(&tp3Textures[1],"../data/iaEditor/pt.png",false);
   loadTexture(&tp3Textures[0],"../data/iaEditor/psdb.png",false);
   loadTexture(&tp3Textures[2],"../data/iaEditor/pfl.png",false);

   for(i = 0; i<5; i++)
   {
      tp3X[i] = 80*(i+1);
      tp3Z[i] = 80;//*(i+1);
      scores[i] = 0;
   }

   lastCongressTime = 0;

   totalPattAgents = 0;
   pattAgents = NULL;

   totalPolitics = 0;
   politics = NULL;

   totalPfs = 0;
   pfs = 0;

   obstacleOrientation = 0;
   obstacleX = 0;
   obstacleZ = 0;
   actualObstacle = NULL;

   glColor3f(1.0,0.2,0.2);
   modelPatt = glmReadOBJ("../data/models/objetos/carros/toon/toon.obj","",1);
   modelPot = glmReadOBJ("../data/models/objetos/geral/et.obj","",1);

   /* TP3 */
   brief = new briefCases();
   politicModel = glmReadOBJ("../data/models/objetos/geral/colarinho_e_oculos.obj","",1);
   pfModel = glmReadOBJ("../data/models/objetos/geral/cap.obj","",1);
}

/********************************************************************
 *                          loadTexture                             *
 ********************************************************************/
void agents::loadTexture(GLuint* texture, string name, bool alpha)
{
   SDL_Surface* img = IMG_Load(name.c_str());
   if(!img)
   {
      printf("Can't Open Texture %s!\n", name.c_str());
   }
   if(alpha)
   {
      carregaTexturaRGBA(img, texture);
   }
   else
   {
      carregaTextura(img, texture);
   }
   SDL_FreeSurface(img);
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

   politic* pol;
   while(totalPolitics > 0)
   {
      pol = politics;
      politics = (politic*)politics->next;
      delete(pol);
      totalPolitics--;
   }

   pf* pfAg;
   while(totalPfs > 0)
   {
      pfAg = pfs;
      pfs = (pf*)pfs->next;
      delete(pfAg);
      totalPfs--;
   }
   brief->removeAll();

}

/********************************************************************
 *                           Destructor                             *
 ********************************************************************/
agents::~agents()
{
   removeAllAgents();

   glDeleteTextures(1,&goalTexture);
   glDeleteTextures(5,&tp3Textures[0]);

   glmDelete(modelPatt);
   glmDelete(modelPot);

   /* TP3 */
   delete(brief);
   glmDelete(politicModel);
   glmDelete(pfModel);

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

   /* Verify Congress Time */
   bool congressWork = false;
   if(SDL_GetTicks()-lastCongressTime >= WORK_INTERVAL)
   {
      lastCongressTime = SDL_GetTicks();
      congressWork = true;
   }

   politic* polAg = politics;
   /* Politics Agents */
   for(aux = 0; aux < totalPolitics; aux++)
   {
      if( (polAg->getState() == STATE_LOOK_OBJECT) && 
          (polAg->currentBriefCase() == NULL))
      {
         GLfloat x,z, sightD, sightA;
         polAg->getPosition(x, z);
         polAg->getSight(sightD, sightA);
         briefCase* tmp = brief->briefCaseInArea(x+politicModel->x1-sightD,
                                                 z+politicModel->z1-sightD,
                                                 x+politicModel->x2+sightD,
                                                 z+politicModel->z2+sightD);
                                                    
         if((!polAg->setBriefCase( tmp )) && (tmp))
         {
            excFunc.addExclamation( tmp->x, tmp->z);
         }
      }
      addVisibleAgents(polAg, actualMap);
      polAg->actualizeMachineAndPosition(congressWork);
      polAg = (politic*)polAg->next;
   }

   pf* pfAg = pfs;
   /* Pfs Agents */
   for(aux = 0; aux < totalPfs; aux++)
   {
      if( (pfAg->getState() == STATE_PATROL) && 
          (pfAg->getTarget() == NULL))
      {
         GLfloat x,z, sightD, sightA;
         pfAg->getPosition(x, z);
         pfAg->getSight(sightD, sightA);
         politic* tmp = getPoliticWithCaseInArea(x+pfModel->x1-sightD,
                                                       z+pfModel->z1-sightD,
                                                       x+pfModel->x2+sightD,
                                                       z+pfModel->z2+sightD);
         if( (!pfAg->setTarget( tmp)) && (tmp))
         {
            excFunc.addExclamation( tp3X[4], tp3Z[4]);
         }
      }

      //addVisibleAgents(pfAg->potAg, actualMap);
      GLfloat posX, posZ;
      pfAg->getPosition(posX, posZ);
      Square* saux = actualMap->quadradoRelativo( (int)(posX / SQUARESIZE),
                                               (int)(posZ / SQUARESIZE));
      pfAg->potAg->clearObstacles();
      addSquareObstacles(pfAg->potAg,saux);
      pfAg->actualizeMachineAndPosition();
      pfAg = pfAg->next;
   }

}

/********************************************************************
 *                      getPoliticwithCaseInArea                    *
 ********************************************************************/
politic* agents::getPoliticWithCaseInArea(GLfloat x1, GLfloat z1,
                                          GLfloat x2, GLfloat z2)
{
   int aux;
   politic* polAg = politics;
   for(aux = 0; aux < totalPolitics; aux++)
   {
      GLfloat x,z;
      polAg->getPosition(x, z);
      if( (polAg->currentBriefCase() != NULL) && (!polAg->busted) &&
          (x >= x1) && (x <= x2) && (z >= z1) && (z <= z2) )
      {
         return(polAg);
      }
      polAg = (politic*)polAg->next;
   }
   return(NULL);
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
      patAg->getPosition(x,z);
      glPushMatrix();
      glTranslatef(x ,0.0, z);
      if(patAg->oriented())
      {
         glRotatef(patAg->orientationValue(),0,1,0);
      }
      drawPattAgent();
      glPopMatrix();
      glColor3f(color,0.5,0.5);
      patAg->drawWayPoints();
      patAg = patAg->next;
      color += 0.1;
   }

   brief->draw();
   if(state == AGENTS_STATE_BRIEFCASE)
   {
      brief->drawAt(obstacleX, obstacleZ, obstacleOrientation);
   }

   politic* polAg = politics;
   GLfloat emission[3] = {0.0,0.0,0.0};
   /* Politic Agents */
   glEnable(GL_COLOR_MATERIAL);
   for(aux = 0; aux < totalPolitics; aux++)
   {
      polAg->getPosition(x,z);
      glPushMatrix();
      glTranslatef(x ,0.0, z);
      if(polAg->oriented())
      {
         glRotatef(polAg->orientationValue(),0,1,0);
      }
      emission[0] = polAg->getTimePercent();
      emission[2] = polAg->getAvaricePercent();
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
      drawPolitic();
      glPopMatrix();
      polAg = (politic*)polAg->next;
   }

   pf* pfAg = pfs;
   /* Pf Agents */
   for(aux = 0; aux < totalPfs; aux++)
   {
      pfAg->getPosition(x,z);
      glPushMatrix();
      glTranslatef(x ,0.0, z);
      glRotatef(pfAg->patAg->orientationValue(),0,1,0);
      emission[0] = pfAg->getTimePercent();
      emission[2] = pfAg->getCorruptPercent();
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
      drawPf();
      glPopMatrix();
      pfAg->patAg->drawWayPoints();
      pfAg = (pf*)pfAg->next;
   }

   emission[0] = 0.0;
   emission[1] = 0.0;
   emission[2] = 0.0;
   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);

   glDisable(GL_COLOR_MATERIAL);
   glColor3f(1.0,1.0,1.0);

   drawTexture(goalTexture, goalX, goalZ, 2, 2);
   for(aux=0; aux < 5; aux++)
   {
      drawTexture(tp3Textures[aux], tp3X[aux], tp3Z[aux], 32, 32);
   }

   excFunc.doStep();

}

/********************************************************************
 *                          drawTexture                             *
 ********************************************************************/
void agents::drawTexture(GLuint texture, GLfloat posX, GLfloat posZ, 
                         GLfloat varX, GLfloat varZ)
{
   glDisable(GL_LIGHTING);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   
   glEnable(GL_TEXTURE_2D);     
   glBindTexture(GL_TEXTURE_2D, texture);
   glBegin(GL_QUADS);
     glTexCoord2f(0.0,0.0);
     glNormal3i(0,1,0);
     glVertex3f( posX-varX , 
                 0.1 , 
                 posZ-varZ );
     glTexCoord2f(0.0,1.0);
     glNormal3i(0,1,0);
     glVertex3f( posX-varX , 
                 0.1 , 
                 posZ+varZ);
     glTexCoord2f(1.0,1.0);
     glNormal3i(0,1,0);
     glVertex3f( posX+varX, 
                 0.1 , 
                 posZ+varZ );
     glTexCoord2f(1.0,0.0);
     glNormal3i(0,1,0);
     glVertex3f( posX+varX, 
                 0.1, 
                 posZ-varZ );
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
   glEnable(GL_LIGHTING);
}

/********************************************************************
 *                        drawPotentAgent                           *
 ********************************************************************/
void agents::drawPotentAgent()
{
   glEnable(GL_COLOR_MATERIAL);
   glmDrawLists(modelPot);
   glDisable(GL_COLOR_MATERIAL);
}

/********************************************************************
 *                             drawPf                               *
 ********************************************************************/
void agents::drawPf()
{
   glEnable(GL_COLOR_MATERIAL);
   glmDrawLists(pfModel);
   glDisable(GL_COLOR_MATERIAL);
}

/********************************************************************
 *                          drawPattAgent                           *
 ********************************************************************/
void agents::drawPattAgent()
{
   glEnable(GL_COLOR_MATERIAL);
   glmDrawLists(modelPatt);
   glDisable(GL_COLOR_MATERIAL);
}

/********************************************************************
 *                           drawPolitic                            *
 ********************************************************************/
void agents::drawPolitic()
{
   glmDrawLists(politicModel);
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
      aux->defineBoundingBox(modelPot->x1, modelPot->z1, 
                             modelPot->x2, modelPot->z2);
      ag = (agent*) potentAgents;
      totalPotentAgents++;
   }
   else if(type == AGENT_TYPE_PATTERN)
   {
      pattAgent* aux = new pattAgent(oriented);
      aux -> next = pattAgents;
      pattAgents = aux;
      aux->defineBoundingBox(modelPatt->x1, modelPatt->z1, 
                             modelPatt->x2, modelPatt->z2);

      ag = (agent*) pattAgents;
      totalPattAgents++;
   }
   else if(type == AGENT_TYPE_ROGUE)
   {
      groupAdd = (groupAdd+1) % 3;
      politic* aux = new politic(&scores[groupAdd]);
      aux->next = politics;
      politics = aux;
      aux->defineBoundingBox(politicModel->x1, politicModel->z1, 
                             politicModel->x2, politicModel->z2);
      ag = (agent*) politics;
      totalPolitics++;
      ag->definePosition(x,z);
      ag->defineSight(sightDist, sightAng);
      ag->defineStepSize(stepSize-0.2);
      actualAgent = ag;
      aux->setBaseArea(groupAdd, tp3X[groupAdd]-32, tp3Z[groupAdd]-32, 
                              tp3X[groupAdd]+32, tp3Z[groupAdd]+32);
      aux->setCongress(tp3X[3], tp3Z[3]);
      aux->setFederal(tp3X[4], tp3Z[4]);
      return;
   }
   else if(type == AGENT_TYPE_POLICE)
   {
      pf* aux = new(pf);
      aux->next = pfs;
      pfs = aux;
      aux->patAg->defineBoundingBox(pfModel->x1, pfModel->z1, 
                                    pfModel->x2, pfModel->z2);
      aux->potAg->defineBoundingBox(pfModel->x1, pfModel->z1, 
                                    pfModel->x2, pfModel->z2);
      ag = (agent*) pfs->patAg;
      totalPfs++;
      aux->definePosition(x,z);
      aux->defineSight(sightDist, sightAng);
      aux->defineDestiny(goalX, goalZ);
      aux->defineStepSize(stepSize);
      aux->setFederal(tp3X[4], tp3Z[4]);
      actualAgent = ag;
      return;
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

   /* Politics Agents */
   potAg = politics;
   for(aux = 0; aux < totalPolitics; aux++)
   {
      if(potAg != ag)
      {
         ag->addIfVisible(potAg);
      }
      potAg = potAg->next;
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

   x[0] = modelPatt->x1;
   z[0] = modelPatt->z1;
   x[1] = modelPatt->x1;
   z[1] = modelPatt->z2;
   x[2] = modelPatt->x2;
   z[2] = modelPatt->z2;
   x[3] = modelPatt->x2;
   z[3] = modelPatt->z1;

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
         x[0] = modelPot->x1;
         z[0] = modelPot->z1;
         x[1] = modelPot->x1;
         z[1] = modelPot->z2;
         x[2] = modelPot->x2;
         z[2] = modelPot->z2;
         x[3] = modelPot->x2;
         z[3] = modelPot->z1;

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
 *                         Remove Politic Agent                   *
 ******************************************************************/
politic* agents::removePoliticAgent(politic* polAg)
{
   politic* aux;

   if(polAg == politics)
   {
      politics = (politic*)polAg->next;
      aux = politics;
   }
   else
   {
      aux = politics;
      while(aux->next != polAg)
      {
         aux = (politic*)aux->next;
      }
      aux->next = (politic*)polAg->next;
      aux = (politic*)polAg->next;
   }
   
   delete(polAg);
   totalPolitics--;
   
   if(totalPolitics == 0)
   {
      politics = NULL;
   }

   return(aux);
}

/******************************************************************
 *                            Remove Pf Agent                     *
 ******************************************************************/
pf* agents::removePfAgent(pattAgent* polAg)
{
   pf* aux;
   pf* del;

   if(polAg == pfs->patAg)
   {
      del = pfs;
      pfs = (pf*)pfs->next;
      aux = pfs;
   }
   else
   {
      aux = pfs;
      while(aux->next->patAg != polAg)
      {
         aux = (pf*)aux->next;
      }
      del = aux->next;
      aux->next = (pf*)aux->next->next;
      aux = (pf*)aux->next;
   }
   
   delete(del);
   totalPfs--;
   
   if(totalPfs == 0)
   {
      pfs = NULL;
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
 *                        redefineFederal                         *
 ******************************************************************/
void agents::redefineFederal(GLfloat x, GLfloat z)
{
   tp3X[4] = x;
   tp3Z[4] = z;

   int aux;
   
   politic* polAg = politics;

   /* Politics Agents */
   for(aux = 0; aux < totalPolitics; aux++)
   {
      polAg->setFederal(tp3X[4], tp3Z[4]);
      polAg = (politic*)polAg->next;
   }

   pf* pfAg = pfs;

   /* Politics Agents */
   for(aux = 0; aux < totalPfs; aux++)
   {
      pfAg->setFederal(tp3X[4], tp3Z[4]);
      pfAg = pfAg->next;
   }

}

/******************************************************************
 *                        redefineCongress                        *
 ******************************************************************/
void agents::redefineCongress(GLfloat x, GLfloat z)
{
   tp3X[3] = x;
   tp3Z[3] = z;

   int aux;
   
   politic* polAg = politics;

   /* Politics Agents */
   for(aux = 0; aux < totalPolitics; aux++)
   {
      polAg->setCongress(tp3X[3], tp3Z[3]);
      polAg = (politic*)polAg->next;
   }
}

/******************************************************************
 *                        redefineBase                            *
 ******************************************************************/
void agents::redefineBase(int groupNum, GLfloat x, GLfloat z)
{
   tp3X[groupNum] = x;
   tp3Z[groupNum] = z;

   int aux;
   
   politic* polAg = politics;

   /* Politics Agents */
   for(aux = 0; aux < totalPolitics; aux++)
   {
      if(polAg->getGroup() == groupNum)
      {
         polAg->setBaseArea(groupNum, tp3X[groupNum]-32, tp3Z[groupNum]-32,
                            tp3X[groupNum]+32, tp3Z[groupNum]+32);
      }
      polAg = (politic*)polAg->next;
   }
}


/******************************************************************
 *                        Verify Action                           *
 ******************************************************************/
void agents::verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                          Uint8 mButton, int tool, Map* actualMap, 
                          mapObjeto** actualObject )
{
   if( (state == AGENTS_STATE_POTENTIAL) && (tool != TOOL_POTENTIAL_ADD))
   {
      if( (actualAgent != NULL))
      {
         removePotentAgent( (potentAgent*) actualAgent);
         actualAgent = NULL;
      }
   }

   if( (state == AGENTS_STATE_ROGUE) && (tool != TOOL_TP3_ROGUE_ADD))
   {
      if( (actualAgent != NULL))
      {
         removePoliticAgent( (politic*) actualAgent);
         actualAgent = NULL;
      }
   }

   if( (state == AGENTS_STATE_POLICE) && (tool != TOOL_TP3_POLICE_ADD))
   {
      if( (actualAgent != NULL))
      {
         removePfAgent( (pattAgent*)actualAgent);
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

      
   if((tool == TOOL_POTENTIAL_ADD))
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
   else if(tool == TOOL_TP3_BRIEF_ADD)
   {
      state = AGENTS_STATE_BRIEFCASE;
      obstacleX = mouseX;
      obstacleZ = mouseZ;

      if(mButton & SDL_BUTTON(1))
      {
         srand(SDL_GetTicks());
         brief->insertBriefCase(mouseX, mouseZ, obstacleOrientation, 
                                (rand() % MAX_VALUE + SUM_VALUE));
         while(mButton & SDL_BUTTON(1))
         {
            //Wait for Mouse Button Release
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }
      }
      else if(mButton & SDL_BUTTON(2))
      {
         obstacleOrientation += 1;
      }

   }
   else if( (tool == TOOL_TP3_FEDERAL_ADD) || (tool == TOOL_TP3_CONGRESS_ADD) ||
            (tool == TOOL_TP3_PSDB_ADD) || (tool == TOOL_TP3_PT_ADD) ||
            (tool == TOOL_TP3_PFL_ADD))
   {
      state = AGENTS_STATE_NONE;
      if(mButton & SDL_BUTTON(1))
      {
         switch(tool)
         {
            case TOOL_TP3_PSDB_ADD:
            case TOOL_TP3_PT_ADD:
            case TOOL_TP3_PFL_ADD:
               redefineBase(tool-TOOL_TP3_PSDB_ADD, mouseX, mouseZ);
            break;
            case TOOL_TP3_FEDERAL_ADD:
               redefineFederal(mouseX, mouseZ);
            break;
            case TOOL_TP3_CONGRESS_ADD:
               redefineCongress(mouseX, mouseZ);
            break;
         }
         while(mButton & SDL_BUTTON(1))
         {
            //Wait for Mouse Button Release
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }
      }
   }
   else if((tool == TOOL_TP3_ROGUE_ADD))
   {
      if(state != AGENTS_STATE_ROGUE)
      {
         addAgent(AGENT_TYPE_ROGUE, mouseX, mouseZ, false, 
                                     1.00, goalX, goalZ, 30, 360);
         state = AGENTS_STATE_ROGUE;
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
      else if(state == AGENTS_STATE_ROGUE)
      {
         actualAgent->definePosition(mouseX, mouseZ);
      }
   }
   else if(tool == TOOL_TP3_POLICE_ADD)
   {
      if( (state != AGENTS_STATE_POLICE) && 
          (state != AGENTS_STATE_POLICE_WAYPOINTS) )
      {
         addAgent(AGENT_TYPE_POLICE, mouseX, mouseZ, true, 
                                     1.0, goalX, goalZ, 30, 360);
         state = AGENTS_STATE_POLICE;
      }
      else if( mButton & SDL_BUTTON(1) )
      {
         if(state == AGENTS_STATE_POLICE)
         {
            state = AGENTS_STATE_POLICE_WAYPOINTS;
            pattAgent* aux = (pattAgent*) actualAgent;
            /* Define Initial Position */
            aux->definePosition(mouseX, mouseZ);
            aux->addWayPoint(mouseX, mouseZ);
         }
         else if(state == AGENTS_STATE_POLICE_WAYPOINTS)
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
      else if( (mButton & SDL_BUTTON(3)) && 
               (state == AGENTS_STATE_POLICE_WAYPOINTS))
      {
         state = AGENTS_STATE_NONE;
         actualAgent = NULL;
      }
      else if(state == AGENTS_STATE_POLICE)
      {
         actualAgent->definePosition(mouseX, mouseZ);
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
   file << "Agents: " << totalPotentAgents << " " << totalPattAgents << 
           " " << totalPolitics << " " << totalPfs << 
           " " << brief->getTotal() << "\n";

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
   
   /* Save All Politics Agents */
   politic* polAg = politics;
   for(i=0; i< totalPolitics; i++)
   {
      polAg->getPosition(x,z);
      polAg->getSight(sightDist, sightAngle);
      if(polAg->oriented())
      {
         oriented = 1;
      }
      else
      {
         oriented = 0;
      }
      file << "Politic: " << polAg->getGroup() << " " << x << " " << z << 
              " " << oriented << " " << polAg->getStepSize() << " " 
              << sightDist << " " << sightAngle << "\n";
      polAg = (politic*) polAg->next;
   }

   /* Save All Pfs Agents */
   pf* p = pfs;
   
   for(i=0; i< totalPfs; i++)
   {
      a = p->patAg;
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

      file << "Pf: " << x << " " << z << " " << oriented << " " << 
              a->getStepSize() << " " << sightDist << " " << sightAngle <<
              " " << numWayPoints << "\n";
      
      wayPoint* wp = a->getWayPoints();
      for(w = 0; w < numWayPoints; w++)
      {
         file << "Way: " << wp->x << " " << wp->z << "\n";
         wp = wp->next;
      }
      p = p->next;
   }

   /* Save All Bases Positions */
   for(i=0; i<5; i++)
   {
      file << "Base: " << tp3X[i] << " " << tp3Z[i] << "\n";
   }

   /* Save All BriefCases */
   briefCase* tmp;
   for(i=0; i<brief->getTotal(); i++)
   {
      tmp = brief->getBriefCase(i);
      file << "BriefCase: " << tmp->x << " " << tmp->z << " " << " " <<
              tmp->angle << " " << tmp->value << "\n";
   }

   file.close();

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
   int numPfs;
   int numPolitics;
   int numCases;
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
   sscanf(aux.c_str(),"Agents: %d %d %d %d %d",&numPotAgents, &numPatAgents, 
          &numPolitics, &numPfs, &numCases);

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

   /* Gets All Politics Agents */
   for(i=0; i< numPolitics; i++)
   {
      int gr;
      getline(file, aux);
      sscanf(aux.c_str(),"Politic: %d %f %f %d %f %f %f", &gr, &x, &z, 
             &oriented, &step, &sightDist, &sightAngle);
      groupAdd = (gr - 1) % 4;
      addAgent(AGENT_TYPE_ROGUE, x, z, oriented == 1, 
               step, goalX, goalZ, sightDist, sightAngle);
      SDL_Delay(20);
   }

   /* Gets All Police PF Agents */
   for(i=0; i< numPfs; i++)
   {
      getline(file, aux);
      sscanf(aux.c_str(),"Pf: %f %f %d %f %f %f %d", &x, &z, &oriented, &step,
                        &sightDist, &sightAngle, &numWayPoints);
      addAgent(AGENT_TYPE_POLICE, x, z, oriented == 1, 
               step, goalX, goalZ, sightDist, sightAngle);
      
      pattAgent* a = (pattAgent*) actualAgent;
      for(w = 0; w < numWayPoints; w++)
      {
         getline(file, aux);
         sscanf(aux.c_str(),"Way: %f %f", &x, &z);
         a->addWayPoint(x, z);
      }
      SDL_Delay(20);
   }

   /* Define All Bases */
   for(i=0; i<5; i++)
   {
      getline(file,aux);
      sscanf(aux.c_str(), "Base: %f %f", &tp3X[i], &tp3Z[i]);
      if(i <= TOOL_TP3_PFL_ADD-TOOL_TP3_PSDB_ADD)
      {
         redefineBase(i, tp3X[i], tp3Z[i]);
      }
      else if( i == TOOL_TP3_CONGRESS_ADD - TOOL_TP3_PSDB_ADD)
      {
         redefineCongress(tp3X[i], tp3Z[i]);
      }
      else
      {
         redefineFederal(tp3X[i], tp3Z[i]);
      }
   }

   /* Insert All BriefCases */
   brief->removeAll();
   int value;
   for(i=0; i<numCases; i++)
   {
      getline(file,aux);
      sscanf(aux.c_str(), "BriefCase: %f %f %f %d", &x, &z, &sightAngle, &value);
      brief->insertBriefCase(x,z, sightAngle, value);
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

