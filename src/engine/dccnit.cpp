/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "dccnit.h"
#include "culling.h"
#include "util.h"
#include "savefile.h"
#include "savewindow.h"
#include "../cbook/comicbook.h"

#include <math.h>
#ifdef __APPLE__
   #include <SDL_image/SDL_image.h>
#else
   #include <SDL/SDL_image.h>
#endif

#include "../etc/extensions.h"
#include "../etc/userinfo.h"
using namespace std;

/*********************************************************************
 *                       Engine's Constructor                        *
 *********************************************************************/
engine::engine()
{
   int i;

   /* Initialize internal lists */
   gui  = new Farso::GuiInterface("");
   curCampaign = NULL;
   callCampaignScript = false;
   actualMap = NULL;
   PCs = NULL;
   NPCs = NULL;
   activeCharacter = NULL;
   frontSurface = NULL;

   inventoryWindow = NULL;

   walkPressTime = 0;
   walkDistance = 0;

   xFloor = 0;
   zFloor = 0;
   xReal = 0;
   yReal = 0;
   zReal = 0;

   mouseWheel = 0;

   imgNumber = 0;
   actualScreen = NULL;

   curConection = NULL;

   walkStatus = ENGINE_WALK_KEYS;

   /* Initialize the Cursor */
   cursors = new(cursor);
   cursors->init();
   cursors->setTextOverFont(dir.getRealFile(DNT_FONT_PALLADIO));

   /* Initialize all shaders */
   glslShaders.init();

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

   /* Load Skills List */
   skillsList.init();
  
   /* Load Features List */
   features = new featsList();
   features->init(dir.getRealFile("feats/"),
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
   weaponsTypes.init();

   /* Create the Scene */
   curScene.init();

   /* Create Special Windows */
   infoWindow = new itemWindow(gui);
   charInfoWindow = new charWindow(gui);
   journal = new journalWindow();
   mapWindow = new miniMapWindow();
   shortcuts = new shortcutsWindow();

   /* Create the fps window */
   fpsGui  = new Farso::GuiInterface("");
   fpsWindow = fpsGui->insertWindow(option->getScreenWidth()-129, 
         option->getScreenHeight()-33, option->getScreenWidth()-1,
         option->getScreenHeight()-1, "", true); 
   fpsText = fpsWindow->getObjectsList()->insertTextBox(10, 10, 118, 30, 0, "");
   fpsText->setFont(dir.getRealFile(DNT_FONT_PALLADIO), 14, 
         Farso::Font::ALIGN_CENTER);
   fpsText->setSolid();
   fpsWindow->setExternPointer(&fpsWindow);
   fpsGui->openWindow(fpsWindow);

   /* Initialize Briefing */
   brief = new briefing();

   /* Initialize readModes variables */
   lastRead = SDL_GetTicks();
   lastMouse = lastRead;
   lastKeyb = lastRead;
   mouseX = 0;
   mouseY = 0;

   showRange = false;
   occPosition = false;

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

   hour = 9.0f;
   float sunPos = (OUTDOOR_FARVIEW * option->getFarViewFactor()) / 2.0f;
   gameSun = new sun(hour, sunPos, sunPos);

   engineMode = ENGINE_MODE_REAL_TIME;

   destinyVariation = -2.0f;
   lastKey = 0;

   /* Colors */
   for(i = 0; i < 3; i++)
   {
      defaultColor[i] = 0.2f;
      blackColor[i] = 0.0f;
   }
   defaultColor[3] = 1.0f;
   blackColor[3] = 1.0f;

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

   closeWindows();

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
   glDeleteTextures(1, &canOccImage);
   glDeleteTextures(1, &featRangeCircle);
   glDeleteTextures(1, &featAreaCircle);
   glDeleteTextures(1, &idTextura);

   /* Clear ModState */
   modifState.clear();

   /* Clear the Inventory too */
   if(inventoryWindow)
   {
      delete(inventoryWindow);
   }

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

   /* Clear Journal Window */
   if(journal)
   {
      delete(journal);
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
   if(fpsGui)
   {
      fpsGui->closeWindow(fpsWindow);
      delete(fpsGui);
   }
   if(gui)
   {
      delete(gui);
   }

   /* Clear Maps */
   if(actualMap)
   {
      delete(actualMap);
   }

   if(curCampaign)
   {
      delete(curCampaign);
   }
 
   /* Clear Cursors */
   cursors->finish();
   delete(cursors);

   /* Delete all shaders */
   glslShaders.finish();

   /* Delete the action controller */
   delete(actionControl);

   /* Removel all objects in the list */
   objectsList::removeAll();

   /* Clear scene */
   curScene.finish();
   
   /* Clear Characters Lists */
   alignList->finish();
   delete(alignList);
   features->finish();
   delete(features);

   raceList->finish();
   delete(raceList);
 
   classList->finish();
   delete(classList);
   skillsList.finish();
   weaponsTypes.finish();

   /* Clear the visibleMatrix */
   for(i = 0; i < 6; i++)
   {
      delete[] visibleMatrix[i];
   }
   delete[] visibleMatrix;
}

/*********************************************************************
 *                           closeWindows                            *
 *********************************************************************/
void engine::closeWindows()
{
   /* Close the static windows, if needed */
   barterWindow tradeWindow;
   dialogWindow dlgWindow;
   tradeWindow.close();
   dlgWindow.close();

   /* Close info Window, character window and journal window too */
   if(infoWindow->isOpen())
   {
      infoWindow->close();
   }
   if(moneySplitWindow.isOpen())
   {
      moneySplitWindow.cancel();
   }
   if(charInfoWindow->isOpen())
   {
      charInfoWindow->close();
   }
   if(journal->isOpen())
   {
      journal->close();
   }

   /* Close default windows */
   if(mapWindow)
   {
      mapWindow->close(gui);
   }
   if(shortcuts)
   {
      shortcuts->close(gui);
   }
   if(brief)
   {
      brief->closeWindow(gui);
   }

   /* Close inventory */
   if(inventoryWindow)
   {
      delete(inventoryWindow);
      inventoryWindow = NULL;
   }

}

/*********************************************************************
 *                         quitCurrentGame                           *
 *********************************************************************/
void engine::quitCurrentGame()
{
   /* Remove All Sound Effects */
   if(snd)
   {
      snd->removeAllSoundEffects();
   }
   walkSound = NULL;

   /* Close all engine related windows */
   closeWindows();

   /* Clear Modifications */
   modifState.clear();
   engineMode = ENGINE_MODE_REAL_TIME;

   /* Delete all particles */
   particleController.deleteAll();

   /* Delete all NPCs */
   if(NPCs)
   {
      delete(NPCs);
      NPCs = NULL;
   }

   /* Clear the Inventory */
   if(PCs)
   {
      PCs->getActiveCharacter()->newInventory();
   }

   if(actualMap)
   {
      delete(actualMap);
      actualMap = NULL;
   }

   if(curCampaign)
   {
      delete(curCampaign);
      curCampaign = NULL;
      callCampaignScript = false;
   }

   /* Clear Objects List */
   objectsList::removeAll();

   /* Clear all missions */
   if(missions)
   {
      missions->finish();
      missions->init(this);
   }

   if(actionControl)
   {
      /* Remove All Pending Actions */
      actionControl->removeAllActions();
   }

   /* Close all Opened windows */
   gui->closeAllWindows();

   /* Remove All Characters */
   if(PCs)
   {
      delete(PCs);
      PCs = NULL;
   }

   /* Restart the Scene */
   curScene.finish();
   curScene.init();

}

/*********************************************************************
 *                               loadGame                            *
 *********************************************************************/
bool engine::loadGame()
{
   bool res = false;

   /* create and run the load/save window */
   saveWindow *savWindow = new saveWindow();
   if(savWindow->run(true, idTextura, 
            proj, modl, viewPort) == DNT_SAVE_WINDOW_CONFIRM)
   {
      saveFile *sav = new saveFile();

      if(sav->loadHeader(savWindow->getSelectedFileName()))
      {
         glDisable(GL_LIGHTING);
         glDisable(GL_FOG);
         mainScreenEffect();
         /* Quit from the current map, if is in one */
         quitCurrentGame();

         /* Load the file */
         sav->load(this);

         /* Open the default windows */
         mapWindow->open(gui,0,0, actualMap);
         brief->openWindow(gui);
         shortcuts->open(gui);

         /* make sure not in battle mode */
         engineMode = ENGINE_MODE_REAL_TIME;

         /* Put the camera at the characters position */
         activeCharacter = PCs->getActiveCharacter();
         if(activeCharacter)
         {
            gameCamera.updateCamera(activeCharacter->scNode->getPosX(),
                  activeCharacter->scNode->getPosY(),
                  activeCharacter->scNode->getPosZ(),
                  activeCharacter->scNode->getAngleY());
         }

         res = true;
      }
      else
      {
         Farso::Warning warn;
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
   Farso::Warning warn;

   if(engineMode != ENGINE_MODE_TURN_BATTLE)
   {
      /* create and run the load/save window */
      saveWindow *savWindow = new saveWindow();
      if(savWindow->run(false, idTextura, 
               proj, modl, viewPort) == DNT_SAVE_WINDOW_CONFIRM)
      {
         saveFile *sav = new saveFile();

         /* Do the Save */
         if(sav->save(savWindow->getSelectedFileName(), this, frontSurface))
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
 *                           keepNPCStatus                           *
 *********************************************************************/
void engine::keepNPCStatus()
{
   if( (NPCs) && (actualMap))
   {
      /* Save All Needed Inventory Status And NPC Psycho */
      character* dude = (character*)NPCs->getFirst();
      int i;
      for(i = 0; i < NPCs->getTotal(); i++)
      {
         /* Save inventory */
         if(dude->inventories->getTotalItems() > 0)
         {
            modifState.mapInventoryAdd(dude->inventories, 
                  dude->getCharacterFile(),
                  actualMap->getFileName());
         }
         /* Save psycho state */
         modifState.mapCharacterAddAction(
                                   MODSTATE_ACTION_CHARACTER_CHANGE_STATE,
                                   dude->getCharacterFile(), 
                                   actualMap->getFileName(), 
                                   dude->scNode->getPosX(),
                                   dude->scNode->getPosY(), 
                                   dude->scNode->getPosZ(),
                                   dude->getPsychoState(), 
                                   dude->initialXPosition,
                                   dude->initialZPosition);

         dude = (character*)dude->getNext();
      }
   }
}

/*********************************************************************
 *                            loadCampaign                           *
 *********************************************************************/
void engine::loadCampaign(std::string campaignFile, bool loadingGame)
{
   if(curCampaign)
   {
      delete(curCampaign);
   }

   /* Create the campaign */
   curCampaign = new DNT::Campaign(this, campaignFile);
   callCampaignScript = !loadingGame;

   if(!loadingGame)
   {
      /* Load and display introduction comic book (if defined) */
      if(!curCampaign->getIntroductionBook().empty())
      {
         comicBook* cBook = new comicBook();
         cBook->load(curCampaign->getIntroductionBook());
         cBook->run();
      }
      
      /* Load the initial map */
      loadMap(curCampaign->getInitialMap());
   }
}

/*********************************************************************
 *                         Load Map to Engine                        *
 *********************************************************************/
int engine::loadMap(string arqMapa, bool loadingGame)
{
   Farso::HealthBar* progress = NULL; /* the progress bar */

   Farso::Font fnt;                   /* DNT font controller */
   int centerY = Farso::SCREEN_Y / 2;   /* Window Y middle */
   int centerX = Farso::SCREEN_X / 2;   /* Window X middle */

   int midW=128, /* Middle Width of load image */
       midH=256; /* Middle Height of load image */
   GLuint texturaCarga; /* Load texture */
   SDL_Surface* fig;    /* Load image */
   
   SDL_Surface* img;    /* Text surface*/
   GLuint texturaTexto; /* Text texture */

   char texto[512];
   string arqVelho = "nada";

   bool alreadyModified = modifState.haveMap(arqMapa);

   /* Set the some initial map-engine status */
   curConection = NULL;
   walkStatus = ENGINE_WALK_KEYS;
   
   /* Generate Loading image Texture */
   glGenTextures(1,&texturaCarga);
   
   /* Clear things */
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
   glDisable(GL_LIGHTING);

   /* Load the random loading image */
   srand(SDL_GetTicks() + (int)(1 + 1000 * (rand() / (RAND_MAX + 1.0))));
   int loadNumber = 1+(int)(ENGINE_LOADING_IMAGES * (rand()/(RAND_MAX + 1.0)));
   sprintf(texto,"comics/loading/loading%d.png",loadNumber);
   fig = IMG_Load(dir.getRealFile(texto).c_str());
   if(!fig)
   {
      cerr << "Faled to load 'loading texture'!" << endl;
   }
   else
   {
      /* Set halfs */
      midW = fig->w / 2;
      midH = fig->h / 2;
            
      /* Load the texture to opengl */
      Farso::setTexture(fig,texturaCarga);
      SDL_FreeSurface(fig);
   }  
   
   /* Create the text surface/texture */
   img = SDL_CreateRGBSurface(SDL_SWSURFACE, 2*midW,32,32,
                              0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   Farso::color_Set(0,0,0,255);
   Farso::rectangle_Fill(img,0,0,255,31);

   /* create the progress bar */
   progress = new Farso::HealthBar(2,20,(midW*2)-3,30, img);
   progress->defineMaxHealth(11);
   progress->defineActualHealth(0);



   /* Fadein the loading image effect */
   fadeInTexture(texturaCarga, centerX-midW, centerY-midH, 
                 centerX+midW, centerY+midH, 256,128);

   /* Loading Map */
   if(actualMap) 
   {
      /* Show the "Saving Map Status" */
      Farso::color_Set(200,20,20,255);
      fnt.defineFont(dir.getRealFile(DNT_FONT_TIMES),10);
      sprintf(texto,"%s", gettext("Saving Current Map Modifications"));
      showLoading(img,&texturaTexto,texturaCarga, texto, progress);
      progress->defineActualHealth(1);

      keepNPCStatus();
      
      arqVelho = actualMap->getFileName();
      delete(actualMap);

      /* Remove All Unused 3D Models */
      curScene.removeUnusedModels();
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
   Farso::color_Set(200,20,20,255);
   fnt.defineFont(dir.getRealFile(DNT_FONT_TIMES),10);
   sprintf(texto,gettext("Loading Map: %s"),arqMapa.c_str());
   showLoading(img,&texturaTexto,texturaCarga, texto, progress);
   progress->defineActualHealth(3);

   actualMap = new Map();
   actualMap->setFileName(arqVelho);
   actualMap->open(arqMapa);

   /* Apply the FOG */
   mapFog fog = actualMap->getFog();
   fog.apply(actualMap->isOutdoor(), option->getFarViewFactor()*OUTDOOR_FARVIEW,
         INDOOR_FARVIEW);

   /* Loading NPCs */
   NPCs = NULL;
   character* per;
   if(!actualMap->getNpcFileName().empty())
   {
      string npcFileName="", name="";
      GLfloat posX=0, posZ=0, ori=0;
      int psycho=0;

      npcFile* arq = new npcFile();
      NPCs = new (characterList);

      if(arq->load(actualMap->getNpcFileName()))
      { 
         while(arq->getNextCharacter(name, npcFileName, posX, posZ, 
                  ori, psycho))
         {
           sprintf(texto, gettext("Loading NPC: %s"), name.c_str());
           showLoading(img,&texturaTexto,texturaCarga,
                       texto, progress);
           per = NPCs->insertCharacter(npcFileName, features, this, arqMapa);
           /* Define Initial Position */
           per->initialXPosition = posX;
           per->initialZPosition = posZ;
           per->scNode->set(posX, actualMap->getHeight(posX, posZ), posZ,
                            0.0f, ori, 0.0f);
           per->setPsychoState(psycho);
           /* Define Occuped Square */
           per->defineOcSquare(actualMap);

           /* Load its inventory (only when not loading game
            * and not having the map at modif state. those restrictions
            * is to avoid "repopulate" a modified inventory with its
            * initial defined state). */
           if( (!loadingGame) &&
               (!alreadyModified) && 
               (!per->getInventoryFile().empty() ))
           {
              modInventory* inv = new modInventory(NULL, "", "");
              inv->load(per->getInventoryFile());
              inv->flush(actualMap, per->inventories);
              delete(inv);
           }
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
   actualMap->drawMiniMap();
   progress->defineActualHealth(8);

   /* Loading Internal Windows */
   showLoading(img,&texturaTexto,texturaCarga,
               gettext("Putting GUI on Screen"), progress);

   progress->defineActualHealth(9);

   /* Reopen, if already opened, some game Windows */
   if(mapWindow->isOpened())
   {
      mapWindow->reOpen(gui, actualMap);
   }
   if(brief->isOpened())
   {
      brief->reOpen(gui);
   }
   if(shortcuts->isOpened())
   {
      shortcuts->reOpen(gui);
   }

   /* Define Map */
   particleController.setActualMap(actualMap, &colisionDetect);
   colisionDetect.defineMap(actualMap, NPCs, PCs);
   actionControl->setCharacterLists(NPCs, PCs);
   fight->setMap(actualMap->getFileName());
   moneySplitWindow.setMap(gui, actualMap);

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
      particleController.deleteAll(true);
   }
   

   /* Put Active Party at initial Position */
   float pX=0.0, pY=0.0, pZ=0.0f, a=0.0f;
   activeCharacter = PCs->getActiveCharacter();
   actualMap->getInitialPosition(pX, pZ, a);
   pY = actualMap->getHeight(pX, pZ);
   activeCharacter->scNode->set(pX, pY, pZ, 0.0f, a, 0.0f);
   gameCamera.updateCamera(activeCharacter->scNode->getPosX(),
                           activeCharacter->scNode->getPosY(),
                           activeCharacter->scNode->getPosZ(),
                           activeCharacter->scNode->getAngleY());
   activeCharacter->ocSquare = actualMap->getInitialSquare();

   showLoading(img,&texturaTexto,texturaCarga,
               gettext("Loading Changes..."), progress);
   progress->defineActualHealth(11);

   /* Do Modifications */
   modifState.doMapModifications(actualMap, NPCs);

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

   /* Done */
   showLoading(img,&texturaTexto,texturaCarga,
               gettext("Done!"), progress);

   /* Somw fadeout effect */
   glDisable(GL_LIGHTING);
   fadeOutTexture(texturaCarga,centerX-midW,centerY-midH,
                  centerX+midW,centerY+midH, 256,128);

   /* Set the Farview to indoor or outdoor */
   if(actualMap->isOutdoor())
   {
      redefineWindow(actualScreen, option->getFarViewFactor()*OUTDOOR_FARVIEW);
   }
   else
   {
      redefineWindow(actualScreen, INDOOR_FARVIEW);
   }

   /* Free Loading Textures */
   SDL_FreeSurface(img);
   glDeleteTextures(1,&texturaTexto);

   glEnable(GL_LIGHTING);

   return(1);
}

/*********************************************************************
 *                               fadeIn                              *
 *********************************************************************/
void engine::fadeIn()
{
   int i;
   for(i=0; i < 50; i++)
   {
      clearOpenGL();

      /* Draw things */
      if(shadowMap.isEnable())
      {
         drawWithShadows(false);
      }
      else
      {
         drawWithoutShadows(false);
      }

      draw2DMode();

      glDisable(GL_LIGHTING);
      glDisable(GL_FOG);
      glDisable(GL_DEPTH_TEST);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_COLOR_MATERIAL);

      glColor4f(i/50.0f, i/50.0f, i/50.0f, (50-i)/50.0f);

      glBegin(GL_QUADS);
         glVertex3f(0, 0, 0);
         glVertex3f(Farso::SCREEN_X, 0, 0);
         glVertex3f(Farso::SCREEN_X, Farso::SCREEN_Y, 0);
         glVertex3f(0, Farso::SCREEN_Y, 0);
      glEnd();
      glDisable(GL_COLOR_MATERIAL);
      glDisable(GL_BLEND);
      glEnable(GL_LIGHTING);
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_FOG);


      /* Rdefine 3D Mode */
      if((actualMap != NULL) && (actualMap->isOutdoor()))
      {
         draw3DMode(option->getFarViewFactor()*OUTDOOR_FARVIEW);
      }
      else
      {
         draw3DMode(INDOOR_FARVIEW);
      }
      glFlush();
      SDL_GL_SwapBuffers();
      SDL_Delay(10);
   }
   glColor3f(1.0,1.0f,1.0f);

}

/*********************************************************************
 *                              fadeOut                              *
 *********************************************************************/
void engine::fadeOut()
{
   int i;
   for(i=49; i >= 0; i--)
   {
      clearOpenGL();
      /* Draw things */
      if(shadowMap.isEnable())
      {
         drawWithShadows(false);
      }
      else
      {
         drawWithoutShadows(false);
      }

      draw2DMode();

      glDisable(GL_LIGHTING);
      glDisable(GL_FOG);
      glDisable(GL_DEPTH_TEST);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_COLOR_MATERIAL);

      glColor4f(i/50.0f, i/50.0f, i/50.0f, (50-i)/50.0f);

      glBegin(GL_QUADS);
         glVertex3f(0, 0, 0);
         glVertex3f(Farso::SCREEN_X, 0, 0);
         glVertex3f(Farso::SCREEN_X, Farso::SCREEN_Y, 0);
         glVertex3f(0, Farso::SCREEN_Y, 0);
      glEnd();
      glDisable(GL_COLOR_MATERIAL);
      glDisable(GL_BLEND);
      glEnable(GL_LIGHTING);
      glEnable(GL_FOG);
      glEnable(GL_DEPTH_TEST);


      /* Rdefine 3D Mode */
      if((actualMap != NULL) && (actualMap->isOutdoor()))
      {
         draw3DMode(option->getFarViewFactor()*OUTDOOR_FARVIEW);
      }
      else
      {
         draw3DMode(INDOOR_FARVIEW);
      }
      glFlush();
      SDL_GL_SwapBuffers();
      SDL_Delay(10);
   }
   glColor3f(1.0,1.0f,1.0f);
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
      clearOpenGL();
      draw2DMode();
      glColor3f(i/50.0f, i/50.0f, i/50.0f);
      Farso::textureToScreen(id,x1,y1,x2,y2,sizeX,sizeY);
      draw3DMode(option->getFarViewFactor()*OUTDOOR_FARVIEW);
      glFlush();
      SDL_GL_SwapBuffers();
      SDL_Delay(10);
   }
   glColor3f(1.0,1.0f,1.0f);
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
      clearOpenGL();
      draw2DMode();
      glColor3f(i/50.0f, i/50.0f, i/50.0f);
      Farso::textureToScreen(id,x1,y1,x2,y2,sizeX,sizeY);
      draw3DMode(option->getFarViewFactor()*OUTDOOR_FARVIEW);
      glFlush();
      SDL_GL_SwapBuffers();
      SDL_Delay(10);
   }
   glColor3f(1.0f, 1.0f, 1.0f);
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
   clearOpenGL();
   updateFrustum(visibleMatrix,proj,modl);
   SDL_Surface* img;
   img = IMG_Load(dir.getRealFile("texturas/general/inicio1.png").c_str()); 
   glDisable(GL_LIGHTING);

   Farso::Font fnt;
   fnt.defineFont(dir.getRealFile(DNT_FONT_TIMES), 20);
   fnt.defineFontAlign(Farso::Font::ALIGN_CENTER);
   Farso::color_Set(130,130,130,255);
   fnt.write(img, 52, 424, gettext("DNT is relesead under GPLv3 or "
                                   "later: Feel free to use its source code "
                                   "and data the way you want (or to blame us "
                                   "the same way). DNT is thankful "
                                   "to the people who developed those "
                                   "libraries: OpenAL, Cal3D, OpenGL, SDL, "
                                   "SDL_image, SDL_ttf, libVorbis."), 
              52, 424, 745, 511);
   fnt.defineFontAlign(Farso::Font::ALIGN_LEFT);
   glGenTextures(1,&id);
   Farso::setTextureRGBA(img,id);
   SDL_FreeSurface(img);

   /* Fade In Screen */
   fadeInTexture(id,0,0,Farso::SCREEN_X-1,Farso::SCREEN_Y-1, 800, 600);

   /* Wait until Mouse Button pressed or time passed */
   while( !done )
   {
      clearOpenGL();
      draw2DMode();
      glColor3f(1.0f, 1.0f, 1.0f);
      Farso::textureToScreen(id, 0, 0, 
            Farso::SCREEN_X-1, Farso::SCREEN_Y-1, 800, 600);
      draw3DMode(option->getFarViewFactor()*OUTDOOR_FARVIEW);
      glFlush();
      SDL_GL_SwapBuffers();

      SDL_PumpEvents();
      clearEventsQueue();
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
      clearEventsQueue();
      mButton = SDL_GetMouseState(&x,&y);
      SDL_Delay(50);
   }

   fadeOutTexture(id,0,0,Farso::SCREEN_X-1,Farso::SCREEN_Y-1, 800, 600);
   
   glEnable(GL_LIGHTING);
   glDeleteTextures(1,&id);
}

/*********************************************************************
 *                       Call Initial Game Menu                      *
 *********************************************************************/
int engine::menuScreen(int Status, bool reloadMusic)
{
   /* Reload Music, if needed */
   if(reloadMusic)
   {
      /*music =*/
      snd->loadMusic("music/main_menu.ogg");
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
 *                          mainScreenEffect                         *
 *********************************************************************/
void engine::mainScreenEffect()
{
   GLfloat scale = 1.0f;
   glColor3f(1.0f, 1.0f, 1.0f);

   while(scale > 0)
   {
      glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

      draw2DMode();
      glPushMatrix();
      glScalef(scale, scale, scale);
      Farso::textureToScreen(idTextura,0,0,
            Farso::SCREEN_X-1,Farso::SCREEN_Y-1,800,600);
      glPopMatrix();
      draw3DMode(option->getFarViewFactor()*OUTDOOR_FARVIEW);

      glFlush();
      SDL_GL_SwapBuffers();

      scale -= 0.04f;

      SDL_Delay(30);
   }
}

/*********************************************************************
 *                       Call Options Game Screen                    *
 *********************************************************************/
int engine::optionsScreen()
{
   int optionW = OPTIONSW_OTHER;
   int time = SDL_GetTicks();
   int timeAnterior = 0;
   Uint8* keys;
   int x,y;
   Farso::GuiObject* object = NULL;
   int eventInfo = Farso::EVENT_NONE;

   glDisable(GL_LIGHTING);
   glDisable(GL_FOG);

   option->displayOptionsScreen(gui);

   while( (optionW != OPTIONSW_CANCEL) && 
          (optionW != OPTIONSW_CONFIRM))
   {
      time = SDL_GetTicks();
      if(time - timeAnterior >= UPDATE_RATE) 
      {
         timeAnterior = time;
         SDL_PumpEvents();
         clearEventsQueue();
         glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
         glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
         keys = SDL_GetKeyState(NULL);
         Uint8 mButton = SDL_GetMouseState(&x,&y);
         object = gui->manipulateEvents(x,y,mButton,keys, eventInfo);
         
         glPushMatrix();
            draw2DMode();
            Farso::textureToScreen(idTextura,0,0,
                  Farso::SCREEN_X-1,Farso::SCREEN_Y-1,800,600);
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
int engine::characterScreen()
{
   int charCreation = CHAR_OTHER;
   int time = SDL_GetTicks();
   int timeAnterior = 0;
   Uint8* keys;
   int x,y;
   Farso::GuiObject* guiObj = NULL;
   int eventInfo = Farso::EVENT_NONE;

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

   /* Feature select window */
   featSelWindow* talentWindow = NULL;

   /* Aspect Window */
   aspectWindow* aspWindow = NULL;

   /* Disable Lights and Fog */
   glDisable(GL_LIGHTING);

   /* Race Window */
   activeCharacter = PCs->getActiveCharacter();
   raceWindow* rcWindow = new raceWindow(gui, &activeCharacter->actualRace);

   while( (status != 7) )
   {
      time = SDL_GetTicks();
      if(time - timeAnterior >= UPDATE_RATE) 
      {
         timeAnterior = time;
         SDL_PumpEvents();
         clearEventsQueue();
         glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
         glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
         keys = SDL_GetKeyState(NULL);
         Uint8 mButton = SDL_GetMouseState(&x,&y);
         guiObj = gui->manipulateEvents(x,y,mButton,keys, eventInfo);

         
         glPushMatrix();
            draw2DMode();
            Farso::textureToScreen(idTextura,0,0,
                  Farso::SCREEN_X-1,Farso::SCREEN_Y-1,800,600);
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
            charCreation = rcWindow->treat(guiObj, eventInfo, gui);
            if(charCreation == RACEW_CONFIRM)
            {
               status = 1;
               delete(rcWindow);
               clWindow = new classWindow(gui,&activeCharacter->actualClass[0]);
            }
            else if(charCreation == RACEW_CANCEL)
            {
               status = 7;
               delete(rcWindow);
               charCreation = CHAR_CANCEL;
            }
         }
         /* Class Window Opened */
         else if(status == 1)
         {
            charCreation = clWindow->treat(guiObj, eventInfo, gui);
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
            charCreation = alWindow->treat(guiObj, eventInfo, gui);
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
             charCreation = atWindow->treat(guiObj, eventInfo, gui,
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
            charCreation = skWindow->treat(guiObj, eventInfo, gui); 
            if(charCreation == SKILLW_CONFIRM)
            {
               status = 5;
               delete(skWindow);
               talentWindow = new featSelWindow(gui);
               talentWindow->open(activeCharacter, features, 2);
            }
            else if(charCreation == SKILLW_CANCEL)
            {
               status = 3;
               delete(skWindow);
               activeCharacter->getAttModifiers(mods);
               atWindow = new attWindow(&activeCharacter->sk, gui, mods, true);
            }
         }
         /* Talent Window */
         else if(status == 5)
         {
            charCreation = talentWindow->treat(guiObj, eventInfo);
            if(charCreation == TALENT_WINDOW_CONFIRM)
            {
               status = 6;
               aspWindow = new aspectWindow(activeCharacter, gui);
            }
            else if(charCreation == TALENT_WINDOW_CANCEL)
            {
               status = 4;
               delete(talentWindow);
               skWindow = new skillWindow(&activeCharacter->sk,
                                          gui, activeCharacter->getLevel());
            }
         }
         /* Aspect Window Opened */
         else if(status == 6)
         {
            charCreation = aspWindow->treat(guiObj, eventInfo, gui);
            if(charCreation == ASPECTW_CONFIRM)
            {
               status = 7;

               /* Done with character creation */
               delete(aspWindow);
               charCreation = CHAR_CONFIRM;
            }
            else if(charCreation == ASPECTW_CANCEL)
            {
               status = 5;
               delete(aspWindow);
               delete(talentWindow);
               /* Clear all current Talents */
               activeCharacter->actualFeats->clear();
               activeCharacter->insertDefaultNeededFeats(features);
               talentWindow = new featSelWindow(gui);
               talentWindow->open(activeCharacter, features, 2);
            }
         }         
      }
      else if((UPDATE_RATE-1) - (time - timeAnterior) > 0 ) 
      {
         SDL_Delay((UPDATE_RATE-1) - (time - timeAnterior) );
      }
   }


   /* Update and calculate things related to character */
   if(charCreation == CHAR_CONFIRM)
   {
      /* Calculate Life Points */
      activeCharacter->defineInitialLifePoints();
      activeCharacter->applyBonusAndSaves();

      /* Insert Tutorial Books on its inventory */
      object* curObj;
      curObj =
         new object("models/objetos/books/combat_tutorial/combat_tutorial.dcc",
                    "");
      activeCharacter->inventories->addObject(curObj);

      /* Insert some money */
      money* m = new money();
      activeCharacter->inventories->addObject((object*)m);
      m->setQuantity(100);

      /* Apply all permanent feats */
      talentWindow->applyAllNewPermanentFeats();
      delete(talentWindow);
   }
   else
   {
      /* Must empty things */
      quitCurrentGame();
   }

   glEnable(GL_LIGHTING);
   
   return(charCreation);
}


/*********************************************************************
 *                     redefine Window to Screen                     *
 *********************************************************************/
void engine::redefineWindow(SDL_Surface *screen, float actualFarView)
{
   glViewport(0, 0, Farso::SCREEN_X, Farso::SCREEN_Y);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f, Farso::SCREEN_X / (float)Farso::SCREEN_Y, 1.0f, 
                  actualFarView);
   glGetIntegerv(GL_VIEWPORT, viewPort);
   glGetFloatv(GL_MODELVIEW_MATRIX, camProj);
   glMatrixMode (GL_MODELVIEW);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

   glEnable(GL_LINE_SMOOTH);
   glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
   glLineWidth(1.5f);
}

/*********************************************************************
 *                       Init Engine Function                        *
 *********************************************************************/
void engine::init(SDL_Surface *screen)
{
   actualScreen = screen;  
   redefineWindow(screen, option->getFarViewFactor()*OUTDOOR_FARVIEW);
   
   /* Clear */
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClearDepth(1.0f);
   glClearStencil(0);

   /* Details Definition */
   glDepthFunc(GL_LEQUAL);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);

   /* Light Definition */
   //GLfloat light_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
   //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
   //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
   //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

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
      cerr << "Error: can't Load Texure: fightMode/normalMovCircle.png" << endl;
   }
   glGenTextures(1, &normalMoveCircle);

   glBindTexture(GL_TEXTURE_2D, normalMoveCircle);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,DNT_IMAGE_FORMAT_A, GL_UNSIGNED_BYTE, img->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   SDL_FreeSurface(img);

   img = IMG_Load(
               dir.getRealFile("texturas/fightMode/fullMovCircle.png").c_str());
   if(!img)
   {
      cerr << "Error: can't Load Texure: fightMode/fullMovCircle.png" << endl;
   }

   glGenTextures(1, &fullMoveCircle);

   glBindTexture(GL_TEXTURE_2D, fullMoveCircle);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,DNT_IMAGE_FORMAT_A, GL_UNSIGNED_BYTE, img->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   SDL_FreeSurface(img);

   /* feat range circle */
   img = IMG_Load(
             dir.getRealFile("texturas/fightMode/featRangeCircle.png").c_str());
   if(!img)
   {
      cerr << "Error: can't Load Texure: fightMode/featRangeCircle.png" << endl;
   }

   glGenTextures(1, &featRangeCircle);

   glBindTexture(GL_TEXTURE_2D, featRangeCircle);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,DNT_IMAGE_FORMAT_A, GL_UNSIGNED_BYTE, img->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   SDL_FreeSurface(img);

   /* feat area circle */
   img = IMG_Load(
             dir.getRealFile("texturas/fightMode/featAreaCircle.png").c_str());
   if(!img)
   {
      cerr << "Error: can't Load Texure: fightMode/featAreaCircle.png" << endl;
   }

   glGenTextures(1, &featAreaCircle);

   glBindTexture(GL_TEXTURE_2D, featAreaCircle);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,DNT_IMAGE_FORMAT_A, GL_UNSIGNED_BYTE, img->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   SDL_FreeSurface(img);


   /* range circle */
   img = IMG_Load(dir.getRealFile("texturas/walk/range.png").c_str());
   if(!img)
   {
      cerr << "Error: can't Load Texure: walk/range.png" << endl;
   }

   glGenTextures(1, &rangeCircle);

   glBindTexture(GL_TEXTURE_2D, rangeCircle);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,DNT_IMAGE_FORMAT_A, GL_UNSIGNED_BYTE, img->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   SDL_FreeSurface(img);

   /* can occupy image */
   img = IMG_Load(dir.getRealFile("texturas/walk/canwalkmark.png").c_str());
   if(!img)
   {
      cerr << "Error: can't Load Texure: walk/canwalkmark.png" << endl;
   }

   glGenTextures(1, &canOccImage);

   glBindTexture(GL_TEXTURE_2D, canOccImage);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,DNT_IMAGE_FORMAT_A, GL_UNSIGNED_BYTE, img->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   SDL_FreeSurface(img);


   /* Move Destiny */
   img = IMG_Load(dir.getRealFile("texturas/walk/destino.png").c_str());
   if(!img)
   {
      cerr << "Error: can't Load Texture: destino.png" << endl;
   }

   glGenTextures(1, &destinyImage);

   glBindTexture(GL_TEXTURE_2D, destinyImage);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,DNT_IMAGE_FORMAT_A, GL_UNSIGNED_BYTE, img->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   SDL_FreeSurface(img);

   /* Main screen texture */
   img = IMG_Load(dir.getRealFile("texturas/general/inicio.png").c_str());
   glGenTextures(1,&idTextura);
   Farso::setTexture(img, idTextura);
   SDL_FreeSurface(img);


    /* ShadowMap */
   shadowMap.init();
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
 *                                 rest                              *
 *********************************************************************/
void engine::rest()
{
   int i;
   int h;
   character* ch;

   if(engineMode == ENGINE_MODE_TURN_BATTLE)
   {
      brief->addText(gettext("Couldn't rest at battle mode."), 220, 20, 20);
      return;
   }

   if(hasEnemies())
   {
      brief->addText(gettext("Enemies nearby."), 220, 20, 20);
      return;
   }

   /* Fade out map */
   fadeOut();
   SDL_Delay(100);

   /* Remove all pc related particles */
   particleController.deleteAllPCRelated();

   ch = (character*)PCs->getFirst();
   for(i=0; i < PCs->getTotal(); i++)
   {
      /* Heal Life Points */
      ch->setLifePoints(ch->getMaxLifePoints());
      ch->updateHealthBar();

      /* Do the rest to effects */
      ch->getEffects()->rest();

      ch = (character*)ch->getNext();
   }

   /* Pass time */
   h = (int)hour;
   hour -= h;
   h = (h+8) % 24;
   hour += h;
   hourToTxt();

   fadeIn();
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

   /* Clear the current selected feat */
   activeCharacter->setActiveFeat(FEAT_WEAPON_ATTACK);
   shortcuts->clearSelectedTalent();
}


/*********************************************************************
 *                            hasEnemies                             *
 *********************************************************************/
bool engine::hasEnemies()
{
   int i;

   if(!NPCs)
   {
      return(false);
   }

   character* ch = (character*)NPCs->getFirst();
   for(i = 0; i < NPCs->getTotal(); i++)
   {
      if( (ch->isAlive()) && (ch->getPsychoState() == PSYCHO_HOSTILE))
      {
         return(true);
      }
   }

   return(false);
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
  barterWindow bartWindow;
  dlgWindow.close();
  bartWindow.close();
  

  activeCharacter = PCs->getActiveCharacter();

  /* Couldn't enter battle mode with a dead character */
  if(!activeCharacter->isAlive())
  {
     return;
  }
  
  /* clear fight status */
  fight->empty();

  /* Verify if there are NPCs */
  if(!NPCs)
  {
     brief->addText(gettext("No NPCs in the area."), 220, 20, 20);
     return;
  }

  /* Add NPCs to Fight */
  int curGroup = 0;
  ch =(character*) NPCs->getFirst();
  for(i = 0; i < NPCs->getTotal(); i++)
  {
      //TODO put enemies on groups, when enemy from enemy
      if(ch->isAlive())
      {
         if(!fight->insertNPC(ch, curGroup))
         {
            curGroup++;
            fight->insertNPC(ch, curGroup);
         }
         numEnemies++;
         /* Set the state to Idle, if the character is alive */
         ch->callIdleAnimation();
         /* Remove Move, if it is moving */
         ch->pathFind.clear();
      }
      ch = (character*) ch->getNext();
      SDL_Delay(1);
  }
                 
   if(numEnemies > 0)
   {
      /* Really Init the Battle */
      snd->addSoundEffect(SOUND_NO_LOOP,"sndfx/other/battleMode.ogg");
      engineMode = ENGINE_MODE_TURN_BATTLE;
      moveCircleX = activeCharacter->scNode->getPosX();
      moveCircleY = activeCharacter->scNode->getPosY();
      moveCircleZ = activeCharacter->scNode->getPosZ();

      /* Put the PCs on group */
      ch =(character*) PCs->getFirst();
      for(i = 0; i < PCs->getTotal(); i++)
      {
         fight->insertPC(ch, 0);
         /* Set the state to Idle */
         ch->callIdleAnimation();
         /* Remove Move, if it is moving */
         ch->pathFind.clear();

         /* take next */
         ch = (character*) ch->getNext(); 
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
       ((SDL_GetTicks() - lastTurnTime) > ENGINE_WAIT_TURN_DELAY) )
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
         per->pathFind.doCycle((engineMode == ENGINE_MODE_TURN_BATTLE), false);
         per = (character*)per->getNext();
      }
   }

   if(PCs)
   {
      per = (character*) PCs->getFirst();
      for(aux=0; aux < PCs->getTotal(); aux++)
      {
         per->pathFind.doCycle((engineMode == ENGINE_MODE_TURN_BATTLE), true);
         per = (character*)per->getNext();
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
            /* Hightklight the enemy, if desired */
            if(option->getHighlightEnemy())
            {
               curTarget = npc;
            }
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
void engine::treatGuiEvents(Farso::GuiObject* object, int eventInfo)
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
   dlgWindow.treat(object, eventInfo, infoWindow, this, keys);

   /* Verify Money Window */
   moneySplitWindow.treat(object, eventInfo);

   /* Verify Inventory Window Actions */
   if( (inventoryWindow) )
   {
      if( ( (engineMode == ENGINE_MODE_TURN_BATTLE) && 
            (fightStatus == FIGHT_PC_TURN) &&
            (PCs->getActiveCharacter()->getCanAttack()) ) ||
          (engineMode == ENGINE_MODE_REAL_TIME) )
      {
         int act =inventoryWindow->treat(object, eventInfo, cursors, actualMap,
               PCs->getActiveCharacter()->scNode->getPosX(),
               PCs->getActiveCharacter()->scNode->getPosZ()); 
         if(act > INVENTORY_ACTION_INTERNAL)
         {
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
      charInfoWindow->treat(object, eventInfo, features, proj, modl, viewPort);
   }

   /* Verify ShortCutsWindow */
   if(shortcuts->isOpened())
   {
      int res = shortcuts->treat(object, eventInfo, engineMode, 
            PCs->getActiveCharacter());

      /* Now call the functions */
      switch(res)
      {
         case SHORTCUTS_WINDOW_MENU:
         {
            /* Go back to the game menu */
            exitEngine = 1;
            cursors->setTextOver("");
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
            /* Open / Close the minimap window */
            if(!mapWindow->isOpened())
            {
               snd->addSoundEffect(SOUND_NO_LOOP, "sndfx/gui/turn_page.ogg");
               mapWindow->open(gui, 0,0, actualMap);
            }
            else
            {
               snd->addSoundEffect(SOUND_NO_LOOP, "sndfx/gui/close.ogg");
               mapWindow->close(gui);
            }
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
            openCloseInventoryWindow();
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
         case SHORTCUTS_WINDOW_JOURNAL:
         {
            if(journal)
            {
               if(journal->isOpen())
               {
                  snd->addSoundEffect(SOUND_NO_LOOP, "sndfx/gui/close.ogg");
                  journal->close();
               }
               else
               {
                  journal->open(gui);
                  snd->addSoundEffect(SOUND_NO_LOOP, "sndfx/gui/turn_page.ogg");
               }
            }
         }
         break;
         case SHORTCUTS_WINDOW_REST:
         {
            rest();
         }
         break;
         case SHORTCUTS_WINDOW_SELECTED_TALENT:
         {
            feat* acFeat = shortcuts->getSelectedTalent();

            if(!activeCharacter->actualFeats->canUse(acFeat))
            {
               Farso::Warning warn;
               warn.show(gettext("Warning"), 
                         gettext("Not enought points to use!"), gui);
            }
            else
            {
               activeCharacter->setActiveFeat(acFeat);

               if( (engineMode == ENGINE_MODE_REAL_TIME) && 
                     (acFeat->info->action == ACT_ATTACK) )
               {
                  enterBattleMode(true);
               }
            }
         }
         break;
      }
  }  

}

/*********************************************************************
 *                              setHour                              *
 *********************************************************************/
void engine::setHour(float h)
{
   hour = h;
}

/*********************************************************************
 *                              getHour                              *
 *********************************************************************/
float engine::getHour()
{
   return(hour);
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
}

/*********************************************************************
 *                         updateMouseWorldPos                       *
 *********************************************************************/
void engine::updateMouseWorldPos()
{
   GLfloat wx,wy,wz=1;
   
   /* Define Mouse OpenGL Window Coordinate */
   wx = mouseX; wy = Farso::SCREEN_Y - mouseY; 
  
   /* Get the Zbuffer position of the mouse */
   glReadPixels((int)wx,(int)wy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz); 

   /* Get the world coordinate of the mouse position */
   gluUnProject(wx,wy,wz,modl,proj,viewPort,&xReal,&yReal,&zReal);

   /* Also, update the mouse floor */
   updateMouseFloorPos();
}

/*********************************************************************
 *                         updateMouseFloorPos                       *
 *********************************************************************/
void engine::updateMouseFloorPos()
{
   GLfloat wx = mouseX,
           wy = Farso::SCREEN_Y - mouseY;

   GLdouble cX=0, cY=0, cZ=0;
   GLdouble fX=0, fY=0, fZ=0;

   GLfloat t=0, vd=0;

   /* Get at camera */
   gluUnProject(wx, wy, 0, modl, proj, viewPort, &cX, &cY, &cZ);

   /* Get at far */
   gluUnProject(wx, wy, 0.9, modl, proj, viewPort, &fX, &fY, &fZ);

   /* Calculate */
   vd = fY - cY;
   if(vd == 0)
   {
      /* Vector Paralell to the floor */
      return;
   }
   t = (-cY / vd);
   xFloor = cX + t*(fX-cX);
   zFloor = cZ + t*(fZ-cZ);
}

/*********************************************************************
 *                         verifyMouseActions                        *
 *********************************************************************/
int engine::verifyMouseActions(Uint8 mButton)
{
   char buf[1024];
   boundingBox colBox;
   character* pers;
   int i;
   int pronto=0;

   Uint32 time = SDL_GetTicks();
   activeCharacter = PCs->getActiveCharacter();

   occPosition = true;

   /* Nullify the current target */
   curTarget = NULL;

   /* Create a bounding box for the mouse position */
   boundingBox mouseBox;
   mouseBox.setMin(xReal-4, yReal-4.0, zReal-4);
   mouseBox.setMax(xReal+4, yReal+4.0, zReal+4);
   boundingBox biggerMouseBox;
   biggerMouseBox.setMin(xReal-6, yReal-10.0, zReal-6);
   biggerMouseBox.setMax(xReal+6, yReal+10.0, zReal+6);

   int qx, qz;
   qx = (int)xReal / actualMap->squareSize();
   qz = (int)zReal / actualMap->squareSize();
   Square* quaux = actualMap->relativeSquare(qx,qz);
   if(quaux != NULL)
   {
      int obj = 0;
      objSquare* sobj = quaux->getFirstObject();

      /* Objects Verification */
      for(pronto = 0; ( (obj < quaux->getTotalObjects()) && (!pronto) );obj++)
      {
         if(sobj->obj)
         {
            colBox = sobj->obj->scNode->getBoundingBox();
            if(mouseBox.intercepts(colBox))
            {
               curTarget = (thing*)sobj->obj;
               cursors->setTextOver(sobj->obj->getName());

               /* Verify if object is walkable (if can't occ position,
                * no need to verify more). */
               if( (occPosition) &&
                 ((sobj->y + sobj->obj->scNode->getBoundingBox().max.y)>=1.5f))
               {
                  /* Isn't, verify depth collision to make sure
                   * the collision with mouse occurs */
                  if(sobj->obj->depthCollision(sobj->angleX, sobj->angleY,
                           sobj->angleZ, sobj->x,
                           sobj->y +
                           actualMap->getHeight(sobj->x,sobj->z),
                           sobj->z, biggerMouseBox))
                  {
                     occPosition = false;
                  }
               }
               
               /* The Object Dialog Window Call */
               if(!sobj->obj->getConversationFile().empty())
               {
                  occPosition = false;
                  cursors->set(CURSOR_USE);
                  if( (mouseButtonReleased[0]) && 
                      (rangeAction(activeCharacter->scNode->getPosX(), 
                                   activeCharacter->scNode->getPosZ(),
                                   sobj->x, sobj->z,
                                   activeCharacter->displacement)) )
                  {
                     dialogWindow dlgWindow;
                     dlgWindow.open(gui, activeCharacter, 
                           (conversation*)sobj->obj->getConversation(), 
                           sobj->obj->get2dModelName());
                  }
                  pronto = 1;
               }
               else if(sobj->obj->canGet())
               {
                  occPosition = false;
                  cursors->set(CURSOR_GET);
                  cursors->setTextOver(sobj->obj->getName()); 
                  if( (mButton & SDL_BUTTON(1)) && 
                      (rangeAction(activeCharacter->scNode->getPosX(), 
                                   activeCharacter->scNode->getPosZ(),
                                   sobj->x, sobj->z,
                                   activeCharacter->displacement) ) )
                  {
                     if( (engineMode != ENGINE_MODE_TURN_BATTLE) ||
                           (activeCharacter->getCanAttack() ) )
                     {
                        bool objGot = false;

                        /* Get Item count as action */
                        activeCharacter->setCanAttack(false);

                        /* Get Object */
                        lastMousePression = time;

                        if(sobj->obj->getType() == OBJECT_TYPE_MONEY)
                        {
                           money* m = (money*)sobj->obj;
                           objGot = activeCharacter->inventories->addMoney(
                                 m->quantity());
                        }
                        else
                        {
                           objGot = activeCharacter->inventories->addObject(
                                 sobj->obj);
                        }

                        if(objGot)
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
                           if(sobj->obj->getType() != OBJECT_TYPE_MONEY)
                           {
                              actualMap->removeObject(sobj->obj);
                           }
                           else
                           {
                              /* If money, delete the original object,
                               * as no more needed */
                              money* m = (money*)sobj->obj;
                              actualMap->removeObject(sobj->obj);
                              delete(m);
                           }
                        }
                        else
                        {
                           brief->addText(gettext("Inventory is full!"),
                                 220,20,20); 
                        }
                     }
                  }
                  pronto = 1;
               }
            }
         }
         sobj = (objSquare*)sobj->getNext();
      }

      /* Doors Verification */
      door* porta = actualMap->getFirstDoor();
      int d;
      for(d=0; ( (d < actualMap->getTotalDoors()) && (!pronto) ); d++)
      {
         colBox = porta->obj->scNode->getBoundingBox();
         if(mouseBox.intercepts(colBox))
         {
            occPosition = false;
            cursors->set(CURSOR_DOOR);
            cursors->setTextOver(gettext("Door")); 
            
            if( (mouseButtonReleased[0]) && 
                (rangeAction(activeCharacter->scNode->getPosX(), 
                             activeCharacter->scNode->getPosZ(),
                             porta->obj->scNode->getPosX(),
                             porta->obj->scNode->getPosZ(),
                             activeCharacter->displacement) ) )
            {
               if(porta->obj->getState() == DOOR_STATE_UNLOCKED)
               {
                  /* Door Unlocked, open or close it */
                  lastMousePression = time;
                  porta->flip();
               }
               else
               {
                  /* Door is locked. Open its dialog. */
                  dialogWindow dlgWindow;
                  dlgWindow.open(gui, activeCharacter, 
                        (conversation*)porta->obj->getConversation(), 
                        porta->obj->get2dModelName());

               }
            }
            pronto = 1;
         }
         porta = (door*)porta->getNext();
      }

      /* Are talent use */
      if((engineMode == ENGINE_MODE_TURN_BATTLE) && 
         (activeCharacter->getCanAttack()) &&
         (fightStatus == FIGHT_PC_TURN) && 
         (activeCharacter->getActiveFeatPtr()->info->type == FEAT_TYPE_ON_AREA))
      {
        feat* f = activeCharacter->getActiveFeatPtr();
        cursors->set(f->info->image);
        if( (mButton & SDL_BUTTON(1)) &&
            (rangeAction(activeCharacter->scNode->getPosX(), 
                         activeCharacter->scNode->getPosZ(),
                           xReal, zReal,
                           activeCharacter->getActiveFeatRange()) ) )
        {
           activeCharacter->setCanAttack(
                 !activeCharacter->actualFeats->useFeatAtArea(
                    activeCharacter, 
                    activeCharacter->getActiveFeat(), xReal, 0.0f, zReal));

           /* Clear the current selected feat */
           activeCharacter->setActiveFeat(FEAT_WEAPON_ATTACK);
           shortcuts->clearSelectedTalent();

           fight->verifyDeads();
        }
      }

      /* Talk And Attack Events Verification */
      if(NPCs)
      {
         pers = (character*) NPCs->getFirst();
         for(i = 0; ((i < NPCs->getTotal()) && (!pronto)); i++)
         {
            colBox = pers->scNode->getBoundingBox();
            if(mouseBox.intercepts(colBox))
            {
               occPosition = false;
               curTarget = (thing*)pers;
               if(pers->isAlive())
               {
                  cursors->setTextOver(pers->name);
               }
               else
               {
                  cursors->setTextOver(pers->name + string(" (") + 
                        string(gettext("dead")) + string(")"));
               }

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
                         (rangeAction(activeCharacter->scNode->getPosX(), 
                                      activeCharacter->scNode->getPosZ(),
                                      pers->scNode->getPosX(), 
                                      pers->scNode->getPosZ(),
                                      activeCharacter->displacement)) )
                     {
                        dialogWindow dlgWindow;
                        dlgWindow.open(gui, activeCharacter, 
                                       (conversation*)pers->getConversation(), 
                                       pers->getPortraitFileName());
                     }
                  }
                  pronto = 1;
               }
               /* Verify attacks */
               else if( (engineMode == ENGINE_MODE_TURN_BATTLE) && 
                        (activeCharacter->getCanAttack()) &&
                        (fightStatus == FIGHT_PC_TURN) && 
                        (activeCharacter->getActiveFeatPtr()->info->type == 
                         FEAT_TYPE_ON_TARGET))
               {
                  if(activeCharacter->getActiveFeat() == FEAT_WEAPON_ATTACK)
                  {
                     cursors->set(CURSOR_ATTACK);
                  }
                  else
                  {
                     feat* f = activeCharacter->getActiveFeatPtr();
                     cursors->set(f->info->image);
                  }

                  if( (mButton & SDL_BUTTON(1)) &&
                      (rangeAction(activeCharacter->scNode->getPosX(), 
                                   activeCharacter->scNode->getPosZ(),
                                   pers->scNode->getPosX(), 
                                   pers->scNode->getPosZ(),
                                   activeCharacter->getActiveFeatRange()) ) )
                  {
                     sprintf(buf, gettext("%s attacks %s"),
                             activeCharacter->name.c_str(),
                             pers->name.c_str());
                     brief->addText(buf);
                     activeCharacter->setCanAttack(
                        !activeCharacter->actualFeats->useFeatAtTarget(
                           activeCharacter, 
                           activeCharacter->getActiveFeat(), pers));

                     /* Clear the current selected feat */
                     activeCharacter->setActiveFeat(FEAT_WEAPON_ATTACK);
                     shortcuts->clearSelectedTalent();

                     fight->verifyDeads();

                     if( pers->getPsychoState() != PSYCHO_HOSTILE)
                     {
                        pers->setPsychoState(PSYCHO_HOSTILE);
                     }
                  }
                  pronto = 1;
               }
            }
            pers = (character*) pers->getNext();
         }
      }

      /* Map Connections Verification */
      if( ( quaux->mapConection.active ) && (!pronto) && 
          (engineMode == ENGINE_MODE_REAL_TIME) ) 
      {
         /* Don't travel on battle mode */
         colBox.setMin(quaux->mapConection.x1, 0.0f, quaux->mapConection.z1);
         colBox.setMax(quaux->mapConection.x2, 0.0f, quaux->mapConection.z2);
         if(mouseBox.intercepts( colBox ) )
         {
            cursors->setTextOver(quaux->mapConection.mapName); 

            curConection = &quaux->mapConection;
            cursors->set(CURSOR_MAPTRAVEL);
            pronto = 1;
            if( (mButton & SDL_BUTTON(1)) && 
                (rangeAction(activeCharacter->scNode->getPosX(), 
                             activeCharacter->scNode->getPosZ(),
                             xReal, zReal,
                             activeCharacter->displacement) ) )
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
   
      /* Verify Walls under mouse */
      if(occPosition)
      {
         wall* w = actualMap->getFirstWall();
         boundingBox wallBox;
         for(i=0; ( (i < actualMap->getTotalWalls()) && (occPosition)); i++)
         {
            wallBox.setMin(w->x1, 0.0f, w->z1);
            wallBox.setMax(w->x2, WALL_HEIGHT, w->z2);
            if(wallBox.intercepts(biggerMouseBox))
            {
               occPosition = false;
            }
            w = (wall*)w->getNext();
         }
      }
   }
   else
   {
      occPosition = false;
      if(!gui->mouseOnGui(mouseX, mouseY))
      {
         /* Mouse Cursor Forbidden (when can't go to position) */
         cursors->set(CURSOR_FORBIDDEN);

      }
   }

   pers = (character*) PCs->getFirst();
   for(i = 0; ((i < PCs->getTotal()) && (!pronto)); i++)
   {
      /* Mouse under portrait or health bar */
      if( (pers->mouseUnderPortrait(mouseX, mouseY)) || 
            (pers->mouseUnderHealthBar(mouseX, mouseY)) )
      {
         occPosition = false;
         pronto = 1;
         break;
      }

      /* Inventory Verification */
      colBox = pers->scNode->getBoundingBox();
      if(mouseBox.intercepts(colBox))
      {
         occPosition = false;
         cursors->set(CURSOR_INVENTORY);
         cursors->setTextOver(pers->name);
         curTarget = (thing*)pers;

         /* Open Inventory when button pressed */
         if( (mButton & SDL_BUTTON(1)) && (!inventoryWindow))
         {
            openCloseInventoryWindow();
         }
         pronto = 1;
      }

      pers = (character*) pers->getNext();
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
   float curWalkInterval = 0.0f; // Current Active Character walk interval
   Uint32 time;              // Actual Time
   GLfloat varX, varZ;       // to avoid GLfloat calculate
   GLfloat dist;
   activeCharacter = PCs->getActiveCharacter();
   int x,y;
   int guiEvent;
   Uint8 mButton;
   int i;

   GLfloat varTempo;  // Time Variation

   time = SDL_GetTicks();
   
   /* Do some randomize (current ticks + random number [0,1000) */
   srand(time + (int)(1 + 1000 * (rand() / (RAND_MAX + 1.0))));

   varTempo = (time-lastRead);
   if( ((varTempo)) >= UPDATE_RATE)
   {
      timePass = true;

      /* Update Time */
      seconds = varTempo / 1000.0f;

      hour = (hour + (seconds / 1000.0f) );
      if(hour > 23.99f)
      {
         hour = 0.0f;
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

      /* Let's check some events */
      SDL_Event event;
      mouseWheel = 0;
      for(i=0; i < MAX_MOUSE_BUTTONS; i++)
      {
         mouseButtonReleased[i] = false;
      }
          
      while(SDL_PollEvent(&event))
      {
         if(event.type == SDL_QUIT)
         {
         }
         else if(event.type == SDL_MOUSEBUTTONDOWN)
         {
            if(event.button.button == SDL_BUTTON_WHEELUP)
            {
               mouseWheel++;
            }
            else if(event.button.button == SDL_BUTTON_WHEELDOWN)
            {
               mouseWheel--;
            }
         }
         else if(event.type == SDL_MOUSEBUTTONUP)
         {
            if(event.button.button <= MAX_MOUSE_BUTTONS)
            {
               mouseButtonReleased[event.button.button-1] = true;
            }
         }
      }

      /* Input is disabled when running initial campaign script */
      if(callCampaignScript)
      {
         /* Run campaign script cycle */
         callCampaignScript = curCampaign->runInitialScript();
         treatPendingActions();
      }
      else
      {
         /* Get AlwaysRun from Options */
         run = option->getAlwaysRun();

         /* Verify if will enter the Battle Mode because of
          * enemies characters at range! */
         if( (engineMode != ENGINE_MODE_TURN_BATTLE) && (NPCs != NULL) &&
               (activeCharacter->isAlive()) )
         {
            character* npc = (character*)NPCs->getFirst();
            for(i = 0; ( (i < NPCs->getTotal()) && 
                     (engineMode != ENGINE_MODE_TURN_BATTLE)); i++)
            {
               /* Only Force Enter Battle Mode with Hostile Character */
               if( (npc->isAlive()) && 
                   (npc->getPsychoState() == PSYCHO_HOSTILE) )
               {
                  /* Verify Distance */
                  dist = sqrt( (npc->scNode->getPosX() - 
                           activeCharacter->scNode->getPosX()) *
                        (npc->scNode->getPosX() - 
                         activeCharacter->scNode->getPosX()) +
                        (npc->scNode->getPosZ() - 
                         activeCharacter->scNode->getPosZ()) *
                        (npc->scNode->getPosZ() - 
                         activeCharacter->scNode->getPosZ()));

                  if(dist < DNT_BATTLE_RANGE)
                  {
                     //TODO -> do a perception check before enter!

                     /* If near, enter battle Mode  */
                     enterBattleMode(false);
                  }
               }
               npc = (character*)npc->getNext();
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
                 (time-lastMousePression >= REFRESH_RATE)) ||
               (mouseButtonReleased[0]) ||
               (mouseButtonReleased[2]) )
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
            else
            {
               occPosition = false;
            }
         }

         /* Keyboard Verification */

         /* Exit Engine */
         if( ( keys[SDLK_ESCAPE] ) && 
               ( (time-lastKeyb >= REFRESH_RATE) || 
                 (lastKey != SDLK_ESCAPE) ) &&
               (!gui->getActiveWindow()->isModal()) )
         {
            defineFrontSurface();
            lastKey = SDLK_ESCAPE;
            lastKeyb = time;
            cursors->setTextOver("");
            return(0);
         }

         /* Rest */
         if( ( keys[option->getKey(DNT_KEY_REST)] ) &&
               ( (time-lastKeyb >= REFRESH_RATE) ||
                 (lastKey != option->getKey(DNT_KEY_REST)) ) &&
               (!gui->getActiveWindow()->isModal()) )
         {
            rest();
            lastKey = option->getKey(DNT_KEY_REST);
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

         /* Show info about occ square */
         if(keys[SDLK_F6])
         {
            Square* sq = PCs->getActiveCharacter()->ocSquare;
            cerr << "occ: " << sq << endl;
            if(sq)
            {
               cerr << sq->posX << " " << sq->posZ << endl;
               cerr << sq->flags << endl;
               cerr << (sq->flags & SQUARE_REFLECT) << endl;
            }
         }

         /* Enter Attack Mode or End Turn */
         if( (keys[option->getKey(DNT_KEY_BATTLE_TURN)]) && 
               ( (time-lastKeyb >= REFRESH_RATE) || 
                 (lastKey != option->getKey(DNT_KEY_BATTLE_TURN)) ) )
         {
            if(engineMode != ENGINE_MODE_TURN_BATTLE)
            {
               enterBattleMode(true);
            }
            else if(fightStatus == FIGHT_PC_TURN)
            {
               endTurn();
            }
            lastKey = option->getKey(DNT_KEY_BATTLE_TURN);
            lastKeyb = time;
         }

         /* Enable / Disable The Range Draw */
         if( (keys[SDLK_k]) && 
               ( (time-lastKeyb >= REFRESH_RATE) || 
                 (lastKey != SDLK_k) ) )
         {
            showRange = !showRange;
            lastKey = SDLK_k;
            lastKeyb = time;
         }

         /* Print All Models on List */
         if( (keys[SDLK_F2]) && 
               ( (time-lastKeyb >= REFRESH_RATE) || 
                 (lastKey != SDLK_F2) ) )
         {
            lastKey = SDLK_F2;
            lastKeyb = time;
            //models.printAll();
         }      

         /* Open Minimap */
         if( (keys[option->getKey(DNT_KEY_WINDOW_MINI_MAP)]) && 
               ( (time-lastKeyb >= REFRESH_RATE) || 
                 (lastKey != option->getKey(DNT_KEY_WINDOW_MINI_MAP)) ) )
         {
            if(!mapWindow->isOpened())
            {
               snd->addSoundEffect(SOUND_NO_LOOP, "sndfx/gui/turn_page.ogg");
               mapWindow->open(gui, 0,0, actualMap);
            }
            else
            {
               snd->addSoundEffect(SOUND_NO_LOOP, "sndfx/gui/close.ogg");
               mapWindow->close(gui);
            }

            lastKey = option->getKey(DNT_KEY_WINDOW_MINI_MAP);
            lastKeyb = time;
         }

         /* Open ShortCuts */
         if( (keys[option->getKey(DNT_KEY_WINDOW_SHORTCUTS)]) && 
               ( (time-lastKeyb >= REFRESH_RATE) || 
                 (lastKey != option->getKey(DNT_KEY_WINDOW_SHORTCUTS)) ) )
         {
            shortcuts->open(gui);
            lastKey = option->getKey(DNT_KEY_WINDOW_SHORTCUTS);
            lastKeyb = time;
         }

         /* Open Briefing */
         if( (keys[option->getKey(DNT_KEY_WINDOW_BRIEFING)]) && 
               ( (time-lastKeyb >= REFRESH_RATE) || 
                 (lastKey != option->getKey(DNT_KEY_WINDOW_BRIEFING)) ) )
         {
            brief->openWindow(gui);
            lastKey = option->getKey(DNT_KEY_WINDOW_BRIEFING);
            lastKeyb = time;
         }

         /* Open Close Inventory */
         if( (keys[option->getKey(DNT_KEY_WINDOW_INVENTORY)]) && 
               ( (time-lastKeyb >= REFRESH_RATE) || 
                 (lastKey != option->getKey(DNT_KEY_WINDOW_INVENTORY)) ) )
         {
            openCloseInventoryWindow(); 
            lastKey = option->getKey(DNT_KEY_WINDOW_INVENTORY);
            lastKeyb = time;
         }

         /* Open Character Info Window */
         if( (keys[option->getKey(DNT_KEY_WINDOW_CHARACTER)]) && 
               ( (time-lastKeyb >= REFRESH_RATE) || 
                 (lastKey != option->getKey(DNT_KEY_WINDOW_CHARACTER)) ) ) 
         {
            lastKey = option->getKey(DNT_KEY_WINDOW_CHARACTER);
            lastKeyb = time;
            if(charInfoWindow)
            {
               charInfoWindow->open(PCs->getActiveCharacter());
            }
         }

         /* Open Journal Window */
         if( (keys[option->getKey(DNT_KEY_WINDOW_JOURNAL)]) && 
               ( (time-lastKeyb >= REFRESH_RATE) || 
                 (lastKey != option->getKey(DNT_KEY_WINDOW_JOURNAL)) ) ) 
         {
            lastKey = option->getKey(DNT_KEY_WINDOW_JOURNAL);
            lastKeyb = time;
            if(journal)
            {
               if(journal->isOpen())
               {
                  snd->addSoundEffect(SOUND_NO_LOOP, "sndfx/gui/close.ogg");
                  journal->close();
               }
               else
               {
                  snd->addSoundEffect(SOUND_NO_LOOP, "sndfx/gui/turn_page.ogg");
                  journal->open(gui);
               }
            }
         }


         /* Pass time */
         if( (keys[SDLK_0]) && 
               ( (time-lastKeyb >= REFRESH_RATE) || 
                 (lastKey != SDLK_0) ) )
         {
            lastKey = SDLK_0;
            lastKeyb = time;
            hour += 0.1;
         }

         /* Toggle Run state */
         if( (keys[option->getKey(DNT_KEY_TOGGLE_RUN_1)]) || 
               (keys[option->getKey(DNT_KEY_TOGGLE_RUN_2)]) )
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
         if( (keys[option->getKey(DNT_KEY_MOVE_LEFT)]) || 
               (keys[option->getKey(DNT_KEY_MOVE_RIGHT)]) )
         {
            walkStatus = ENGINE_WALK_KEYS;
            varX = curWalkInterval * 
               sin(deg2Rad(activeCharacter->scNode->getAngleY()+90.0f));
            varZ = curWalkInterval * 
               cos(deg2Rad(activeCharacter->scNode->getAngleY()+90.0f));
            // Left walk
            if(keys[option->getKey(DNT_KEY_MOVE_LEFT)]) 
            {
               varX *= -1;
               varZ *= -1;
            }

            walked |= tryWalk(varX, varZ);

         }
         else if( (keys[option->getKey(DNT_KEY_MOVE_FORWARD)]) || 
               (keys[option->getKey(DNT_KEY_MOVE_BACKWARD)]) )
         { 
            walkStatus = ENGINE_WALK_KEYS;
            varX = curWalkInterval * 
               sin(deg2Rad(activeCharacter->scNode->getAngleY()));
            varZ = curWalkInterval * 
               cos(deg2Rad(activeCharacter->scNode->getAngleY()));
            if(keys[option->getKey(DNT_KEY_MOVE_FORWARD)]) 
            {
               varX *= -1;
               varZ *= -1;
            }

            walked |= tryWalk(varX, varZ);
         }

         if( (keys[option->getKey(DNT_KEY_ROTATE_LEFT)]) || 
               (keys[option->getKey(DNT_KEY_ROTATE_RIGHT)]))
         {
            GLfloat ori = activeCharacter->scNode->getAngleY();
            walkStatus = ENGINE_WALK_KEYS;
            // CounterClockWise Character turn
            if( (keys[option->getKey(DNT_KEY_ROTATE_LEFT)]) && 
                  (canWalk(0,0,TURN_VALUE)) )  
            {
               ori += TURN_VALUE;
               if(ori > 360.0f)
               { 
                  ori -= 360.0f;
               }
               activeCharacter->scNode->setAngle(0.0f, ori, 0.0f);
               walked = true;
            }
            // Clockwise Character Turn
            if( (keys[option->getKey(DNT_KEY_ROTATE_RIGHT)]) && 
                  (canWalk(0,0,-TURN_VALUE)) )
            {
               ori -= TURN_VALUE;
               if(ori < 0.0f)
               {
                  ori += 360.0f;
               }
               activeCharacter->scNode->setAngle(0.0f, ori, 0.0f);
            }
            walked = true;
         }
         if(keys[option->getKey(DNT_KEY_CHANGE_CHARACTER)]) //Activate Character
         {
            walkStatus = ENGINE_WALK_KEYS;
            if(keys[SDLK_LCTRL]) //Previous Character
            {
               PCs->setActiveCharacter(
                     (character*)activeCharacter->getPrevious());
            }
            else //Next Character
            {
               PCs->setActiveCharacter((character*)activeCharacter->getNext());
            }
            activeCharacter = PCs->getActiveCharacter();
            gameCamera.updateCamera(activeCharacter->scNode->getPosX(),
                  activeCharacter->scNode->getPosY(), 
                  activeCharacter->scNode->getPosZ(),
                  activeCharacter->scNode->getAngleY());
            SDL_Delay(100);
         }

         /* Camera Verification */
         gameCamera.doIO(keys, mButton, x, y, mouseWheel, DELTA_CAMERA );

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
            if(activeCharacter->pathFind.getState() == ASTAR_STATE_RUNNING)
            {
               /* Must stop the A* */
               activeCharacter->pathFind.clear();
            }
         }
         else if( (walkPressTime != 0) && !(mButton & SDL_BUTTON(3)) && 
               (pcHaveWalkAction()) )
         {
            /* Path Verification */
            walkPressTime = 0;

            activeCharacter->pathFind.defineMap(actualMap);

            activeCharacter->pathFind.findPath(activeCharacter, xReal, zReal, 
                  activeCharacter->walk_interval, NPCs, PCs, 
                  engineMode == ENGINE_MODE_TURN_BATTLE);
         }

         /* Verify Continuous Walk with Mouse */
         if(walkStatus == ENGINE_WALK_MOUSE)
         {
            cursors->set(CURSOR_WALK_CONT);
            if(mButton & SDL_BUTTON(3))
            {
               /* Set character orientation (if mouse is far from character,
                * because if it is too near, some weird angles appears) */
               dist = sqrt( (xFloor - activeCharacter->scNode->getPosX()) *
                     (xFloor - activeCharacter->scNode->getPosX()) +
                     (zFloor - activeCharacter->scNode->getPosZ()) *
                     (zFloor - activeCharacter->scNode->getPosZ()) );

               walkAngle = getAngle(activeCharacter->scNode->getPosX(),
                     activeCharacter->scNode->getPosZ(),
                     xFloor, zFloor);
               if(dist > 4)
               {
                  /* Try to change the angle */
                  if(canWalk(0, 0, 
                           walkAngle-activeCharacter->scNode->getAngleY()))
                  { 
                     /* can change */
                     activeCharacter->scNode->setAngle(0.0f, walkAngle, 0.0f);
                  }

                  /* Verify if is running or walking */
                  run = dist >= ENGINE_CONTINUOUS_RUN_DISTANCE;
               }
               else
               {
                  /* Keep the direction angle */
                  walkAngle = activeCharacter->scNode->getAngleY();
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
         int astate = activeCharacter->pathFind.getState();
         if(astate == ASTAR_STATE_FOUND)
         {
            /* Found path to, so walk */
            walkStatus = ENGINE_WALK_MOUSE_ASTAR;
         }
         else if(astate == ASTAR_STATE_NOT_FOUND)
         {
            brief->addText(gettext("A* could not find a path!"), 
                  220,20,220, true);
         }

         if(walkStatus == ENGINE_WALK_MOUSE_ASTAR)
         {
            float pX=activeCharacter->scNode->getPosX();
            float pY=activeCharacter->scNode->getPosY();
            float pZ=activeCharacter->scNode->getPosZ();
            float aY=activeCharacter->scNode->getAngleY();
            if(! activeCharacter->pathFind.getNewPosition(pX,pZ,aY,
                     engineMode == ENGINE_MODE_TURN_BATTLE, run))
            {
               walkStatus = ENGINE_WALK_KEYS;
            }
            else
            {
               /* Define New Occuped Square */
               activeCharacter->scNode->set(pX,pY,pZ,0.0f,aY,0.0f);
               activeCharacter->defineOcSquare(actualMap);

               /* Define New Height */
               defineCharacterHeight(activeCharacter, 
                     activeCharacter->scNode->getPosX(),
                     activeCharacter->scNode->getPosZ());
            }

            gameCamera.updateCamera(activeCharacter->scNode->getPosX(),
                  activeCharacter->scNode->getPosY(),
                  activeCharacter->scNode->getPosZ(),
                  activeCharacter->scNode->getAngleY());
            walked = true;
         }

         /* IA cycle */
         treatScripts();
      }

      /* A* cycle */
      doAStar();

      /* GUI Events */

      /* Update MiniMap */
      mapWindow->updateCharacterPosition(activeCharacter->scNode->getPosX(),
                                         activeCharacter->scNode->getPosZ());

      /* Get GUI Event */ 
      if(!callCampaignScript)
      {
         Farso::GuiObject* object;
         object = gui->manipulateEvents(x,y,mButton,keys, guiEvent);
         /* Threat the GUI */
         treatGuiEvents(object, guiEvent);
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
      actualFPS = (actualFPS + (1000.0f / (SDL_GetTicks() - lastRead))) / 2;

      /* Limit its max */
      if(actualFPS > 1000)
      {
         actualFPS = 1000;
      }

      /* Show it! */
      if( time-lastFPS >= 500 )
      {
         /* FIXME -> reshow FPS! */
         std::stringstream sconv;
         std::string fpsStr;
         sconv << actualFPS;
         sconv >> fpsStr;
         fpsText->setText(fpsStr);
         lastFPS = time;
      }
      
      /* Verify Sounds FIXME -> for npc sounds! */
      if( (walked) && (activeCharacter->isAlive()) )
      {
         /* Make sure no conversation or barter is opened */
         dialogWindow dlgWindow;
         barterWindow bartWindow;
         dlgWindow.close();
         bartWindow.close();

//FIXME: Disabled sound for walk, until we'll do one for run
//       and for NPCS (anyway, its too bad for now)
#if 0       
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
#endif         

         /* Set the animation (if not yet defined) */
         if(run)
         {
            activeCharacter->scNode->getModel()->setState(STATE_RUN);
         }
         else
         {
            activeCharacter->scNode->getModel()->setState(STATE_WALK);
         }
      }
      else if( (timePass) && (activeCharacter->isAlive()))
      { 
         /* The move stoped (or never occurred)  */
         if( ( (activeCharacter->scNode->getModel()->getState()==STATE_WALK) ||
               (activeCharacter->scNode->getModel()->getState()==STATE_RUN) ) &&
               (engineMode == ENGINE_MODE_TURN_BATTLE) && 
               (fightStatus == FIGHT_PC_TURN) )
         {
            /* Stoped, so must set that cannot move */
            activeCharacter->setCanMove(false);

            /* And verify if can attack */
            if(walkDistance > activeCharacter->displacement)
            {
               activeCharacter->setCanAttack(false);
            }
            walkDistance = 0;
         }

         /* Put at Idle animation */
         activeCharacter->scNode->getModel()->setState(STATE_IDLE);

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
 *                            clearOpenGL                           *
 ********************************************************************/
void engine::clearOpenGL()
{
   if(option->getStencilBufferSize() > 0)
   {
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | 
            GL_STENCIL_BUFFER_BIT);
   }
   else
   {
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   }
}

/********************************************************************
 *                            RenderScene                           *
 ********************************************************************/
void engine::renderScene(bool lightPass, bool updateAnimations)
{
   boundingBox bbox;
   bool shadow = ( (option->getShadowType() == SHADOWS_PROJECTIVE) && 
                   (actualMap->isOutdoor()) && 
                   (gameSun->shadowTime()) );

   /* Renable fog */
   if(!lightPass)
   {
      glEnable(GL_LIGHTING);
      glEnable(GL_FOG);
   }

   /* SKY */
   if( (!lightPass) && (actualMap->isOutdoor()) )
   {
      glPushMatrix();
         glTranslatef(gameCamera.getCameraX(), 0.0f, gameCamera.getCameraZ());
         gameSky->draw(actualMap, gameSun->getRotation(), 
                       !actualMap->getFog().enabled);
      glPopMatrix();
   }

   if((!lightPass) && (option->getStencilBufferSize() > 0))
   {

      glPushMatrix();
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
      glPopMatrix();
   }

   /* Render terrain for shadows */
   if(shadow)
   {
      glPushMatrix();
      actualMap->render(gameCamera.getCameraX(), gameCamera.getCameraY(),
            gameCamera.getCameraZ(), visibleMatrix,
            PCs->getActiveCharacter()->scNode->getPosX(),
            PCs->getActiveCharacter()->scNode->getPosZ());
      glPopMatrix();
   }

   /* Draw Scene */
   curScene.render(visibleMatrix, updateAnimations, 
         /* Enable reflexion */
         ((option->getReflexionType() >= REFLEXIONS_CHARACTERS) &&
          (!actualMap->isOutdoor()) &&
          (option->getStencilBufferSize() > 0)),
          /* Enable Projective Shadow */
         ((shadow) && (gameSun->visibleTime())),
         gameSun->getShadowMatrix(), gameSun->getShadowAlpha());

   /* Render Terrain at last for reflexion */
   if(!shadow)
   {
      glPushMatrix();
      actualMap->render(gameCamera.getCameraX(), gameCamera.getCameraY(),
            gameCamera.getCameraZ(), visibleMatrix,
            PCs->getActiveCharacter()->scNode->getPosX(),
            PCs->getActiveCharacter()->scNode->getPosZ());
      glPopMatrix();
   }
}

/********************************************************************
 *                       RenderNoShadowThings                       *
 ********************************************************************/
void engine::renderNoShadowThings()
{

   activeCharacter = PCs->getActiveCharacter(); 

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
                                     activeCharacter->scNode->getPosX() - 
                                                  activeCharacter->displacement,
                                     activeCharacter->scNode->getPosZ() - 
                                                  activeCharacter->displacement,
                                     activeCharacter->scNode->getPosX() + 
                                                  activeCharacter->displacement,
                                     activeCharacter->scNode->getPosZ() + 
                                                  activeCharacter->displacement,
                                     0.05f, 20);
   }

   /* The Current Connection */
   if(option->getAlwaysHighlightConnections())
   {
      actualMap->renderConnections();
   }
   else if(curConection)
   {
      actualMap->renderConnection(curConection);
   }


   if( (occPosition) && (walkStatus != ENGINE_WALK_MOUSE) )
   {
      /* Can walk to position mark */
      actualMap->renderSurfaceOnMap(canOccImage,
                                    xReal-4, zReal-4,
                                    xReal+4, zReal+4,
                                     0.075f, 20);
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
                  moveCircleX-2*turnCharacter->displacement,
                  moveCircleZ-2*turnCharacter->displacement,
                  moveCircleX+2*turnCharacter->displacement, 
                  moveCircleZ+2*turnCharacter->displacement,
                  0.3f,12);
         }
         /* Normal Circle */
         actualMap->renderSurfaceOnMap(normalMoveCircle,
               moveCircleX-turnCharacter->displacement,
               moveCircleZ-turnCharacter->displacement,
               moveCircleX+turnCharacter->displacement, 
               moveCircleZ+turnCharacter->displacement,
               0.4f,20);
      }
      if( (turnCharacter->getCanAttack()) && 
          (walkDistance < turnCharacter->displacement) )
      {
         /* Feat Range Circle */
         float rangeValue = turnCharacter->getActiveFeatRange();
         actualMap->renderSurfaceOnMap(featRangeCircle, 
               turnCharacter->scNode->getPosX()-rangeValue,
               turnCharacter->scNode->getPosZ()-rangeValue, 
               turnCharacter->scNode->getPosX()+rangeValue, 
               turnCharacter->scNode->getPosZ()+rangeValue, 
               0.5f,20);
      }
   }

   /* Draw Area-Feat radius circle */
   if((engineMode == ENGINE_MODE_TURN_BATTLE) && 
      (activeCharacter->getCanAttack()) &&
      (fightStatus == FIGHT_PC_TURN) && 
      (activeCharacter->getActiveFeatPtr()->info->type == FEAT_TYPE_ON_AREA))
   {
      /* Feat Range Circle */
      float rangeValue = activeCharacter->getActiveFeatPtr()->info->radius;
      actualMap->renderSurfaceOnMap(featAreaCircle,
            xReal-rangeValue, zReal-rangeValue,
            xReal+rangeValue, zReal+rangeValue,
            0.6f,20);
   }


   /* Draw A* movimentation things */
   if(walkStatus == ENGINE_WALK_MOUSE_ASTAR)
   {
      GLfloat destX =0, destZ=0;
#ifdef DEBUG_ASTAR
      activeCharacter->pathFind.drawPath();
#endif
      activeCharacter->pathFind.getDestiny(destX, destZ);
      
       /* Draw Movimentation Destiny */
       if(destinyVariation >= 2.0f)
       {
          destinyVariation = -2.0f;
       }
       destinyVariation += 0.1f;

       actualMap->renderSurfaceOnMap(destinyImage,
                                     destX - (4 + destinyVariation), 
                                     destZ - (4 + destinyVariation),
                                     destX + (4 + destinyVariation),
                                     destZ + (4 + destinyVariation),
                                     0.25f,4);
   }

   /* Draw Particles */
   if(option->getEnableParticles())
   {
      glPushMatrix();
         particleController.updateAll(visibleMatrix, option->getEnableGrass());
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
      glPushMatrix();
         fpsGui->draw(proj, modl, viewPort);
      glPopMatrix();

      /* Mouse Cursor */
      glPushMatrix();
         if(walkStatus == ENGINE_WALK_MOUSE)
         {
            /* Set the cursor to the current walkAngle orientation */
            if(PCs->getActiveCharacter()->scNode->getModel()->getState() 
                  == STATE_RUN)
            {
               /* Is running, must scale a bit the cursor to demonstrate this */
               cursors->draw(mouseX, mouseY,
                             walkAngle - gameCamera.getPhi(),
                             1.0f, 1.2f, 1.0f);
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
void engine::drawWithShadows(bool flush)
{
   // FIXME: shadows for indoor maps
   GLfloat sunPos[4];

   /* Clear and update things */
   updateBeforeRender();

   /* Define Light View Projetion */
   gameSun->getPosition(sunPos);
   shadowMap.defineLightView(sunPos[0], sunPos[1], sunPos[2],
                       (actualMap->squareSize()*actualMap->getSizeX())/2.0f, 
                       (actualMap->squareSize()*actualMap->getSizeZ())/2.0f);
   
   /* Update frustum for light view */
   updateFrustum(visibleMatrix,proj,modl);
   
   /* Render the Scene from light view */
   shadowMap.beginLightRender();
   renderScene(true, true);
   shadowMap.endLightRender();

   /* Now set to the camera view and render again! */
   glViewport(0, 0, Farso::SCREEN_X, Farso::SCREEN_Y);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f, Farso::SCREEN_X / (float)Farso::SCREEN_Y, 1.0f, 
                  OUTDOOR_FARVIEW);
   glGetIntegerv(GL_VIEWPORT, viewPort);
   glGetFloatv(GL_MODELVIEW_MATRIX, camProj);
   glMatrixMode(GL_MODELVIEW);
   clearOpenGL();
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

#ifdef DEBUG_ASTAR
   /* Debug for a* */
   activeCharacter->pathFind.drawSearch();
#endif
   
   if(flush)
   {
      /* Flush */
      glFlush();
      SDL_GL_SwapBuffers();

      printOpenGLErrors();
   }
}

/*********************************************************************
 *                       Draw Scene Function                         *
 *********************************************************************/
void engine::drawWithoutShadows(bool flush)
{
   /* Clear and update things before render */
   updateBeforeRender();
   updateFrustum(visibleMatrix,proj,modl);
   
   /* Render all things */
   renderScene(false, true);
   renderNoShadowThings();

   renderGUI();

#ifdef DEBUG_ASTAR
   /* Debug for a* */
   activeCharacter->pathFind.drawSearch();
#endif

   /* Flush */
   if(flush)
   {
      glFlush();
      SDL_GL_SwapBuffers();

      printOpenGLErrors();
   }
}

/*********************************************************************
 *                          updateBeforeRender                       *
 *********************************************************************/
void engine::updateBeforeRender()
{
   clearOpenGL();
   glLoadIdentity();

   /* Apply effects and characters' influences */
   if(PCs)
   {
      PCs->update();
   }
   if(NPCs)
   {
      NPCs->update();
   }

   /* Redefine camera position */
   gameCamera.lookAt(actualMap);

   snd->setListenerPosition(gameCamera.getCenterX(), gameCamera.getCenterY(),
                            gameCamera.getCenterZ(), gameCamera.getTheta(),
                            gameCamera.getPhi(), gameCamera.getD(),
                            gameCamera.getDeltaY());

   /* Sun Definition */
   if(actualMap->isOutdoor())
   {
      gameSun->updateHourOfDay(hour, 
            (actualMap->getSizeX()/2.0f)*actualMap->squareSize(), 
            (actualMap->getSizeZ()/2.0f)*actualMap->squareSize());
      
      //PCs->getActiveCharacter()->xPosition,
      //                         PCs->getActiveCharacter()->zPosition);
      gameSun->setLight();
   }
   else
   {
      gameSun->disableLight();
   }
   /* Other light definitions */
   actualMap->setLights(PCs->getActiveCharacter()->scNode->getPosX(),
                        PCs->getActiveCharacter()->scNode->getPosZ());
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
      activeCharacter->scNode->setPosition(
            activeCharacter->scNode->getPosX() + varX,
            activeCharacter->scNode->getPosY(),
            activeCharacter->scNode->getPosZ() + varZ);

      gameCamera.updateCamera(activeCharacter->scNode->getPosX(),
            activeCharacter->scNode->getPosY(),
            activeCharacter->scNode->getPosZ(),
            activeCharacter->scNode->getAngleY());
      return(true);
   }

   /* Can't, so try only X move */
   else if(((varX > 0)&&(canWalk(activeCharacter->walk_interval,0,0))) ||
           ((varX < 0)&&(canWalk(-activeCharacter->walk_interval,0,0))) )
   {
      if(varX < 0)
      {
         activeCharacter->scNode->setPosition(
            activeCharacter->scNode->getPosX() - activeCharacter->walk_interval,
            activeCharacter->scNode->getPosY(),
            activeCharacter->scNode->getPosZ());
      }
      else
      {
         activeCharacter->scNode->setPosition(
            activeCharacter->scNode->getPosX() + activeCharacter->walk_interval,
            activeCharacter->scNode->getPosY(),
            activeCharacter->scNode->getPosZ());
      }
      gameCamera.updateCamera(activeCharacter->scNode->getPosX(),
            activeCharacter->scNode->getPosY(),
            activeCharacter->scNode->getPosZ(),
            activeCharacter->scNode->getAngleY());
      return(true);
   }

   /* Can't too, so try only Z move */
   else if(((varZ > 0) && canWalk(0,activeCharacter->walk_interval,0)) ||
           ((varZ < 0) && canWalk(0,-activeCharacter->walk_interval,0)) )
   {
      if(varZ < 0)
      {
         activeCharacter->scNode->setPosition(
            activeCharacter->scNode->getPosX(),
            activeCharacter->scNode->getPosY(),
            activeCharacter->scNode->getPosZ()-activeCharacter->walk_interval);
      }
      else
      {
         activeCharacter->scNode->setPosition(
            activeCharacter->scNode->getPosX(),
            activeCharacter->scNode->getPosY(),
            activeCharacter->scNode->getPosZ()+activeCharacter->walk_interval);
      }
      gameCamera.updateCamera(activeCharacter->scNode->getPosX(),
            activeCharacter->scNode->getPosY(),
            activeCharacter->scNode->getPosZ(),
            activeCharacter->scNode->getAngleY());
      return(true);
   }

   /* Can't move! */
   return(false);
}

/*********************************************************************
 *                         pcHaveWalkAction                          *
 *********************************************************************/
bool engine::pcHaveWalkAction()
{
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
   }

   /* Can try to move */
   return(true);
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
  
   if(!pcHaveWalkAction())
   {
      return(false);
   }

   /* Verify walk limits on fight mode */
   if((engineMode == ENGINE_MODE_TURN_BATTLE) && 
         (fightStatus == FIGHT_PC_TURN))
   {
      /* Calculate distance to the orign point*/ 
      walkDistance = sqrt( 
            (activeCharacter->scNode->getPosX() + varX - moveCircleX) *
            (activeCharacter->scNode->getPosX() + varX - moveCircleX) +
            (activeCharacter->scNode->getPosZ() + varZ - moveCircleZ) *
            (activeCharacter->scNode->getPosZ() + varZ - moveCircleZ) );

      /* Verify walk limits at FightMode */
      if( ( (activeCharacter->getCanAttack()) && 
            (walkDistance > 2*activeCharacter->displacement) ) || 
          ( (!activeCharacter->getCanAttack()) &&
            (walkDistance > activeCharacter->displacement) )  )
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
      activeCharacter->scNode->setPosition(
          activeCharacter->scNode->getPosX(),
          activeCharacter->scNode->getPosY() + varHeight,
          activeCharacter->scNode->getPosZ());
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
                                       gui, infoWindow, this);

      /* Play a sound on inventory window open */
      snd->addSoundEffect(SOUND_NO_LOOP, "sndfx/gui/cloth-inventory.ogg");
   }
   else
   {
      snd->addSoundEffect(SOUND_NO_LOOP, "sndfx/gui/close.ogg");
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
      pers = (character*) pers->getNext();
   }
}

/*********************************************************************
 *                             showImage                             *
 *********************************************************************/
void engine::showImage(string fileName)
{
   GLuint id;
   int w=0,h=0;
   Uint8 mButton = 0;
   SDL_Surface* img = IMG_Load(fileName.c_str()); 
   glDisable(GL_LIGHTING);
   glGenTextures(1,&id);
   Farso::setTextureRGBA(img,id);
   w = img->w;
   h = img->h;
   SDL_FreeSurface(img);

   fadeInTexture(id,0,0,Farso::SCREEN_X-1,Farso::SCREEN_Y-1, w, h);

   /* Wait until Mouse Button pressed */
   while(!(mButton & SDL_BUTTON(1)))
   {
      //Wait for Mouse Button Release
      SDL_PumpEvents();
      clearEventsQueue();
      int x,y;
      mButton = SDL_GetMouseState(&x,&y);

      clearOpenGL();
      draw2DMode();
      glColor3f(1.0f, 1.0f, 1.0f);
      Farso::textureToScreen(id, 0, 0, Farso::SCREEN_X-1, 
            Farso::SCREEN_Y-1, w, h);
      draw3DMode(option->getFarViewFactor()*OUTDOOR_FARVIEW);
      glFlush();
      SDL_GL_SwapBuffers();

      SDL_Delay(50);
   }

   fadeOutTexture(id,0,0,Farso::SCREEN_X-1,Farso::SCREEN_Y-1, w, h);
   
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
 *                        getCurrentCampaign                         *
 *********************************************************************/
DNT::Campaign* engine::getCurrentCampaign()
{
   return(curCampaign);
}

/*********************************************************************
 *                          getCurrentMap                            *
 *********************************************************************/
Map* engine::getCurrentMap()
{
   return(actualMap);
}

/*********************************************************************
 *                        clearEventsQueue                           *
 *********************************************************************/
void engine::clearEventsQueue()
{
   /* Clear events */
   SDL_Event event;
   while(SDL_PollEvent(&event))
   {
   }
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
   actualFPS = 10.0f;
   lastFPS = 0;

   /* Unhide All Windows */
   gui->showAll();

   /* Open Needed windows */
   if(!commingBack)
   {
      mapWindow->open(gui,0,0, actualMap);
      brief->openWindow(gui);
      shortcuts->open(gui);
   }

   /* Main Things Run */
   while(treatIO(surface))
   {

      /* Wait Die Animation */
      if( (engineMode == ENGINE_MODE_WAIT_DIE_ANIMATION) && 
          ( (SDL_GetTicks() - waitDieTimer) >= ENGINE_WAIT_DIE_DELAY) )
      {
         /* All Pcs are Dead, so Death Screen! */
         snd->loadMusic("music/death_game.ogg");
         showImage(dir.getRealFile("texturas/fightMode/death.png"));

         quitCurrentGame();

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
              pers = (character*) pers->getNext();
           }
           if(!alive) 
           {
              /* All Pcs are Dead wait the dying animation */
              engineMode = ENGINE_MODE_WAIT_DIE_ANIMATION;
              waitDieTimer = SDL_GetTicks();
           }
        }
        /* Continue the fight */
        else if(fightStatus == FIGHT_CONTINUE) 
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

                  moveCircleX = activeCharacter->scNode->getPosX();
                  moveCircleY = activeCharacter->scNode->getPosY();
                  moveCircleZ = activeCharacter->scNode->getPosZ();
               }
               else
               { //FIXME
                  fightStatus = FIGHT_CONTINUE;
               }
           }
           else if(fightStatus == FIGHT_NPC_TURN)
           {
              activeCharacter = fight->actualCharacterTurn();
              moveCircleX = activeCharacter->scNode->getPosX();
              moveCircleY = activeCharacter->scNode->getPosY();
              moveCircleZ = activeCharacter->scNode->getPosZ();
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


