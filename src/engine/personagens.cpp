/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "personagens.h"
#include <SDL/SDL_image.h>

/*********************************************************************
 *                    Construtor do Personagem                       *
 *********************************************************************/
personagem::personagem()
{
  m_calCoreModel = new CalCoreModel("");

  m_state = STATE_IDLE;
  m_motionBlend[0] = 1.0f;
  m_motionBlend[1] = 1.0f;
  m_motionBlend[2] = 1.0f;
  m_animationCount = 0;
  m_meshCount = 0;
  m_renderScale = 1.0f;
  m_lodLevel = 1.0f;
}

/*********************************************************************
 *                    Destruidor do Personagem                       *
 *********************************************************************/
personagem::~personagem()
{
  
}

/*********************************************************************
 *                    Carrega Textura do Personagem                  *
 *********************************************************************/
GLuint personagem::loadTexture(const string& strFilename)
{
   GLuint pId = 0;
 
   /* carrega a textura do arquivo */
   const char* arq;
   arq = strFilename.c_str();
   SDL_Surface* img = IMG_Load(arq);
   if(!img)
   {
      printf("Can't Open Texture");
      cout << strFilename << endl; 
      //free(arq);
      return(0);
   }

   
   /* Transforma a textura em potencia de 2 */
   SDL_Surface *imgPotencia = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       img->w,img->h,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   SDL_BlitSurface(img,NULL,imgPotencia,NULL);  


   glGenTextures(1, &pId);
   glBindTexture(GL_TEXTURE_2D, pId);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,imgPotencia->w,imgPotencia->h, 
                0, GL_RGBA, GL_UNSIGNED_BYTE, imgPotencia->pixels);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   /* Libera a memoria utilizada */
   SDL_FreeSurface(img);
   SDL_FreeSurface(imgPotencia);

   //free(arq);

   return pId;
} 

/*********************************************************************
 *              Carrega o modelo Cal3d do personagem                 *
 *********************************************************************/
