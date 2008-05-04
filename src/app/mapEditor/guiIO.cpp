#include "guiIO.h"
#include "../../engine/util.h"

/****************************************************************
 *                         Constructor                          *
 ****************************************************************/
guiIO::guiIO()
{
   gui = new guiInterface(NULL);

   state = GUI_IO_STATE_INITIAL;
   tool = TOOL_NONE;

   selectedText = "";
   curFileName = "";

   fogWindow = NULL;
   actualFog = NULL;
   fileWindow = NULL;

   /* Change the minimun Zoom */
   gameCamera.defineMinZoom(2*ZOOM_MIN);

   /* Open Windows */
   grWindow = new grassWindow(gui);
   wtWindow = new waterWindow(gui);
   ltWindow = new listWindow(gui);
   ltWindow->setState(0);
   openActWindow();
   openNavWindow();
   openMainWindow();
   openWallWindow();
   openTerrainWindow();
   openParticleWindow();
   openPortalWindow();
   openTextureWindow();
   openMessageWindow();
   openObjectWindow();
}

/****************************************************************
 *                          Destructor                          *
 ****************************************************************/
guiIO::~guiIO()
{
   delete(gui);
   delete(grWindow);
   delete(ltWindow);
   delete(wtWindow);
}

/****************************************************************
 *                        getGrassWindow                        *
 ****************************************************************/
grassWindow* guiIO::getGrassWindow()
{
   return(grWindow);
}

/****************************************************************
 *                        getWaterWindow                        *
 ****************************************************************/
waterWindow* guiIO::getWaterWindow()
{
   return(wtWindow);
}

/****************************************************************
 *                      Open File  Window                       *
 ****************************************************************/
void guiIO::openFileWindow(bool load)
{
   fileLoading = load;
   fileWindow = gui->insertWindow(200,100,460,285,"File");
   fileSelector = fileWindow->getObjectsList()->insertFileSel(5,18,load,
                                                              "../data/mapas/");
   fileSelector->setFilter(".map");
   if( (!fileLoading) && (!curFileName.empty()) )
   {
      fileSelector->setFileName(curFileName);
   }
   fileWindow->setAttributes(false,true,false,false);
   fileWindow->setExternPointer(&fileWindow);
   gui->openWindow(fileWindow);
}

/****************************************************************
 *                       Open Act  Window                       *
 ****************************************************************/
void guiIO::openActWindow()
{
   actWindow = gui->insertWindow(0,0,185,63,"Main Actions");
   newButton = actWindow->getObjectsList()->insertButton(10,27,50,45,"New",1);
   openButton = actWindow->getObjectsList()->insertButton(51,27,91,45,
                                                           "Open",1);
   saveButton = actWindow->getObjectsList()->insertButton(92,27,132,45,
                                                           "Save",1);
   exitButton = actWindow->getObjectsList()->insertButton(133,27,173,45,
                                                           "Exit",1);
   actWindow->setAttributes(false,true,false,false);
   actWindow->setExternPointer(&actWindow);
   gui->openWindow(actWindow);
}

/****************************************************************
 *                       Open Texture Window                    *
 ****************************************************************/
void guiIO::openTextureWindow()
{
   textureWindow = gui->insertWindow(0,163,185,244,"Texture");
   texturePreviousButton = textureWindow->getObjectsList()->insertButton(7,56,
                                                                         74,74,
                                                                  "Previous",1);
   textureNextButton = textureWindow->getObjectsList()->insertButton(110,56,
                                                                     177,74,
                                                                     "Next",1);
   textureInsertButton = textureWindow->getObjectsList()->insertButton(20,35,
                                                                       153,53,
                                                                    "Insert",1);
   textureWindow->setAttributes(false,true,false,false);
   textureWindow->setExternPointer(&textureWindow);
   gui->openWindow(textureWindow);
}

/****************************************************************
 *                       Open Object Window                     *
 ****************************************************************/
