/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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

#ifndef _dnt_modelslist_h
#define _dnt_modelslist_h

#include "animodel.h"
#include <string>
using namespace std;

/*! The Bounding Box Class */
class boundingBox
{
   public:
      GLfloat x1,
              z1,
              y1,
              x2,
              z2,
              y2;
};

/*! A render Positon of a Scenery Static Model Object */
class sceneryRenderPosition: public dntListElement
{
   public:
      GLfloat x,        /**< x coordinate */
              y,        /**< y coordinate */
              z;        /**< z coordinate */
      GLfloat angle;    /**< scenery angle */
};

/*! List of sceneryRenderPositions */
class sceneryRenderList: public dntList
{
   public:
      /*! Constructor */
      sceneryRenderList();
      /*! Destructor */
      ~sceneryRenderList();
   protected:
      /*! Free sceneryRenderPosition memory
       * \param obj -> pointer to free */
      void freeElement(dntListElement* obj);
};

/*! The 3D model of an object */
class model3d: public aniModel, public dntListElement
{
   public:
      /*! Constructor
       * \param path -> string containing the path of model 
       * \param texturePath -> path to all textures of the model
       * \param staticScenery -> if the model is a static scenery */
      model3d(string path, string texturePath, bool staticScenery);

      /*! Destructor */
      ~model3d();

      /*! Draw Object on World
       * \note -> non static scenery ones */
      void draw();

      /*! Draw All Visible Static Scenery Objects Based on model
       * \param matrix -> visible matrix
       * \param inverted -> if inverted on Y axys. Usefull for reflexion.
       * \param shadowMatrix -> the shadow matrix to render projective
       *                        shadows (NULL to not render shadows) 
       * \param alpha -> the alpha value for projective shadows */
      void draw(GLfloat** matriz, bool inverted, GLfloat* shadowMatrix=NULL,
                float alpha=0.0f);

      /*! Increment the used flag */
      void incUsed();

      /*! Decrement the used flag */
      void decUsed();

      /*! Get used Flag Value
       * \return actual value of the used flag. */
      int getUsedFlag();

      /*! Gets the model filename
       * \return string with full filename of the model */
      string getFileName();

      /*! Gets the Bounding box of the model */
      boundingBox getBoundingBox();

      /*! Verify if the model is static scenery or not
       * \return -> true if the model is a static scenery */
      bool isStaticScenery();

      /*! Add a render position.
       * \note -> this only affects staticScenery objects 
       * \param x -> x coordinate
       * \param y -> y coordinate
       * \param z -> z coordinate 
       * \param angle -> orientation angle */
      void addPosition(float x, float y, float z, float angle);

      /*!
       **********************************************/
       void callDeadAnimation();

      /*!
       **********************************************/
       void callAttackAnimation();

      /*!
       **********************************************/
       void callIdleAnimation();

       /*! Just for thing compatibility  */
       void updateHealthBar();
      
   protected:
      string fileName;   /**< Nome of the Model's file */

      int usedFlag;      /**< The used flag. When used is 0, remove the 
                              object from game models list. Each thing that 
                              is with the objecy (like map is with object, 
                              inventory is with it, etc) will need to increment
                              the used flag in the moment they take the object,
                              and decrement the used flag when is no more 
                              using them. */

      bool staticFlag;   /**< The Static Flag: if this flag is used, the 
                              model is static, and can be utilized by more
                              than one entity.
                              \note -> this flag is only used for non-pickeable 
                              scenery objects and optimize a lot their render*/

      sceneryRenderList* positions;  /**< Positions List for 'static' objects */
};

/*! Implementation of a model3d list */
class model3dList: public dntList
{
   public:
      /*! Constructor */
      model3dList();
      /*! Destructor */
      ~model3dList();

   protected:
      /*! Delete a model3 */
      void freeElement(dntListElement* obj);
};

/*! List of all 3D models actually on game */
class modelList
{
   public:
      /*! Constructor */
      void init();
      /*! Destructor */
      void finish();

      /*! Add Model to the List
       * \param path -> string containing the path of model
       * \param texturePath -> path to all textures of the model
       * \param staticScenery -> true if object is a static scenery
       * \return -> pointer to the added model (NULL if not added) */
      model3d* addModel(string path, string texturePath, bool staticScenery);

      /*! Find Model on the List
       * \param path -> string containing the path of model
       * \return -> pointer to the founded model (NULL if not found) */
      model3d* findModel(string path);

      /*! Remove Model from List
       * \param mdl -> model to remove */
      void removeModel(model3d* mdl);

      /*! Remove All Unused Models */
      void removeUnusedModels();

      /*! Get First MOdel on List (usually used on map editor application) */
      model3d* getFirst();

      /*! Get Total Models on list
       * \return -> number of models on the list */
      int getTotalModels();

      /*! Render all Scenery Objects
       * \param visibleMatrix -> current visible matrix
       * \param inverted -> if will render XZ reflexions too
       * \param shadowMatrix -> the shadow matrix to render projective
       *                        shadows (NULL to not render shadows)
       * \param alpha -> the shadow alpha value */
      void renderSceneryObjects(GLfloat** visibleMatrix, bool inverted,
                                GLfloat* shadowMatrix=NULL, float alpha=0.0f);

      /*! Print all Models with printf for debug! */
      void printAll();

   protected:
      static model3dList* list;      /**< List of model3d */
};

#endif

