/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "dccnit.h"
#include "culling.h"
#include "util.h"
#include "../lang/lang.h"
#include <math.h>
#include <SDL/SDL_image.h>
#include "../etc/glm.h"


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
   imgNumber = 0;

   curConection = NULL;

   walkStatus = ENGINE_WALK_KEYS;

   /* Initialize the Cursor */
   cursors = new(cursor);

   /* Initialize sounds and musics */
   snd = new(sound);
   walkSound = NULL;

   /* Load Options */
   option = new options("dcc.opc");

   /* Load Language's files */
   language.ReloadFile(option->langNumber);

   /* Set sound and music volume, based on options */
   snd->changeVolume(option->musicVolume, option->sndfxVolume);

   /* Set Camera, based on options */
   gameCamera.defineCameraType(option->cameraNumber);

   /* Load Skills List */
   skillsList = new skills(language.SKILLS_DIR.c_str(),
                           "../data/skills/skills.skl");
  
   /* Load Features List */
   features = new featsList(language.FEATS_DIR,"../data/feats/feats.ftl");

   /* Load Alignments & Tendecies */
   alignList = new aligns(language.ALIGN_DIR.c_str(), "../data/alignment/alignment.lst");

   /* Load Races */
   raceList = new races(language.RACE_DIR.c_str(), "../data/races/races.lst");

   /* Load Classes */
   classList = new classes(language.CLASS_DIR.c_str(),
                           "../data/classes/classes.lst");

   /* Create 3D Models List */
   models = new modelList();

   /* Initialize readModes variables */
   lastRead = SDL_GetTicks();
   lastMouse = lastRead;
   lastKeyb = lastRead;
   mouseX = 0;
   mouseY = 0;

   showRange = false;

   particleSystem = new partSystem();

   hour = 9.0;
   gameSun = new sun(hour , HALFFARVIEW, HALFFARVIEW);

   engineMode = ENGINE_MODE_REAL_TIME;

   destinyVariation = -2.0;

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
      delete(particleSystem);

   /* Close option */
   delete(option);

   /* Clear Sky */
   delete(gameSky);

   /* Clear Other Textures */
   glDeleteTextures(1, &normalMoveCircle);
   glDeleteTextures(1, &fullMoveCircle);
   glDeleteTextures(1, &destinyImage);
   glDeleteTextures(1, &rangeCircle);

   /* Clear Characters */
   if(NPCs)
      delete(NPCs);
   if(PCs)
      delete(PCs);

   /* Clear Sun */
   if(gameSun)
   {
      delete(gameSun);
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

   /* Clear 3D Models List */
   delete(models);
   
   /* Clear Characters Lists */
   delete(alignList);
   delete(features);
   delete(raceList);
   delete(classList);
   delete(skillsList);
}

/*********************************************************************
 *                       Informations Screen                         *
 *********************************************************************/
void engine::InformationScreen()
{
   Uint8 *keys;
   SDL_Surface* img = IMG_Load(language.TEXTURE_INFORMATION.c_str());

   GLuint texturaInfo;
   carregaTexturaRGBA(img,&texturaInfo);

   glDisable(GL_LIGHTING);
   AtualizaFrustum(visibleMatrix,proj,modl);
   AtualizaTela2D(texturaInfo,proj,modl,viewPort,272,44,527,555,0.0001);
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
   personagem* per;
   if(PCs)
   {
      delete(PCs);
   }
   PCs  = new (Lpersonagem);
   per = PCs->insertCharacter("../data/characters/pcs/logan.pc",
                              features, this);
}

/*********************************************************************
 *                         Load Map to Engine                        *
 *********************************************************************/
int engine::LoadMap(string arqMapa, int RecarregaPCs)
{
   char texto[255];
   string arqVelho = "nada";
   curConection = NULL;
   
   walkStatus = ENGINE_WALK_KEYS;
      
   glClearColor(0,0,0,1);
   glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
   glDisable(GL_LIGHTING);
   SDL_Surface* fig = IMG_Load("../data/texturas/carregar.jpg");

   /* Initializing Load Screen*/
   GLuint texturaCarga;
   carregaTextura(fig,&texturaCarga);
   SDL_FreeSurface(fig);
   

   SDL_Surface* img = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       256,32,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   
   cor_Definir(0,0,0);
   retangulo_Colorir(img,0,0,255,31,0);
   cor_Definir(200,20,20);
   selFonte(FFARSO,CENTRALIZADO,3);
   sprintf(texto,language.LOAD_MAP.c_str(),arqMapa.c_str());
   escxy(img,128,0,texto);
   GLuint texturaTexto;
   carregaTexturaRGBA(img,&texturaTexto);
   fadeInTexture(texturaCarga, 272,236,527,363);

   AtualizaFrustum(visibleMatrix,proj,modl);
   AtualizaTela2D(texturaCarga,proj,modl,viewPort,272,236,527,363,0.01);
   AtualizaTela2D(texturaTexto,proj,modl,viewPort,272,365,527,396,0.01);
   glFlush();
   SDL_GL_SwapBuffers();

   /* Loading Map */
   if(actualMap) 
   {
     arqVelho = actualMap->getFileName();
     delete(actualMap);
     /* Remove All Unused 3D Models */
     models->removeUnusedModels();
   }
   actualMap = new(Map);
   actualMap->setFileName(arqVelho);
   actualMap->open(arqMapa,*models);

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
      glEnable(GL_FOG);
      {
        GLfloat color[3]={1.0,1.0,1.0};
        glFogi(GL_FOG_MODE,GL_LINEAR);
        glFogfv(GL_FOG_COLOR, color);
        glFogf(GL_FOG_DENSITY, 0.10);
        glHint(GL_FOG_HINT, GL_DONT_CARE);
        glFogf(GL_FOG_START, 100);
        glFogf(GL_FOG_END, HALFFARVIEW);
      }
   }

   /* Loading NPCs */
   if(NPCs)
     delete(NPCs);
   NPCs = NULL;
   personagem* per;
   if(!actualMap->getNpcFileName().empty())
   {
      FILE* arq;
      if(!(arq = fopen(actualMap->getNpcFileName().c_str(),"r")))
      {
         printf("Ouch, can't load NPC's file: %s.\n",
                                           actualMap->getNpcFileName().c_str());
      }
      else
      {
         NPCs = new (Lpersonagem);
         int total;
         int npc;
         char nome[30];
         char arquivo[255];
         GLfloat posX, posZ;
         fscanf(arq,"%d",&total);
         for(npc = 0; npc < total; npc++)
         {
           fscanf(arq,"%s %s %f %f",&nome[0],&arquivo[0],&posX,&posZ);
           sprintf(texto, language.LOAD_NPC.c_str(), nome);
           atualizaCarga(img,&texturaTexto,texturaCarga,
                         texto,
                         proj, modl, viewPort);
           per = NPCs->insertCharacter(arquivo,features, this);
           per->posicaoLadoX = posX;
           per->posicaoLadoZ = posZ;
         }
         fclose(arq);
      }  
   }

   /* Loading PCs */
   if(RecarregaPCs)
   {
       loadPCs(); 
       atualizaCarga(img,&texturaTexto,texturaCarga,
                 "Loading Character: Logan",
                 proj, modl, viewPort);
       /*PCs->InserirPersonagem(7,6,9,7,"../data/pics/logan/portrait.jpg",
                              "Gushm",
                       "../data/models/personagens/Gushm/modelo.cfg");*/
   }

   /* Loading Internal Windows */
   atualizaCarga(img,&texturaTexto,texturaCarga,
                 language.LOAD_WINDOWS.c_str(),
                 proj, modl, viewPort);

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
   per = (personagem*) PCs->primeiro->proximo;
   for(aux=0;aux < PCs->getTotal();aux++)
   {
      per->update(0); 
      per->calculateBoundingBox();  
      //FIXME gambiarra to make logan bounding ok when arms down.
        per->max[0] /= 2.0;
        per->min[0] /= 2.0;
      per = (personagem*) per->proximo;
   }

   /* Updating the BoundingBoxes for NPCs */
   if(NPCs)
   { 
      per = (personagem*) NPCs->primeiro->proximo;
      for(aux=0; aux < NPCs->getTotal();aux++)
      {
         per->update(0); 
         per->calculateBoundingBox();  
         per = (personagem*) per->proximo;
      }
   }

   /* Update  particle System to a stable state */
   atualizaCarga(img,&texturaTexto,texturaCarga,
                 language.LOAD_PARTICLE.c_str(),
                 proj, modl, viewPort);
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
   personagem* activeCharacter = PCs->getActiveCharacter();
   actualMap->getInitialPosition(activeCharacter->posicaoLadoX,
                                 activeCharacter->posicaoLadoZ);
   gameCamera.actualizeCamera(activeCharacter->posicaoLadoX,
                              activeCharacter->posicaoLadoY,
                              activeCharacter->posicaoLadoZ,
                              activeCharacter->orientacao);
   activeCharacter->ocupaQuad = actualMap->squareInic;

   atualizaCarga(img,&texturaTexto,texturaCarga,
                 "Loading Changes...",
                 proj, modl, viewPort);

   /* Do Modifications */
   modifState.doMapModifications(actualMap);

   /* Change Music, if needed */
   if(!actualMap->getMusicFileName().empty())
   {
      snd->loadMusic(actualMap->getMusicFileName());
   }

   /* Done */
   atualizaCarga(img,&texturaTexto,texturaCarga,
                 language.LOAD_DONE.c_str(),
                 proj, modl, viewPort);

   glDisable(GL_LIGHTING);
   fadeOutTexture(texturaCarga, 272,236,527,363);

   /* Free Loading Textures */
   SDL_FreeSurface(img);
   glDeleteTextures(1,&texturaCarga);
   glDeleteTextures(1,&texturaTexto);

   glEnable(GL_LIGHTING);

   return(1);
}


