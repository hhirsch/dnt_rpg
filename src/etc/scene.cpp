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

#include "scene.h"
using namespace std;

/***********************************************************************
 *                             constructor                             *
 ***********************************************************************/
nodesList::nodesList()
{
   name = ANIMATED_LIST_NAME;
   model = NULL;
}

/***********************************************************************
 *                             constructor                             *
 ***********************************************************************/
nodesList::nodesList(string modelFileName)
{
   name = modelFileName;

   /* Load the model, based on its type */
   if(modelFileName.find(".md5") != string::npos)
   {
      model = new md5Model();
   }
   else
   {
      model = new cal3DModel();
   }

   model->load(modelFileName);
   model->calculateCrudeBoundingBox();
}

/***********************************************************************
 *                              destructor                             *
 ***********************************************************************/
nodesList::~nodesList()
{
   clearList();
   if(model)
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
            delete(model);
         }
         break;
      }
   }
}

/***********************************************************************
 *                            getSceneNode                             *
 ***********************************************************************/
sceneNode* nodesList::getSceneNode(boundingBox& scBox)
{
   int i;
   renderPosition* rp = (renderPosition*)getFirst();

   /* Verify at animated list */
   for(i=0; i < total; i++)
   {
      if(rp->node->getBoundingBox().intercepts(scBox))
      {
         /* Interceptd... must return */
         return(rp->node);
      }
      rp = (renderPosition*)rp->getNext();
   }

   return(NULL);
}

/***********************************************************************
 *                          deleteSceneNode                            *
 ***********************************************************************/
void nodesList::deleteSceneNode(sceneNode* node)
{
   /* search for the node */
   int i;
   renderPosition* sc = (renderPosition*)first;

   /* Find list */
   for(i=0; i < total; i++)
   {
      if(sc->node == node)
      {
         remove(sc);
         return;
      }
      sc = (renderPosition*)sc->getNext();
   }

   /* node found on list! */
   cerr << "Couldn't find sceneNode '" << node << "' on list!" << endl;
   delete(node);
}

/***********************************************************************
 *                                render                               *
 ***********************************************************************/
void nodesList::render(GLfloat** viewMatrix, bool update, bool reflexion,
      bool shadow, GLfloat* shadowMatrix, float alpha)
{
   bool modelLoaded = false;
   int i;
   renderPosition* pos = (renderPosition*)first; 

   /* Let's render all */
   for(i=0; i < total; i++)
   {
      pos->node->render(viewMatrix, update, reflexion, 
            shadow, shadowMatrix, alpha, modelLoaded);
      if(!model)
      {
         /* Animated list, set to false to force load of next model */
         modelLoaded = false;
         /* Debug render bounding box */
         //pos->node->getBoundingBox().render();
      }
      pos = (renderPosition*)pos->getNext();
   }

   if( (modelLoaded)  && (model) )
   {
      /* If static and loaded, must remove from graphic memory */
      model->removeFromGraphicMemory();
   }      
}

/***********************************************************************
 *                             freeElements                            *
 ***********************************************************************/
void nodesList::freeElement(dntListElement* obj)
{
   renderPosition* r = (renderPosition*)obj;
   /* Delete the scene node */
   delete(r->node);
   delete(r);
}

/***********************************************************************
 *                             constructor                             *
 ***********************************************************************/
listNodesList::listNodesList()
{
}

/***********************************************************************
 *                              destructor                             *
 ***********************************************************************/
listNodesList::~listNodesList()
{
   clearList();
}

/***********************************************************************
 *                                 find                                *
 ***********************************************************************/
nodesList* listNodesList::find(string name)
{
   int i;
   nodesList* l = (nodesList*)first;

   /* Find list */
   for(i=0; i < total; i++)
   {
      if(l->getName() == name)
      {
         return(l);
      }
      l = (nodesList*)l->getNext();
   }

   return(NULL);
}

/***********************************************************************
 *                                 find                                *
 ***********************************************************************/
nodesList* listNodesList::find(aniModel* model)
{
   int i;
   nodesList* l = (nodesList*)first;

   /* Find list */
   for(i=0; i < total; i++)
   {
      if(l->getModel() == model)
      {
         return(l);
      }
      l = (nodesList*)l->getNext();
   }

   return(NULL);
}

/***********************************************************************
 *                               render                                *
 ***********************************************************************/
void listNodesList::render(GLfloat** viewMatrix, bool update, bool reflexion,
      bool shadow, GLfloat* shadowMatrix, float alpha)
{
   int i;
   nodesList* l = (nodesList*)first;

   /* Find list */
   for(i=0; i < total; i++)
   {
      l->render(viewMatrix, update, reflexion, shadow, shadowMatrix, alpha);
      l = (nodesList*)l->getNext();
   }
}

