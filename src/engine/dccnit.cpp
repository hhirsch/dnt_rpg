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

#define REFRESH_RATE 100


/*********************************************************************
 *                       Engine's Constructor                        *
 *********************************************************************/
engine::engine()
{
   /* Initialize internal lists */
   gui  = new interface(NULL);
   actualMap = NULL;
   PCs = NULL;
   NPCs = NULL;
   miniMapWindow = NULL;
   shortCutsWindow = NULL;
   imgNumber = 0;
   /* Define Camera initial Position */
   theta=25;
   phi=0;
   d=150;
   centerX = centerZ = 0;
   centerY = 30;
   deltaY = 0;
   cameraX = centerX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centerY + (float) d * sin(deg2Rad(theta));
   cameraZ = centerZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));

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

   /* Initialize readModes variables */
   lastRead = SDL_GetTicks();
   lastMouse = lastRead;
   lastKeyb = lastRead;
   mouseX = 0;
   mouseY = 0;

   particleSystem = new partSystem();

   engineMode = ENGINE_MODE_REAL_TIME;

   /*part1* particula;

   particula = (part1*) particleSystem->addParticle(PART_WATERFALL,
                                                    150,60,120,
                                            "../data/particles/waterfall1.par");
   particula->addPlane(148,59,118,152,59,123,-1,0,PLANE_NO_INCLINATION);
   particula->addPlane(150,40,118,160,32,123,-1,0,PLANE_INCLINATION_X);
   particula->addPlane(160,20,110,175,20,130,-1,0,PLANE_NO_INCLINATION);

   particleSystem->addParticle(PART_FIRE,200,0,220,
                                          "../data/particles/fire1.par");

   particleSystem->addParticle(PART_FIRE,200,0,350,
                                          "../data/particles/fire2.par");
   particleSystem->addParticle(PART_WATER_SURFACE,300,20,300,"");
   particleSystem->addParticle(PART_SMOKE,240,0,340,
                                          "../data/particles/smoke1.par");
   particleSystem->addParticle(PART_SNOW,340,80,100,
                                                 "../data/particles/snow1.par");
   */
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

   /* Clear Characters */
   if(NPCs)
      delete(NPCs);
   if(PCs)
      delete(PCs);

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
         float posX, posZ;
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
       /*atualizaCarga(img,&texturaTexto,texturaCarga,
                 "Loading Character: Gushm",
                 proj, modl, viewPort);
       PCs->InserirPersonagem(7,6,9,7,"../data/pics/logan/portrait.jpg",
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
   centerX = actualMap->xInic;
   centerZ = actualMap->zInic;
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
      if(tempo - tempoAnterior >= 20) 
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
      else if(19 - (tempo - tempoAnterior) > 0 ) 
      {
         SDL_Delay(19 - (tempo - tempoAnterior) );
      }
   }

   snd->ChangeVolume(option->musicVolume, option->sndfxVolume);

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

  
   /*TODO Other screens*/

   /* Call Skill Screen */
   skills* sk = new skills(language.SKILLS_DIR.c_str(),
                           "../data/skills/skills.skl"); 
   skillWindow* skWindow = new skillWindow(sk, 20, gui);

   while( (charCreation != CHAR_CANCEL) && 
          (charCreation != CHAR_CONFIRM))
   {
      tempo = SDL_GetTicks();
      if(tempo - tempoAnterior >= 20) 
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
         charCreation = skWindow->treat(object, eventInfo, gui);
      }
      else if(19 - (tempo - tempoAnterior) > 0 ) 
      {
         SDL_Delay(19 - (tempo - tempoAnterior) );
      }
   }
   delete(sk);
   delete(skWindow);
   return(charCreation);
}


/*********************************************************************
 *                  redmensionateWindowte Engine to Screen                   *
 *********************************************************************/