void guiIO::openObjectWindow()
{
   dirs dir;
   objectWindow = gui->insertWindow(0,244,185,599-248,"Objects");
   objectTabButton = objectWindow->getObjectsList()->insertTabButton(7,17,0,0,
                              dir.getRealFile("mapEditor/objects.png").c_str());
   objectCommonButton = objectTabButton->insertButton(0,0,24,19);
   objectGunsButton = objectTabButton->insertButton(25,0,44,19);
   objectBuildButton = objectTabButton->insertButton(49,0,71,19);
   objectCarsButton = objectTabButton->insertButton(74,0,96,19);
   objectIcexButton = objectTabButton->insertButton(99,0,120,19);
   objectNaturalButton = objectTabButton->insertButton(125,0,145,19);
   objectCharButton = objectTabButton->insertButton(147,0,170,19);
   objectMacabreButton = objectTabButton->insertButton(0,25,24,45);
   objectBathButton = objectTabButton->insertButton(25,25,44,45);
   objectStreetButton = objectTabButton->insertButton(49,25,71,45);
   objectWindow->setExternPointer(&objectWindow);
   gui->openWindow(objectWindow);
}

/****************************************************************
 *                       Open Nav Window                        *
 ****************************************************************/
void guiIO::openNavWindow()
{
   dirs dir;
   navWindow = gui->insertWindow(799-68,599-75,799,599,"Nav");
   navTabButton = navWindow->getObjectsList()->insertTabButton(7,17,0,0,
                                  dir.getRealFile("mapEditor/nav.png").c_str());
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
   navWindow->setAttributes(false,false,false,false);
   navWindow->setExternPointer(&navWindow);
   gui->openWindow(navWindow);
}

/****************************************************************
 *                       Open Message Window                    *
 ****************************************************************/
void guiIO::openMessageWindow()
{
   messageWindow = gui->insertWindow(153,599-37,799-67,599,"Messages");
   messageText = messageWindow->getObjectsList()->insertTextBox(7,16,610,31,0,
                 "Welcome to DccNiTghtmare Map Editor!");
   messageWindow->setAttributes(false,true,false,false);
   messageWindow->setExternPointer(&messageWindow);
   gui->openWindow(messageWindow);
}


/****************************************************************
 *                       Open Main Window                       *
 ****************************************************************/
void guiIO::openMainWindow()
{
   mainWindow = gui->insertWindow(0,63,185,163,"Main");
   terrainButton = mainWindow->getObjectsList()->insertButton(5,17,59,35,
                                                              "Terrain",0);
   wallButton = mainWindow->getObjectsList()->insertButton(60,17,123,35,
                                                           "Wall",0);
   portalButton = mainWindow->getObjectsList()->insertButton(124,17,179,35,
                                                             "Portal",0);
   objectButton = mainWindow->getObjectsList()->insertButton(5,36,59,54,
                                                             "Object",0);
   particleButton =mainWindow->getObjectsList()->insertButton(60,36,123,54,
                                                              "Particle",0);
   npcButton = mainWindow->getObjectsList()->insertButton(124,36,179,54,
                                                          "NPC",0);
   destroyButton = mainWindow->getObjectsList()->insertButton(5,55,59,73,
                                                              "Destroy",0);
   lightButton = mainWindow->getObjectsList()->insertButton(60,55,123,73,
                                                            "Light",0);
   fogButton = mainWindow->getObjectsList()->insertButton(124,55,179,73,
                                                          "Fog",0);
   optionsButton = mainWindow->getObjectsList()->insertButton(5,74,59,92,
                                                              "Options",0);
   musicButton = mainWindow->getObjectsList()->insertButton(60,74,123,92,
                                                            "Music",0);
   sndFxButton = mainWindow->getObjectsList()->insertButton(124,74,179,92,
                                                            "SndFx",0);

   mainWindow->setAttributes(false,true,false,false);
   mainWindow->setExternPointer(&mainWindow);
   gui->openWindow(mainWindow);
}

/****************************************************************
 *                       Open Wall Window                       *
 ****************************************************************/