/***********************************************************************
 *                            getSceneNode                             *
 ***********************************************************************/
sceneNode* listNodesList::getSceneNode(boundingBox& scBox)
{
   int i;
   nodesList* l = (nodesList*)getFirst();
   sceneNode* scNode=NULL;

   /* Verify at animated list */
   for(i=0; i < total; i++)
   {
      /* Verify interception on this list */
      scNode = l->getSceneNode(scBox);
      if(scNode)
      {
         return(scNode);
      }

      /* None... must verify next */
      l = (nodesList*)l->getNext();
   }

   return(NULL);
}


/***********************************************************************
 *                             freeElements                            *
 ***********************************************************************/
void listNodesList::freeElement(dntListElement* obj)
{
   nodesList* l = (nodesList*)obj;
   delete(l);
}


/***********************************************************************
 *                             constructor                             *
 ***********************************************************************/
scene::scene()
{
}

/***********************************************************************
 *                              destructor                             *
 ***********************************************************************/
scene::~scene()
{
}

/***********************************************************************
 *                                  init                               *
 ***********************************************************************/
void scene::init()
{
   nLists = new listNodesList();
   animatedList = new nodesList();
}

/***********************************************************************
 *                                  finish                             *
 ***********************************************************************/
void scene::finish()
{
   delete(nLists);
   delete(animatedList);
}

/***********************************************************************
 *                        removeUnusedModels                           *
 ***********************************************************************/
void scene::removeUnusedModels()
{
   int i;
   int total = nLists->getTotal();
   nodesList* l=NULL, *aux=NULL;

   /* Search for lists without sceneNodes */
   l = (nodesList*)nLists->getFirst();
   for(i=0; i < total; i++)
   {
      if(l->getTotal() == 0)
      {
         aux = l;
         l = (nodesList*)l->getNext();
         nLists->remove(l->getPrevious());
      }
      else
      {
         l = (nodesList*)l->getNext();
      }
   }
}

/***********************************************************************
 *                               render                                *
 ***********************************************************************/
void scene::render(GLfloat** viewMatrix, bool update, bool reflexion,
      bool shadow, GLfloat* shadowMatrix, float alpha)
{
   nLists->render(viewMatrix, update, reflexion, shadow, shadowMatrix, alpha);
   animatedList->render(viewMatrix, update, reflexion, shadow, shadowMatrix,
         alpha);
}

/***********************************************************************
 *                          createSceneNode                            *
 ***********************************************************************/
sceneNode* scene::createSceneNode(bool staticModel, string modelFileName,
      GLfloat x, GLfloat y, GLfloat z, GLfloat aX, GLfloat aY, GLfloat aZ)
{
   renderPosition* rp = new renderPosition;
   rp->node = NULL;

   if(staticModel)
   {
      /* Verify if a list exists for this model */
      nodesList* l = nLists->find(modelFileName);
      if(!l)
      {
         /* No list for it, must create a new one */
         l = new nodesList(modelFileName);
         nLists->insert(l);
      }
      /* Create and insert the new sceneNode */
      rp->node = new sceneNode(l->getModel(), x,y,z, aX,aY,aZ);
      l->insert(rp);
   }
   else
   {
      /* Create the scene node & render position */
      rp->node = new sceneNode(modelFileName, x,y,z, aX,aY,aZ);

      /* insert it at the animation list */
      animatedList->insert(rp);
   }

   return(rp->node);
}

/***********************************************************************
 *                            getSceneNode                             *
 ***********************************************************************/
sceneNode* scene::getSceneNode(boundingBox& scBox)
{
   sceneNode* scNode = NULL;

   /* Verify at animated list */
   scNode = animatedList->getSceneNode(scBox);

   /* Verify at static nodes list */
   if(!scNode)
   {
      scNode = nLists->getSceneNode(scBox);
   }

   return(scNode);
}

/***********************************************************************
 *                          deleteSceneNode                            *
 ***********************************************************************/
void scene::deleteSceneNode(sceneNode* node)
{
   if(node->isAnimated())
   {
      animatedList->deleteSceneNode(node);
   }
   else
   {
      nodesList* l = nLists->find(node->getModel());
      if(l)
      {
         l->deleteSceneNode(node);
      }
      else
      {
         cerr << "Couldn't find list for node! " << node->getModel() << endl;
         delete(node);
      }
   }
}


listNodesList* scene::nLists=NULL;
nodesList* scene::animatedList=NULL;

