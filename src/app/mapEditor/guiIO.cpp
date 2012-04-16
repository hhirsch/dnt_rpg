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

#include "guiIO.h"
#include "../../engine/util.h"
#include "../../engine/cursor.h"
#include "../../gui/farsoopts.h"

using namespace std;

/****************************************************************
 *                         Constructor                          *
 ****************************************************************/
guiIO::guiIO()
{
   gui = new Farso::GuiInterface("");

   state = GUI_IO_STATE_INITIAL;
   tool = TOOL_NONE;

   selectedText = "";
   curFileName = "";

   fogWindow = NULL;
   actualFog = NULL;
   fileWindow = NULL;

   invertMultiTexture = false;

   /* Change the minimun Zoom */
   gameCamera.defineMinZoom(2*ZOOM_MIN);

   /* Open Windows */
   grWindow = new grassWindow(gui);
   wtWindow = new waterWindow(gui);
   ltWindow = new listWindow(gui);
   ltWindow->setState(0);
   openActWindow();
   openNavWindow();
   openWallWindow();
   openTileWallWindow();
   openTerrainWindow();
   openParticleWindow();
   openPortalWindow();
   openTextureWindow();
   openMessageWindow();
   openObjectWindow();
   openMainWindow();

   nodeEdit = new nodeEditor(gui);
}

/****************************************************************
 *                          Destructor                          *
 ****************************************************************/
guiIO::~guiIO()
{
   delete(gui);
   delete(nodeEdit);
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
   if(fileWindow)
   {
       /* Close the current opened */
       gui->closeWindow(fileWindow);
   }

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
   objectCommonButton->setMouseHint("Common");
   objectGunsButton = objectTabButton->insertButton(25,0,44,19);
   objectGunsButton->setMouseHint("Weapons");
   objectBuildButton = objectTabButton->insertButton(49,0,71,19);
   objectBuildButton->setMouseHint("Indoor");
   objectCarsButton = objectTabButton->insertButton(74,0,96,19);
   objectCarsButton->setMouseHint("Vehicles");
   objectIcexButton = objectTabButton->insertButton(99,0,120,19);
   objectIcexButton->setMouseHint("School-like");
   objectNaturalButton = objectTabButton->insertButton(125,0,145,19);
   objectNaturalButton->setMouseHint("Nature");
   objectCharButton = objectTabButton->insertButton(147,0,170,19);
   objectCharButton->setMouseHint("Characters");
   objectMacabreButton = objectTabButton->insertButton(0,25,24,45);
   objectMacabreButton->setMouseHint("Macabre");
   objectBathButton = objectTabButton->insertButton(25,25,44,45);
   objectBathButton->setMouseHint("Bathroom");
   objectStreetButton = objectTabButton->insertButton(49,25,71,45);
   objectStreetButton->setMouseHint("Street");
   objectJunkButton = objectTabButton->insertButton(74,25,96,45);
   objectJunkButton->setMouseHint("Junk");
   objectSelectButton = objectTabButton->insertButton(0, 50, 24, 70);
   objectSelectButton->setMouseHint("Select object to edit");
   
   objectWindow->setExternPointer(&objectWindow);
   gui->openWindow(objectWindow);
}

/****************************************************************
 *                       Open Nav Window                        *
 ****************************************************************/
void guiIO::openNavWindow()
{
   dirs dir;
   navWindow = gui->insertWindow(Farso::SCREEN_X-69, Farso::SCREEN_Y-76,
         Farso::SCREEN_X-1,Farso::SCREEN_Y-1,"Nav", true);
   Farso::Picture* pic = navWindow->getObjectsList()->insertPicture(7,17,0,0,
         dir.getRealFile("mapEditor/nav.png").c_str());
   navTabButton = navWindow->getObjectsList()->insertTabButton(7,17,53,50,NULL);
   navTabButton->setObjectBelow(pic);
   navTabButton->setStyle(Farso::TabButton::STYLE_HIGH);
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
   navWindow->setExternPointer(&navWindow);
   gui->openWindow(navWindow);
}

/****************************************************************
 *                       Open Message Window                    *
 ****************************************************************/
