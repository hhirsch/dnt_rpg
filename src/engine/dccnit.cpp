/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "dccnit.h"
#include "culling.h"
#include "util.h"
#include "savefile.h"
#include "savewindow.h"

#include <math.h>
#include <SDL/SDL_image.h>
#include "../etc/extensions.h"
#include "../etc/userinfo.h"

/*********************************************************************
 *                       Engine's Constructor                        *
 *********************************************************************/
engine::engine()
{
   int i;

   effect = NULL;
   /* Initialize internal lists */
   gui  = new guiInterface(NULL);
   actualMap = NULL;
   PCs = NULL;
   NPCs = NULL;
   activeCharacter = NULL;
   frontSurface = NULL;

   inventoryWindow = NULL;

   walkPressTime = 0;
   walkDistance = 0;

   imgNumber = 0;
   actualScreen = NULL;

   curConection = NULL;

   walkStatus = ENGINE_WALK_KEYS;

   /* Initialize the Cursor */
   cursors = new(cursor);
   cursors->init();

   /* Initialize sounds and musics */
   snd = new(sound);
   snd->init();
   walkSound = NULL;

   /* Alloc the visible Matrix */
   visibleMatrix = new GLfloat*[6];
   for(i = 0; i < 6; i++)
   {
      visibleMatrix[i] = new GLfloat[4];
   }

   /* Create Options */
   option = new options();

   /* Set sound and music volume, based on options */
   snd->changeVolume(option->getMusicVolume(), option->getSndFxVolume());

   /* Set Camera, based on options */
   gameCamera.defineCameraType(option->getCameraNumber());

   /* Set Language, base on options */
   option->setLanguage();

   /* Define all OpenGL Extensions used */
   extensions ext;
   ext.defineAllExtensions();

   /* Load Skills List */
   skillsList.init();
  
   /* Load Features List */
   features = new featsList(dir.getRealFile("feats/"),
                            dir.getRealFile("feats/feats.ftl"));

   /* Load Alignments & Tendecies */
   alignList = new aligns();
   alignList->init();

   /* Load Races */
   raceList = new races();
   raceList->init();

   /* Load Classes */
   classList = new classes();
   classList->init();

   /* Load Weapons Types */
   weaponsTypes = new weaponTypes();

   /* Create 3D Models List */
   models = new modelList();

   /* Create Special Windows */
   infoWindow = new itemWindow(gui);
   charInfoWindow = new charWindow(gui);
   mapWindow = new miniMapWindow();
   shortcuts = new shortcutsWindow();

   /* Initialize Briefing */
   brief = new briefing();

   /* Initialize readModes variables */
   lastRead = SDL_GetTicks();
   lastMouse = lastRead;
   lastKeyb = lastRead;
   mouseX = 0;
   mouseY = 0;

   showRange = false;

   /* Create the particle System */
   particleController.init();

   /* Create the message controller */
   msgController = new messageController();
   msgController->init();

   /* Create the fight system */
   fight = new fightSystem();

   /* Create the action Controller */
   actionControl = new pendingActionController();

   /* Create the missions controller */
   missions = new missionsController();
   missions->init(this);

   hour = 9.0;
   float sunPos = (OUTDOOR_FARVIEW * option->getFarViewFactor()) / 2.0;
   gameSun = new sun(hour, sunPos, sunPos);

   engineMode = ENGINE_MODE_REAL_TIME;

   destinyVariation = -2.0;
   lastKey = 0;

   /* Colors */
   for(i = 0; i < 3; i++)
   {
      defaultColor[i] = 0.2;
      blackColor[i] = 0.0;
   }
   defaultColor[3] = 1.0;
   blackColor[3] = 1.0;

}

/*********************************************************************
 *                         Engine Destructor                         *
 *********************************************************************/
engine::~engine()
{
   int i;

   /* Stops and free music & sounds */
   if(snd)
   {
      snd->finish();
      delete(snd);
   }

   /* Close the static windows, if needed */
   barterWindow tradeWindow;
   dialogWindow dlgWindow;
   tradeWindow.close();
   dlgWindow.close();


   /* Finish particles */
   particleController.finish();

   /* Finish the current front surface, if one */
   if(frontSurface)
   {
      SDL_FreeSurface(frontSurface);
   }

   /* Close option */
   delete(option);

   /* Clear Sky */
   delete(gameSky);

   /* Delete message Controller */
   msgController->finish();
   delete(msgController);

   /* Delete missions controller */
   missions->finish();
   delete(missions);

   /* Delete fight system */
   delete(fight);

   /* Bye briefing! */
   delete(brief);

   /* Clear Other Textures */
   glDeleteTextures(1, &normalMoveCircle);
   glDeleteTextures(1, &fullMoveCircle);
   glDeleteTextures(1, &destinyImage);
   glDeleteTextures(1, &rangeCircle);
   glDeleteTextures(1, &featRangeCircle);

   /* Clear ModState */
   modifState.clear();

   /* Clear Characters */
   if(NPCs)
   {
      delete(NPCs);
   }
   if(PCs)
   {
      delete(PCs);
   }

   /* Clear Sun */
   if(gameSun)
   {
      delete(gameSun);
   }

   

   /* Clear the Inventory too */
   if(inventoryWindow)
   {
      delete(inventoryWindow);
   }

   /* Clear InfoWindow */
   if(infoWindow)
   {
      delete(infoWindow);
   }

   /* Clear CharInfoWindow */
   if(charInfoWindow)
   {
      delete(charInfoWindow);
   }

   /* Clear MiniMap Window */
   if(mapWindow)
   {
      delete(mapWindow);
   }

   /* Clear ShortcutsWindow */
   if(shortcuts)
   {
      delete(shortcuts);
   }

   /* Clear GUI */
   if(gui)
   {
      delete(gui);
   }

   /* Clear Maps */
   if(actualMap)
   {
      delete(actualMap);
   }
 
   /* Clear Cursors */
   cursors->finish();
   delete(cursors);

   /* Delete the action controller */
   delete(actionControl);

   /* Removel all objects in the list */
   objectsList::removeAll();

   /* Clear 3D Models List */
   delete(models);
   
   /* Clear Characters Lists */
   alignList->finish();
   delete(alignList);
   delete(features);

   raceList->finish();
   delete(raceList);
 
   classList->finish();
   delete(classList);
   skillsList.finish();
   delete(weaponsTypes);

   /* Clear the visibleMatrix */
   for(i = 0; i < 6; i++)
   {
      delete[] visibleMatrix[i];
   }
   delete[] visibleMatrix;

}

/*********************************************************************
 *                               loadGame                            *
 *********************************************************************/
bool engine::loadGame()
{
   bool res = false;

   /* create and run the load/save window */
   saveWindow *savWindow = new saveWindow();
   if(savWindow->run(true, proj, modl, viewPort) == DNT_SAVE_WINDOW_CONFIRM)
   {
      saveFile *sav = new saveFile();

      if(sav->loadHeader(savWindow->getSelectedFileName()))
      {
         sav->load((void*)this);

         /* make sure not in battle mode */
         engineMode = ENGINE_MODE_REAL_TIME;

         /* Put the camera at the characters position */
         activeCharacter = PCs->getActiveCharacter();
         if(activeCharacter)
         {
            gameCamera.updateCamera(activeCharacter->xPosition,
                  activeCharacter->yPosition,
                  activeCharacter->zPosition,
                  activeCharacter->orientation);
         }
         res = true;
      }
      else
      {
         warning warn;
         warn.show(gettext("Error"),gettext("Can't load the saved game!"),gui);
         res = false;
      }

      delete(sav);
   }
   
   delete(savWindow);
   return(res);
}

/*********************************************************************
 *                         defineFrontSurface                        *
 *********************************************************************/
void engine::defineFrontSurface()
{
   /* Delete Previous, if One exists */
   if(frontSurface)
   {
      SDL_FreeSurface(frontSurface);
   }

   /* Create with the buffer */
   frontSurface = readFrontBuffer();
}

/*********************************************************************
 *                               saveGame                            *
 *********************************************************************/
void engine::saveGame()
{
   warning warn;

   if(engineMode != ENGINE_MODE_TURN_BATTLE)
   {
      /* create and run the load/save window */
      saveWindow *savWindow = new saveWindow();
      if(savWindow->run(false, proj, modl, viewPort) == DNT_SAVE_WINDOW_CONFIRM)
      {
         saveFile *sav = new saveFile();

         /* Do the Save */
         if(sav->save(savWindow->getSelectedFileName(), (void*)this, 
                      frontSurface))
         {
            warn.show(gettext("Information"), 
                      gettext("The game was saved!"), gui);
         }
         else
         {
            warn.show(gettext("Error"), gettext("Can't save game!"), gui);
         }

         delete(sav);
      }
      delete(savWindow);
   }
   else
   {
      warn.show(gettext("Warning"), 
                gettext("Can't save game while fighting!"), gui);
   }
}

/*********************************************************************
 *                                 loadPC                            *
 *********************************************************************/
bool engine::loadPC(string pcFile)
{
   if(PCs)
   {
      delete(PCs);
   }
   PCs  = new (characterList);
   activeCharacter = PCs->insertCharacter(pcFile, features, this, "PC");
   return(activeCharacter != NULL);
}

/*********************************************************************
 *                       keepNPCInventoryStatus                      *
 *********************************************************************/
void engine::keepNPCInventoryStatus()
{
   if( (NPCs) && (actualMap))
   {
      /* Save All Needed Inventory Status */
      character* dude = NPCs->getFirst();
      int i;
      for(i = 0; i < NPCs->getTotal(); i++)
      {
         if(dude->inventories->getTotalItems() > 0)
         {
            modifState.mapInventoryAdd(dude->inventories, 
                  dude->getCharacterFile(),
                  actualMap->getFileName());
         }
         dude = (character*)dude->next;
      }
   }
}

/*********************************************************************
 *                         Load Map to Engine                        *
 *********************************************************************/
