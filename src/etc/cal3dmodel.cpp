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

#include "cal3dmodel.h"
#include "defparser.h"
#include "../engine/util.h"
#include "dirs.h"

#include <string>
#include <sstream>

using namespace std;

/**********************************************************************
 *                            Constructor                             *
 **********************************************************************/
cal3DModel::cal3DModel(): aniModel(aniModel::TYPE_CAL3D)
{
   int i;

   dY = 0.0f;

   m_calCoreModel = new CalCoreModel("");
   m_calModel = NULL;
   m_state = STATE_IDLE;
   m_motionBlend[0] = 1.0f;
   m_motionBlend[1] = 1.0f;
   m_motionBlend[2] = 1.0f;
   m_animationCount = 0;
   m_meshCount = 0;
   m_lodLevel = 1.0f;
   fileName = "";
   for(i=0;i<ANIMODEL_MAX_ANIMATIONS;i++)
   {
      m_animationId[i] = -1;
   }

   totalVertex = 0;
   totalFaces = 0;

   meshVertices = NULL;
   meshNormals = NULL;
   meshTextureCoordinates = NULL;
   meshFaces = NULL;
}

/**********************************************************************
 *                             Destructor                             *
 **********************************************************************/
cal3DModel::~cal3DModel()
{
   /* Bye texture */
   if( (m_calModel) && (loadedTexture) )
   {
      glDeleteTextures(1, &meshMaterial.textureId);
   }

   /* Bye */
   if(m_calCoreModel)
   {
      delete m_calCoreModel;
   }
   if(m_calModel)
   {
      delete m_calModel;
   }
   if(meshVertices)
   {
      delete[] meshVertices;
   }
   if(meshNormals)
   {
      delete []meshNormals;
   }
   if(meshTextureCoordinates)
   {
      delete[] meshTextureCoordinates;
   }
   if(meshFaces)
   {
      delete[] meshFaces;
   }
}

/*********************************************************************
 *                              loadModel                            *
 *********************************************************************/