void guiIO::openMessageWindow()
{
   Farso::Options fopt;
   int width = Farso::SCREEN_X - 50 - 437;
   messageWindow = gui->insertWindow(437,0,Farso::SCREEN_X-50,37, "Messages");
   messageText = messageWindow->getObjectsList()->insertTextBox(7,16,
                 width-90,31,0,"Welcome to DccNiTghtmare Map Editor!");
   mouseCoordText = messageWindow->getObjectsList()->insertTextBox(width-90,16,
         width-7, 31, 0, "");
   mouseCoordText->setFont(fopt.getDefaultFont(), 9, Farso::Font::ALIGN_CENTER);
   messageWindow->setAttributes(false,true,false,false);
   messageWindow->setExternPointer(&messageWindow);
   gui->openWindow(messageWindow);
}

/****************************************************************
 *                    updateMouseWorldCoordinates               *
 ****************************************************************/
void guiIO::updateMouseWorldCoordinates(float mX, float mZ)
{
   char buf[64];

   sprintf(buf, "%.2f,%.2f", mX, mZ);
   mouseCoordText->setText(buf);
   messageWindow->draw(0,0);
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
   wallAddButton = wallTabButton->insertButton(0,0,19,19);       /* Wall Add */
   wallAddButton->setMouseHint("Create Wall");
   wallEditButton = wallTabButton->insertButton(20,0,39,19);     /* Wall Edit */
   wallEditButton->setMouseHint("Edit Wall");
   wallCutButton = wallTabButton->insertButton(40,0,59,19);      /* Wall Cut */
   wallCutButton->setMouseHint("Cut Wall");
   wallDestroyButton = wallTabButton->insertButton(100,0,119,19); /* Destroy */
   wallDestroyButton->setMouseHint("Destroy Wall");
   wallTextureButton = wallTabButton->insertButton(0,20,19,39);   /* Texture */
   wallTextureButton->setMouseHint("Set Wall Texture");
   wallLessXTexture = wallTabButton->insertButton(20,20,39,39);   /* Less X */
   wallLessXTexture->setMouseHint("Decrease Wall X Texture Repeat");
   wallMoreXTexture = wallTabButton->insertButton(40,20,59,39);   /* More X */
   wallMoreXTexture->setMouseHint("Increase Wall Z Texture Repeat");
   wallLessYTexture = wallTabButton->insertButton(60,20,79,39);   /* Less Y */
   wallLessYTexture->setMouseHint("Decrease Wall Y Texture Repeat");
   wallMoreYTexture = wallTabButton->insertButton(80,20,99,39);   /* Less Y */
   wallMoreYTexture->setMouseHint("Increase Wall Y Texture Repeat");
   wallLessZTexture = wallTabButton->insertButton(100,20,119,39);   /* Less Z */
   wallLessZTexture->setMouseHint("Decrease Wall Z Texture Repeat");
   wallMoreZTexture = wallTabButton->insertButton(120,20,139,39);   /* Less Z */
   wallMoreZTexture->setMouseHint("Increase Wall Z Texture Repeat");

   wallWindow->setExternPointer(&wallWindow);
   gui->openWindow(wallWindow);
}

/****************************************************************
 *                     Open Tile Wall Window                    *
 ****************************************************************/
void guiIO::openTileWallWindow()
{
   dirs dir;
   tileWallWindow = gui->insertWindow(0,599,153,661,"TileWall");
   tileWallTabButton = tileWallWindow->getObjectsList()->insertTabButton(
         7,17,0,0, dir.getRealFile("mapEditor/tilewall.png").c_str());
   tileWallAddButton = tileWallTabButton->insertButton(0,0,19,19);
   tileWallAddButton->setMouseHint("Add TileWall");
   tileWallRemoveButton = tileWallTabButton->insertButton(20,0,39,19);
   tileWallRemoveButton->setMouseHint("Remove TileWall");

   tileWallWindow->setExternPointer(&tileWallWindow);
   gui->openWindow(tileWallWindow);
}


/****************************************************************
 *                      Open Sound Window                       *
 ****************************************************************/
