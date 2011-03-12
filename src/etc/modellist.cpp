/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
using namespace std;

#include "modellist.h"
#include "../engine/culling.h"
#include "../engine/util.h"

/*************************************************************************
 *                           sceneryRenderList                           *
 *************************************************************************/

/********************************************************
 *                     Constructor                      *
 ********************************************************/
sceneryRenderList::sceneryRenderList()
{
}

/********************************************************
 *                     Destructor                       *
 ********************************************************/
sceneryRenderList::~sceneryRenderList()
{
   clearList();
}

/********************************************************
 *                     freeElement                      *
 ********************************************************/
void sceneryRenderList::freeElement(dntListElement* obj)
{
   sceneryRenderPosition* pos = (sceneryRenderPosition*)obj;
   delete(pos);
}

/*************************************************************************
 *                              model3dList                              *
 *************************************************************************/

/********************************************************
 *                     Constructor                      *
 ********************************************************/
model3dList::model3dList()
{
}

/********************************************************
 *                      Destructor                      *
 ********************************************************/
model3dList::~model3dList()
{
   clearList();
}

/********************************************************
 *                     freeElement                      *
 ********************************************************/
void model3dList::freeElement(dntListElement* obj)
{
   model3d* m = (model3d*)obj;
   delete(m);
}

/*************************************************************************
 *                               model3D                                 *
 *************************************************************************/

/********************************************************
 *                     Constructor                      *
 ********************************************************/
model3d::model3d(string path, string texturePath, bool staticScenery)
{
   fileName = path;
   loadModel(path);
   usedFlag = 0;
   staticFlag = staticScenery;
   positions = new sceneryRenderList();
}

/********************************************************
 *                     Destructor                       *
 ********************************************************/
model3d::~model3d()
{
   if(positions != NULL)
   {
      delete(positions);
   }
}

/********************************************************
 *                   isStaticScenery                    *
 ********************************************************/
bool model3d::isStaticScenery()
{
   return(staticFlag);
}

/********************************************************
 *                      addPosition                     *
 ********************************************************/
void model3d::addPosition(float x, float y, float z, float angleX, 
      float angleY, float angleZ)
{
   /* Create Position */
   sceneryRenderPosition* pos = new sceneryRenderPosition();
   pos->x = x;
   pos->y = y;
   pos->z = z;
   pos->angleY = angleY;
   pos->angleX = angleX;
   pos->angleZ = angleZ;

   /* Add it to the list */
   positions->insert(pos);
}

/********************************************************
 *                   drawBoundingBoxes                  *
 ********************************************************/
void model3d::drawBoundingBoxes(GLfloat** matriz) 
{
   boundingBox bound;
   int i;
   
   sceneryRenderPosition* pos = (sceneryRenderPosition*)positions->getFirst();

   /* Render All Models */
   for(i=0; i < positions->getTotal(); i++)
   {
      xPosition = pos->x;
      yPosition = pos->y;
      zPosition = pos->z;
      orientationY = pos->angleY;
      orientationX = pos->angleX;
      orientationZ = pos->angleZ;

      /* Do View Frustum Culling */
      bound = getBoundingBox();
      bound.rotate(pos->angleX, pos->angleY, pos->angleZ);
      bound.translate(pos->x, pos->y, pos->z);

      if(bound.isVisible(matriz))
      {
         bound.render();
      }
      pos = (sceneryRenderPosition*)pos->getNext();
   }
}

/********************************************************
 *                        draw                          *
 ********************************************************/
