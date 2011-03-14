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

#include "animodel.h"
#include "defparser.h"
#include "../engine/util.h"
#include "dirs.h"
#include "overlaps.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>

/**********************************************************************
 *                            Constructor                             *
 **********************************************************************/
aniModel::aniModel()
{
   int i;

   m_calCoreModel = new CalCoreModel("");
   m_calModel = NULL;
   m_state = STATE_IDLE;
   m_motionBlend[0] = 1.0f;
   m_motionBlend[1] = 1.0f;
   m_motionBlend[2] = 1.0f;
   m_animationCount = 0;
   m_meshCount = 0;
   m_renderScale = 1.0f;
   m_lodLevel = 1.0f;
   modelFileName = "";
   for(i=0;i<ANIMODEL_MAX_ANIMATIONS;i++)
   {
      m_animationId[i] = -1;
   }
}

/**********************************************************************
 *                             Destructor                             *
 **********************************************************************/
aniModel::~aniModel()
{
   if(m_calCoreModel)
   {
      delete(m_calCoreModel);
   }
   if(m_calModel)
   {
      delete(m_calModel);
   }
}

/*********************************************************************
 *                            loadTexture                            *
 *********************************************************************/
GLuint aniModel::loadTexture(const string& strFilename)
{
   GLuint pId = 0;
 
   /* Load texture image from file */
   SDL_Surface* img = IMG_Load(strFilename.c_str());
   if(!img)
   {
      cout << "Can't Open Texture" << strFilename << endl; 
      return(0);
   }

   GLint format = GL_RGB;

   if(img->format->BytesPerPixel == 4)
   {
      format = GL_RGBA;
   }

   /* Show Warning if not power of 2 */
   if( (img->h != smallestPowerOfTwo(img->h)) ||
       (img->w != smallestPowerOfTwo(img->w)) )
   {
      cout << "Warning: image '" << strFilename 
           << "' is of non-power of two dimension '" 
           << img->w << "x" << img->h << "'" << endl;
   }

   
   glGenTextures(1, &pId);
   glBindTexture(GL_TEXTURE_2D, pId);
   glTexImage2D(GL_TEXTURE_2D,0,format,img->w,img->h, 
                0, format, GL_UNSIGNED_BYTE, img->pixels);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                   GL_LINEAR_MIPMAP_LINEAR );
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   gluBuild2DMipmaps(GL_TEXTURE_2D, format, img->w,
                     img->h, format, GL_UNSIGNED_BYTE, 
                     img->pixels );

   /* Free memory */
   SDL_FreeSurface(img);

   return(pId);
}

/*********************************************************************
 *                        calculateBoundingBox                       *
 *********************************************************************/
void aniModel::calculateCrudeBoundingBox()
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
     min[aux] *= m_renderScale;
     max[aux] *= m_renderScale;
  }

   crudeBox.setMin(min);
   crudeBox.setMax(max);
}

/*********************************************************************
 *                       getCrudeBoundingBox                         *
 *********************************************************************/
boundingBox aniModel::getCrudeBoundingBox()
{
   return(crudeBox);
}

/*********************************************************************
 *                              loadModel                            *
 *********************************************************************/