int engine::loadMap(string arqMapa)
{
   healthBar* progress = NULL; /* the progress bar */

   dntFont fnt;                   /* DNT font controller */
   int centerY = SCREEN_Y / 2;   /* Window Y middle */
   int centerX = SCREEN_X / 2;   /* Window X middle */

   int midW=128, /* Middle Width of load image */
       midH=256; /* Middle Height of load image */
   GLuint texturaCarga; /* Load texture */
   SDL_Surface* fig;    /* Load image */
   
   SDL_Surface* img;    /* Text surface*/
   GLuint texturaTexto; /* Text texture */

   char texto[512];
   string arqVelho = "nada";
   
   /* Set the some initial map-engine status */
   curConection = NULL;
   walkStatus = ENGINE_WALK_KEYS;
   
   /* Generate Loading image Texture */
   glGenTextures(1,&texturaCarga);
   
   /* Clear things */
   glClearColor(0,0,0,1);
   glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
   glDisable(GL_LIGHTING);

   /* Load the loading image */
   fig = IMG_Load(dir.getRealFile("texturas/general/carregar.png").c_str());
   if(!fig)
   {
      cerr << "Faled to load 'loading texture'!" << endl;
      /* create default size progress bar */
      progress = new healthBar(2,20,253,30);
      progress->defineMaxHealth(10);
      progress->defineActualHealth(0);
   }
   else
   {
      /* Set halfs */
      midW = fig->w / 2;
      midH = fig->h / 2;
      
      /* create the progress bar */
      progress = new healthBar(2,20,fig->w-3,30);
      progress->defineMaxHealth(11);
      progress->defineActualHealth(0);
      
      /* Load the texture to opengl */
      setTexture(fig,texturaCarga);
      SDL_FreeSurface(fig);
   }

   /* Create the text surface/texture */
   img = SDL_CreateRGBSurface(SDL_SWSURFACE, 2*midW,32,32,
                              0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   color_Set(0,0,0,255);
   rectangle_Fill(img,0,0,255,31);
  

   /* Fadein the loading image effect */
   fadeInTexture(texturaCarga, centerX-midW, centerY-midH, 
                 centerX+midW, centerY+midH, 256,128);

   /* Loading Map */
   if(actualMap) 
   {
      /* Show the "Saving Map Status" */
      color_Set(200,20,20,255);
      fnt.defineFont(DNT_FONT_TIMES,10);
      sprintf(texto,gettext("Saving Current Map Modifications"));
      showLoading(img,&texturaTexto,texturaCarga, texto, progress);
      progress->defineActualHealth(1);

      keepNPCInventoryStatus();
      
      arqVelho = actualMap->getFileName();
      delete(actualMap);

      /* Remove All Unused 3D Models */
      models->removeUnusedModels();
   }

   /* Remove all NPCS  */
   if(NPCs)
   {
     delete(NPCs);
   }

   /* Close the Dialog or Barter windows, if opened */
   barterWindow btWindow;
   dialogWindow dlgWindow;
   btWindow.close();
   dlgWindow.close();

   /* Remove All Pending Actions */
   actionControl->removeAllActions();

   /* Remove All Sound Effects */
   snd->removeAllSoundEffects();
   walkSound = NULL;

   /* Show the "Loading Map" */
   color_Set(200,20,20,255);
   fnt.defineFont(DNT_FONT_TIMES,10);
   sprintf(texto,gettext("Loading Map: %s"),arqMapa.c_str());
   showLoading(img,&texturaTexto,texturaCarga, texto, progress);
   progress->defineActualHealth(3);

   actualMap = new Map();
   actualMap->setFileName(arqVelho);
   actualMap->open(arqMapa,*models, *weaponsTypes);

   /* Enable, if needed, the FOG */
   mapFog fog = actualMap->getFog();
   if(fog.enabled)
   {
      glEnable(GL_FOG);
      {
        glFogi(GL_FOG_MODE,GL_LINEAR);
        glFogfv(GL_FOG_COLOR, fog.color);
        glFogf(GL_FOG_DENSITY, fog.density);
        glHint(GL_FOG_HINT,GL_DONT_CARE);
        glFogf(GL_FOG_START, fog.start);
        glFogf(GL_FOG_END, fog.end);
      }
   }
   else
   {
      GLdouble fogEnd = (option->getFarViewFactor()*OUTDOOR_FARVIEW) + 4000;
      GLdouble fogStart = 200;
      GLdouble fogDensity = 5.0;
      GLfloat color[3] = {1.0,1.0,1.0};
      if(!actualMap->isOutdoor())
      {
         color[0] = 0.0;
         color[1] = 0.0;
         color[2] = 0.0;
         fogStart = 40;
         fogDensity = 1.0;
         fogEnd = (INDOOR_FARVIEW)-2;
      }
      glEnable(GL_FOG);
      {
        glFogi(GL_FOG_MODE,GL_LINEAR);
        glFogfv(GL_FOG_COLOR, color);
        glFogf(GL_FOG_DENSITY, fogDensity);
        glHint(GL_FOG_HINT, GL_DONT_CARE);
        glFogf(GL_FOG_START, fogStart);
        glFogf(GL_FOG_END, fogEnd);
      }
   }

   /* Loading NPCs */
   NPCs = NULL;
   character* per;
   if(!actualMap->getNpcFileName().empty())
   {
      string npcFileName="", name="";
      GLfloat posX=0, posZ=0, ori=0;

      npcFile* arq = new npcFile();
      NPCs = new (characterList);

      if(arq->load(actualMap->getNpcFileName()))
      { 
         while(arq->getNextCharacter(name, npcFileName, posX, posZ, ori))
         {
           sprintf(texto, gettext("Loading NPC: %s"), name.c_str());
           showLoading(img,&texturaTexto,texturaCarga,
                       texto, progress);
           per = NPCs->insertCharacter(npcFileName, features, this, arqMapa);
           /* Define Initial Position */
           per->initialXPosition = posX;
           per->initialZPosition = posZ;
           per->orientation = ori;
           per->xPosition = posX;
           per->zPosition = posZ;
           per->yPosition = actualMap->getHeight(posX, posZ);
           /* Define Occuped Square */
           int posX =(int)floor(per->xPosition / actualMap->squareSize());
           int posZ =(int)floor(per->zPosition / actualMap->squareSize());
           per->ocSquare = actualMap->relativeSquare(posX,posZ);
         }
      }  
      delete(arq);
   }
   progress->defineActualHealth(6);

   //FIXME
   progress->defineActualHealth(7);

   /* Create the MiniMap */
   showLoading(img,&texturaTexto,texturaCarga,
               gettext("Creating MiniMap"), progress);
   actualMap->drawMiniMap(models);
   progress->defineActualHealth(8);

   /* Loading Internal Windows */
   showLoading(img,&texturaTexto,texturaCarga,
               gettext("Putting GUI on Screen"), progress);

   progress->defineActualHealth(9);

   /* Reopen, if already opened, some game Windows */
   if(mapWindow->isOpened())
   {
      mapWindow->close(gui);
      mapWindow->open(gui,0,0, actualMap);
   }
   if(shortcuts->isOpened())
   {
      shortcuts->close(gui);
      shortcuts->open(gui);
   }
   if(brief->isOpened())
   {
      brief->closeWindow(gui);
      brief->openWindow(gui);
   }

   /* Updating the BoundingBoxes for PCs */
   int aux;
   per = (character*) PCs->getFirst();
   for(aux=0;aux < PCs->getTotal();aux++)
   {
      per->setState(STATE_IDLE);
      per->update(0); 
      per->calculateBoundingBox();  
      per = (character*) per->next;
   }

   /* Updating the BoundingBoxes for NPCs */
   if(NPCs)
   { 
      per = (character*) NPCs->getFirst();
      for(aux=0; aux < NPCs->getTotal();aux++)
      {
         per->setState(STATE_IDLE);
         per->update(0); 
         per->calculateBoundingBox();  
         per = (character*) per->next;
      }
   }

   /* Update  particle System to a stable state */
   showLoading(img,&texturaTexto,texturaCarga,
               gettext("Loading Particles"), progress);
   progress->defineActualHealth(10);
   if(!actualMap->getParticlesFileName().empty())
   {
       particleController.loadFromFile(actualMap->getParticlesFileName());
       if(option->getEnableParticles())
       {
          particleController.stabilizeAll();
       }
   }
   else
   {
      particleController.deleteAll();
   }
   

   /* Put Active Party at initial Position */
   activeCharacter = PCs->getActiveCharacter();
   actualMap->getInitialPosition(activeCharacter->xPosition,
                                 activeCharacter->zPosition,
                                 activeCharacter->orientation);
   activeCharacter->yPosition=actualMap->getHeight(activeCharacter->xPosition,
                                                   activeCharacter->zPosition);
   gameCamera.updateCamera(activeCharacter->xPosition,
                           activeCharacter->yPosition,
                           activeCharacter->zPosition,
                           activeCharacter->orientation);
   activeCharacter->ocSquare = actualMap->getInitialSquare();

   showLoading(img,&texturaTexto,texturaCarga,
               gettext("Loading Changes..."), progress);
   progress->defineActualHealth(11);

   /* Do Modifications */
   modifState.doMapModifications(actualMap, NPCs, *models, *weaponsTypes);

   /* Change Music, if needed */
   if(!actualMap->getMusicFileName().empty())
   {
      snd->loadMusic(actualMap->getMusicFileName());
   }

   /* Flush Sound Effects, if needed, setting listener before to avoid
    * volume and positional errors */
   snd->setListenerPosition(gameCamera.getCameraX(), gameCamera.getCameraY(),
                            gameCamera.getCameraZ(), gameCamera.getTheta(),
                            gameCamera.getPhi(), gameCamera.getD(),
                            gameCamera.getDeltaY());
   actualMap->flushSounds();

   /* Define Map */
   particleController.setActualMap(actualMap, &colisionDetect);
   colisionDetect.defineMap(actualMap, NPCs, PCs);
   actionControl->setCharacterLists(NPCs, PCs);
   fight->setMap(actualMap->getFileName());

   /* Done */
   showLoading(img,&texturaTexto,texturaCarga,
               gettext("Done!"), progress);

   /* Somw fadeout effect */
   glDisable(GL_LIGHTING);
   fadeOutTexture(texturaCarga,centerX-midW,centerY-midH,
                  centerX+midW,centerY+midH, 256,128);

   /* Free Loading Textures */
   SDL_FreeSurface(img);
   glDeleteTextures(1,&texturaTexto);

   /* Set the Farview to indoor or outdoor */
   if(actualMap->isOutdoor())
   {
      redefineWindow(actualScreen, option->getFarViewFactor()*OUTDOOR_FARVIEW);
   }
   else
   {
      redefineWindow(actualScreen, INDOOR_FARVIEW);
   }

   glEnable(GL_LIGHTING);

   return(1);
}

/*********************************************************************
 *                            fadeInTexture                          *
 *********************************************************************/
void engine::fadeInTexture(GLuint id, int x1, int y1, int x2, int y2,
                           int sizeX, int sizeY)
{
   int i;
   for(i=0; i < 50; i++)
   {
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT
              | GL_STENCIL_BUFFER_BIT);
      draw2DMode();
      glColor3f(i/50.0, i/50.0, i/50.0);
      textureToScreen(id,x1,y1,x2,y2,sizeX,sizeY);
      draw3DMode(option->getFarViewFactor()*OUTDOOR_FARVIEW);
      glFlush();
      SDL_GL_SwapBuffers();
      SDL_Delay(10);
   }
   glColor3f(1.0,1.0,1.0);
}

/*********************************************************************
 *                            fadeOutTexture                         *
 *********************************************************************/
void engine::fadeOutTexture(GLuint id, int x1, int y1, int x2, int y2,
                            int sizeX, int sizeY)
{
   int i;
   for(i=49; i >= 0; i--)
   {
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT
              | GL_STENCIL_BUFFER_BIT);
      draw2DMode();
      glColor3f(i/50.0, i/50.0, i/50.0);
      textureToScreen(id,x1,y1,x2,y2,sizeX,sizeY);
      draw3DMode(option->getFarViewFactor()*OUTDOOR_FARVIEW);
      glFlush();
      SDL_GL_SwapBuffers();
      SDL_Delay(10);
   }
   glColor3f(1.0, 1.0, 1.0);
}


/*********************************************************************
 *                             SplashScreen                          *
 *********************************************************************/
void engine::splashScreen()
{
   GLuint id;
   Uint32 mButton = 0;
   Uint8* keys;
   bool done = false;
   int x,y;
   Uint32 time = SDL_GetTicks();
   glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   updateFrustum(visibleMatrix,proj,modl);
   SDL_Surface* img;
   img = IMG_Load(dir.getRealFile("texturas/general/inicio1.png").c_str()); 
   glDisable(GL_LIGHTING);

   dntFont fnt;
   fnt.defineFont(DNT_FONT_TIMES, 20);
   fnt.defineFontAlign(DNT_FONT_ALIGN_CENTER);
   color_Set(130,3,3,255);
   fnt.write(img, 52, 424, gettext("DccNiTghtmare is developed under Public "
                                   "Domain: Feel free to use its source code "
                                   "and data the way you want (or to blame us "
                                   "the same way). DccNiTghtmare is thankful "
                                   "to the people who developed those "
                                   "libraries: OpenAL, Cal3D, OpenGL, SDL, "
                                   "SDL_image, SDL_ttf, libVorbis."), 
              52, 424, 745, 511);
   fnt.defineFontAlign(DNT_FONT_ALIGN_LEFT);
   glGenTextures(1,&id);
   setTextureRGBA(img,id);
   SDL_FreeSurface(img);

   /* Fade In Screen */
   fadeInTexture(id,0,0,SCREEN_X-1,SCREEN_Y-1, 800, 600);

   /* Wait until Mouse Button pressed or time passed */
   while( !done )
   {

      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT
              | GL_STENCIL_BUFFER_BIT);
      draw2DMode();
      glColor3f(1.0, 1.0, 1.0);
      textureToScreen(id, 0, 0, SCREEN_X-1, SCREEN_Y-1, 800, 600);
      draw3DMode(option->getFarViewFactor()*OUTDOOR_FARVIEW);
      glFlush();
      SDL_GL_SwapBuffers();

      SDL_PumpEvents();
      mButton = SDL_GetMouseState(&x,&y);
      keys = SDL_GetKeyState(NULL);

      done = ( (mButton & SDL_BUTTON(1)) || 
               (keys[SDLK_ESCAPE]) ||
               (keys[SDLK_RETURN]) ||
               ( (SDL_GetTicks() - time) > 5000) );

      SDL_Delay(50);
   }

   /* Wait Mouse Button release */
   while(mButton & SDL_BUTTON(1))
   {
      SDL_PumpEvents();
      mButton = SDL_GetMouseState(&x,&y);
      SDL_Delay(50);
   }

   fadeOutTexture(id,0,0,SCREEN_X-1,SCREEN_Y-1, 800, 600);
   
   glEnable(GL_LIGHTING);
   glDeleteTextures(1,&id);
}

/*********************************************************************
 *                       Call Initial Game Menu                      *
 *********************************************************************/
int engine::menuScreen(int Status, GLuint idTextura, bool reloadMusic)
{
   /* Reload Music, if needed */
   if(reloadMusic)
   {
      /*music =*/
      snd->loadMusic("music/musica1.ogg");
   }

   cursors->set(CURSOR_WALK);

   /* Executes Initial Screen */
   updateFrustum(visibleMatrix,proj,modl);
   initialScreen* inic = new(initialScreen);
   int result = inic->run(Status, proj, modl, viewPort, idTextura, snd, gui);
   delete(inic);
   return(result);
}

/*********************************************************************
 *                       Call Options Game Screen                    *
 *********************************************************************/
int engine::optionsScreen(GLuint idTextura)
{
   int optionW = OPTIONSW_OTHER;
   int time = SDL_GetTicks();
   int timeAnterior = 0;
   Uint8* keys;
   int x,y;
   guiObject* object = NULL;
   int eventInfo = NOTHING;

   glDisable(GL_LIGHTING);

   option->displayOptionsScreen(gui);

   while( (optionW != OPTIONSW_CANCEL) && 
          (optionW != OPTIONSW_CONFIRM))
   {
      time = SDL_GetTicks();
      if(time - timeAnterior >= UPDATE_RATE) 
      {
         timeAnterior = time;
         SDL_PumpEvents();
         glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
         glClearColor(0,0,0,1);
         keys = SDL_GetKeyState(NULL);
         Uint8 mButton = SDL_GetMouseState(&x,&y);
         object = gui->manipulateEvents(x,y,mButton,keys, eventInfo);
         
         glPushMatrix();
            draw2DMode();
            textureToScreen(idTextura,0,0,SCREEN_X-1,SCREEN_Y-1,800,600);
            gui->draw(proj,modl,viewPort);

            glPushMatrix();
               cursors->draw(x, y);
            glPopMatrix();

            draw3DMode(option->getFarViewFactor()*OUTDOOR_FARVIEW);
         glPopMatrix();
         glFlush();
         SDL_GL_SwapBuffers();
         optionW = option->treat(object,eventInfo,gui,proj,modl,viewPort);
      }
      else if((UPDATE_RATE-1) - (time - timeAnterior) > 0 ) 
      {
         SDL_Delay((UPDATE_RATE-1) - (time - timeAnterior) );
      }
   }
  
   if(optionW == OPTIONSW_CONFIRM)
   {
      snd->changeVolume(option->getMusicVolume(), option->getSndFxVolume());
      gameCamera.defineCameraType(option->getCameraNumber());
      /* Change the language */
      option->setLanguage();
   }

   glEnable(GL_LIGHTING);
   return(optionW);
}