void model3d::draw(GLfloat** matriz, bool inverted, GLfloat* shadowMatrix,
                   float alpha)
{
   GLfloat min[3], max[3];
   GLfloat X[4], Z[4];
   boundingBox bound;
   int i;
   
   sceneryRenderPosition* pos = (sceneryRenderPosition*)positions->getFirst();

   /* Load Model To Graphic Card Memory */
   loadToGraphicMemory();

   /* Render All Models */
   for(i=0; i < positions->getTotal(); i++)
   {
      xPosition = pos->x;
      yPosition = pos->y;
      zPosition = pos->z;
      orientationY = pos->angleY;
      orientationX = pos->angleX;
      orientationZ = pos->angleZ;

      /* Do View Frustum Culling */
      bound = getBoundingBox();
      X[0] = bound.x1;
      Z[0] = bound.z1;
      X[1] = bound.x1;
      Z[1] = bound.z2; 
      X[2] = bound.x2;
      Z[2] = bound.z2;
      X[3] = bound.x2;
      Z[3] = bound.z1;
      rotTransBoundingBox(pos->angleY, X, Z, pos->x, 
            pos->y+bound.y1, pos->y+bound.y2, 
            pos->z, min, max );
      if( (matriz == NULL) ||
            (visibleCube(min[0],min[1],min[2],max[0],max[1],max[2],
                         matriz)) )
      {
         /* Is visible, so render */
         glPushMatrix();
         glTranslatef(xPosition, yPosition, zPosition);
         glRotatef(orientationZ, 0, 0, 1);
         glRotatef(orientationX, 1, 0, 0);
         glRotatef(orientationY, 0, 1, 0);
         renderFromGraphicMemory();
         glPopMatrix();
      }

      /* Reflected Draw */
      if( (inverted) && (pos->y >= 0) )
      {
         /* Do Clulling */
         rotTransBoundingBox(pos->angleY, X, Z, pos->x, 
               pos->y-bound.y2, pos->y-bound.y1, 
               pos->z, min, max);
         if(visibleCube(min[0],min[1],min[2],max[0],max[1],max[2],
                  matriz))
         {
            /* Is visible, so render the reflexion */
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_EQUAL, 1, 0xffffffff);  /* draw if ==1 */
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glEnable(GL_NORMALIZE);
            glPushMatrix();
            glTranslatef(xPosition, -yPosition, zPosition);
            glRotatef(orientationZ, 0, 0, 1);
            glRotatef(orientationX, 1, 0, 0);
            glRotatef(orientationY, 0, 1, 0);
            glScalef(1.0,-1.0,1.0);
            renderFromGraphicMemory();
            glPopMatrix();
            glDisable(GL_NORMALIZE);
            glDisable(GL_STENCIL_TEST);
         }
      }

      /* Do Projective Shadow */
      //FIXME: apply culling too!
      if( (shadowMatrix != NULL) && 
          ( (pos->y > 0) || ( (pos->y == 0) && (max[1] > 2) ) ) )
      {
         orientationY = pos->angleY;
         glPushMatrix();
         renderShadow(shadowMatrix, alpha);
         glPopMatrix();
      }

      pos = (sceneryRenderPosition*)pos->getNext();
   }

   /* remove Model from graphic card memory */
   removeFromGraphicMemory();

   /* Render bounding boxes */
   drawBoundingBoxes(matriz);
}

/********************************************************
 *                        draw                          *
 ********************************************************/
void model3d::draw()
{
   render();
   if(staticFlag)
   {
      /*cerr << "Warning: Rendering Static as Non Static!" << endl;*/
   }
}

/********************************************************
 *                      incUsed                         *
 ********************************************************/
void model3d::incUsed()
{
   usedFlag++;
}

/********************************************************
 *                       decUsed                        *
 ********************************************************/
void model3d::decUsed()
{
   usedFlag--;
   if(usedFlag < 0)
   {
      cerr << "Used flag for model '" << fileName << "' is underflow!" << endl;
   }
}

/********************************************************
 *                     getUsedFlag                      *
 ********************************************************/
int model3d::getUsedFlag()
{
   return(usedFlag);
}

/********************************************************
 *                     getFileName                      *
 ********************************************************/
string model3d::getFileName()
{
   return(fileName);
}

/********************************************************
 *                    getBoundingBox                    *
 ********************************************************/
boundingBox model3d::getBoundingBox()
{
   boundingBox b;
   calculateBoundingBox();
   b.x1 = min[0];
   b.x2 = max[0];
   b.z1 = min[2];
   b.z2 = max[2];
   b.y1 = min[1];
   b.y2 = max[1];
   return(b);
}