bool aniModel::loadModel(const string& strFilename)
{
   dirs dir;

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

   modelFileName = strFilename;

   string strKey = "", strData = "";

   /* Interpret each one */
   while(parser.getNextTuple(strKey, strData))
   {
      /*  handle the model creation */
      if(strKey == "scale")
      {
         /* set rendering scale factor */
         m_renderScale = atof(strData.c_str());
      }
      else if(strKey == "path")
      {
         /* set the new path for the data files 
          * if one hasn't been set already */
         if (m_path == "") strPath = dir.getRealFile(strData);
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
         textureId = loadTexture(strPath + strFilename);

         /* store the opengl texture id in the user data of the map */
         pCoreMaterial->setMapUserData(mapId, (Cal::UserData*)textureId);
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
   int meshId;
   for(meshId = 0; meshId < m_calCoreModel->getCoreMeshCount(); meshId++)
   {
      m_calModel->attachMesh(meshId);
   }

   /* set the material set of the whole model */
   m_calModel->setMaterialSet(0);

   /* set initial animation state */
   curPos =  11 + (int)(30 * (rand() / (RAND_MAX + 1.0))); 
   m_state = -1;
   setState(STATE_IDLE);

   m_calModel->update(curPos);

   /* Define all key vertices */
   defineKeyVertex();

   /* End of CAL3D LOAD */
   return(true);
}

/*********************************************************************
 *                        loadToGraphicMemory                        *
 *********************************************************************/
void aniModel::loadToGraphicMemory(bool useTexture)
{
  m_calModel->getSkeleton()->calculateBoundingBoxes();
  // get the renderer of the model
  pCalRenderer = m_calModel->getRenderer();

  if(!pCalRenderer->beginRendering()) return;

  // set the global OpenGL states
  glShadeModel(GL_SMOOTH);
  glDisable(GL_COLOR_MATERIAL);

  // we will use vertex arrays, so enable them
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  // get the number of meshes
  int meshCount;
  meshCount = pCalRenderer->getMeshCount();

  /* Load the ONLY ONE mesh of the model */
  int meshId = 0;
  if(meshId < meshCount)
  {
    // get the number of submeshes
    int submeshCount;
    submeshCount = pCalRenderer->getSubmeshCount(meshId);

    // Load the ONLY ONE submesh of the mesh
    int submeshId = 0;
    if(submeshId < submeshCount)
    {
      // select mesh and submesh for further data access
      if(pCalRenderer->selectMeshSubmesh(meshId, submeshId))
      {
        unsigned char meshColor[4];
        GLfloat materialColor[4];

        // set the material ambient color
        pCalRenderer->getAmbientColor(&meshColor[0]);
        materialColor[0] = meshColor[0] / 255.0f;  
        materialColor[1] = meshColor[1] / 255.0f; 
        materialColor[2] = meshColor[2] / 255.0f; 
        materialColor[3] = meshColor[3] / 255.0f;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialColor);

        // set the material diffuse color
        pCalRenderer->getDiffuseColor(&meshColor[0]);
        materialColor[0] = meshColor[0] / 255.0f;  
        materialColor[1] = meshColor[1] / 255.0f; 
        materialColor[2] = meshColor[2] / 255.0f; 
        materialColor[3] = meshColor[3] / 255.0f;
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialColor);

        // set the material specular color
        pCalRenderer->getSpecularColor(&meshColor[0]);
        materialColor[0] = meshColor[0] / 255.0f;  
        materialColor[1] = meshColor[1] / 255.0f; 
        materialColor[2] = meshColor[2] / 255.0f; 
        materialColor[3] = meshColor[3] / 255.0f;
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialColor);

        // set the material shininess factor
        float shininess;
        shininess = 50.0f;// pCalRenderer->getShininess();
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);

        // get the transformed vertices of the submesh
        
        int vertexCount;
        vertexCount = pCalRenderer->getVertices(&meshVertices[0][0]);

        // get the transformed normals of the submesh
        pCalRenderer->getNormals(&meshNormals[0][0]);

        // get the texture coordinates of the submesh
        textureCoordinateCount = pCalRenderer->getTextureCoordinates(0,
                                         &meshTextureCoordinates[0][0]);

        // get the faces of the submesh
        faceCount = pCalRenderer->getFaces(&meshFaces[0][0]);

        // set the vertex and normal buffers
        glVertexPointer(3, GL_FLOAT, 0, &meshVertices[0][0]);
        glNormalPointer(GL_FLOAT, 0, &meshNormals[0][0]);

        // set the texture coordinate buffer and state if necessary
        if( (pCalRenderer->getMapCount() > 0) && 
            (textureCoordinateCount > 0))
        {
          glEnable(GL_TEXTURE_2D);
          glEnableClientState(GL_TEXTURE_COORD_ARRAY);
          glEnable(GL_COLOR_MATERIAL);

          if(useTexture)
          {
             // set the texture id we stored in the map user data
             glBindTexture(GL_TEXTURE_2D, 
                           (unsigned long)pCalRenderer->getMapUserData(0));
          }

          // set the texture coordinate buffer
          glTexCoordPointer(2, GL_FLOAT, 0, &meshTextureCoordinates[0][0]);
        }
      }
    }
  }
}