/*********************************************************************
 *              Call Screens to Create, Evolute Character            *
 *********************************************************************/
int engine::characterScreen(GLuint idTextura)
{
   int charCreation = CHAR_OTHER;
   int time = SDL_GetTicks();
   int timeAnterior = 0;
   Uint8* keys;
   int x,y;
   guiObject* object = NULL;
   int eventInfo = NOTHING;

   int mods[6];

   int status = 0;

   /* First Load PCs */
   loadPC("characters/pcs/metaleiro.pc");

   /* Att Screen */
   attWindow* atWindow = NULL;

   /* Skill Screen */
   skillWindow* skWindow = NULL;

   /* Alignment Window */
   alignWindow* alWindow = NULL;

   /* Class Window */
   classWindow* clWindow = NULL;

   /* Aspect Window */
   aspectWindow* aspWindow = NULL;

   /* Disable Lights and Fog */
   glDisable(GL_LIGHTING);

   /* Race Window */
   activeCharacter = PCs->getActiveCharacter();
   raceWindow* rcWindow = new raceWindow(gui, &activeCharacter->actualRace);

   while( (status != 6) )
   {
      time = SDL_GetTicks();
      if(time - timeAnterior >= UPDATE_RATE) 
      {
         timeAnterior = time;
         SDL_PumpEvents();
         glClearColor(0,0,0,1);
         glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
         keys = SDL_GetKeyState(NULL);
         Uint8 mButton = SDL_GetMouseState(&x,&y);
         object = gui->manipulateEvents(x,y,mButton,keys, eventInfo);

         
         glPushMatrix();
            draw2DMode();
            textureToScreen(idTextura,0,0,SCREEN_X-1,SCREEN_Y-1,800,600);
            gui->draw(proj,modl,viewPort);

            glPushMatrix();
               cursors->draw(x, y);
            glPopMatrix();

            draw3DMode(option->getFarViewFactor()*OUTDOOR_FARVIEW);
         glPopMatrix();
         glFlush();
         SDL_GL_SwapBuffers();

         /* Race Window Opened */
         if(status == 0)
         {
            charCreation = rcWindow->treat(object, eventInfo, gui);
            if(charCreation == RACEW_CONFIRM)
            {
               status = 1;
               delete(rcWindow);
               clWindow = new classWindow(gui,&activeCharacter->actualClass[0]);
            }
            else if(charCreation == RACEW_CANCEL)
            {
               status = 6;
               delete(rcWindow);
               charCreation = CHAR_CANCEL;
            }
         }
         /* Class Window Opened */
         else if(status == 1)
         {
            charCreation = clWindow->treat(object, eventInfo, gui);
            if(charCreation == CLASSW_CONFIRM)
            {
               status = 2;
               delete(clWindow);
               alWindow = new alignWindow(alignList, gui, 
                                          &activeCharacter->actualAlign);
            }
            else if(charCreation == CLASSW_CANCEL)
            {
               status = 0;
               delete(clWindow);
               rcWindow = new raceWindow(gui,&activeCharacter->actualRace);
            }
         }
         /* Aligment Window Opened */
         else if(status == 2)
         {
            charCreation = alWindow->treat(object, eventInfo, gui);
            if(charCreation == ALIGNW_CONFIRM)
            {
               status = 3;
               delete(alWindow);
               activeCharacter->getAttModifiers(mods);
               atWindow = new attWindow(&activeCharacter->sk, gui, mods, false);
            }
            else if(charCreation == ALIGNW_CANCEL)
            {
               status = 1;
               delete(alWindow);
               clWindow = new classWindow(gui,&activeCharacter->actualClass[0]);
            }
         }
         /* Attribute Window Opened */
         else if(status == 3)
         {
             charCreation = atWindow->treat(object, eventInfo, gui,
                                            proj, modl,viewPort);
             if(charCreation == ATTW_CONFIRM)
             {
               status = 4;
               delete(atWindow);
               activeCharacter->clearSkills();
               skWindow = new skillWindow(&activeCharacter->sk, gui, 
                                          activeCharacter->getLevel());
             }
             else if(charCreation == ATTW_CANCEL)
             {
                status = 2;
                delete(atWindow);
                alWindow = new alignWindow(alignList, gui,
                                           &activeCharacter->actualAlign);
             }
         }
         /* Skills Window Opened */
         else if(status == 4)
         {
            charCreation = skWindow->treat(object, eventInfo, gui); 
            if(charCreation == SKILLW_CONFIRM)
            {
               status = 5;
               delete(skWindow);
               aspWindow = new aspectWindow(activeCharacter, gui);
            }
            else if(charCreation == SKILLW_CANCEL)
            {
               status = 3;
               delete(skWindow);
               activeCharacter->getAttModifiers(mods);
               atWindow = new attWindow(&activeCharacter->sk, gui, mods, true);
            }
         }
         /* Aspect Window Opened */
         else if(status == 5)
         {
            charCreation = aspWindow->treat(object, eventInfo, gui);
            if(charCreation == ASPECTW_CONFIRM)
            {
               status = 6;
               delete(aspWindow);
               charCreation = CHAR_CONFIRM;
            }
            else if(charCreation == ASPECTW_CANCEL)
            {
               status = 4;
               delete(aspWindow);
               skWindow = new skillWindow(&activeCharacter->sk,
                                          gui, activeCharacter->getLevel());
            }
         }         
      }
      else if((UPDATE_RATE-1) - (time - timeAnterior) > 0 ) 
      {
         SDL_Delay((UPDATE_RATE-1) - (time - timeAnterior) );
      }
   }


   /* Apdate and calculate things related to character */
   if(charCreation == CHAR_CONFIRM)
   {
      /* Calculate Life Points */
      activeCharacter->defineInitialLifePoints();
      activeCharacter->applyBonusAndSaves();
   }

   glEnable(GL_LIGHTING);
   
   return(charCreation);
}


/*********************************************************************
 *                     redefine Window to Screen                     *
 *********************************************************************/
void engine::redefineWindow(SDL_Surface *screen, float actualFarView)
{
   glViewport (0, 0, SCREEN_X, SCREEN_Y);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, SCREEN_X / (float)SCREEN_Y, 1.0, 
                  actualFarView);
   glGetIntegerv(GL_VIEWPORT, viewPort);
   glGetFloatv(GL_MODELVIEW_MATRIX, camProj);
   glMatrixMode (GL_MODELVIEW);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

   glEnable(GL_LINE_SMOOTH);
   glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
   glLineWidth (1.5);
}

/*********************************************************************
 *                       Init Engine Function                        *
 *********************************************************************/
void engine::init(SDL_Surface *screen)
{
   actualScreen = screen;  
   redefineWindow(screen, option->getFarViewFactor()*OUTDOOR_FARVIEW);
   
   /* Clear */
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClearDepth(1.0);
   glClearStencil(0);

   /* Details Definition */
   glDepthFunc(GL_LEQUAL);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);

   /* Light Definition */
   GLfloat light_ambient[] = { 0.25, 0.25, 0.25, 1.0 };
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

   glEnable(GL_LIGHTING);
   glEnable(GL_NORMALIZE);

   if(option->getAntiAliasing())
   {
      glEnable(GL_MULTISAMPLE);
   }

   /* Sky Creation */
   gameSky = new(sky);

   SDL_Surface* img;

   /* Battle Circle Textures */
   img = IMG_Load(
             dir.getRealFile("texturas/fightMode/normalMovCircle.png").c_str());
   if(!img)
   {
      printf("Error: can't Load Texure: fightMode/normalMovCircle.png\n");
   }
   glGenTextures(1, &normalMoveCircle);

   glBindTexture(GL_TEXTURE_2D, normalMoveCircle);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   SDL_FreeSurface(img);

   img = IMG_Load(
               dir.getRealFile("texturas/fightMode/fullMovCircle.png").c_str());
   if(!img)
   {
      printf("Error: can't Load Texure: fightMode/fullMovCircle.png\n");
   }

   glGenTextures(1, &fullMoveCircle);

   glBindTexture(GL_TEXTURE_2D, fullMoveCircle);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   SDL_FreeSurface(img);

   /* feat range circle */
   img = IMG_Load(
             dir.getRealFile("texturas/fightMode/featRangeCircle.png").c_str());
   if(!img)
   {
      printf("Error: can't Load Texure: fightMode/featRangeCircle.png\n");
   }

   glGenTextures(1, &featRangeCircle);

   glBindTexture(GL_TEXTURE_2D, featRangeCircle);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   SDL_FreeSurface(img);

   /* range circle */
   img = IMG_Load(dir.getRealFile("texturas/walk/range.png").c_str());
   if(!img)
   {
      printf("Error: can't Load Texure: walk/range.png\n");
   }

   glGenTextures(1, &rangeCircle);

   glBindTexture(GL_TEXTURE_2D, rangeCircle);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   SDL_FreeSurface(img);


   /* Move Destiny */
   img = IMG_Load(dir.getRealFile("texturas/walk/destino.png").c_str());
   if(!img)
   {
      printf("Error: can't Load Texture: destino.png\n");
   }

   glGenTextures(1, &destinyImage);

   glBindTexture(GL_TEXTURE_2D, destinyImage);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   SDL_FreeSurface(img);

    /* ShadowMap */
   shadowMap.init();
   //glPolygonOffset(-2.0, -1.0);
}

/*********************************************************************
 *                             rangeAction                           *
 *********************************************************************/
bool engine::rangeAction(GLfloat posX, GLfloat posZ, 
                         GLfloat targX, GLfloat targZ,
                         GLfloat range)
{
   if(! actionInRange(posX, posZ, targX, targZ, range) )
   {
      brief->addText(gettext("Too far away for the action!"), 220, 20, 20);
      return(false);
   }
   return(true);
}

/*********************************************************************
 *                           exitBattleMode                          *
 *********************************************************************/
void engine::exitBattleMode()
{
   /* Empty Fight */
   fight->empty();

   /* Change Engine Mode */
   engineMode = ENGINE_MODE_REAL_TIME;

   /* Abort All pending Actions */
   actionControl->abortAllActions();
}

/*********************************************************************
 *                          enterBattleMode                          *
 *********************************************************************/
void engine::enterBattleMode(bool surprisePC)
{
  int numEnemies = 0;
  character* ch;
  int i;

  /* Close Dialog window, if openned */
  dialogWindow dlgWindow;
  dlgWindow.close();
  
  activeCharacter = PCs->getActiveCharacter();
  
  /* clear fight status */
  fight->empty();

  /* Verify if there are NPCs */
  if(!NPCs)
  {
     brief->addText(gettext("No NPCs in the area."), 220, 20, 20);
     return;
  }

  /* Add NPCs to Fight */
  ch =(character*) NPCs->getFirst();
  for(i = 0; i < NPCs->getTotal(); i++)
  {
      //TODO put enemies on groups, when enemy from enemy
      fight->insertNPC(ch, 0);
      numEnemies++;
      /* Set the state to Idle, if the character is alive */
      if(ch->isAlive())
      {
         ch->callIdleAnimation();
         /* Remove Move, if it is moving */
         ch->pathFind.clear();
      }
      ch = (character*) ch->next; 
      SDL_Delay(1);
  }
                 
   if(numEnemies > 0)
   {
      /* Really Init the Battle */
      snd->addSoundEffect(SOUND_NO_LOOP,"sndfx/battleMode.ogg");
      engineMode = ENGINE_MODE_TURN_BATTLE;
      moveCircleX = activeCharacter->xPosition;
      moveCircleY = activeCharacter->yPosition;
      moveCircleZ = activeCharacter->zPosition;

      /* Put the PCs on group */
      ch =(character*) PCs->getFirst();
      for(i = 0; i < PCs->getTotal(); i++)
      {
         fight->insertPC(ch, 0);
         ch = (character*) ch->next; 
         /* Set the state to Idle */
         ch->callIdleAnimation();
         /* Remove Move, if it is moving */
         ch->pathFind.clear();
         SDL_Delay(1);
      }
                   
      /* Define PC turn, cause the round of surprise attack! */
      if(surprisePC)
      {
         fightStatus = FIGHT_PC_TURN;
         PCs->getActiveCharacter()->setCanMove(true);
         PCs->getActiveCharacter()->setCanAttack(true);
         fight->setActualActor(PCs->getActiveCharacter());
         brief->addText(gettext("Surprise Attack Turn."));
      }
      else
      {
         fightStatus = FIGHT_CONTINUE;
      }
      attackFeat = activeCharacter->getActiveFeatRangeType();

      /* Abort All pending Actions */
      actionControl->abortAllActions();

   }
   else
   {
      brief->addText(gettext("No enemies in the area."));
   }
}

/*********************************************************************
 *                              endTurn                              *
 *********************************************************************/
void engine::endTurn()
{
   if( (engineMode == ENGINE_MODE_TURN_BATTLE) &&
       (fightStatus == FIGHT_PC_TURN)  && 
       ((SDL_GetTicks() - lastTurnTime) > 200) )
   {
      fightStatus = FIGHT_CONTINUE;
   }
   else if( (engineMode == ENGINE_MODE_TURN_BATTLE) &&
            (fightStatus == FIGHT_NPC_TURN) )
   {
      fightStatus = FIGHT_CONTINUE;
   }
}

/*********************************************************************
 *                         do A* search cycle                        *
 *********************************************************************/
void engine::doAStar()
{
   int aux = 0;
   character* per = NULL;

   if(NPCs)
   {
      per = (character*) NPCs->getFirst();
      for(aux=0; aux < NPCs->getTotal(); aux++)
      {
         per->pathFind.doCycle((engineMode == ENGINE_MODE_TURN_BATTLE));
         per = per->next;
      }
   }

   if(PCs)
   {
      per = (character*) PCs->getFirst();
      for(aux=0; aux < PCs->getTotal(); aux++)
      {
         per->pathFind.doCycle((engineMode == ENGINE_MODE_TURN_BATTLE));
         per = per->next;
      }

   }
}

