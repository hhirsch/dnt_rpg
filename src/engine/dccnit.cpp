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



#define REFRESH_RATE 100.0
#define ACTUALIZATION_RATE 20
#define WALK_ACTUALIZATION 0.02


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
   imgNumber = 0;

   walkStatus = ENGINE_WALK_KEYS;

   /* Define Camera initial Position */
   /*theta=25;
   phi=0.1;
   d=150;
   gameCamera.centerX = gameCamera.centerZ = 0;
   gameCamera.centerY = 30;
   deltaY = 0;
   cameraX = gameCamera.centerX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = gameCamera.centerY + (float) d * sin(deg2Rad(theta));
   cameraZ = gameCamera.centerZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));*/

   /* Initialize the Cursor */
   cursors = new(cursor);

   /* Initialize sounds and musics */
   snd = new(sound);
   music = NULL;

   /* Load Options */
   option = new options("dcc.opc");

   /* Load Language's files */
   language.ReloadFile(option->langNumber);

   /* Set sound and music volume, based on options */
   snd->ChangeVolume(option->musicVolume, option->sndfxVolume);
  
   /* Load Features List */
   features = new featsList(language.FEATS_DIR,"../data/feats/feats.ftl");

   /* Load Alignments & Tendecies */
   alignList = new aligns(language.ALIGN_DIR.c_str(), "../data/alignment/alignment.lst");

   /* Load Races */
   raceList = new races(language.RACE_DIR.c_str(), "../data/races/races.lst");


   /* Initialize readModes variables */
   lastRead = SDL_GetTicks();
   lastMouse = lastRead;
   lastKeyb = lastRead;
   mouseX = 0;
   mouseY = 0;

   particleSystem = new partSystem();

   hour = 9.0;
   gameSun = new sun(hour , FARVIEW, FARVIEW);

   engineMode = ENGINE_MODE_REAL_TIME;

   waveTest = new waves("", 300, 20, 300, 5, 20);

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
   if(music)
   {
      snd->StopMusic(music);
   }
   delete(snd);

   if(waveTest)
      delete(waveTest);

   /* Delete particles */
   if(particleSystem != NULL)
      delete(particleSystem);

   /* Close option */
   delete(option);

   /* Clear Sky */
   glDeleteLists(skyList,1);
   glDeleteTextures(1, &sky);

   /* Clear Other Textures */
   glDeleteTextures(1, &normalMoveCircle);
   glDeleteTextures(1, &fullMoveCircle);
   glDeleteTextures(1, &destinyImage);

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

   delete(alignList);

   delete(features);

   delete(raceList);
}

/*********************************************************************
 *                       Informations Screen                         *
 *********************************************************************/
void engine::InformationScreen()
{
   Uint8 *keys;
   SDL_Surface* img = IMG_Load(language.TEXTURE_INFORMATION.c_str());

   GLuint texturaInfo;
   carregaTextura(img,&texturaInfo);

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
 *                         Load Map to Engine                        *
 *********************************************************************/
int engine::LoadMap(string arqMapa, int RecarregaPCs)
{
   char texto[255];
   string arqVelho = "nada";
   
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

   AtualizaFrustum(visibleMatrix,proj,modl);
   AtualizaTela2D(texturaCarga,proj,modl,viewPort,272,236,527,363,0.01);
   AtualizaTela2D(texturaTexto,proj,modl,viewPort,272,365,527,396,0.01);
   glFlush();
   SDL_GL_SwapBuffers();

   /* Loading Map */
   if(actualMap) 
   {
     arqVelho = actualMap->name;
     delete(actualMap);
   }
   actualMap = new(Map);
   actualMap->name = arqVelho;
   actualMap->open(arqMapa);

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
      glDisable(GL_FOG);
   }

   /* Loading NPCs */
   if(NPCs)
     delete(NPCs);
   NPCs = NULL;
   personagem* per;
   if(!actualMap->npcFileName.empty())
   {
      FILE* arq;
      if(!(arq = fopen(actualMap->npcFileName.c_str(),"r")))
      {
         printf("Ouch, can't load NPC's file: %s.\n",actualMap->npcFileName.c_str());
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
           per = NPCs->InserirPersonagem(arquivo,features);
           per->posicaoLadoX = posX;
           per->posicaoLadoZ = posZ;
         }
         fclose(arq);
      }  
   }

   /* Loading PCs */
   if(RecarregaPCs)
   {
       if(PCs)
          delete(PCs);
       PCs  = new (Lpersonagem);
       sprintf(texto, language.LOAD_PC.c_str(), "Logan");
       atualizaCarga(img,&texturaTexto,texturaCarga,
                 texto,
                 proj, modl, viewPort);
       per = PCs->InserirPersonagem("../data/characters/pcs/logan.pc",features);
       per->DefineMaxLifePoints(per->maxLifePoints);
       atualizaCarga(img,&texturaTexto,texturaCarga,
                 "Loading Character: Gushm",
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
     miniMapWindow->Fechar(gui->ljan);
   if(shortCutsWindow)
     shortCutsWindow->Fechar(gui->ljan);
   OpenMiniMapWindow();
   OpenShortcutsWindow();

   /* Updating the BoundingBoxes for PCs */
   int aux;
   per = (personagem*) PCs->primeiro->proximo;
   for(aux=0;aux < PCs->total;aux++)
   {
      per->m_calModel->update(0); 
      per->CalculateBoundingBox();  
      //FIXME gambiarra to make logan bounding ok when arms down.
        per->max[0] /= 2.0;
        per->min[0] /= 2.0;
      per = (personagem*) per->proximo;
   }

   /* Updating the BoundingBoxes for NPCs */
   if(NPCs)
   { 
      per = (personagem*) NPCs->primeiro->proximo;
      for(aux=0; aux < NPCs->total;aux++)
      {
         per->m_calModel->update(0); 
         per->CalculateBoundingBox();  
         per = (personagem*) per->proximo;
      }
   }

   /* Update  particle System to a stable state */
   atualizaCarga(img,&texturaTexto,texturaCarga,
                 language.LOAD_PARTICLE.c_str(),
                 proj, modl, viewPort);
   if(!actualMap->particlesFileName.empty())
   {
       particleSystem->loadFromFile(actualMap->particlesFileName);
       particleSystem->stabilizeAll();
   }
   else
   {
      particleSystem->deleteAll();
   }
   

   glEnable(GL_LIGHTING);


   /* Put Active Party on Init Position */
   PCs->personagemAtivo->posicaoLadoX = actualMap->xInic;
   PCs->personagemAtivo->posicaoLadoZ = actualMap->zInic;
   gameCamera.centerX = actualMap->xInic;
   gameCamera.centerZ = actualMap->zInic;
   PCs->personagemAtivo->ocupaQuad = actualMap->squareInic;

   atualizaCarga(img,&texturaTexto,texturaCarga,
                 language.LOAD_DONE.c_str(),
                 proj, modl, viewPort);

   /* Free Loading Textures */
   SDL_FreeSurface(img);
   glDeleteTextures(1,&texturaCarga);
   glDeleteTextures(1,&texturaTexto);

   /* Change Music, if needed */
   if(!actualMap->music.empty())
   {
      snd->StopMusic(music);
      music = snd->LoadMusic(actualMap->music);
   }

   return(1);
}

/*********************************************************************
 *                       Call Initial Game Menu                      *
 *********************************************************************/
int engine::InitialScreen(int Status, GLuint* idTextura, bool reloadMusic)
{
   /* Reload Music, if needed */
   if( (music) && (reloadMusic) )
   {
     snd->StopMusic(music);
   }
   if(reloadMusic)
      music = snd->LoadMusic("../data/music/musica1.ogg");

   /* Executes Initial Screen */
   AtualizaFrustum(visibleMatrix,proj,modl);
   initialScreen* inic = new(initialScreen);
   int result = inic->Execute(Status, proj, modl, viewPort, idTextura);
   delete(inic);
   return(result);
}

/*********************************************************************
 *                       Call Options Game Screen                    *
 *********************************************************************/
int engine::OptionsScreen(GLuint* idTextura)
{
   interface* interf = new interface(NULL);

   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);

   int optionW = OPTIONSW_OTHER;
   int tempo = SDL_GetTicks();
   int tempoAnterior = 0;
   Uint8* keys;
   int x,y;
   Tobjeto* object = NULL;
   int eventInfo = NADA;

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
         AtualizaTela2D(*idTextura,proj,modl,viewPort,0,0,799,599,0.012);
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

   snd->ChangeVolume(option->musicVolume, option->sndfxVolume);

   delete(alignList);
   alignList = new aligns(language.ALIGN_DIR.c_str(),
                          "../data/alignment/alignment.lst");

   glEnable(GL_LIGHTING);
   SDL_ShowCursor(SDL_DISABLE);

   delete(interf);

   return(optionW);
}