void engine::draw2DMode()
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0, (GLdouble) 800, 0.0, (GLdouble) 600);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void engine::draw3DMode()
{
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, 800 / 600, 1.0, FARVIEW);
   glMatrixMode (GL_MODELVIEW);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/*********************************************************************
 *                            fadeInTexture                          *
 *********************************************************************/
void engine::fadeInTexture(GLuint id, int x1, int y1, int x2, int y2)
{
   int i;
   for(i=0; i < 50; i++)
   {
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT
              | GL_STENCIL_BUFFER_BIT);
      AtualizaFrustum(visibleMatrix,proj,modl);
      glColor3f(i/50.0, i/50.0, i/50.0);
      AtualizaTela2D(id,proj,modl,viewPort,x1,y1,x2,y2,0.012);
      glFlush();
      SDL_GL_SwapBuffers();
      SDL_Delay(10);
   }
   glColor3f(1.0,1.0,1.0);
}

/*********************************************************************
 *                            fadeOutTexture                         *
 *********************************************************************/
void engine::fadeOutTexture(GLuint id, int x1, int y1, int x2, int y2)
{
   int i;
   for(i=49; i >= 0; i--)
   {
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT
              | GL_STENCIL_BUFFER_BIT);
      AtualizaFrustum(visibleMatrix,proj,modl);
      glColor3f(i/50.0, i/50.0, i/50.0);
      AtualizaTela2D(id,proj,modl,viewPort,x1,y1,x2,y2,0.012);
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
   int x,y;
   Uint32 time = SDL_GetTicks();
   glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   AtualizaFrustum(visibleMatrix,proj,modl);
   SDL_Surface* img = IMG_Load("../data/texturas/inicio1.png"); 
   glDisable(GL_LIGHTING);
   carregaTexturaRGBA(img,&id);
   SDL_FreeSurface(img);

   /* Fade In Screen */
   fadeInTexture(id,0,0,799,599);

   /* Wait until Mouse Button pressed or time passed */
   while( (!(mButton & SDL_BUTTON(1))) && 
          ( (SDL_GetTicks() - time) <= 5000) )
   {
      SDL_PumpEvents();
      mButton = SDL_GetMouseState(&x,&y);
      SDL_Delay(50);
   }

   /* Wait Mouse Button release */
   while(mButton & SDL_BUTTON(1))
   {
      SDL_PumpEvents();
      mButton = SDL_GetMouseState(&x,&y);
      SDL_Delay(50);
   }

   fadeOutTexture(id,0,0,799,599);
   
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
      snd->loadMusic("../data/music/musica1.ogg");
   }

   /* Executes Initial Screen */
   AtualizaFrustum(visibleMatrix,proj,modl);
   initialScreen* inic = new(initialScreen);
   int result = inic->Execute(Status, proj, modl, viewPort, idTextura, snd);
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
   Tobjeto* object = NULL;
   int eventInfo = NADA;

   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);

   option->DisplayOptionsScreen(interf);

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
         Uint8 Mbotao = SDL_GetMouseState(&x,&y);
         object = interf->manipulateEvents(x,y,Mbotao,keys,&eventInfo);
         AtualizaTela2D(idTextura,proj,modl,viewPort,0,0,799,599,0.012);
         interf->draw(proj,modl,viewPort);
         glFlush();
         SDL_GL_SwapBuffers();
         optionW = option->Treat(object, eventInfo, interf);
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
   }

   delete(alignList);
   alignList = new aligns(language.ALIGN_DIR.c_str(),
                          "../data/alignment/alignment.lst");

   delete(features);
   features = new featsList(language.FEATS_DIR,"../data/feats/feats.ftl");

   delete(raceList);
   raceList = new races(language.RACE_DIR.c_str(), "../data/races/races.lst");

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
   Tobjeto* object = NULL;
   int eventInfo = NADA;

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
   personagem* activeCharacter = PCs->getActiveCharacter();
   raceWindow* rcWindow = new raceWindow(raceList,&activeCharacter->sk,gui,
                                         &activeCharacter->actualRace);

   //TODO Apply skills cost points modifiers                              

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
         Uint8 Mbotao = SDL_GetMouseState(&x,&y);
         object = gui->manipulateEvents(x,y,Mbotao,keys,&eventInfo);

         AtualizaTela2D(idTextura,proj,modl,viewPort,0,0,799,599,0.012);
         gui->draw(proj,modl,viewPort);
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
               atWindow = new attWindow(skillsList, &activeCharacter->sk,
                                        gui, false);
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
                                          gui);
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
               atWindow = new attWindow(skillsList, &activeCharacter->sk,
                                        gui, true);
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
                                          gui);
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
 *                  redmensionate Window to Screen                   *
 *********************************************************************/
void engine::redmensionateWindow(SDL_Surface *screen)
{
   glViewport (0, 0, (GLsizei) screen->w, (GLsizei) screen->h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, (GLsizei)screen->w / (GLsizei)screen->h, 1.0, FARVIEW);
   glGetIntegerv(GL_VIEWPORT, viewPort);
   glMatrixMode (GL_MODELVIEW);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/*********************************************************************
 *                       Init Engine Function                        *
 *********************************************************************/
void engine::Init(SDL_Surface *screen)
{
  
   redmensionateWindow(screen);
   
   /* Clear */
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClearDepth(1.0);
   glClearStencil(0);

   /* Details Definition */
   glDepthFunc(GL_LEQUAL);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);

   /* Light Definition */
   GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

   glEnable(GL_LIGHTING);

   /* Culling */
   /*glCullFace(GL_BACK);
   glEnable(GL_CULL_FACE);*/

   /* Sky Creation */
   gameSky = new(sky);

   SDL_Surface* img;

   /* Battle Circle Textures */
   img = IMG_Load("../data/texturas/fightMode/normalMovCircle.png");
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

   img = IMG_Load("../data/texturas/fightMode/fullMovCircle.png");
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

   /* range circle */
   img = IMG_Load("../data/texturas/walk/range.png");
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
   img = IMG_Load("../data/texturas/walk/destino.png");
   if(!img)
   {
      printf("Error: can't Load Texure: destino.png\n");
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
  char   pixel_data[3*W*H];
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
         string txt = "|" + language.MSG_FAR_AWAY;
         briefTxt->addText(txt);
      }
      return(false);
   }
   return(true);
}


/*********************************************************************
 *                          enterBattleMode                          *
 *********************************************************************/