/*********************************************************************
 *                        treat all scripts                          *
 *********************************************************************/
void engine::treatScripts()
{
   if(engineMode != ENGINE_MODE_TURN_BATTLE)
   {
      /* Treat all pending actions */
      treatPendingActions();

      /* Treat missions scripts */
      missions->treat(actualMap, NPCs);

      /* Treat NPCs scripts */
      if(NPCs)
      {
         NPCs->treatGeneralScripts(actualMap, NPCs);
      }
   }
   else
   {
      /* Treat Pending Combat Actions */
      treatPendingActions();

      /* Treat actual NPC script, if have one */
      if(fightStatus == FIGHT_NPC_TURN)
      {
         character* npc = fight->actualCharacterTurn();
         if(npc)
         {
            iaScript* script = (iaScript*) npc->getBattleScript();
            if( (script) && (npc->isAlive()))
            {
               script->defineMap(actualMap, NPCs);
               script->run(MAX_SCRIPT_LINES);

               /* End turn when script finished */
               if(script->finished())
               {
                  /* Verify Deads */
                  fight->verifyDeads();
                  /* End the NPC turn */
                  endTurn();
                  /* Reput script at start */
                  script->restart();
               }
            }
         }
      }
   }
}

/*********************************************************************
 *                       Treat Pending Actions                       *
 *********************************************************************/
void engine::treatPendingActions()
{
   actionControl->treatActions(actualMap, 
                               engineMode == ENGINE_MODE_TURN_BATTLE);
}

/*********************************************************************
 *                          Treat GUI Events                         *
 *********************************************************************/
void engine::treatGuiEvents(guiObject* object, int eventInfo)
{
   barterWindow tradeWindow;
   dialogWindow dlgWindow;
   
   /* Verify if Inventory Window is opened */
   if(inventoryWindow)
   {
      if(!inventoryWindow->isOpen())
      {
         /* window is no more opened, so free structs */
         openCloseInventoryWindow();
      }  
   }

   /* Verify if Barter Window is Opened */
   tradeWindow.treat(object, eventInfo, mouseX, mouseY, cursors, actualMap);

   /* Verify Dialog Windows */
   dlgWindow.treat(object, eventInfo, infoWindow, this);

   /* Verify Inventory Window Actions */
   if( (inventoryWindow) )
   {
      if( ( (engineMode == ENGINE_MODE_TURN_BATTLE) && 
            (fightStatus == FIGHT_PC_TURN) ) ||
          (engineMode == ENGINE_MODE_REAL_TIME) )
      {
         if(inventoryWindow->treat(object, eventInfo, cursors, actualMap,
                                   PCs->getActiveCharacter()->xPosition,
                                   PCs->getActiveCharacter()->zPosition))
         {
            /* Redefine, if need, the weapons */
            PCs->getActiveCharacter()->defineWeapon();

            /* Menu Use count as an action */
            PCs->getActiveCharacter()->setCanAttack(false);

            /* TODO redefine the armors! */
         }
      }
   }

   /* Verify Item Window */
   if( infoWindow->isOpen() )
   {
      infoWindow->treat(object, eventInfo);
   }

   /* Verify Character Window */
   if( (charInfoWindow->isOpen()) || (charInfoWindow->hasChildrenWindows()) )
   {
      charInfoWindow->treat(object, eventInfo, proj, modl, viewPort);
   }

   /* Verify ShortCutsWindow */
   if(shortcuts->isOpened())
   {
      int res = shortcuts->treat(object, eventInfo, engineMode);
      if( (res == SHORTCUTS_WINDOW_NONE) && (gui->mouseOnGui(mouseX, mouseY)) )
      {
         shortcuts->setThing(gettext("Nothing"));
      }
      /* Now call the functions */
      switch(res)
      {
         case SHORTCUTS_WINDOW_MENU:
         {
            /* Go back to the game menu */
            exitEngine = 1;
         }
         break;
         case SHORTCUTS_WINDOW_SAVE:
         {
            /* Go to the save dialog */
            defineFrontSurface();
            saveGame();
         }
         break;
         case SHORTCUTS_WINDOW_LOAD:
         {
            /* Go to the load dialog */
            loadGame();
         }
         break;
         case SHORTCUTS_WINDOW_ATTACK_MODE:
         {
            if( engineMode != ENGINE_MODE_TURN_BATTLE )
            {
               /* Enter the battle mode! */
               enterBattleMode(true);
            }
            else
            {
               // TODO -> set the attack to base one.
            }
         }
         break;
         case SHORTCUTS_WINDOW_MAP:
         {
            /* Open, if not opened the minimap window */
            mapWindow->open(gui, 0,0, actualMap);
         }
         break;
         case SHORTCUTS_WINDOW_END_TURN:
         {
            if(fightStatus == FIGHT_PC_TURN)
            {
               /* End the Playable Character turn */
               endTurn();
            }
         }
         break;
         case SHORTCUTS_WINDOW_INVENTORY:
         {
            /* Open, if not already opened, the invetory window */
            if(!inventoryWindow)
            {
               openCloseInventoryWindow();
            }
         }
         break;
         case SHORTCUTS_WINDOW_CHARACTER:
         {
            /* Open, if not Opened, the character window */
            if(charInfoWindow)
            {
               charInfoWindow->open(PCs->getActiveCharacter());
            }
         }
         break;

         //TODO other results!
      }
  }  


}

/*********************************************************************
 *                              hourToTxt                            *
 *********************************************************************/
void engine::hourToTxt()
{
   char htmp[32];
   int ihour = (int)hour;
   int imin = (int) (( hour - ihour ) * 60 );
   
   sprintf(&htmp[0],"%.2d:%.2d", ihour, imin);

   shortcuts->setHour(htmp);
}

/*********************************************************************
 *                         updateMouseWorldPos                       *
 *********************************************************************/
void engine::updateMouseWorldPos()
{
   GLfloat wx,wy,wz=1;
   
   /* Define Mouse OpenGL Window Coordinate */
   wx = mouseX; wy = SCREEN_Y - mouseY; 
  
   /* Get the Z position of the mouse */
   glReadPixels((int)wx,(int)wy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz); 

   /* Get the world coordinate of the mouse position */
   gluUnProject(wx,wy,wz,modl,proj,viewPort,&xReal,&yReal,&zReal);
}

/*********************************************************************
 *                         verifyMouseActions                        *
 *********************************************************************/
int engine::verifyMouseActions(Uint8 mButton)
{
   char buf[1024];

   Uint32 time = SDL_GetTicks();
   activeCharacter = PCs->getActiveCharacter();

   /* Create a bounding box for the mouse position */
   GLfloat minMouse[3], maxMouse[3];
   minMouse[0] = xReal-3;  maxMouse[0] = xReal+3;
   minMouse[1] = 0.0;      maxMouse[1] = 0.0;
   minMouse[2] = zReal-3;  maxMouse[2] = zReal+3;

   int qx, qz;
   qx = (int)xReal / actualMap->squareSize();
   qz = (int)zReal / actualMap->squareSize();
   Square* quaux = actualMap->relativeSquare(qx,qz);
   if(quaux != NULL)
   {
      int pronto;
      int obj = 0;
      GLfloat minObj[3], maxObj[3];
      objSquare* sobj = quaux->getFirstObject();

      /* Objects Verification */
      for(pronto = 0; ( (obj < quaux->getTotalObjects()) && (!pronto) );obj++)
      {
         if( (sobj->obj) && ( (sobj->obj->canGet()) || 
             (!sobj->obj->getConversationFile().empty()) ) )
         {
            boundingBox bound = sobj->obj->getBoundingBox();
            GLfloat X[4]; GLfloat Z[4];
            X[0] = bound.x1;
            Z[0] = bound.z1;
            X[1] = bound.x1;
            Z[1] = bound.z2; 
            X[2] = bound.x2;
            Z[2] = bound.z2;
            X[3] = bound.x2;
            Z[3] = bound.z1;
            rotTransBoundingBox(sobj->orientation, X, Z, sobj->x, 0.0, 
                  0.0, sobj->z, minObj, maxObj);
            if(intercepts( minObj, maxObj, minMouse, maxMouse))
            {
               shortcuts->setThing(sobj->obj->getName());
               
               /* The Object Dialog Window Call */
               if(!sobj->obj->getConversationFile().empty())
               {
                  cursors->set(CURSOR_USE);
                  if( (mButton & SDL_BUTTON(1)) && 
                      (rangeAction(activeCharacter->xPosition, 
                                   activeCharacter->zPosition,
                                   sobj->x, sobj->z,
                                   WALK_PER_MOVE_ACTION)) )
                  {
                     dialogWindow dlgWindow;
                     dlgWindow.open(gui, activeCharacter, 
                           (conversation*)sobj->obj->getConversation(), 
                           sobj->obj->get2dModelName());
                  }
               }
               else
               {
                  cursors->set(CURSOR_GET);
                  shortcuts->setThing(sobj->obj->getName()); 
                  if( (mButton & SDL_BUTTON(1)) && 
                      (rangeAction(activeCharacter->xPosition, 
                                   activeCharacter->zPosition,
                                   sobj->x, sobj->z,
                                   WALK_PER_MOVE_ACTION) ) )
                  {
                     if( (engineMode != ENGINE_MODE_TURN_BATTLE) ||
                           (activeCharacter->getCanAttack() ) )
                     {
                        /* Get Item count as action */
                        activeCharacter->setCanAttack(false);

                        /* Get Object */
                        lastMousePression = time;

                        if(activeCharacter->inventories->addObject(sobj->obj))
                        {
                           snd->addSoundEffect(sobj->x, sobj->y, sobj->z, 
                                 SOUND_NO_LOOP,
                                 "sndfx/objects/take_item.ogg");

                           sprintf(buf,gettext("%s taken."),
                                 sobj->obj->getName().c_str());

                           brief->addText(buf);

                           /* Log State to the modState */
                           modifState.mapObjectAddAction(
                                 MODSTATE_ACTION_OBJECT_REMOVE,
                                 sobj->obj->getFileName(),
                                 actualMap->getFileName(),
                                 sobj->x, sobj->y, sobj->z);

                           /* Remove object from Map */
                           actualMap->removeObject(sobj->obj);

                           if(inventoryWindow)
                           {
                              inventoryWindow->reDraw();
                           }
                        }
                        else
                        {
                           brief->addText(gettext("Inventory is full!"),
                                 220,20,20); 
                        }
                     }
                  }
               }
               pronto = 1;
            }
         }
         sobj = sobj->next;
      }

      /* Doors Verification */
      door* porta = actualMap->getFirstDoor();
      while( (porta != NULL) && (!pronto) )
      {
         boundingBox bound = porta->obj->getBoundingBox();
         GLfloat X[4]; GLfloat Z[4];
         X[0] = bound.x1;
         Z[0] = bound.z1;
         X[1] = bound.x1;
         Z[1] = bound.z2; 
         X[2] = bound.x2;
         Z[2] = bound.z2;
         X[3] = bound.x2;
         Z[3] = bound.z1;
         rotTransBoundingBox(porta->orientation + porta->delta, 
                             X, Z,porta->x, 
                             0.0,0.0,porta->z, minObj, maxObj);
         if(intercepts( minObj, maxObj, minMouse, maxMouse))
         {
            cursors->set(CURSOR_DOOR);
            shortcuts->setThing(gettext("Door")); 
            if( (mButton & SDL_BUTTON(1)) && 
                (rangeAction(activeCharacter->xPosition, 
                             activeCharacter->zPosition,
                             porta->x, porta->z,
                             WALK_PER_MOVE_ACTION) ) )
            {
               lastMousePression = time;
               if(porta->status)
               {
                  /* Is Closing Door */
                  //porta->orientation -= 90;
                  porta->status = DOOR_STATUS_CLOSED;
                  snd->addSoundEffect(porta->x, 
                                      actualMap->getHeight(porta->x, porta->z),
                                      porta->z, SOUND_NO_LOOP, 
                                      "sndfx/objects/door_close.ogg");
               }
               else
               {
                  /* Is Openning Door */
                  //porta->orientation += 90;
                  porta->status = DOOR_STATUS_OPENED;
                  snd->addSoundEffect(porta->x, 
                                      actualMap->getHeight(porta->x, porta->z),
                                      porta->z, SOUND_NO_LOOP, 
                                      "sndfx/objects/door_open.ogg");
               }
            }
            pronto = 1;
         }
         porta = porta->next;
      }

      /* Inventory Verification */
      int i;
      character* pers = (character*) PCs->getFirst();
      for(i = 0; ((i < PCs->getTotal()) && (!pronto)); i++)
      {
         GLfloat x[4],z[4];
         GLfloat min[3], max[3];
         x[0] = pers->min[0];
         z[0] = pers->min[2];
         x[1] = pers->min[0];
         z[1] = pers->max[2]; 
         x[2] = pers->max[0];
         z[2] = pers->max[2];
         x[3] = pers->max[0];
         z[3] = pers->min[2];

         rotTransBoundingBox(pers->orientation, x, z,pers->xPosition,0.0, 0.0,
                             pers->zPosition, min, max );

         if(intercepts( min, max, minMouse, maxMouse))
         {
            cursors->set(CURSOR_INVENTORY);
            shortcuts->setThing(pers->name); 

            /* Open Inventory when button pressed */
            if( (mButton & SDL_BUTTON(1)) && (!inventoryWindow))
            {
               openCloseInventoryWindow();
            }
            pronto = 1;
         }
         pers = (character*) pers->next;
      }

      /* Talk And Attack Events Verification */
      if(NPCs)
      {
         pers = (character*) NPCs->getFirst();
         for(i = 0; ((i < NPCs->getTotal()) && (!pronto)); i++)
         {
            GLfloat x[4],z[4];
            GLfloat min[3], max[3];

            x[0] = pers->min[0];
            z[0] = pers->min[2];
            x[1] = pers->min[0];
            z[1] = pers->max[2]; 
            x[2] = pers->max[0];
            z[2] = pers->max[2];
            x[3] = pers->max[0];
            z[3] = pers->min[2];

            rotTransBoundingBox(pers->orientation, x, z, 
                                pers->xPosition, 0.0, 0.0, 
                                pers->zPosition, min, max );

            if(intercepts( min, max, minMouse, maxMouse))
            {
               if( engineMode == ENGINE_MODE_REAL_TIME )
               {
                  if(!pers->isAlive())
                  {
                     cursors->set(CURSOR_GET);
                  }

                  /* The Character Dialog Window Call */
                  else if(pers->getConversationFile() != "")
                  {
                     cursors->set(CURSOR_TALK);
                     if( (mButton & SDL_BUTTON(1)) && 
                         (rangeAction(activeCharacter->xPosition, 
                                      activeCharacter->zPosition,
                                      pers->xPosition, pers->zPosition,
                                      WALK_PER_MOVE_ACTION)) )
                     {
                        dialogWindow dlgWindow;
                        dlgWindow.open(gui, activeCharacter, 
                                       (conversation*)pers->getConversation(), 
                                       pers->getPortraitFileName());
                     }
                  }
                  shortcuts->setThing(pers->name); 
                  pronto = 1;
               }
               /* Verify attacks */
               else if( (engineMode == ENGINE_MODE_TURN_BATTLE) && 
                        (activeCharacter->getCanAttack()) &&
                        (fightStatus == FIGHT_PC_TURN) )
               {
                  cursors->set(CURSOR_ATTACK);
                  shortcuts->setThing(pers->name); 

                  if( (mButton & SDL_BUTTON(1)) &&
                      (rangeAction(activeCharacter->xPosition, 
                                   activeCharacter->zPosition,
                                   pers->xPosition, pers->zPosition,
                                   activeCharacter->getActiveFeatRange() *
                                   METER_TO_DNT) ) )
                  {
                     sprintf(buf, gettext("%s attacks %s"),
                             activeCharacter->name.c_str(),
                             pers->name.c_str());
                     brief->addText(buf);
                     activeCharacter->setCanAttack(
                                             !activeCharacter->actualFeats.
                                                   applyAttackAndBreakFeat(
                                                          *activeCharacter,
                                                          attackFeat, pers));

                     fight->verifyDeads();

                     if( pers->getPsychoState() != PSYCHO_HOSTILE)
                     {
                        pers->setPsychoState(PSYCHO_HOSTILE);
                     }
                  }
                  pronto = 1;
               }
            }
            pers = (character*) pers->next;
         }
      }

      /* Map Connections Verification */
      if( ( quaux->mapConection.active ) && (!pronto) && 
          (engineMode == ENGINE_MODE_REAL_TIME) ) 
      {
         /* Don't travel on battle mode */
         GLfloat minCon[3], maxCon[3];
         minCon[0] = quaux->mapConection.x1;
         minCon[1] = 0.0;
         minCon[2] = quaux->mapConection.z1;
         maxCon[0] = quaux->mapConection.x2;
         maxCon[1] = 0.0;
         maxCon[2] = quaux->mapConection.z2;
         GLfloat minMouse[3], maxMouse[3];
         minMouse[0] = xReal-2;  maxMouse[0] = xReal+2;
         minMouse[1] = 0.0;      maxMouse[1] = 0.0;
         minMouse[2] = zReal-2;  maxMouse[2] = zReal+2;
         if( intercepts( minCon, maxCon, minMouse, maxMouse ) )
         {
            shortcuts->setThing(quaux->mapConection.mapName); 

            curConection = &quaux->mapConection;
            cursors->set(CURSOR_MAPTRAVEL);
            pronto = 1;
            if( (mButton & SDL_BUTTON(1)) && 
                (rangeAction(activeCharacter->xPosition, 
                             activeCharacter->zPosition,
                             xReal, zReal,
                             WALK_PER_MOVE_ACTION) ) )
            {
               loadMap(quaux->mapConection.mapName);
               return(1);
            }
         }
      }
      else
      {
         curConection = NULL;
      }

      if( (!pronto) )
      {
         shortcuts->setThing(gettext("Nothing")); 
      }
   }
   return(0);
}



