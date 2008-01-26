/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "dccnit.h"
#include "culling.h"
#include "util.h"
#include <math.h>
#include <SDL/SDL_image.h>
#include "../etc/extensions.h"

/*********************************************************************
 *                       Engine's Constructor                        *
 *********************************************************************/
engine::engine()
{
   effect = NULL;
   /* Initialize internal lists */
   gui  = new interface(NULL);
   actualMap = NULL;
   PCs = NULL;
   NPCs = NULL;

   miniMapWindow = NULL;
   shortCutsWindow = NULL;
   inventoryWindow = NULL;
   tradeWindow = NULL;

   imgNumber = 0;
   actualScreen = NULL;

   curConection = NULL;

   objectsList = new(lObject);

   walkStatus = ENGINE_WALK_KEYS;

   /* Initialize the Cursor */
   cursors = new(cursor);

   /* Initialize sounds and musics */
   snd = new(sound);
   walkSound = NULL;

   /* Load Options */
   option = new options();

   /* Set sound and music volume, based on options */
   snd->changeVolume(option->musicVolume, option->sndfxVolume);

   /* Set Camera, based on options */
   gameCamera.defineCameraType(option->cameraNumber);

   /* Set Language, base on options */
   option->setLanguage();

   /* Define all OpenGL Extensions used */
   extensions ext;
   ext.defineAllExtensions();

   /* Load Skills List */
   skillsList = new skills(dir.getRealFile("skills/"),
                           dir.getRealFile("skills/skills.skl"));
  
   /* Load Features List */
   features = new featsList(dir.getRealFile("feats/"),
                            dir.getRealFile("feats/feats.ftl"));

   /* Load Alignments & Tendecies */
   alignList = new aligns(dir.getRealFile("alignment/"),
                          dir.getRealFile("alignment/alignment.lst"));

   /* Load Races */
   raceList = new races(dir.getRealFile("races/"), 
                        dir.getRealFile("races/races.lst"));

   /* Load Classes */
   classList = new classes(dir.getRealFile("classes/"),
                           dir.getRealFile("classes/classes.lst"));

   /* Load Weapons Types */
   weaponsTypes = new weaponTypes();

   /* Create 3D Models List */
   models = new modelList();

   /* Create Special Windows */
   infoWindow = new itemWindow(gui);

   /* Initialize readModes variables */
   lastRead = SDL_GetTicks();
   lastMouse = lastRead;
   lastKeyb = lastRead;
   mouseX = 0;
   mouseY = 0;

   showRange = false;

   /* Create the particle System */
   particleSystem = new partSystem();

   /* Create the message controller */
   msgController = new messageController();

   /* Create the fight system */
   fight = new fightSystem(msgController, particleSystem);

   /* Create the action Controller */
   actionControl = new actionController();

   /* Create the missions controller */
   missions = new missionsController(this);

   /* FIXME remove from here the addMission */
   missions->addNewMission(dir.getRealFile("missions/tutorial/mission1.dsl"));

   hour = 9.0;
   gameSun = new sun(hour, HALFFARVIEW, HALFFARVIEW);

   engineMode = ENGINE_MODE_REAL_TIME;

   destinyVariation = -2.0;
   lastKey = 0;

   /* Colors */
   int i;
   for(i = 0; i < 3; i++)
   {
      defaultColor[i] = 0.2;
      blackColor[i] = 0.0;
   }
   defaultColor[3] = 1.0;
   blackColor[3] = 1.0;

#ifdef VIDEO_MODE
   startVideo = false;
#endif

}

/*********************************************************************
 *                         Engine Destructor                         *
 *********************************************************************/