void engine::enterBattleMode(bool surprisePC)
{
  int numEnemies = 0;
  personagem* ch;
  string brief = "";
  string briefInit = "";
  //FIXME not here the dices!
  diceThing dc;
  dc.baseDice.diceID = DICE_D8;
  dc.baseDice.numberOfDices = 2;
  dc.baseDice.sumNumber = 4;
  dc.initialLevel = 1;
  personagem* activeCharacter = PCs->getActiveCharacter();
  
  fight.empty();
  if(!NPCs)
  {
     if(shortCutsWindow != NULL)
     {
        briefTxt->setText(language.FIGHT_NO_NPCS);
     }
     return;
  }
  ch =(personagem*) NPCs->primeiro->proximo;
  while(ch != NPCs->primeiro)
  {
      //TODO put enemies on groups, when enemy from enemy
      fight.insertNPC(ch, 0, briefInit);
      brief += briefInit + "|";
      numEnemies++;
      ch->actualFeats.defineMeleeWeapon(dc); //FIXME
      ch = (personagem*) ch->proximo; 
      SDL_Delay(1);
  }
                 
  if(numEnemies > 0)
  {
      snd->addSoundEffect(false,"../data/sndfx/battleMode.ogg");
      engineMode = ENGINE_MODE_TURN_BATTLE;
      moveCircleX = activeCharacter->posicaoLadoX;
      moveCircleY = activeCharacter->posicaoLadoY;
      moveCircleZ = activeCharacter->posicaoLadoZ;

      /* Put the PCs on group */
      ch =(personagem*) PCs->primeiro->proximo;
      while(ch != PCs->primeiro)
      {
         fight.insertPC(ch, 0, briefInit);
         brief += briefInit + "|";
         ch->actualFeats.defineMeleeWeapon(dc); //FIXME
         ch = (personagem*) ch->proximo; 
         SDL_Delay(1);
      }
                   
      /* Define PC turn, cause the round of surprise attack! */
      if(surprisePC)
      {
         fightStatus = FIGHT_PC_TURN;
         fullMovePCAction = false;
         canMove = true;
         canAttack = true;
         brief += language.FIGHT_SURPRISE_TURN;
      }
      else
      {
         fightStatus = FIGHT_CONTINUE;
         fullMovePCAction = false;
         canMove = false;
         canAttack = false;
      }
      //TODO Verify if weapon is ranged before do this
      attackFeat = FEAT_MELEE_ATTACK;
      
      
   }
   else
   {
      brief = language.FIGHT_NO_NPCS;
   }
   if(shortCutsWindow != NULL)
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
}

/*********************************************************************
 *                         Threat GUI Events                         *
 *********************************************************************/
void engine::threatGuiEvents(Tobjeto* object, int eventInfo)
{
   /* Verify if Inventory Window is opened */
   if(inventoryWindow)
   {
      if(!inventoryWindow->isOpen())
      {
         /* window is no more opened, so free structs */
         OpenCloseInventoryWindow();
      }  
   }

   /* Verify Dialog Windows */
   if(NPCs != NULL)
   {
      personagem* ch =(personagem*) NPCs->primeiro->proximo;
      while(ch != NPCs->primeiro)
      {
         ch->treatConversation(object, eventInfo, gui);
         ch = (personagem*) ch->proximo;
      }
   }

   /* Verify Inventory Window */
   if( (inventoryWindow) )
   {
      inventoryWindow->treat(object, eventInfo);
   }

   /* Verify ShortCutsWindow */
   switch(eventInfo)
   {
       case TABBOTAOPRESSIONADO:
       {
           if(object == (Tobjeto*) buttonAttackMode)
           {
              if( engineMode != ENGINE_MODE_TURN_BATTLE )
              {
                 enterBattleMode(true);
              }
           }
           else if( object == (Tobjeto*) buttonMap)
           {
              /* Open, if not opened, the minimap window */
              if(!miniMapWindow)
              {
                  OpenMiniMapWindow();
              }
           } 
           else if(object == (Tobjeto*) buttonEndTurn)
           {
              endTurn(); 
           }
           else if(object == (Tobjeto*) buttonInventory)
           {
              if(!inventoryWindow)
              {
                 OpenCloseInventoryWindow();
              }
           }
           break;
       }
       case BOTAOPRESSIONADO:
       {
         if(object == (Tobjeto*) buttonMenu)
         {
            exitEngine = 1;
         }
         else if(object == (Tobjeto*) buttonSave)
         {
            save();
         }
         else if(object == (Tobjeto*) buttonLoad)
         {
            load();
         }
         break;
       }
   }
}

/*********************************************************************
 *                              hourToTxt                            *
 *********************************************************************/
void engine::hourToTxt()
{
   char htmp[15];
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
      hourTxt->texto = htmp;
   }
}

/*********************************************************************
 *                   Threat Input/Output Events                      *
 *********************************************************************/