void guiIO::openSoundWindow()
{
   dirs dir;
   soundWindow = gui->insertWindow(0,599,153,661,"Sound");
   soundTabButton = soundWindow->getObjectsList()->insertTabButton(7,17,0,0,
                                dir.getRealFile("mapEditor/sound.png").c_str());
   soundAddButton = soundTabButton->insertButton(0,0,19,19);    /* Sound Add */
   soundAddButton->setMouseHint("Add sound");
   soundRemoveButton = soundTabButton->insertButton(20,0,39,19);/* Sound Rm */
   soundRemoveButton->setMouseHint("Remove sound");
   soundEditButton = soundTabButton->insertButton(40,0,59,19);  /* Sound Edit */
   soundEditButton->setMouseHint("Edit sound info");

   soundWindow->setExternPointer(&soundWindow);
   gui->openWindow(soundWindow);
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
   portalAddButton->setMouseHint("Add Portal");
   portalTagButton = portalTabButton->insertButton(20,0,39,19); /* Tag */
   portalTagButton->setMouseHint("Set Portal Destination");
   portalDoorButton = portalTabButton->insertButton(40,0,59,19); /* Door */
   portalDoorButton->setMouseHint("Add a door to an wall");
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
   terrainUpButton->setMouseHint("Up Terrain");
   terrainNivButton = terrainTabButton->insertButton(20,0,39,19);  
   terrainNivButton->setMouseHint("Nivelate Terrain");
   terrainDownButton = terrainTabButton->insertButton(40,0,59,19); 
   terrainDownButton->setMouseHint("Down Terrain");
   terrainTextureButton = terrainTabButton->insertButton(20,20,39,39);
   terrainTextureButton->setMouseHint("Set Terrain Texture");
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
   fireButton->setMouseHint("Fire");
   smokeButton = particleTabButton->insertButton(20,0,39,19); 
   smokeButton->setMouseHint("Smoke");
   waterfallButton = particleTabButton->insertButton(40,0,59,19);
   waterfallButton->setMouseHint("Waterfall");
   waterSurfaceButton = particleTabButton->insertButton(60,0,79,19);
   waterSurfaceButton->setMouseHint("Water Surface");
   snowButton = particleTabButton->insertButton(80,0,99,19);
   snowButton->setMouseHint("Snow");
   grassButton = particleTabButton->insertButton(0,20,19,39);
   grassButton->setMouseHint("Grass");
   
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
   char buf[32];
   fogWindow = gui->insertWindow(185,212,289,368,"Fog");
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
      actualFog->apply(true, OUTDOOR_FARVIEW, INDOOR_FARVIEW);
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
 *                          clearState                          *
 ****************************************************************/
void guiIO::clearState()
{
   state = GUI_IO_STATE_INITIAL;
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
 *                     getInvertMultiTexture                    *
 ****************************************************************/
bool guiIO::getInvertMultiTexture()
{
   return(invertMultiTexture);
}

/****************************************************************
 *                             DoIO                             *
 ****************************************************************/
int guiIO::doIO(int mouseX, int mouseY, Uint8 mButton, Uint8 *keys,
                bool outdoor)
{
   int eventInfo = Farso::EVENT_NONE;
   float qty = 4.0f;
   cursor curs;
   Farso::GuiObject* object;

   /* Get Events */
   object = gui->manipulateEvents(mouseX, mouseY, mButton, keys, eventInfo);
   
   /* Multitexture toggle */
   invertMultiTexture = (keys[SDLK_i] != 0);


   /* Do Camera IO */
   if(eventInfo != Farso::EVENT_WROTE_TEXT_BAR)
   {
      gameCamera.doIO(keys, mButton, mouseX, mouseY, DELTA_CAMERA );

      if( (keys[SDLK_RSHIFT]) || (keys[SDLK_LSHIFT]) )
      {
         qty = 16;
      }

      if(keys[SDLK_KP8])
      {
         gameCamera.updateCamera(gameCamera.getCenterX() -
                                 qty * sin(deg2Rad(gameCamera.getPhi())),
                                 gameCamera.getCenterY()-30,
                                 gameCamera.getCenterZ() - 
                                 qty * cos(deg2Rad(gameCamera.getPhi())), 
                                 0.0);
      }
      if(keys[SDLK_KP2])
      {
         gameCamera.updateCamera(gameCamera.getCenterX() +
                                 qty * sin(deg2Rad(gameCamera.getPhi())),
                                 gameCamera.getCenterY()-30,
                                 gameCamera.getCenterZ() + 
                                 qty * cos(deg2Rad(gameCamera.getPhi())), 
                                 0.0);
      }
      if(keys[SDLK_KP4])
      {
         gameCamera.updateCamera(gameCamera.getCenterX() -
                                 qty * sin(deg2Rad(gameCamera.getPhi()) + 
                                       deg2Rad(90)),
                                 gameCamera.getCenterY()-30,
                                 gameCamera.getCenterZ() - 
                                 qty * cos(deg2Rad(gameCamera.getPhi()) + 
                                       deg2Rad(90)),
                                 0.0);
      }
      if(keys[SDLK_KP6])
      {
         gameCamera.updateCamera(gameCamera.getCenterX() +
                                 qty * sin(deg2Rad(gameCamera.getPhi()) + 
                                        deg2Rad(90)),
                                 gameCamera.getCenterY()-30,
                                 gameCamera.getCenterZ() +
                                 qty * cos(deg2Rad(gameCamera.getPhi()) +
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
            case STATE_SMOKE:
            case STATE_SNOW:
            case STATE_WATERFALL:
               state = GUI_IO_STATE_PARTICLES;
               tool = TOOL_PARTICLE_DEFAULT;
            break;
            case STATE_GRASS:
               state = GUI_IO_STATE_PARTICLES;
               tool = TOOL_PARTICLE_GRASS;
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
            case STATE_JUNK:
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
   /*else if(grWindow->eventGot(eventInfo, object))
   {
   }*/
   else if(wtWindow->eventGot(eventInfo, object))
   {
   }
   else if(nodeEdit->eventGot(eventInfo, object))
   {
   }

   switch(eventInfo)
   {
      case Farso::EVENT_ON_PRESS_TAB_BUTTON:
      {
         /*  Navigation Buttons  */
         if(object == (Farso::GuiObject*) upButton)
         {
            gameCamera.updateCamera(gameCamera.getCenterX() -
                                       qty * sin(deg2Rad(gameCamera.getPhi())),
                                       gameCamera.getCenterY()-30,
                                       gameCamera.getCenterZ() - 
                                       qty * cos(deg2Rad(gameCamera.getPhi())), 
                                       0.0);
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Farso::GuiObject*) downButton)
         {
            gameCamera.updateCamera(gameCamera.getCenterX() +
                                       qty * sin(deg2Rad(gameCamera.getPhi())),
                                       gameCamera.getCenterY()-30,
                                       gameCamera.getCenterZ() + 
                                       qty * cos(deg2Rad(gameCamera.getPhi())), 
                                       0.0);
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Farso::GuiObject*) leftButton)
         {
            gameCamera.updateCamera(gameCamera.getCenterX() -
                            qty * sin(deg2Rad(gameCamera.getPhi())+deg2Rad(90)),
                                      gameCamera.getCenterY()-30,
                                      gameCamera.getCenterZ() - 
                            qty * cos(deg2Rad(gameCamera.getPhi())+deg2Rad(90)),
                                       0.0);
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Farso::GuiObject*) rightButton)
         {
            gameCamera.updateCamera(gameCamera.getCenterX() +
                           qty * sin(deg2Rad(gameCamera.getPhi())+deg2Rad(90)),
                                     gameCamera.getCenterY()-30,
                                     gameCamera.getCenterZ() +
                           qty * cos(deg2Rad(gameCamera.getPhi())+deg2Rad(90)),
                                     0.0);
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Farso::GuiObject*) rotUpButton)
         {
            gameCamera.sumTheta(1);
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Farso::GuiObject*) rotDownButton)
         {
            gameCamera.sumTheta(-1);
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Farso::GuiObject*) rotLeftButton)
         {
            gameCamera.sumPhi(1);
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Farso::GuiObject*) rotRightButton)
         {
            gameCamera.sumPhi(-1);
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Farso::GuiObject*) moreZoomButton)
         {
            gameCamera.sumD(-1);
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Farso::GuiObject*) lessZoomButton)
         {
            gameCamera.sumD(1);
            return(GUI_IO_NEW_POSITION);
         }
      break;
      
      case Farso::EVENT_PRESSED_TAB_BUTTON:

         /*  Terrain Buttons  */
         if(object == (Farso::GuiObject*) terrainUpButton)
         {
            state = GUI_IO_STATE_TERRAIN;
            tool = TOOL_TERRAIN_UP;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Farso::GuiObject*) terrainDownButton)
         {
            state = GUI_IO_STATE_TERRAIN;
            tool = TOOL_TERRAIN_DOWN;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Farso::GuiObject*) terrainNivButton)
         {
            state = GUI_IO_STATE_TERRAIN;
            tool = TOOL_TERRAIN_NIVELATE;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Farso::GuiObject*) terrainTextureButton)
         {
            state = GUI_IO_STATE_TERRAIN;
            tool = TOOL_TERRAIN_TEXTURE;
            return(GUI_IO_NEW_STATE);
         }
         /* Portal Buttons */
         else if(object == (Farso::GuiObject*) portalAddButton)
         {
            state = GUI_IO_STATE_PORTAL;
            tool = TOOL_PORTAL_ADD;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Farso::GuiObject*) portalTagButton)
         {
            state = GUI_IO_STATE_PORTAL;
            tool = TOOL_PORTAL_TAG;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Farso::GuiObject*) portalDoorButton)
         {
            ltWindow->setState(STATE_DOORS);
         }

         else if(object == (Farso::GuiObject*) tileWallAddButton)
         {
            state = GUI_IO_STATE_TILE_WALL;
            tool = TOOL_TILE_WALL_ADD;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Farso::GuiObject*) tileWallRemoveButton)
         {
            state = GUI_IO_STATE_TILE_WALL;
            tool = TOOL_TILE_WALL_REMOVE;
            return(GUI_IO_NEW_STATE);
         }

         /* Wall Buttons */
         else if(object == (Farso::GuiObject*) wallAddButton)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_ADD;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Farso::GuiObject*) wallEditButton)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_EDIT;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Farso::GuiObject*) wallCutButton)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_CUT;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Farso::GuiObject*) wallTextureButton)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_TEXTURE;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Farso::GuiObject*) wallLessYTexture)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_LESS_Y_TEXTURE;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Farso::GuiObject*) wallMoreYTexture)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_MORE_Y_TEXTURE;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Farso::GuiObject*) wallLessXTexture)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_LESS_X_TEXTURE;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Farso::GuiObject*) wallMoreXTexture)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_MORE_X_TEXTURE;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Farso::GuiObject*) wallLessZTexture)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_LESS_Z_TEXTURE;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Farso::GuiObject*) wallMoreZTexture)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_MORE_Z_TEXTURE;
            return(GUI_IO_NEW_STATE);
         }
         else if(object == (Farso::GuiObject*) wallDestroyButton)
         {
            state = GUI_IO_STATE_WALL;
            tool = TOOL_WALL_DESTROY;
            return(GUI_IO_NEW_STATE);
         }

         /* Objects Buttons */
         else if(object == (Farso::GuiObject*) objectCommonButton)
         {
            ltWindow->setState(STATE_COMMON);
         }
         else if(object == (Farso::GuiObject*) objectGunsButton)
         {
            ltWindow->setState(STATE_GUNS);
         }
         else if(object == (Farso::GuiObject*) objectBuildButton)
         {
            ltWindow->setState(STATE_BUILDING);
         }
         else if(object == (Farso::GuiObject*) objectCarsButton)
         {
            ltWindow->setState(STATE_CARS);
         }
         else if(object == (Farso::GuiObject*) objectIcexButton)
         {
            ltWindow->setState(STATE_ICEX);
         }
         else if(object == (Farso::GuiObject*) objectNaturalButton)
         {
            ltWindow->setState(STATE_NATURE);
         }
         else if(object == (Farso::GuiObject*) objectCharButton)
         {
            ltWindow->setState(STATE_CHARACTERS);
         }
         else if(object == (Farso::GuiObject*) objectMacabreButton)
         {
            ltWindow->setState(STATE_MACABRE);
         }
         else if(object == (Farso::GuiObject*) objectBathButton)
         {
            ltWindow->setState(STATE_BATH);
         }
         else if(object == (Farso::GuiObject*) objectStreetButton)
         {
            ltWindow->setState(STATE_STREET);
         }
         else if(object == (Farso::GuiObject*) objectJunkButton)
         {
            ltWindow->setState(STATE_JUNK);
         }
         else if(object == (Farso::GuiObject*) objectSelectButton)
         {
            state = GUI_IO_STATE_OBJECTS;
            tool = TOOL_NODE_EDITOR;
         }
         /* Particles Buttons */
         else if(object == (Farso::GuiObject*) fireButton)
         {
            ltWindow->setState(STATE_FIRE);
         }
         else if(object == (Farso::GuiObject*) smokeButton)
         {
            ltWindow->setState(STATE_SMOKE);
         }
         else if(object == (Farso::GuiObject*) snowButton)
         {
            ltWindow->setState(STATE_SNOW);
         }
         else if(object == (Farso::GuiObject*) waterfallButton)
         {
            ltWindow->setState(STATE_WATERFALL);
         }
         else if(object == (Farso::GuiObject*) waterSurfaceButton)
         {
            state = GUI_IO_STATE_PARTICLES;
            tool = TOOL_PARTICLE_LAKE;
         }
         else if(object == (Farso::GuiObject*) grassButton)
         {
            ltWindow->setState(STATE_GRASS);
         }
         else if(object == (Farso::GuiObject*) soundAddButton)
         {
         }
         else if(object == (Farso::GuiObject*) soundRemoveButton)
         {
         }
         else if(object == (Farso::GuiObject*) soundEditButton)
         {
         }
         break;
      }

      /* File Selectors Things */
      case Farso::EVENT_FILE_SEL_ACCEPT:
      {
         if(fileWindow)
         {
            if(object == (Farso::GuiObject*)fileSelector) 
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
      case Farso::EVENT_FILE_SEL_CANCEL:
      {
         if(fileWindow)
         {
            if(object == (Farso::GuiObject*)fileSelector) 
            {
               gui->closeWindow(fileWindow);
            }
            return(GUI_IO_OTHER);
         }
         break;
      }


      /* Buttons */
      case Farso::EVENT_PRESSED_BUTTON:
      {
         if(object == (Farso::GuiObject*) exitButton)
         {
            return(GUI_IO_EXIT);
         }
         else if(object == (Farso::GuiObject*) newButton)
         {
            return(GUI_IO_NEW_MAP);
         }
         else if(object == (Farso::GuiObject*) openButton)
         {
            /* Open File Window for Load */
            openFileWindow(true);
            return(GUI_IO_OTHER);
         }
         else if(object == (Farso::GuiObject*) saveButton)
         {
            /* Open File Window for Save */
            openFileWindow(false);
            return(GUI_IO_OTHER);
         }
         else if(object == (Farso::GuiObject*) terrainButton)
         {
            if(!terrainWindow)
            {
               openTerrainWindow();
            }
            return(GUI_IO_OTHER);
         }
         else if(object == (Farso::GuiObject*) wallButton)
         {
            if(!wallWindow)
            {
               openWallWindow();
            }
            if(!tileWallWindow)
            {
               openTileWallWindow();
            }
            return(GUI_IO_OTHER);
         }
         else if(object == (Farso::GuiObject*) portalButton)
         {
            if(!portalWindow)
            {
               openPortalWindow();
            }
            return(GUI_IO_OTHER);
         }
         else if(object == (Farso::GuiObject*) particleButton)
         {
            if(!particleWindow)
            {
               openParticleWindow();
            }
            return(GUI_IO_OTHER);
         }
         else if(object == (Farso::GuiObject*) objectButton)
         {
            if(!objectWindow)
            {
               openObjectWindow();
            }
            return(GUI_IO_OTHER);
         }
         else if(object == (Farso::GuiObject*) fogButton)
         {
            if(!fogWindow)
            {
               openFogWindow();
            }
            return(GUI_IO_OTHER);
         }
         else if(object == (Farso::GuiObject*) npcButton)
         {
            ltWindow->setState(STATE_CHARACTERS);
         }
         else if(object == (Farso::GuiObject*) musicButton)
         {
            ltWindow->setState(STATE_MUSIC);
         }
         /* Texture Window */
         else if(object == (Farso::GuiObject*) textureNextButton)
         {
            return(GUI_IO_TEXTURE_NEXT);
         }
         else if(object == (Farso::GuiObject*) texturePreviousButton)
         {
            return(GUI_IO_TEXTURE_PREVIOUS);
         }
         else if(object == (Farso::GuiObject*) textureInsertButton)
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
         else if(object == (Farso::GuiObject*) fogApplyButton)
         {
            applyFog();
            return(GUI_IO_NOTHING);
         }
         break;
      }
   }
   if(eventInfo == Farso::EVENT_NONE)
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