void guiIO::openWallWindow()
{
   dirs dir;
   wallWindow = gui->insertWindow(0,599-124,153,599-62,"Wall");
   wallTabButton = wallWindow->getObjectsList()->insertTabButton(7,17,0,0,
                                 dir.getRealFile("mapEditor/wall.png").c_str());
   wallXButton = wallTabButton->insertButton(0,0,19,19);          /* Wall X */
   wallZButton = wallTabButton->insertButton(20,0,39,19);         /* Wall Z */
   wallX2Button = wallTabButton->insertButton(40,0,59,19);        /* Wall X */
   wallZ2Button = wallTabButton->insertButton(60,0,79,19);        /* Wall Z */
   wallPreviousButton = wallTabButton->insertButton(80,0,99,19);  /* Previous */
   wallDestroyButton = wallTabButton->insertButton(100,0,119,19); /* Destroy */
   wallNextButton = wallTabButton->insertButton(120,0,139,19);    /* Next */
   wallTextureButton = wallTabButton->insertButton(0,20,19,39);   /* Texture */
   wallLessXTexture = wallTabButton->insertButton(20,20,39,39);   /* Less X */
   wallMoreXTexture = wallTabButton->insertButton(40,20,59,39);   /* More X */
   wallLessYTexture = wallTabButton->insertButton(60,20,79,39);   /* Less Y */
   wallMoreYTexture = wallTabButton->insertButton(80,20,99,39);   /* Less Y */
   wallLessZTexture = wallTabButton->insertButton(100,20,119,39);   /* Less Z */
   wallMoreZTexture = wallTabButton->insertButton(120,20,139,39);   /* Less Z */

   wallWindow->setExternPointer(&wallWindow);
   gui->openWindow(wallWindow);
}

/****************************************************************
 *                       Open Portal Window                     *
 ****************************************************************/
void guiIO::openPortalWindow()
{
   dirs dir;
   portalWindow = gui->insertWindow(0,599-186,153,599-124,"Portal");
   portalTabButton = portalWindow->getObjectsList()->insertTabButton(7,17,0,0,
                               dir.getRealFile("mapEditor/portal.png").c_str());
   portalAddButton = portalTabButton->insertButton(0,0,19,19); /* Add */
   portalTagButton = portalTabButton->insertButton(20,0,39,19); /* Tag */
   portalDoorButton = portalTabButton->insertButton(40,0,59,19); /* Door */
   portalWindow->setExternPointer(&portalWindow);
   gui->openWindow(portalWindow);
}


/****************************************************************
 *                       Open Terrain Window                    *
 ****************************************************************/
void guiIO::openTerrainWindow()
{
   dirs dir;
   terrainWindow = gui->insertWindow(0,599-62,153,599,"Terrain");
   terrainTabButton = terrainWindow->getObjectsList()->insertTabButton(7,17,0,0,
                              dir.getRealFile("mapEditor/terrain.png").c_str());
   terrainUpButton = terrainTabButton->insertButton(0,0,19,19);    
   terrainNivButton = terrainTabButton->insertButton(20,0,39,19);  
   terrainDownButton = terrainTabButton->insertButton(40,0,59,19); 
   terrainWalkableButton = terrainTabButton->insertButton(0,20,19,39);
   terrainTextureButton = terrainTabButton->insertButton(20,20,39,39);
   terrainWindow->setExternPointer(&terrainWindow);
   gui->openWindow(terrainWindow);
}

/****************************************************************
 *                       Open Particle Window                   *
 ****************************************************************/
void guiIO::openParticleWindow()
{
   dirs dir;
   particleWindow = gui->insertWindow(0,599-248,153,599-186,"Particle");
   particleTabButton = particleWindow->getObjectsList()->insertTabButton(7,17,
                                                                         0,0,
                             dir.getRealFile("mapEditor/particle.png").c_str());
   fireButton = particleTabButton->insertButton(0,0,19,19);
   smokeButton = particleTabButton->insertButton(20,0,39,19); 
   waterfallButton = particleTabButton->insertButton(40,0,59,19);
   waterSurfaceButton = particleTabButton->insertButton(60,0,79,19);
   snowButton = particleTabButton->insertButton(80,0,99,19);
   grassButton = particleTabButton->insertButton(0,20,19,39);
   particleWindow->setExternPointer(&particleWindow);
   gui->openWindow(particleWindow);
}

/****************************************************************
 *                          Open Fog Window                     *
 ****************************************************************/