int engine::threatIO(SDL_Surface *screen,int *forcaAtualizacao)
{
   exitEngine = 0;           // Exit the engine ?
   bool redesenha = false;   // Redraw things ?
   bool andou = false;       // Character Walk ?
   bool passouTempo = false; // The time to actualize passes ?
   Uint32 tempo;             // Actual Time
   GLfloat varX, varZ;        // to avoid GLfloat calculate
   personagem* activeCharacter = PCs->getActiveCharacter();

   GLfloat passo;     // How much the character walks, based on time
   GLfloat rotacao;   // How much the character turns, based on time
   GLfloat varCamera; // Camera Variation
   GLfloat varTempo;  // Time Variation
   GLfloat wx,wy,wz;

   tempo = SDL_GetTicks();
   srand(tempo);
   varTempo = (tempo-lastRead);
   if( ((varTempo)) >= ACTUALIZATION_RATE)
   {
      redesenha = true;
      passouTempo = true;

      /* Actualize Characters Animations */
      seconds = varTempo / 1000.0;

      hour = (hour + seconds / 100.0 );
      if(hour > 23.99)
      {
         hour = 0.0;
      }
      hourToTxt();
      int aux;
      personagem *per = (personagem*) PCs->primeiro->proximo;
      for(aux=0;aux< PCs->getTotal();aux++)
      {
         per->update(WALK_ACTUALIZATION/*seconds*/); 
         //per->CalculateBoundingBox(); 
         per = (personagem*) per->proximo;
      }
    
      if(NPCs)
      {
        per = (personagem*) NPCs->primeiro->proximo;
        for(aux=0;aux < NPCs->getTotal();aux++)
        {
           per->update(WALK_ACTUALIZATION/*seconds*/);   
           per->calculateBoundingBox();
           per = (personagem*) per->proximo;
        }
      }

      /* Calculate the real Modification on walk, rotate, turn, etc */
      GLfloat vt = 1;//varTempo / ACTUALIZATION_RATE;
      passo = (vt)*ANDAR;
      if(passo > 9)
        passo = 9;  /* To avoid phantom efects when LAGs occurs */
      rotacao = (vt)*GIRAR;
      varCamera = vt*DELTACAMERA;
      
      SDL_PumpEvents();

      lastRead = tempo;

        
      //redesenha = TrataIA();
       
      /* Keyboard Events */
      Uint8 *keys;
      keys = SDL_GetKeyState(NULL);

      int x,y;
      int guiEvent = 0;
      Uint8 Mbotao = SDL_GetMouseState(&x,&y);
      mouseX = x;
      mouseY = y;
      
      if( (tempo-lastMouse>=  REFRESH_RATE ) || 
          ( (Mbotao & SDL_BUTTON(1)) && 
	    (tempo-lastMousePression >= REFRESH_RATE)) )
      {
         cursors->setActual(CURSOR_WALK);
         lastMouse = tempo;
         wx = mouseX; wy = SCREEN_Y - mouseY; 
            
         glReadPixels((int)wx,(int)wy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz); 
         gluUnProject(wx,wy,wz,modl,proj,viewPort,&xReal,&yReal,&zReal); 

         GLfloat minMouse[3], maxMouse[3];
         minMouse[0] = xReal-2;  maxMouse[0] = xReal+2;
         minMouse[1] = 0.0;      maxMouse[1] = 0.0;
         minMouse[2] = zReal-2;  maxMouse[2] = zReal+2;

         int qx, qz;
         qx = (int)xReal / SQUARESIZE;
         qz = (int)zReal / SQUARESIZE;
         Square* quaux = actualMap->relativeSquare(qx,qz);
       if(quaux != NULL)
       {
         int pronto;
         int obj = 0;
         GLfloat minObj[3], maxObj[3];

         /* Objects Verification */
         for(pronto = 0; ( (obj<MAXOBJETOS) && (!pronto) );obj++)
         {
            if( (quaux->objects[obj]) && (quaux->objects[obj]->canGet()) )
            {
               boundingBox bound = quaux->objects[obj]->getBoundingBox();
               GLfloat X[4]; GLfloat Z[4];
               X[0] = bound.x1;
               Z[0] = bound.z1;
               X[1] = bound.x1;
               Z[1] = bound.z2; 
               X[2] = bound.x2;
               Z[2] = bound.z2;
               X[3] = bound.x2;
               Z[3] = bound.z1;
               rotTransBoundingBox(quaux->objectsOrientation[obj], X, Z,
                              quaux->Xobjects[obj], 0.0, 
                              0.0,quaux->Zobjects[obj], 
                              minObj, maxObj);
               if(estaDentro( minObj, maxObj, minMouse, maxMouse, 1))
               {
                   cursors->setActual(CURSOR_GET);
                   if(shortCutsWindow)
                   {
                      ObjTxt->texto = quaux->objects[obj]->getName(); 
                      shortCutsWindow->Desenhar(mouseX,mouseY);
                   }
                   if( (Mbotao & SDL_BUTTON(1)) && 
                       (rangeAction(activeCharacter->posicaoLadoX, 
                                    activeCharacter->posicaoLadoZ,
                                    quaux->Xobjects[obj],
                                    quaux->Zobjects[obj],
                                    WALK_PER_MOVE_ACTION) ) )
                   {
                      /* Get Object */
                      lastMousePression = tempo;
                      briefTxt->addText("|");

                      int inv = 0;

                      while((!activeCharacter->inventories[inv]->addObject(
                             quaux->objects[obj]) && 
                            (inv < INVENTORY_PER_CHARACTER)))
                      {
                         inv++;
                      }
                            

                      if(inv < INVENTORY_PER_CHARACTER)
                      {
                         briefTxt->addText(quaux->objects[obj]->getName() + " "                                            + language.ACTION_TAKEN); 
                         shortCutsWindow->Desenhar(mouseX,mouseY);

                         /* Log State to the modState */
                         modifState.mapObjectAddAction(
                                                 MODSTATE_ACTION_MAP_REMOVE,
                                                 quaux->objects[obj]->getName(),
                                                 actualMap->getFileName(),
                                                 quaux->Xobjects[obj],
                                                 quaux->Zobjects[obj]);
                         
                         /* Remove object from Map */
                         actualMap->removeObject(quaux->Xobjects[obj],
                                                 quaux->Zobjects[obj],
                                                 quaux->objects[obj]);
                         
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
                            shortCutsWindow->Desenhar(mouseX,mouseY);
                         }
                      }
                   }
                   if(Mbotao & SDL_BUTTON(2))
                   {
                      /* TODO Open Menu of choices */
                   }
                   pronto = 1;
               }
            }
         }

         /* Doors Verification */
         door* porta = actualMap->portas;
         while( (porta != NULL) && (!pronto) )
         {
             boundingBox bound = porta->object->getBoundingBox();
             GLfloat X[4]; GLfloat Z[4];
             X[0] = bound.x1;
             Z[0] = bound.z1;
             X[1] = bound.x1;
             Z[1] = bound.z2; 
             X[2] = bound.x2;
             Z[2] = bound.z2;
             X[3] = bound.x2;
             Z[3] = bound.z1;
             rotTransBoundingBox(porta->orientacao, X, Z,
                                 porta->x, 0.0, 
                                 0.0,porta->z, 
                                 minObj, maxObj);
             if(estaDentro( minObj, maxObj, minMouse, maxMouse, 1))
             {
                 cursors->setActual(CURSOR_DOOR);
                 if(shortCutsWindow)
                 {
                    ObjTxt->texto = language.OBJ_DOOR.c_str(); 
                    shortCutsWindow->Desenhar(mouseX, mouseY);
                 }
                 if( (Mbotao & SDL_BUTTON(1)) && 
                     (rangeAction(activeCharacter->posicaoLadoX, 
                                  activeCharacter->posicaoLadoZ,
                                  porta->x, porta->z,
                                  WALK_PER_MOVE_ACTION) ) )
                 {
                    lastMousePression = tempo;
                    if(porta->status)
                    {
                       porta->orientacao -= 90;
                       porta->status = 0;
                    }
                    else
                    {
                       porta->orientacao += 90;
                       porta->status = 1;
                    }
                    redesenha = 1; 
                 }
                 pronto = 1;
             }
           porta = porta->proximo;
         }


         /* Inventory Verification */
         personagem* pers = (personagem*) PCs->primeiro->proximo;
         while( (pers != PCs->primeiro) && (!pronto) )
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

            rotTransBoundingBox(pers->orientacao, x, z,
                                pers->posicaoLadoX, 
                                0.0, 0.0, 
                                pers->posicaoLadoZ, min, max );

            if(estaDentro( min, max, minMouse, maxMouse, 1))
            {
                cursors->setActual(CURSOR_INVENTORY);
                if(shortCutsWindow)
                {
                   ObjTxt->texto = pers->nome; 
                   shortCutsWindow->Desenhar(mouseX, mouseY);
                }

                /* Open Inventory when button pressed */
                if( (Mbotao & SDL_BUTTON(1)) && (!inventoryWindow))
                {
                   OpenCloseInventoryWindow();
                }
                pronto = 1;
            }
            pers = (personagem*) pers->proximo;
         }

         /* Talk And Attack Events Verification */
         if(NPCs)
         {
            pers = (personagem*) NPCs->primeiro->proximo;
            while( (pers != NPCs->primeiro) && (!pronto) )
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

               rotTransBoundingBox(pers->orientacao, x, z,
                                pers->posicaoLadoX, 
                                0.0, 0.0, 
                                pers->posicaoLadoZ, min, max );

              if(estaDentro( min, max, minMouse, maxMouse, 1))
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
                       if( (Mbotao & SDL_BUTTON(1)) && 
                           (rangeAction(activeCharacter->posicaoLadoX, 
                                        activeCharacter->posicaoLadoZ,
                                        pers->posicaoLadoX, pers->posicaoLadoZ,
                                        WALK_PER_MOVE_ACTION)) )
                       {
                          pers->openConversationDialog(gui,
                                                       activeCharacter);
                       }
                    }
                    if(shortCutsWindow)
                    {
                       ObjTxt->texto = pers->nome; 
                       shortCutsWindow->Desenhar(mouseX, mouseY);
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
                        ObjTxt->texto = pers->nome; 
                        shortCutsWindow->Desenhar(mouseX, mouseY);
                     }

                     //TODO -> verify if weapon is ranged, so distance is other
                     if( (Mbotao & SDL_BUTTON(1)) &&
                         (rangeAction(activeCharacter->posicaoLadoX, 
                                      activeCharacter->posicaoLadoZ,
                                      pers->posicaoLadoX, pers->posicaoLadoZ,
                                      WALK_PER_MOVE_ACTION) ) )
                     {
                        brief = activeCharacter->nome + " " + 
                                language.FIGHT_ATTACKS + " " + 
                                pers->nome + "|";
                        canAttack = !activeCharacter->actualFeats.
                                                        applyAttackAndBreakFeat(
                                                          *activeCharacter,
                                                          attackFeat, *pers, 
                                                          brief);
                        if(!pers->isAlive())
                        {
                           brief += "|" + pers->nome + " " +  
                                    language.FIGHT_DEAD;
                        }
                        if( pers->psychoState != PSYCHO_HOSTILE)
                        {
                            pers->psychoState = PSYCHO_HOSTILE;
                        }
                        if(shortCutsWindow != NULL)
                        {
                           briefTxt->setText(brief);
                        }

                     }
                     pronto = 1;
                 }
              }
              pers = (personagem*) pers->proximo;
            }
         }

         /* Map Connections Verification */
         if( ( quaux->mapConection.active ) && (!pronto) && 
             (engineMode == ENGINE_MODE_REAL_TIME) ) 
             /* Don't travel on battle mode */
         {
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
            if( estaDentro( minCon, maxCon, minMouse, maxMouse, 1 ) )
            {
               if(shortCutsWindow)
               {
                  ObjTxt->texto = quaux->mapConection.mapName; 
                  shortCutsWindow->Desenhar(mouseX, mouseY);
               }
               curConection = &quaux->mapConection;
               cursors->setActual(CURSOR_MAPTRAVEL);
               pronto = 1;
               if( (Mbotao & SDL_BUTTON(1)) && 
                   (rangeAction(activeCharacter->posicaoLadoX, 
                                activeCharacter->posicaoLadoZ,
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
            ObjTxt->texto = language.OBJ_NOTHING.c_str(); 
            shortCutsWindow->Desenhar(mouseX, mouseY);
         }
        }
      }

      if(tempo-lastKeyb >= REFRESH_RATE)
      {
         lastKeyb = tempo;
         /* Keyboard Verification */

         /* Exit Engine */
         if ( keys[SDLK_ESCAPE] )
         {
            return(0);
         }

         /* Enter Attack Mode or End Turn */
         if(keys[SDLK_SPACE])
         {
            if(engineMode != ENGINE_MODE_TURN_BATTLE)
            {
               enterBattleMode(true);
            }
            else
            {
               endTurn();
            }
         }

         /* Enable / Disable The Range Draw */
         if(keys[SDLK_r])
         {
            showRange = !showRange;
         }

         /* Print All Models on List */
         if(keys[SDLK_F2])
         {
            models->printAll();
         }

         /* Open Minimap */
         if(keys[SDLK_m]) 
         {
             if(!miniMapWindow)
             {
               OpenMiniMapWindow();
               redesenha = true;
             }
         }

         /* Open ShortCuts */
         if(keys[SDLK_n])
         {
             if(!shortCutsWindow)
             {
                 OpenShortcutsWindow();
                 redesenha = true;
             }
         }

         /* Open Inventory */
         if(keys[SDLK_i])
         {
            OpenCloseInventoryWindow(); 
         }

         if(keys[SDLK_F1]) //Call Information Screen
         {
            InformationScreen();
            redesenha = true;
         }

         /* Temporariamente, para visualizar o efeito de sangue */
         if(keys[SDLK_y])
         {
            if(!effect)
            {
               effect = (part2*)particleSystem->addParticle(PART_FIRE,
                                          activeCharacter->posicaoLadoX,0,
                                          activeCharacter->posicaoLadoZ,
                                          "../data/particles/effect1.par");
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
         if(keys[SDLK_p])
         {
            part5 *p;
            p =  (part5*)particleSystem->addParticle(PART_BLOOD,
                                    activeCharacter->posicaoLadoX,28,
                                    activeCharacter->posicaoLadoZ, 
                                    "../data/particles/blood1.par");
            if(p)
               p->followPC = true;
         }   
         if(keys[SDLK_o])
         {
             part5 *p;
             p = (part5*) particleSystem->addParticle(PART_BLOOD,
                                         activeCharacter->posicaoLadoX,28,
                                         activeCharacter->posicaoLadoZ, 
                                         "../data/particles/blood2.par");
             if(p) 
               p->followPC = true;
         }
         if(keys[SDLK_l])
         {
            particleSystem->addParticle(PART_LIGHTNING,
                                        activeCharacter->posicaoLadoX,250,
                                        activeCharacter->posicaoLadoZ,
                                        "../data/particles/lightning1.par");
         }
         if(keys[SDLK_k])
         {
            lastTurnTime = SDL_GetTicks();
            PCs->getActiveCharacter()->setState(STATE_DIE);
            engineMode = ENGINE_MODE_DEAD;
         }

         if(keys[SDLK_0])
         {
            hour += 0.1;
         }

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
          varX = passo * sin(deg2Rad(activeCharacter->orientacao+90.0));
          varZ = passo * cos(deg2Rad(activeCharacter->orientacao+90.0));
         // Left walk
         if(keys[SDLK_q]) 
         {
             varX *= -1;
             varZ *= -1;
         }

         if(canWalk(varX,varZ,0)) 
         {
            activeCharacter->posicaoLadoX += varX;
            activeCharacter->posicaoLadoZ += varZ;
            gameCamera.actualizeCamera(activeCharacter->posicaoLadoX,
                                       activeCharacter->posicaoLadoY,
                                       activeCharacter->posicaoLadoZ,
                                       activeCharacter->orientacao);
            redesenha = true;
            andou = true;
         }
         else if( ((varX > 0) && (canWalk(passo,0,0))) ||
                  ((varX < 0) && (canWalk(-passo,0,0))) )       
         {
            if(varX < 0)
               passo *= -1;
            activeCharacter->posicaoLadoX += passo;
            gameCamera.actualizeCamera(activeCharacter->posicaoLadoX,
                                       activeCharacter->posicaoLadoY,
                                       activeCharacter->posicaoLadoZ,
                                       activeCharacter->orientacao);
            redesenha = true;
            andou = true;
         }
         else if( ((varZ > 0) && canWalk(0,passo,0)) ||
                  ((varZ < 0) && canWalk(0,-passo,0)) )
         {
            if(varZ < 0) 
               passo *= -1;
            activeCharacter->posicaoLadoZ += passo;
            gameCamera.actualizeCamera(activeCharacter->posicaoLadoX,
                                       activeCharacter->posicaoLadoY,
                                       activeCharacter->posicaoLadoZ,
                                       activeCharacter->orientacao);
            redesenha = true;
            andou = true;
         }
        
      }
      else if(keys[SDLK_w] || keys[SDLK_s])
      { 
         walkStatus = ENGINE_WALK_KEYS;
         varX = passo * sin(deg2Rad(activeCharacter->orientacao));
         varZ = passo * cos(deg2Rad(activeCharacter->orientacao));
         if(keys[SDLK_w]) 
         {
              varX *= -1;
              varZ *= -1;
         }
         if((canWalk(varX,varZ,0)) ) 
         {
             activeCharacter->posicaoLadoX += varX;
             activeCharacter->posicaoLadoZ += varZ;
             gameCamera.actualizeCamera(activeCharacter->posicaoLadoX,
                                        activeCharacter->posicaoLadoY,
                                        activeCharacter->posicaoLadoZ,
                                        activeCharacter->orientacao);
             redesenha = true;
             andou  = true;
         }
         else if( ((varX > 0) && (canWalk(passo,0,0))) ||
                  ((varX < 0) && (canWalk(-passo,0,0))) ) 
                   
         {
              if(varX < 0 )
                 passo *= -1;
              activeCharacter->posicaoLadoX += passo;
              gameCamera.actualizeCamera(activeCharacter->posicaoLadoX,
                              activeCharacter->posicaoLadoY,
                              activeCharacter->posicaoLadoZ,
                              activeCharacter->orientacao);
              redesenha = true;
              andou = true;
         }
         else if( ((varZ > 0) && (canWalk(0,passo,0))) ||
                  ((varZ < 0) && (canWalk(0,-passo,0))) )
         {
              if( varZ < 0 )
                 passo *= -1;
              activeCharacter->posicaoLadoZ += passo;
              gameCamera.actualizeCamera(activeCharacter->posicaoLadoX,
                              activeCharacter->posicaoLadoY,
                              activeCharacter->posicaoLadoZ,
                              activeCharacter->orientacao);
              redesenha = true;
              andou = true;
         }

      }

      if( (keys[SDLK_a]) || (keys[SDLK_d]))
      {
         walkStatus = ENGINE_WALK_KEYS;
         // CounterClockWise Character turn
         if((keys[SDLK_a]) && (canWalk(0,0,rotacao)) )  
         {
            activeCharacter->orientacao += rotacao;
            if(activeCharacter->orientacao > 360.0)
               activeCharacter->orientacao = 
                               activeCharacter->orientacao  - 360.0;
            redesenha = true;
            andou = true;
         }
         // Clockwise Character Turn
         if((keys[SDLK_d]) && (canWalk(0,0,-rotacao)) )
         {
            activeCharacter->orientacao -= rotacao;
            if(activeCharacter->orientacao < 0.0)
               activeCharacter->orientacao = 360.0 + 
                                        activeCharacter->orientacao ;
         }
         redesenha = true;
         andou = true;
      }
      if(keys[SDLK_TAB]) //Activate Character
      {
         walkStatus = ENGINE_WALK_KEYS;
         if(keys[SDLK_LCTRL]) //Previous Character
         {
            PCs->setActiveCharacter((personagem*)activeCharacter->anterior);
         }
         else //Next Character
         {
            PCs->setActiveCharacter((personagem*)activeCharacter->proximo);
         }
         activeCharacter = PCs->getActiveCharacter();
         gameCamera.actualizeCamera(activeCharacter->posicaoLadoX,
                                    activeCharacter->posicaoLadoY,
                                    activeCharacter->posicaoLadoZ,
                                    activeCharacter->orientacao);
         redesenha = true;
         SDL_Delay(100);
      }

      /* Camera Verification */
      if(gameCamera.doIO(keys, Mbotao, x, y, varCamera ))
      {
         redesenha = true;
      }
      
      /* Path Verification */
      if(Mbotao & SDL_BUTTON(3))
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
               /* Disable attack move on round if move more than one move act */
               canAttack = false;
            }
            activeCharacter->pathFind.defineMap(actualMap);
       
            activeCharacter->pathFind.findPath(
                                             activeCharacter->posicaoLadoX,
                                             activeCharacter->posicaoLadoZ,
                                             xReal, zReal, ANDAR, 
                                             activeCharacter->orientacao,
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
                                             activeCharacter->posicaoLadoX,
                                             activeCharacter->posicaoLadoZ,
                                             activeCharacter->orientacao))
            {
               walkStatus = ENGINE_WALK_KEYS;
            }
            else
            {
               /* Define New Occuped Square */
               int posX =(int)floor(activeCharacter->posicaoLadoX /
                                    SQUARESIZE);
               int posZ =(int)floor(activeCharacter->posicaoLadoZ / 
                                    SQUARESIZE);
               activeCharacter->ocupaQuad = 
                                         actualMap->relativeSquare(posX,posZ);
               /* Define New Height */
               defineActiveCharacterHeight(activeCharacter->posicaoLadoX,
                                           activeCharacter->posicaoLadoZ);
            }

            gameCamera.actualizeCamera(activeCharacter->posicaoLadoX,
                                       activeCharacter->posicaoLadoY,
                                       activeCharacter->posicaoLadoZ,
                                       activeCharacter->orientacao);
            redesenha = true;
            andou = true;
      }

      /* GUI Events */

      /* Redraw the needed GUI */
      if(miniMapWindow)
      {
         GLint x = (int)(((activeCharacter->posicaoLadoX) / (SQUARESIZE)));
         if(x > actualMap->getSizeX()-1)
         {
            x = actualMap->getSizeX()-1;
         }
         GLint z = (int)(((activeCharacter->posicaoLadoZ) / (SQUARESIZE)));
         if( z > actualMap->getSizeZ()-1)
         {
            z = actualMap->getSizeZ()-1;
         }
         x = 8 + (x*4);
         z = 20 + (z*4);

         botPerMiniMap->x1 = x;
         botPerMiniMap->x2 = x+3;
         botPerMiniMap->y1 = z;
         botPerMiniMap->y2 = z+3;

         miniMapWindow->Desenhar(mouseX, mouseY);
      }
      if(shortCutsWindow)
      {
         shortCutsWindow->Desenhar(mouseX, mouseY);
      }
      Tobjeto* object;
      object = gui->manipulateEvents(x,y,Mbotao,keys, &guiEvent);
      /* Threat the GUI */
      if(guiEvent != NADA)
      {
         threatGuiEvents(object, guiEvent);
         redesenha = true;
      }

   }
   else if(*forcaAtualizacao == 0)
   {
      int tmp = (int) ((ACTUALIZATION_RATE-1) - varTempo);
      if(tmp > 0)
      SDL_Delay(tmp);
   }
   
   if( (redesenha) || ( (*forcaAtualizacao != 0)))
   {      
      Draw();
      SDL_GL_SwapBuffers();


      /* Actualize FPS */
      actualFPS = (actualFPS + (1000.0 / (SDL_GetTicks() - lastRead))) / 2;
      if( (shortCutsWindow) && (tempo-lastFPS >= 500))
      {
         lastFPS = tempo;
         char texto[15];
         sprintf(texto,"FPS: %3.2f",actualFPS);
         FPS->texto = texto;
         sprintf(texto," Part: %d",particleSystem->numParticles());
         FPS->texto += texto;
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
         ScreenDump(name,800,600);
         imgNumber++;
      }
#endif
      *forcaAtualizacao = 0;
   }
 
   if(andou)
   {
      if(!walkSound)
      {
         walkSound = snd->addSoundEffect(activeCharacter->posicaoLadoX,0.0,
                                        activeCharacter->posicaoLadoZ,true,
                                        "../data/sndfx/passos.ogg" );
      }
      else
      {
         walkSound->redefinePosition(activeCharacter->posicaoLadoX, 0.0,
                                     activeCharacter->posicaoLadoZ);
      }
      activeCharacter->setState(STATE_WALK);
      #ifdef REDE
        movchar(&clientData, activeCharacter->ID, 
          activeCharacter->posicaoLadoX,activeCharacter->posicaoLadoZ,
          activeCharacter->orientacao );
      #endif
   }
   else if( (passouTempo) && (engineMode != ENGINE_MODE_DEAD))
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
 
   return(!exitEngine);
}