void engine::redmensionateWindow(SDL_Surface *screen)
{
   glViewport (0, 0, (GLsizei) screen->w, (GLsizei) screen->h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, (GLsizei)screen->w / (GLsizei)screen->h, 1.0, FARVIEW);
//   glFrustum(-1.0, 1.0, 1.0, 1.0, -1.0, FARVIEW);
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
   double theta1,theta2,theta3;
   double ex,ey,ez;
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
         glTexCoord2f(i/(double)n,2*(j+1)/(double)n);
         glVertex3f(ex,ey,ez);

         ex = cos(theta1) * cos(theta3);
         ey = sin(theta1);
         ez = cos(theta1) * sin(theta3);

         glNormal3f(ex,ey,ez);
         glTexCoord2f(i/(double)n,2*j/(double)n);
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
#if 0
   GLfloat light_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
   GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_position[] = { 1.0, 1.0, 0.0, 1.0 };
   GLfloat light_position2[] = {240.0,30.0,25.0,1.0};
   GLfloat light_direction[] = {1.0,-1.0,0.0};
   
   /* Carrega a Luz */
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   /* Luz1 */
   glLightfv(GL_LIGHT1, GL_AMBIENT, light_specular);
   glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
   glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction);
   glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
   glLightf (GL_LIGHT1, GL_SPOT_EXPONENT, 2.5);
