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


#ifndef _dnt_scene_h
#define _dnt_scene_h

#include "animodel.h"
#include "scenenode.h"
#include "list.h"

#define ANIMATED_LIST_NAME  "animatedListName"

/*! The render position is just a sceneNode for list */
class renderPosition: public dntListElement
{
   public:
      sceneNode* node;
};

/*! A list of nodes */
class nodesList: public dntListElement, public dntList
{
   public:
      /*! Constructor for a list of animated nodes
       * \note: list name will be ANIMATED_LIST_NAME */
      nodesList();
      /*! Constructor for a list of inanimated nodes of specific model
       * \note: listName will be modelFileName */
      nodesList(string modelFileName);
      /*! Destructor */
      ~nodesList();

      /*! Render all models on list */
      void render(GLfloat** viewMatrix, bool update, bool reflexion,
            bool shadow, GLfloat* shadowMatrix, float alpha);

      /*! Delete scene node from list */
      void deleteSceneNode(sceneNode* node);

      /*! Get the nodes list name */
      string getName(){return(name);};
      /*! Get the nodes aniModel */
      aniModel* getModel(){return(model);};

   protected:
      /*! from list */
      void freeElement(dntListElement* obj);
    
      aniModel* model;  /**< Model loaded or NULL */
      string name;      /**< Name of the list */
};

/*! A list of nodes lists   */
class listNodesList: public dntList
{
    public:
       /*! Constructor */
       listNodesList();
       /*! Destructor */
       ~listNodesList();

       /*! Find a list of name
        * \param name -> name of the list 
        * \return list pointer or NULL */
       nodesList* find(string name);
       /*! Find a list of model
        * \param model -> pointer to the model to find list of
        * \return list pointer or NULL */
       nodesList* find(aniModel* model);

       /*! Render all models on list */
      void render(GLfloat** viewMatrix, bool update, bool reflexion,
            bool shadow, GLfloat* shadowMatrix, float alpha);

    protected:
       void freeElement(dntListElement* obj);
};

/*! A scene is the controller of all 3d models on the game */
class scene
{
   public:
      /*! Constructor */
      scene();
      /*! Destructor */
      ~scene();

      /*! Init the scene to use */
      void init();
      /*! Finish the use of scene */
      void finish();
      /*! Remove All Unused Models */
      void removeUnusedModels();

      /*! Create an scene node  */
      sceneNode* createSceneNode(bool staticModel, string modelFileName, 
            GLfloat x, GLfloat y, GLfloat z,
            GLfloat aX, GLfloat aY, GLfloat aZ);

      /*! Delete an scene node from scene */
      void deleteSceneNode(sceneNode* node);

      /*! Render all models on scene, doing frustum culling */
      void render(GLfloat** viewMatrix, bool update, bool reflexion,
            bool shadow, GLfloat* shadowMatrix, float alpha);

   protected:
      static listNodesList* nLists; /**< List of nodes list */
      static nodesList* animatedList; /**< List of animated models */
      
};

#endif

