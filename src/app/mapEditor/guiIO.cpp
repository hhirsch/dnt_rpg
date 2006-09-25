#include "guiIO.h"
#include "../../engine/util.h"

/****************************************************************
 *                         Constructor                          *
 ****************************************************************/
guiIO::guiIO()
{
   gui = new interface(NULL);

   state = GUI_IO_STATE_INITIAL;
   tool = TOOL_NONE;

   /* Open Windows */
   openFileWindow();
   openNavWindow();
   openMainWindow();
   openWallWindow();
   openTerrainWindow();
   openParticleWindow();
   openPortalWindow();
   openTextureWindow();
   openMessageWindow();
   
   /* Camera Things */
   theta=25;
   phi=0;
   d=150;
   centerX = centerZ = 0;
   centerY = 30;
   deltaY = 0;
   cameraX = centerX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centerY + (float) d * sin(deg2Rad(theta));
   cameraZ = centerZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));

}

/****************************************************************
 *                          Destructor                          *
 ****************************************************************/
guiIO::~guiIO()
{
   delete(gui);
}

/****************************************************************
 *                       Open File Window                       *
 ****************************************************************/
void guiIO::openFileWindow()
{
   fileWindow = gui->ljan->InserirJanela(0,0,184,63,"File",1,1,NULL,NULL);
   newButton = fileWindow->objects->InserirBotao(10,37,50,55,
                                                  fileWindow->Cores.corBot.R,
                                                  fileWindow->Cores.corBot.G,
                                                  fileWindow->Cores.corBot.B,
                                                  "New",1,NULL);
   openButton = fileWindow->objects->InserirBotao(51,37,91,55,
                                                  fileWindow->Cores.corBot.R,
                                                  fileWindow->Cores.corBot.G,
                                                  fileWindow->Cores.corBot.B,
                                                  "Open",1,NULL);
   saveButton = fileWindow->objects->InserirBotao(92,37,132,55,
                                                  fileWindow->Cores.corBot.R,
                                                  fileWindow->Cores.corBot.G,
                                                  fileWindow->Cores.corBot.B,
                                                  "Save",1,NULL);
   exitButton = fileWindow->objects->InserirBotao(133,37,173,55,
                                                  fileWindow->Cores.corBot.R,
                                                  fileWindow->Cores.corBot.G,
                                                  fileWindow->Cores.corBot.B,
                                                  "Exit",1,NULL);
   fileText = fileWindow->objects->InserirBarraTexto(10,17,173,33,
                                                     "../data/mapas/",0,NULL);
   fileWindow->fechavel = 0;
   fileWindow->ptrExterno = &fileWindow;
   fileWindow->Abrir(gui->ljan);
}

/****************************************************************
 *                       Open Texture Window                    *
 ****************************************************************/
void guiIO::openTextureWindow()
{
   textureWindow = gui->ljan->InserirJanela(0,164,184,244,"Texture",1,1,
                                            NULL, NULL);
   texturePreviousButton = textureWindow->objects->InserirBotao(7,56,74,74,
                                                  textureWindow->Cores.corBot.R,
                                                  textureWindow->Cores.corBot.G,
                                                  textureWindow->Cores.corBot.B,
                                                  "Previous",1,NULL);
   textureNextButton = textureWindow->objects->InserirBotao(110,56,177,74,
                                                  textureWindow->Cores.corBot.R,
                                                  textureWindow->Cores.corBot.G,
                                                  textureWindow->Cores.corBot.B,
                                                  "Next",1,NULL);
   textureInsertButton = textureWindow->objects->InserirBotao(20,35,153,53,
                                                  textureWindow->Cores.corBot.R,
                                                  textureWindow->Cores.corBot.G,
                                                  textureWindow->Cores.corBot.B,
                                                  "Insert",1,NULL);
   textureText = textureWindow->objects->InserirBarraTexto(10,17,173,33,
                                                     "../data/texturas/",0,NULL);
   textureWindow->fechavel = 0;
   textureWindow->ptrExterno = &textureWindow;
   textureWindow->Abrir(gui->ljan);
}