/*********************************************************************
 *                       renderFromGraphicMemory                     *
 *********************************************************************/
void aniModel::renderFromGraphicMemory(float pX, float pY, float pZ, 
      float angleX, float angleY, float angleZ, bool inverted)
{
   glPushMatrix();
      glTranslatef(pX, (!inverted)?pY:-pY, pZ);
      glRotatef(angleZ, 0.0f, 0.0f, 1.0f);
      glRotatef(angleX, 1.0f, 0.0f, 0.0f);
      glRotatef(angleY, 0.0f, 1.0f, 0.0f);
      if(inverted)
      {
         glScalef(1.0f, -1.0f, 1.0f);
      }
      renderFromGraphicMemory();
   glPopMatrix();
}

/*********************************************************************
 *                       renderFromGraphicMemory                     *
 *********************************************************************/
void aniModel::renderFromGraphicMemory(float pX, float pY, float pZ, 
      float angleX, float angleY, float angleZ, 
      float angle, float aX, float aY, float aZ, bool inverted)
{
   glPushMatrix();
      glTranslatef(pX, pY, pZ);
      glRotatef(angle, aX, aY, aZ);
      glRotatef(angleZ, 0.0f, 0.0f, 1.0f);
      glRotatef(angleX, 1.0f, 0.0f, 0.0f);
      glRotatef(angleY, 0.0f, 1.0f, 0.0f);
      if(inverted)
      {
         glScalef(1.0f, -1.0f, 1.0f);
      }
      renderFromGraphicMemory();
   glPopMatrix();
}

/*********************************************************************
 *                       renderFromGraphicMemory                     *
 *********************************************************************/
void aniModel::renderFromGraphicMemory()
{
  glPushMatrix();
   /* Correct from blender to dnt coordinates */
   glRotatef(180,0,1,0);
   glRotatef(-90,1,0,0);
   /* Scale, if needed */
   if(m_renderScale != 1.0)
   {
      glScalef(m_renderScale,m_renderScale,m_renderScale);
   }

   // draw the loaded thing
   if(sizeof(CalIndex)==2)
   {
      glDrawElements(GL_TRIANGLES, faceCount * 3, 
                     GL_UNSIGNED_SHORT, &meshFaces[0][0]);
   }
   else
   {
	   glDrawElements(GL_TRIANGLES, faceCount * 3, 
                     GL_UNSIGNED_INT, &meshFaces[0][0]);
   }

  glPopMatrix();

  glColor3f(1.0f, 1.0f, 1.0f);

}

/*********************************************************************
 *                       removeFromgraphicMemory                     *
 *********************************************************************/
void aniModel::removeFromGraphicMemory()
{
   if( (pCalRenderer->getMapCount() > 0) &&
       (textureCoordinateCount > 0) )
   {
      glDisable(GL_COLOR_MATERIAL);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      glDisable(GL_TEXTURE_2D);
  }
  // clear vertex array state
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  pCalRenderer->endRendering();
}

/*********************************************************************
 *                       renderBoundingBox                           *
 *********************************************************************/