bool personagem::LoadModel(const string& strFilename)
{
   /* CAL3D Load Model  */  

   // open the model configuration file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
  if(!file)
  {
    std::cerr << "Failed to open model configuration file '" << strFilename << "'." << std::endl;
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
      std::cout << "Loading skeleton '" << strData << "'..." << std::endl;
      if(!m_calCoreModel->loadCoreSkeleton(strPath + strData))
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "animation")
    {
      // load core animation
      std::cout << "Loading animation '" << strData << "'..." << std::endl;
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
      std::cout << "Loading mesh '" << strData << "'..." << std::endl;
      if(m_calCoreModel->loadCoreMesh(strPath + strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "material")
    {
      // load core material
      std::cout << "Loading material '" << strData << "'..." << std::endl;
      if(m_calCoreModel->loadCoreMaterial(strPath + strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else
    {
      std::cerr << strFilename << "(" << line << "): Invalid syntax." << std::endl;
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
      pCoreMaterial->setMapUserData(mapId, (Cal::UserData)textureId);
    }
  }

  // make one material thread for each material
  // NOTE: this is not the right way to do it, but this viewer can't do the right
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
  SetState(STATE_IDLE);

   /* End of CAL3D LOAD */
   return(true);
}

/*********************************************************************
 *                       Renderiza Personagem                        *
 *********************************************************************/
void personagem::Render()
{
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
        materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);

        // set the material diffuse color
        pCalRenderer->getDiffuseColor(&meshColor[0]);
        materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);

        // set the material specular color
        pCalRenderer->getSpecularColor(&meshColor[0]);
        materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
        glMaterialfv(GL_FRONT, GL_SPECULAR, materialColor);

        // set the material shininess factor
        float shininess;
        shininess = 50.0f; //TODO: pCalRenderer->getShininess();
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
          glBindTexture(GL_TEXTURE_2D, (GLuint)pCalRenderer->getMapUserData(0));

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
 *               Seta Estado da animação do Personagem               *
 *********************************************************************/
void personagem::SetState(int state)
{
   if(state != m_state)
   {
       if(state == STATE_IDLE)
       {
           m_calModel->getMixer()->clearCycle(m_animationId[STATE_WALK],0.02);
           m_calModel->getMixer()->blendCycle(m_animationId[STATE_IDLE],
                                             1.0f,0.0);
         
          m_state = STATE_IDLE;
       }
       else if(state == STATE_WALK)
       {
          m_calModel->getMixer()->clearCycle(m_animationId[STATE_IDLE],0.02);
          m_calModel->getMixer()->blendCycle(m_animationId[STATE_WALK],
                                             1.0f,0.0);
          m_state = STATE_WALK;

       }
//TODO add others states to model
   }
}


/*********************************************************************
 *                      Destruidor da Lista                          *
 *********************************************************************/
Lpersonagem::~Lpersonagem()
{
   personagem* per = (personagem*) primeiro->proximo;
   int aux;
   for(aux=0;aux<total;aux++)
   {
      RetirarPersonagem(per,0);
      per = (personagem*) per->proximo;
   }
} 
 
/*********************************************************************
 *                    Insere Personagem na Lista                     *
 *********************************************************************/
personagem* Lpersonagem::InserirPersonagem(int forca,int agilidade,
                                           int inteligencia,
                                           int esperteza, char* retrato,
                                           string nome, string arqmodelo)

{
   personagem* novo;
   novo = new(personagem);
   novo->retrato = new(Tlista);
   novo->objetos = new(Tlista);
   //novo->x = x;
   //novo->y = y;
   novo->tipo = PERSONAGEM;
   novo->forca        = forca;
   novo->agilidade    = agilidade;
   novo->inteligencia = inteligencia;
   novo->esperteza    = esperteza;
   novo->armaAtual = NULL;
   novo->orientacao = 0.0;
   novo->posicaoFrente = 0.0;
   novo->posicaoLadoX = 0.0;
   novo->posicaoLadoZ = 0.0;
   novo->posicaoLadoY = 0.0;
   novo->nome = nome;
   /* Define os Retratos */
   figura* fig;
   fig = novo->retrato->InserirFigura(POSRETX,POSRETY,retrato);
    glGenTextures(1, &novo->portrait);
      glBindTexture(GL_TEXTURE_2D, novo->portrait);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fig->fig->w, fig->fig->h, 
                                  0, GL_RGBA, GL_UNSIGNED_BYTE, fig->fig->pixels);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#if 0
   /* Abre o modelo3d */
   novo-> modelo3d = glmReadOBJ(arqmodelo,dirTexturas,1); 
   /* Define os grupos */
     novo->pe_d = _glmFindGroup(novo->modelo3d, "pe_d");
     novo->pe_e = _glmFindGroup(novo->modelo3d, "pe_e");
     novo->perna_d = _glmFindGroup(novo->modelo3d, "perna_d");
     novo->perna_e = _glmFindGroup(novo->modelo3d, "perna_e");
     novo->coxa_d = _glmFindGroup(novo->modelo3d, "coxa_d");
     novo->coxa_e = _glmFindGroup(novo->modelo3d, "coxa_e");
     novo->bacia_d = _glmFindGroup(novo->modelo3d, "bacia_d");
     novo->bacia_e = _glmFindGroup(novo->modelo3d, "bacia_e");
     novo->tronco = _glmFindGroup(novo->modelo3d, "tronco");
     novo->ante_d = _glmFindGroup(novo->modelo3d, "ante_d");
     novo->ante_e = _glmFindGroup(novo->modelo3d, "ante_e");
     novo->braco_d = _glmFindGroup(novo->modelo3d, "braco_d");
     novo->braco_e = _glmFindGroup(novo->modelo3d, "braco_e");
     novo->mao_d = _glmFindGroup(novo->modelo3d, "mao_d");
     novo->mao_e = _glmFindGroup(novo->modelo3d, "mao_e");
     novo->cabeca = _glmFindGroup(novo->modelo3d, "cabeca");
#endif

   /*string arquivo;

   //Initialize model
   novo->coreModel = new CalCoreModel(nome);
   arquivo = arqmodelo + "modelo.xsf";
   //Next, Load model's armature
   if(!novo->coreModel->loadCoreSkeleton(arquivo))
   {
     printf("Can't Load Armature for Character: ");
     cout << arquivo << endl;
   }
   //Next, load model's meshes*/

   novo->LoadModel(arqmodelo);
      
   novo->tipo = PERSONAGEM;
   InserirObj(novo);
   personagemAtivo = novo;
   /*novo->personagemDesenhar = glGenLists(1);
   glNewList(novo->personagemDesenhar,GL_COMPILE);
     glmDraw(novo->modelo3d, GLM_NONE | GLM_COLOR | GLM_SMOOTH | GLM_TEXTURE);
   glEndList();*/
   return(novo);
} 

/*********************************************************************
 *                   Retira Personagem da Lista                      *
 *********************************************************************/
void Lpersonagem::RetirarPersonagem(personagem* persona, int tiraMemoria)
{
   //glDeleteLists(persona->personagemDesenhar,1);
   glDeleteTextures(1,&persona->portrait);
   //glmDelete(persona->modelo3d);
   delete(persona->m_calCoreModel);
   delete(persona->m_calModel);
   delete(persona->retrato);
   delete(persona->objetos);
   if(tiraMemoria)
     Retirar(persona);
}