#endif

   GLfloat light_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

   glEnable(GL_LIGHTING);

   //glEnable(GL_LIGHT0);
   //glDisable(GL_LIGHT0);
   //glEnable(GL_LIGHT1);
  
   /* Sky Creation */
   drawSphereToList(10,10);

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
                 briefTxt->texto = "";
                 fight.empty();
                 if(!NPCs)
                 {
                    briefTxt->texto = language.FIGHT_NO_NPCS;
                    return;
                 }
                 ch =(personagem*) NPCs->primeiro->proximo;
                 while(ch != NPCs->primeiro)
                 {
                    //TODO put enemies on groups, when enemy from enemy
                    fight.insertNPC(ch, 0, briefInit);
                    briefTxt->texto += briefInit + "|";
                    numEnemies++;
                    ch->actualFeats.defineMeleeWeapon(dc); //FIXME
                    ch = (personagem*) ch->proximo; 
                    SDL_Delay(1);
                 }
                 
                 if(numEnemies > 0)
                 {
                    engineMode = ENGINE_MODE_TURN_BATTLE;
                    moveCircleX = PCs->personagemAtivo->posicaoLadoX;
                    moveCircleY = PCs->personagemAtivo->posicaoLadoY;
                    moveCircleZ = PCs->personagemAtivo->posicaoLadoZ;

                    /* Put the PCs on group */
                    ch =(personagem*) PCs->primeiro->proximo;
                    while(ch != PCs->primeiro)
                    {
                       fight.insertPC(ch, 0, briefInit);
                       briefTxt->texto += briefInit + "|";
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

                    briefTxt->texto += language.FIGHT_SURPRISE_TURN;
                 }
                 else
                 {
                    briefTxt->texto = language.FIGHT_NO_NPCS;
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
                  (fightStatus == FIGHT_PC_TURN) )
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
   double varX, varZ;        // to avoid double calculate

   double passo;     // How much the character walks, based on time
   double rotacao;   // How much the character turns, based on time
   double varCamera; // Camera Variation
   double varTempo;  // Time Variation
   float wx,wy,wz;

   tempo = SDL_GetTicks();
   srand(tempo);
   varTempo = (tempo-lastRead);
   if( ((varTempo)) >= 20)
   {
      redesenha = true;
      passouTempo = true;

      /* Actualize Characters Animations */
      seconds = varTempo / 1000.0;
      int aux;
      personagem *per = (personagem*) PCs->primeiro->proximo;
      for(aux=0;aux< PCs->total;aux++)
      {
         per->m_calModel->update(seconds); 
         //per->CalculateBoundingBox(); 
         per = (personagem*) per->proximo;
      }
    
      if(NPCs)
      {
        per = (personagem*) NPCs->primeiro->proximo;
        for(aux=0;aux < NPCs->total;aux++)
        {
           per->m_calModel->update(seconds);   
           per->CalculateBoundingBox();
           per = (personagem*) per->proximo;
        } 
      }

      /* Calculate the real Modification on walk, rotate, turn, etc */
      double vt = varTempo / 20.0;
      passo = (vt)*ANDAR;
      if(passo > 9)
        passo = 9;  /* To avoid phantom efects when LAGs occurs */
      rotacao = (vt)*GIRAR;
      varCamera = vt*DELTACAMERA;
      
      actualFPS = (actualFPS + (1000.0 / varTempo)) / 2; 
     
      SDL_PumpEvents();
      if( (shortCutsWindow) && (tempo-lastFPS >= 500))
      {
         lastFPS = tempo;
         char texto[15];
         sprintf(texto,"FPS: %3.2f",1000.0 / (tempo-lastRead));
         FPS->texto = texto;
         sprintf(texto," Part: %d",particleSystem->numParticles());
         FPS->texto += texto;
         shortCutsWindow->Desenhar(mouseX, mouseY);
      }
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
      if( (tempo-lastMouse>=100 ) || 
          ( (Mbotao & SDL_BUTTON(1)) && (tempo-lastMousePression>=100)) )
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

         /* TODO Talk Events Verification */
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
                    cursors->SetActual(CURSOR_TALK);
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
                     cursors->SetActual(CURSOR_ATTACK);
                     if(shortCutsWindow)
                     {
                        ObjTxt->texto = pers->nome; 
                        shortCutsWindow->Desenhar(mouseX, mouseY);
                     }
                     if(Mbotao & SDL_BUTTON(1))
                     {
                        briefTxt->texto = "";
                        canAttack = !PCs->personagemAtivo->actualFeats.
                                                        applyAttackAndBreakFeat(
                                                          *PCs->personagemAtivo,
                                                          attackFeat, *pers, 
                                                          briefTxt->texto);
                        if( pers->psychoState != PSYCHO_HOSTILE)
                        {
                            pers->psychoState = PSYCHO_HOSTILE;
                        }

                     }
                     pronto = 1;
                 }
              }
              pers = (personagem*) pers->proximo;
            }
         }

         /* Map Connections Verification */
         if( ( quaux->mapConection.active ) && (!pronto) )
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
         if(keys[SDLK_p])
         {
            part5 *p;
            p =  (part5*)particleSystem->addParticle(PART_BLOOD,
                                    PCs->personagemAtivo->posicaoLadoX,28,
                                    PCs->personagemAtivo->posicaoLadoZ, 
                                    "../data/particles/blood1.par");
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

      }

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

      /* Keys to Camera Moviments */
      if(keys[SDLK_UP])  // Increases Zoom
      {
          if (d>ZOOMMAXIMO)
          {
             d-= varCamera;
             redesenha = true;
          }
      }
      if(keys[SDLK_DOWN]) // Decreases Zoom
      {
         if(d<ZOOMMINIMO)
         {
             d+= varCamera; 
             redesenha = true;
         }
      }
      if(keys[SDLK_RIGHT]) // Rotate Camera CounterClockWise
      {
          phi -= varCamera;  
          redesenha = true;
      }
      if(keys[SDLK_LEFT]) // Rotate Camera ClockWise
      {
         phi += varCamera;
         redesenha = true;
      }
      if(keys[SDLK_PAGEUP]) // Maximize Up Camera
      {
            theta += varCamera;
            redesenha = true;
            if(theta > 89) 
               theta = 89;
      }
      if(keys[SDLK_PAGEDOWN]) // Minimize Up Camera
      {
         theta -= varCamera;
         redesenha = true;
         if(theta < 0)
            theta = 0;
      }
      if (keys[SDLK_HOME]) // Maximize zoom
      {
         d = ZOOMMAXIMO;
         redesenha = true;
      }
      if(keys[SDLK_END]) // Minimize zoom
      {
         d = ZOOMMINIMO;
         redesenha = true;
      }   
      if(keys[SDLK_INSERT]) //Up view Max
      {
         theta = 89;
         redesenha = true;
      }
      if(keys[SDLK_DELETE]) //Down view Max
      {
         theta = 0;
         redesenha = true;
      }

      /* Keys to character's movimentation */
      if(keys[SDLK_q] || keys[SDLK_e])
      {
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
            centerX += varX;
            centerZ += varZ;
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
            centerX += passo;
            PCs->personagemAtivo->posicaoLadoX += passo;
            redesenha = true;
            andou = true;
         }
         else if( ((varZ > 0) && canWalk(0,passo,0)) ||
                  ((varZ < 0) && canWalk(0,-passo,0)) )
         {
            if(varZ < 0) 
               passo *= -1;
            centerZ += passo;
            PCs->personagemAtivo->posicaoLadoZ += passo;
            redesenha = true;
            andou = true;
         }
        
      }
      else if(keys[SDLK_w] || keys[SDLK_s])
      { 
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
             centerX += varX;
             centerZ += varZ;
             redesenha = true;
             andou  = true;
         }
         else if( ((varX > 0) && (canWalk(passo,0,0))) ||
                  ((varX < 0) && (canWalk(-passo,0,0))) ) 
                   
         {
              if(varX < 0 )
                 passo *= -1;
              PCs->personagemAtivo->posicaoLadoX += passo;
              centerX += passo;
              redesenha = true;
              andou = true;
         }
         else if( ((varZ > 0) && (canWalk(0,passo,0))) ||
                  ((varZ < 0) && (canWalk(0,-passo,0))) )
         {
              if( varZ < 0 )
                 passo *= -1;
              PCs->personagemAtivo->posicaoLadoZ += passo;
              centerZ += passo;
              redesenha = true;
              andou = true;
         }

      }

      if( (keys[SDLK_a]) || (keys[SDLK_d]))
      {
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
         centerX = PCs->personagemAtivo->posicaoLadoX;
         centerZ = PCs->personagemAtivo->posicaoLadoZ;
         redesenha = true;
         SDL_Delay(100);
      }
      
      /* Mouse Verification */

      /* Mouse to move Camera */
      if(x == 0)  // Turn Clockwise
      {
         phi+=2; 
         redesenha = true;  
      }
      if(x == screen->w-1) // Turn CounterClockWise
      {
        phi-=2; 
        redesenha = true;
      }
   }
   }
   else if(*forcaAtualizacao == 0)
   {
      int tmp = (int) (19 - varTempo);
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
#ifdef VIDEO_MODE
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

   glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   glLoadIdentity();

   /* Redefine camera position */
   cameraX = centerX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centerY + deltaY + (float) d * sin(deg2Rad(theta));
   cameraZ = centerZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
   gluLookAt(cameraX,cameraY,cameraZ, centerX,centerY,centerZ,0,1,0);

   /* Atualize to culling and to GUI */
   AtualizaFrustum(visibleMatrix,proj,modl);

   /* SKY */
   glPushMatrix();
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, sky);
      glTranslatef(actualMap->x*HALFSQUARESIZE, 0 , actualMap->z*HALFSQUARESIZE);
      glScalef(HALFFARVIEW,HALFFARVIEW,HALFFARVIEW);
      glRotated(90,0,1,0);
      glRotated(180,1,0,0);
      //gluSphere(atmosfera,HALFFARVIEW,5,5);
      glCallList(skyList);
      glDisable(GL_TEXTURE_2D);
   glPopMatrix();

   glPushMatrix();

   /* Draws World, doing view frustum culling */
   actualMap->draw(cameraX,cameraY,cameraZ,visibleMatrix);

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
         per = (personagem*) per->proximo;
      }
   }

   /* Draw Combat Mode Things */
   if( (engineMode == ENGINE_MODE_TURN_BATTLE) && 
       (fightStatus == FIGHT_PC_TURN))
   {
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

      glDisable(GL_BLEND);
   }

   /* Draw Particles */
   glPushMatrix();
      particleSystem->actualizeAll(PCs->personagemAtivo->posicaoLadoX,
                                   PCs->personagemAtivo->posicaoLadoZ);
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
      /*glNormal3f( 0, 0, 1 );
      glTexCoord2f(0,0);
      glVertex2f(mouseX,Y);
      glTexCoord2f(0,1);
      glVertex2f(mouseX,Y-32);
      glTexCoord2f(1,1);
      glVertex2f(mouseX+32,Y-32);
      glTexCoord2f(1,0);
      glVertex2f(mouseX+32,Y);*/
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);

   glPopMatrix();

   //glDisable(GL_ALPHA_TEST);

   glEnable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
 
   glFlush();
}