void aniModel::renderBoundingBox()
{  
   CalSkeleton *pCalSkeleton = m_calModel->getSkeleton();

   std::vector<CalBone*> &vectorCoreBone = pCalSkeleton->getVectorBone();

   glColor4f(1.0f, 1.0f, 0.5f, 0.5f);
   glBegin(GL_LINES);      

   for(size_t boneId=0;boneId<vectorCoreBone.size();++boneId)
   {
      CalBoundingBox &calBoundingBox = vectorCoreBone[boneId]->getBoundingBox();

      CalVector p[8];
      calBoundingBox.computePoints(p);


      glVertex3f(p[0].x,p[0].y,p[0].z);
      glVertex3f(p[1].x,p[1].y,p[1].z);

      glVertex3f(p[0].x,p[0].y,p[0].z);
      glVertex3f(p[2].x,p[2].y,p[2].z);

      glVertex3f(p[1].x,p[1].y,p[1].z);
      glVertex3f(p[3].x,p[3].y,p[3].z);

      glVertex3f(p[2].x,p[2].y,p[2].z);
      glVertex3f(p[3].x,p[3].y,p[3].z);

      glVertex3f(p[4].x,p[4].y,p[4].z);
      glVertex3f(p[5].x,p[5].y,p[5].z);

      glVertex3f(p[4].x,p[4].y,p[4].z);
      glVertex3f(p[6].x,p[6].y,p[6].z);

      glVertex3f(p[5].x,p[5].y,p[5].z);
      glVertex3f(p[7].x,p[7].y,p[7].z);

      glVertex3f(p[6].x,p[6].y,p[6].z);
      glVertex3f(p[7].x,p[7].y,p[7].z);

      glVertex3f(p[0].x,p[0].y,p[0].z);
      glVertex3f(p[4].x,p[4].y,p[4].z);

      glVertex3f(p[1].x,p[1].y,p[1].z);
      glVertex3f(p[5].x,p[5].y,p[5].z);

      glVertex3f(p[2].x,p[2].y,p[2].z);
      glVertex3f(p[6].x,p[6].y,p[6].z);

      glVertex3f(p[3].x,p[3].y,p[3].z);
      glVertex3f(p[7].x,p[7].y,p[7].z);  

   }

   glEnd();

}

/*********************************************************************
 *                                Render                             *
 *********************************************************************/
void aniModel::render()
{
  glPushMatrix();
   /* Correct from blender to dnt coordinates */
   glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
   glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
   /* Scale, if needed */
   if(m_renderScale != 1.0f)
   {
      glScalef(m_renderScale,m_renderScale,m_renderScale);
   }

  m_calModel->getSkeleton()->calculateBoundingBoxes();
  // get the renderer of the model
  CalRenderer *pCalRenderer;
  pCalRenderer = m_calModel->getRenderer();

  if(!pCalRenderer->beginRendering()) return;

  // set the global OpenGL states
  glShadeModel(GL_SMOOTH);
  glDisable(GL_COLOR_MATERIAL);

  // we will use vertex arrays, so enable them
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  // get the number of meshes
  int meshCount;
  meshCount = pCalRenderer->getMeshCount();

  // render all meshes of the model
  int meshId;
  for(meshId = 0; meshId < meshCount; meshId++)
  {
    // get the number of submeshes
    int submeshCount;
    submeshCount = pCalRenderer->getSubmeshCount(meshId);

    // render all submeshes of the mesh
    int submeshId;
    for(submeshId = 0; submeshId < submeshCount; submeshId++)
    {
      // select mesh and submesh for further data access
      if(pCalRenderer->selectMeshSubmesh(meshId, submeshId))
      {
        unsigned char meshColor[4];
        GLfloat materialColor[4];

        // set the material ambient color
        pCalRenderer->getAmbientColor(&meshColor[0]);
        materialColor[0] = meshColor[0] / 255.0f;  
        materialColor[1] = meshColor[1] / 255.0f; 
        materialColor[2] = meshColor[2] / 255.0f; 
        materialColor[3] = meshColor[3] / 255.0f;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialColor);

        // set the material diffuse color
        pCalRenderer->getDiffuseColor(&meshColor[0]);
        materialColor[0] = meshColor[0] / 255.0f;  
        materialColor[1] = meshColor[1] / 255.0f; 
        materialColor[2] = meshColor[2] / 255.0f; 
        materialColor[3] = meshColor[3] / 255.0f;
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialColor);

        // set the material specular color
        pCalRenderer->getSpecularColor(&meshColor[0]);
        materialColor[0] = meshColor[0] / 255.0f;  
        materialColor[1] = meshColor[1] / 255.0f; 
        materialColor[2] = meshColor[2] / 255.0f; 
        materialColor[3] = meshColor[3] / 255.0f;
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialColor);

        // set the material shininess factor
        float shininess;
        shininess = 50.0f;// pCalRenderer->getShininess();
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);

        // get the transformed vertices of the submesh
        int vertexCount;
        vertexCount = pCalRenderer->getVertices(&meshVertices[0][0]);

        // get the transformed normals of the submesh
        pCalRenderer->getNormals(&meshNormals[0][0]);

        // get the texture coordinates of the submesh
        textureCoordinateCount = pCalRenderer->getTextureCoordinates(0,
                                         &meshTextureCoordinates[0][0]);

        // get the faces of the submesh
        faceCount = pCalRenderer->getFaces(&meshFaces[0][0]);

        // set the vertex and normal buffers
        glVertexPointer(3, GL_FLOAT, 0, &meshVertices[0][0]);
        glNormalPointer(GL_FLOAT, 0, &meshNormals[0][0]);

        // set the texture coordinate buffer and state if necessary
        if((pCalRenderer->getMapCount() > 0) && (textureCoordinateCount > 0))
        {
          glEnable(GL_TEXTURE_2D);
          glEnableClientState(GL_TEXTURE_COORD_ARRAY);
          glEnable(GL_COLOR_MATERIAL);

          // set the texture id we stored in the map user data
          glBindTexture(GL_TEXTURE_2D, 
                        (unsigned long)pCalRenderer->getMapUserData(0));

          // set the texture coordinate buffer
          glTexCoordPointer(2, GL_FLOAT, 0, &meshTextureCoordinates[0][0]);
          glColor3f(1.0f, 1.0f, 1.0f);
        }

        // draw the submesh
        
        if(sizeof(CalIndex)==2)
			  glDrawElements(GL_TRIANGLES, faceCount * 3, 
                                   GL_UNSIGNED_SHORT, &meshFaces[0][0]);
		  else
			  glDrawElements(GL_TRIANGLES, faceCount * 3, 
                                     GL_UNSIGNED_INT, &meshFaces[0][0]);

        // disable the texture coordinate state if necessary
        if( (pCalRenderer->getMapCount() > 0) 
            && (textureCoordinateCount > 0) )
        {
          glDisable(GL_COLOR_MATERIAL);
          glDisableClientState(GL_TEXTURE_COORD_ARRAY);
          glDisable(GL_TEXTURE_2D);
        }
      }
    }
  }

  // clear vertex array state
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  pCalRenderer->endRendering();

  glPopMatrix();

  glColor3f(1.0f, 1.0f, 1.0f);

}