void guiIO::openFogWindow()
{
 if(actualFog != NULL)
 {
   char buf[10];
   fogWindow = gui->insertWindow(185,97,289,253,"Fog");
   fogWindow->getObjectsList()->insertTextBox(5,17,45,30,0,"Red");
   sprintf(buf,"%.4f",actualFog->color[0]);
   fogColor[0] = fogWindow->getObjectsList()->insertTextBar(48,17,98,30,buf,1);
   fogWindow->getObjectsList()->insertTextBox(5,31,45,44,0,"Green");
   sprintf(buf,"%.4f",actualFog->color[1]);
   fogColor[1] = fogWindow->getObjectsList()->insertTextBar(48,31,98,44,buf,1);
   fogWindow->getObjectsList()->insertTextBox(5,45,45,58,0,"Blue");
   sprintf(buf,"%.4f",actualFog->color[2]);
   fogColor[2] = fogWindow->getObjectsList()->insertTextBar(48,45,98,58,buf,1);
   fogWindow->getObjectsList()->insertTextBox(5,59,45,72,0,"Alpha");
   sprintf(buf,"%.4f",actualFog->color[3]);
   fogColor[3] = fogWindow->getObjectsList()->insertTextBar(48,59,98,72,buf,1);
   fogWindow->getObjectsList()->insertTextBox(5,73,45,86,0,"Dense");
   sprintf(buf,"%.4f",actualFog->density);
   fogDensity = fogWindow->getObjectsList()->insertTextBar(48,73,98,86,buf,1);
   fogWindow->getObjectsList()->insertTextBox(5,87,45,100,0,"Start");
   sprintf(buf,"%.4f",actualFog->start);
   fogStart = fogWindow->getObjectsList()->insertTextBar(48,87,98,100,buf,1);
   fogWindow->getObjectsList()->insertTextBox(5,101,45,114,0,"End");
   sprintf(buf,"%.4f",actualFog->end);
   fogEnd = fogWindow->getObjectsList()->insertTextBar(48,101,98,114,buf,1);
   fogWindow->getObjectsList()->insertTextBox(5,115,45,128,0,"Enable");
   fogEnabled = fogWindow->getObjectsList()->insertCxSel(48,117,
                                                         actualFog->enabled);

   fogApplyButton = fogWindow->getObjectsList()->insertButton(24,130,79,148,
                                                              "Apply",1);
   fogWindow->setExternPointer(&fogWindow);
   gui->openWindow(fogWindow);
 }
}

/****************************************************************
 *                            setFog                            *
 ****************************************************************/
void guiIO::setFog(mapFog* fog)
{
   actualFog = fog;
   if(fogWindow)
   {
      char buf[10];
      int i;
      for(i=0; i < 4; i++)
      {
         sprintf(buf,"%.4f",actualFog->color[i]);
         fogColor[i]->setText(buf);
      }
      sprintf(buf,"%.4f",actualFog->density);
      fogDensity->setText(buf);
      sprintf(buf,"%.4f",actualFog->start);
      fogStart->setText(buf);
      sprintf(buf,"%.4f",actualFog->end);
      fogEnd->setText(buf);
      fogEnabled->setSelection(actualFog->enabled);
      fogWindow->draw(0,0);
   }
}

/****************************************************************
 *                          applyFog                            *
 ****************************************************************/