/******************************************/
/*    PARTE DA DETECCAO DE COLISAO!!!!!!! */
/******************************************/

/*********************************************************************
 *          Faz o teste se o Quadrado quad é factivel de ser         *
 *                    ocupado pelo personagem                        *
 *********************************************************************/
int testa(GLfloat min[3], GLfloat max[3],Square* quad)
{
   int result = 0;
   GLfloat min2[3];
   GLfloat max2[3];

   Square* proxima = quad;
   if(proxima->flags & PISAVEL)
   {
     result = 1;
   }
   if(result) // Se possivel entrar, testa com Muros
   {
      int mur = 0;
      while((mur < MAXMUROS ) && (proxima->muros[mur] != NULL))
      {
         min2[0] = proxima->muros[mur]->x1; 
         min2[1] = 0; 
         min2[2] = proxima->muros[mur]->z1;
         max2[0] = proxima->muros[mur]->x2; 
         max2[1] = MUROALTURA; 
         max2[2] = proxima->muros[mur]->z2;
         result &= !estaDentro(min,max,min2,max2,1);
         if(!result)
           return(0);
         mur++;
      }
   }
   if(result) // Se eh possivel entrar, testa com os objects
   {
      int ob = 0;
      //GLfloat u1,u2,v1,v2;
      GLMmodel* modelo3d;
      GLfloat X[4], Z[4];
      while( (proxima->objects[ob] != NULL)) 
      {
        if(!proxima->pisavelObj[ob])
        {
          modelo3d = (GLMmodel*)proxima->objects[ob]->modelo3d;
          X[0] = modelo3d->x1;
          Z[0] = modelo3d->z1;
          X[1] = modelo3d->x1;
          Z[1] = modelo3d->z2;
          X[2] = modelo3d->x2;
          Z[2] = modelo3d->z2;
          X[3] = modelo3d->x2;
          Z[3] = modelo3d->z1;
/* TODO +Yobjects */
          rotTransBoundingBox(proxima->objectsOrientation[ob], X, Z,
                              proxima->Xobjects[ob], modelo3d->y1, 
                              modelo3d->y2,proxima->Zobjects[ob], 
                              min2, max2);

          result &= !estaDentro(min,max,min2,max2,1);
          if(!result) //se ja achou que nao pode, cai fora
             return(0);
        }
        ob++;
      }
   }

   return(result);
}