/*********************************************************************
 *                             RenderShadow                          *
 *********************************************************************/
void aniModel::renderShadow(float pX, float pY, float pZ, float angleX,
            float angleY, float angleZ, GLfloat* shadowMatrix, float alpha)
{
   /* Set Changes */
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_STENCIL_TEST);
   glStencilFunc(GL_EQUAL, 1, 0xffffffff);
   glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
   glPolygonOffset(-2.0f,-1.0f);
   glEnable(GL_POLYGON_OFFSET_FILL);
   glDisable(GL_LIGHTING);
   glDisable(GL_COLOR_MATERIAL);
   glColor4f(0.0f, 0.0f, 0.0f, alpha);

   /* Render */
   glPushMatrix();
      glMultMatrixf(shadowMatrix);
      glPushMatrix();
         glTranslatef(pX, pY, pZ);
         glRotatef(angleZ,0.0f,0.0f,1.0f);
         glRotatef(angleX,1.0f,0.0f,0.0f);
         glRotatef(angleY,0.0f,1.0f,0.0f);
         renderFromGraphicMemory();
     glPopMatrix();
   glPopMatrix();

   /* Unset Changes */
   glEnable(GL_LIGHTING);
   glDisable(GL_POLYGON_OFFSET_FILL);
   glDisable(GL_STENCIL_TEST);
   glEnable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

/*********************************************************************
 *                           renderReflexion                         *
 *********************************************************************/
void aniModel::renderReflexion(float pX, float pY, float pZ, float angleX,
      float angleY, float angleZ)
{
   glEnable(GL_STENCIL_TEST);
   glStencilFunc(GL_EQUAL, 1, 0xffffffff);  /* draw if ==1 */
   glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
   glEnable(GL_NORMALIZE);

   renderFromGraphicMemory(pX, pY, pZ, angleX, angleY, angleZ, true);

   glDisable(GL_NORMALIZE);
   glDisable(GL_STENCIL_TEST);
}