bool cal3DModel::load(const string& strFilename)
{
   dirs dir;

   bool definedDeltaY = false;

   /* initialize the data path */
   string strPath = m_path;

   /* initialize the animation count */
   int animationCount;
   animationCount = 0;

   /* Get the definitions */
   defParser parser;
   if(!parser.load(strFilename))
   {
      return(false);
   }

   fileName = strFilename;

   string strKey = "", strData = "";

   /* Interpret each one */
   while(parser.getNextTuple(strKey, strData))
   {
      /*  handle the model creation */
      if(strKey == "scale")
      {
         /* set rendering scale factor */
         std::stringstream sconv;
         sconv << strData;
         sconv >> renderScale;
      }
      else if(strKey == "path")
      {
         /* set the new path for the data files 
          * if one hasn't been set already */
         if(m_path == "")
         {
            strPath = dir.getRealFile(strData);
            m_path = strData;
         }
      }
      else if(strKey == "skeleton")
      {
         /* load core skeleton */
         if(!m_calCoreModel->loadCoreSkeleton(strPath + strData))
         {
            CalError::printLastError();
            return false;
         }
      }
      else if(strKey == "animation")
      {
         /* load core animation */
         m_animationId[animationCount] = 
            m_calCoreModel->loadCoreAnimation(strPath + strData);
         if(m_animationId[animationCount] == -1)
         {
            CalError::printLastError();
            return false;
         }

         animationCount++;
      }
      else if(strKey == "mesh")
      {
         /* load core mesh */
         int meshID = m_calCoreModel->loadCoreMesh(strPath + strData);
         if(meshID == -1)
         {
            CalError::printLastError();
            return false;
         }
      }
      else if(strKey == "material")
      {
         /* load core material */
         if(m_calCoreModel->loadCoreMaterial(strPath + strData) == -1)
         {
            CalError::printLastError();
            return false;
         }
      }
      else if(strKey == "deltaY")
      {
         sscanf(strData.c_str(), "%f", &dY);
         definedDeltaY = true;
      }
      else
      {
         cerr << strFilename << ": Unknow key '" << strKey
            << "'" << endl;
         return false;
      }
   }

   /* load all textures and store the opengl texture id in the 
    * corresponding map in the material */
   int materialId;
   for( materialId = 0; 
         materialId < m_calCoreModel->getCoreMaterialCount(); 
         materialId++)
   {
      /* get the core material */
      CalCoreMaterial *pCoreMaterial;
      pCoreMaterial = m_calCoreModel->getCoreMaterial(materialId);

      /* loop through all maps of the core material */
      int mapId;
      for(mapId = 0; mapId < pCoreMaterial->getMapCount(); mapId++)
      {
         /* get the filename of the texture */
         std::string strFilename;
         strFilename = pCoreMaterial->getMapFilename(mapId);

         /* load the texture from the file */
         GLuint textureId;
         textureId = loadTexture(m_path + strFilename);

         /* store the opengl texture id in the user data of the map */
         pCoreMaterial->setMapUserData(mapId, 
               (Cal::UserData*)(long)textureId);
      }
   }

   /* make one material thread for each material
    * mapping without further information on the model etc. */
   for(materialId = 0; 
         materialId < m_calCoreModel->getCoreMaterialCount(); materialId++)
   {
      /* create the a material thread */
      m_calCoreModel->createCoreMaterialThread(materialId);

      /* initialize the material thread */
      m_calCoreModel->setCoreMaterialId(materialId, 0, materialId);
   }

   /* Calculate Bounding Boxes */
   m_calCoreModel->getCoreSkeleton()->calculateBoundingBoxes(m_calCoreModel);

   m_calModel = new CalModel(m_calCoreModel);

   /* attach all meshes to the model */
   m_meshCount = m_calCoreModel->getCoreMeshCount();
   int meshId;
   for(meshId = 0; meshId < m_calCoreModel->getCoreMeshCount(); meshId++)
   {
      m_calModel->attachMesh(meshId);
   }

   /* set the material set of the whole model */
   m_calModel->setMaterialSet(0);

   /* Get the delta Y, if it'isnt already set */
   if(!definedDeltaY)
   {
      calculateDeltaY();
   }
  
   /* Alloc all render arrays
    * FIXME: assuming only 1 mesh with 1 submesh */
   CalCoreSubmesh* smesh = m_calCoreModel->getCoreMesh(0)->getCoreSubmesh(0);
   totalVertex = smesh->getVertexCount();
   totalFaces = smesh->getFaceCount();
   meshVertices = new vector3f_t[totalVertex];
   meshNormals = new vector3f_t[totalVertex];
   meshTextureCoordinates = new vector2f_t[totalVertex];
   meshFaces = new vector3i_t[totalFaces];

   /* set initial animation state */
   curPos =  11 + (int)(30 * (rand() / (RAND_MAX + 1.0))); 
   m_state = STATE_NONE;
   setAnimation(STATE_IDLE);
   update(curPos, true);

   /* Define the material */
   defineMaterial();

   /* Define all key vertices */
   defineKeyVertex();

   /* End of CAL3D LOAD */
   return(true);
}

/*********************************************************************
 *                        callActionAnimation                        *
 *********************************************************************/
void cal3DModel::callActionAnimation(int animationId, bool mergeWithPrevious)
{
   /* Verify if animation is defined */
   if( (animationId > ANIMODEL_MAX_ANIMATIONS) || (animationId < 0) ||
       (m_animationId[animationId] == -1) )
   {
      return;
   }

   m_calModel->getMixer()->executeAction(m_animationId[animationId],
         0.3f,0.3f);
   m_state = animationId;
}

/*********************************************************************
 *                            setAnimation                           *
 *********************************************************************/
