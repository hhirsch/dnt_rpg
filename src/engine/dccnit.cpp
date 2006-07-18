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


/********************************************************************
 *                      Engine's Variables                          *
 ********************************************************************/
int exitEngine; //if defined, exits engine.

/*********************************************************************
 *                       Engine's Constructor                        *
 *********************************************************************/
engine::engine()
{
   /* Initialize internal lists */
   gui  = new interface(NULL);
   mapa = NULL;
   PCs = NULL;
   NPCs = NULL;
   janMiniMapa = NULL;
   janAtalhos = NULL;
   imgNumber = 0;
   /* Define Camera initial Position */
   theta=25;
   phi=0;
   d=150;
   centroX = centroZ = 0;
   centroY = 30;
   deltaY = 0;
   cameraX = centroX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centroY + (float) d * sin(deg2Rad(theta));
   cameraZ = centroZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));

   /* Initialize the Cursor */
   cursors = new(cursor);

   /* Initialize sounds and musics */
   snd = new(sound);
   musica = NULL;

   /* Load Options */
   option = new options("dcc.opc");

   /* Load Language's files */
   language.ReloadFile(option->langNumber);

   /* Set sound and music volume, based on options */
   snd->ChangeVolume(option->musicVolume, option->sndfxVolume);
  
   /* FIXME internationalization here, please! */
   features = new featsList("../data/feats/Portugues/","../data/feats/feats.ftl");

   /* Initialize readModes variables */
   ultimaLeitura = SDL_GetTicks();
   ultimoMouse = ultimaLeitura;
   ultimaKeyb = ultimaLeitura;
   mouseX = 0;
   mouseY = 0;

   particleSystem = new partSystem();

   /* FIXME Initialize Particles, it will not be here!! */

   part1* particula;

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
}

/*********************************************************************
 *                         Engine Destructor                         *
 *********************************************************************/