/*********************************************************************
 *                               setState                            *
 *********************************************************************/
void aniModel::setState(int state)
{
   /* Verify if animation is defined */
   if( (state > ANIMODEL_MAX_ANIMATIONS) || (state < 0) ||
       (m_animationId[state] == -1) )
   {
      return;
   }

   /* Only change animation if not actually on it */
   if(state != m_state)
   {
       if(state == STATE_DIE)
       {
          m_calModel->getMixer()->clearCycle(m_animationId[m_state],0.1f);
          
          if(m_state == STATE_ATTACK_MEELE)
          {
             /* The idle is merged with the attack, so clear it too */
             m_calModel->getMixer()->clearCycle(m_animationId[STATE_IDLE],0.1f);
          }

          m_calModel->getMixer()->executeAction(m_animationId[state],
                                                0.3f,0.3f);
          m_calModel->getMixer()->blendCycle(m_animationId[STATE_DEAD],
                                            0.1f,0.1f);
       }
       else if(state == STATE_ATTACK_MEELE)
       {         
          m_calModel->getMixer()->executeAction(m_animationId[state],
                                                0.3f,0.3f);
       }
       else
       {
          m_calModel->getMixer()->clearCycle(m_animationId[m_state],0.1f);
          m_calModel->getMixer()->blendCycle(m_animationId[state],
                                             1.0f,0.1f);
       }
       m_state = state;
   }
}

/*********************************************************************
 *                               getState                            *
 *********************************************************************/
int aniModel::getState()
{
   return(m_state);
}

/*********************************************************************
 *                                update                             *
 *********************************************************************/
void aniModel::update(GLfloat pos, float angleY, float pX, float pY, float pZ)
{
   curPos = pos;
   m_calModel->update(pos);
   updateKeyVertex(leftHand, angleY, pX, pY, pZ);
   updateKeyVertex(rightHand, angleY, pX, pY, pZ);
   updateKeyVertex(head, angleY, pX, pY, pZ);
}

/*********************************************************************
 *                            getCurrentPos                          *
 *********************************************************************/
GLfloat aniModel::getCurrentPos()
{
   return(curPos); 
}

/*********************************************************************
 *                             getBoneId                             *
 *********************************************************************/
int aniModel::getBoneId(string bName)
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
 *                         getInfluencedVertex                       *
 *********************************************************************/
bool aniModel::getInfluencedVertex(int boneId, vertexInfo& inf)
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
                  inf.iY = vert[v].position.z;
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
void aniModel::defineKeyVertex()
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

/*********************************************************************
 *                          updateKeyVertex                          *
 *********************************************************************/