/****************************************************************
 *                       Open Nav Window                        *
 ****************************************************************/
void guiIO::openNavWindow()
{
   navWindow = gui->ljan->InserirJanela(799-67,599-74,799,599,"Nav",1,1,
                                        NULL,NULL);
   navTabButton = navWindow->objects->InserirTabButton(7,17,0,0,
                                                    "../data/mapEditor/nav.png");
   moreZoomButton = navTabButton->insertButton(0,0,8,8);    /* More Zoom */
   lessZoomButton = navTabButton->insertButton(9,0,17,8);   /* Less Zoom */
   upButton = navTabButton->insertButton(19,0,31,17);       /* Up */
   downButton = navTabButton->insertButton(19,27,31,44);    /* Down */
   leftButton = navTabButton->insertButton(6,16,23,28);     /* Left */
   rightButton = navTabButton->insertButton(27,16,44,28);   /* Right */
   rotUpButton = navTabButton->insertButton(45,0,53,11);    /* Rotation Up */
   rotDownButton = navTabButton->insertButton(45,13,53,24); /* Rotation Down */
   rotLeftButton = navTabButton->insertButton(40,36,50,50); /* Rotation Left */
   rotRightButton = navTabButton->insertButton(0,36,10,50); /* Rotation Right */
   navWindow->fechavel = 0;
   navWindow->ptrExterno = &navWindow;
   navWindow->Abrir(gui->ljan);
}

/****************************************************************
 *                       Open Message Window                    *
 ****************************************************************/
void guiIO::openMessageWindow()
{
   messageWindow = gui->ljan->InserirJanela(113,599-36,799-68,599,
                                            "Messages",1,1,NULL,NULL);
   messageText = messageWindow->objects->InserirQuadroTexto(7,16,610,31,0,
                 "Welcome to DccNiTghtmare Map Editor!");
   messageWindow->fechavel = 0;
   messageWindow->ptrExterno = &messageWindow;
   messageWindow->Abrir(gui->ljan);
}


/****************************************************************
 *                       Open Main Window                       *
 ****************************************************************/