void guiIO::applyFog()
{
   if(actualFog)
   {
      int i;
      for(i=0; i < 4; i++)
      {
         sscanf(fogColor[i]->getText().c_str(),"%f",&actualFog->color[i]);
      }
      sscanf(fogDensity->getText().c_str(),"%f",&actualFog->density);
      sscanf(fogStart->getText().c_str(),"%f",&actualFog->start);
      sscanf(fogEnd->getText().c_str(),"%f",&actualFog->end);
      actualFog->enabled = fogEnabled->isSelected();
      if(actualFog->enabled)
      {
         glEnable(GL_FOG);
         {
            glFogi(GL_FOG_MODE,GL_LINEAR);
            glFogfv(GL_FOG_COLOR,actualFog->color);
            glFogf(GL_FOG_DENSITY,actualFog->density);
            glHint(GL_FOG_HINT,GL_DONT_CARE);
            glFogf(GL_FOG_START,actualFog->start);
            glFogf(GL_FOG_END,actualFog->end);
         }
      }
      else
      {
         glEnable(GL_FOG);
         {
            GLfloat color[3]={1.0,1.0,1.0};
            glFogi(GL_FOG_MODE,GL_LINEAR);
            glFogfv(GL_FOG_COLOR, color);
            glFogf(GL_FOG_DENSITY, 0.10);
            glHint(GL_FOG_HINT, GL_DONT_CARE);
            glFogf(GL_FOG_START, 100);
            glFogf(GL_FOG_END, FARVIEW / 2.0);
         }
      }
   }
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
 *                           setTool                            *
 ****************************************************************/
void guiIO::setTool(int toolID)
{
   tool = toolID;
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
int guiIO::doIO(int mouseX, int mouseY, Uint8 mButton, Uint8 *keys,
                bool outdoor)
{
   int eventInfo=NOTHING;
   guiObject* object;

   /* Get Events */
   object = gui->manipulateEvents(mouseX, mouseY, mButton, keys, eventInfo);


   /* Do Camera IO */
   if(eventInfo != WROTE_TEXT_BAR)
   {
      gameCamera.doIO(keys, mButton, mouseX, mouseY, DELTA_CAMERA );

      if(keys[SDLK_KP8])
      {
         gameCamera.updateCamera(gameCamera.getCenterX() -
                                 4.0 * sin(deg2Rad(gameCamera.getPhi())),
                                 gameCamera.getCenterY()-30,
                                 gameCamera.getCenterZ() - 
                                 4.0 * cos(deg2Rad(gameCamera.getPhi())), 
                                 0.0);
      }
      if(keys[SDLK_KP2])
      {
         gameCamera.updateCamera(gameCamera.getCenterX() +
                                 4.0 * sin(deg2Rad(gameCamera.getPhi())),
                                 gameCamera.getCenterY()-30,
                                 gameCamera.getCenterZ() + 
                                 4.0 * cos(deg2Rad(gameCamera.getPhi())), 
                                 0.0);
      }
      if(keys[SDLK_KP4])
      {
         gameCamera.updateCamera(gameCamera.getCenterX() -
                                 4.0 * sin(deg2Rad(gameCamera.getPhi()) + 
                                       deg2Rad(90)),
                                 gameCamera.getCenterY()-30,
                                 gameCamera.getCenterZ() - 
                                 4.0 * cos(deg2Rad(gameCamera.getPhi()) + 
                                       deg2Rad(90)),
                                 0.0);
      }
      if(keys[SDLK_KP6])
      {
         gameCamera.updateCamera(gameCamera.getCenterX() +
                                 4.0 * sin(deg2Rad(gameCamera.getPhi()) + 
                                        deg2Rad(90)),
                                 gameCamera.getCenterY()-30,
                                 gameCamera.getCenterZ() +
                                 4.0 * cos(deg2Rad(gameCamera.getPhi()) +
                                       deg2Rad(90)),
                                 0.0);
      }
   }


   if(ltWindow->eventGot(eventInfo, object))
   {
      selectedText = ltWindow->getFileName();
      if(!selectedText.empty())
      {
         switch(ltWindow->getState())
         {
            /* Particles */
            case STATE_FIRE:
               state = GUI_IO_STATE_PARTICLES;
               tool = TOOL_PARTICLE_FIRE;
            break;
            case STATE_GRASS:
               state = GUI_IO_STATE_PARTICLES;
               tool = TOOL_PARTICLE_GRASS;
            break;
            case STATE_SMOKE:
               state = GUI_IO_STATE_PARTICLES;
               tool = TOOL_PARTICLE_SMOKE;
            break;
            case STATE_SNOW:
               state = GUI_IO_STATE_PARTICLES;
               tool = TOOL_PARTICLE_SNOW;
            break;
            case STATE_WATERFALL:
               state = GUI_IO_STATE_PARTICLES;
               tool = TOOL_PARTICLE_WATERFALL;
            break;

            /* Doors */
            case STATE_DOORS:
               state = GUI_IO_STATE_PORTAL;
               tool = TOOL_PORTAL_DOOR;
            break;

            /* Objects */
            case STATE_BUILDING:
            case STATE_CARS:
            case STATE_COMMON:
            case STATE_GUNS:
            case STATE_ICEX:
            case STATE_NATURE:
            case STATE_MACABRE:
            case STATE_BATH:
            case STATE_STREET:
               state = GUI_IO_STATE_OBJECTS;
               tool = TOOL_OBSTACLE_ADD;
            break;

            /* Characters */
            case STATE_CHARACTERS:
               state = GUI_IO_STATE_NPCS;
               tool = TOOL_NPC_ADD;
            break;

            /* Music */
            case STATE_MUSIC:
               state = GUI_IO_STATE_MUSIC;
               tool = TOOL_NONE;
            break;

            case STATE_INDOOR_TEXTURE:
            case STATE_OUTDOOR_TEXTURE:
               return(GUI_IO_TEXTURE_INSERT);
            break;
         }
         return(GUI_IO_NEW_STATE);
      }
   }
   else if(grWindow->eventGot(eventInfo, object))
   {
   }
   else if(wtWindow->eventGot(eventInfo, object))
   {
   }

   switch(eventInfo)
   {
      case ON_PRESS_TAB_BUTTON:
      {
         /*  Navigation Buttons  */
         if(object == (guiObject*) upButton)
         {
            gameCamera.updateCamera(gameCamera.getCenterX() -
                                       4.0 * sin(deg2Rad(gameCamera.getPhi())),
                                       gameCamera.getCenterY()-30,
                                       gameCamera.getCenterZ() - 
                                       4.0 * cos(deg2Rad(gameCamera.getPhi())), 
                                       0.0);
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (guiObject*) downButton)
         {
            gameCamera.updateCamera(gameCamera.getCenterX() +
                                       4.0 * sin(deg2Rad(gameCamera.getPhi())),
                                       gameCamera.getCenterY()-30,
                                       gameCamera.getCenterZ() + 
                                       4.0 * cos(deg2Rad(gameCamera.getPhi())), 
                                       0.0);
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (guiObject*) leftButton)
         {
            gameCamera.updateCamera(gameCamera.getCenterX() -
                            4.0 * sin(deg2Rad(gameCamera.getPhi())+deg2Rad(90)),
                                      gameCamera.getCenterY()-30,
                                      gameCamera.getCenterZ() - 
                            4.0 * cos(deg2Rad(gameCamera.getPhi())+deg2Rad(90)),
                                       0.0);
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (guiObject*) rightButton)
         {
            gameCamera.updateCamera(gameCamera.getCenterX() +
                           4.0 * sin(deg2Rad(gameCamera.getPhi())+deg2Rad(90)),
                                     gameCamera.getCenterY()-30,
                                     gameCamera.getCenterZ() +
                           4.0 * cos(deg2Rad(gameCamera.getPhi())+deg2Rad(90)),
                                     0.0);
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (guiObject*) rotUpButton)
         {
            gameCamera.sumTheta(1);
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (guiObject*) rotDownButton)
         {
            gameCamera.sumTheta(-1);
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (guiObject*) rotLeftButton)
         {
            gameCamera.sumPhi(1);
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (guiObject*) rotRightButton)
         {
            gameCamera.sumPhi(-1);
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (guiObject*) moreZoomButton)
         {
            gameCamera.sumD(-1);
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (guiObject*) lessZoomButton)
         {
            gameCamera.sumD(1);
            return(GUI_IO_NEW_POSITION);
         }
      break;
      
      case PRESSED_TAB_BUTTON:

         /*  Terrain Buttons  */
         if(object == (guiObject*) terrainUpButton)
         {
            state = GUI_IO_STATE_TERRAIN;
            tool = TOOL_TERRAIN_UP;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (guiObject*) terrainDownButton)
         {
            state = GUI_IO_STATE_TERRAIN;
            tool = TOOL_TERRAIN_DOWN;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (guiObject*) terrainNivButton)
         {
            state = GUI_IO_STATE_TERRAIN;
            tool = TOOL_TERRAIN_NIVELATE;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (guiObject*) terrainTextureButton)
         {
            state = GUI_IO_STATE_TERRAIN;
            tool = TOOL_TERRAIN_TEXTURE;
            return(GUI_IO_NEW_STATE);
         }
         /* Portal Buttons */
         else if(object == (guiObject*) portalAddButton)
         {
            state = GUI_IO_STATE_PORTAL;
            tool = TOOL_PORTAL_ADD;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (guiObject*) portalTagButton)
         {
            state = GUI_IO_STATE_PORTAL;
            tool = TOOL_PORTAL_TAG;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (guiObject*) portalDoorButton)
         {
            ltWindow->setState(STATE_DOORS);
         }

         /* Wall Buttons */
         else if(object == (guiObject*) wallXButton)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_ADD_X;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (guiObject*) wallZButton)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_ADD_Z;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (guiObject*) wallX2Button)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL2_ADD_X;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (guiObject*) wallZ2Button)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL2_ADD_Z;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (guiObject*) wallTextureButton)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_TEXTURE;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (guiObject*) wallLessYTexture)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_LESS_Y_TEXTURE;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (guiObject*) wallMoreYTexture)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_MORE_Y_TEXTURE;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (guiObject*) wallLessXTexture)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_LESS_X_TEXTURE;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (guiObject*) wallMoreXTexture)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_MORE_X_TEXTURE;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (guiObject*) wallLessZTexture)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_LESS_Z_TEXTURE;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (guiObject*) wallMoreZTexture)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_MORE_Z_TEXTURE;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (guiObject*) wallNextButton)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_NEXT;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (guiObject*) wallPreviousButton)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_PREVIOUS;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (guiObject*) wallDestroyButton)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_DESTROY;
            return(GUI_IO_NEW_STATE);
         }

         /* Objects Buttons */
         else if(object == (guiObject*) objectCommonButton)
         {
            ltWindow->setState(STATE_COMMON);
         }
         else if(object == (guiObject*) objectGunsButton)
         {
            ltWindow->setState(STATE_GUNS);
         }
         else if(object == (guiObject*) objectBuildButton)
         {
            ltWindow->setState(STATE_BUILDING);
         }
         else if(object == (guiObject*) objectCarsButton)
         {
            ltWindow->setState(STATE_CARS);
         }
         else if(object == (guiObject*) objectIcexButton)
         {
            ltWindow->setState(STATE_ICEX);
         }
         else if(object == (guiObject*) objectNaturalButton)
         {
            ltWindow->setState(STATE_NATURE);
         }
         else if(object == (guiObject*) objectCharButton)
         {
            ltWindow->setState(STATE_CHARACTERS);
         }
         else if(object == (guiObject*) objectMacabreButton)
         {
            ltWindow->setState(STATE_MACABRE);
         }
         else if(object == (guiObject*) objectBathButton)
         {
            ltWindow->setState(STATE_BATH);
         }
         else if(object == (guiObject*) objectStreetButton)
         {
            ltWindow->setState(STATE_STREET);
         }
         /* Particles Buttons */
         else if(object == (guiObject*) fireButton)
         {
            ltWindow->setState(STATE_FIRE);
         }
         else if(object == (guiObject*) smokeButton)
         {
            ltWindow->setState(STATE_SMOKE);
         }
         else if(object == (guiObject*) snowButton)
         {
            ltWindow->setState(STATE_SNOW);
         }
         else if(object == (guiObject*) waterfallButton)
         {
            ltWindow->setState(STATE_WATERFALL);
         }
         else if(object == (guiObject*) waterSurfaceButton)
         {
            state = GUI_IO_STATE_PARTICLES;
            tool = TOOL_PARTICLE_LAKE;
         }
         else if(object == (guiObject*) grassButton)
         {
            ltWindow->setState(STATE_GRASS);
         }
         break;
      }

      /* File Selectors Things */
      case FILE_SEL_ACCEPT:
      {
         if(fileWindow)
         {
            if(object == (guiObject*)fileSelector) 
            {
               curFileName = fileSelector->getFileName();
               gui->closeWindow(fileWindow);
               if(fileLoading)
               {
                  return(GUI_IO_OPEN_MAP);
               }
               else
               {
                  return(GUI_IO_SAVE_MAP);
               }
            }
         }
         break;
      }
      case FILE_SEL_CANCEL:
      {
         if(fileWindow)
         {
            if(object == (guiObject*)fileSelector) 
            {
               gui->closeWindow(fileWindow);
            }
            return(GUI_IO_OTHER);
         }
         break;
      }


      /* Buttons */
      case PRESSED_BUTTON:
      {
         if(object == (guiObject*) exitButton)
         {
            return(GUI_IO_EXIT);
         }
         else if(object == (guiObject*) newButton)
         {
            return(GUI_IO_NEW_MAP);
         }
         else if(object == (guiObject*) openButton)
         {
            /* Open File Window for Load */
            openFileWindow(true);
            return(GUI_IO_OTHER);
         }
         else if(object == (guiObject*) saveButton)
         {
            /* Open File Window for Save */
            openFileWindow(false);
            return(GUI_IO_OTHER);
         }
         else if(object == (guiObject*) terrainButton)
         {
            if(!terrainWindow)
            {
               openTerrainWindow();
            }
            return(GUI_IO_OTHER);
         }
         else if(object == (guiObject*) wallButton)
         {
            if(!wallWindow)
            {
               openWallWindow();
            }
            return(GUI_IO_OTHER);
         }
         else if(object == (guiObject*) portalButton)
         {
            if(!portalWindow)
            {
               openPortalWindow();
            }
            return(GUI_IO_OTHER);
         }
         else if(object == (guiObject*) particleButton)
         {
            if(!particleWindow)
            {
               openParticleWindow();
            }
            return(GUI_IO_OTHER);
         }
         else if(object == (guiObject*) objectButton)
         {
            if(!objectWindow)
            {
               openObjectWindow();
            }
            return(GUI_IO_OTHER);
         }
         else if(object == (guiObject*) fogButton)
         {
            if(!fogWindow)
            {
               openFogWindow();
            }
            return(GUI_IO_OTHER);
         }
         else if(object == (guiObject*) npcButton)
         {
            ltWindow->setState(STATE_CHARACTERS);
         }
         else if(object == (guiObject*) musicButton)
         {
            ltWindow->setState(STATE_MUSIC);
         }
         /* Texture Window */
         else if(object == (guiObject*) textureNextButton)
         {
            return(GUI_IO_TEXTURE_NEXT);
         }
         else if(object == (guiObject*) texturePreviousButton)
         {
            return(GUI_IO_TEXTURE_PREVIOUS);
         }
         else if(object == (guiObject*) textureInsertButton)
         {
            if(outdoor)
            {
               ltWindow->setState(STATE_OUTDOOR_TEXTURE);
            }
            else
            {
               ltWindow->setState(STATE_INDOOR_TEXTURE);
            }
         }
         /*Fog Buttons */
         else if(object == (guiObject*) fogApplyButton)
         {
            applyFog();
            return(GUI_IO_NOTHING);
         }
         break;
      }
   }
   if(eventInfo == NOTHING)
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
void guiIO::cameraPos(Map* acMap)
{
   /*cameraX = centerX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centerY + deltaY + (float) d * sin(deg2Rad(theta));
   cameraZ = centerZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
   gluLookAt(cameraX,cameraY,cameraZ, centerX,centerY,centerZ,0,1,0);*/
   gameCamera.lookAt(acMap);
}

/****************************************************************
 *                      getSelectedText                         *
 ****************************************************************/
string guiIO::getSelectedText()
{
   return(selectedText);
}


/****************************************************************
 *                        getFileName                           *
 ****************************************************************/
string guiIO::getFileName()
{
   return(curFileName);
}

/****************************************************************
 *                   getTextureFileName                         *
 ****************************************************************/
string guiIO::getTextureFileName()
{
   return(selectedText);
}

/****************************************************************
 *                    getObjectFileName                         *
 ****************************************************************/
string guiIO::getObjectFileName()
{
   //FIXME
   return(""/*objectText->texto*/);
}

/****************************************************************
 *                        Show Message                          *
 ****************************************************************/
void guiIO::showMessage(string msg)
{
   messageText->setText(msg);
   messageWindow->draw(0,0);
}