void model3d::callDeadAnimation()
{
   //TODO
}

void model3d::callAttackAnimation()
{
   //TODO
}

void model3d::callIdleAnimation()
{
   //TODO
}

void model3d::updateHealthBar()
{
}


/*************************************************************************
 *                              modelList                                *
 *************************************************************************/

/********************************************************
 *                     Constructor                      *
 ********************************************************/
void modelList::init()
{
   list = new model3dList();
}

/********************************************************
 *                     Destructor                       *
 ********************************************************/
void modelList::finish()
{
   if(list)
   {
      delete(list);
   }
}

/********************************************************
 *                     findModel                       *
 ********************************************************/
model3d* modelList::findModel(string path)
{
   model3d* mdl = (model3d*)list->getFirst();
   int i;
   for(i=0; i < list->getTotal(); i++)
   {
      if(mdl->getFileName() == path)
      {
         return(mdl);
      }
      mdl = (model3d*)mdl->getNext();
   }
   return(NULL);
}

/********************************************************
 *                       addModel                       *
 ********************************************************/
model3d* modelList::addModel(string path, string texturePath,
                             bool staticScenery)
{
   model3d* mdl = findModel(path);
   if(mdl)
   {
      /* Already Inserted! */
      return(mdl);
   }

   /* Create model */
   mdl = new model3d(path, texturePath, staticScenery);

   /* Insert it on the list */
   list->insert(mdl);

   return(mdl);
}

/********************************************************
 *                     removeModel                      *
 ********************************************************/
void modelList::removeModel(model3d* mdl)
{
   if(mdl != NULL)
   {
      list->remove(mdl);
   }
}

/********************************************************
 *                       getFirst                       *
 ********************************************************/
model3d* modelList::getFirst()
{
   return((model3d*)list->getFirst());
}

/********************************************************
 *                    getTotalModels                    *
 ********************************************************/
int modelList::getTotalModels()
{
   return(list->getTotal());
}

/********************************************************
 *                  removeUnusedModels                  *
 ********************************************************/
void modelList::removeUnusedModels()
{
   model3d* mdl = (model3d*)list->getFirst();
   model3d* aux = NULL;
   int i, total;

   total = list->getTotal();
   
   for(i = 0; i < total; i++)
   {
      if(mdl->getUsedFlag() <= 0)
      {
         aux = mdl;
         mdl = (model3d*)mdl->getNext();
         removeModel(aux);
      }
      else
      {
         mdl = (model3d*)mdl->getNext();
      }
   }
#ifdef DEBUG   
   printAll();
#endif   
}

/********************************************************************
 *                        RenderSceneryObjects                      *
 ********************************************************************/
void modelList::renderSceneryObjects(GLfloat** visibleMatrix, bool inverted,
                                     GLfloat* shadowMatrix, float alpha)
{
   model3d* mdl = getFirst();
   int i;
   for(i = 0; i< getTotalModels(); i++)
   {
      /* Only Render here the Static Scenery Objects */
      if(mdl->isStaticScenery())
      {
         mdl->draw(visibleMatrix, inverted, shadowMatrix, alpha);
      }
      mdl = (model3d*)mdl->getNext();
   }
}

/********************************************************
 *                       printAll                       *
 ********************************************************/
void modelList::printAll()
{
   int i;
   model3d* mdl = (model3d*)list->getFirst();
   cerr << "*****************************************************" << endl;
   cerr << "*              Current Models on List               *" << endl;
   for(i = 0; i < list->getTotal(); i++)
   {
      cerr << "* Obj: "<< mdl->getFileName() << endl;
      cerr << "* Used for: " << mdl->getUsedFlag() << endl;
      mdl = (model3d*)mdl->getNext();
   }
   cerr << "*****************************************************"<<endl<<endl;
}

/********************************************************
 *                   static members                     *
 ********************************************************/
model3dList* modelList::list = NULL;