/*********************************************************************
 *                   Verifica Colisao com MeioFio                    *
 *********************************************************************/
int ColisaoComMeioFio(GLfloat min[3],GLfloat max[3], muro* meiosFio)
{
    GLfloat min2[3];
    GLfloat max2[3];
    muro* maux = meiosFio;
    while(maux)
    {
       min2[0] = maux->x1;
       max2[0] = maux->x2;
       min2[1] = 0;
       max2[1] = MEIOFIOALTURA;
       min2[2] = maux->z1;
       max2[2] = maux->z2;
       if( (estaDentro(min, max, min2, max2, 1)) )
          return(1);
       maux = maux->proximo;
    }
    return(0);
}

/*********************************************************************
 *                   Verifica se nao ha Colisao                      *
 *********************************************************************/
int engine::canWalk(GLfloat varX, GLfloat varZ, GLfloat varAlpha)
{
   int result = 1;
   Square* saux;
   GLfloat dist = 0;

   GLfloat min[3],min2[3];
   GLfloat max[3],max2[3];

   if((engineMode == ENGINE_MODE_TURN_BATTLE) && 
       (fightStatus != FIGHT_PC_TURN))
   {
       /* In turn mode, and not character's turn. */
       return(0);
   }
   else
   if((engineMode == ENGINE_MODE_TURN_BATTLE) && 
       (fightStatus == FIGHT_PC_TURN))
   {
      if(!canMove)
      {
         /* Already Moved */
         return(0);
      }
      //verify distance to the orign point
      dist = sqrt( (PCs->personagemAtivo->posicaoLadoX + varX - moveCircleX) *
                   (PCs->personagemAtivo->posicaoLadoX + varX - moveCircleX) +
                   (PCs->personagemAtivo->posicaoLadoZ + varZ - moveCircleZ) *
                   (PCs->personagemAtivo->posicaoLadoZ + varZ - moveCircleZ) );
      if(dist > 2*WALK_PER_MOVE_ACTION)
      {
         return(0);
      }
   }


   GLfloat x[4],z[4];

   x[0] = PCs->personagemAtivo->min[0];
   z[0] = PCs->personagemAtivo->min[2];

   x[1] = PCs->personagemAtivo->min[0];
   z[1] = PCs->personagemAtivo->max[2]; 

   x[2] = PCs->personagemAtivo->max[0];
   z[2] = PCs->personagemAtivo->max[2];

   x[3] = PCs->personagemAtivo->max[0];
   z[3] = PCs->personagemAtivo->min[2];

   /* Rotaciona e translada o Bounding Box */
   rotTransBoundingBox(PCs->personagemAtivo->orientacao+varAlpha, x, z,
                       PCs->personagemAtivo->posicaoLadoX+varX, 
                       PCs->personagemAtivo->min[1] + 
                         PCs->personagemAtivo->posicaoLadoY, 
                       PCs->personagemAtivo->max[1] + 
                         PCs->personagemAtivo->posicaoLadoY,
                       PCs->personagemAtivo->posicaoLadoZ+varZ,
                       min, max );

   /* Testa limites do Mapa */
   if( (min[0]<2) || (min[2]<2) || 
       (max[0]>actualMap->x*SQUARESIZE-2) || (max[2]>actualMap->z*SQUARESIZE-2))
   {
      return(0);
   }

   /* Testa Portas */
   door* porta = actualMap->portas;
   while( porta != NULL )
   {
      GLfloat minObj[3], maxObj[3];
      GLMmodel* modeloPorta = (GLMmodel*) porta->object->modelo3d;
      GLfloat XA[4]; GLfloat ZA[4];
      XA[0] = modeloPorta->x1;
      ZA[0] = modeloPorta->z1;

      XA[1] = modeloPorta->x1;
      ZA[1] = modeloPorta->z2; 

      XA[2] = modeloPorta->x2;
      ZA[2] = modeloPorta->z2;

      XA[3] = modeloPorta->x2;
      ZA[3] = modeloPorta->z1;
      rotTransBoundingBox(porta->orientacao, XA, ZA,
                          porta->x, 0.0,0.0,porta->z, 
                          minObj, maxObj);
      if(estaDentro( min, max, minObj, maxObj, 1))
      {
         return(0);
      }
      porta = porta->proximo;
   }

   /* Testa o Atual, ja que eh GRANDE! */
   min2[0] = PCs->personagemAtivo->ocupaQuad->x1;
   min2[1] = 0;
   min2[2] = PCs->personagemAtivo->ocupaQuad->z1;
   max2[0] = PCs->personagemAtivo->ocupaQuad->x2;
   max2[1] = 400;
   max2[2] = PCs->personagemAtivo->ocupaQuad->z2;
   if(estaDentro(min,max,min2,max2,1))
   {
      result &= testa(min,max,PCs->personagemAtivo->ocupaQuad);
      if(!result)
      {
         //printf("sai na atual\n"); 
         return(0);
      }
   }

 
   /* Testa quadrados a direita */
   saux = actualMap->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX+1,
                             PCs->personagemAtivo->ocupaQuad->posZ);
   if(saux) 
   { 
      /* leste */
      min2[0] = saux->x1;
      min2[2] = saux->z1;
      max2[0] = saux->x2;
      max2[2] = saux->z2;
      if(estaDentro(min,max,min2,max2,1) )
      {
         result &= testa(min,max,saux);
         if(!result)
         {
            //printf("sai na direita\n"); 
            return(0);
         }
      }
      /* Nordeste */
      saux = actualMap->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX+1,
                             PCs->personagemAtivo->ocupaQuad->posZ-1);
      if( saux )
      {
         min2[0] = saux->x1;
         min2[2] = saux->z1;
         max2[0] = saux->x2;
         max2[2] = saux->z2;
         if(estaDentro(min,max,min2,max2,1) )
         {
            result &= testa(min,max,saux);
            if(!result) 
            {
               //printf("sai na direita->cima\n"); 
               return(0);
            }
         }
      }
      /* Sudeste */
      saux = actualMap->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX+1,
                             PCs->personagemAtivo->ocupaQuad->posZ+1);
      if( saux )
      {
         min2[0] = saux->x1;
         min2[2] = saux->z1;
         max2[0] = saux->x2;
         max2[2] = saux->z2;
         if(estaDentro(min,max,min2,max2,1))
         {
            result &= testa(min,max,saux);
            if(!result) 
            {
               //printf("sai na direita->baixo\n"); 
               return(0);
            }
         }
      }
   }

   /* Testa quadrados a esquerda */
   saux = actualMap->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX-1,
                             PCs->personagemAtivo->ocupaQuad->posZ);
   if( saux ) 
   { 
      /* oeste */
      min2[0] = saux->x1;
      min2[2] = saux->z1;
      max2[0] = saux->x2;
      max2[2] = saux->z2;
      if(estaDentro(min,max,min2,max2,1))
      {
         result &= testa(min,max,saux);
         if(!result) 
         {
            //printf("sai na esquerda\n"); 
            return(0);
         }
      }

      /* Noroeste */
      saux = actualMap->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX-1,
                             PCs->personagemAtivo->ocupaQuad->posZ-1);
      if( saux )
      {
         min2[0] = saux->x1;
         min2[2] = saux->z1;
         max2[0] = saux->x2;
         max2[2] = saux->z2;
         if(estaDentro(min,max,min2,max2,1) )
         {
            result &= testa(min,max,saux);
            if(!result) 
            {
               //printf("sai na esquerda->cima\n"); 
               return(0);
            }
         }
      }
      /* Sudoeste */
      saux = actualMap->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX-1,
                             PCs->personagemAtivo->ocupaQuad->posZ+1);
      if( saux )
      { 
         min2[0] = saux->x1;
         min2[2] = saux->z1;
         max2[0] = saux->x2;
         max2[2] = saux->z2;
         if(estaDentro(min,max,min2,max2,1))
         {
            result &=testa(min,max,saux);
            if(!result) 
            {
                //printf("sai na esquerda->baixo\n"); 
               return(0);
            }
         }
      }
   }
  
   /* Testa quadrados abaixo */
   saux = actualMap->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX,
                             PCs->personagemAtivo->ocupaQuad->posZ+1);
   if( saux ) 
   {   
      min2[0] = saux->x1;
      min2[2] = saux->z1;
      max2[0] = saux->x2;
      max2[2] = saux->z2;
      if(estaDentro(min,max,min2,max2,1) )
      { 
         /* sul */
         result &= testa(min,max,saux);
         if(!result) 
         {
            //printf("sai no de baixo\n"); 
            return(0);
         }
      }
   }

   /* Testa quadrados acima */
   saux = actualMap->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX,
                             PCs->personagemAtivo->ocupaQuad->posZ-1);
   if( saux )
   {  
      min2[0] = saux->x1;
      min2[2] = saux->z1;
      max2[0] = saux->x2;
      max2[2] = saux->z2;
      if(estaDentro(min,max,min2,max2,1) )
      { 
         /* norte */
         result &= testa(min,max,saux);
         if(!result) 
         {
            //printf("sai no de cima\n"); 
            return(0);
         }
      }
   }

   /* Testa colisao com npcs */
   if(NPCs)
   {
      personagem* pers = (personagem*) NPCs->primeiro->proximo;
      while( (pers != NPCs->primeiro) )
      {
         x[0] = pers->min[0];
         z[0] = pers->min[2];

         x[1] = pers->min[0];
         z[1] = pers->max[2]; 

         x[2] = pers->max[0];
         z[2] = pers->max[2];
 
         x[3] = pers->max[0];
         z[3] = pers->min[2];

         /* Rotaciona e translada o Bounding Box */
         rotTransBoundingBox(pers->orientacao, x, z,
                          pers->posicaoLadoX, 
                          0.0, 0.0, 
                          pers->posicaoLadoZ, min2, max2 );

         if(estaDentro( min, max, min2, max2, 1))
         {
            return(0);
         }
    
         pers = (personagem*) pers->proximo;
      }
   }

      
   /* Testa Meio-fio */
   float altura_atual = PCs->personagemAtivo->posicaoLadoY;
   if( ColisaoComMeioFio( min, max, actualMap->meiosFio) )
   {
      PCs->personagemAtivo->posicaoLadoY = MEIOFIOALTURA+0.1;
   }
   else
   {
      PCs->personagemAtivo->posicaoLadoY = 0.0;
   }
  
   

   
   GLfloat nx = ((min[0] + max[0]) / 2);//(PCs->personagemAtivo->posicaoLadoX+varX);
   GLfloat nz = ((min[2] + max[2]) / 2);//(PCs->personagemAtivo->posicaoLadoZ+varZ);

   int posX =(int)floor( nx / (SQUARESIZE));

   int posZ =(int)floor( nz / (SQUARESIZE));

   PCs->personagemAtivo->ocupaQuad = actualMap->quadradoRelativo(posX,posZ);

   saux = actualMap->quadradoRelativo( (int)(nx/SQUARESIZE),
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
       return(0);
   }
 
   PCs->personagemAtivo->posicaoLadoY += res;
   centerY = res+30;

   if(result)
   {
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
 *                      Tratamento da IA dos NPCs                    *
 *********************************************************************/
/* Monta Conjunto de campos do Quadrado */
inline void engine::verificaQuad(Square* quad)
{
   if(quad)
   {
       if(quad->flags & PISAVEL)
       {
            if(quad->objects[0])
            {
               ia->campoInfluencia(quad->posX,quad->posZ,
                                   TIPOOBSTACULO,30);
            }
        }
        else
           ia->campoInfluencia(quad->posX,quad->posZ,
                               TIPOOBSTACULO,30);
   }
}

/* Monta conjunto de Campos da linha */
inline void engine::verificaLinha(Square* center)
{
   Square* saux;
   if(center)
   {
       verificaQuad(center);
       saux = actualMap->quadradoRelativo(center->posX-1,center->posZ);
       if(saux)
       {
          verificaQuad(saux);
          saux = actualMap->quadradoRelativo(center->posX-2,center->posZ);
          verificaQuad(saux);
       }
       saux = actualMap->quadradoRelativo(center->posX+1,center->posZ);
       if(saux)
       {
          verificaQuad(saux);
          saux = actualMap->quadradoRelativo(center->posX+2,center->posZ);
          verificaQuad(saux);
       }
   }   
}

/* Retorno !=0 se modificou posicao do personagem */
int engine::TrataIA()
{   

   int posX, posZ;     //Posicao Auxiliar
   double antX,antZ;
   Square* saux;
   personagem* per;    //Personagem Atual
   per = (personagem*) NPCs->primeiro->proximo;

   ia->iniciaListaCampos();

   /* Coloca o Unico PC existente */
   ia->campoInfluencia(PCs->personagemAtivo->posicaoLadoX,
                       PCs->personagemAtivo->posicaoLadoZ,
                       TIPOPC, 100);

    /* Em tese seria para todos os NPCs, mas como a perseguicao
       atual eh para so um NPC, entao roda so para ele.
    */

    /* Testa-se entao o campo de visao, montando os campos de influencia */
    if(per->ocupaQuad)
    {
       verificaLinha(per->ocupaQuad);
       saux = actualMap->quadradoRelativo(per->ocupaQuad->posX,
                                     per->ocupaQuad->posZ-1);
       if(saux)
       {
          verificaLinha(saux);
          verificaLinha(saux);
       }
       saux = actualMap->quadradoRelativo(per->ocupaQuad->posX,
                                     per->ocupaQuad->posZ+1);
       verificaLinha(saux);
    }
    else
      printf("What the HEll!! Square Map Out of bounds!\n");
   
    antX = per->posicaoLadoX;
    antZ = per->posicaoLadoZ;
    ia->destinoNpc(per);

    /* Define-se A posicao do Personagem NPC */  
    posX =(int)floor((per->posicaoLadoX) / (SQUARESIZE))+1;
    posZ =(int)floor((per->posicaoLadoZ) / (SQUARESIZE))+1;
    per->ocupaQuad = actualMap->quadradoRelativo(posX,posZ);

    return( (antX!=per->posicaoLadoX) || (antZ!=per->posicaoLadoZ));
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
   /*ObjTxt->Cores.corCont[1].R = 0; ObjTxt->Cores.corCont[1].G = 25; 
   ObjTxt->Cores.corCont[1].B = 255;*/
   ObjTxt = shortCutsWindow->objects->InserirQuadroTexto(151,20,249,35,2,
                                 language.OBJ_NOTHING.c_str());

   shortCutsWindow->objects->InserirBotao(25,102,93,120,shortCutsWindow->Cores.corBot.R, 
                                 shortCutsWindow->Cores.corBot.G,
                                 shortCutsWindow->Cores.corBot.B,
                                 language.INITIAL_SAVE.c_str(),
                                 0,NULL);
   buttonMenu = shortCutsWindow->objects->InserirBotao(94,102,162,120,
                                 shortCutsWindow->Cores.corBot.R, 
                                 shortCutsWindow->Cores.corBot.G,
                                 shortCutsWindow->Cores.corBot.B,
                                 "Menu",
                                 0,NULL);
   shortCutsWindow->objects->InserirBotao(163,102,231,120,
                                 shortCutsWindow->Cores.corBot.R, 
                                 shortCutsWindow->Cores.corBot.G,
                                 shortCutsWindow->Cores.corBot.B,
                                 language.INITIAL_LOAD.c_str(),
                                 0,NULL);

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

/*********************************************************************
 *                          Runs the Engine                          *
 *********************************************************************/
int engine::Run(SDL_Surface *surface)
{

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

   
   /* AI init FIXME not here, but in NPCs, etc */
   ia = new(AI); 
   
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
           briefTxt->texto += "|" + language.FIGHT_EXIT;
        }
        //FIXME define max time by animations "called". call animations. jeje
        else if( (fightStatus == FIGHT_CONTINUE) &&
                 ((time - lastTurnTime) > 4000) ) 
        {
           lastTurnTime = time;
           fightStatus = fight.doBattleCicle(briefTxt->texto);
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

   delete(ia);
   return(1);
}


