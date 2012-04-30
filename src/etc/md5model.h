/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
  
  Originally based on the work of David Henry <tfc_duke@club-internet.fr>
  Copyright (c) 2005-2007 David HENRY
 
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

#ifndef _dnt_md5_model_h
#define _dnt_md5_model_h

#include <SDL/SDL_opengl.h>

#include <string>
#include "quaternion.h"
#include "md5anim.h"
#include "list.h"

#include "animodel.h"

/*! Vertex */
class md5_vertex_t
{
   public:
      int start;   /**< start weight */
      int count;   /**< weight count */

      vec3_t normal;  /**< The precalculated vertex normal "weights" */
      vec3_t tangent; /**< The precalculated vertex tangent "weights" */
};

/*! Weight */
class md5_weight_t
{
   public:
      int joint;
      float bias;

      vec3_t pos;
};

/*! MD5 mesh */
struct md5_mesh_t
{
   public:
      md5_vertex_t *vertices;
      
      md5_weight_t *weights;

      vector3f_t* vertexArray;   /**< Array of vertices to render */
      vector3f_t* normalArray;   /**< Array of normals */
      vector3f_t* tangentArray;  /**< Array of tangents */
      vector2f_t* uvArray;       /**< Array of vertices texture uv map */
      vector3i_t* triangles;     /**< Array of faces (triangles) */

      int num_verts;
      int num_tris;
      int num_weights;

      std::string shader;
};

/*! A single model animation */
class md5ModelAnimation: public dntListElement
{
   public:
      /*! Constructor */
      md5ModelAnimation(int id)
      {
         animation = new md5Anim(id);
      }
      /*! Destructor */
      ~md5ModelAnimation()
      {
         delete(animation);
      }


      md5Anim* animation;
};

/*! The MD5 model abstraction. It's composed by a .md5mesh file and
 * some md5anim files. Currently, there's no mixer for animations (as there
 * is in Doom3, for example), so only one active animation per time. */
class md5Model: public aniModel, dntList
{
   public:
      /*! Constructor */
      md5Model();

      /*! Destructor */
      ~md5Model();

      /*! Render the skeleton (usually, for debug reasons) */
      void renderSkeleton();

      /*! Load .cfg file and all needed meshes/animations, etc. */
      bool load(const std::string& strFileName);

      /*! Calculate the model bounding box for current animation frame */
      void calculateCrudeBoundingBox();

      /*! Update the vertex info */
      void updateKeyVertex(vertexInfo& v,
            float angleY, float pX, float pY, float pZ);

      /*! Get mesh material */
      aniModelMaterial* getMeshMaterial(int meshId);

      /*! Call Action Animation (just a cycle to blend)
       * \param aniId -> animation ID 
       * \param mergeWithPrevious -> if will merge with previous animation */ 
      void callActionAnimation(int aniId, bool mergeWithPrevious=true);

      /*! Set the current animation to "animationId" */
      void setAnimation(int animationId);

      /*! Load mesh model from md5 file */
      bool loadMeshFile(const std::string strFileName);

      /*! Load anim file to the md5 model */
      bool loadAnimFile(const std::string strFileName);

      /*! Get the current animated vertices
       * \param meshId -> current Id of the mesh
       * \param count -> on return will have the total vertices
       * \return pointer to current vertices of the mesh */
      vector3f_t* getMeshVertices(int meshId, int& count);

      /*! Get the current animated faces (vertices index)
       * \param meshId -> current Id of the mesh
       * \param count -> on return will have the total faces
       * \return pointer to current faces of the mesh */
      vector3i_t* getMeshFaces(int meshId, int& count);

      /*! Get the current animated texture coordinates
       * \param meshId -> current Id of the mesh
       * \param count -> on return will have the total texture coordinates
       * \return pointer to current vertices of the mesh */
      vector2f_t* getMeshTexCoords(int meshId, int& count);

      /*! Get the current animated normals
       * \param meshId -> current Id of the mesh
       * \param count -> on return will have the total normals
       * \return pointer to current vertices of the mesh */
      vector3f_t* getMeshNormals(int meshId, int& count);

      /*! Get the current animated vertex tangents
       * \param meshId -> current Id of the mesh
       * \param count -> on return will have the total normals
       * \return pointer to current vertices tangents of the mesh */
      vector3f_t* getMeshTangents(int meshId, int& count);
   
   protected:

      /*! precalculate normals and tangents for latter per-frame use */
      void preCalculateNormalsAndTangents();

      /*! Update the animation, delta seconds after previous update */
      void update(float delta, bool isVisible);

      /*! Delete and clean all created structures */
      void clean();

      /*! Delete the animation from list */
      void freeElement(dntListElement* obj);

      /*! Calculate mesh vertex and normal positions,
       * based on current skeleton. */
      void calculateMeshes();

      /*! Get the Id of a bone
       * \param bName name of the bone to get its it
       * \return bone's ID or -1 if not found */
      int getBoneId(std::string bName);

      /*! Get the number of meshes on model */
      int getTotalMeshes(){return(totalMeshes);};

      
      /* The model info */
      md5_joint_t* baseSkel;       /**< Base skeleton */
      md5_mesh_t* meshes;          /**< Model meshes */

      aniModelMaterial meshMaterial; /**< the model material */

      md5_joint_t* curSkel;        /**< Current skeleton */

      int totalJoints;             /**< Total joint in model */
      int totalMeshes;             /**< Total model meshes */

      int blendFrames;             /**< Total frames when blending */

      md5ModelAnimation* curAnimation; /**< Current animation */
      md5ModelAnimation* backAnimation; /**< Animation to return after a
                                          "callAction" (one cycle) animation */
      bool actionAnimation;            /**< if curAnimation is an action-one */

      std::string path;    /**< Model's path */

      bool needToCalculate;  /**< if need to calculate mesh positions when
                                  no active animations */
      bool needToCalculateBoundingBox;

};

#endif