void guiIO::openMainWindow()
{
   mainWindow = gui->ljan->InserirJanela(0,64,184,163,"Main",1,1,NULL,NULL);
   terrainButton = mainWindow->objects->InserirBotao(5,17,59,35,
                                                     mainWindow->Cores.corBot.R,
                                                     mainWindow->Cores.corBot.G,
                                                     mainWindow->Cores.corBot.B,
                                                     "Terrain",0,NULL);
   wallButton = mainWindow->objects->InserirBotao(60,17,123,35,
                                                  mainWindow->Cores.corBot.R,
                                                  mainWindow->Cores.corBot.G,
                                                  mainWindow->Cores.corBot.B,
                                                  "Wall",0,NULL);
   portalButton = mainWindow->objects->InserirBotao(124,17,179,35,
                                                    mainWindow->Cores.corBot.R,
                                                    mainWindow->Cores.corBot.G,
                                                    mainWindow->Cores.corBot.B,
                                                    "Portal",0,NULL);
   objectButton = mainWindow->objects->InserirBotao(5,36,59,54,
                                                    mainWindow->Cores.corBot.R,
                                                    mainWindow->Cores.corBot.G,
                                                    mainWindow->Cores.corBot.B,
                                                    "Object",0,NULL);
   particleButton = mainWindow->objects->InserirBotao(60,36,123,54,
                                                      mainWindow->Cores.corBot.R,
                                                      mainWindow->Cores.corBot.G,
                                                      mainWindow->Cores.corBot.B,
                                                      "Particle",0,NULL);
   npcButton = mainWindow->objects->InserirBotao(124,36,179,54,
                                                 mainWindow->Cores.corBot.R,
                                                 mainWindow->Cores.corBot.G,
                                                 mainWindow->Cores.corBot.B,
                                                 "NPC",0,NULL);
   destroyButton = mainWindow->objects->InserirBotao(5,55,59,73,
                                                     mainWindow->Cores.corBot.R,
                                                     mainWindow->Cores.corBot.G,
                                                     mainWindow->Cores.corBot.B,
                                                     "Destroy",0,NULL);
   lightButton = mainWindow->objects->InserirBotao(60,55,123,73,
                                                   mainWindow->Cores.corBot.R,
                                                   mainWindow->Cores.corBot.G,
                                                   mainWindow->Cores.corBot.B,
                                                   "Light",0,NULL);
   fogButton = mainWindow->objects->InserirBotao(124,55,179,73,
                                                 mainWindow->Cores.corBot.R,
                                                 mainWindow->Cores.corBot.G,
                                                 mainWindow->Cores.corBot.B,
                                                 "Fog",0,NULL);
   optionsButton = mainWindow->objects->InserirBotao(5,74,59,92,
                                                     mainWindow->Cores.corBot.R,
                                                     mainWindow->Cores.corBot.G,
                                                     mainWindow->Cores.corBot.B,
                                                     "Options",0,NULL);
   musicButton = mainWindow->objects->InserirBotao(60,74,123,92,
                                                   mainWindow->Cores.corBot.R,
                                                   mainWindow->Cores.corBot.G,
                                                   mainWindow->Cores.corBot.B,
                                                   "Music",0,NULL);
   sndFxButton = mainWindow->objects->InserirBotao(124,74,179,92,
                                                 mainWindow->Cores.corBot.R,
                                                 mainWindow->Cores.corBot.G,
                                                 mainWindow->Cores.corBot.B,
                                                 "SndFx",0,NULL);


   mainWindow->fechavel = 0;
   mainWindow->ptrExterno = &mainWindow;
   mainWindow->Abrir(gui->ljan);
}

/****************************************************************
 *                       Open Wall Window                       *
 ****************************************************************/
void guiIO::openWallWindow()
{
   wallWindow = gui->ljan->InserirJanela(0,599-123,112,599-62,"Wall",
                                         1,1,NULL,NULL);
   wallTabButton = wallWindow->objects->InserirTabButton(7,17,0,0,
                                                   "../data/mapEditor/wall.png");
   wallXButton = wallTabButton->insertButton(0,0,19,19);          /* Wall X */
   wallZButton = wallTabButton->insertButton(20,0,39,19);         /* Wall Z */
   wallX2Button = wallTabButton->insertButton(40,0,59,19);        /* Wall X */
   wallZ2Button = wallTabButton->insertButton(60,0,79,19);        /* Wall Z */
   wallTextureButton = wallTabButton->insertButton(0,20,19,39);   /* Texture */
   wallLessVerTexture = wallTabButton->insertButton(20,20,39,39); /* Less V */
   wallMoreVerTexture = wallTabButton->insertButton(40,20,59,39); /* More V */
   wallLessHorTexture = wallTabButton->insertButton(60,20,79,39); /* Less H */
   wallMoreHorTexture = wallTabButton->insertButton(80,20,99,39); /* Less H */


   wallWindow->ptrExterno = &wallWindow;
   wallWindow->Abrir(gui->ljan);
}

/****************************************************************
 *                       Open Portal Window                     *
 ****************************************************************/
void guiIO::openPortalWindow()
{
   portalWindow = gui->ljan->InserirJanela(0,599-185,112,599-124,"Portal",
                                           1,1,NULL,NULL);
   portalTabButton = portalWindow->objects->InserirTabButton(7,17,0,0,
                                                 "../data/mapEditor/portal.png");
   portalAddButton = portalTabButton->insertButton(0,0,19,19); /* Add */
   portalTagButton = portalTabButton->insertButton(20,0,39,19); /* Tag */
   portalWindow->ptrExterno = &portalWindow;
   portalWindow->Abrir(gui->ljan);
}


