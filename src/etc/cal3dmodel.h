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

#ifndef _dnt_cal3d_model_h
#define _dnt_cal3d_model_h

#include <cal3d/cal3d.h>
#include <SDL/SDL_opengl.h>
#include <string>

#include "animodel.h"

/*! The Cal3D model abstraction. */
class cal3DModel: public aniModel
{
   public:
      /*! Constructor */
      cal3DModel();

      /*! Destructor */
      ~cal3DModel();

      /*! Call Action Animation (just a cycle to blend)
       * \param aniId -> animation ID */
      void callActionAnimation(int aniId, bool mergeWithPrevious);

      /*! Load .cfg file and all needed meshes/animations, etc. */
      bool load(const std::string& strFileName);

      /*! Render the skeleton (usually, for debug reasons) */
      void renderSkeleton();

      /*! Calculate the model bounding box for current animation frame */
      void calculateCrudeBoundingBox();

      /*! Update the vertex info */
      void updateKeyVertex(vertexInfo& v,
            float angleY, float pX, float pY, float pZ);

      /*! Get mesh material */
      aniModelMaterial* getMeshMaterial(int meshId);
   
   protected:

      /*! Update the animation, delta seconds after previous update */
      void update(float delta, bool isVisible);

      /*! Set the current animation to "animationId" */
      void setAnimation(int animationId);

      /*! Get the current animated vertices
       * \param meshId -> current Id of the mesh
       * \param count -> on return will have the total vertices
       * \return pointer to current vertices of the mesh */
      vector3f_t* getMeshVertices(int meshId, int& count);

      /*! Get the current animated normals
       * \param meshId -> current Id of the mesh
       * \param count -> on return will have the total normals
       * \return pointer to current vertices of the mesh */
      vector3f_t* getMeshNormals(int meshId, int& count);

      /*! Get the current animated texture coordinates
       * \param meshId -> current Id of the mesh
       * \param count -> on return will have the total texture coordinates
       * \return pointer to current vertices of the mesh */
      vector2f_t* getMeshTexCoords(int meshId, int& count);

      /*! Get the current animated faces (vertices index)
       * \param meshId -> current Id of the mesh
       * \param count -> on return will have the total faces
       * \return pointer to current faces of the mesh */
      vector3i_t* getMeshFaces(int meshId, int& count);

      /*! Get the number of meshes on model */
      int getTotalMeshes(){return(m_meshCount);};

      /*! Update all render arrays to current animation state */
      void updateArrays();

      /*! Define the mesh material */
      void defineMaterial();

      /*! Get the Id of a bone
       * \param bName name of the bone to get its it
       * \return bone's ID or -1 if not found */
      int getBoneId(std::string bName);

      /*! Define all key vertices */
      void defineKeyVertex();

      /*! Get influenced vertex for boneId.
       * \return true if found */
      bool getInfluencedVertex(int boneId, vertexInfo& inf);
  

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
      float m_lodLevel;              /**< Cal3D Level of Detail to render */
      float curPos;                  /**< current time pos */
      std::string m_path;            /**< Path to cal3D model */

      CalRenderer *pCalRenderer;     /**< Pointer to themodel renderer */
      
      /* Auxiliar renderers */
      vector3f_t* meshVertices;
      vector3f_t* meshNormals;
      vector2f_t* meshTextureCoordinates;
      vector3i_t* meshFaces;
      aniModelMaterial meshMaterial;

      int totalVertex;
      int totalFaces;

};

#endif