/*********************************************************************
 *              Call Screens to Create, Evolute Character            *
 *********************************************************************/
int engine::CharacterScreen(GLuint* idTextura)
{
   int charCreation = CHAR_OTHER;
   int tempo = SDL_GetTicks();
   int tempoAnterior = 0;
   Uint8* keys;
   int x,y;
   Tobjeto* object = NULL;
   int eventInfo = NADA;

   int status = 0;

   align* selected; // TODO remove it from here
   race* sel; //TODO remove from here

  
   /*TODO Other screens*/

   /* TODO  put on character screen */
   
   skills* sk = new skills(language.SKILLS_DIR.c_str(),
                           "../data/skills/skills.skl"); 

   /* Att Screen */
   attWindow* atWindow = NULL;//new attWindow(sk, gui);

   /* Skill Screen */
   skillWindow* skWindow = NULL;

   /* Alignment Window */
   alignWindow* alWindow = NULL;

   /* Race Window */
   raceWindow* rcWindow = new raceWindow(raceList, gui);

   while( (status != 4) )
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

         AtualizaTela2D(*idTextura,proj,modl,viewPort,0,0,799,599,0.012);
         gui->draw(proj,modl,viewPort);
         glFlush();
         SDL_GL_SwapBuffers();

         if(status == 0)
         {
            charCreation = rcWindow->treat(object, eventInfo, gui, &sel);
            if(charCreation == RACEW_CONFIRM)
            {
               status = 1;
               delete(rcWindow);
               alWindow = new alignWindow(alignList, gui);
            }
            else if(charCreation == RACEW_CANCEL)
            {
               status = 4;
               delete(rcWindow);
               charCreation = CHAR_CANCEL;
            }
         }
         else if(status == 1)
         {
            charCreation = alWindow->treat(object, eventInfo, gui, &selected);
            if(charCreation == ALIGNW_CONFIRM)
            {
               status = 2;
               delete(alWindow);
               atWindow = new attWindow(sk, gui);
            }
            else if(charCreation == ALIGNW_CANCEL)
            {
               status = 0;
               delete(alWindow);
               rcWindow = new raceWindow(raceList, gui);
            }
         }
         else if(status == 2)
         {
             charCreation = atWindow->treat(object, eventInfo, gui,
                                            proj, modl,viewPort);
             if(charCreation == ATTW_CONFIRM)
             {
                status = 3;
                delete(atWindow);
                skWindow = new skillWindow(sk, 20, gui);
             }
             else if(charCreation == ATTW_CANCEL)
             {
                status = 1;
                delete(atWindow);
                alWindow = new alignWindow(alignList, gui);
             }
         }
         else if(status == 3)
         {
            charCreation = skWindow->treat(object, eventInfo, gui); 
            if(charCreation == SKILLW_CONFIRM)
            {
               status = 4;
               delete(skWindow);
               charCreation = CHAR_CONFIRM;
            }
            else if(charCreation == SKILLW_CANCEL)
            {
               status = 2;
               delete(skWindow);
               atWindow = new attWindow(sk, gui);
            }
         }
         
      }
      else if((ACTUALIZATION_RATE-1) - (tempo - tempoAnterior) > 0 ) 
      {
         SDL_Delay((ACTUALIZATION_RATE-1) - (tempo - tempoAnterior) );
      }
   }
   
   delete(sk);
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
 *                  Draw a sphere Sky to SkyList                     *
 *********************************************************************/