void cal3DModel::setAnimation(int animationId)
{
   /* Verify if animation is defined */
   if( (animationId > ANIMODEL_MAX_ANIMATIONS) || (animationId < 0) ||
       (m_animationId[animationId] == -1) )
   {
      return;
   }

   /* Only change animation if not actually on it */
   if(animationId != m_state)
   {
       if(animationId == STATE_DIE)
       {
          m_calModel->getMixer()->clearCycle(m_animationId[m_state],0.1f);
          
          if(m_state == STATE_ATTACK_MEELE)
          {
             /* The idle is merged with the attack, so clear it too */
             m_calModel->getMixer()->clearCycle(m_animationId[STATE_IDLE],0.1f);
          }

          m_calModel->getMixer()->executeAction(m_animationId[animationId],
                                                0.3f,0.3f);
          m_calModel->getMixer()->blendCycle(m_animationId[STATE_DEAD],
                                            0.1f,0.1f);
       }
       else if(animationId == STATE_ATTACK_MEELE)
       {         
          m_calModel->getMixer()->executeAction(m_animationId[animationId],
                                                0.3f,0.3f);
       }
       else
       {
          m_calModel->getMixer()->clearCycle(m_animationId[m_state],0.1f);
          m_calModel->getMixer()->blendCycle(m_animationId[animationId],
                                             1.0f,0.1f);
       }
       m_state = animationId;
   }
}

/*********************************************************************
 *                                update                             *
 *********************************************************************/
void cal3DModel::update(float delta, bool isVisible)
{
   curPos = delta;

   if(isVisible)
   {
      m_calModel->update(delta);
      updateArrays();
   }
}

/*********************************************************************
 *                          getMeshMaterial                          *
 *********************************************************************/
aniModelMaterial* cal3DModel::getMeshMaterial(int meshId)
{
   return(&meshMaterial);
}

/*********************************************************************
 *                           defineMaterial                          *
 *********************************************************************/
void cal3DModel::defineMaterial()
{
   unsigned char meshColor[4];
   
   /* get the renderer of the model */
   pCalRenderer = m_calModel->getRenderer();

   if(!pCalRenderer->beginRendering()) return;
   if(pCalRenderer->selectMeshSubmesh(0, 0))
   {
      /* set the material ambient color */
      pCalRenderer->getAmbientColor(&meshColor[0]);
      meshMaterial.ambient[0] = meshColor[0] / 255.0f;  
      meshMaterial.ambient[1] = meshColor[1] / 255.0f; 
      meshMaterial.ambient[2] = meshColor[2] / 255.0f; 
      meshMaterial.ambient[3] = meshColor[3] / 255.0f;

      /* set the material diffuse color */
      pCalRenderer->getDiffuseColor(&meshColor[0]);
      meshMaterial.diffuse[0] = meshColor[0] / 255.0f;  
      meshMaterial.diffuse[1] = meshColor[1] / 255.0f; 
      meshMaterial.diffuse[2] = meshColor[2] / 255.0f; 
      meshMaterial.diffuse[3] = meshColor[3] / 255.0f;

      /* set the material specular color */
      pCalRenderer->getSpecularColor(&meshColor[0]);
      meshMaterial.specular[0] = meshColor[0] / 255.0f;  
      meshMaterial.specular[1] = meshColor[1] / 255.0f; 
      meshMaterial.specular[2] = meshColor[2] / 255.0f; 
      meshMaterial.specular[3] = meshColor[3] / 255.0f;

      /* set the material shininess factor */
      meshMaterial.shininess = pCalRenderer->getShininess();

      /* Set textureID */
      meshMaterial.textureId = (unsigned long)pCalRenderer->getMapUserData(0);
   }

   pCalRenderer->endRendering();
}

/*********************************************************************
 *                            updateArrays                           *
 *********************************************************************/
void cal3DModel::updateArrays()
{
  m_calModel->getSkeleton()->calculateBoundingBoxes();
  
  /* get the renderer of the model */
  pCalRenderer = m_calModel->getRenderer();

  if(!pCalRenderer->beginRendering()) return;

  /* get the number of meshes */
  int meshCount;
  meshCount = pCalRenderer->getMeshCount();

  /* Load the ONLY ONE mesh of the model */
  int meshId = 0;
  if(meshId < meshCount)
  {
    /* get the number of submeshes */
    int submeshCount;
    submeshCount = pCalRenderer->getSubmeshCount(meshId);

    // Load the ONLY ONE submesh of the mesh
    int submeshId = 0;
    if(submeshId < submeshCount)
    {
      // select mesh and submesh for further data access
      if(pCalRenderer->selectMeshSubmesh(meshId, submeshId))
      {       
        /* get the transformed vertices of the submesh */
        pCalRenderer->getVertices(&meshVertices[0][0]);

        /* get the transformed normals of the submesh */
        pCalRenderer->getNormals(&meshNormals[0][0]);

        /* get the texture coordinates of the submesh */
        pCalRenderer->getTextureCoordinates(0, &meshTextureCoordinates[0][0]);

        /* get the faces of the submesh */
        pCalRenderer->getFaces(&meshFaces[0][0]);
      }
    }
  }

  pCalRenderer->endRendering();
}