/*********************************************************************
 *                   Threat Input/Output Events                      *
 *********************************************************************/
int engine::treatIO(SDL_Surface *screen)
{
   exitEngine = 0;           // Exit the engine ?
   bool walked = false;      // Character Walk ?
   bool timePass = false;    // The time to update passes ?
   bool run = false;         // Is to run, instead of walk ?
   float curWalkInterval = 0.0; // Current Active Character walk interval
   Uint32 time;              // Actual Time
   GLfloat varX, varZ;       // to avoid GLfloat calculate
   GLfloat dist;
   activeCharacter = PCs->getActiveCharacter();
   Uint8 *keys;
   int x,y;
   int guiEvent;
   Uint8 mButton;
   int i;

   GLfloat varTempo;  // Time Variation

   time = SDL_GetTicks();
   srand(time);
   varTempo = (time-lastRead);
   if( ((varTempo)) >= UPDATE_RATE)
   {
      timePass = true;

      /* Update Time */
      seconds = varTempo / 1000.0;

      hour = (hour + seconds / 100.0 );
      if(hour > 23.99)
      {
         hour = 0.0;
      }
      hourToTxt();
      lastRead = time;
      guiEvent = 0;

      SDL_PumpEvents();

      /* Get Keyboard State */
      keys = SDL_GetKeyState(NULL);

      /* Get Mouse State */
      mButton = SDL_GetMouseState(&x,&y);
      mouseX = x;
      mouseY = y;

      /* Get AlwaysRun from Options */
      run = option->getAlwaysRun();

      /* Verify if will enter the Battle Mode because of
       * enemies characters at range! */
      if( (engineMode != ENGINE_MODE_TURN_BATTLE) && (NPCs != NULL) &&
          (activeCharacter->isAlive()) )
      {
         character* npc = NPCs->getFirst();
         for(i = 0; ( (i < NPCs->getTotal()) && 
                      (engineMode != ENGINE_MODE_TURN_BATTLE)); i++)
         {
            /* Only Force Enter Battle Mode with Hostile Character */
            if( (npc->isAlive()) && (npc->getPsychoState() == PSYCHO_HOSTILE) )
            {
               /* Verify Distance */
               dist = sqrt( (npc->xPosition - activeCharacter->xPosition)*
                            (npc->xPosition - activeCharacter->xPosition) +
                            (npc->zPosition - activeCharacter->zPosition)*
                            (npc->zPosition - activeCharacter->zPosition));

               if(dist < 3*WALK_PER_MOVE_ACTION)
               {
                  //TODO -> do a perception check before enter!

                  /* If near, enter battle Mode  */
                  enterBattleMode(false);
               }
            }
            npc = npc->next;
         }
      }

      /* Verify if need to endTurn */
      if(option->getAutoEndTurn())
      {
         if( (engineMode == ENGINE_MODE_TURN_BATTLE) &&
             (fightStatus == FIGHT_PC_TURN) &&
             (!PCs->getActiveCharacter()->getCanAttack()) &&
             (!PCs->getActiveCharacter()->getCanMove()) )
         {
            /* Can't take more actions, so autoEnd the turn! */
            brief->addText(gettext("No more actions. Turn ended."));
            endTurn();
         }
      }

      /* Get Mouse Coordinates at world system */
      updateMouseWorldPos();

      if( (time-lastMouse >=  REFRESH_RATE ) || 
            ( (mButton & SDL_BUTTON(1) ) && 
              (time-lastMousePression >= REFRESH_RATE)) )
      {
         cursors->set(CURSOR_WALK);
         lastMouse = time;

         if(!gui->mouseOnGui(mouseX, mouseY))
         {
            if(verifyMouseActions(mButton) == 1)
            {
               /* Changed Map, so */
               return(1);
            }
         }
      }

      /* Keyboard Verification */

      /* Exit Engine */
      if( ( keys[SDLK_ESCAPE] ) && 
            ( (time-lastKeyb >= REFRESH_RATE) || 
              (lastKey != SDLK_ESCAPE) ) )
      {
         defineFrontSurface();
         lastKey = SDLK_ESCAPE;
         lastKeyb = time;
         return(0);
      }

      /* Screenshot */
      if( ( keys[SDLK_F12] ) && 
            ( (time-lastKeyb >= REFRESH_RATE) || 
              (lastKey != SDLK_F12) ) )
      {
         userInfo user;
         string screenFile = user.getUserHome() + "screenshot.bmp";
         screenshot(screenFile);
         lastKey = SDLK_F12;
         lastKeyb = time;
      }

      /* Enter Attack Mode or End Turn */
      if( (keys[SDLK_SPACE]) && 
            ( (time-lastKeyb >= REFRESH_RATE) || 
              (lastKey != SDLK_SPACE) ) )
      {
         if(engineMode != ENGINE_MODE_TURN_BATTLE)
         {
            enterBattleMode(true);
         }
         else if(fightStatus == FIGHT_PC_TURN)
         {
            endTurn();
         }
         lastKey = SDLK_SPACE;
         lastKeyb = time;
      }

      /* Enable / Disable The Range Draw */
      if( (keys[SDLK_r]) && 
            ( (time-lastKeyb >= REFRESH_RATE) || 
              (lastKey != SDLK_r) ) )
      {
         showRange = !showRange;
         lastKey = SDLK_r;
         lastKeyb = time;
      }

      /* Print All Models on List */
      if( (keys[SDLK_F2]) && 
            ( (time-lastKeyb >= REFRESH_RATE) || 
              (lastKey != SDLK_F2) ) )
      {
         lastKey = SDLK_F2;
         lastKeyb = time;
         models->printAll();
      }      

      /* Open Minimap */
      if( (keys[SDLK_m]) && 
            ( (time-lastKeyb >= REFRESH_RATE) || 
              (lastKey != SDLK_m) ) )
      {
         if(!mapWindow->isOpened())
         {
            mapWindow->open(gui, 0,0, actualMap);
         }
         else
         {
            mapWindow->close(gui);
         }

         lastKey = SDLK_m;
         lastKeyb = time;
      }

      /* Open ShortCuts */
      if( (keys[SDLK_n]) && 
            ( (time-lastKeyb >= REFRESH_RATE) || 
              (lastKey != SDLK_n) ) )
      {
         shortcuts->open(gui);
         lastKey = SDLK_n;
         lastKeyb = time;
      }

      /* Open Briefing */
      if( (keys[SDLK_l]) && 
            ( (time-lastKeyb >= REFRESH_RATE) || 
              (lastKey != SDLK_n) ) )
      {
         brief->openWindow(gui);
         lastKey = SDLK_l;
         lastKeyb = time;
      }

      /* Open Close Inventory */
      if( (keys[SDLK_i]) && 
            ( (time-lastKeyb >= REFRESH_RATE) || 
              (lastKey != SDLK_i) ) )
      {
         openCloseInventoryWindow(); 
         lastKey = SDLK_i;
         lastKeyb = time;
      }

      /* Open Character Info Window */
      if( (keys[SDLK_c]) && 
            ( (time-lastKeyb >= REFRESH_RATE) || 
              (lastKey != SDLK_c) ) ) 
      {
         lastKey = SDLK_c;
         lastKeyb = time;
         if(charInfoWindow)
         {
            charInfoWindow->open(PCs->getActiveCharacter());
         }
      }


      /* FIXME Remove all temporary tests from here */
      if( (keys[SDLK_y]) && 
            ( (time-lastKeyb >= REFRESH_RATE) || 
              (lastKey != SDLK_y) ) )
      {
         lastKey = SDLK_y;
         lastKeyb = time;
         if(!effect)
         {
            effect = (part2*)particleController.addParticle(PART_FIRE,
                  activeCharacter->xPosition,0,
                  activeCharacter->zPosition,
                  "particles/effect1.par");
            if(effect)
            {
               effect->setFollowPC(true);
            }
         }
         else
         {
            particleController.removeParticle(PART_FIRE, effect);
            effect = NULL;
         }
      }
      if( (keys[SDLK_t]) && 
            ( (time-lastKeyb >= REFRESH_RATE) || 
              (lastKey != SDLK_t) ) )
      {
         lastKey = SDLK_t;
         lastKeyb = time;
         particleController.addParticle(PART_METEOR,
               activeCharacter->xPosition,
               MAX_HEIGHT+100,
               activeCharacter->zPosition,
               0.0, -1.0, 0.0,
               activeCharacter->xPosition,
               actualMap->getHeight(
                  activeCharacter->xPosition,
                  activeCharacter->zPosition),
               activeCharacter->zPosition,
               "particles/fire1.par");
      }
      if( (keys[SDLK_u]) && 
            ( (time-lastKeyb >= REFRESH_RATE) || 
              (lastKey != SDLK_u) ) )
      {
         lastKey = SDLK_u;
         GLfloat incZ = -cos(deg2Rad(activeCharacter->orientation));
         GLfloat incX = -sin(deg2Rad(activeCharacter->orientation));
         particleController.addParticle(PART_METEOR,
               activeCharacter->xPosition,
               activeCharacter->yPosition + 15,
               activeCharacter->zPosition,
               2*incX, 0.0, 2*incZ,
               activeCharacter->xPosition + 800*incX,
               activeCharacter->yPosition + 15,
               activeCharacter->zPosition + 800*incZ,
               "particles/fire1.par");
      }
      if( (keys[SDLK_l]) && 
            ( (time-lastKeyb >= REFRESH_RATE) || 
              (lastKey != SDLK_l) ) )
      {
         lastKey = SDLK_l;
         lastKeyb = time;
         particleController.addParticle(PART_LIGHTNING,
               activeCharacter->xPosition,250,
               activeCharacter->zPosition,
               "particles/lightning1.par");
      }

      if( (keys[SDLK_0]) && 
            ( (time-lastKeyb >= REFRESH_RATE) || 
              (lastKey != SDLK_0) ) )
      {
         lastKey = SDLK_0;
         lastKeyb = time;
         hour += 0.1;
      }

      /* Toggle Run state */
      if( (keys[SDLK_LSHIFT]) || (keys[SDLK_RSHIFT]) )
      {
         run = !option->getAlwaysRun();
      }

      /* Define current walk interval */
      if(run)
      {
         curWalkInterval = activeCharacter->walk_interval * 
                           ENGINE_RUN_MULTIPLIER;
      }
      else
      {
         curWalkInterval = activeCharacter->walk_interval;
      }

      /* Keys to character's movimentation */
      if(keys[SDLK_q] || keys[SDLK_e])
      {
         walkStatus = ENGINE_WALK_KEYS;
          varX = curWalkInterval * 
                 sin(deg2Rad(activeCharacter->orientation+90.0));
          varZ = curWalkInterval * 
                 cos(deg2Rad(activeCharacter->orientation+90.0));
         // Left walk
         if(keys[SDLK_q]) 
         {
             varX *= -1;
             varZ *= -1;
         }

         walked |= tryWalk(varX, varZ);
        
      }
      else if(keys[SDLK_w] || keys[SDLK_s])
      { 
         walkStatus = ENGINE_WALK_KEYS;
         varX = curWalkInterval * 
                sin(deg2Rad(activeCharacter->orientation));
         varZ = curWalkInterval * 
                cos(deg2Rad(activeCharacter->orientation));
         if(keys[SDLK_w]) 
         {
              varX *= -1;
              varZ *= -1;
         }
         
         walked |= tryWalk(varX, varZ);
      }

      if( (keys[SDLK_a]) || (keys[SDLK_d]))
      {
         GLfloat ori = activeCharacter->orientation;
         walkStatus = ENGINE_WALK_KEYS;
         // CounterClockWise Character turn
         if((keys[SDLK_a]) && (canWalk(0,0,TURN_VALUE)) )  
         {
            ori += TURN_VALUE;
            if(ori > 360.0)
            { 
               ori -= 360.0;
            }
            activeCharacter->setOrientation(ori);
            walked = true;
         }
         // Clockwise Character Turn
         if((keys[SDLK_d]) && (canWalk(0,0,-TURN_VALUE)) )
         {
            ori -= TURN_VALUE;
            if(ori < 0.0)
            {
               ori += 360.0;
            }
            activeCharacter->setOrientation(ori);
         }
         walked = true;
      }
      if(keys[SDLK_TAB]) //Activate Character
      {
         walkStatus = ENGINE_WALK_KEYS;
         if(keys[SDLK_LCTRL]) //Previous Character
         {
            PCs->setActiveCharacter((character*)activeCharacter->previous);
         }
         else //Next Character
         {
            PCs->setActiveCharacter((character*)activeCharacter->next);
         }
         activeCharacter = PCs->getActiveCharacter();
         gameCamera.updateCamera(activeCharacter->xPosition,
                                    activeCharacter->yPosition,
                                    activeCharacter->zPosition,
                                    activeCharacter->orientation);
         SDL_Delay(100);
      }

      /* Camera Verification */
      gameCamera.doIO(keys, mButton, x, y, DELTA_CAMERA );

      /* Set press time, if needed */
      if( (mButton & SDL_BUTTON(3)) && (!gui->mouseOnGui(x,y)) && 
          (walkPressTime == 0) && (walkStatus != ENGINE_WALK_MOUSE) )
      {
         walkPressTime = time;
      }

      /* Verify, if we got a continuous mouse action or a A* init */
      if( ( time - walkPressTime >= ENGINE_WALK_ACTION_DELAY ) &&
          ( walkPressTime != 0) && (mButton & SDL_BUTTON(3)) )
      {
            /* Continuous walk */
            walkStatus = ENGINE_WALK_MOUSE;
            walkPressTime = 0;
      }
      else if ( (walkPressTime != 0) && !(mButton & SDL_BUTTON(3)) )
      {
         /* Path Verification */
         walkPressTime = 0;

         activeCharacter->pathFind.defineMap(actualMap);

         if(option->getAlwaysRun())
         {
            activeCharacter->pathFind.findPath(activeCharacter, xReal, zReal, 
                  activeCharacter->walk_interval * ENGINE_RUN_MULTIPLIER, 
                  NPCs, PCs, 
                  engineMode == ENGINE_MODE_TURN_BATTLE);
         }
         else
         {
            activeCharacter->pathFind.findPath(activeCharacter, xReal, zReal, 
                  activeCharacter->walk_interval, NPCs, PCs, 
                  engineMode == ENGINE_MODE_TURN_BATTLE);
         }
      }

      /* Verify Continuous Walk with Mouse */
      if(walkStatus == ENGINE_WALK_MOUSE)
      {
         cursors->set(CURSOR_WALK_CONT);
         if(mButton & SDL_BUTTON(3))
         {
            /* Set character orientation (if mouse is far from character,
             * because if it is too near, some weird angles appears) */
            dist = sqrt( (xReal - activeCharacter->xPosition) *
                         (xReal - activeCharacter->xPosition) +
                         (zReal - activeCharacter->zPosition) *
                         (zReal - activeCharacter->zPosition) );
            
            walkAngle = getAngle(activeCharacter->xPosition,
                                 activeCharacter->zPosition,
                                 xReal, zReal);
            if(dist > 8)
            {
               /* Try to change the angle */
               if(canWalk(0,0, walkAngle - activeCharacter->orientation))
               { 
                  /* can change */
                  activeCharacter->orientation = walkAngle; 
               }

               /* Verify if is running or walking */
               run = dist >= ENGINE_CONTINUOUS_RUN_DISTANCE;
            }
            else
            {
               /* Keep the direction angle */
               walkAngle = activeCharacter->orientation;
               run = false;
            }

            /* Reset, now for the continuous walk, the interval */
            if(run)
            {
               curWalkInterval = activeCharacter->walk_interval * 
                                 ENGINE_RUN_MULTIPLIER;
            }
            else
            {
               curWalkInterval = activeCharacter->walk_interval;
            }

            /* Try to move it forward the angle */
            varX = -1 * curWalkInterval * sin(deg2Rad(walkAngle));
            varZ = -1 * curWalkInterval * cos(deg2Rad(walkAngle));
            walked |= tryWalk(varX, varZ);
         }
         else
         {
            /* Move Ends */
            walkStatus = ENGINE_WALK_KEYS;
            cursors->set(CURSOR_WALK);
         }
      }

      /* Verify if found path in aStar */
      if(activeCharacter->pathFind.getState() == ASTAR_STATE_FOUND)
      {
         //Found path to, so walk
         walkStatus = ENGINE_WALK_MOUSE_ASTAR;
      }

      if(walkStatus == ENGINE_WALK_MOUSE_ASTAR)
      {
            if(! activeCharacter->pathFind.getNewPosition(
                                        activeCharacter->xPosition,
                                        activeCharacter->zPosition,
                                        activeCharacter->orientation,
                                        engineMode == ENGINE_MODE_TURN_BATTLE))
            {
               walkStatus = ENGINE_WALK_KEYS;
            }
            else
            {
               /* Define New Occuped Square */
               int posX =(int)floor(activeCharacter->xPosition /
                                    actualMap->squareSize());
               int posZ =(int)floor(activeCharacter->zPosition / 
                                    actualMap->squareSize());
               activeCharacter->ocSquare = 
                                         actualMap->relativeSquare(posX,posZ);
               /* Define New Height */
               defineCharacterHeight(activeCharacter, 
                                     activeCharacter->xPosition,
                                     activeCharacter->zPosition);
            }

            gameCamera.updateCamera(activeCharacter->xPosition,
                                    activeCharacter->yPosition,
                                    activeCharacter->zPosition,
                                    activeCharacter->orientation);
            walked = true;
      }

      /* IA cycle */
      treatScripts();

      /* A* cycle */
      doAStar();

      /* GUI Events */

      /* Update MiniMap */
      mapWindow->updateCharacterPosition(activeCharacter->xPosition,
                                         activeCharacter->zPosition);

      /* Get GUI Event */ 
      guiObject* object;
      object = gui->manipulateEvents(x,y,mButton,keys, guiEvent);
      /* Threat the GUI */
      treatGuiEvents(object, guiEvent);


      
      /* Verify Mouse Cursor Forbidden (when can't go to position) */
      if(!gui->mouseOnGui(mouseX, mouseY))
      {
         int posX = (int) floor(xReal / actualMap->squareSize());
         int posZ = (int) floor(zReal / actualMap->squareSize());
         Square* sq = actualMap->relativeSquare(posX, posZ);
         if( (sq == NULL) || (sq->flags == 0))
         {
            cursors->set(CURSOR_FORBIDDEN);
         }
      }
   
      /* Draw things */
      if(shadowMap.isEnable())
      {
         drawWithShadows();
      }
      else
      {
         drawWithoutShadows();
      }

      /* Update FPS */
      actualFPS = (actualFPS + (1000.0 / (SDL_GetTicks() - lastRead))) / 2;

      /* Limit its max */
      if(actualFPS > 1000)
      {
         actualFPS = 1000;
      }

      /* Show it! */
      if( time-lastFPS >= 500 )
      {
         lastFPS = time;
         shortcuts->setFPS(actualFPS);
         shortcuts->setParticlesNumber(particleController.numParticles());
      }
      
      /* Verify Sounds FIXME -> for npc sounds! */
      if( (walked) && (activeCharacter->isAlive()) )
      {
         /* Make sure no conversation is opened */
         dialogWindow dlgWindow;
         dlgWindow.close();

         if( (!walkSound) && (snd) )
         {
            string walkSoundFile = "sndfx/passos.ogg";

            if(actualMap->isOutdoor())
            {
               walkSoundFile = "sndfx/passos_terra.ogg";
            }

            walkSound = snd->addSoundEffect(activeCharacter->xPosition,0.0,
                                            activeCharacter->zPosition,
                                            SOUND_AUTO_LOOP,
                                            walkSoundFile);
         }
         else if(walkSound)
         {
            walkSound->redefinePosition(activeCharacter->xPosition, 0.0,
                                        activeCharacter->zPosition);
         }

         /* Set the animation (if not yet defined) */
         if(run)
         {
            activeCharacter->setState(STATE_RUN);
         }
         else
         {
            activeCharacter->setState(STATE_WALK);
         }
      }
      else if( (timePass) && (activeCharacter->isAlive()))
      { 
         /* The move stoped (or never occurred)  */
         if( ( (activeCharacter->getState() == STATE_WALK) ||
               (activeCharacter->getState() == STATE_RUN) ) &&
               (engineMode == ENGINE_MODE_TURN_BATTLE) && 
               (fightStatus == FIGHT_PC_TURN) )
         {
            /* Stoped, so must set that cannot move */
            activeCharacter->setCanMove(false);

            /* And verify if can attack */
            if(walkDistance > WALK_PER_MOVE_ACTION)
            {
               activeCharacter->setCanAttack(false);
            }
            walkDistance = 0;
         }

         /* Put at Idle animation */
         activeCharacter->setState(STATE_IDLE);

         /* And remove walk sound effect */
         if(walkSound)
         {
            snd->removeSoundEffect(walkSound);
            walkSound = NULL;
         }
      }

   }
   else
   {
      int tmp = (int) ((UPDATE_RATE-1) - varTempo);
      if( (tmp > 0) && (tmp < UPDATE_RATE))
      {
         SDL_Delay(tmp);
      }
   }

   return(!exitEngine);
}

