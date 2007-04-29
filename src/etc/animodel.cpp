/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "animodel.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>

/**********************************************************************
 *                            Constructor                             *
 **********************************************************************/
aniModel::aniModel()
{
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
 
   /* carrega a textura do arquivo */
   SDL_Surface* img = IMG_Load(strFilename.c_str());
   if(!img)
   {
      cout << "Can't Open Texture" << strFilename << endl; 
      //free(arq);
      return(0);
   }

   
   glGenTextures(1, &pId);
   glBindTexture(GL_TEXTURE_2D, pId);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img->w,img->h, 
                0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, img->w,
                     img->h, GL_RGB, GL_UNSIGNED_BYTE, 
                     img->pixels );

   /* Libera a memoria utilizada */
   SDL_FreeSurface(img);

   //free(arq);

   return pId;
}

/*********************************************************************
 *                        calculateBoundingBox                       *
 *********************************************************************/
void aniModel::calculateBoundingBox()
{
  m_calModel->getSkeleton()->calculateBoundingBoxes();

  GLuint aux, aux2;
  int computed = 0;
  CalVector p[8];

  CalSkeleton *pCalSkeleton = m_calModel->getSkeleton();

  std::vector<CalBone*> &vectorCoreBone = pCalSkeleton->getVectorBone();
  for(aux = 0; aux < vectorCoreBone.size(); aux++)
  {
     CalBoundingBox &calBoundingBox=vectorCoreBone[aux]->getBoundingBox();
     calBoundingBox.computePoints(p);
     
     for(aux2 = 0;aux2 < 8; aux2++)
     {
        if(!computed)
        {
           min[0] = p[aux2].x; max[0] = p[aux2].x; 
           min[1] = p[aux2].y; max[1] = p[aux2].y;
           min[2] = p[aux2].z; max[2] = p[aux2].z;
           computed = 1;
        }
        else
        {
           if(p[aux2].x > max[0])
             max[0] = p[aux2].x;
           if(p[aux2].x < min[0])
             min[0] = p[aux2].x;
           if(p[aux2].y > max[1])
             max[1] = p[aux2].y;
           if(p[aux2].y < min[1])
             min[1] = p[aux2].y;
           if(p[aux2].z > max[2])
             max[2] = p[aux2].z;
           if(p[aux2].z < min[2])
             min[2] = p[aux2].z;
        }
     }
  }

}

/*********************************************************************
 *                              loadModel                            *
 *********************************************************************/