/*********************************************************************
 *                           getMeshVertices                         *
 *********************************************************************/
vector3f_t* cal3DModel::getMeshVertices(int meshId, int& count)
{
   count = totalVertex; 
   return(&meshVertices[0]);
}

/*********************************************************************
 *                           getMeshNormals                          *
 *********************************************************************/
vector3f_t* cal3DModel::getMeshNormals(int meshId, int& count)
{
   count = totalVertex;
   return(&meshNormals[0]);
}

/*********************************************************************
 *                          getMeshTexCoords                         *
 *********************************************************************/
vector2f_t* cal3DModel::getMeshTexCoords(int meshId, int& count)
{
   count = totalVertex;
   return(&meshTextureCoordinates[0]);
}

/*********************************************************************
 *                           getMeshFaces                            *
 *********************************************************************/
vector3i_t* cal3DModel::getMeshFaces(int meshId, int& count)
{
   count = totalFaces;

   return(&meshFaces[0]);
}


/*********************************************************************
 *                          renderSkeleton                           *
 *********************************************************************/
void cal3DModel::renderSkeleton()
{
   /* TODO! */
}

/*********************************************************************
 *                        calculateBoundingBox                       *
 *********************************************************************/
void cal3DModel::calculateCrudeBoundingBox()
{
   m_calModel->getSkeleton()->calculateBoundingBoxes();

   GLuint aux, aux2;
   int computed = 0;
   CalVector p[8];
   float min[3], max[3];

   CalSkeleton *pCalSkeleton = m_calModel->getSkeleton();

   std::vector<CalBone*> &vectorCoreBone = pCalSkeleton->getVectorBone();
   for(aux = 0; aux < vectorCoreBone.size(); aux++)
   {
      CalBoundingBox &calBoundingBox=vectorCoreBone[aux]->getBoundingBox();
      calBoundingBox.computePoints(p);
      for(aux2 = 0;aux2 < 8; aux2++)
      {
         /* NOTE: Do not forget that if the blender coordinate system is
          * (x,y,z), the DNT system is (-x,z,y) */
         if(!computed)
         {
            min[0] = -p[aux2].x; max[0] = -p[aux2].x; 
            min[1] = p[aux2].z; max[1] = p[aux2].z;
            min[2] = p[aux2].y; max[2] = p[aux2].y;

            computed = 1;
         }
         else
         {
            if(-p[aux2].x > max[0])
               max[0] = -p[aux2].x;
            if(-p[aux2].x < min[0])
               min[0] = -p[aux2].x;
            if(p[aux2].z > max[1])
               max[1] = p[aux2].z;
            if(p[aux2].z < min[1])
               min[1] = p[aux2].z;
            if(p[aux2].y > max[2])
               max[2] = p[aux2].y;
            if(p[aux2].y < min[2])
               min[2] = p[aux2].y;
         }
      }
   }

   /* Do the Scale to the bounding box */
   for(aux = 0; aux < 3; aux++)
   {
      min[aux] *= renderScale;
      max[aux] *= renderScale;
   }

   /* Apply the delta */
   min[1] += dY;
   max[1] += dY;

   crudeBox.setMin(min);
   crudeBox.setMax(max);
}

/*********************************************************************
 *                             getBoneId                             *
 *********************************************************************/
int cal3DModel::getBoneId(string bName)
{
   Uint16 i;
   CalSkeleton *pCalSkeleton = m_calModel->getSkeleton();
   std::vector<CalBone *>& vectorBone = pCalSkeleton->getVectorBone();
   CalCoreBone* coreBone;

   for(i=0; i < vectorBone.size(); i++)
   {
      coreBone = vectorBone[i]->getCoreBone();
      if( (coreBone) && (coreBone->getName() == bName))
      {
         return(i);
      }
   }
   return(-1);
}