/********************************************************************
 *                            RenderScene                           *
 ********************************************************************/
void engine::renderScene(bool lightPass, bool updateAnimations)
{
   GLfloat min[3],max[3];
   GLfloat x[4],z[4];

   bool shadow = false;//actualMap->isOutdoor() && gameSun->visibleTime();

   /* SKY */
   if( (!lightPass) && (actualMap->isOutdoor()) )
   {
      glPushMatrix();
         glTranslatef(gameCamera.getCameraX(), 0.0, gameCamera.getCameraZ());
         gameSky->draw(actualMap, gameSun->getRotation());
      glPopMatrix();
   }

   glPushMatrix();

   if(!lightPass)
   {
      /* Draw The Floor with Stencil Buffer */
      if( ((option->getReflexionType() != REFLEXIONS_NONE) && 
               (!actualMap->isOutdoor()))  || (shadow))
      {
         glDisable(GL_DEPTH_TEST);
         glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
         glEnable(GL_STENCIL_TEST);
         glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
         glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
         actualMap->renderFloor(gameCamera.getCameraX(),gameCamera.getCameraY(),
               gameCamera.getCameraZ(), visibleMatrix, true);
         glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
         glEnable(GL_DEPTH_TEST);
         glDisable(GL_STENCIL_TEST);
      }
   }

   /* Draw Playable Characters (PCs) */
   character* per = (character*) PCs->getFirst();
   int aux;

   for(aux=0;aux < PCs->getTotal();aux++)
   {
      /* Update the model */
      if(updateAnimations)
      {
         per->update(WALK_UPDATE);
      }

      /* Load the Model */
      per->loadToGraphicMemory();

      /* Draw Character */
      glPushMatrix();
      glTranslatef(per->xPosition, per->yPosition,
            per->zPosition);
      glRotatef(per->orientation,0,1,0);
      per->renderFromGraphicMemory();

      //per->renderBoundingBox();
      /*glColor3f(1.0,0.1,0.1);
        glBegin(GL_QUADS);
        glVertex3f(per->min[0],per->min[1]+1,per->min[2]);
        glVertex3f(per->min[0],per->min[1]+1,per->max[2]);
        glVertex3f(per->max[0],per->min[1]+1,per->max[2]);
        glVertex3f(per->max[0],per->min[1]+1,per->min[2]);
        glEnd();*/
      glPopMatrix();

      /* Draw Reflection */
      if( (option->getReflexionType() >= REFLEXIONS_CHARACTERS) && 
            (!actualMap->isOutdoor()) )
      {
         glEnable(GL_STENCIL_TEST);
         glStencilFunc(GL_EQUAL, 1, 0xffffffff);  /* draw if ==1 */
         glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
         glEnable(GL_NORMALIZE);

         //glCullFace(GL_FRONT);

         glPushMatrix();
         glTranslatef(per->xPosition, per->yPosition,
               per->zPosition);
         glRotatef(per->orientation,0,1,0);
         glScalef(1.0, -1.0, 1.0);
         per->renderFromGraphicMemory();
         glPopMatrix();
         glDisable(GL_NORMALIZE);
         //glCullFace(GL_FRONT);
         glDisable(GL_STENCIL_TEST);
      }

      /* Draw Projective Shadow */
      if( (shadow) && (gameSun->visibleTime()) )
      {
         per->renderShadow(gameSun->getShadowMatrix());
      }

      /* Unload Model Graphics Memory */
      per->removeFromGraphicMemory();

      per = (character*) per->next;
   }
   glPopMatrix();

   /* Draw the NPCs */
   if(NPCs)
   {
      per = (character*) NPCs->getFirst();
      for(aux=0;aux < NPCs->getTotal();aux++)
      {
         /* Update the model */
         if(updateAnimations)
         {
            per->update(WALK_UPDATE);
         }
 
         /* Load the Model */
         per->loadToGraphicMemory();

         /* Verify Bounding Box */
         x[0] = per->min[0];
         z[0] = per->min[2];
         x[1] = per->min[0];
         z[1] = per->max[2]; 
         x[2] = per->max[0];
         z[2] = per->max[2];
         x[3] = per->max[0];
         z[3] = per->min[2];
         rotTransBoundingBox(per->orientation, x, z,per->xPosition, 
                             -per->max[1] + per->yPosition, //To get reflection
                             per->max[1] + per->yPosition,
                             per->zPosition, min, max );

         /* Only Draw Visible Characters */
         if(visibleCube(min[0],min[1],min[2],max[0],max[1],max[2],
                        visibleMatrix))
         {
            glPushMatrix();
              glTranslatef(per->xPosition, per->yPosition,
                           per->zPosition);
              glRotatef(per->orientation,0,1,0);
              per->renderFromGraphicMemory();
              /*glColor3f(1.0,0.1,0.1);
              glBegin(GL_QUADS);
                 glVertex3f(per->min[0],per->min[1]+1,per->min[2]);
                 glVertex3f(per->min[0],per->min[1]+1,per->max[2]);
                 glVertex3f(per->max[0],per->min[1]+1,per->max[2]);
                 glVertex3f(per->max[0],per->min[1]+1,per->min[2]);
              glEnd();*/
            glPopMatrix();

              //per->RenderBoundingBox();
              /*glDisable(GL_LIGHTING);
              glColor3f(0.6,0.1,0.1);
              glBegin(GL_POLYGON);
                 glVertex3f(per->min[0],per->min[1]+1,per->min[2]);
                 glVertex3f(per->min[0],per->min[1]+1,per->max[2]);
                 glVertex3f(per->max[0],per->min[1]+1,per->max[2]);
                 glVertex3f(per->max[0],per->min[1]+1,per->min[2]);
              glEnd();
              glEnable(GL_LIGHTING);*/

              /* Draw Reflection */
              if( (option->getReflexionType() >= REFLEXIONS_CHARACTERS) && 
                  (!actualMap->isOutdoor()) )
              {
                 glEnable(GL_STENCIL_TEST);
                 glStencilFunc(GL_EQUAL, 1, 0xffffffff);  /* draw if ==1 */
                 glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
                 //glCullFace(GL_FRONT);
                 glEnable(GL_NORMALIZE);
                 glPushMatrix();
                    glTranslatef(per->xPosition, per->yPosition,
                                 per->zPosition);
                    glRotatef(per->orientation,0,1,0);
                    glScalef(1.0, -1.0, 1.0);
                    per->renderFromGraphicMemory();
                 glPopMatrix();
                 glDisable(GL_NORMALIZE);
                 //glCullFace(GL_FRONT);
                 glDisable(GL_STENCIL_TEST);
              }

              /* Draw Projective Shadow */
              if( (shadow) && (gameSun->visibleTime()) )
              {
                 per->renderShadow(gameSun->getShadowMatrix());
              }
         }
         /* Remove the Model From Graphic Memory */
         per->removeFromGraphicMemory();

         per = (character*) per->next;
      }
   }

   /* Draw the Map Objects with Reflexions */
   if( (option->getReflexionType() >= REFLEXIONS_ALL) && 
       (!actualMap->isOutdoor()) )
   {
      glEnable(GL_STENCIL_TEST);
      glStencilFunc(GL_EQUAL, 1, 0xffffffff);  /* draw if ==1 */
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
      glEnable(GL_NORMALIZE);
      glPushMatrix();
        actualMap->renderObjects(gameCamera.getCameraX(),
                                 gameCamera.getCameraY(),
                                 gameCamera.getCameraZ(), visibleMatrix, true);
      glPopMatrix();
      glDisable(GL_NORMALIZE);
      glDisable(GL_STENCIL_TEST);
   }

   models->renderSceneryObjects(visibleMatrix,
                                (option->getReflexionType() >= REFLEXIONS_ALL) 
                                 && (!actualMap->isOutdoor()),
                                 shadow?gameSun->getShadowMatrix():NULL);

   glPushMatrix();
   actualMap->render(gameCamera.getCameraX(), gameCamera.getCameraY(),
                     gameCamera.getCameraZ(), visibleMatrix,
                     PCs->getActiveCharacter()->xPosition,
                     PCs->getActiveCharacter()->zPosition);
   glPopMatrix();
}