void aniModel::updateKeyVertex(vertexInfo& v, 
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
            ((-meshVertices[v.vertexId][0])*angleCos*m_renderScale) +
            ((meshVertices[v.vertexId][1])*angleSin*m_renderScale);
      v.y = pY + meshVertices[v.vertexId][2]*m_renderScale;
      v.z = pZ + 
            ((meshVertices[v.vertexId][0]*angleSin*m_renderScale)) +
            ((meshVertices[v.vertexId][1]*angleCos*m_renderScale));
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
 *                           depthColision                           *
 *********************************************************************/
bool aniModel::depthCollision(GLfloat angleX, GLfloat angleY, GLfloat angleZ,
      GLfloat pX, GLfloat pY, GLfloat pZ, boundingBox colBox)
{
   /* Calculate the sin and cos of the angles, to do less calculations */
   float sinAngleX = sinf(deg2Rad(angleX));
   float cosAngleX = cosf(deg2Rad(angleX));
   float sinAngleY = sinf(deg2Rad(angleY));
   float cosAngleY = cosf(deg2Rad(angleY));
   float sinAngleZ = sinf(deg2Rad(angleZ));
   float cosAngleZ = cosf(deg2Rad(angleZ));

   GLushort* facesShort = NULL;
   GLuint* facesInt = NULL;

   /* get the renderer of the model */
   pCalRenderer = m_calModel->getRenderer();

   /* get the number of meshes */
   int meshCount;
   meshCount = pCalRenderer->getMeshCount();

   /* verify all meshes of the model */
   int meshId;
   for(meshId = 0; meshId < meshCount; meshId++)
   {
      /* get the number of submeshes */
      int submeshCount;
      submeshCount = pCalRenderer->getSubmeshCount(meshId);

      /* verify all submeshes of the mesh */
      int submeshId;
      for(submeshId = 0; submeshId < submeshCount; submeshId++)
      {
         /* select mesh and submesh for further data access */
         if(pCalRenderer->selectMeshSubmesh(meshId, submeshId))
         {
            /* get the transformed vertices of the submesh */
            int vertexCount;
            vertexCount = pCalRenderer->getVertices(&meshVertices[0][0]);

            /* get faces */
            faceCount = pCalRenderer->getFaces(&meshFaces[0][0]);
         
            /* Transform faces pointer to the desired one */
            if(sizeof(CalIndex)==2)
            {
               facesShort = (GLushort*)(void*)&meshFaces[0][0];
            }
            else
            {
               facesInt = (GLuint*)(void*)&meshFaces[0][0];
            }

            /* Verify each model face with each bounding box face */
            int f;
            float V0[3], V1[3], V2[3];
            float U0[3], U1[3], U2[3];
            int index0=0, index1=0, index2=0;
            for(f = 0; f < faceCount*3; f+=3)
            {

               /* Define Triangle Vertex index */
               if(sizeof(CalIndex)==2)
               {
                  index0 = facesShort[f];
                  index1 = facesShort[f+1];
                  index2 = facesShort[f+2];
               }
               else
               {
                  index0 = facesInt[f];
                  index1 = facesInt[f+1];
                  index2 = facesInt[f+2];
               }

               /* Translate and rotate the coordinates.
                * NOTE: Do not forget that if the blender coordinate system is
                * (x,y,z), the DNT system is (-x,z,y) */
               rotatePoint(-meshVertices[index0][0]*m_renderScale,
                           meshVertices[index0][2]*m_renderScale,
                           meshVertices[index0][1]*m_renderScale,
                           angleX, angleY, angleZ, sinAngleX, cosAngleX, 
                           sinAngleY, cosAngleY, sinAngleZ, cosAngleZ,
                           V0[0], V0[1], V0[2]);
               V0[0] += pX;
               V0[1] += pY;
               V0[2] += pZ;

               rotatePoint(-meshVertices[index1][0]*m_renderScale,
                           meshVertices[index1][2]*m_renderScale,
                           meshVertices[index1][1]*m_renderScale,
                           angleX, angleY, angleZ, sinAngleX, cosAngleX, 
                           sinAngleY, cosAngleY, sinAngleZ, cosAngleZ,
                           V1[0], V1[1], V1[2]);
               V1[0] += pX;
               V1[1] += pY;
               V1[2] += pZ;

               rotatePoint(-meshVertices[index2][0]*m_renderScale,
                           meshVertices[index2][2]*m_renderScale,
                           meshVertices[index2][1]*m_renderScale,
                           angleX, angleY, angleZ, sinAngleX, cosAngleX, 
                           sinAngleY, cosAngleY, sinAngleZ, cosAngleZ,
                           V2[0], V2[1], V2[2]);
               V2[0] += pX;
               V2[1] += pY;
               V2[2] += pZ;

               /* Bounding Box Faces */

               /* Upper Face A */
               U0[0] = colBox.x1;
               U0[1] = colBox.y2;   
               U0[2] = colBox.z1;
               
               U1[0] = colBox.x2;
               U1[1] = colBox.y2;
               U1[2] = colBox.z1;

               U2[0] = colBox.x1;
               U2[1] = colBox.y2;
               U2[2] = colBox.z2;

               if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
               {
                  /* Detected Collision at Upper Face A! */
                  return(true);
               }

               /* Upper Face B */
               U0[0] = colBox.x2;
               U0[1] = colBox.y2;
               U0[2] = colBox.z2;
               //U1 = same as upper face A
               //U2 = same as upper face A
               

               if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
               {
                  /* Detected Collision at Upper Face B! */
                  return(true);
               }

               /* Lower Face A */
               U0[0] = colBox.x1;
               U0[1] = colBox.y1;   
               U0[2] = colBox.z1;
               
               U1[0] = colBox.x2;
               U1[1] = colBox.y1;
               U1[2] = colBox.z1;

               U2[0] = colBox.x1;
               U2[1] = colBox.y1;
               U2[2] = colBox.z2;

               if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
               {
                  /* Detected Collision at Lower Face A! */
                  return(true);
               }

               /* Lower Face B */
               U0[0] = colBox.x2;
               U0[1] = colBox.y1;
               U0[2] = colBox.z2;
               //U1 = same as lower face A
               //U2 = same as lower face A
               

               if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
               {
                  /* Detected Collision at Lower Face B! */
                  return(true);
               }

               
               /* Left Face A */
               U0[0] = colBox.x1;
               U0[1] = colBox.y1;   
               U0[2] = colBox.z1;
               
               U1[0] = colBox.x1;
               U1[1] = colBox.y1;
               U1[2] = colBox.z2;

               U2[0] = colBox.x1;
               U2[1] = colBox.y2;
               U2[2] = colBox.z1;

               if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
               {
                  /* Detected Collision at Left Face A! */  
                  return(true);
               }

               /* Left Face B */
               U0[0] = colBox.x1;
               U0[1] = colBox.y2;
               U0[2] = colBox.z2;
               //U1 = same as left face A
               //U2 = same as left face A

               if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
               {
                  /* Detected Collision at Left Face B! */
                  return(true);
               }

               /* Right Face A */
               U0[0] = colBox.x2;
               U0[1] = colBox.y1;   
               U0[2] = colBox.z1;
               
               U1[0] = colBox.x2;
               U1[1] = colBox.y1;
               U1[2] = colBox.z2;

               U2[0] = colBox.x2;
               U2[1] = colBox.y2;
               U2[2] = colBox.z1;

               if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
               {
                  /* Detected Collision at Right Face A! */
                  return(true);
               }

               /* Right Face B */
               U0[0] = colBox.x2;
               U0[1] = colBox.y2;
               U0[2] = colBox.z2;
               //U1 = same as right face A
               //U2 = same as right face A

               if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
               {
                  /* Detected Collision at Right Face B! */
                  return(true);
               }

            
               /* Front Face A */
               U0[0] = colBox.x1;
               U0[1] = colBox.y1;   
               U0[2] = colBox.z1;
               
               U1[0] = colBox.x2;
               U1[1] = colBox.y1;
               U1[2] = colBox.z1;

               U2[0] = colBox.x1;
               U2[1] = colBox.y2;
               U2[2] = colBox.z1;

               if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
               {
                  /* Detected Collision at Front Face A! */
                  return(true);
               }

               /* Front Face B */
               U0[0] = colBox.x2;
               U0[1] = colBox.y2;
               U0[2] = colBox.z1;
               //U1 = same as front face A
               //U2 = same as front face A

               if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
               {
                  /* Detected Collision at Front Face B! */
                  return(true);
               }

               /* Back Face A */
               U0[0] = colBox.x1;
               U0[1] = colBox.y1;   
               U0[2] = colBox.z2;
               
               U1[0] = colBox.x2;
               U1[1] = colBox.y1;
               U1[2] = colBox.z2;

               U2[0] = colBox.x1;
               U2[1] = colBox.y2;
               U2[2] = colBox.z2;

               if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
               {
                  /* Detected Collision at Back Face A! */
                  return(true);
               }

               /* Back Face B */
               U0[0] = colBox.x2;
               U0[1] = colBox.y2;
               U0[2] = colBox.z2;
               //U1 = same as back face A
               //U2 = same as back face A

               if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
               {
                  /* Detected Collision at Back Face B! */
                  return(true);
               }
            }
         }
      }
   }

  /* If got here, no collision occurs */
  return(false);
}

/*********************************************************************
 *                          Static Variables                         *
 *********************************************************************/
float aniModel::meshVertices[30000][3];
float aniModel::meshNormals[30000][3];
float aniModel::meshTextureCoordinates[30000][2];
CalIndex aniModel::meshFaces[50000][3];

