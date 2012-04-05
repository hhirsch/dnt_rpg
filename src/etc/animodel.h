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
#include "shader.h"
#include "extensions.h"

typedef float vector2f_t[2];
typedef float vector3f_t[3];
typedef int vector3i_t[3];

/*! The animodel material definition */
class aniModelMaterial
{
   public:
      /*! Material Constructor */
      aniModelMaterial()
      {
         ambient[0]=1.0f;ambient[1]=1.0f;ambient[2]=1.0f;ambient[3]=1.0f;
         diffuse[0]=1.0f;diffuse[1]=1.0f;diffuse[2]=1.0f;diffuse[3]=1.0f;
         specular[0]=1.0f;specular[1]=1.0f;specular[2]=1.0f;specular[3]=1.0f;
         emission[0]=0.5f;emission[1]=0.5f;emission[2]=0.5f;emission[3]=1.0f;
         shininess = 0.0f;
         normalMap = false;
      };

      GLfloat ambient[4];   /**< Ambient color */
      GLfloat diffuse[4];   /**< Diffuse color */
      GLfloat specular[4];  /**< Specular color */
      GLfloat emission[4];  /**< Color emission */
      GLfloat shininess;    /**< Shininess factor */

      GLuint textureId;     /**< GL id of texture used */
 
      bool normalMap;       /**< if uses a normal map */
      GLuint normalTexId;   /**< GL id of normal map texture */
};

#define STATE_NONE        -1 /**< No animation */
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

      enum
      {
         TYPE_MD5,
         TYPE_CAL3D
      };

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
      aniModel(int modelType);
      /*! Destructor */
      virtual ~aniModel();

      /*****************************************************************
       * Virtuals
       *****************************************************************/
      /*! Render the skeleton (usually, for debug reasons) */
      virtual void renderSkeleton()=0;

      /*! Load the model to be this character's body and animations.
       * \param strFilename -> string with the file name.
       * \return true if success on load. */
      virtual bool load(const std::string& strFilename)=0;

      /*! Call Action Animation (just a cycle to blend)
       * \param aniId -> animation ID */
      virtual void callActionAnimation(int aniId, 
            bool mergeWithPrevious=true)=0;
      /*! Set the current animation to "animationId" */
      virtual void setAnimation(int animationId)=0;
      /*! Calculate the model bounding box for current animation frame */
      virtual void calculateCrudeBoundingBox()=0;
      /*! Update the vertex info */
      virtual void updateKeyVertex(vertexInfo& v,
         float angleY, float pX, float pY, float pZ)=0;
      /*! Get mesh material */
      virtual aniModelMaterial* getMeshMaterial(int meshId)=0;
      /*****************************************************************
       * /Virtuals
       *****************************************************************/


      /*! Update Model to pos time 
       * \param pos -> time of the animation
       * \param isVisible -> result of the view frustum culling for
       *                     the last bounding box calculation. 
       *                     If isn't visible, only update the frame,
       *                     but not calculate the mesh position,
       *                     to avoid spending cpu/gpu time if things
       *                     that at end will not be visible. */
      void update(float pos, float angleY, float pX, float pY, float pZ,
         bool isVisible);

      /*! Set the animation state of the model.
       * \param state -> state ID to be defined. */
      void setState(int state);

      /*! Gets the animation state of the model.
       * \return state ID of the current animation */
      int getState();

      /*! Get the animodel type */
      int getType(){return(type);};

      /*! Load the model to video card memory
       * \param useTexture -> true to use the model texture, false otherwise
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

      /*! Render all normals (usualy, for debug reasons) */
      void renderNormals();

      /*! Render all tangents (usually, for debug) */
      void renderTangents();

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
            GLfloat pX, GLfloat pY, GLfloat pZ, boundingBox& colBox);

      /*! Verify, with all Meshes of the model, if the model colides
       *  with the ray defined as parameter. 
       *  \param pX -> model X position
       *  \param pY -> model Y position
       *  \param pZ -> model Z position
       *  \param colRay -> colider ray
       *  \return -> true if one or more meshes colides, false otherwise */
      bool depthCollision(GLfloat angleX, GLfloat angleY, GLfloat angleZ, 
            GLfloat pX, GLfloat pY, GLfloat pZ, ray& colRay);

      /*! Define all key vertices (left and right hand, for example) */
      void defineKeyVertex();

      /*! Get the first vertex id influenced by a bone
       * \param boneId -> id of the bone
       * \param inf -> will receive the vertex indexes
       * \return true if found */
      bool getInfluencedVertex(int boneId, vertexInfo& inf);

      /*! Get the model fileName */
      std::string getFileName(){return(fileName);};

      vertexInfo leftHand;           /**< Base vertex at left hand */
      vertexInfo rightHand;          /**< Base vextex at right hand */
      vertexInfo head;               /**< Base vertex at head */

   protected:
      std::string fileName;          /**< current model fileName */

      boundingBox crudeBox;  /**< Crude bounding box */
      float dY;              /**< Delta to Y position to y1 be 0 */
      float renderScale;     /**< Render scale factor */
      bool loadedTexture;    /**< True if loaded texture */

      shaders dntShaders;    /**< Shaders on DNT */
      extensions ext;        /**< OpenGL extensions */

      /*! Load the a texture to the model.
       * \param strFilename -> \c string with the texture file name.
       * \return the \c GLuint with the GL texture ID. */
      GLuint loadTexture(const std::string& strFilename);

      /*! Render from the graphic memory at default position */
      void renderFromGraphicMemory();

      /*! Calculate the model deltaY */
      void calculateDeltaY();

      /*****************************************************************
       * Virtuals
       *****************************************************************/

      /*! Update the model animation after delta */
      virtual void update(float delta, bool isVisible)=0;

      /*! Get the Id of a bone
       * \param bName name of the bone to get its it
       * \return bone's ID or -1 if not found */
      virtual int getBoneId(std::string bName)=0;

      /*! Get the current animated vertices
       * \param meshId -> current Id of the mesh
       * \param count -> on return will have the total vertices
       * \return pointer to current vertices of the mesh */
      virtual vector3f_t* getMeshVertices(int meshId, int& count)=0;

      /*! Get the current animated normals
       * \param meshId -> current Id of the mesh
       * \param count -> on return will have the total normals
       * \return pointer to current normals of the mesh */
      virtual vector3f_t* getMeshNormals(int meshId, int& count)=0;

      /*! Get the current animated texture coordinates
       * \param meshId -> current Id of the mesh
       * \param count -> on return will have the total texture coordinates
       * \return pointer to current texture coordinates of the mesh */
      virtual vector2f_t* getMeshTexCoords(int meshId, int& count)=0;

      /*! Get the current animated vertex tangents
       * \param meshId -> current Id of the mesh
       * \param count -> on return will have the total normals
       * \return pointer to current vertices tangents of the mesh */
      virtual vector3f_t* getMeshTangents(int meshId, int& count)=0;

      /*! Get the current animated faces (vertices index)
       * \param meshId -> current Id of the mesh
       * \param count -> on return will have the total faces
       * \return pointer to current faces of the mesh */
      virtual vector3i_t* getMeshFaces(int meshId, int& count)=0;

      /*! Get the number of meshes */
      virtual int getTotalMeshes()=0;

   private:
      int type;          /**< model type */
      int curState;      /**< current animation state */
      int faceCount;     /**< number of faces to render */
      vector3i_t* faces;       /**< current faces to render */
      GLint tangentAttrib;  /**< the tangent attrib */
 
};

#endif