/********************************************************************
 *                       RenderNoShadowThings                       *
 ********************************************************************/
void engine::renderNoShadowThings()
{

   activeCharacter = PCs->getActiveCharacter(); 

   /* Draw Path */
   /*if(walkStatus == ENGINE_WALK_MOUSE_ASTAR)
   {
      activeCharacter->pathFind.drawPath();
   }*/

   /* The SUN or MOON */
   if(actualMap->isOutdoor())
   {
      if(!actualMap->getFog().enabled)
      {
         glDisable(GL_FOG);
      }
      gameSun->drawSun();
      if(!actualMap->getFog().enabled)
      {
         glEnable(GL_FOG);
      }
   }

   if( showRange )
   {
       /* Range Circle */
       actualMap->renderSurfaceOnMap(rangeCircle,
                                     activeCharacter->xPosition - 
                                                           WALK_PER_MOVE_ACTION,
                                     activeCharacter->zPosition - 
                                                           WALK_PER_MOVE_ACTION,
                                     activeCharacter->xPosition + 
                                                           WALK_PER_MOVE_ACTION,
                                     activeCharacter->zPosition + 
                                                           WALK_PER_MOVE_ACTION,
                                     0.05, 20);
   }

   /* Draw Combat Mode Things */
   if( (engineMode == ENGINE_MODE_TURN_BATTLE) && 
         ( (fightStatus == FIGHT_PC_TURN) || 
           ( (fightStatus == FIGHT_NPC_TURN) && 
             (option->getShowEnemyCircles()) )))
   {
      character* turnCharacter = fight->actualCharacterTurn();
      if(!turnCharacter)
      {
         turnCharacter = activeCharacter;
      }
      /* Draw Movimentation Circles */
      if(turnCharacter->getCanMove())
      {
         /* Full Circle */
         if(turnCharacter->getCanAttack())
         {
            actualMap->renderSurfaceOnMap(fullMoveCircle,
                  moveCircleX-2*WALK_PER_MOVE_ACTION,
                  moveCircleZ-2*WALK_PER_MOVE_ACTION,
                  moveCircleX+2*WALK_PER_MOVE_ACTION, 
                  moveCircleZ+2*WALK_PER_MOVE_ACTION,
                  0.1,12);
         }
         /* Normal Circle */
         actualMap->renderSurfaceOnMap(normalMoveCircle,
               moveCircleX-WALK_PER_MOVE_ACTION,
               moveCircleZ-WALK_PER_MOVE_ACTION,
               moveCircleX+WALK_PER_MOVE_ACTION, 
               moveCircleZ+WALK_PER_MOVE_ACTION,
               0.2,20);
      }
      if( (turnCharacter->getCanAttack()) && 
          (walkDistance < WALK_PER_MOVE_ACTION) )
      {
         /* Feat Range Circle */
         float rangeValue = turnCharacter->getActiveFeatRange()*METER_TO_DNT;
         actualMap->renderSurfaceOnMap(featRangeCircle, 
               turnCharacter->xPosition-rangeValue,
               turnCharacter->zPosition-rangeValue, 
               turnCharacter->xPosition+rangeValue, 
               turnCharacter->zPosition+rangeValue, 
               0.3,20);
      }

   }

   /* Draw A* movimentation things */
   if(walkStatus == ENGINE_WALK_MOUSE_ASTAR)
   {
      GLfloat destX =0, destZ=0;
      //activeCharacter->pathFind.drawPath();
      activeCharacter->pathFind.getDestiny(destX, destZ);
      
       /* Draw Movimentation Destiny */
       if(destinyVariation >= 2.0)
       {
          destinyVariation = -2.0;
       }
       destinyVariation += 0.1;

       actualMap->renderSurfaceOnMap(destinyImage,
                                     destX - (4 + destinyVariation), 
                                     destZ - (4 + destinyVariation),
                                     destX + (4 + destinyVariation),
                                     destZ + (4 + destinyVariation),
                                     0.25,4);
   }

   /* The Current Connection */
   if(curConection)
   {
      GLfloat ambient[] = { 0.94, 0.292, 0.22, 0.45 };
      glPushMatrix();
      glEnable( GL_BLEND );
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
      glBegin(GL_QUADS);
         glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
         glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ambient);
         glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ambient);
         glColor4fv(ambient);
         glNormal3f(0,1,0);
         glVertex3f(curConection->x1, 0.5, curConection->z1);
         glVertex3f(curConection->x1, 0.5, curConection->z2);
         glVertex3f(curConection->x2, 0.5, curConection->z2);
         glVertex3f(curConection->x2, 0.5, curConection->z1);
      glEnd();

      glDisable( GL_BLEND );
      glColor3f(1.0,1.0,1.0);
      glPopMatrix();
   }

   /* Draw Particles */
   if(option->getEnableParticles())
   {
      glPushMatrix();
         particleController.updateAll(activeCharacter->xPosition,
                                   activeCharacter->yPosition,
                                   activeCharacter->zPosition, 
                                   visibleMatrix, option->getEnableGrass());
      glPopMatrix();
   }

}

/********************************************************************
 *                             RenderGUI                            *
 ********************************************************************/
void engine::renderGUI()
{
   /* Draw the GUI */   
   glPushMatrix();
      draw2DMode();

      /* World Messages */
      msgController->draw(modl, proj, viewPort);

      glDisable(GL_LIGHTING);
      glDisable(GL_FOG);

      glDisable(GL_DEPTH_TEST);
      glDisable(GL_BLEND);
      /* Player's Portrait */
      PCs->getActiveCharacter()->drawMainPortrait();

      glPushMatrix();
         gui->draw(proj,modl,viewPort);
      glPopMatrix();

      /* Mouse Cursor */
      glPushMatrix();
         if(walkStatus == ENGINE_WALK_MOUSE)
         {
            /* Set the cursor to the current walkAngle orientation */
            if(PCs->getActiveCharacter()->getState() == STATE_RUN)
            {
               /* Is running, must scale a bit the cursor to demonstrate this */
               cursors->draw(mouseX, mouseY,
                             walkAngle - gameCamera.getPhi(),
                             1.0f,1.2f,1.0f);
            }
            else
            {
               cursors->draw(mouseX, mouseY,
                             walkAngle - gameCamera.getPhi());
            }
         }
         else
         {
            /* Keep cursor */
            cursors->draw(mouseX, mouseY);
         }
      glPopMatrix();

      /* Rdefine 3D Mode */
      if(actualMap->isOutdoor())
      {
         draw3DMode(option->getFarViewFactor()*OUTDOOR_FARVIEW);
      }
      else
      {
         draw3DMode(INDOOR_FARVIEW);
      }

   glPopMatrix();
   
   /* Renable things */
   glEnable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_FOG);
}

/*********************************************************************
 *                         drawWithShadow                            *
 *********************************************************************/
