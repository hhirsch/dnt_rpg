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

#include "scenenode.h"
#include "../engine/util.h"

/***********************************************************************
 *                             constructor                             *
 ***********************************************************************/
sceneNode::sceneNode(aniModel* a, GLfloat x, GLfloat y, GLfloat z,
      GLfloat aX, GLfloat aY, GLfloat aZ)
{
   /* No animated (no-owner), only need to set */
   model = a;
   animated = false;
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
   model = new aniModel();

   /* Load and calculate bounding box */
   model->loadModel(modelFileName);

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
      delete(model);
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
 *                           updateBoundingBox                         *
 ***********************************************************************/
void sceneNode::updateBoundingBox()
{
   /* Update the crude */
   if(animated)
   {
      model->calculateCrudeBoundingBox();
   }
   
   /* and rotate/translate bounding box */
   bbox = model->getCrudeBoundingBox();
   bbox.rotate(angleX, angleY, angleZ);
   bbox.translate(posX, posY, posZ);
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
         model->update(WALK_UPDATE, angleY, posX, posY, posZ);
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
      model->renderFromGraphicMemory(posX, posY, posZ, 
            angleX, angleY, angleZ, false);
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
         model->renderReflexion(posX, posY, posZ, angleX, angleY, angleZ);
      }
   }

   /* TODO: the culling for shadow! */
   if( (shadow) && (shadowMatrix) && 
       ( (posY > 0) || ( (posY == 0) && (bbox.y2 > 2) ) ) )
   {
      if(!modelLoaded)
      {
         model->loadToGraphicMemory();
         modelLoaded = true;
      }
      model->renderShadow(posX, posY, posZ, angleX, angleY, angleZ, 
            shadowMatrix, alpha);
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