/****************************************************************
 *                       Open Terrain Window                    *
 ****************************************************************/
void guiIO::openTerrainWindow()
{
   terrainWindow = gui->ljan->InserirJanela(0,599-61,112,599,"Terrain",
                                            1,1,NULL,NULL);
   terrainTabButton = terrainWindow->objects->InserirTabButton(7,17,0,0,
                                                "../data/mapEditor/terrain.png");
   terrainUpButton = terrainTabButton->insertButton(0,0,19,19);    
   terrainNivButton = terrainTabButton->insertButton(20,0,39,19);  
   terrainDownButton = terrainTabButton->insertButton(40,0,59,19); 
   terrainWalkableButton = terrainTabButton->insertButton(0,20,19,39);
   terrainTextureButton = terrainTabButton->insertButton(20,20,39,39);
   terrainWindow->ptrExterno = &terrainWindow;
   terrainWindow->Abrir(gui->ljan);
}

/****************************************************************
 *                       Open Particle Window                   *
 ****************************************************************/
void guiIO::openParticleWindow()
{
   particleWindow = gui->ljan->InserirJanela(0,599-247,112,599-186,"Particle",
                                             1,1,NULL,NULL);
   particleTabButton = particleWindow->objects->InserirTabButton(7,17,0,0,
                                               "../data/mapEditor/particle.png");
   particleWindow->ptrExterno = &particleWindow;
   particleWindow->Abrir(gui->ljan);
}



/****************************************************************
 *                           getState                           *
 ****************************************************************/
int guiIO::getState()
{
   return(state);
}

/****************************************************************
 *                           getTool                            *
 ****************************************************************/
int guiIO::getTool()
{
   return(tool);
}


/****************************************************************
 *                             Draw                             *
 ****************************************************************/
void guiIO::draw(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_BLEND);
   gui->draw(proj,modl,viewPort);
   glEnable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
}


/****************************************************************
 *                             DoIO                             *
 ****************************************************************/