engine::~engine()
{
   /* Stops and free music & sounds */
   if(musica)
   {
      snd->StopMusic(musica);
   }
   delete(snd);

   /* Delete particles */
   if(particleSystem != NULL)
      delete(particleSystem);

   /* Close option */
   delete(option);

   /* Clear Sky */
   glDeleteLists(listAtmosfera,1);
   glDeleteTextures(1, &ceu);

   /* Clear Characters */
   if(NPCs)
      delete(NPCs);
   if(PCs)
      delete(PCs);

   /* Clear GUI */
   if(gui)
      delete(gui);

   /* Clear Maps */
   if(mapa)
   {
      delete(mapa);
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
   AtualizaFrustum(matrizVisivel,proj,modl);
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
   ultimaLeitura = SDL_GetTicks();
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

   AtualizaFrustum(matrizVisivel,proj,modl);
   AtualizaTela2D(texturaCarga,proj,modl,viewPort,272,236,527,363,0.01);
   AtualizaTela2D(texturaTexto,proj,modl,viewPort,272,365,527,396,0.01);
   glFlush();
   SDL_GL_SwapBuffers();

   /* Loading Map */
   if(mapa) 
   {
     arqVelho = mapa->name;
     delete(mapa);
   }
   mapa = new(Map);
   mapa->name = arqVelho;
   mapa->open(arqMapa);

   /* Enable, if needed, the FOG */
   if(mapa->fog.enabled)
   {
      glEnable(GL_FOG);
      {
        glFogi(GL_FOG_MODE,GL_LINEAR);
        glFogfv(GL_FOG_COLOR,mapa->fog.color);
        glFogf(GL_FOG_DENSITY,mapa->fog.density);
        glHint(GL_FOG_HINT,GL_DONT_CARE);
        glFogf(GL_FOG_START,mapa->fog.start);
        glFogf(GL_FOG_END,mapa->fog.end);
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
   if(!mapa->npcFileName.empty())
   {
      FILE* arq;
      if(!(arq = fopen(mapa->npcFileName.c_str(),"r")))
      {
         printf("Ouch, can't load NPC's file: %s.\n",mapa->npcFileName.c_str());
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
           per = NPCs->InserirPersonagem("../data/pics/logan/portrait.jpg",
                                         nome,arquivo,features);
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
       per = PCs->InserirPersonagem("../data/pics/logan/portrait.jpg",
                              "Logan",
                       "../data/models/personagens/Logan/modelo.cfg",
                       features);
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

   if(janMiniMapa)
     janMiniMapa->Fechar(gui->ljan);
   if(janAtalhos)
     janAtalhos->Fechar(gui->ljan);
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

   glEnable(GL_LIGHTING);


   /* Put Active Party on Init Position */
   PCs->personagemAtivo->posicaoLadoX = mapa->xInic;
   PCs->personagemAtivo->posicaoLadoZ = mapa->zInic;
   centroX = mapa->xInic;
   centroZ = mapa->zInic;
   PCs->personagemAtivo->ocupaQuad = mapa->squareInic;

   atualizaCarga(img,&texturaTexto,texturaCarga,
                 language.LOAD_DONE.c_str(),
                 proj, modl, viewPort);

   /* Free Loading Textures */
   SDL_FreeSurface(img);
   glDeleteTextures(1,&texturaCarga);
   glDeleteTextures(1,&texturaTexto);

   /* Change Music, if needed */
   if(!mapa->music.empty())
   {
      snd->StopMusic(musica);
      musica = snd->LoadMusic(mapa->music);
   }

   return(1);
}

/*********************************************************************
 *                       Call Initial Game Menu                      *
 *********************************************************************/
int engine::InitialScreen(int Status, GLuint* idTextura, bool reloadMusic)
{
   /* Reload Music, if needed */
   if( (musica) && (reloadMusic) )
   {
     snd->StopMusic(musica);
   }
   if(reloadMusic)
      musica = snd->LoadMusic("../data/music/musica1.ogg");

   /* Executes Initial Screen */
   AtualizaFrustum(matrizVisivel,proj,modl);
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
         interf->ManipulaEventos(x,y,Mbotao,keys);
         AtualizaTela2D(*idTextura,proj,modl,viewPort,0,0,799,599,0.012);
         interf->Desenhar(proj,modl,viewPort);
         glFlush();
         SDL_GL_SwapBuffers();
      }
      else if(19 - (tempo - tempoAnterior) > 0 ) 
      {
         SDL_Delay(19 - (tempo - tempoAnterior) );
      }

      optionW = option->Treat(interf);
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
         gui->ManipulaEventos(x,y,Mbotao,keys);
         AtualizaTela2D(*idTextura,proj,modl,viewPort,0,0,799,599,0.012);
         gui->Desenhar(proj,modl,viewPort);
         glFlush();
         SDL_GL_SwapBuffers();
      }
      else if(19 - (tempo - tempoAnterior) > 0 ) 
      {
         SDL_Delay(19 - (tempo - tempoAnterior) );
      }

      charCreation = skWindow->treat(gui);
   }
   delete(sk);
   delete(skWindow);
   return(charCreation);
}


/*********************************************************************
 *                  Redmensionate Engine to Screen                   *
 *********************************************************************/
void engine::Redmensiona(SDL_Surface *screen)
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
   listAtmosfera = glGenLists(1);
   glNewList(listAtmosfera,GL_COMPILE);

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
  
   Redmensiona(screen);
   
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
   glGenTextures(1, &ceu);
   glBindTexture(GL_TEXTURE_2D, ceu);
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
 *                   Threat Input/Output Events                      *
 *********************************************************************/
int engine::TrataES(SDL_Surface *screen,int *forcaAtualizacao)
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
   varTempo = (tempo-ultimaLeitura);
   if( ((varTempo)) >= 20)
   {
      redesenha = true;
      passouTempo = true;

      /* Actualize Characters Animations */
      segundos = varTempo / 1000.0;
      int aux;
      personagem *per = (personagem*) PCs->primeiro->proximo;
      for(aux=0;aux< PCs->total;aux++)
      {
         per->m_calModel->update(segundos); 
         //per->CalculateBoundingBox(); 
         per = (personagem*) per->proximo;
      }
    
      if(NPCs)
      {
        per = (personagem*) NPCs->primeiro->proximo;
        for(aux=0;aux < NPCs->total;aux++)
        {
           per->m_calModel->update(segundos);   
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
      
      FPSatual = (FPSatual + (1000.0 / varTempo)) / 2; 
     
      SDL_PumpEvents();
      if( (janAtalhos) && (tempo-ultimaFPS >= 300))
      {
         ultimaFPS = tempo;
         char texto[15];
         sprintf(texto,"FPS: %3.2f",1000.0 / (tempo-ultimaLeitura));
         FPS->texto = texto;
         sprintf(texto,"    Part: %d",particleSystem->numParticles());
         FPS->texto += texto;
         janAtalhos->Desenhar(mouseX, mouseY);
      }
      ultimaLeitura = tempo;

        
      //redesenha = TrataIA();
       
      /* Keyboard Events */
      Uint8 *keys;
      keys = SDL_GetKeyState(NULL);

      int x,y;
      Uint8 Mbotao = SDL_GetMouseState(&x,&y);
      mouseX = x;
      mouseY = y;

      /* GUI Events */
      if(gui->ManipulaEventos(x,y,Mbotao,keys)!=NADA)
      {
         redesenha = true;
      }
      else
      { 
      if( (tempo-ultimoMouse>=100 ) || 
          ( (Mbotao & SDL_BUTTON(1)) && (tempo-ultimaPressaoMouse>=100)) )
      {
         cursors->SetActual(CURSOR_WALK);
         ultimoMouse = tempo;
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
         Square* quaux = mapa->quadradoRelativo(qx,qz);
       if(quaux != NULL)
       {
         int pronto;
         int obj = 0;
         GLfloat minObj[3], maxObj[3];
         for(pronto = 0; ( (obj<MAXOBJETOS) && (!pronto) );obj++)
         {
            if(quaux->objetos[obj])
            {
               GLMmodel* modelo3d = (GLMmodel*) quaux->objetos[obj]->modelo3d;
               GLfloat X[4]; GLfloat Z[4];
               X[0] = modelo3d->x1;
               Z[0] = modelo3d->z1;
               X[1] = modelo3d->x1;
               Z[1] = modelo3d->z2; 
               X[2] = modelo3d->x2;
               Z[2] = modelo3d->z2;
               X[3] = modelo3d->x2;
               Z[3] = modelo3d->z1;
               rotTransBoundingBox(quaux->orientacaoObjetos[obj], X, Z,
                              quaux->Xobjetos[obj], 0.0, 
                              0.0,quaux->Zobjetos[obj], 
                              minObj, maxObj);
               if(estaDentro( minObj, maxObj, minMouse, maxMouse, 1))
               {
                   cursors->SetActual(CURSOR_GET);
                   if(janAtalhos)
                   {
                      ObjTxt->texto = quaux->objetos[obj]->nome; 
                      janAtalhos->Desenhar(mouseX,mouseY);
                   }
                   if(Mbotao & SDL_BUTTON(1))
                   {
                      ultimaPressaoMouse = tempo;
                   }
                   pronto = 1;
               }
            }
         }

         /* Doors Verification */
         door* porta = mapa->portas;
         while( (porta != NULL) && (!pronto) )
         {
             GLMmodel* modelo3d = (GLMmodel*) porta->objeto->modelo3d;
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
                 if(janAtalhos)
                 {
                    ObjTxt->texto = language.OBJ_DOOR.c_str(); 
                    janAtalhos->Desenhar(mouseX, mouseY);
                 }
                 if(Mbotao & SDL_BUTTON(1))
                 {
                    ultimaPressaoMouse = tempo;
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
                if(janAtalhos)
                {
                   ObjTxt->texto = pers->nome; 
                   janAtalhos->Desenhar(mouseX, mouseY);
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
                   cursors->SetActual(CURSOR_TALK);
                   if(janAtalhos)
                   {
                      ObjTxt->texto = pers->nome; 
                      janAtalhos->Desenhar(mouseX, mouseY);
                   }
                   pronto = 1;
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
               if(janAtalhos)
               {
                  ObjTxt->texto = quaux->mapConection.mapName; 
                  janAtalhos->Desenhar(mouseX, mouseY);
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

         if( (janAtalhos) && (!pronto) )
         {
            ObjTxt->texto = language.OBJ_NOTHING.c_str(); 
            janAtalhos->Desenhar(mouseX, mouseY);
         }
        }
      }

      if(tempo-ultimaKeyb >= REFRESH_RATE)
      {
         ultimaKeyb = tempo;
         /* Keyboard Verification */
         if ( keys[SDLK_ESCAPE] ) // Exit Engine
            return(0);

         if(keys[SDLK_m]) //Open Minimap
         {
             if(!janMiniMapa)
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
          if(!janAtalhos)
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

         if(podeAndar(varX,varZ,0)) 
         {
            centroX += varX;
            centroZ += varZ;
            PCs->personagemAtivo->posicaoLadoX += varX;
            PCs->personagemAtivo->posicaoLadoZ += varZ;
            redesenha = true;
            andou = true;
         }
         else if( ((varX > 0) && (podeAndar(passo,0,0))) ||
                  ((varX < 0) && (podeAndar(-passo,0,0))) )       
         {
            if(varX < 0)
               passo *= -1;
            centroX += passo;
            PCs->personagemAtivo->posicaoLadoX += passo;
            redesenha = true;
            andou = true;
         }
         else if( ((varZ > 0) && podeAndar(0,passo,0)) ||
                  ((varZ < 0) && podeAndar(0,-passo,0)) )
         {
            if(varZ < 0) 
               passo *= -1;
            centroZ += passo;
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
         if((podeAndar(varX,varZ,0)) ) 
         {
             PCs->personagemAtivo->posicaoLadoX += varX;
             PCs->personagemAtivo->posicaoLadoZ += varZ;
             centroX += varX;
             centroZ += varZ;
             redesenha = true;
             andou  = true;
         }
         else if( ((varX > 0) && (podeAndar(passo,0,0))) ||
                  ((varX < 0) && (podeAndar(-passo,0,0))) ) 
                   
         {
              if(varX < 0 )
                 passo *= -1;
              PCs->personagemAtivo->posicaoLadoX += passo;
              centroX += passo;
              redesenha = true;
              andou = true;
         }
         else if( ((varZ > 0) && (podeAndar(0,passo,0))) ||
                  ((varZ < 0) && (podeAndar(0,-passo,0))) )
         {
              if( varZ < 0 )
                 passo *= -1;
              PCs->personagemAtivo->posicaoLadoZ += passo;
              centroZ += passo;
              redesenha = true;
              andou = true;
         }

      }

      if( (keys[SDLK_a]) || (keys[SDLK_d]))
      {
         // CounterClockWise Character turn
         if((keys[SDLK_a]) && (podeAndar(0,0,rotacao)) )  
         {
            PCs->personagemAtivo->orientacao += rotacao;
            if(PCs->personagemAtivo->orientacao > 360.0)
               PCs->personagemAtivo->orientacao = 
                               PCs->personagemAtivo->orientacao  - 360.0;
            redesenha = true;
            andou = true;
         }
         // Clockwise Character Turn
         if((keys[SDLK_d]) && (podeAndar(0,0,-rotacao)) )
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
         centroX = PCs->personagemAtivo->posicaoLadoX;
         centroZ = PCs->personagemAtivo->posicaoLadoZ;
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
      if(janMiniMapa)
      {
         GLint x = (int)(((PCs->personagemAtivo->posicaoLadoX) / (SQUARESIZE)));
         if(x > mapa->x-1)
         {
            x = mapa->x-1;
         }
         GLint z = (int)(((PCs->personagemAtivo->posicaoLadoZ) / (SQUARESIZE)));
         if( z > mapa->z-1)
         {
            z = mapa->z-1;
         }
         x = 8 + (x*4);
         z = 20 + (z*4);

         botPerMiniMap->x1 = x;
         botPerMiniMap->x2 = x+3;
         botPerMiniMap->y1 = z;
         botPerMiniMap->y2 = z+3;

         janMiniMapa->Desenhar(mouseX, mouseY);
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
   cameraX = centroX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centroY + deltaY + (float) d * sin(deg2Rad(theta));
   cameraZ = centroZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
   gluLookAt(cameraX,cameraY,cameraZ, centroX,centroY,centroZ,0,1,0);

   /* Atualize to culling and to GUI */
   AtualizaFrustum(matrizVisivel,proj,modl);

   /* SKY */
   glPushMatrix();
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, ceu);
      glTranslatef(mapa->x*HALFSQUARESIZE, 0 , mapa->z*HALFSQUARESIZE);
      glScalef(HALFFARVIEW,HALFFARVIEW,HALFFARVIEW);
      glRotated(90,0,1,0);
      glRotated(180,1,0,0);
      //gluSphere(atmosfera,HALFFARVIEW,5,5);
      glCallList(listAtmosfera);
      glDisable(GL_TEXTURE_2D);
   glPopMatrix();

   glPushMatrix();

   /* Draws World, doing view frustum culling */
   mapa->draw(cameraX,cameraY,cameraZ,matrizVisivel);

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

   gui->Desenhar(proj,modl,viewPort);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   
   /* mouse Cursor Draw */
   GLfloat Z = 0.01;
   GLuint Y = SCREEN_Y - mouseY;
   gluUnProject(mouseX,Y, Z, modl, proj, viewPort, &x1, &y1, &z1);
   gluUnProject(mouseX,Y-32,Z, modl, proj, viewPort, &x2, &y2, &z2);
   gluUnProject(mouseX+32,Y-32,Z,modl,proj,viewPort, &x3, &y3, &z3);
   gluUnProject(mouseX+32,Y,Z, modl, proj, viewPort, &x4, &y4, &z4);

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
   //glDisable(GL_ALPHA_TEST);

   glEnable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
 
   glFlush();
}


/******************************************/
/*    PARTE DA DETECCAO DE COLISAO!!!!!!! */
/******************************************/

/*********************************************************************
 *          Faz o teste se o Quadrado quad � factivel de ser         *
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
   if(result) // Se eh possivel entrar, testa com os objetos
   {
      int ob = 0;
      //GLfloat u1,u2,v1,v2;
      GLMmodel* modelo3d;
      GLfloat X[4], Z[4];
      while( (proxima->objetos[ob] != NULL)) 
      {
        if(!proxima->pisavelObj[ob])
        {
          modelo3d = (GLMmodel*)proxima->objetos[ob]->modelo3d;
          X[0] = modelo3d->x1;
          Z[0] = modelo3d->z1;
          X[1] = modelo3d->x1;
          Z[1] = modelo3d->z2;
          X[2] = modelo3d->x2;
          Z[2] = modelo3d->z2;
          X[3] = modelo3d->x2;
          Z[3] = modelo3d->z1;
/* TODO +Yobjetos */
          rotTransBoundingBox(proxima->orientacaoObjetos[ob], X, Z,
                              proxima->Xobjetos[ob], modelo3d->y1, 
                              modelo3d->y2,proxima->Zobjetos[ob], 
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
int engine::podeAndar(GLfloat varX, GLfloat varZ, GLfloat varAlpha)
{
   int result = 1;
   Square* saux;

   GLfloat min[3],min2[3];
   GLfloat max[3],max2[3];

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
       (max[0]>mapa->x*SQUARESIZE-2) || (max[2]>mapa->z*SQUARESIZE-2))
   {
      return(0);
   }

   /* Testa Portas */
   door* porta = mapa->portas;
   while( porta != NULL )
   {
      GLfloat minObj[3], maxObj[3];
      GLMmodel* modeloPorta = (GLMmodel*) porta->objeto->modelo3d;
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
   saux = mapa->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX+1,
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
      saux = mapa->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX+1,
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
      saux = mapa->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX+1,
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
   saux = mapa->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX-1,
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
      saux = mapa->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX-1,
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
      saux = mapa->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX-1,
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
   saux = mapa->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX,
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
   saux = mapa->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX,
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
   if( ColisaoComMeioFio( min, max, mapa->meiosFio) )
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

   PCs->personagemAtivo->ocupaQuad = mapa->quadradoRelativo(posX,posZ);

   saux = mapa->quadradoRelativo( (int)(nx/SQUARESIZE),
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
   centroY = res+30;
   
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
            if(quad->objetos[0])
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
inline void engine::verificaLinha(Square* centro)
{
   Square* saux;
   if(centro)
   {
       verificaQuad(centro);
       saux = mapa->quadradoRelativo(centro->posX-1,centro->posZ);
       if(saux)
       {
          verificaQuad(saux);
          saux = mapa->quadradoRelativo(centro->posX-2,centro->posZ);
          verificaQuad(saux);
       }
       saux = mapa->quadradoRelativo(centro->posX+1,centro->posZ);
       if(saux)
       {
          verificaQuad(saux);
          saux = mapa->quadradoRelativo(centro->posX+2,centro->posZ);
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
       saux = mapa->quadradoRelativo(per->ocupaQuad->posX,
                                     per->ocupaQuad->posZ-1);
       if(saux)
       {
          verificaLinha(saux);
          verificaLinha(saux);
       }
       saux = mapa->quadradoRelativo(per->ocupaQuad->posX,
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
    per->ocupaQuad = mapa->quadradoRelativo(posX,posZ);

    return( (antX!=per->posicaoLadoX) || (antZ!=per->posicaoLadoZ));
}

/*********************************************************************
 *                       Load MiniMap Window                         *
 *********************************************************************/
void engine::OpenMiniMapWindow()
{
   GLint x = (int)(((PCs->personagemAtivo->posicaoLadoX) / (SQUARESIZE)));
   if(x > mapa->x-1)
   {
      x = mapa->x-1;
   }
   GLint z = (int)(((PCs->personagemAtivo->posicaoLadoZ) / (SQUARESIZE)));
   if( z > mapa->z-1)
   {
      z = mapa->z-1;
   }
   x = 8 + (x*3);
   z = 20 + (z*3);
   janMiniMapa = gui->ljan->InserirJanela(0,344,255,471,
                                          language.WINDOW_MAP.c_str(),1,1,
                                          NULL,NULL);

   botPerMiniMap = janMiniMapa->objetos->InserirBotao(x,z,x+2,z+2,255,255,128,
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

   figura* fig = janMiniMapa->objetos->InserirFigura(8,20,240,95,NULL);
   mapa->drawMinimap(fig->fig);

   janMiniMapa->objetos->InserirFigura(3,15,252,120,
                                       "../data/texturas/shortcut2.png");

   
                   
   janMiniMapa->ptrExterno = &janMiniMapa;
   janMiniMapa->Abrir(gui->ljan);
}

/*********************************************************************
 *                         Load ShortCuts Window                     *
 *********************************************************************/
int botaoMenu(void *jan,void *ljan,SDL_Surface *screen)
{
   exitEngine = 1;
   return(1);
}

void engine::OpenShortcutsWindow()
{
   janAtalhos=gui->ljan->InserirJanela(0,472,511,599,
                                  language.WINDOW_SHORTCUTS.c_str(),1,1,
                                       NULL,NULL);
   FPS = janAtalhos->objetos->InserirQuadroTexto(8,20,100,45,0,
                                  language.WINDOW_SHORTCUTS_FPS.c_str());
   ObjTxt = janAtalhos->objetos->InserirQuadroTexto(8,76,180,101,0,
                                  language.WINDOW_SHORTCUTS_HELP.c_str());
   ObjTxt->Cores.corCont[1].R = 0; ObjTxt->Cores.corCont[1].G = 25; 
   ObjTxt->Cores.corCont[1].B = 255;
   ObjTxt = janAtalhos->objetos->InserirQuadroTexto(8,46,150,71,0,
                                 language.OBJ_NOTHING.c_str());

   janAtalhos->objetos->InserirBotao(6,102,74,120,janAtalhos->Cores.corBot.R, 
                                 janAtalhos->Cores.corBot.G,
                                 janAtalhos->Cores.corBot.B,
                                 language.INITIAL_SAVE.c_str(),
                                 0,NULL);
   janAtalhos->objetos->InserirBotao(75,102,143,120,janAtalhos->Cores.corBot.R, 
                                 janAtalhos->Cores.corBot.G,
                                 janAtalhos->Cores.corBot.B,
                                 "Menu",
                                 0,&botaoMenu);
   janAtalhos->objetos->InserirBotao(144,102,212,120,
                                 janAtalhos->Cores.corBot.R, 
                                 janAtalhos->Cores.corBot.G,
                                 janAtalhos->Cores.corBot.B,
                                 language.INITIAL_LOAD.c_str(),
                                 0,NULL);

   tabBotao* tb;
   tb = janAtalhos->objetos->InserirTabBotao(252,15,0,0,
                                             "../data/texturas/shortcuts.png");
   tb->inserirBotao(7,4,43,36,NULL);/* Attack Mode */
   tb->inserirBotao(7,40,43,72,NULL);/* Attack 1 */
   tb->inserirBotao(7,75,43,107,NULL);/* Attack 7 */

   tb->inserirBotao(53,4,89,36,NULL);/* Guard/Sleep Mode */
   tb->inserirBotao(53,40,89,72,NULL);/* Attack 2 */
   tb->inserirBotao(53,75,89,107,NULL);/* Attack 8 */

   tb->inserirBotao(99,4,135,36,NULL);/* Inventory */
   tb->inserirBotao(99,40,135,72,NULL);/* Attack 3 */
   tb->inserirBotao(99,75,135,107,NULL);/* Attack 9 */

   tb->inserirBotao(141,4,177,36,NULL);/* Map */
   tb->inserirBotao(141,40,177,72,NULL);/* Attack 4 */
   tb->inserirBotao(141,75,177,107,NULL);/* Attack 10 */
   
   tb->inserirBotao(180,4,216,36,NULL);/* Party */
   tb->inserirBotao(180,40,216,72,NULL);/* Attack 5 */
   tb->inserirBotao(180,75,216,107,NULL);/* Assign Attacks */

   tb->inserirBotao(220,4,256,36,NULL);/* Get */
   tb->inserirBotao(220,40,256,72,NULL);/* Attack 6 */
   tb->inserirBotao(220,75,256,107,NULL);/* Info */

   janAtalhos->objetos->InserirFigura(3,15,252,120,"../data/texturas/shortcut2.png");
   
   janAtalhos->ptrExterno = &janAtalhos;
   janAtalhos->Abrir(gui->ljan);
}

/*********************************************************************
 *                          Runs the Engine                          *
 *********************************************************************/
int engine::Run(SDL_Surface *surface)
{

   if(!mapa->music.empty())
   {
      snd->StopMusic(musica);
      musica = snd->LoadMusic(mapa->music);
   }

   snd->LoadSample(SOUND_WALK,"../data/sndfx/passos.ogg");

   int forcaAtualizacao = 0; //force screen atualization FIXME, no more used
   FPSatual = 10.0;
   ultimaFPS = 0;

   
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
   while(TrataES(surface,&forcaAtualizacao))
   {
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