engine::~engine()
{
   /* Stops and free music & sounds */
   delete(snd);

   /* Delete particles */
   if(particleSystem != NULL)
   {
      delete(particleSystem);
   }

   /* Close option */
   delete(option);

   /* Clear Sky */
   delete(gameSky);

   /* Delete message Controller */
   delete(msgController);

   /* Delete missions controller */
   delete(missions);

   /* Delete fight system */
   delete(fight);

   /* Clear Other Textures */
   glDeleteTextures(1, &normalMoveCircle);
   glDeleteTextures(1, &fullMoveCircle);
   glDeleteTextures(1, &destinyImage);
   glDeleteTextures(1, &rangeCircle);
   glDeleteTextures(1, &featRangeCircle);

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

   /* Clear the Barter Window */
   if(tradeWindow)
   {
      delete(tradeWindow);
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

   /* Clear GUI */
   if(gui)
      delete(gui);

   /* Clear Maps */
   if(actualMap)
   {
      delete(actualMap);
   }
 
   /* Clear Cursors */
   delete(cursors);

   /* Delete the action controller */
   delete(actionControl);

   /* Delete the list of objects */
   delete(objectsList);

   /* Clear 3D Models List */
   delete(models);
   
   /* Clear Characters Lists */
   delete(alignList);
   delete(features);
   delete(raceList);
   delete(classList);
   delete(skillsList);
   delete(weaponsTypes);
}

/*********************************************************************
 *                       Informations Screen                         *
 *********************************************************************/
void engine::InformationScreen()
{
   Uint8 *keys;
   SDL_Surface* img;
   img = IMG_Load(dir.getRealFile("/texturas/general/info.png").c_str());

   GLuint texturaInfo;
   setTextureRGBA(img,&texturaInfo);

   glDisable(GL_LIGHTING);
   actualizeFrustum(visibleMatrix,proj,modl);
   textureToScreen(texturaInfo,proj,modl,viewPort,272,44,527,555,256,213,
                   0.0001);
   glEnable(GL_LIGHTING);
   glFlush();
   SDL_GL_SwapBuffers();

   SDL_Delay(100);
   SDL_PumpEvents();
   keys = SDL_GetKeyState(NULL);

   /* Wait user requests to exit from this screen */
   while(!(keys[SDLK_F1] || keys[SDLK_ESCAPE] || keys[SDLK_RETURN]) )
   {
       SDL_Delay(40);
       SDL_PumpEvents();
       keys = SDL_GetKeyState(NULL);
   }

   glDeleteTextures(1,&texturaInfo);
   SDL_Delay(100);
   lastRead = SDL_GetTicks();
}

/*********************************************************************
 *                                 load                              *
 *********************************************************************/
void engine::load()
{
   //TODO
   modifState.loadState("");
}

/*********************************************************************
 *                                 save                              *
 *********************************************************************/
void engine::save()
{
   //TODO
   modifState.saveState("");
}

/*********************************************************************
 *                                loadPCs                            *
 *********************************************************************/
void engine::loadPCs()
{
   character* per;
   if(PCs)
   {
      delete(PCs);
   }
   PCs  = new (characterList);
   per = PCs->insertCharacter(dir.getRealFile("characters/pcs/metaleiro.pc"),
                              features, this);
}

/*********************************************************************
 *                         Load Map to Engine                        *
 *********************************************************************/
int engine::LoadMap(string arqMapa, int RecarregaPCs)
{
   healthBar* progress = new healthBar(2,20,253,30);
   progress->defineMaxHealth(10);
   progress->defineActualHealth(0);

   dntFont fnt;
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;

   char texto[512];
   string arqVelho = "nada";
   curConection = NULL;
   
   walkStatus = ENGINE_WALK_KEYS;
      
   glClearColor(0,0,0,1);
   glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
   glDisable(GL_LIGHTING);
   SDL_Surface* fig;
   fig = IMG_Load(dir.getRealFile("texturas/general/carregar.png").c_str());

   /* Initializing Load Screen*/
   GLuint texturaCarga;
   setTexture(fig,&texturaCarga);
   SDL_FreeSurface(fig);
   

   SDL_Surface* img = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       256,32,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   
   color_Set(0,0,0);
   rectangle_Fill(img,0,0,255,31);
   color_Set(200,20,20);
   fnt.defineFont(DNT_FONT_TIMES,10);
   sprintf(texto,gettext("Loading Map: %s"),arqMapa.c_str());
   GLuint texturaTexto;
   fadeInTexture(texturaCarga, centerX-128, centerY-64, 
                 centerX+127, centerY+63, 256,128);

   actualizeFrustum(visibleMatrix,proj,modl);
   showLoading(img,&texturaTexto,texturaCarga,
               texto, progress,
               proj, modl, viewPort);
   progress->defineActualHealth(3);

   /* Loading Map */
   if(actualMap) 
   {
     arqVelho = actualMap->getFileName();
     delete(actualMap);
     /* Remove All Unused Objects */
     objectsList->removeUnusedObjects();
     /* Remove All Unused 3D Models */
     models->removeUnusedModels();
   }

   /* Remove All Pending Actions */
   actionControl->removeAllActions();

   actualMap = new Map(objectsList);
   actualMap->setFileName(arqVelho);
   actualMap->open(arqMapa,*models, *weaponsTypes);

   /* Enable, if needed, the FOG */
   if(actualMap->fog.enabled)
   {
      glEnable(GL_FOG);
      {
        glFogi(GL_FOG_MODE,GL_LINEAR);
        glFogfv(GL_FOG_COLOR,actualMap->fog.color);
        glFogf(GL_FOG_DENSITY,actualMap->fog.density);
        glHint(GL_FOG_HINT,GL_DONT_CARE);
        glFogf(GL_FOG_START,actualMap->fog.start);
        glFogf(GL_FOG_END,actualMap->fog.end);
      }
   }
   else
   {
      GLdouble fogEnd = HALFFARVIEW+200;
      GLdouble fogStart = 200;
      GLdouble fogDensity = 0.001;
      GLfloat color[3] = {0.8,0.8,0.8};
      if(!actualMap->isOutdoor())
      {
         color[0] = 0.0;
         color[1] = 0.0;
         color[2] = 0.0;
         fogStart = 40;
         fogDensity = 1.0;
         fogEnd = INDOOR_FARVIEW-2;
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
   if(NPCs)
     delete(NPCs);
   NPCs = NULL;
   character* per;
   if(!actualMap->getNpcFileName().empty())
   {
      FILE* arq;
      if(!(arq = fopen(actualMap->getNpcFileName().c_str(),"r")))
      {
         printf(gettext("Ouch, can't load NPC's file: %s.\n"),
                actualMap->getNpcFileName().c_str());
      }
      else
      {
         NPCs = new (characterList);
         int total;
         int npc;
         char name[30];
         char arquivo[255];
         GLfloat posX, posZ;
         fscanf(arq,"%d",&total);
         for(npc = 0; npc < total; npc++)
         {
           fscanf(arq,"%s %s %f %f",&name[0],&arquivo[0],&posX,&posZ);
           sprintf(texto, gettext("Loading NPC: %s"), name);
           showLoading(img,&texturaTexto,texturaCarga,
                         texto, progress,
                         proj, modl, viewPort);
           per = NPCs->insertCharacter(arquivo,features, this);
           per->xPosition = posX;
           per->zPosition = posZ;
           per->yPosition = actualMap->getHeight(posX, posZ);
           /* Define Occuped Square */
           int posX =(int)floor(per->xPosition / actualMap->squareSize());
           int posZ =(int)floor(per->zPosition / actualMap->squareSize());
           per->ocSquare = actualMap->relativeSquare(posX,posZ);
         }
         fclose(arq);
      }  
   }
   progress->defineActualHealth(6);

   /* Loading PCs */
   if(RecarregaPCs)
   {
       loadPCs(); 
       showLoading(img,&texturaTexto,texturaCarga,
                   gettext("Loading Character"), progress,
                 proj, modl, viewPort);
   }
   progress->defineActualHealth(7);

   /* Loading Internal Windows */
   showLoading(img,&texturaTexto,texturaCarga,
               gettext("Putting GUI on Screen"), progress,
                 proj, modl, viewPort);

   progress->defineActualHealth(8);

   if(miniMapWindow)
   {
     gui->closeWindow(miniMapWindow);
   }
   if(shortCutsWindow)
   {
     gui->closeWindow(shortCutsWindow);
   }
   OpenMiniMapWindow();
   OpenShortcutsWindow();

   /* Updating the BoundingBoxes for PCs */
   int aux;
   per = (character*) PCs->first->next;
   for(aux=0;aux < PCs->getTotal();aux++)
   {
      per->update(0); 
      per->calculateBoundingBox();  
      per = (character*) per->next;
   }

   /* Updating the BoundingBoxes for NPCs */
   if(NPCs)
   { 
      per = (character*) NPCs->first->next;
      for(aux=0; aux < NPCs->getTotal();aux++)
      {
         per->update(0); 
         per->calculateBoundingBox();  
         per = (character*) per->next;
      }
   }

   /* Update  particle System to a stable state */
   showLoading(img,&texturaTexto,texturaCarga,
               gettext("Loading Particles"), progress,
                 proj, modl, viewPort);
   progress->defineActualHealth(9);
   if(!actualMap->getParticlesFileName().empty())
   {
       particleSystem->loadFromFile(actualMap->getParticlesFileName());
       if(option->enableParticles)
       {
          particleSystem->stabilizeAll();
       }
   }
   else
   {
      particleSystem->deleteAll();
   }
   

   /* Put Active Party on Init Position */
   character* activeCharacter = PCs->getActiveCharacter();
   actualMap->getInitialPosition(activeCharacter->xPosition,
                                 activeCharacter->zPosition);
   activeCharacter->yPosition=actualMap->getHeight(activeCharacter->xPosition,
                                                   activeCharacter->zPosition);
   gameCamera.actualizeCamera(activeCharacter->xPosition,
                              activeCharacter->yPosition,
                              activeCharacter->zPosition,
                              activeCharacter->orientation);
   activeCharacter->ocSquare = actualMap->squareInic;

   showLoading(img,&texturaTexto,texturaCarga,
               gettext("Loading Changes..."), progress,
                 proj, modl, viewPort);
   progress->defineActualHealth(10);

   /* Do Modifications */
   modifState.doMapModifications(actualMap);

   /* Change Music, if needed */
   if(!actualMap->getMusicFileName().empty())
   {
      snd->loadMusic(actualMap->getMusicFileName());
   }

   /* Define Map */
   particleSystem->setActualMap(actualMap, &colisionDetect);
   colisionDetect.defineMap(actualMap, NPCs);

   /* Done */
   showLoading(img,&texturaTexto,texturaCarga,
               gettext("Done!"), progress,
                 proj, modl, viewPort);

   glDisable(GL_LIGHTING);
   fadeOutTexture(texturaCarga,centerX-128,centerY-64,
                  centerX+127,centerY+63, 256,128);

   /* Free Loading Textures */
   SDL_FreeSurface(img);
   glDeleteTextures(1,&texturaTexto);

   /* Set the Farview to indoor or outdoor */
   if(actualMap->isOutdoor())
   {
      redefineWindow(actualScreen, FARVIEW);
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
      actualizeFrustum(visibleMatrix,proj,modl);
      glColor3f(i/50.0, i/50.0, i/50.0);
      textureToScreen(id,proj,modl,viewPort,x1,y1,x2,y2,sizeX,sizeY,0.012);
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
      actualizeFrustum(visibleMatrix,proj,modl);
      glColor3f(i/50.0, i/50.0, i/50.0);
      textureToScreen(id,proj,modl,viewPort,x1,y1,x2,y2,sizeX,sizeY,0.012);
      glFlush();
      SDL_GL_SwapBuffers();
      SDL_Delay(10);
   }
   glColor3f(1.0, 1.0, 1.0);
}


/*********************************************************************
 *                             SplashScreen                          *
 *********************************************************************/
void engine::SplashScreen()
{
   GLuint id;
   Uint32 mButton = 0;
   Uint8* keys;
   bool done = false;
   int x,y;
   Uint32 time = SDL_GetTicks();
   glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   actualizeFrustum(visibleMatrix,proj,modl);
   SDL_Surface* img;
   img = IMG_Load(dir.getRealFile("texturas/general/inicio1.png").c_str()); 
   glDisable(GL_LIGHTING);

   dntFont fnt;
   fnt.defineFont(DNT_FONT_TIMES, 20);
   fnt.defineFontAlign(DNT_FONT_ALIGN_CENTER);
   color_Set(48,3,3);
   fnt.write(img, 52, 424, gettext("DccNiTghtmare is developed under Public "
                                   "Domain: Feel free to use its source code "
                                   "and data the way you want (or to blame us "
                                   "the same way). DccNiTghtmare is thankful "
                                   "to the people who developed those "
                                   "libraries: OpenAL, Cal3D, OpenGL, SDL, "
                                   "SDL_image, SDL_ttf, libVorbis."), 
              52, 424, 745, 511);
   fnt.defineFontAlign(DNT_FONT_ALIGN_LEFT);
   setTextureRGBA(img,&id);
   SDL_FreeSurface(img);

   /* Fade In Screen */
   fadeInTexture(id,0,0,SCREEN_X-1,SCREEN_Y-1, 800, 600);

   /* Wait until Mouse Button pressed or time passed */
   while( !done )
   {
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
int engine::InitialScreen(int Status, GLuint idTextura, bool reloadMusic)
{
   /* Reload Music, if needed */
   if(reloadMusic)
   {
      /*music =*/
      snd->loadMusic(dir.getRealFile("music/musica1.ogg"));
   }

   /* Executes Initial Screen */
   actualizeFrustum(visibleMatrix,proj,modl);
   initialScreen* inic = new(initialScreen);
   int result = inic->run(Status, proj, modl, viewPort, idTextura, snd);
   delete(inic);
   return(result);
}

/*********************************************************************
 *                       Call Options Game Screen                    *
 *********************************************************************/
int engine::OptionsScreen(GLuint idTextura)
{
   interface* interf = new interface(NULL);
   int optionW = OPTIONSW_OTHER;
   int tempo = SDL_GetTicks();
   int tempoAnterior = 0;
   Uint8* keys;
   int x,y;
   guiObject* object = NULL;
   int eventInfo = NOTHING;

   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);

   option->displayOptionsScreen(interf);

   while( (optionW != OPTIONSW_CANCEL) && 
          (optionW != OPTIONSW_CONFIRM))
   {
      tempo = SDL_GetTicks();
      if(tempo - tempoAnterior >= ACTUALIZATION_RATE) 
      {
         tempoAnterior = tempo;
         SDL_PumpEvents();
         glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
         glClearColor(0,0,0,1);
         keys = SDL_GetKeyState(NULL);
         Uint8 Mbutton = SDL_GetMouseState(&x,&y);
         object = interf->manipulateEvents(x,y,Mbutton,keys,&eventInfo);
         textureToScreen(idTextura,proj,modl,viewPort,
                         0,0,SCREEN_X-1,SCREEN_Y-1,800,600,0.012);
         glPushMatrix();
            draw2DMode();
            interf->draw(proj,modl,viewPort);
            draw3DMode(FARVIEW);
         glPopMatrix();
         glFlush();
         SDL_GL_SwapBuffers();
         optionW = option->treat(object,eventInfo,interf,proj,modl,viewPort);
      }
      else if((ACTUALIZATION_RATE-1) - (tempo - tempoAnterior) > 0 ) 
      {
         SDL_Delay((ACTUALIZATION_RATE-1) - (tempo - tempoAnterior) );
      }
   }
  
   if(optionW == OPTIONSW_CONFIRM)
   {
      snd->changeVolume(option->musicVolume, option->sndfxVolume);
      gameCamera.defineCameraType(option->cameraNumber);
      /* Change the language */
      option->setLanguage();
   }

   delete(alignList);
   alignList = new aligns(dir.getRealFile("alignment/"),
                          dir.getRealFile("alignment/alignment.lst"));

   delete(features);
   features = new featsList(dir.getRealFile("feats/"),
                            dir.getRealFile("feats/feats.ftl"));

   delete(raceList);
   raceList = new races(dir.getRealFile("races/"), 
                        dir.getRealFile("races/races.lst"));

   glEnable(GL_LIGHTING);
   SDL_ShowCursor(SDL_DISABLE);

   delete(interf);

   return(optionW);
}

/*********************************************************************
 *              Call Screens to Create, Evolute Character            *
 *********************************************************************/
int engine::CharacterScreen(GLuint idTextura)
{
   int charCreation = CHAR_OTHER;
   int tempo = SDL_GetTicks();
   int tempoAnterior = 0;
   Uint8* keys;
   int x,y;
   guiObject* object = NULL;
   int eventInfo = NOTHING;

   int mods[6];

   int status = 0;

   /* First Load PCs */
   loadPCs();

   /* Att Screen */
   attWindow* atWindow = NULL;//new attWindow(sk, gui);

   /* Skill Screen */
   skillWindow* skWindow = NULL;

   /* Alignment Window */
   alignWindow* alWindow = NULL;

   /* Class Window */
   classWindow* clWindow = NULL;

   /* Aspect Window */
   aspectWindow* aspWindow = NULL;

   /* Race Window */
   character* activeCharacter = PCs->getActiveCharacter();
   raceWindow* rcWindow = new raceWindow(raceList,&activeCharacter->sk,gui,
                                         &activeCharacter->actualRace);

   while( (status != 6) )
   {
      tempo = SDL_GetTicks();
      if(tempo - tempoAnterior >= ACTUALIZATION_RATE) 
      {
         tempoAnterior = tempo;
         SDL_PumpEvents();
         glClearColor(0,0,0,1);
         glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
         keys = SDL_GetKeyState(NULL);
         Uint8 Mbutton = SDL_GetMouseState(&x,&y);
         object = gui->manipulateEvents(x,y,Mbutton,keys,&eventInfo);

         textureToScreen(idTextura,proj,modl,viewPort,0,0,
                         SCREEN_X-1,SCREEN_Y-1,800,600,0.012);
         glPushMatrix();
            draw2DMode();
            gui->draw(proj,modl,viewPort);
            draw3DMode(FARVIEW);
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
               clWindow = new classWindow(classList, &activeCharacter->sk,
                                         gui,
                                         &activeCharacter->actualClass[0]);
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
            if(charCreation == ALIGNW_CONFIRM)
            {
               status = 2;
               delete(clWindow);
               alWindow = new alignWindow(alignList, gui, 
                                          &activeCharacter->actualAlign);
            }
            else if(charCreation == ALIGNW_CANCEL)
            {
               status = 0;
               delete(clWindow);
               rcWindow = new raceWindow(raceList, &activeCharacter->sk, 
                                         gui,
                                         &activeCharacter->actualRace);
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
               atWindow = new attWindow(skillsList, &activeCharacter->sk,
                                        gui, mods, false);
            }
            else if(charCreation == ALIGNW_CANCEL)
            {
               status = 1;
               delete(alWindow);
               clWindow = new classWindow(classList, &activeCharacter->sk,
                                         gui,
                                         &activeCharacter->actualClass[0]);
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
               skWindow = new skillWindow(skillsList,&activeCharacter->sk,
                                          gui, activeCharacter->getLevel());
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
               atWindow = new attWindow(skillsList, &activeCharacter->sk,
                                        gui, mods, true);
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
               skWindow = new skillWindow(skillsList,&activeCharacter->sk,
                                          gui, activeCharacter->getLevel());
            }
         }         
      }
      else if((ACTUALIZATION_RATE-1) - (tempo - tempoAnterior) > 0 ) 
      {
         SDL_Delay((ACTUALIZATION_RATE-1) - (tempo - tempoAnterior) );
      }
   }


   /* Actualize and calculate things related to the character */
   if(charCreation == CHAR_CONFIRM)
   {
      /* Calculate Life Points */
      activeCharacter->defineInitialLifePoints();
   }
   
   return(charCreation);
}


/*********************************************************************
 *                     redefine Window to Screen                     *
 *********************************************************************/
void engine::redefineWindow(SDL_Surface *screen, int actualFarView)
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
void engine::Init(SDL_Surface *screen)
{
   actualScreen = screen;  
   redefineWindow(screen, FARVIEW);
   
   /* Clear */
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClearDepth(1.0);
   glClearStencil(0);

   /* ShadowMap */
   shadowMap.init();
   glPolygonOffset(-2.0, -1.0);

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

   /* Culling */
   /*glCullFace(GL_BACK);
   glEnable(GL_CULL_FACE);*/

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

}

/*********************************************************************
 *                              ScreenDump                           *
 *********************************************************************/
#ifdef VIDEO_MODE
void ScreenDump(char *destFile, short W, short H) 
{
  FILE   *out = fopen(destFile, "w");
  char   pixel_data[3*SCREEN_X*SCREEN_Y];
  short  TGAhead[] = {0, 2, 0, 0, 0, 0, W, H, 24};
  glReadBuffer(GL_FRONT);
  glReadPixels(0, 0, W, H, GL_BGR, GL_UNSIGNED_BYTE, pixel_data);
  fwrite(&TGAhead, sizeof(TGAhead), 1, out);
  fwrite(pixel_data, 3*W*H, 1, out);
  fclose(out); 
}
#endif

/*********************************************************************
 *                             rangeAction                           *
 *********************************************************************/
bool engine::rangeAction(GLfloat posX, GLfloat posZ, 
                         GLfloat targX, GLfloat targZ,
                         GLfloat range)
{
   if(! actionInRange(posX, posZ, targX, targZ, range) )
   {
      if(shortCutsWindow != NULL)
      {
         briefTxt->setText(gettext("Too far away for the action!"));
      }
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
  string brief = "";
  string briefInit = "";
  
  character* activeCharacter = PCs->getActiveCharacter();
  
  fight->empty();
  if(!NPCs)
  {
     if(shortCutsWindow != NULL)
     {
        briefTxt->setText(gettext("No NPCs in the area."));
     }
     return;
  }
  ch =(character*) NPCs->first->next;
  while(ch != NPCs->first)
  {
      //TODO put enemies on groups, when enemy from enemy
      fight->insertNPC(ch, 0, briefInit);
      brief += briefInit + "|";
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
      snd->addSoundEffect(false,dir.getRealFile("sndfx/battleMode.ogg"));
      engineMode = ENGINE_MODE_TURN_BATTLE;
      moveCircleX = activeCharacter->xPosition;
      moveCircleY = activeCharacter->yPosition;
      moveCircleZ = activeCharacter->zPosition;

      /* Put the PCs on group */
      ch =(character*) PCs->first->next;
      while(ch != PCs->first)
      {
         fight->insertPC(ch, 0, briefInit);
         brief += briefInit + "|";
         ch = (character*) ch->next; 
         /* Set the state to Idle */
         ch->callIdleAnimation();
         SDL_Delay(1);
      }
                   
      /* Define PC turn, cause the round of surprise attack! */
      if(surprisePC)
      {
         fightStatus = FIGHT_PC_TURN;
         fullMovePCAction = false;
         canMove = true;
         canAttack = true;
         fight->setActualActor(PCs->getActiveCharacter());
         brief += gettext("Surprise Attack Turn.");
         brief += "|";
      }
      else
      {
         fightStatus = FIGHT_CONTINUE;
         fullMovePCAction = false;
         canMove = false;
         canAttack = false;
      }
      attackFeat = activeCharacter->getActiveFeatRangeType();

      /* Abort All pending Actions */
      actionControl->abortAllActions();
      
   }
   else
   {
      brief = gettext("No NPCs in the area.");
   }
   if( (shortCutsWindow != NULL) && (!brief.empty()))
   {
      briefTxt->setText(brief);
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
 *                        treat all scripts                          *
 *********************************************************************/
void engine::treatScripts()
{
   if(engineMode != ENGINE_MODE_TURN_BATTLE)
   {
      /* Treat all pending actions */
      treatPendingActions();

      /* Treat missions scripts */
      missions->treat(actualMap);

      /* Treat NPCs scripts */
      if(NPCs)
      {
         int i;
         character* npc = NPCs->first;
         iaScript* script;
         for(i=0; i < NPCs->getTotal(); i++)
         {
            npc = npc->next;
            script = (iaScript*) npc->getGeneralScript();
            if( (script) && (npc->isAlive()))
            {
               script->defineMap(actualMap);
               script->run(MAX_SCRIPT_LINES);
            }
         }
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
               script->defineMap(actualMap);
               script->run(MAX_SCRIPT_LINES);

               /* End turn when script finished */
               if(script->finished())
               {
                  endTurn();
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
   actionControl->treatActions(actualMap);
}

/*********************************************************************
 *                          Treat GUI Events                         *
 *********************************************************************/
void engine::treatGuiEvents(guiObject* object, int eventInfo)
{
   bool defined = false;

   /* Verify if Inventory Window is opened */
   if(inventoryWindow)
   {
      if(!inventoryWindow->isOpen())
      {
         /* window is no more opened, so free structs */
         OpenCloseInventoryWindow();
      }  
   }

   /* Verify if Barter Window is Opened */
   if(tradeWindow)
   {
      if(!tradeWindow->isOpen())
      {
         delete(tradeWindow);
         tradeWindow = NULL;
      }
      else
      {
         tradeWindow->treat(object, eventInfo);
      }
   }

   /* Verify Dialog Windows */
   if(NPCs != NULL)
   {
      character* ch =(character*) NPCs->first->next;
      while(ch != NPCs->first)
      {
         ch->treatConversation(object, eventInfo, gui, &tradeWindow, 
                               infoWindow);
         ch = (character*) ch->next;
      }
   }

   /* Verify Inventory Window Actions */
   if( (inventoryWindow) )
   {
      if( ( (engineMode == ENGINE_MODE_TURN_BATTLE) && 
            (fightStatus == FIGHT_PC_TURN) ) ||
          (engineMode == ENGINE_MODE_REAL_TIME) )
      {
         if(inventoryWindow->treat(object, eventInfo, cursors, actualMap,
                                   PCs->getActiveCharacter()->xPosition,
                                   PCs->getActiveCharacter()->zPosition,
                                   &modifState))
         {
            /* Redefine, if need, the weapons */
            PCs->getActiveCharacter()->defineWeapon();

            /* Use of Menu count as an action */
            canAttack = false;

            /* TODO redefine the armors! */
         }
      }
   }

   /* Verify Item Window */
   if( infoWindow->isOpen() )
   {
      infoWindow->treat(object, eventInfo);
   }

   /* Verify ShortCutsWindow */
   switch(eventInfo)
   {
       case ON_FOCUS_TAB_BUTTON:
       {
          if(object == (guiObject*) buttonAttackMode)
          {
             if(engineMode != ENGINE_MODE_TURN_BATTLE)
             {
                objTxt->setText(gettext("Enter Battle Mode"));
             }
             else
             {
                objTxt->setText(gettext("Select Normal Attack"));
             }
             defined = true;
          }
          else if(object == (guiObject*) buttonQuest)
          {
            //TODO Do the quest window
            objTxt->setText(gettext("Open Quests Window"));
            defined = true;
          }
          else if(object == (guiObject*) buttonMap)
          {
             if(!miniMapWindow)
             {
                objTxt->setText(gettext("Open Map Window"));
             }
             else
             {
                objTxt->setText(gettext("Map Window already opened!"));
             }
             defined = true;
          }
          else if(object == (guiObject*) buttonEndTurn)
          {
             if(engineMode == ENGINE_MODE_TURN_BATTLE)
             {
                objTxt->setText(gettext("End Character's Turn"));
             }
             else
             {
                objTxt->setText(gettext("Only Avaible on Battle Mode"));
             }
             defined = true;
          }
          else if(object == (guiObject*) buttonInventory)
          {
             if(!inventoryWindow)
             {
                objTxt->setText(gettext("Open Inventory Window"));
             }
             else
             {
                objTxt->setText(gettext("Inventory already opened!"));
             }
             defined = true;
          }
          else if(object == (guiObject*) buttonAssign)
          {
            //TODO Do the assign window
            objTxt->setText(gettext("Open Assign Attacks Window"));
            defined = true;
          }
          else if(object == (guiObject*) buttonCharacter)
          {
            //TODO Do the character window
            objTxt->setText(gettext("View Character Informations"));
            defined = true;
          }
          else if(object == (guiObject*) buttonGroup)
          {
            //TODO Do the group/party window
            objTxt->setText(gettext("Open Group/Party Window"));
            defined = true;
          }
       }
       break;
       case PRESSED_TAB_BUTTON:
       {
           if(object == (guiObject*) buttonAttackMode)
           {
              if( engineMode != ENGINE_MODE_TURN_BATTLE )
              {
                 enterBattleMode(true);
              }
              else
              {
                 //TODO -> set the attack as base one.
              }
           }
           else if( object == (guiObject*) buttonMap)
           {
              /* Open, if not opened, the minimap window */
              if(!miniMapWindow)
              {
                  OpenMiniMapWindow();
              }
           } 
           else if(object == (guiObject*) buttonEndTurn)
           {
              if(fightStatus == FIGHT_PC_TURN)
              {
                 endTurn(); 
              }
           }
           else if(object == (guiObject*) buttonInventory)
           {
              if(!inventoryWindow)
              {
                 OpenCloseInventoryWindow();
              }
           }
           break;
       }
       case PRESSED_BUTTON:
       {
         if(object == (guiObject*) buttonMenu)
         {
            exitEngine = 1;
         }
         else if(object == (guiObject*) buttonSave)
         {
            save();
         }
         else if(object == (guiObject*) buttonLoad)
         {
            load();
         }
         break;
       }
   }

   if( (!defined) && (gui->mouseOnGui(mouseX, mouseY)) )
   {
      objTxt->setText(gettext("Nothing"));
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
   if( (imin >= 10) && (ihour >= 10))
   {
      sprintf(&htmp[0],"%d:%d", ihour, imin);
   }
   else if(imin >= 10)
   {
      sprintf(&htmp[0],"0%d:%d", ihour, imin);
   }
   else if(ihour >= 10)
   {
      sprintf(&htmp[0],"%d:0%d", ihour, imin);
   }
   else
   {
      sprintf(&htmp[0],"0%d:0%d", ihour, imin);
   }
   if(shortCutsWindow)
   {
      hourTxt->setText(htmp);
   }
}

/*********************************************************************
 *                         verifyMouseActions                        *
 *********************************************************************/
int engine::verifyMouseActions(Uint8 Mbutton)
{
   char buf[1024];
   GLfloat wx,wy,wz;
   Uint32 tempo = SDL_GetTicks();
   character* activeCharacter = PCs->getActiveCharacter();

   /* Define Mouse OpenGL Window Coordinate */
   wx = mouseX; wy = SCREEN_Y - mouseY; 
   
   /* Get the Z position of the mouse */
   glReadPixels((int)wx,(int)wy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz); 

   /* Get the world coordinate of the mouse position */
   gluUnProject(wx,wy,wz,modl,proj,viewPort,&xReal,&yReal,&zReal); 

   /* Create a bounding box for the mouse position */
   GLfloat minMouse[3], maxMouse[3];
   minMouse[0] = xReal-2;  maxMouse[0] = xReal+2;
   minMouse[1] = 0.0;      maxMouse[1] = 0.0;
   minMouse[2] = zReal-2;  maxMouse[2] = zReal+2;

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
         if( (sobj->obj) && (sobj->obj->canGet()) )
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
            if(intercepts( minObj, maxObj, minMouse, maxMouse, 1))
            {
                cursors->setActual(CURSOR_GET);
                if(shortCutsWindow)
                {
                   objTxt->setText(sobj->obj->getName()); 
                }
                if( (Mbutton & SDL_BUTTON(1)) && 
                    (rangeAction(activeCharacter->xPosition, 
                                 activeCharacter->zPosition,
                                 sobj->x, sobj->z,
                                 WALK_PER_MOVE_ACTION) ) )
                {
                   /* Get Object */
                   lastMousePression = tempo;
                   briefTxt->addText("|");
                   if(activeCharacter->inventories->addObject(sobj->obj))
                   {
                      sprintf(buf,gettext("%s taken."),
                              sobj->obj->getName().c_str());
                      briefTxt->addText(buf);

                      /* Log State to the modState */
                      modifState.mapObjectAddAction(
                                              MODSTATE_ACTION_OBJECT_REMOVE,
                                              sobj->obj->getFileName(),
                                              actualMap->getFileName(),
                                              sobj->x, sobj->z);
                         
                      /* Remove object from Map */
                      actualMap->removeObject(sobj->x, sobj->z, sobj->obj);
                         
                      if(inventoryWindow)
                      {
                         inventoryWindow->reDraw();
                      }
                   }
                   else
                   {
                      if(shortCutsWindow)
                      {
                         briefTxt->addText("Inventory Full!"); 
                      }
                   }
                }
                if(Mbutton & SDL_BUTTON(2))
                {
                   /* TODO Open Menu of choices */
                }
                pronto = 1;
            }
         }
         sobj = sobj->next;
      }

      /* Doors Verification */
      door* porta = actualMap->doors;
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
         rotTransBoundingBox(porta->orientation, X, Z,porta->x, 
                             0.0,0.0,porta->z, minObj, maxObj);
         if(intercepts( minObj, maxObj, minMouse, maxMouse, 1))
         {
            cursors->setActual(CURSOR_DOOR);
            if(shortCutsWindow)
            {
               objTxt->setText(gettext("Door")); 
            }
            if( (Mbutton & SDL_BUTTON(1)) && 
                (rangeAction(activeCharacter->xPosition, 
                             activeCharacter->zPosition,
                             porta->x, porta->z,
                             WALK_PER_MOVE_ACTION) ) )
            {
               lastMousePression = tempo;
               if(porta->status)
               {
                  porta->orientation -= 90;
                  porta->status = 0;
               }
               else
               {
                  porta->orientation += 90;
                  porta->status = 1;
               }
            }
            pronto = 1;
         }
         porta = porta->next;
      }

      /* Inventory Verification */
      character* pers = (character*) PCs->first->next;
      while( (pers != PCs->first) && (!pronto) )
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

         if(intercepts( min, max, minMouse, maxMouse, 1))
         {
            cursors->setActual(CURSOR_INVENTORY);
            if(shortCutsWindow)
            {
               objTxt->setText(pers->name); 
            }

            /* Open Inventory when button pressed */
            if( (Mbutton & SDL_BUTTON(1)) && (!inventoryWindow))
            {
               OpenCloseInventoryWindow();
            }
            pronto = 1;
         }
         pers = (character*) pers->next;
      }

      /* Talk And Attack Events Verification */
      if(NPCs)
      {
         pers = (character*) NPCs->first->next;
         while( (pers != NPCs->first) && (!pronto) )
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

            if(intercepts( min, max, minMouse, maxMouse, 1))
            {
               if( engineMode == ENGINE_MODE_REAL_TIME )
               {
                  if(!pers->isAlive())
                  {
                     cursors->setActual(CURSOR_GET);
                  }
                  else if(pers->getConversationFile() != "")
                  {
                     cursors->setActual(CURSOR_TALK);
                     if( (Mbutton & SDL_BUTTON(1)) && 
                         (rangeAction(activeCharacter->xPosition, 
                                      activeCharacter->zPosition,
                                      pers->xPosition, pers->zPosition,
                                      WALK_PER_MOVE_ACTION)) )
                     {
                        pers->openConversationDialog(gui,activeCharacter);
                     }
                  }
                  if(shortCutsWindow)
                  {
                     objTxt->setText(pers->name); 
                  }
                  pronto = 1;
               }
               else
               /* Verify Attacks! */
               if( (engineMode == ENGINE_MODE_TURN_BATTLE) && (canAttack) &&
                   (fightStatus == FIGHT_PC_TURN) && (!fullMovePCAction))
               {
                  string brief = "";
                  cursors->setActual(CURSOR_ATTACK);
                  if(shortCutsWindow)
                  {
                     objTxt->setText(pers->name); 
                  }

                  if( (Mbutton & SDL_BUTTON(1)) &&
                      (rangeAction(activeCharacter->xPosition, 
                                   activeCharacter->zPosition,
                                   pers->xPosition, pers->zPosition,
                                   activeCharacter->getActiveFeatRange() *
                                   METER_TO_DNT) ) )
                  {
                     sprintf(buf, gettext("%s attacks %s|"),
                             activeCharacter->name.c_str(),
                             pers->name.c_str());
                     brief = buf;
                     canAttack = !activeCharacter->actualFeats.
                                                   applyAttackAndBreakFeat(
                                                          *activeCharacter,
                                                          attackFeat, *pers, 
                                                          brief, 
                                                          msgController,
                                                          particleSystem);

                     activeCharacter->actualEnemy = pers;
                     fight->verifyDeads(brief);

                     if( pers->psychoState != PSYCHO_HOSTILE)
                     {
                        pers->psychoState = PSYCHO_HOSTILE;
                     }
                     if( (shortCutsWindow != NULL) && (!brief.empty()))
                     {
                        briefTxt->addText(brief);
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
         if( intercepts( minCon, maxCon, minMouse, maxMouse, 1 ) )
         {
            if(shortCutsWindow)
            {
               objTxt->setText(quaux->mapConection.mapName); 
            }
            curConection = &quaux->mapConection;
            cursors->setActual(CURSOR_MAPTRAVEL);
            pronto = 1;
            if( (Mbutton & SDL_BUTTON(1)) && 
                (rangeAction(activeCharacter->xPosition, 
                             activeCharacter->zPosition,
                             xReal, zReal,
                             WALK_PER_MOVE_ACTION) ) )
            {
               LoadMap(quaux->mapConection.mapName, 0);
               return(1);
            }
         }
      }
      else
      {
         curConection = NULL;
      }

      if( (shortCutsWindow) && (!pronto) )
      {
         objTxt->setText(gettext("Nothing")); 
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
   bool andou = false;       // Character Walk ?
   bool passouTempo = false; // The time to actualize passes ?
   Uint32 tempo;             // Actual Time
   GLfloat varX, varZ;        // to avoid GLfloat calculate
   character* activeCharacter = PCs->getActiveCharacter();
   Uint8 *keys;
   int x,y;
   int guiEvent;
   Uint8 Mbutton;

   GLfloat varTempo;  // Time Variation
   
   tempo = SDL_GetTicks();
   srand(tempo);
   varTempo = (tempo-lastRead);
   if( ((varTempo)) >= ACTUALIZATION_RATE)
   {
      passouTempo = true;

      /* Actualize Time */
      seconds = varTempo / 1000.0;

      hour = (hour + seconds / 100.0 );
      if(hour > 23.99)
      {
         hour = 0.0;
      }
      hourToTxt();
      lastRead = tempo;
      guiEvent = 0;

      SDL_PumpEvents();
      
        
      /* Keyboard Events */
      keys = SDL_GetKeyState(NULL);

      Mbutton = SDL_GetMouseState(&x,&y);
      mouseX = x;
      mouseY = y;

      if( (tempo-lastMouse >=  REFRESH_RATE ) || 
          ( (Mbutton & SDL_BUTTON(1) ) && 
	       (tempo-lastMousePression >= REFRESH_RATE)) )
      {
         cursors->setActual(CURSOR_WALK);
         lastMouse = tempo;

         if(!gui->mouseOnGui(mouseX, mouseY))
         {
            if(verifyMouseActions(Mbutton) == 1)
            {
               /* Changed Map, so */
               return(1);
            }
         }
      }

               /* Keyboard Verification */

         /* Exit Engine */
         if( ( keys[SDLK_ESCAPE] ) && 
             ( (tempo-lastKeyb >= REFRESH_RATE) || 
               (lastKey != SDLK_ESCAPE) ) )
         {
            lastKey = SDLK_ESCAPE;
            lastKeyb = tempo;
            return(0);
         }

         /* Enter Attack Mode or End Turn */
         if( (keys[SDLK_SPACE]) && 
             ( (tempo-lastKeyb >= REFRESH_RATE) || 
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
            lastKeyb = tempo;
         }

         /* Enable / Disable The Range Draw */
         if( (keys[SDLK_r]) && 
             ( (tempo-lastKeyb >= REFRESH_RATE) || 
               (lastKey != SDLK_r) ) )
         {
            showRange = !showRange;
            lastKey = SDLK_r;
            lastKeyb = tempo;
         }

         /* Print All Models on List */
         if( (keys[SDLK_F2]) && 
             ( (tempo-lastKeyb >= REFRESH_RATE) || 
               (lastKey != SDLK_F2) ) )
         {
            lastKey = SDLK_F2;
            lastKeyb = tempo;
            models->printAll();
         }

         /* Open Minimap */
         if( (keys[SDLK_m]) && 
             ( (tempo-lastKeyb >= REFRESH_RATE) || 
               (lastKey != SDLK_m) ) )
         {
             if(!miniMapWindow)
             {
               OpenMiniMapWindow();
             }
             lastKey = SDLK_m;
             lastKeyb = tempo;
         }

         /* Open ShortCuts */
         if( (keys[SDLK_n]) && 
             ( (tempo-lastKeyb >= REFRESH_RATE) || 
               (lastKey != SDLK_n) ) )
         {
             if(!shortCutsWindow)
             {
                 OpenShortcutsWindow();
             }
             lastKey = SDLK_n;
             lastKeyb = tempo;
         }

         /* Open Inventory */
         if( (keys[SDLK_i]) && 
             ( (tempo-lastKeyb >= REFRESH_RATE) || 
               (lastKey != SDLK_i) ) )
         {
            OpenCloseInventoryWindow(); 
            lastKey = SDLK_i;
            lastKeyb = tempo;
         }

         if(keys[SDLK_F1]) //Call Information Screen
         {
            InformationScreen();
         }

         /* FIXME only to test */
         if( (keys[SDLK_c])&& 
             ( (tempo-lastKeyb >= REFRESH_RATE) || 
               (lastKey != SDLK_c) ) )
         {
            lastKey = SDLK_c;
            lastKeyb = tempo;
            msgController->addMessage(PCs->getActiveCharacter()->xPosition,
                                      PCs->getActiveCharacter()->yPosition +
                                      PCs->getActiveCharacter()->max[1],
                                      PCs->getActiveCharacter()->zPosition,
                                      "80");
         }

         /* FIXME Remove all temporary tests from here */
         if( (keys[SDLK_y]) && 
             ( (tempo-lastKeyb >= REFRESH_RATE) || 
               (lastKey != SDLK_y) ) )
         {
            lastKey = SDLK_y;
            lastKeyb = tempo;
            if(!effect)
            {
               effect = (part2*)particleSystem->addParticle(PART_FIRE,
                                          activeCharacter->xPosition,0,
                                          activeCharacter->zPosition,
                                      dir.getRealFile("particles/effect1.par"));
               if(effect)
               {
                  effect->followPC = true;
               }
            }
            else
            {
               particleSystem->removeParticle(PART_FIRE, effect);
               effect = NULL;
            }
         }
         if( (keys[SDLK_t]) && 
             ( (tempo-lastKeyb >= REFRESH_RATE) || 
               (lastKey != SDLK_t) ) )
         {
            lastKey = SDLK_t;
            lastKeyb = tempo;
            particleSystem->addParticle(PART_METEOR,
                                        activeCharacter->xPosition,
                                        MAX_HEIGHT+100,
                                        activeCharacter->zPosition,
                                        0.0, -1.0, 0.0,
                                        activeCharacter->xPosition,
                                        actualMap->getHeight(
                                                activeCharacter->xPosition,
                                                activeCharacter->zPosition),
                                                activeCharacter->zPosition,
                                        dir.getRealFile("particles/fire1.par"));
         }
         if( (keys[SDLK_u]) && 
             ( (tempo-lastKeyb >= REFRESH_RATE) || 
               (lastKey != SDLK_u) ) )
         {
            lastKey = SDLK_u;
            GLfloat incZ = -cos(deg2Rad(activeCharacter->orientation));
            GLfloat incX = -sin(deg2Rad(activeCharacter->orientation));
            particleSystem->addParticle(PART_METEOR,
                                       activeCharacter->xPosition,
                                       activeCharacter->yPosition + 15,
                                       activeCharacter->zPosition,
                                       2*incX, 0.0, 2*incZ,
                                       activeCharacter->xPosition + 800*incX,
                                       activeCharacter->yPosition + 15,
                                       activeCharacter->zPosition + 800*incZ,
                                       dir.getRealFile("particles/fire1.par"));
         }
         if( (keys[SDLK_l]) && 
             ( (tempo-lastKeyb >= REFRESH_RATE) || 
               (lastKey != SDLK_l) ) )
         {
            lastKey = SDLK_l;
            lastKeyb = tempo;
            particleSystem->addParticle(PART_LIGHTNING,
                                        activeCharacter->xPosition,250,
                                        activeCharacter->zPosition,
                                   dir.getRealFile("particles/lightning1.par"));
         }

         if( (keys[SDLK_0]) && 
             ( (tempo-lastKeyb >= REFRESH_RATE) || 
               (lastKey != SDLK_0) ) )
         {
            lastKey = SDLK_0;
            lastKeyb = tempo;
            hour += 0.1;
         }

#ifdef VIDEO_MODE
   if(keys[SDLK_v])
   {
      startVideo = true;
      printf("Started Video\n");
   }
#endif 

      /* Keys to character's movimentation */
      if(keys[SDLK_q] || keys[SDLK_e])
      {
         walkStatus = ENGINE_WALK_KEYS;
          varX = activeCharacter->walk_interval * 
                 sin(deg2Rad(activeCharacter->orientation+90.0));
          varZ = activeCharacter->walk_interval * 
                 cos(deg2Rad(activeCharacter->orientation+90.0));
         // Left walk
         if(keys[SDLK_q]) 
         {
             varX *= -1;
             varZ *= -1;
         }

         if(canWalk(varX,varZ,0)) 
         {
            activeCharacter->xPosition += varX;
            activeCharacter->zPosition += varZ;
            gameCamera.actualizeCamera(activeCharacter->xPosition,
                                       activeCharacter->yPosition,
                                       activeCharacter->zPosition,
                                       activeCharacter->orientation);
            andou = true;
         }
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
            gameCamera.actualizeCamera(activeCharacter->xPosition,
                                       activeCharacter->yPosition,
                                       activeCharacter->zPosition,
                                       activeCharacter->orientation);
            andou = true;
         }
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

            gameCamera.actualizeCamera(activeCharacter->xPosition,
                                       activeCharacter->yPosition,
                                       activeCharacter->zPosition,
                                       activeCharacter->orientation);
            andou = true;
         }
        
      }
      else if(keys[SDLK_w] || keys[SDLK_s])
      { 
         walkStatus = ENGINE_WALK_KEYS;
         varX = activeCharacter->walk_interval * 
                sin(deg2Rad(activeCharacter->orientation));
         varZ = activeCharacter->walk_interval * 
                cos(deg2Rad(activeCharacter->orientation));
         if(keys[SDLK_w]) 
         {
              varX *= -1;
              varZ *= -1;
         }
         if((canWalk(varX,varZ,0)) ) 
         {
             activeCharacter->xPosition += varX;
             activeCharacter->zPosition += varZ;
             gameCamera.actualizeCamera(activeCharacter->xPosition,
                                        activeCharacter->yPosition,
                                        activeCharacter->zPosition,
                                        activeCharacter->orientation);
             andou  = true;
         }
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
              gameCamera.actualizeCamera(activeCharacter->xPosition,
                              activeCharacter->yPosition,
                              activeCharacter->zPosition,
                              activeCharacter->orientation);
              andou = true;
         }
         else if(((varZ > 0)&&(canWalk(0,activeCharacter->walk_interval,0))) ||
                 ((varZ < 0)&&(canWalk(0,-activeCharacter->walk_interval,0))))
         {
              if(varZ < 0)
              {
                 activeCharacter->zPosition -= activeCharacter->walk_interval;
              }
              else
              {
                 activeCharacter->zPosition += activeCharacter->walk_interval;
              }
              gameCamera.actualizeCamera(activeCharacter->xPosition,
                              activeCharacter->yPosition,
                              activeCharacter->zPosition,
                              activeCharacter->orientation);
              andou = true;
         }

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
            andou = true;
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
         andou = true;
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
         gameCamera.actualizeCamera(activeCharacter->xPosition,
                                    activeCharacter->yPosition,
                                    activeCharacter->zPosition,
                                    activeCharacter->orientation);
         SDL_Delay(100);
      }

      /* Camera Verification */
      gameCamera.doIO(keys, Mbutton, x, y, DELTA_CAMERA );

      /* Path Verification */
      if( (Mbutton & SDL_BUTTON(3)) && (!gui->mouseOnGui(x,y)))
      {
         GLfloat dist;
         dist = sqrt( (xReal - moveCircleX) *
                      (xReal - moveCircleX) +
                      (zReal - moveCircleZ) *
                      (zReal - moveCircleZ) );

         /* Only Find Path if move is avaible */
         if( (engineMode != ENGINE_MODE_TURN_BATTLE) || 
             ( (canMove) && (dist <= WALK_PER_MOVE_ACTION)) ||
             ( (canMove) && (canAttack) && (dist <= 2*WALK_PER_MOVE_ACTION )) )
         {
            if(dist > WALK_PER_MOVE_ACTION)
            {
               /* Disable attack move on round if move more than 
                * one move act */
               canAttack = false;
            }
            activeCharacter->pathFind.defineMap(actualMap);
       
            activeCharacter->pathFind.findPath(
                                             activeCharacter->xPosition,
                                             activeCharacter->zPosition,
                                             xReal, zReal, 
                                             activeCharacter->walk_interval, 
                                             activeCharacter->orientation,
                                             activeCharacter->min[0],
                                             activeCharacter->min[1],
                                             activeCharacter->min[2],
                                             activeCharacter->max[0],
                                             activeCharacter->max[1],
                                             activeCharacter->max[2]);
         }
      }

      /* Verify if found path in aStar */
      if(activeCharacter->pathFind.getState() == ASTAR_STATE_FOUND)
      {
         //Found path to, so walk
         walkStatus = ENGINE_WALK_MOUSE;
      }

      if(walkStatus == ENGINE_WALK_MOUSE)
      {
            if(! activeCharacter->pathFind.getNewPosition(
                                             activeCharacter->xPosition,
                                             activeCharacter->zPosition,
                                             activeCharacter->orientation))
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

            gameCamera.actualizeCamera(activeCharacter->xPosition,
                                       activeCharacter->yPosition,
                                       activeCharacter->zPosition,
                                       activeCharacter->orientation);
            andou = true;
      }

      /* IA cycle */
      treatScripts();

      /* GUI Events */

      /* Redraw the needed GUI */
      if(miniMapWindow)
      {
         GLint x = (int)( activeCharacter->xPosition / 
                          actualMap->squareSize() );
         if(x > actualMap->getSizeX()-1)
         {
            x = actualMap->getSizeX()-1;
         }
         GLint z = (int)( activeCharacter->zPosition / 
                          actualMap->squareSize() );
         if( z > actualMap->getSizeZ()-1)
         {
            z = actualMap->getSizeZ()-1;
         }

         if(actualMap->isOutdoor())
         {
            x -= 7;
            z -= 7;
         }
         x = 8 + (x*2);
         z = 20 + (z*2);

         botPerMiniMap->setCoordinate(x, z, x+1, z+1);

         miniMapWindow->draw(mouseX, mouseY);
      }
      if(shortCutsWindow)
      {
         if(shortCutsWindow == gui->getActiveWindow())
         {
            shortCutsWindow->draw(mouseX, mouseY);
         }
         else
         {
            shortCutsWindow->draw(-1,-1);
         }
      }
      guiObject* object;
      object = gui->manipulateEvents(x,y,Mbutton,keys, &guiEvent);
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
            cursors->setActual(CURSOR_FORBIDDEN);
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
      SDL_GL_SwapBuffers();


      /* Actualize FPS */
      actualFPS = (actualFPS + (1000.0 / (SDL_GetTicks() - lastRead))) / 2;
      if( (shortCutsWindow) && (tempo-lastFPS >= 500))
      {
         lastFPS = tempo;
         char texto[32];
         sprintf(texto,"FPS: %3.2f",actualFPS);
         FPS->setText(texto);
         sprintf(texto," Part: %d",particleSystem->numParticles());
         FPS->setText(FPS->getText()+texto);
      }
      
#ifdef VIDEO_MODE
      if(startVideo)
      {
         /* Save frame images to compose demonstration video */
         char name[50];
         if(imgNumber < 10)
            sprintf(name,"img/teste000%d.tga",imgNumber);
         else if(imgNumber < 100)
            sprintf(name,"img/teste00%d.tga",imgNumber);
         else if(imgNumber < 1000)
            sprintf(name,"img/teste0%d.tga",imgNumber);
         else
            sprintf(name,"img/teste%d.tga",imgNumber);
         ScreenDump(name,SCREEN_X,SCREEN_Y);
         imgNumber++;
      }
#endif
 
      /* Verify Sounds FIXME -> for npc sounds! */
      if( (andou) && (activeCharacter->isAlive()) )
      {
         if(!walkSound)
         {
            walkSound = snd->addSoundEffect(activeCharacter->xPosition,0.0,
                                            activeCharacter->zPosition,true,
                                           dir.getRealFile("sndfx/passos.ogg"));
         }
         else
         {
            walkSound->redefinePosition(activeCharacter->xPosition, 0.0,
                                        activeCharacter->zPosition);
         }
         activeCharacter->setState(STATE_WALK);
         #ifdef REDE
           movchar(&clientData, activeCharacter->ID, 
             activeCharacter->xPosition,activeCharacter->zPosition,
             activeCharacter->orientation );
         #endif
      }
      else if( (passouTempo) && (activeCharacter->isAlive()))
      { 
         if( (activeCharacter->getState() == STATE_WALK) &&
             (engineMode == ENGINE_MODE_TURN_BATTLE) && 
             (fightStatus == FIGHT_PC_TURN) )
         {
            canMove = false;
         }
         activeCharacter->setState(STATE_IDLE);
         if(walkSound)
         {
            snd->removeSoundEffect(walkSound);
            walkSound = NULL;
         }
      }
   }
   else
   {
      int tmp = (int) ((ACTUALIZATION_RATE-1) - varTempo);
      if( (tmp > 0) && (tmp < ACTUALIZATION_RATE))
      {
         SDL_Delay(tmp);
      }
   }

   return(!exitEngine);
}

/********************************************************************
 *                            RenderScene                           *
 ********************************************************************/
void engine::renderScene()
{
   GLfloat min[3],max[3];
   GLfloat x[4],z[4];

   bool shadow = false;//actualMap->isOutdoor();

   /* SKY */
   if(actualMap->isOutdoor())
   {
      glPushMatrix();
         glTranslatef(gameCamera.getCameraX(), 0.0, gameCamera.getCameraZ());
         gameSky->draw(actualMap, gameSun->getRotation());
      glPopMatrix();
   }

   glPushMatrix();
   /* Draw The Floor with Stencil Buffer */
   if( ((option->reflexionType != REFLEXIONS_NONE) && (!actualMap->isOutdoor()))
       || (shadow))
   {
      glDisable(GL_DEPTH_TEST);
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glEnable(GL_STENCIL_TEST);
      glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
      glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
      actualMap->drawFloor(gameCamera.getCameraX(),gameCamera.getCameraY(),
                           gameCamera.getCameraZ(),visibleMatrix);
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      glEnable(GL_DEPTH_TEST);
      glDisable(GL_STENCIL_TEST);
   }

   /* Draw Playable Characters (PCs) */
      character* per = (character*) PCs->first->next;
      int aux;
      for(aux=0;aux < PCs->getTotal();aux++)
      {
         /* Actualize the model */
         per->update(WALK_ACTUALIZATION);
         /* Draw Character */
         glPushMatrix();
           glTranslatef(per->xPosition, per->yPosition,
                        per->zPosition);
           glRotatef(per->orientation,0,1,0);
           per->render();

           //per->renderBoundingBox();
           /*glColor3f(1.0,0.1,0.1);
           glBegin(GL_QUADS);
              glVertex3f(per->min[0],per->min[1]+1,per->min[2]);
              glVertex3f(per->min[0],per->min[1]+1,per->max[2]);
              glVertex3f(per->max[0],per->min[1]+1,per->max[2]);
              glVertex3f(per->max[0],per->min[1]+1,per->min[2]);

           glEnd();*/


           /* Draw Reflection */
           if( (option->reflexionType >= REFLEXIONS_CHARACTERS) && 
               (!actualMap->isOutdoor()) )
           {
              glEnable(GL_STENCIL_TEST);
              glStencilFunc(GL_EQUAL, 1, 0xffffffff);  /* draw if ==1 */
              glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
              glCullFace(GL_FRONT);
              glEnable(GL_NORMALIZE);
              glPushMatrix();
                 glTranslatef(per->xPosition, per->yPosition,
                        per->zPosition);
                 glRotatef(per->orientation,0,1,0);
                 glScalef(1.0, -1.0, 1.0);
                 per->render();
              glPopMatrix();
              glDisable(GL_NORMALIZE);
              glCullFace(GL_FRONT);
              glDisable(GL_STENCIL_TEST);
           }
         glPopMatrix();

           /* Draw Projective Shadow */
           if(shadow)
           {
              glEnable(GL_STENCIL_TEST);
              glStencilFunc(GL_EQUAL, 1, 0xffffffff);
              glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
              glEnable(GL_POLYGON_OFFSET_FILL);
              glEnable(GL_BLEND);
              glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
              glDisable(GL_LIGHTING);  
              glColor4f(0.0, 0.0, 0.0, 0.5);
              glPushMatrix();
                 gameSun->mulShadowMatrix();
                 per->renderShadow();
              glPopMatrix();
              glDisable(GL_BLEND);
              glEnable(GL_LIGHTING);
              glDisable(GL_POLYGON_OFFSET_FILL);
              glDisable(GL_STENCIL_TEST);
           }


           per = (character*) per->next;
      }
   glPopMatrix();

   /* Draw the NPCs */
   if(NPCs)
   {
      per = (character*) NPCs->first->next;
      for(aux=0;aux < NPCs->getTotal();aux++)
      {
         /* Actualize the model */
         per->update(WALK_ACTUALIZATION);
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
                             per->min[1] + per->yPosition, 
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
              per->render();

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
              if( (option->reflexionType >= REFLEXIONS_CHARACTERS) && 
                  (!actualMap->isOutdoor()) )
              {
                 glEnable(GL_STENCIL_TEST);
                 glStencilFunc(GL_EQUAL, 1, 0xffffffff);  /* draw if ==1 */
                 glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
                 glCullFace(GL_FRONT);
                 glEnable(GL_NORMALIZE);
                 glPushMatrix();
                    glTranslatef(per->xPosition, per->yPosition,
                                 per->zPosition);
                    glRotatef(per->orientation,0,1,0);
                    glScalef(1.0, -1.0, 1.0);
                    per->render();
                 glPopMatrix();
                 glDisable(GL_NORMALIZE);
                 glCullFace(GL_FRONT);
                 glDisable(GL_STENCIL_TEST);
              }

              glPopMatrix();
         }
         per = (character*) per->next;
      }
   }

   /* Draw the Map Objects && Walls with Reflexions */
   if( (option->reflexionType >= REFLEXIONS_ALL) && (!actualMap->isOutdoor()) )
   {
      glEnable(GL_STENCIL_TEST);
      glStencilFunc(GL_EQUAL, 1, 0xffffffff);  /* draw if ==1 */
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
      glEnable(GL_NORMALIZE);
      glPushMatrix();
        actualMap->drawObjects(gameCamera.getCameraX(),gameCamera.getCameraY(),
                               gameCamera.getCameraZ(),visibleMatrix, true);
      glPopMatrix();
      glPushMatrix();
         glScalef(1.0,-1.0,1.0);
         actualMap->drawWalls(gameCamera.getCameraX(),gameCamera.getCameraY(),
                             gameCamera.getCameraZ(),visibleMatrix, true);
      glPopMatrix();
      glDisable(GL_NORMALIZE);
      glDisable(GL_STENCIL_TEST);
   }

   glPushMatrix();
   actualMap->draw(gameCamera.getCameraX(),gameCamera.getCameraY(),
                   gameCamera.getCameraZ(),visibleMatrix,
                   PCs->getActiveCharacter()->xPosition,
                   PCs->getActiveCharacter()->zPosition);
   glPopMatrix();

}

/********************************************************************
 *                       RenderNoShadowThings                       *
 ********************************************************************/
void engine::renderNoShadowThings()
{

   character* activeCharacter = PCs->getActiveCharacter(); 

   /* Draw Path */
   /*if(walkStatus == ENGINE_WALK_MOUSE)
   {
      activeCharacter->pathFind.drawPath();
   }*/

   if( showRange )
   {
       /* Range Circle */
       actualMap->drawSurfaceOnMap(rangeCircle,
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
       ( (fightStatus == FIGHT_PC_TURN) || (fightStatus == FIGHT_NPC_TURN) ) )
   {
        character* turnCharacter = fight->actualCharacterTurn();
        if(!turnCharacter)
        {
           turnCharacter = activeCharacter;
        }
       /* Draw Movimentation Circles */
          /* Full Circle */
          actualMap->drawSurfaceOnMap(fullMoveCircle,
                                      moveCircleX-2*WALK_PER_MOVE_ACTION,
                                      moveCircleZ-2*WALK_PER_MOVE_ACTION,
                                      moveCircleX+2*WALK_PER_MOVE_ACTION, 
                                      moveCircleZ+2*WALK_PER_MOVE_ACTION,
                                      0.1,12);
          /* Normal Circle */
          actualMap->drawSurfaceOnMap(normalMoveCircle,
                                      moveCircleX-WALK_PER_MOVE_ACTION,
                                      moveCircleZ-WALK_PER_MOVE_ACTION,
                                      moveCircleX+WALK_PER_MOVE_ACTION, 
                                      moveCircleZ+WALK_PER_MOVE_ACTION,
                                      0.2,20);

          if( (canAttack) || (fightStatus == FIGHT_NPC_TURN) )
          {
             /* Feat Range Circle */
             float rangeValue = activeCharacter->getActiveFeatRange() *
                                METER_TO_DNT;
             actualMap->drawSurfaceOnMap(featRangeCircle, 
                                         turnCharacter->xPosition-rangeValue,
                                         turnCharacter->zPosition-rangeValue, 
                                         turnCharacter->xPosition+rangeValue, 
                                         turnCharacter->zPosition+rangeValue, 
                                         0.3,20);
          }
                                       
   }

   if(walkStatus == ENGINE_WALK_MOUSE)
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

       actualMap->drawSurfaceOnMap(destinyImage,
                                   destX - (4 + destinyVariation), 
                                   destZ - (4 + destinyVariation),
                                   destX + (4 + destinyVariation),
                                   destZ + (4 + destinyVariation),
                                   0.25,4);
   }

   /* The SUN or MOON */
   if(actualMap->isOutdoor())
   {
      if(!actualMap->fog.enabled)
      {
         glDisable(GL_FOG);
      }
      gameSun->drawSun();
      if(!actualMap->fog.enabled)
      {
         glEnable(GL_FOG);
      }
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
   if(option->enableParticles)
   {
      glPushMatrix();
         particleSystem->actualizeAll(activeCharacter->xPosition,
                                      activeCharacter->yPosition,
                                      activeCharacter->zPosition, 
                                      visibleMatrix, option->enableGrass);
      glPopMatrix();
   }

   /* World Messages */
   msgController->draw(modl, gameCamera.getCameraX(), 
                       gameCamera.getCameraY(), gameCamera.getCameraZ() );

}

/********************************************************************
 *                             RenderGUI                            *
 ********************************************************************/
void engine::renderGUI()
{
   /* Draw the GUI */   
   glDisable(GL_LIGHTING);
   glDisable(GL_FOG);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_BLEND);

   glPushMatrix();
      draw2DMode();


   /* Player's Portrait */
   PCs->getActiveCharacter()->drawMainPortrait();

   glPushMatrix();
      gui->draw(proj,modl,viewPort);
   glPopMatrix();

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

   
   /* Mouse Cursor */
   glPushMatrix();
         cursors->draw(mouseX, mouseY);
   glPopMatrix();
      if(actualMap->isOutdoor())
      {
         draw3DMode(FARVIEW);
      }
      else
      {
         draw3DMode(INDOOR_FARVIEW);
      }
   glPopMatrix();
   
   glEnable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_FOG);
}

/*********************************************************************
 *                         drawWithShadow                            *
 *********************************************************************/
void engine::drawWithShadows()
{
}

/*********************************************************************
 *                       Draw Scene Function                         *
 *********************************************************************/
void engine::drawWithoutShadows()
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
      gameSun->actualizeHourOfDay(hour, PCs->getActiveCharacter()->xPosition,
                                  PCs->getActiveCharacter()->zPosition);
      gameSun->setLight();
   }
   else
   {
      gameSun->disableLight();
   }
   
   /* Atualize to culling and to GUI */
   actualizeFrustum(visibleMatrix,proj,modl);

   /* Render all things */
   renderScene();
   renderNoShadowThings();
   renderGUI();
   
   /* Flush */
   glFlush();
}

/*********************************************************************
 *                              canWalk                              *
 *********************************************************************/
bool engine::canWalk(GLfloat varX, GLfloat varZ, GLfloat varAlpha)
{
   bool result = true;
   GLfloat dist = 0;
   GLfloat varHeight = 0;
   GLfloat nx, nz;
   character* activeCharacter = PCs->getActiveCharacter();
  
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
   else
   if((engineMode == ENGINE_MODE_TURN_BATTLE) && 
       (fightStatus == FIGHT_PC_TURN))
   {
      if(!canMove)
      {
         /* Already Moved */
         return(false);
      }
      //verify distance to the orign point
      dist = sqrt( (activeCharacter->xPosition + varX - moveCircleX) *
                   (activeCharacter->xPosition + varX - moveCircleX) +
                   (activeCharacter->zPosition + varZ - moveCircleZ) *
                   (activeCharacter->zPosition + varZ - moveCircleZ) );
      if( ( (canAttack) && (dist > 2*WALK_PER_MOVE_ACTION)) || 
            (!canAttack) && (dist > WALK_PER_MOVE_ACTION))
      {
         return(false);
      }
   }


   colisionDetect.defineMap(actualMap, NPCs);
   result = colisionDetect.canWalk(activeCharacter->xPosition + varX,
                                   activeCharacter->yPosition,
                                   activeCharacter->zPosition + varZ,
                                   activeCharacter->min[0],
                                   activeCharacter->min[1],
                                   activeCharacter->min[2],
                                   activeCharacter->max[0],
                                   activeCharacter->max[1],
                                   activeCharacter->max[2],
                                   activeCharacter->orientation + varAlpha, 
                                   activeCharacter->ocSquare, varHeight,
                                   nx, nz);

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
      
      /* Verify Turn Based Mode Action */
      if((engineMode == ENGINE_MODE_TURN_BATTLE) && 
         (fightStatus == FIGHT_PC_TURN))
      {
         if(dist > WALK_PER_MOVE_ACTION)
         {
            fullMovePCAction = true;
         }
         else
         {
            fullMovePCAction = false;
         }
      }
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
 *                       Load MiniMap Window                         *
 *********************************************************************/
void engine::OpenMiniMapWindow()
{
   character* activeCharacter = PCs->getActiveCharacter();
   GLint x = (int)(((activeCharacter->xPosition) / (actualMap->squareSize())));
   if(x > actualMap->getSizeX()-1)
   {
      x = actualMap->getSizeX()-1;
   }
   GLint z = (int)(((activeCharacter->zPosition) / (actualMap->squareSize())));
   if( z > actualMap->getSizeZ()-1)
   {
      z = actualMap->getSizeZ()-1;
   }
   x = 8 + (x*3);
   z = 20 + (z*3);
   miniMapWindow = gui->insertWindow(SCREEN_X-288,SCREEN_Y-128,
                                     SCREEN_X-1,SCREEN_Y-1,
                                     gettext("Map"));

   botPerMiniMap = miniMapWindow->getObjectsList()->insertButton(x,z,x+2,z+2,
                                                                 "",0);
   picture* fig = miniMapWindow->getObjectsList()->insertPicture(8,20,240,95,
                                                                 NULL);
   actualMap->drawMinimap(fig->get());

   FPS = miniMapWindow->getObjectsList()->insertTextBox(3,108,150/*100*/,123,2,
                                  gettext("FPS:"));

   miniMapWindow->getObjectsList()->insertPicture(3,15,252,120,
                              dir.getRealFile("texturas/mapw/map.png").c_str());

   
                   
   miniMapWindow->setExternPointer(&miniMapWindow);
   gui->openWindow(miniMapWindow);
}

/*********************************************************************
 *                         Load ShortCuts Window                     *
 *********************************************************************/
void engine::OpenShortcutsWindow()
{
   shortCutsWindow = gui->insertWindow(0,SCREEN_Y-128,511,SCREEN_Y-1,
                                       gettext("Shortcuts"));
   briefTxt = shortCutsWindow->getObjectsList()->insertRolBar(8,36,249,100,
                                  gettext("Press F1 for Help"),
                                  shortCutsWindow->getSurface());
   objTxt = shortCutsWindow->getObjectsList()->insertTextBox(8,20,249,35,2,
                                                           gettext("Nothing"));

   buttonSave = shortCutsWindow->getObjectsList()->insertButton(8,102,76,120,
                                                            gettext("Save"),0);
   buttonMenu = shortCutsWindow->getObjectsList()->insertButton(77,102,140,120,
                                                            gettext("Menu"),0);
   buttonLoad = shortCutsWindow->getObjectsList()->insertButton(141,102,
                                                                209,120,
                                                            gettext("Load"),0);
   hourTxt = shortCutsWindow->getObjectsList()->insertTextBox(210,102,
                                                              249,120,2,
                                                              "00:00");
   hourTxt->setFont(DNT_FONT_TIMES,11,DNT_FONT_ALIGN_CENTER);
   hourToTxt();

   tabButton* tb;
   tb = shortCutsWindow->getObjectsList()->insertTabButton(252,15,0,0,
                  dir.getRealFile("texturas/shortcutsw/shortcuts.png").c_str());
   buttonAttackMode = tb->insertButton(7,4,43,36);/* Attack Mode */
   tb->insertButton(7,40,43,72);/* Attack 1 */
   tb->insertButton(7,75,43,107);/* Attack 7 */

   buttonQuest = tb->insertButton(53,4,89,36);/* Quest Window */
   tb->insertButton(53,40,89,72);/* Attack 2 */
   tb->insertButton(53,75,89,107);/* Attack 8 */

   buttonInventory = tb->insertButton(99,4,135,36);/* Inventory */
   tb->insertButton(99,40,135,72);/* Attack 3 */
   tb->insertButton(99,75,135,107);/* Attack 9 */

   buttonMap = tb->insertButton(141,4,177,36);/* Map */
   tb->insertButton(141,40,177,72);/* Attack 4 */
   tb->insertButton(141,75,177,107);/* Attack 10 */
   
   buttonGroup = tb->insertButton(180,4,216,36);/* Party/Group */
   tb->insertButton(180,40,216,72);/* Attack 5 */
   buttonAssign = tb->insertButton(180,75,216,107);/* Assign Attacks */

   buttonCharacter = tb->insertButton(220,4,256,36);/* Character */
   tb->insertButton(220,40,256,72);/* Attack 6 */
   buttonEndTurn = tb->insertButton(220,75,256,107);/* End Turn */

   shortCutsWindow->getObjectsList()->insertPicture(3,15,252,120,
                  dir.getRealFile("texturas/shortcutsw/shortcut2.png").c_str());
   
   shortCutsWindow->setExternPointer(&shortCutsWindow);
   gui->openWindow(shortCutsWindow);
}

/*********************************************************************
 *                      OpenCloseInventoryWindow                     *
 *********************************************************************/
void engine::OpenCloseInventoryWindow()
{
   if(!inventoryWindow)
   {
      inventoryWindow=new inventWindow(PCs->getActiveCharacter()->inventories,
                                       gui, infoWindow); 
   }
   else
   {
      delete(inventoryWindow);
      inventoryWindow = NULL;
   }
}

/*********************************************************************
 *                    Actualize Actual Health Bars                   *
 *********************************************************************/
void engine::actualizeAllHealthBars()
{
   character* pers = (character*) PCs->first->next;
   while(pers != PCs->first)
   {
      pers->defineActualLifePoints(pers->lifePoints);
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
   setTextureRGBA(img,&id);
   SDL_FreeSurface(img);

   fadeInTexture(id,0,0,SCREEN_X-1,SCREEN_Y-1, 800, 600);

   /* Wait until Mouse Button pressed */
   while(!(mButton & SDL_BUTTON(1)))
   {
      //Wait for Mouse Button Release
      SDL_PumpEvents();
      int x,y;
      mButton = SDL_GetMouseState(&x,&y);
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
 *                          Runs the Engine                          *
 *********************************************************************/
int engine::Run(SDL_Surface *surface)
{
   string brief;
   
   if(!actualMap->getMusicFileName().empty())
   {
      snd->loadMusic(actualMap->getMusicFileName());
   }

   int time;
   actualFPS = 10.0;
   lastFPS = 0;

   
   #ifdef REDE
     /* if using network. FIXME abandoned code, almost for now. */
     netevent_p_t eventoRede;
     
     initclientdata( &clientData );
     if ( ( startconnection( &clientData, server, DEFAULTPORT )) == -1 )
     {
         printf("Can't connect!\nAborting...\n");
         return(1);
     }
     entergame( &clientData );
   #endif
  
   /* Main Things Run */
   while(treatIO(surface))
   {

      /* Wait Die Animation */
      if( (engineMode == ENGINE_MODE_WAIT_DIE_ANIMATION) && 
          ( (SDL_GetTicks() - waitDieTimer) >= ENGINE_WAIT_DIE_DELAY) )
      {
         /* All Pcs are Dead, so Death Screen! */
         snd->loadMusic(dir.getRealFile("music/musica8.ogg"));
         showImage(dir.getRealFile("texturas/fightMode/death.png"));

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

         /* Clear the Models List */
         if(models)
         {
            delete(models);
            models = new modelList();
         }
              
         /* Put the animation state on normal */
         PCs->getActiveCharacter()->setState(STATE_IDLE);
         return(0);
     }

     /* Verify battle events */
     else if(engineMode == ENGINE_MODE_TURN_BATTLE) 
     {
        time = SDL_GetTicks();
        if(fightStatus == FIGHT_END)
        {
           engineMode = ENGINE_MODE_REAL_TIME;
           if(shortCutsWindow)
           {
              briefTxt->setText( gettext("|Exit Battle Mode"));
           }
           /* Verify if any PC is alive. */
           character* pers = (character*) PCs->first->next;
           bool alive = false;
           while((!alive) && (pers != PCs->first))
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
           fightStatus = fight->doBattleCicle(brief);
           if( (shortCutsWindow) && (!brief.empty()))
           {
              briefTxt->setText(brief);
           }
           actualizeAllHealthBars();

           if(fightStatus == FIGHT_PC_TURN)
           {
               if(fight->actualCharacterTurn()) 
               {
                  PCs->setActiveCharacter(fight->actualCharacterTurn());
                  character* activeCharacter = PCs->getActiveCharacter();
                  fullMovePCAction = false;
                  canMove = true;
                  attackFeat = activeCharacter->getActiveFeatRangeType();
                  canAttack = true;

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
              character* activeCharacter = fight->actualCharacterTurn();
              moveCircleX = activeCharacter->xPosition;
              moveCircleY = activeCharacter->yPosition;
              moveCircleZ = activeCharacter->zPosition;
           }
 
        }
     }

     #ifdef REDE
      /* Network Code. For now, we aren't using the network anymore, 
       * it's not on the initial project scope, only being useful in
       * an estable state. In other words, we hate MMORPG, and don't
       * plain to do one. This will be a standalone RPG.
       */
      while( (eventoRede = pollnet( &clientData ) ) != NULL )
      {
         switch(eventoRede->type)
         {
             case MT_NEWCHAR: /* Insert new character */
             {
                character* per;
                per = NPCs->InserirPersonagem(6,8,3,8,
                             dir.getRealFile("pics/logan/cara.bmp"),0,0,
                                             "LoganNPC",
                                              "???",
                             dir.getRealFile("pics/logan/"),features);
                per->xPosition = eventoRede->x;
                per->zPosition = eventoRede->y; 
                per->orientation = eventoRede->teta;
                per->ID = eventoRede->obj;
                forcaAtualizacao = 1;
                break; 
             }
             case MT_MOV: /* character movimentation */
             {
                character* per = (character*)NPCs->first->next;
                if(per != NPCs->first) 
                {
                   while((per!=NPCs->first) && (eventoRede->obj!=per->ID))
                   {
                      per = (character*) per->next;
                   }
                   if(per!=NPCs->first)
                   {
                       per->xPosition = eventoRede->x;
                       per->zPosition = eventoRede->y; 
                       per->orientation = eventoRede->teta;
                   }
                }
                break; 
             }
             case MT_ERROR:
             {
                 printf("Arrived error from server\n");
                 return(1);
             }
             case MT_ENDSYNC:
             {
                 character* activeCharacter = PCs->getActiveCharacter();
                 activeCharacter->ID = createchar( &clientData, 
                 activeCharacter->xPosition, 
                 activeCharacter->zPosition, 
                 activeCharacter->orientation );
                 if( activeCharacter->ID == -2 )
                 {
                     entergame( &clientData );
                 }
                 else if ( activeCharacter->ID == -1 )
                 {
                     printf("Server Full\n");
                     return(1);
                 }
             }
             default:break; /* By default, nothing! */
         }
      }
    #endif


   }

   return(1);
}