void engine::drawWithShadows()
{
   // FIXME: shadows for indoor maps
   GLfloat sunPos[4];

   /* Clear and update things */
   updateBeforeRender();

   /* Define Light View Projetion */
   gameSun->getPosition(sunPos);
   shadowMap.defineLightView(sunPos[0], sunPos[1], sunPos[2],
                       (actualMap->squareSize()*actualMap->getSizeX())/2.0, 
                       (actualMap->squareSize()*actualMap->getSizeZ())/2.0);
   
   /* Update frustum for light view */
   updateFrustum(visibleMatrix,proj,modl);
   
   /* Render the Scene from light view */
   shadowMap.beginLightRender();
   renderScene(true, true);
   shadowMap.endLightRender();

   /* Now set to the camera view and render again! */
   glViewport (0, 0, SCREEN_X, SCREEN_Y);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, SCREEN_X / (float)SCREEN_Y, 1.0, 
                  OUTDOOR_FARVIEW);
   glGetIntegerv(GL_VIEWPORT, viewPort);
   glGetFloatv(GL_MODELVIEW_MATRIX, camProj);
   glMatrixMode (GL_MODELVIEW);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   glLoadIdentity();
   gameCamera.lookAt(actualMap);
   updateFrustum(visibleMatrix,proj,modl);

   /* Render Things with shadow */
   shadowMap.beginShadowRender();
   renderScene(false, false);
   shadowMap.endShadowRender();

   /* Render remaining things (and GUI) */
   renderNoShadowThings();
   renderGUI();
   
   /* Flush */
   glFlush();
   SDL_GL_SwapBuffers();

   printOpenGLErrors();
}

/*********************************************************************
 *                       Draw Scene Function                         *
 *********************************************************************/
void engine::drawWithoutShadows()
{
   /* Clear and update things before render */
   updateBeforeRender();
   updateFrustum(visibleMatrix,proj,modl);
   
   /* Render all things */
   renderScene(false, true);
   renderNoShadowThings();

   renderGUI();

   /* Flush */
   glFlush();
   SDL_GL_SwapBuffers();

   printOpenGLErrors();
}

/*********************************************************************
 *                          updateBeforeRender                       *
 *********************************************************************/
void engine::updateBeforeRender()
{
   glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   glLoadIdentity();

   /* Redefine camera position */
   gameCamera.lookAt(actualMap);

   snd->setListenerPosition(gameCamera.getCameraX(), gameCamera.getCameraY(),
                            gameCamera.getCameraZ(), gameCamera.getTheta(),
                            gameCamera.getPhi(), gameCamera.getD(),
                            gameCamera.getDeltaY());

   /* Sun Definition */
   if(actualMap->isOutdoor())
   {
      gameSun->updateHourOfDay(hour, 
            (actualMap->getSizeX()/2.0)*actualMap->squareSize(), 
            (actualMap->getSizeZ()/2.0)*actualMap->squareSize());
      
      //PCs->getActiveCharacter()->xPosition,
      //                         PCs->getActiveCharacter()->zPosition);
      gameSun->setLight();
   }
   else
   {
      gameSun->disableLight();
   }
}

/*********************************************************************
 *                              tryWalk                              *
 *********************************************************************/
bool engine::tryWalk(GLfloat varX, GLfloat varZ)
{
   activeCharacter = PCs->getActiveCharacter();
   
   /* Try Normal Move */
   if(canWalk(varX,varZ,0)) 
   {
      activeCharacter->xPosition += varX;
      activeCharacter->zPosition += varZ;
      gameCamera.updateCamera(activeCharacter->xPosition,
            activeCharacter->yPosition,
            activeCharacter->zPosition,
            activeCharacter->orientation);
      return(true);
   }

   /* Can't, so try only X move */
   else if(((varX > 0)&&(canWalk(activeCharacter->walk_interval,0,0))) ||
           ((varX < 0)&&(canWalk(-activeCharacter->walk_interval,0,0))) )
   {
      if(varX < 0)
      {
         activeCharacter->xPosition -= activeCharacter->walk_interval;
      }
      else
      {
         activeCharacter->xPosition += activeCharacter->walk_interval;
      }
      gameCamera.updateCamera(activeCharacter->xPosition,
            activeCharacter->yPosition,
            activeCharacter->zPosition,
            activeCharacter->orientation);
      return(true);
   }

   /* Can't too, so try only Z move */
   else if(((varZ > 0) && canWalk(0,activeCharacter->walk_interval,0)) ||
           ((varZ < 0) && canWalk(0,-activeCharacter->walk_interval,0)) )
   {
      if(varZ < 0)
      {
         activeCharacter->zPosition -= activeCharacter->walk_interval;
      }
      else
      {
         activeCharacter->zPosition += activeCharacter->walk_interval;
      }

      gameCamera.updateCamera(activeCharacter->xPosition,
            activeCharacter->yPosition,
            activeCharacter->zPosition,
            activeCharacter->orientation);
      return(true);
   }

   /* Can't move! */
   return(false);
}

/*********************************************************************
 *                              canWalk                              *
 *********************************************************************/
bool engine::canWalk(GLfloat varX, GLfloat varZ, GLfloat varAlpha)
{
   bool result = true;
   GLfloat varHeight = 0;
   GLfloat nx, nz;
   activeCharacter = PCs->getActiveCharacter();
  
   if(!activeCharacter->isAlive())
   {
      /* Dead Characters can't walk too! */
      return(false);
   }
   else if((engineMode == ENGINE_MODE_TURN_BATTLE) && 
           (fightStatus != FIGHT_PC_TURN))
   {
       /* In turn mode, and not character's turn. */
       return(false);
   }
   else if((engineMode == ENGINE_MODE_TURN_BATTLE) && 
           (fightStatus == FIGHT_PC_TURN))
   {
      if(!activeCharacter->getCanMove())
      {
         /* Already Moved */
         return(false);
      }

      //verify distance to the orign point
      walkDistance = sqrt( (activeCharacter->xPosition + varX - moveCircleX) *
                           (activeCharacter->xPosition + varX - moveCircleX) +
                           (activeCharacter->zPosition + varZ - moveCircleZ) *
                           (activeCharacter->zPosition + varZ - moveCircleZ) );

      /* Verify walk limits at FightMode */
      if( ( (activeCharacter->getCanAttack()) && 
            (walkDistance > 2*WALK_PER_MOVE_ACTION) ) || 
          ( (!activeCharacter->getCanAttack()) &&
            (walkDistance > WALK_PER_MOVE_ACTION) )  )
      {
         return(false);
      }
   }


   colisionDetect.defineMap(actualMap, NPCs, PCs);
   result = colisionDetect.canWalk(activeCharacter, varX, 0, varZ, varAlpha, 
                                   varHeight, nx, nz);

   if(result)
   {
      /* Define New Occuped Square */
      int posX =(int)floor( nx / (actualMap->squareSize()));
      int posZ =(int)floor( nz / (actualMap->squareSize()));
      activeCharacter->ocSquare = actualMap->relativeSquare(posX,posZ);

      /* Define New Heigh */
      if(!defineCharacterHeight(activeCharacter, nx, nz))
      {
         /* Can't define new height or too much
          * to up or down, so can't move */
         return(false);
      }

      /* Apply VarHeight */
      activeCharacter->yPosition += varHeight;
   }
   
   return(result);
}

/*********************************************************************
 *                      defineCharacterHeight                  *
 *********************************************************************/
bool engine::defineCharacterHeight(character* c, GLfloat nx, GLfloat nz)
{
   return(actualMap->defineThingHeight(c, nx, nz));
}

/*********************************************************************
 *                      OpenCloseInventoryWindow                     *
 *********************************************************************/
void engine::openCloseInventoryWindow()
{
   if(!inventoryWindow)
   {
      inventoryWindow=new inventWindow(PCs->getActiveCharacter(),
                                       gui, infoWindow); 
   }
   else
   {
      delete(inventoryWindow);
      inventoryWindow = NULL;
   }
}

/*********************************************************************
 *                        updateAllHealthBars                        *
 *********************************************************************/
void engine::updateAllHealthBars()
{
   int i;
   character* pers = (character*) PCs->getFirst();
   for(i = 0; i < PCs->getTotal(); i++)
   {
      pers->setLifePoints(pers->getLifePoints());
      pers = (character*) pers->next;
   }
}

/*********************************************************************
 *                             showImage                             *
 *********************************************************************/
void engine::showImage(string fileName)
{
   GLuint id;
   Uint8 mButton = 0;
   SDL_Surface* img = IMG_Load(fileName.c_str()); 
   glDisable(GL_LIGHTING);
   glGenTextures(1,&id);
   setTextureRGBA(img,id);
   SDL_FreeSurface(img);

   fadeInTexture(id,0,0,SCREEN_X-1,SCREEN_Y-1, 800, 600);

   /* Wait until Mouse Button pressed */
   while(!(mButton & SDL_BUTTON(1)))
   {
      //Wait for Mouse Button Release
      SDL_PumpEvents();
      int x,y;
      mButton = SDL_GetMouseState(&x,&y);

      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT
              | GL_STENCIL_BUFFER_BIT);
      draw2DMode();
      glColor3f(1.0, 1.0, 1.0);
      textureToScreen(id, 0, 0, SCREEN_X-1, SCREEN_Y-1, 800, 600);
      draw3DMode(option->getFarViewFactor()*OUTDOOR_FARVIEW);
      glFlush();
      SDL_GL_SwapBuffers();

      SDL_Delay(50);
   }

   fadeOutTexture(id,0,0,SCREEN_X-1,SCREEN_Y-1, 800, 600);
   
   glEnable(GL_LIGHTING);
   glDeleteTextures(1,&id);
}

/*********************************************************************
 *                          getFightSystem                           *
 *********************************************************************/
fightSystem* engine::getFightSystem()
{
   return(fight);
}

/*********************************************************************
 *                          getCurrentMap                            *
 *********************************************************************/
Map* engine::getCurrentMap()
{
   return(actualMap);
}

/*********************************************************************
 *                          getModelList                             *
 *********************************************************************/
modelList* engine::getModelList()
{
   return(models);
}

/*********************************************************************
 *                          getWeaponTypes                           *
 *********************************************************************/
weaponTypes* engine::getWeaponTypes()
{
   return(weaponsTypes);
}

/*********************************************************************
 *                          Runs the Engine                          *
 *********************************************************************/
int engine::run(SDL_Surface *surface, bool commingBack)
{
   /* Put back map's sound and music if comming back to game */
   if(commingBack)
   {
      /* Reload the map's music */
      if(!actualMap->getMusicFileName().empty())
      {
         snd->loadMusic(actualMap->getMusicFileName());
      }

      /* Reload the map's sound effects */
      actualMap->flushSounds();
   }

   int time;
   actualFPS = 10.0;
   lastFPS = 0;

   /* Unhide All Windows */
   gui->showAll();

   /* Open Needed windows */
   if(!commingBack)
   {
      mapWindow->open(gui,0,0, actualMap);
      shortcuts->open(gui);
      brief->openWindow(gui);
   }

   /* Main Things Run */
   while(treatIO(surface))
   {

      /* Wait Die Animation */
      if( (engineMode == ENGINE_MODE_WAIT_DIE_ANIMATION) && 
          ( (SDL_GetTicks() - waitDieTimer) >= ENGINE_WAIT_DIE_DELAY) )
      {
         /* All Pcs are Dead, so Death Screen! */
         snd->loadMusic("music/musica8.ogg");
         showImage(dir.getRealFile("texturas/fightMode/death.png"));

         /* Remove All Sound Effects */
         snd->removeAllSoundEffects();

         /* Clear Modifications */
         modifState.clear();
         engineMode = ENGINE_MODE_REAL_TIME;
         gui->closeAllWindows();

         /* Clear the Inventory */
         PCs->getActiveCharacter()->newInventory();

         /* Clear the Actual Map */
         if(actualMap)
         {
            delete(actualMap);
            actualMap = NULL;
         }

         /* Clear Objects List */
         objectsList::removeAll();

         /* Clear the Models List */
         if(models)
         {
            delete(models);
            models = new modelList();
         }

         /* Clear all missions */
         missions->finish();
         missions->init(this);
              
         /* Put the animation state on normal */
         PCs->getActiveCharacter()->setState(STATE_IDLE);

         cursors->set(CURSOR_WALK);
         return(0);
     }

     /* Verify battle events */
     else if(engineMode == ENGINE_MODE_TURN_BATTLE) 
     {
        time = SDL_GetTicks();
        if(fightStatus == FIGHT_END)
        {
           engineMode = ENGINE_MODE_REAL_TIME;
           brief->addText( gettext("Exit Battle Mode"));
           /* Verify if any PC is alive. */
           character* pers = (character*) PCs->getFirst();
           bool alive = false;
           int j;
           for(j = 0; ( (j < PCs->getTotal()) && (!alive)); j++)
           {
              if(pers->isAlive())
              {
                 alive = true;
              }
              pers = (character*) pers->next;
           }
           if(!alive) 
           {
              /* All Pcs are Dead wait the dying animation */
              engineMode = ENGINE_MODE_WAIT_DIE_ANIMATION;
              waitDieTimer = SDL_GetTicks();
           }
        }
        /* Continue the fight */
        else if( (fightStatus == FIGHT_CONTINUE) ) 
        {
           lastTurnTime = time;
           fightStatus = fight->doBattleCicle();
           
           updateAllHealthBars();

           if(fightStatus == FIGHT_PC_TURN)
           {
               if(fight->actualCharacterTurn()) 
               {
                  PCs->setActiveCharacter(fight->actualCharacterTurn());
                  activeCharacter = PCs->getActiveCharacter();
                  attackFeat = activeCharacter->getActiveFeatRangeType();

                  moveCircleX = activeCharacter->xPosition;
                  moveCircleY = activeCharacter->yPosition;
                  moveCircleZ = activeCharacter->zPosition;
               }
               else
               { //FIXME
                  fightStatus = FIGHT_CONTINUE;
               }
           }
           else if(fightStatus == FIGHT_NPC_TURN)
           {
              activeCharacter = fight->actualCharacterTurn();
              moveCircleX = activeCharacter->xPosition;
              moveCircleY = activeCharacter->yPosition;
              moveCircleZ = activeCharacter->zPosition;
           }
 
        }
     }
   }

   /* Remove All Sound Effects */
   snd->removeAllSoundEffects();

   /* Hide All Active Windows */
   gui->hideAll();

   return(1);
}


