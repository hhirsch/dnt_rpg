/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

#include "scenenode.h"
#include "../engine/util.h"
using namespace std;

/***********************************************************************
 *                             constructor                             *
 ***********************************************************************/
sceneNode::sceneNode(aniModel* a, GLfloat x, GLfloat y, GLfloat z,
      GLfloat aX, GLfloat aY, GLfloat aZ)
{
   /* No animated (no-owner), only need to set */
   model = a;
   animated = false;
   updateCrude = false;
   enableRotLast = false;
   set(x, y, z, aX, aY, aZ);
}


/***********************************************************************
 *                             constructor                             *
 ***********************************************************************/
sceneNode::sceneNode(string modelFileName, GLfloat x, GLfloat y, GLfloat z,
      GLfloat aX, GLfloat aY, GLfloat aZ)
{
   /* Animated must have its own model */
   animated = true;
   updateCrude = true;
   enableRotLast = false;

   /* Load the model, based on its type */
   if(modelFileName.find(".md5") != string::npos)
   {
      model = new md5Model();
   }
   else
   {
      model = new cal3DModel();
   }

   /* Load and calculate bounding box */
   model->load(modelFileName);

   /* Now, set its position/angle */
   set(x, y, z, aX, aY, aZ);
}

/***********************************************************************
 *                              destructor                             *
 ***********************************************************************/
sceneNode::~sceneNode()
{
   /* If animated, no more need to keep model */
   if(animated)
   {
      switch(model->getType())
      {
         case aniModel::TYPE_CAL3D:
         {
            cal3DModel* c = (cal3DModel*)model;
            delete(c);
         }
         break;
         case aniModel::TYPE_MD5:
         {
            md5Model* m = (md5Model*)model;
            delete(m);
         }
         break;
         default:
         {
            std::cerr << "Warning: will delete as aniModel: '" 
               << model->getFileName() << "'" << std::endl;
            delete(model);
         }
         break;
      }
   }
}

/***********************************************************************
 *                                 set                                 *
 ***********************************************************************/
void sceneNode::set(GLfloat x, GLfloat y, GLfloat z, 
      GLfloat aX, GLfloat aY, GLfloat aZ)
{
   /* Set things */
   posX = x;
   posY = y;
   posZ = z;
   angleX = aX;
   angleY = aY;
   angleZ = aZ;

   /* Update the bounding box */
   updateBoundingBox();
}

/***********************************************************************
 *                             setPosition                             *
 ***********************************************************************/
void sceneNode::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
   posX = x;
   posY = y;
   posZ = z;
   updateBoundingBox();
}

/***********************************************************************
 *                              setAngle                               *
 ***********************************************************************/
void sceneNode::setAngle(GLfloat aX, GLfloat aY, GLfloat aZ)
{
   angleX = aX;
   angleY = aY;
   angleZ = aZ;
   updateBoundingBox();
}

/***********************************************************************
 *                           setRotationLast                           *
 ***********************************************************************/
void sceneNode::setRotationLast(float angle, float x, float y, float z)
{
   rotLast[0] = angle;
   rotLast[1] = x;
   rotLast[2] = y;
   rotLast[3] = z;
   enableRotLast = true;
}

/***********************************************************************
 *                           updateBoundingBox                         *
 ***********************************************************************/
void sceneNode::updateBoundingBox()
{
   /* Update the crude */
   if( (animated) && 
       ( (updateCrude) || (model->getType() == aniModel::TYPE_MD5) ) )
   {
      model->calculateCrudeBoundingBox();
   }
   
   /* and rotate/translate bounding box */
   bbox = model->getCrudeBoundingBox();
   bbox.rotate(angleX, angleY, angleZ);
   bbox.translate(posX, posY, posZ);
}

/***********************************************************************
 *                      enableCrudeBoundingBox                         *
 ***********************************************************************/
void sceneNode::enableCrudeBoundingBoxUpdate()
{
   updateCrude = true;
}

/***********************************************************************
 *                     disableCrudeBoundingBox                         *
 ***********************************************************************/
void sceneNode::disableCrudeBoundingBoxUpdate()
{
   updateCrude = false;
}

/***********************************************************************
 *                                 render                              *
 ***********************************************************************/
void sceneNode::render(GLfloat** viewMatrix, bool update, bool reflexion, 
      bool shadow, GLfloat* shadowMatrix, float alpha, bool& modelLoaded)
{
   if(animated)
   {
      /* Update animation */
      if(update)
      {
         model->update(WALK_UPDATE, angleY, posX, posY, posZ, 
               bbox.isVisible(viewMatrix));
         if( (updateCrude) || (model->getType() == aniModel::TYPE_MD5) )
         {
            /* Need to recalculate bounding box, based on position *
             * As md5 models have bounding boxes pre-calculated per frame,
             * always get them, ignoring the updateCrude state. */
            updateBoundingBox();
         }
      }
   }

   /* Do the frustum culling for model */
   if(bbox.isVisible(viewMatrix))
   {
      /* Visible, render the model */
      if(!modelLoaded)
      {
         model->loadToGraphicMemory();
         modelLoaded = true;
      }
      if(!enableRotLast)
      {
         model->renderFromGraphicMemory(posX, posY, posZ, 
               angleX, angleY, angleZ, false);
      }
      else
      {
         model->renderFromGraphicMemory(posX, posY, posZ, 
               angleX, angleY, angleZ, rotLast[0], rotLast[1],
               rotLast[2], rotLast[3], false); 
      }
   }

   /* Do the culling for reflexion */
   if(reflexion)
   {
      boundingBox refBox = bbox;
      refBox.notY();
      if(refBox.isVisible(viewMatrix))
      {
         /* Visible, render the reflexion */
         if(!modelLoaded)
         {
            model->loadToGraphicMemory();
            modelLoaded = true;
         }
         if(!enableRotLast)
         {
            model->renderReflexion(posX, posY, posZ, angleX, angleY, angleZ);
         }
         else
         {
            model->renderReflexion(posX, posY, posZ, angleX, angleY, angleZ,
                  rotLast[0], rotLast[1], rotLast[2], rotLast[3]);
         }
      }
   }

   /* The shadow render */
   if( (shadow) && (shadowMatrix) && 
       ( (posY > 0) || ( (posY == 0) && (bbox.max.y > 2) ) ) )
   {
      boundingBox shadBox = bbox;
      shadBox.multiplyShadow(shadowMatrix);
      if(shadBox.isVisible(viewMatrix))
      {
         if(!modelLoaded)
         {
            model->loadToGraphicMemory();
            modelLoaded = true;
         }

         if(!enableRotLast)
         {
            model->renderShadow(posX, posY, posZ, angleX, angleY, angleZ, 
                  shadowMatrix, alpha);
         }
         else
         {
            model->renderShadow(posX, posY, posZ, angleX, angleY, angleZ, 
                  shadowMatrix, alpha, rotLast[0], rotLast[1], 
                  rotLast[2], rotLast[3]);
         }
      }
   }

   if(animated)
   {
      if(modelLoaded)
      {
         /* remove Model from graphic card memory */
         model->removeFromGraphicMemory();
      }
   }
}