int guiIO::doIO(int mouseX, int mouseY, Uint8 mButton, Uint8 *keys)
{
   int eventInfo;
   Tobjeto* object;
   object = gui->manipulateEvents(mouseX, mouseY, mButton, keys, &eventInfo);
   switch(eventInfo)
   {
      case TABBOTAOPRESSIONADO:
      {
         /*  Navigation Buttons  */
         if(object == (Tobjeto*) upButton)
         {
            centerX -= 4.0 * sin(deg2Rad(phi));
            centerZ -= 4.0 * cos(deg2Rad(phi));
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Tobjeto*) downButton)
         {
            centerX += 4.0 * sin(deg2Rad(phi));
            centerZ += 4.0 * cos(deg2Rad(phi));
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Tobjeto*) leftButton)
         {
            centerX -= 4.0 * sin(deg2Rad(phi)+deg2Rad(90));
            centerZ -= 4.0 * cos(deg2Rad(phi)+deg2Rad(90));
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Tobjeto*) rightButton)
         {
            centerX += 4.0 * sin(deg2Rad(phi)+deg2Rad(90));
            centerZ += 4.0 * cos(deg2Rad(phi)+deg2Rad(90));
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Tobjeto*) rotUpButton)
         {
            theta += 1;
            if(theta > 89) 
               theta = 89;
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Tobjeto*) rotDownButton)
         {
            theta -= 1;
            if(theta < 0)
               theta = 0;
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Tobjeto*) rotLeftButton)
         {
            phi += 1;
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Tobjeto*) rotRightButton)
         {
            phi -= 1;
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Tobjeto*) moreZoomButton)
         {
            d -= 1;
            if (d<1) d = 1;
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Tobjeto*) lessZoomButton)
         {
            d += 1;
            if (d>300) d = 300;
            return(GUI_IO_NEW_POSITION);
         }

         /*  Terrain Buttons  */
         else if(object == (Tobjeto*) terrainUpButton)
         {
            state = GUI_IO_STATE_TERRAIN;
            tool = TOOL_TERRAIN_UP;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Tobjeto*) terrainDownButton)
         {
            state = GUI_IO_STATE_TERRAIN;
            tool = TOOL_TERRAIN_DOWN;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Tobjeto*) terrainNivButton)
         {
            state = GUI_IO_STATE_TERRAIN;
            tool = TOOL_TERRAIN_NIVELATE;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Tobjeto*) terrainTextureButton)
         {
            state = GUI_IO_STATE_TERRAIN;
            tool = TOOL_TERRAIN_TEXTURE;
            return(GUI_IO_NEW_STATE);
         }
         /* Portal Buttons */
         else if(object == (Tobjeto*) portalAddButton)
         {
            state = GUI_IO_STATE_PORTAL;
            tool = TOOL_PORTAL_ADD;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Tobjeto*) portalTagButton)
         {
            state = GUI_IO_STATE_PORTAL;
            tool = TOOL_PORTAL_TAG;
            return(GUI_IO_NEW_STATE);
         }



         break;
      }
      case BOTAOPRESSIONADO:
      {
         if(object == (Tobjeto*) exitButton)
         {
            return(GUI_IO_EXIT);
         }
         else if(object == (Tobjeto*) newButton)
         {
            return(GUI_IO_NEW_MAP);
         }
         else if(object == (Tobjeto*) openButton)
         {
            return(GUI_IO_OPEN_MAP);
         }
         else if(object == (Tobjeto*) saveButton)
         {
            return(GUI_IO_SAVE_MAP);
         }
         else if(object == (Tobjeto*) terrainButton)
         {
            if(!terrainWindow)
            {
               openTerrainWindow();
            }
            return(GUI_IO_OTHER);
         }
         else if(object == (Tobjeto*) wallButton)
         {
            if(!wallWindow)
            {
               openWallWindow();
            }
            return(GUI_IO_OTHER);
         }
         else if(object == (Tobjeto*) portalButton)
         {
            if(!portalWindow)
            {
               openPortalWindow();
            }
            return(GUI_IO_OTHER);
         }
         else if(object == (Tobjeto*) particleButton)
         {
            if(!particleWindow)
            {
               openParticleWindow();
            }
            return(GUI_IO_OTHER);
         }
         /* Texture Window */
         else if(object == (Tobjeto*) textureNextButton)
         {
            return(GUI_IO_TEXTURE_NEXT);
         }
         else if(object == (Tobjeto*) texturePreviousButton)
         {
            return(GUI_IO_TEXTURE_PREVIOUS);
         }
         else if(object == (Tobjeto*) textureInsertButton)
         {
            return(GUI_IO_TEXTURE_INSERT);
         }


         break;
      }
   }
   if(eventInfo == NADA)
   {
      return(GUI_IO_NOTHING);
   }
   else
   {
      return(GUI_IO_OTHER);
   }
}

/****************************************************************
 *                             camera                           *
 ****************************************************************/
void guiIO::camera()
{
   cameraX = centerX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centerY + deltaY + (float) d * sin(deg2Rad(theta));
   cameraZ = centerZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
   gluLookAt(cameraX,cameraY,cameraZ, centerX,centerY,centerZ,0,1,0);
}

/****************************************************************
 *                        getFileName                           *
 ****************************************************************/
string guiIO::getFileName()
{
   return(fileText->texto);
}

/****************************************************************
 *                   getTextureFileName                         *
 ****************************************************************/
string guiIO::getTextureFileName()
{
   return(textureText->texto);
}

/****************************************************************
 *                        Show Message                          *
 ****************************************************************/
void guiIO::showMessage(string msg)
{
   messageText->texto = msg;
   messageWindow->Desenhar(0,0);
}

