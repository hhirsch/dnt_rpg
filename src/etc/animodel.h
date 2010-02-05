/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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

#ifndef _dnt_animodel_h
#define _dnt_animodel_h

#include <cal3d/cal3d.h>
#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;

#include "../classes/thing.h"

#define STATE_IDLE         0 /**< Character Animation State Idle */
#define STATE_WALK         1 /**< Character Animation State Walk */
#define STATE_DIE          2 /**< Character Animation State Die */
#define STATE_DEAD         3 /**< Character Animation State Dead */
#define STATE_ATTACK_MEELE 4 /**< Character Animation State Meele Attack */
#define STATE_RUN          5 /**< Character Animation State Run */

/*! Animated Model Definition (using cal3d) */
class aniModel:public thing
{
   public:
      /*! Constructor */
      aniModel();
      /*! Destructor */
      ~aniModel();

      /*! Update Model to pos time 
       * \param pos -> time of the animation */
      void update(GLfloat pos);

      /*! Get the model current pos time
       * \return current model pos time */
      GLfloat getCurrentPos();

      /*! Load the cal3D model to be this character's body and animations.
       * \param strFilename -> \c string with the cal3d file name.
       * \return true if success on load. */
      bool loadModel(const string& strFilename);

      /*! Add a model meshes to this one
       * \param mdl -> model to add to this one */
      void addModel(aniModel* mdl);

      /*! Set the animation state of the model.
       * \param state -> state ID to be defined. */
      void setState(int state);

      /*! Gets the animation state of the model.
       * \return state ID of the current animation */
      int getState();

      /*! Load the model to video card memory
       * \param useTexture -> true to use the model texture, false otherwise
       *                      (if false, user must bind the desired texture by
       *                        himself)
       * \note -> must be called before all renderFromGraphicMemory */
      void loadToGraphicMemory(bool useTexture=true);

      /*! Render the loaded Graphic Memory to screen */
      void renderFromGraphicMemory();

      /*! End the renderer State (unLoad the model from videocard).
       * \note -> must be called after all renderFromGraphicMemory */
      void removeFromGraphicMemory();

      /*! Render the model to the current frame state on screen. */
      void render();

      /*! Render the shadow 
       * \param shadowMatrix -> light shadow matrix used
       * \param alpha -> alpha of the shadow
       * \note -> need to define the stencil to the projected surface
       * \note -> this function works as renderFromGraphicMemory()  */
      void renderShadow(GLfloat* shadowMatrix, float alpha);

      /*! Calculate the model static bounding box */
      void calculateBoundingBox();

      /*! Render the model bounding box. Only used on debugging */
      void renderBoundingBox();

      /*! Verify, with all Meshes of the model, if the model colides
       *  with the bounding box defined as parameter. 
       *  \param angle -> model angle
       *  \param pX -> model X position
       *  \param pY -> model Y position
       *  \param pZ -> model Z position
       *  \param colMin -> colider min values of bounding box
       *  \param colMax -> colider max values of bounding box 
       *  \return -> true if one or more meshes colides, false otherwise */
      bool depthCollision(GLfloat angle, GLfloat pX, GLfloat pY, GLfloat pZ,
                          GLfloat colMin[3], GLfloat colMax[3]);

   protected:
      int m_state;                   /**< current animation state */
      CalCoreModel* m_calCoreModel;  /**< Cal3D Core Model of character */
      CalModel* m_calModel;          /**< Cal3D Model of character */
      int m_animationId[16];         /**< Cal3D animation ID */
      int m_animationCount;          /**< Cal3D number of animations */
      int m_meshId[32];              /**< Cal3D meshes ID */
      int m_meshCount;               /**< Cal3D meshes count */
      GLuint m_textureId[32];        /**< Cal3D texture ID */
      int m_textureCount;            /**< Cal3D texture Count */
      float m_motionBlend[3];        /**< Cal3D motion blend */
      float m_renderScale;           /**< Cal3D scale on render */
      float m_lodLevel;              /**< Cal3D Level of Detail to render */
      float curPos;                  /**< current time pos */
      string m_path;                 /**< Path to cal3D model */
      string modelFileName;          /**< Filename of the model */

      CalRenderer *pCalRenderer;     /**< Pointer to themodel renderer */
      static float meshVertices[30000][3]; /**< Model Vertices */
      static float meshNormals[30000][3];  /**< Model Normals */
      static float meshTextureCoordinates[30000][2]; /**< Texture Coords */
      static CalIndex meshFaces[50000][3]; /**< Model Faces */
      int faceCount;                       /**< Number of Faces */
      int textureCoordinateCount;          /**< Number of Texture Coordinates */

      /*! Load the a texture to the model.
       * \param strFilename -> \c string with the texture file name.
       * \return the \c GLuint with the GL texture ID. */
      GLuint loadTexture(const string& strFilename);
};

#endif