void engine::drawSphereToList(int lats, int longs) 
{
   skyList = glGenLists(1);
   glNewList(skyList,GL_COMPILE);

   int i,j;
   GLfloat theta1,theta2,theta3;
   GLfloat ex,ey,ez;
   int n = lats;

   for (j=0;j<n/2;j++) {
      theta1 = j * TWOPI / n - PID2;
      theta2 = (j + 1) * TWOPI / n - PID2;

      glBegin(GL_QUAD_STRIP);
      for (i=0;i<=n;i++) {
         theta3 = i * TWOPI / n;

         ex = cos(theta2) * cos(theta3);
         ey = sin(theta2);
         ez = cos(theta2) * sin(theta3);

         glNormal3f(ex,ey,ez);
         glTexCoord2f(i/(GLfloat)n,2*(j+1)/(GLfloat)n);
         glVertex3f(ex,ey,ez);

         ex = cos(theta1) * cos(theta3);
         ey = sin(theta1);
         ez = cos(theta1) * sin(theta3);

         glNormal3f(ex,ey,ez);
         glTexCoord2f(i/(GLfloat)n,2*j/(GLfloat)n);
         glVertex3f(ex,ey,ez);
      }
      glEnd();
   }

   glEndList();
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

   /* Sky Creation */
   drawSphereToList(80,80);

   SDL_Surface* img = IMG_Load("../data/texturas/ceu.jpg");
   glGenTextures(1, &sky);
   glBindTexture(GL_TEXTURE_2D, sky);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 
                0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);

   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                   GL_LINEAR_MIPMAP_LINEAR );
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, img->w,
                     img->h, GL_RGB, GL_UNSIGNED_BYTE, 
                     img->pixels );

   SDL_FreeSurface(img);

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
 *                         Threat GUI Events                         *
 *********************************************************************/