/*********************************************************************
 *                       Draw Scene Function                         *
 *********************************************************************/
void engine::Draw()
{
   personagem* activeCharacter = PCs->getActiveCharacter();
   GLdouble x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4;

   GLfloat min[3],max[3];
   GLfloat x[4],z[4];

   glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   glLoadIdentity();

   /* Redefine camera position */
   gameCamera.lookAt();

   snd->setListenerPosition(gameCamera.getCameraX(), gameCamera.getCameraY(),
                            gameCamera.getCameraZ(), gameCamera.getTheta(),
                            gameCamera.getPhi(), gameCamera.getD(),
                            gameCamera.getDeltaY());

   /* Sun Definition */
   gameSun->actualizeHourOfDay(hour);
   
   /* Atualize to culling and to GUI */
   AtualizaFrustum(visibleMatrix,proj,modl);

   /* SKY */
   glPushMatrix();
      gameSky->draw(actualMap,gameSun->getRotation());
   glPopMatrix();

   glPushMatrix();

   /* Draws World, doing view frustum culling */
   actualMap->draw(gameCamera.getCameraX(),gameCamera.getCameraY(),
                   gameCamera.getCameraZ(),visibleMatrix);
   
   /* Draw Playable Characters (PCs) */
      personagem* per = (personagem*) PCs->primeiro->proximo;
      int aux;
      for(aux=0;aux < PCs->getTotal();aux++)
      {
         /* Draw Character */
         glPushMatrix();
           glTranslatef(per->posicaoLadoX, per->posicaoLadoY,
                        per->posicaoLadoZ);
           glRotatef(per->orientacao,0,1,0);
           per->render();
           /*per->RenderBoundingBox();
           glColor3f(0.6,0.1,0.1);
           glBegin(GL_POLYGON);
              glVertex3f(per->min[0],per->min[1]+1,per->min[2]);
              glVertex3f(per->min[0],per->min[1]+1,per->max[2]);
              glVertex3f(per->max[0],per->min[1]+1,per->max[2]);
              glVertex3f(per->max[0],per->min[1]+1,per->min[2]);

           glEnd();*/
         glPopMatrix();
         
         per = (personagem*) per->proximo;
      }
   glPopMatrix();

   /* Draw the NPCs */
   if(NPCs)
   {
      per = (personagem*) NPCs->primeiro->proximo;
      for(aux=0;aux < NPCs->getTotal();aux++)
      {
         /* Verify Bounding Box */
         x[0] = per->min[0];
         z[0] = per->min[2];
         x[1] = per->min[0];
         z[1] = per->max[2]; 
         x[2] = per->max[0];
         z[2] = per->max[2];
         x[3] = per->max[0];
         z[3] = per->min[2];
         rotTransBoundingBox(per->orientacao, x, z,per->posicaoLadoX, 
                             per->min[1] + per->posicaoLadoY, 
                             per->max[1] + per->posicaoLadoY,
                             per->posicaoLadoZ, min, max );

         /* Only Draw Visible Characters */
         if(quadradoVisivel(min[0],min[1],min[2],max[0],max[1],max[2],
                            visibleMatrix))
         {
            glPushMatrix();
              glTranslatef(per->posicaoLadoX, 0 ,per->posicaoLadoZ);
              glRotatef(per->orientacao,0,1,0);
              per->render();
              /*per->RenderBoundingBox();
              glColor3f(0.6,0.1,0.1);
              glBegin(GL_POLYGON);
                 glVertex3f(per->min[0],per->min[1]+1,per->min[2]);
                 glVertex3f(per->min[0],per->min[1]+1,per->max[2]);
                 glVertex3f(per->max[0],per->min[1]+1,per->max[2]);
                 glVertex3f(per->max[0],per->min[1]+1,per->min[2]);
   
              glEnd();*/
            glPopMatrix();
         }
         per = (personagem*) per->proximo;
      }
   }

   if( showRange )
   {
       /* Range Circle */
       actualMap->drawSurfaceOnMap(rangeCircle,
                                   activeCharacter->posicaoLadoX - 
                                                           WALK_PER_MOVE_ACTION,
                                   activeCharacter->posicaoLadoZ - 
                                                           WALK_PER_MOVE_ACTION,
                                   activeCharacter->posicaoLadoX + 
                                                           WALK_PER_MOVE_ACTION,
                                   activeCharacter->posicaoLadoZ + 
                                                           WALK_PER_MOVE_ACTION,
                                   0.05);
   }

   /* Draw Combat Mode Things */
   if( (engineMode == ENGINE_MODE_TURN_BATTLE) && 
       (fightStatus == FIGHT_PC_TURN))
   {
       /* Draw Movimentation Circles */
          /* Full Circle */
          actualMap->drawSurfaceOnMap(fullMoveCircle,
                                      moveCircleX-2*WALK_PER_MOVE_ACTION,
                                      moveCircleZ-2*WALK_PER_MOVE_ACTION,
                                      moveCircleX+2*WALK_PER_MOVE_ACTION, 
                                      moveCircleZ+2*WALK_PER_MOVE_ACTION,
                                      0.1);
          /* Normal Circle */
          actualMap->drawSurfaceOnMap(normalMoveCircle,
                                      moveCircleX-WALK_PER_MOVE_ACTION,
                                      moveCircleZ-WALK_PER_MOVE_ACTION,
                                      moveCircleX+WALK_PER_MOVE_ACTION, 
                                      moveCircleZ+WALK_PER_MOVE_ACTION,
                                      0.2);
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
                                   0.25);
   }

   if(!actualMap->fog.enabled)
   {
      glDisable(GL_FOG);
   }
   gameSun->drawSun();

   if(!actualMap->fog.enabled)
   {
      glEnable(GL_FOG);
   }

   

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
         particleSystem->actualizeAll(activeCharacter->posicaoLadoX,
                                      activeCharacter->posicaoLadoZ, 
                                      visibleMatrix, option->enableGrass);
      glPopMatrix();
   }

   /* Draw the GUI and others */

   /* Get Portrait position */
   gluUnProject(SCREEN_X,SCREEN_Y, 0.01, modl, proj, viewPort, &x1, &y1, &z1);
   gluUnProject(SCREEN_X,SCREEN_Y-64,0.01, modl, proj, viewPort, &x2, &y2, &z2);
   gluUnProject(SCREEN_X-64,SCREEN_Y-64,0.01,modl,proj,viewPort, &x3, &y3, &z3);
   gluUnProject(SCREEN_X-64,SCREEN_Y,0.01, modl, proj, viewPort, &x4, &y4, &z4);

   glDisable(GL_LIGHTING);
   
   
   glDisable(GL_DEPTH_TEST);


   /* Player's Portrait */
   per = (personagem*) activeCharacter;
   per->drawMainPortrait(x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4);
 
   gui->draw(proj,modl,viewPort);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

   
   /* Mouse Cursor */
   glPushMatrix();
      draw2DMode();
         cursors->draw(mouseX, mouseY);
      draw3DMode();
   glPopMatrix();
   
   glEnable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
 
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
   personagem* activeCharacter = PCs->getActiveCharacter();
   
   if((engineMode == ENGINE_MODE_TURN_BATTLE) && 
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
      dist = sqrt( (activeCharacter->posicaoLadoX + varX - moveCircleX) *
                   (activeCharacter->posicaoLadoX + varX - moveCircleX) +
                   (activeCharacter->posicaoLadoZ + varZ - moveCircleZ) *
                   (activeCharacter->posicaoLadoZ + varZ - moveCircleZ) );
      if( ( (canAttack) && (dist > 2*WALK_PER_MOVE_ACTION)) || 
            (!canAttack) && (dist > WALK_PER_MOVE_ACTION))
      {
         return(false);
      }
   }


   colisionDetect.defineMap(actualMap);
   result = colisionDetect.canWalk(activeCharacter->posicaoLadoX + varX,
                                   activeCharacter->posicaoLadoY,
                                   activeCharacter->posicaoLadoZ + varZ,
                                   activeCharacter->min[0],
                                   activeCharacter->min[1],
                                   activeCharacter->min[2],
                                   activeCharacter->max[0],
                                   activeCharacter->max[1],
                                   activeCharacter->max[2],
                                   activeCharacter->orientacao + varAlpha, 
                                   activeCharacter->ocupaQuad, NPCs, varHeight,
                                   nx, nz);

   if(result)
   {
      /* Define New Occuped Square */
      int posX =(int)floor( nx / (SQUARESIZE));
      int posZ =(int)floor( nz / (SQUARESIZE));
      activeCharacter->ocupaQuad = actualMap->relativeSquare(posX,posZ);

      /* Define New Heigh */
      if(!defineActiveCharacterHeight(nx, nz))
      {
         /* Can't define new height or too much
          * to up or down, so can't move */
         return(false);
      }

      /* Apply VarHeight */
      activeCharacter->posicaoLadoY += varHeight;
      
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
 *                      defineActiveCharacterHeight                  *
 *********************************************************************/
bool engine::defineActiveCharacterHeight(GLfloat nx, GLfloat nz)
{
   personagem* activeCharacter = PCs->getActiveCharacter();
   GLfloat altura_atual = activeCharacter->posicaoLadoY;

   GLfloat res = actualMap->getHeight(nx, nz);

   if( res - altura_atual > ANDAR)
   {
       activeCharacter->posicaoLadoY = altura_atual;
       return(false);
   }

   activeCharacter->posicaoLadoY = res;
   return(true);
}

/*********************************************************************
 *                       Load MiniMap Window                         *
 *********************************************************************/
void engine::OpenMiniMapWindow()
{
   personagem* activeCharacter = PCs->getActiveCharacter();
   GLint x = (int)(((activeCharacter->posicaoLadoX) / (SQUARESIZE)));
   if(x > actualMap->getSizeX()-1)
   {
      x = actualMap->getSizeX()-1;
   }
   GLint z = (int)(((activeCharacter->posicaoLadoZ) / (SQUARESIZE)));
   if( z > actualMap->getSizeZ()-1)
   {
      z = actualMap->getSizeZ()-1;
   }
   x = 8 + (x*3);
   z = 20 + (z*3);
   miniMapWindow = gui->insertWindow(512,472,799,599,//0,344,255,471,
                                     language.WINDOW_MAP.c_str(),1,1);

   botPerMiniMap = miniMapWindow->objects->InserirBotao(x,z,x+2,z+2,255,255,128,
                                                      "",0,NULL);
   botPerMiniMap->Cores.corCont[0].R = 255;
   botPerMiniMap->Cores.corCont[0].G = 255;
   botPerMiniMap->Cores.corCont[0].B = 128;
   botPerMiniMap->Cores.corCont[1].R = 255;
   botPerMiniMap->Cores.corCont[1].G = 255;
   botPerMiniMap->Cores.corCont[1].B = 128;
   botPerMiniMap->Cores.corCont[2].R = 255;
   botPerMiniMap->Cores.corCont[2].G = 255;
   botPerMiniMap->Cores.corCont[2].B = 128;

   figura* fig = miniMapWindow->objects->InserirFigura(8,20,240,95,NULL);
   actualMap->drawMinimap(fig->fig);

   miniMapWindow->objects->InserirFigura(3,15,252,120,
                                       "../data/texturas/map.png");

   
                   
   miniMapWindow->ptrExterno = &miniMapWindow;
   gui->openWindow(miniMapWindow);
}

/*********************************************************************
 *                         Load ShortCuts Window                     *
 *********************************************************************/
void engine::OpenShortcutsWindow()
{
   shortCutsWindow = gui->insertWindow(0,472,511,599,
                                     language.WINDOW_SHORTCUTS.c_str(),1,1);
   FPS = shortCutsWindow->objects->InserirQuadroTexto(8,20,150/*100*/,35,2,
                                  language.WINDOW_SHORTCUTS_FPS.c_str());
   briefTxt = shortCutsWindow->objects->insertRolBar(8,36,249,100,
                                  language.WINDOW_SHORTCUTS_HELP.c_str(),
                                  shortCutsWindow->cara);
   //briefTxt->fonte = FMINI;
   /*ObjTxt->Cores.corCont[1].R = 0; ObjTxt->Cores.corCont[1].G = 25; 
   ObjTxt->Cores.corCont[1].B = 255;*/
   ObjTxt = shortCutsWindow->objects->InserirQuadroTexto(151,20,249,35,2,
                                 language.OBJ_NOTHING.c_str());

   buttonSave = shortCutsWindow->objects->InserirBotao(8,102,76,120,
                                 shortCutsWindow->Cores.corBot.R, 
                                 shortCutsWindow->Cores.corBot.G,
                                 shortCutsWindow->Cores.corBot.B,
                                 language.INITIAL_SAVE.c_str(),
                                 0,NULL);
   buttonMenu = shortCutsWindow->objects->InserirBotao(77,102,140,120,
                                 shortCutsWindow->Cores.corBot.R, 
                                 shortCutsWindow->Cores.corBot.G,
                                 shortCutsWindow->Cores.corBot.B,
                                 "Menu",
                                 0,NULL);
   buttonLoad = shortCutsWindow->objects->InserirBotao(141,102,209,120,
                                 shortCutsWindow->Cores.corBot.R, 
                                 shortCutsWindow->Cores.corBot.G,
                                 shortCutsWindow->Cores.corBot.B,
                                 language.INITIAL_LOAD.c_str(),
                                 0,NULL);
   hourTxt = shortCutsWindow->objects->InserirQuadroTexto(210,102,249,120,2,
                                                          "00:00");
   hourTxt->fonte = FMINI;
   hourTxt->tamFonte = 1;
   hourToTxt();

   tabButton* tb;
   tb = shortCutsWindow->objects->InserirTabButton(252,15,0,0,
                                             "../data/texturas/shortcuts.png");
   buttonAttackMode = tb->insertButton(7,4,43,36);/* Attack Mode */
   tb->insertButton(7,40,43,72);/* Attack 1 */
   tb->insertButton(7,75,43,107);/* Attack 7 */

   tb->insertButton(53,4,89,36);/* Guard/Sleep Mode */
   tb->insertButton(53,40,89,72);/* Attack 2 */
   tb->insertButton(53,75,89,107);/* Attack 8 */

   buttonInventory = tb->insertButton(99,4,135,36);/* Inventory */
   tb->insertButton(99,40,135,72);/* Attack 3 */
   tb->insertButton(99,75,135,107);/* Attack 9 */

   buttonMap = tb->insertButton(141,4,177,36);/* Map */
   tb->insertButton(141,40,177,72);/* Attack 4 */
   tb->insertButton(141,75,177,107);/* Attack 10 */
   
   tb->insertButton(180,4,216,36);/* Party */
   tb->insertButton(180,40,216,72);/* Attack 5 */
   tb->insertButton(180,75,216,107);/* Assign Attacks */

   tb->insertButton(220,4,256,36);/* Get */
   tb->insertButton(220,40,256,72);/* Attack 6 */
   buttonEndTurn = tb->insertButton(220,75,256,107);/* End Turn */

   shortCutsWindow->objects->InserirFigura(3,15,252,120,"../data/texturas/shortcut2.png");
   
   shortCutsWindow->ptrExterno = &shortCutsWindow;
   gui->openWindow(shortCutsWindow);
}

/*********************************************************************
 *                      OpenCloseInventoryWindow                     *
 *********************************************************************/
void engine::OpenCloseInventoryWindow()
{
   if(!inventoryWindow)
   {
      /* TODO get the right inventories!!! (when openning inventory from 
       * another person) */
      inventoryWindow = new inventWindow(PCs->getActiveCharacter()->inventories,
                                         gui); 
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
   personagem* pers = (personagem*) PCs->primeiro->proximo;
   while(pers != PCs->primeiro)
   {
      pers->defineActualLifePoints(pers->lifePoints);
      pers = (personagem*) pers->proximo;
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
   carregaTexturaRGBA(img,&id);
   SDL_FreeSurface(img);

   fadeInTexture(id,0,0,799,599);

   /* Wait until Mouse Button pressed */
   while(!(mButton & SDL_BUTTON(1)))
   {
      //Wait for Mouse Button Release
      SDL_PumpEvents();
      int x,y;
      mButton = SDL_GetMouseState(&x,&y);
      SDL_Delay(50);
   }

   fadeOutTexture(id,0,0,799,599);
   
   glEnable(GL_LIGHTING);
   glDeleteTextures(1,&id);
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

   int forcaAtualizacao = 0; //force screen atualization FIXME, no more used
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
   while(threatIO(surface,&forcaAtualizacao))
   {

     /* Verify Dead events */ 
     if(engineMode == ENGINE_MODE_DEAD)
     {
        if(SDL_GetTicks() - lastTurnTime >= ENGINE_ANIMATION_DELAY)
        {
           /* The Animation Delay is Ended, so Call Death Screen */
           showImage("../data/texturas/fightMode/death.png");
           engineMode = ENGINE_MODE_REAL_TIME;
           gui->closeAllWindows();
           /* Put the animation state on normal */
           PCs->getActiveCharacter()->setState(STATE_IDLE);
           return(0);
        }
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
              briefTxt->setText("|" + language.FIGHT_EXIT);
           }
           /* Verify if any PC is alive. */
           personagem* pers = (personagem*) PCs->primeiro->proximo;
           bool alive = false;
           while((!alive) && (pers != PCs->primeiro))
           {
              if(pers->isAlive())
              {
                 alive = true;
              }
              pers = (personagem*) pers->proximo;
           }
           if(!alive)
           {
              /* All Pcs are Dead, so Dead Mode! */
              engineMode = ENGINE_MODE_DEAD;
              lastTurnTime = time;
           }
        }
        /* Wait for animations before change the turn */
        else if( (fightStatus == FIGHT_CONTINUE) &&
                 ((time - lastTurnTime) > ENGINE_ANIMATION_DELAY) ) 
        {
           lastTurnTime = time;
           fightStatus = fight.doBattleCicle(brief);
           if(shortCutsWindow)
           {
              briefTxt->setText(brief);
           }
           actualizeAllHealthBars();

           if(fightStatus == FIGHT_PC_TURN)
           {
               if(fight.actualCharacterTurn()) 
               {
                  personagem* activeCharacter = PCs->getActiveCharacter();
                  PCs->setActiveCharacter(fight.actualCharacterTurn());
                  fullMovePCAction = false;
                  canMove = true;
                  //TODO Verify if weapon is ranged before do this
                  attackFeat = FEAT_MELEE_ATTACK;
                  canAttack = true;

                  moveCircleX = activeCharacter->posicaoLadoX;
                  moveCircleY = activeCharacter->posicaoLadoY;
                  moveCircleZ = activeCharacter->posicaoLadoZ;
               }
               else
               { //FIXME
                  fightStatus = FIGHT_CONTINUE;
               }
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
                personagem* per;
                per = NPCs->InserirPersonagem(6,8,3,8,
                             "../data/pics/logan/cara.bmp",0,0,
               "LoganNPC","../data/models/personagens/logan_completo_final.obj",
                  "../data/pics/logan/",features);
                per->posicaoLadoX = eventoRede->x;
                per->posicaoLadoZ = eventoRede->y; 
                per->orientacao = eventoRede->teta;
                per->ID = eventoRede->obj;
                forcaAtualizacao = 1;
                break; 
             }
             case MT_MOV: /* character movimentation */
             {
                personagem* per = (personagem*)NPCs->primeiro->proximo;
                if(per != NPCs->primeiro) 
                {
                   while((per!=NPCs->primeiro) && (eventoRede->obj!=per->ID))
                   {
                      per = (personagem*) per->proximo;
                   }
                   if(per!=NPCs->primeiro)
                   {
                       per->posicaoLadoX = eventoRede->x;
                       per->posicaoLadoZ = eventoRede->y; 
                       per->orientacao = eventoRede->teta;
		       forcaAtualizacao = 1;
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
                 personagem* activeCharacter = PCs->getActiveCharacter();
                 activeCharacter->ID = createchar( &clientData, 
                 activeCharacter->posicaoLadoX, 
                 activeCharacter->posicaoLadoZ, 
                 activeCharacter->orientacao );
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