/*********************************************************************
 *                          updateKeyVertex                          *
 *********************************************************************/
void cal3DModel::updateKeyVertex(vertexInfo& v, 
      float angleY, float pX, float pY, float pZ)
{
   /* Calculate the sin and cos of the angle */
   float angleSin = sinf(deg2Rad(angleY));
   float angleCos = cosf(deg2Rad(angleY));
   
   /* Get the model renderer */ 
   pCalRenderer = m_calModel->getRenderer();

   if(v.vertexId != -1)
   {
      /* Define the mesh/submesh and get its vertices */
      pCalRenderer->selectMeshSubmesh(v.meshId, v.subMeshId);
      pCalRenderer->getVertices(&meshVertices[0][0]);

      /* Calculate angles TODO */
      v.angleXY = (atanf( (meshVertices[v.vertexId][2] - v.iY) /
                                 (-meshVertices[v.vertexId][0] - v.iX) ));
      v.angleYZ = (atanf( (meshVertices[v.vertexId][2] - v.iY) /
                                 (meshVertices[v.vertexId][1] - v.iZ) ));

      v.angleXY = rad2Deg(v.angleXY);
      v.angleYZ = rad2Deg(v.angleYZ);

      /* Translate and rotate the coordinates.
       * NOTE: Do not forget that if the blender coordinate system is
       * (x,y,z), the DNT system is (-x,z,y) */
      v.x = pX + 
            ((-meshVertices[v.vertexId][0])*angleCos*renderScale) +
            ((meshVertices[v.vertexId][1])*angleSin*renderScale);
      v.y = pY + meshVertices[v.vertexId][2]*renderScale + dY;
      v.z = pZ + 
            ((meshVertices[v.vertexId][0]*angleSin*renderScale)) +
            ((meshVertices[v.vertexId][1]*angleCos*renderScale));
   }
   else
   {
      /* Key vertex not defined */
      v.x = 0;
      v.y = 0;
      v.z = 0;
   }
}


/*********************************************************************
 *                         getInfluencedVertex                       *
 *********************************************************************/
bool cal3DModel::getInfluencedVertex(int boneId, vertexInfo& inf)
{
   Uint16 i, j, v, c;
   CalCoreMesh* mesh;
   CalCoreSubmesh* subMesh;
   std::vector<CalCoreSubmesh::Vertex> vert;

   for(i=0; i < m_calCoreModel->getCoreMeshCount(); i++)
   {
      mesh = m_calCoreModel->getCoreMesh(i);
      for(j=0; j < mesh->getCoreSubmeshCount(); j++)
      {
         subMesh = mesh->getCoreSubmesh(j);
         vert = subMesh->getVectorVertex();
         for(v=0; v < subMesh->getVertexCount(); v++)
         {
            for(c=0; c < vert[v].vectorInfluence.size(); c++)
            {
               if(vert[v].vectorInfluence[c].boneId == boneId)
               { 
                  inf.meshId = i;
                  inf.subMeshId = j;
                  inf.vertexId = v;
                  /* DNT coordinates: (-x, z, y) */
                  inf.iX = -vert[v].position.x;
                  inf.iY = vert[v].position.z+dY;
                  inf.iZ = vert[v].position.y;
                  return(true);
               }
            }
         }
      }
   }
   return(false);
}

/*********************************************************************
 *                          defineKeyVertex                          *
 *********************************************************************/
void cal3DModel::defineKeyVertex()
{
   int boneId = -1;

   /* Get left hand */
   boneId = getBoneId("hand_left");
   if((boneId == -1) || (!getInfluencedVertex(boneId, leftHand)))
   {
      leftHand.vertexId = -1;
   }

   /* Get right hand */
   boneId = getBoneId("hand_right");
   if((boneId == -1) || (!getInfluencedVertex(boneId, rightHand)))
   {
      rightHand.vertexId = -1;
   }

   /* Get head */
   boneId = getBoneId("head");
   if((boneId == -1) || (!getInfluencedVertex(boneId, head)))
   {
      head.vertexId = -1;
   }
}

