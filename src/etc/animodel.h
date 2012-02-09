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

#ifndef _dnt_animodel_h
#define _dnt_animodel_h

#include <cal3d/cal3d.h>
#include <SDL/SDL_opengl.h>
#include <string>

#include "boundingbox.h"

#define STATE_IDLE         0 /**< Character Animation State Idle */
#define STATE_WALK         1 /**< Character Animation State Walk */
#define STATE_DIE          2 /**< Character Animation State Die */
#define STATE_DEAD         3 /**< Character Animation State Dead */
#define STATE_ATTACK_MEELE 4 /**< Character Animation State Meele Attack */
#define STATE_RUN          5 /**< Character Animation State Run */
#define STATE_HIT          6 /**< Character Animation State Hit */
#define STATE_GUN_USE      7 /**< Character Animation State Gun Use */

#define ANIMODEL_MAX_ANIMATIONS 16

/*! Animated Model Definition (using cal3d) */
class aniModel
{
   public:

      /*! Vertex info for some key vertices */
      struct vertexInfo
      {
         int vertexId;   /**< Id of the vertex */
         int subMeshId;  /**< Id of the subMesh the vertex is */
         int meshId;     /**< Id of the mesh the vertex is */

         float x;  /**< Current x position */
         float y;  /**< Current y position */
         float z;  /**< current z position */

         float iX; /**< Initial x position */
         float iY; /**< Initial y position */
         float iZ; /**< Initial z position */

         float angleXY; /**< current X/Y angle */
         float angleYZ; /**< current Y/Z angle */
      };

      /*! Constructor */
      aniModel();
      /*! Destructor */
      ~aniModel();

      /*! Update Model to pos time 
       * \param pos -> time of the animation */
      void update(GLfloat pos, float angleY, float pX, float pY, float pZ);

      /*! Get the model current pos time
       * \return current model pos time */
      GLfloat getCurrentPos();

      /*! Load the cal3D model to be this character's body and animations.
       * \param strFilename -> \c string with the cal3d file name.
       * \return true if success on load. */
      bool loadModel(const std::string& strFilename);

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
      /*! Render the loaded Graphic Memory to screen at the position
       * \param inverted -> if will invert Y axys */
      void renderFromGraphicMemory(float pX, float pY, float pZ, float angleX,
            float angleY, float angleZ, bool inverted);
      void renderFromGraphicMemory(float pX, float pY, float pZ, 
            float angleX, float angleY, float angleZ, 
            float angle, float aX, float aY, float aZ, bool inverted);

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
      void renderShadow(float pX, float pY, float pZ, float angleX,
            float angleY, float angleZ, GLfloat* shadowMatrix, float alpha);
      void renderShadow(float pX, float pY, float pZ, float angleX,
            float angleY, float angleZ, GLfloat* shadowMatrix, float alpha,
            float angle, float aX, float aY, float aZ);

      /*! Render the model reflexion on floor axys */
      void renderReflexion(float pX, float pY, float pZ, float angleX, 
            float angleY, float angleZ);
      void renderReflexion(float pX, float pY, float pZ, float angleX,
            float angleY, float angleZ,
            float angle, float aX, float aY, float aZ);

      /*! Calculate the model bounding box for current animation frame */
      void calculateCrudeBoundingBox();
      /*! Get the last calculated bounding box */
      boundingBox getCrudeBoundingBox();

      /*! Render the model bounding box. Only used on debugging */
      void renderBoundingBox();

      /*! Verify, with all Meshes of the model, if the model colides
       *  with the bounding box defined as parameter. 
       *  \param pX -> model X position
       *  \param pY -> model Y position
       *  \param pZ -> model Z position
       *  \param colBox -> colider bounding box
       *  \return -> true if one or more meshes colides, false otherwise */
      bool depthCollision(GLfloat angleX, GLfloat angleY, GLfloat angleZ, 
            GLfloat pX, GLfloat pY, GLfloat pZ, boundingBox colBox);

      /*! Define all key vertices (left and right hand, for example) */
      void defineKeyVertex();

      /*! Update the vertex info */
      void updateKeyVertex(vertexInfo& v,
         float angleY, float pX, float pY, float pZ);

      /*! Get the first vertex id influenced by a bone
       * \param boneId -> id of the bone
       * \param inf -> will receive the vertex indexes
       * \return true if found */
      bool getInfluencedVertex(int boneId, vertexInfo& inf);

      /*! Get the Id of a bone
       * \param bName name of the bone to get its it
       * \return bone's ID or -1 if not found */
      int getBoneId(std::string bName);

      /*! Get the model fileName */
      std::string getFileName(){return(modelFileName);};

      vertexInfo leftHand;           /**< Base vertex at left hand */
      vertexInfo rightHand;          /**< Base vextex at right hand */
      vertexInfo head;               /**< Base vertex at head */

   protected:
      int m_state;                   /**< current animation state */
      CalCoreModel* m_calCoreModel;  /**< Cal3D Core Model of character */
      CalModel* m_calModel;          /**< Cal3D Model of character */
      int m_animationId[ANIMODEL_MAX_ANIMATIONS]; /**< Cal3D animation ID */
      int m_animationCount;          /**< Cal3D number of animations */
      int m_meshId[32];              /**< Cal3D meshes ID */
      int m_meshCount;               /**< Cal3D meshes count */
      GLuint m_textureId[32];        /**< Cal3D texture ID */
      int m_textureCount;            /**< Cal3D texture Count */
      float m_motionBlend[3];        /**< Cal3D motion blend */
      float m_renderScale;           /**< Cal3D scale on render */
      float m_lodLevel;              /**< Cal3D Level of Detail to render */
      float curPos;                  /**< current time pos */
      std::string m_path;            /**< Path to cal3D model */
      std::string modelFileName;     /**< Filename of the model */

      CalRenderer *pCalRenderer;     /**< Pointer to themodel renderer */
      static float meshVertices[30000][3]; /**< Model Vertices */
      static float meshNormals[30000][3];  /**< Model Normals */
      static float meshTextureCoordinates[30000][2]; /**< Texture Coords */
      static CalIndex meshFaces[50000][3]; /**< Model Faces */
      int faceCount;                       /**< Number of Faces */
      int textureCoordinateCount;          /**< Number of Texture Coordinates */

      boundingBox crudeBox;                /**< Crude bounding box */

      /*! Load the a texture to the model.
       * \param strFilename -> \c string with the texture file name.
       * \return the \c GLuint with the GL texture ID. */
      GLuint loadTexture(const std::string& strFilename);

      /*! Render from the graphic memory at default position */
      void renderFromGraphicMemory();
};

#endif