bool aniModel::loadModel(const string& strFilename)
{
   /* CAL3D Load Model  */  

   // open the model configuration file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
  if(!file)
  {
    std::cerr << "Failed to open model configuration file '" << 
                  strFilename << "'." << std::endl;
    return false;
  }

  // initialize the data path
  std::string strPath = m_path;

  // initialize the animation count
  int animationCount;
  animationCount = 0;

  // parse all lines from the model configuration file
  int line;
  for(line = 1; ; line++)
  {
    // read the next model configuration line
    std::string strBuffer;
    std::getline(file, strBuffer);

    // stop if we reached the end of file
    if(file.eof()) break;

    // check if an error happend while reading from the file
    if(!file)
    {
      std::cerr << "Error while reading from the model configuration file '" << strFilename << "'." << std::endl;
      return false;
    }

    // find the first non-whitespace character
    std::string::size_type pos;
    pos = strBuffer.find_first_not_of(" \t");

    // check for empty lines
    if((pos == std::string::npos) || (strBuffer[pos] == '\n') || (strBuffer[pos] == '\r') || (strBuffer[pos] == 0)) continue;

    // check for comment lines
    if(strBuffer[pos] == '#') continue;

    // get the key
    std::string strKey;
    strKey = strBuffer.substr(pos, strBuffer.find_first_of(" =\t\n\r", pos) - pos);
    pos += strKey.size();

    // get the '=' character
    pos = strBuffer.find_first_not_of(" \t", pos);
    if((pos == std::string::npos) || (strBuffer[pos] != '='))
    {
      std::cerr << strFilename << "(" << line << "): Invalid syntax." << std::endl;
      return false;
    }

    // find the first non-whitespace character after the '=' character
    pos = strBuffer.find_first_not_of(" \t", pos + 1);

    // get the data
    std::string strData;
    strData = strBuffer.substr(pos, strBuffer.find_first_of("\n\r", pos) - pos);

    // handle the model creation
    if(strKey == "scale")
    {
      // set rendering scale factor
      m_renderScale = atof(strData.c_str());
    }
    else if(strKey == "path")
    {
      // set the new path for the data files if one hasn't been set already
      if (m_path == "") strPath = strData;
    }
    else if(strKey == "skeleton")
    {
      // load core skeleton
      //std::cout << "Loading skeleton '" << strData << "'..." << std::endl;
      if(!m_calCoreModel->loadCoreSkeleton(strPath + strData))
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "animation")
    {
      // load core animation
      //std::cout << "Loading animation '" << strData << animationCount <<"'..." << std::endl;
      m_animationId[animationCount] = m_calCoreModel->loadCoreAnimation(strPath + strData);
      if(m_animationId[animationCount] == -1)
      {
        CalError::printLastError();
        return false;
      }

      animationCount++;
    }
    else if(strKey == "mesh")
    {
      // load core mesh
      //std::cout << "Loading mesh '" << strData << "'..." << std::endl;
      if(m_calCoreModel->loadCoreMesh(strPath + strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "material")
    {
      // load core material
      //std::cout << "Loading material '" << strData << "'..." << std::endl;
      if(m_calCoreModel->loadCoreMaterial(strPath + strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else
    {
      std::cerr << strFilename << "(" << line << "): Invalid Syntax." << std::endl;
      return false;
    }
  }

  // explicitely close the file
  file.close();

  // load all textures and store the opengl texture id in the corresponding map in the material
  int materialId;
  for(materialId = 0; materialId < m_calCoreModel->getCoreMaterialCount(); materialId++)
  {
    // get the core material
    CalCoreMaterial *pCoreMaterial;
    pCoreMaterial = m_calCoreModel->getCoreMaterial(materialId);

    // loop through all maps of the core material
    int mapId;
    for(mapId = 0; mapId < pCoreMaterial->getMapCount(); mapId++)
    {
      // get the filename of the texture
      std::string strFilename;
      strFilename = pCoreMaterial->getMapFilename(mapId);

      // load the texture from the file
      GLuint textureId;
      textureId = loadTexture(strPath + strFilename);

      // store the opengl texture id in the user data of the map
      pCoreMaterial->setMapUserData(mapId, (Cal::UserData&)textureId);
    }
  }

  // make one material thread for each material
  // mapping without further information on the model etc.
  for(materialId = 0; materialId < m_calCoreModel->getCoreMaterialCount(); materialId++)
  {
    // create the a material thread
    m_calCoreModel->createCoreMaterialThread(materialId);

    // initialize the material thread
    m_calCoreModel->setCoreMaterialId(materialId, 0, materialId);
  }

  // Calculate Bounding Boxes

  m_calCoreModel->getCoreSkeleton()->calculateBoundingBoxes(m_calCoreModel);

  m_calModel = new CalModel(m_calCoreModel);

  // attach all meshes to the model
  int meshId;
  for(meshId = 0; meshId < m_calCoreModel->getCoreMeshCount(); meshId++)
  {
    m_calModel->attachMesh(meshId);
  }

  // set the material set of the whole model
  m_calModel->setMaterialSet(0);

  // set initial animation state
  m_state = -1;
  setState(STATE_IDLE);

  m_calModel->update(10);

   /* End of CAL3D LOAD */
   return(true);
}

/*********************************************************************
 *                       renderBoundingBox                           *
 *********************************************************************/
void aniModel::renderBoundingBox()
{  

   CalSkeleton *pCalSkeleton = m_calModel->getSkeleton();

   std::vector<CalBone*> &vectorCoreBone = pCalSkeleton->getVectorBone();

   glColor3f(1.0f, 1.0f, 0.5f);
   glBegin(GL_LINES);      

   for(size_t boneId=0;boneId<vectorCoreBone.size();++boneId)
   {
      CalBoundingBox & calBoundingBox  = vectorCoreBone[boneId]->getBoundingBox();

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

  m_calModel->getSkeleton()->calculateBoundingBoxes();
  // get the renderer of the model
  CalRenderer *pCalRenderer;
  pCalRenderer = m_calModel->getRenderer();

  if(!pCalRenderer->beginRendering()) return;

  // set the global OpenGL states
  glShadeModel(GL_SMOOTH);

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
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);

        // set the material diffuse color
        pCalRenderer->getDiffuseColor(&meshColor[0]);
        materialColor[0] = meshColor[0] / 255.0f;  
        materialColor[1] = meshColor[1] / 255.0f; 
        materialColor[2] = meshColor[2] / 255.0f; 
        materialColor[3] = meshColor[3] / 255.0f;
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);

        // set the material specular color
        pCalRenderer->getSpecularColor(&meshColor[0]);
        materialColor[0] = meshColor[0] / 255.0f;  
        materialColor[1] = meshColor[1] / 255.0f; 
        materialColor[2] = meshColor[2] / 255.0f; 
        materialColor[3] = meshColor[3] / 255.0f;
        glMaterialfv(GL_FRONT, GL_SPECULAR, materialColor);

        // set the material shininess factor
        float shininess;
        shininess = /*50.0f;*/ pCalRenderer->getShininess();
        glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);

        // get the transformed vertices of the submesh
        static float meshVertices[30000][3];
        int vertexCount;
        vertexCount = pCalRenderer->getVertices(&meshVertices[0][0]);

        // get the transformed normals of the submesh
        static float meshNormals[30000][3];
        pCalRenderer->getNormals(&meshNormals[0][0]);

        // get the texture coordinates of the submesh
        static float meshTextureCoordinates[30000][2];
        int textureCoordinateCount;
        textureCoordinateCount = pCalRenderer->getTextureCoordinates(0,
                                         &meshTextureCoordinates[0][0]);

        // get the faces of the submesh
        static CalIndex meshFaces[50000][3];
        int faceCount;
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
          glBindTexture(GL_TEXTURE_2D, (unsigned long)pCalRenderer->getMapUserData(0));

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


}

/*********************************************************************
 *                               setState                            *
 *********************************************************************/
void aniModel::setState(int state)
{
   if(state != m_state)
   {
       m_calModel->getMixer()->clearCycle(m_animationId[m_state],0.1f);
       if(state == STATE_DIE)
       {
          m_calModel->getMixer()->executeAction(m_animationId[state],
                                                0.3f,0.3f);
          m_calModel->getMixer()->blendCycle(m_animationId[STATE_DEAD],
                                            0.1f,0.1f);
       }
       else if(state >= STATE_ATTACK_MEELE)
       {
         m_calModel->getMixer()->executeAction(m_animationId[state],
                                                0.3f,0.3f);
       }
       else
       {
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
void aniModel::update(GLfloat pos)
{
   m_calModel->update(pos);
}