void engine::threatGuiEvents(Tobjeto* object, int eventInfo)
{
   int numEnemies = 0;
   personagem* ch;
   string briefInit;
   string brief;

   //FIXME not here the dices!
   diceThing dc;
   dc.baseDice.diceID = DICE_D8;
   dc.baseDice.numberOfDices = 2;
   dc.baseDice.sumNumber = 4;
   dc.initialLevel = 1;

   switch(eventInfo)
   {
       case TABBOTAOPRESSIONADO:
       {
           if(object == (Tobjeto*) buttonAttackMode)
           {
              if( engineMode != ENGINE_MODE_TURN_BATTLE )
              {
                 brief = "";
                 fight.empty();
                 if(!NPCs)
                 {
                    if(shortCutsWindow != NULL)
                    {
                       briefTxt->texto = language.FIGHT_NO_NPCS;
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
                    snd->LoadSample(SOUND_ACTION3,"../data/sndfx/battleMode.ogg");
                    snd->PlaySample(SOUND_ACTION3);
                    engineMode = ENGINE_MODE_TURN_BATTLE;
                    moveCircleX = PCs->personagemAtivo->posicaoLadoX;
                    moveCircleY = PCs->personagemAtivo->posicaoLadoY;
                    moveCircleZ = PCs->personagemAtivo->posicaoLadoZ;

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
                    fightStatus = FIGHT_PC_TURN;
                    fullMovePCAction = false;
                    canMove = true;
                    //TODO Verify if weapon is ranged before do this
                    attackFeat = FEAT_MELEE_ATTACK;
                    canAttack = true;

                    brief += language.FIGHT_SURPRISE_TURN;
                 }
                 else
                 {
                    brief = language.FIGHT_NO_NPCS;
                 }
                 if(shortCutsWindow != NULL)
                 {
                    briefTxt->texto = brief;
                 }
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
              /* END TURN */
              if( (engineMode == ENGINE_MODE_TURN_BATTLE) &&
                  (fightStatus == FIGHT_PC_TURN)  && 
                  ((SDL_GetTicks() - lastTurnTime) > 200) )
              {
                 fightStatus = FIGHT_CONTINUE;
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
         break;
       }
   }
}

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
      for(aux=0;aux< PCs->total;aux++)
      {
         per->m_calModel->update(WALK_ACTUALIZATION/*seconds*/); 
         //per->CalculateBoundingBox(); 
         per = (personagem*) per->proximo;
      }
    
      if(NPCs)
      {
        per = (personagem*) NPCs->primeiro->proximo;
        for(aux=0;aux < NPCs->total;aux++)
        {
           per->m_calModel->update(WALK_ACTUALIZATION/*seconds*/);   
           per->CalculateBoundingBox();
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

      /* GUI Events */
      Tobjeto* object;
      object = gui->manipulateEvents(x,y,Mbotao,keys, &guiEvent);
      if(guiEvent != NADA)
      {
         threatGuiEvents(object, guiEvent);
         redesenha = true;
      }
      else
      { 
      if( (tempo-lastMouse>=  REFRESH_RATE ) || 
          ( (Mbotao & SDL_BUTTON(1)) && 
	    (tempo-lastMousePression >= REFRESH_RATE)) )
      {
         cursors->SetActual(CURSOR_WALK);
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
         Square* quaux = actualMap->quadradoRelativo(qx,qz);
       if(quaux != NULL)
       {
         int pronto;
         int obj = 0;
         GLfloat minObj[3], maxObj[3];
         for(pronto = 0; ( (obj<MAXOBJETOS) && (!pronto) );obj++)
         {
            if(quaux->objects[obj])
            {
               GLMmodel* modelo3d = (GLMmodel*) quaux->objects[obj]->modelo3d;
               GLfloat X[4]; GLfloat Z[4];
               X[0] = modelo3d->x1;
               Z[0] = modelo3d->z1;
               X[1] = modelo3d->x1;
               Z[1] = modelo3d->z2; 
               X[2] = modelo3d->x2;
               Z[2] = modelo3d->z2;
               X[3] = modelo3d->x2;
               Z[3] = modelo3d->z1;
               rotTransBoundingBox(quaux->objectsOrientation[obj], X, Z,
                              quaux->Xobjects[obj], 0.0, 
                              0.0,quaux->Zobjects[obj], 
                              minObj, maxObj);
               if(estaDentro( minObj, maxObj, minMouse, maxMouse, 1))
               {
                   cursors->SetActual(CURSOR_GET);
                   if(shortCutsWindow)
                   {
                      ObjTxt->texto = quaux->objects[obj]->nome; 
                      shortCutsWindow->Desenhar(mouseX,mouseY);
                   }
                   if(Mbotao & SDL_BUTTON(1))
                   {
                      lastMousePression = tempo;
                   }
                   pronto = 1;
               }
            }
         }

         /* Doors Verification */
         door* porta = actualMap->portas;
         while( (porta != NULL) && (!pronto) )
         {
             GLMmodel* modelo3d = (GLMmodel*) porta->object->modelo3d;
             GLfloat X[4]; GLfloat Z[4];
             X[0] = modelo3d->x1;
             Z[0] = modelo3d->z1;
             X[1] = modelo3d->x1;
             Z[1] = modelo3d->z2; 
             X[2] = modelo3d->x2;
             Z[2] = modelo3d->z2;
             X[3] = modelo3d->x2;
             Z[3] = modelo3d->z1;
             rotTransBoundingBox(porta->orientacao, X, Z,
                                 porta->x, 0.0, 
                                 0.0,porta->z, 
                                 minObj, maxObj);
             if(estaDentro( minObj, maxObj, minMouse, maxMouse, 1))
             {
                 cursors->SetActual(CURSOR_DOOR);
                 if(shortCutsWindow)
                 {
                    ObjTxt->texto = language.OBJ_DOOR.c_str(); 
                    shortCutsWindow->Desenhar(mouseX, mouseY);
                 }
                 if(Mbotao & SDL_BUTTON(1))
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


         /* TODO Inventory Verification */
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
                cursors->SetActual(CURSOR_INVENTORY);
                if(shortCutsWindow)
                {
                   ObjTxt->texto = pers->nome; 
                   shortCutsWindow->Desenhar(mouseX, mouseY);
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
                    if(pers->dead) 
                    {
                       cursors->SetActual(CURSOR_GET);
                    }
                    else
                    {
                       cursors->SetActual(CURSOR_TALK);
                       //TODO Talk
                    }
                    if(shortCutsWindow)
                    {
                       ObjTxt->texto = pers->nome; 
                       shortCutsWindow->Desenhar(mouseX, mouseY);
                    }
                    pronto = 1;
                 }
                 else
                 if( (engineMode == ENGINE_MODE_TURN_BATTLE) && (canAttack) &&
                     (fightStatus == FIGHT_PC_TURN) && (!fullMovePCAction))
                 {
                     //TODO verify in-range distances
                     string brief = "";
                     cursors->SetActual(CURSOR_ATTACK);
                     if(shortCutsWindow)
                     {
                        ObjTxt->texto = pers->nome; 
                        shortCutsWindow->Desenhar(mouseX, mouseY);
                     }
                     if(Mbotao & SDL_BUTTON(1))
                     {
                        brief = PCs->personagemAtivo->nome + " " + 
                                language.FIGHT_ATTACKS + " " + 
                                pers->nome + "|";
                        canAttack = !PCs->personagemAtivo->actualFeats.
                                                        applyAttackAndBreakFeat(
                                                          *PCs->personagemAtivo,
                                                          attackFeat, *pers, 
                                                          brief);
                        if(pers->dead)
                        {
                           brief += "|" + pers->nome + " " +  
                                    language.FIGHT_DEAD;
                        }
                        if( pers->psychoState != PSYCHO_HOSTILE)
                        {
                            pers->psychoState = PSYCHO_HOSTILE;
                        }

                     }
                     if(shortCutsWindow != NULL)
                     {
                        briefTxt->texto = brief;
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
               cursors->SetActual(CURSOR_MAPTRAVEL);
               pronto = 1;
               if(Mbotao & SDL_BUTTON(1))
               {
                  LoadMap(quaux->mapConection.mapName, 0);
                  return(1);
               }
            }
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
         if ( keys[SDLK_ESCAPE] ) // Exit Engine
            return(0);

         if(keys[SDLK_m]) //Open Minimap
         {
             if(!miniMapWindow)
             {
               OpenMiniMapWindow();
               redesenha = true;
             }
         }

         /* Temporariamente, para visualizar o efeito de sangue */
         if(keys[SDLK_y])
         {
            if(!effect)
            {
               effect = (part2*)particleSystem->addParticle(PART_FIRE,
                                          PCs->personagemAtivo->posicaoLadoX,0,
                                          PCs->personagemAtivo->posicaoLadoZ,
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
                                    PCs->personagemAtivo->posicaoLadoX,28,
                                    PCs->personagemAtivo->posicaoLadoZ, 
                                    "../data/particles/blood1.par");
            if(p)
               p->followPC = true;
         }   
         if(keys[SDLK_o])
         {
             part5 *p;
             p = (part5*) particleSystem->addParticle(PART_BLOOD,
                                         PCs->personagemAtivo->posicaoLadoX,28,
                                         PCs->personagemAtivo->posicaoLadoZ, 
                                         "../data/particles/blood2.par");
             if(p) 
               p->followPC = true;
         }
         if(keys[SDLK_l])
         {
            particleSystem->addParticle(PART_LIGHTNING,
                                        PCs->personagemAtivo->posicaoLadoX,250,
                                        PCs->personagemAtivo->posicaoLadoZ,
                                        "../data/particles/lightning1.par");
         }

         int i;
         if(keys[SDLK_z])
         {
            for(i=0; i<5;i++)
            {
               waveTest->insertWave(i, 0, 2, 0.2, 200, 0, WAVE_DIRECTION_DOWN);
            }
         }
         if(keys[SDLK_x])
         {
            for(i=0; i<20;i++)
            {
               waveTest->insertWave(0, i, 2, 0.2, 200, 0, WAVE_DIRECTION_RIGHT);
            }
         }

      }

#ifdef VIDEO_MODE
   if(keys[SDLK_v])
   {
      startVideo = true;
      printf("Started Video\n");
   }
#endif

      if(keys[SDLK_n])
      {
          if(!shortCutsWindow)
          {
              OpenShortcutsWindow();
              redesenha = true;
          }
      }

      if(keys[SDLK_F1]) //Call Information Screen
      {
         InformationScreen();
         redesenha = true;
      }

      

      /* Keys to character's movimentation */
      if(keys[SDLK_q] || keys[SDLK_e])
      {
         walkStatus = ENGINE_WALK_KEYS;
          varX = passo * sin(deg2Rad(PCs->personagemAtivo->orientacao+90.0));
          varZ = passo * cos(deg2Rad(PCs->personagemAtivo->orientacao+90.0));
         // Left walk
         if(keys[SDLK_q]) 
         {
             varX *= -1;
             varZ *= -1;
         }

         if(canWalk(varX,varZ,0)) 
         {
            gameCamera.centerX += varX;
            gameCamera.centerZ += varZ;
            PCs->personagemAtivo->posicaoLadoX += varX;
            PCs->personagemAtivo->posicaoLadoZ += varZ;
            redesenha = true;
            andou = true;
         }
         else if( ((varX > 0) && (canWalk(passo,0,0))) ||
                  ((varX < 0) && (canWalk(-passo,0,0))) )       
         {
            if(varX < 0)
               passo *= -1;
            gameCamera.centerX += passo;
            PCs->personagemAtivo->posicaoLadoX += passo;
            redesenha = true;
            andou = true;
         }
         else if( ((varZ > 0) && canWalk(0,passo,0)) ||
                  ((varZ < 0) && canWalk(0,-passo,0)) )
         {
            if(varZ < 0) 
               passo *= -1;
            gameCamera.centerZ += passo;
            PCs->personagemAtivo->posicaoLadoZ += passo;
            redesenha = true;
            andou = true;
         }
        
      }
      else if(keys[SDLK_w] || keys[SDLK_s])
      { 
         walkStatus = ENGINE_WALK_KEYS;
         varX = passo * sin(deg2Rad(PCs->personagemAtivo->orientacao));
         varZ = passo * cos(deg2Rad(PCs->personagemAtivo->orientacao));
         if(keys[SDLK_w]) 
         {
              varX *= -1;
              varZ *= -1;
         }
         if((canWalk(varX,varZ,0)) ) 
         {
             PCs->personagemAtivo->posicaoLadoX += varX;
             PCs->personagemAtivo->posicaoLadoZ += varZ;
             gameCamera.centerX += varX;
             gameCamera.centerZ += varZ;
             redesenha = true;
             andou  = true;
         }
         else if( ((varX > 0) && (canWalk(passo,0,0))) ||
                  ((varX < 0) && (canWalk(-passo,0,0))) ) 
                   
         {
              if(varX < 0 )
                 passo *= -1;
              PCs->personagemAtivo->posicaoLadoX += passo;
              gameCamera.centerX += passo;
              redesenha = true;
              andou = true;
         }
         else if( ((varZ > 0) && (canWalk(0,passo,0))) ||
                  ((varZ < 0) && (canWalk(0,-passo,0))) )
         {
              if( varZ < 0 )
                 passo *= -1;
              PCs->personagemAtivo->posicaoLadoZ += passo;
              gameCamera.centerZ += passo;
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
            PCs->personagemAtivo->orientacao += rotacao;
            if(PCs->personagemAtivo->orientacao > 360.0)
               PCs->personagemAtivo->orientacao = 
                               PCs->personagemAtivo->orientacao  - 360.0;
            redesenha = true;
            andou = true;
         }
         // Clockwise Character Turn
         if((keys[SDLK_d]) && (canWalk(0,0,-rotacao)) )
         {
            PCs->personagemAtivo->orientacao -= rotacao;
            if(PCs->personagemAtivo->orientacao < 0.0)
               PCs->personagemAtivo->orientacao = 360.0 + 
                                        PCs->personagemAtivo->orientacao ;
         }
         redesenha = true;
         andou = true;
      }
      if(keys[SDLK_TAB]) //Activate Character
      {
         walkStatus = ENGINE_WALK_KEYS;
         if(keys[SDLK_LCTRL]) //Previous Character
         {
            PCs->personagemAtivo = (personagem*)PCs->personagemAtivo->anterior;
            if(PCs->personagemAtivo == PCs->primeiro)
               PCs->personagemAtivo = (personagem*)PCs->primeiro->anterior;
         }
         else //Next Character
         {
            PCs->personagemAtivo = (personagem*)PCs->personagemAtivo->proximo;
            if(PCs->personagemAtivo == PCs->primeiro)
               PCs->personagemAtivo = (personagem*)PCs->primeiro->proximo;
         }
         gameCamera.centerX = PCs->personagemAtivo->posicaoLadoX;
         gameCamera.centerZ = PCs->personagemAtivo->posicaoLadoZ;
         redesenha = true;
         SDL_Delay(100);
      }

      /* Camera Verification */
      if(gameCamera.doIO(keys, Mbotao, x, y, varCamera ))
      {
         redesenha = true;
      }
      
      /* Mouse Verification */
      if(Mbotao & SDL_BUTTON(3))
      {
         
         PCs->personagemAtivo->pathFind.defineMap(actualMap);
         if(PCs->personagemAtivo->pathFind.findPath(
                                             PCs->personagemAtivo->posicaoLadoX,
                                             PCs->personagemAtivo->posicaoLadoZ,
                                             xReal, zReal, ANDAR, 
                                             PCs->personagemAtivo->orientacao,
                                             PCs->personagemAtivo->min[0],
                                             PCs->personagemAtivo->min[1],
                                             PCs->personagemAtivo->min[2],
                                             PCs->personagemAtivo->max[0],
                                             PCs->personagemAtivo->max[1],
                                             PCs->personagemAtivo->max[2]))
         {
            //Found path to, so walk
            walkStatus = ENGINE_WALK_MOUSE;
         }
         else
         {
            walkStatus = ENGINE_WALK_KEYS;
         }
      }

      if(walkStatus == ENGINE_WALK_MOUSE)
      {
            if(! PCs->personagemAtivo->pathFind.getNewPosition(
                                             PCs->personagemAtivo->posicaoLadoX,
                                             PCs->personagemAtivo->posicaoLadoZ,
                                             PCs->personagemAtivo->orientacao))
            {
               walkStatus = ENGINE_WALK_KEYS;
            }
            gameCamera.centerX = PCs->personagemAtivo->posicaoLadoX;
            gameCamera.centerZ = PCs->personagemAtivo->posicaoLadoZ;
            redesenha = true;
            andou = true;
      }

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
      if(miniMapWindow)
      {
         GLint x = (int)(((PCs->personagemAtivo->posicaoLadoX) / (SQUARESIZE)));
         if(x > actualMap->x-1)
         {
            x = actualMap->x-1;
         }
         GLint z = (int)(((PCs->personagemAtivo->posicaoLadoZ) / (SQUARESIZE)));
         if( z > actualMap->z-1)
         {
            z = actualMap->z-1;
         }
         x = 8 + (x*4);
         z = 20 + (z*4);

         botPerMiniMap->x1 = x;
         botPerMiniMap->x2 = x+3;
         botPerMiniMap->y1 = z;
         botPerMiniMap->y2 = z+3;

         miniMapWindow->Desenhar(mouseX, mouseY);
      }
      
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
         shortCutsWindow->Desenhar(mouseX, mouseY);
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
      snd->PlaySample(SOUND_WALK);
      PCs->personagemAtivo->SetState(STATE_WALK);
      #ifdef REDE
        movchar(&clientData, PCs->personagemAtivo->ID, 
          PCs->personagemAtivo->posicaoLadoX,PCs->personagemAtivo->posicaoLadoZ,
          PCs->personagemAtivo->orientacao );
      #endif
   }
   else if(passouTempo)
   { 
      if( (PCs->personagemAtivo->GetState() == STATE_WALK) &&
          (engineMode == ENGINE_MODE_TURN_BATTLE) && 
          (fightStatus == FIGHT_PC_TURN) )
      {
         canMove = false;
      }
      PCs->personagemAtivo->SetState(STATE_IDLE);
      snd->StopSample(SOUND_WALK);
   }
 
   return(!exitEngine);
}

/*********************************************************************
 *                       Draw Scene Function                         *
 *********************************************************************/
void engine::Draw()
{
   GLdouble x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4;

   GLfloat min[3],max[3];
   GLfloat x[4],z[4];

   glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   glLoadIdentity();

   /* Redefine camera position */
   gameCamera.lookAt();

   /* Sun Definition */
   gameSun->actualizeHourOfDay(hour);
   
   /* Other Lights */
   /*GLfloat light_position[] = { 183, 10.0, 327.5, 1.0 };
   glLightfv(GL_LIGHT1, GL_POSITION, light_position);
   glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.02);
   glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.01);
   glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.00088);*/

   /* Atualize to culling and to GUI */
   AtualizaFrustum(visibleMatrix,proj,modl);

   /* SKY */
   glPushMatrix();
      //glDisable(GL_LIGHTING);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, sky);
      glTranslatef(actualMap->x*HALFSQUARESIZE, 0 , actualMap->z*HALFSQUARESIZE);
      glScalef(HALFFARVIEW,HALFFARVIEW,HALFFARVIEW);
      glRotated(90,0,1,0);
      glRotated(180,1,0,0);
      //gluSphere(atmosfera,HALFFARVIEW,5,5);
      glCallList(skyList);
      glDisable(GL_TEXTURE_2D);
      //glEnable(GL_LIGHTING);
   glPopMatrix();

   glPushMatrix();

   /* Draws World, doing view frustum culling */
   actualMap->draw(gameCamera.getCameraX(),gameCamera.getCameraY(),
                   gameCamera.getCameraZ(),visibleMatrix);

   /* Draw Playable Characters (PCs) */
      personagem* per = (personagem*) PCs->primeiro->proximo;
      int aux;
      for(aux=0;aux < PCs->total;aux++)
      {
         /* Draw Character */
         glPushMatrix();
           glTranslatef(per->posicaoLadoX, per->posicaoLadoY,
                        per->posicaoLadoZ);
           glRotatef(per->orientacao,0,1,0);
           per->Render();
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
      for(aux=0;aux < NPCs->total;aux++)
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
              per->Render();
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

   /* Draw Combat Mode Things */
   if( (engineMode == ENGINE_MODE_TURN_BATTLE) && 
       (fightStatus == FIGHT_PC_TURN))
   {
       glDisable(GL_LIGHTING);
       /* Draw Movimentation Circles */
       glColor4f(1,1,1,1);
       glEnable(GL_BLEND);
       glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

       /* Full Circle */
       glPushMatrix();
         glEnable(GL_TEXTURE_2D);
         glBindTexture(GL_TEXTURE_2D, fullMoveCircle );
         glBegin(GL_QUADS);
            glTexCoord2f(0,0);
            glVertex3f(moveCircleX-2*WALK_PER_MOVE_ACTION, 0.1, 
                       moveCircleZ-2*WALK_PER_MOVE_ACTION);
            glTexCoord2f(0,1);
            glVertex3f(moveCircleX-2*WALK_PER_MOVE_ACTION, 0.1, 
                       moveCircleZ+2*WALK_PER_MOVE_ACTION);
            glTexCoord2f(1,1);
            glVertex3f(moveCircleX+2*WALK_PER_MOVE_ACTION, 0.1, 
                       moveCircleZ+2*WALK_PER_MOVE_ACTION);
            glTexCoord2f(1,0);
            glVertex3f(moveCircleX+2*WALK_PER_MOVE_ACTION, 0.1, 
                       moveCircleZ-2*WALK_PER_MOVE_ACTION);
         glEnd();
         glDisable(GL_TEXTURE_2D);
      glPopMatrix();


       /* Normal Circle */
       glPushMatrix();
         glEnable(GL_TEXTURE_2D);
         glBindTexture(GL_TEXTURE_2D, normalMoveCircle );
         glBegin(GL_QUADS);
            glTexCoord2f(0,0);
            glVertex3f(moveCircleX-WALK_PER_MOVE_ACTION, 0.2, 
                       moveCircleZ-WALK_PER_MOVE_ACTION);
            glTexCoord2f(0,1);
            glVertex3f(moveCircleX-WALK_PER_MOVE_ACTION, 0.2, 
                       moveCircleZ+WALK_PER_MOVE_ACTION);
            glTexCoord2f(1,1);
            glVertex3f(moveCircleX+WALK_PER_MOVE_ACTION, 0.2, 
                       moveCircleZ+WALK_PER_MOVE_ACTION);
            glTexCoord2f(1,0);
            glVertex3f(moveCircleX+WALK_PER_MOVE_ACTION, 0.2, 
                       moveCircleZ-WALK_PER_MOVE_ACTION);
         glEnd();
         glDisable(GL_TEXTURE_2D);
      glPopMatrix();

      glEnable(GL_LIGHTING);

      glDisable(GL_BLEND);
   }

   if(walkStatus == ENGINE_WALK_MOUSE)
   {
      GLfloat destX =0, destZ=0;
      //PCs->personagemAtivo->pathFind.drawPath();
      PCs->personagemAtivo->pathFind.getDestiny(destX, destZ);
      
      glDisable(GL_LIGHTING);
       /* Draw Movimentation Destiny */
       if(destinyVariation >= 2.0)
       {
          destinyVariation = -2.0;
       }
       destinyVariation += 0.1;
       glColor4f(1,1,1,1);
       glEnable(GL_BLEND);
       glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
       glPushMatrix();
         glEnable(GL_TEXTURE_2D);
         glBindTexture(GL_TEXTURE_2D, destinyImage );
         glBegin(GL_QUADS);
            glTexCoord2f(0,0);
            glVertex3f(destX-(4+destinyVariation), 0.25, 
                       destZ-(4+destinyVariation));
            glTexCoord2f(0,1);
            glVertex3f(destX-(4+destinyVariation), 0.25, 
                       destZ+(4+destinyVariation));
            glTexCoord2f(1,1);
            glVertex3f(destX+(4+destinyVariation), 0.25, 
                       destZ+(4+destinyVariation));
            glTexCoord2f(1,0);
            glVertex3f(destX+(4+destinyVariation), 0.25, 
                       destZ-(4+destinyVariation));
         glEnd();
         glDisable(GL_TEXTURE_2D);
         glDisable(GL_BLEND);
         glEnable(GL_LIGHTING);
      glPopMatrix();
   }

   waveTest->doStep();
   waveTest->draw();

   /* Draw Particles */
   glPushMatrix();
      particleSystem->actualizeAll(PCs->personagemAtivo->posicaoLadoX,
                                   PCs->personagemAtivo->posicaoLadoZ, 
                                   visibleMatrix);
   glPopMatrix();


   /* Draw the GUI and others */
   gluUnProject(SCREEN_X,SCREEN_Y, 0.01, modl, proj, viewPort, &x1, &y1, &z1);
   gluUnProject(SCREEN_X,SCREEN_Y-80,0.01, modl, proj, viewPort, &x2, &y2, &z2);
   gluUnProject(SCREEN_X-60,SCREEN_Y-80,0.01,modl,proj,viewPort, &x3, &y3, &z3);
   gluUnProject(SCREEN_X-60,SCREEN_Y,0.01, modl, proj, viewPort, &x4, &y4, &z4);

   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);

   /* Player's Portrait */
   per = (personagem*) PCs->personagemAtivo;
   per->DrawMainPortrait(x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4);

   gui->draw(proj,modl,viewPort);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   
   /* mouse Cursor Draw */
   GLfloat Z = 0.01;

   GLuint Y = SCREEN_Y - mouseY;

   /*if(mouseY > SCREEN_Y - 32)
   {
      Y = 32;
   }
   else
   {
      Y = SCREEN_Y - mouseY;
   }*/

   gluUnProject(mouseX,Y, Z, modl, proj, viewPort, &x1, &y1, &z1);
   gluUnProject(mouseX,Y-32,Z, modl, proj, viewPort, &x2, &y2, &z2);
   gluUnProject(mouseX+32,Y-32,Z,modl,proj,viewPort, &x3, &y3, &z3);
   gluUnProject(mouseX+32,Y,Z, modl, proj, viewPort, &x4, &y4, &z4);

   /*gluUnProject(0,SCREEN_Y, Z, modl, proj, viewPort, &x1, &y1, &z1);
   gluUnProject(0,SCREEN_Y-32,Z, modl, proj, viewPort, &x2, &y2, &z2);
   gluUnProject(32,SCREEN_Y-32,Z,modl,proj,viewPort, &x3, &y3, &z3);
   gluUnProject(32,SCREEN_Y,Z, modl, proj, viewPort, &x4, &y4, &z4);*/

   glPushMatrix();

   //FIXME BUG when mouse after screenY-32
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, cursors->actualCursor );
   glBegin(GL_QUADS);
      glColor4f(1,1,1,1);
      glTexCoord2f(0,0);
      glVertex3f(x1,y1,z1);
      glTexCoord2f(0,1);
      glVertex3f(x2,y2,z2);
      glTexCoord2f(1,1);
      glVertex3f(x3,y3,z3);
      glTexCoord2f(1,0);
      glVertex3f(x4,y4,z4);
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);

   glPopMatrix();

   //glDisable(GL_ALPHA_TEST);

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
      dist = sqrt( (PCs->personagemAtivo->posicaoLadoX + varX - moveCircleX) *
                   (PCs->personagemAtivo->posicaoLadoX + varX - moveCircleX) +
                   (PCs->personagemAtivo->posicaoLadoZ + varZ - moveCircleZ) *
                   (PCs->personagemAtivo->posicaoLadoZ + varZ - moveCircleZ) );
      if( ( (canAttack) && (dist > 2*WALK_PER_MOVE_ACTION)) || 
            (!canAttack) && (dist > WALK_PER_MOVE_ACTION))
      {
         return(false);
      }
   }


   colisionDetect.defineMap(actualMap);
   result = colisionDetect.canWalk(PCs->personagemAtivo->posicaoLadoX + varX,
                          PCs->personagemAtivo->posicaoLadoY,
                          PCs->personagemAtivo->posicaoLadoZ + varZ,
                          PCs->personagemAtivo->min[0],
                          PCs->personagemAtivo->min[1],
                          PCs->personagemAtivo->min[2],
                          PCs->personagemAtivo->max[0],
                          PCs->personagemAtivo->max[1],
                          PCs->personagemAtivo->max[2],
                          PCs->personagemAtivo->orientacao + varAlpha, 
                          PCs->personagemAtivo->ocupaQuad, NPCs, varHeight,
                          nx, nz);

   if(result)
   {
      GLfloat altura_atual = PCs->personagemAtivo->posicaoLadoY;

      int posX =(int)floor( nx / (SQUARESIZE));
   
      int posZ =(int)floor( nz / (SQUARESIZE));

      PCs->personagemAtivo->ocupaQuad = actualMap->quadradoRelativo(posX,posZ);

      Square* saux = actualMap->quadradoRelativo( (int)(nx/SQUARESIZE),
                                          (int)(nz/SQUARESIZE));

      GLfloat dx1 = fabs(nx - saux->x1) / SQUARESIZE;
      GLfloat dz1 = fabs(nz - saux->z1) / SQUARESIZE;
      GLfloat dx2 = fabs(nx - saux->x2) / SQUARESIZE;
      GLfloat dz2 = fabs(nz - saux->z2) / SQUARESIZE;

      GLfloat ha = (dx2 * PCs->personagemAtivo->ocupaQuad->h1) + 
                   (dx1 * PCs->personagemAtivo->ocupaQuad->h4);
      GLfloat hb = (dx2 * PCs->personagemAtivo->ocupaQuad->h2) + 
                   (dx1 * PCs->personagemAtivo->ocupaQuad->h3);

      GLfloat res = (ha * dz2) + (hb * dz1);

      if( res - altura_atual > ANDAR)
      {
          PCs->personagemAtivo->posicaoLadoY = altura_atual;
          return(false);
      }
 
      PCs->personagemAtivo->posicaoLadoY = res + varHeight;
      gameCamera.centerY = res+30;

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
 *                       Load MiniMap Window                         *
 *********************************************************************/
void engine::OpenMiniMapWindow()
{
   GLint x = (int)(((PCs->personagemAtivo->posicaoLadoX) / (SQUARESIZE)));
   if(x > actualMap->x-1)
   {
      x = actualMap->x-1;
   }
   GLint z = (int)(((PCs->personagemAtivo->posicaoLadoZ) / (SQUARESIZE)));
   if( z > actualMap->z-1)
   {
      z = actualMap->z-1;
   }
   x = 8 + (x*3);
   z = 20 + (z*3);
   miniMapWindow = gui->ljan->InserirJanela(512,472,799,599,//0,344,255,471,
                                          language.WINDOW_MAP.c_str(),1,1,
                                          NULL,NULL);

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
   miniMapWindow->Abrir(gui->ljan);
}

/*********************************************************************
 *                         Load ShortCuts Window                     *
 *********************************************************************/
void engine::OpenShortcutsWindow()
{
   shortCutsWindow=gui->ljan->InserirJanela(0,472,511,599,
                                  language.WINDOW_SHORTCUTS.c_str(),1,1,
                                       NULL,NULL);
   FPS = shortCutsWindow->objects->InserirQuadroTexto(8,20,150/*100*/,35,2,
                                  language.WINDOW_SHORTCUTS_FPS.c_str());
   briefTxt = shortCutsWindow->objects->InserirQuadroTexto(8,36,249,100,2,
                                  language.WINDOW_SHORTCUTS_HELP.c_str());
   briefTxt->fonte = FMINI;
   /*ObjTxt->Cores.corCont[1].R = 0; ObjTxt->Cores.corCont[1].G = 25; 
   ObjTxt->Cores.corCont[1].B = 255;*/
   ObjTxt = shortCutsWindow->objects->InserirQuadroTexto(151,20,249,35,2,
                                 language.OBJ_NOTHING.c_str());

   shortCutsWindow->objects->InserirBotao(8,102,76,120,shortCutsWindow->Cores.corBot.R, 
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
   shortCutsWindow->objects->InserirBotao(141,102,209,120,
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

   tb->insertButton(99,4,135,36);/* Inventory */
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
   shortCutsWindow->Abrir(gui->ljan);
}

/*********************************************************************
 *                    Actualize Actual Health Bars                   *
 *********************************************************************/
void engine::actualizeAllHealthBars()
{
   personagem* pers = (personagem*) PCs->primeiro->proximo;
   while(pers != PCs->primeiro)
   {
      pers->DefineActualLifePoints(pers->lifePoints);
      pers = (personagem*) pers->proximo;
   }
}


void engine::gameOver()
{
   GLuint id;
   SDL_Surface* img = IMG_Load("../data/texturas/fightMode/death.png"); 
   /*cor_Definir(245,25,25);
   selFonte(FPLATINA, CENTRALIZADO, 3);
   escxy(img,256,426,"All Playable Characters Died.");*/
   carregaTextura(img,&id);
   SDL_FreeSurface(img);
   AtualizaTela2D(id,proj,modl,viewPort,0,0,799,599,0.012);
   glFlush();
   SDL_GL_SwapBuffers();
   SDL_Delay(4000);
   glDeleteTextures(1,&id);
}

/*********************************************************************
 *                          Runs the Engine                          *
 *********************************************************************/
int engine::Run(SDL_Surface *surface)
{
   string brief;
   
   if(!actualMap->music.empty())
   {
      snd->StopMusic(music);
      music = snd->LoadMusic(actualMap->music);
   }

   snd->LoadSample(SOUND_WALK,"../data/sndfx/passos.ogg");

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

     /* Verify battle events */
     if(engineMode == ENGINE_MODE_TURN_BATTLE) 
     {
        time = SDL_GetTicks();
        if(fightStatus == FIGHT_END)
        {
           engineMode = ENGINE_MODE_REAL_TIME;
           if(shortCutsWindow)
           {
              briefTxt->texto += "|" + language.FIGHT_EXIT;
           }
           /* Verify if any PC is alive. */
           personagem* pers = (personagem*) PCs->primeiro->proximo;
           bool alive = false;
           while((!alive) && (pers != PCs->primeiro))
           {
              if(!pers->dead)
              {
                 alive = true;
              }
              pers = (personagem*) pers->proximo;
           }
           if(!alive)
           {
              //TODO call the game over screen
              gameOver();
              return(0);
           }
        }
        //FIXME define max time by animations "called". call animations. jeje
        else if( (fightStatus == FIGHT_CONTINUE) &&
                 ((time - lastTurnTime) > 4000) ) 
        {
           lastTurnTime = time;
           fightStatus = fight.doBattleCicle(brief);
           if(shortCutsWindow)
           {
              briefTxt->texto = brief;
           }
           actualizeAllHealthBars();

           if(fightStatus == FIGHT_PC_TURN)
           {
               if(fight.actualCharacterTurn()) 
               {
                  PCs->personagemAtivo = fight.actualCharacterTurn();
                  fullMovePCAction = false;
                  canMove = true;
                  //TODO Verify if weapon is ranged before do this
                  attackFeat = FEAT_MELEE_ATTACK;
                  canAttack = true;

                  moveCircleX = PCs->personagemAtivo->posicaoLadoX;
                  moveCircleY = PCs->personagemAtivo->posicaoLadoY;
                  moveCircleZ = PCs->personagemAtivo->posicaoLadoZ;
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
                 PCs->personagemAtivo->ID = createchar( &clientData, 
                   PCs->personagemAtivo->posicaoLadoX, 
                   PCs->personagemAtivo->posicaoLadoZ, 
                   PCs->personagemAtivo->orientacao );
                 if( PCs->personagemAtivo->ID == -2 )
                 {
                     entergame( &clientData );
                 }
                 else if ( PCs->personagemAtivo->ID == -1 )
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


