/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "dccnit.h"
#include "culling.h"
#include "../lang/lang.h"
#include <math.h>
#include <SDL/SDL_image.h>
#include "../etc/glm.h"


#define DELAY           0

#define ANDAR           0.75     /* O quanto o personagem anda a cada frame
                                  * A Velocidade do Caracter pode ser calculada
                                  * por ANDAR / 20, em unidades/milisegundo 
                                  */
 
#define GIRAR           2.5     // O quanto ele gira a cada frame
#define DELTACAMERA     1.5     // O quanto a camera move a cada frame
#define ZOOMMAXIMO      80      // Valor máximo de zoom
#define ZOOMMINIMO      280     // Valor mínimo do zoom

#define SCREEN_X        800     // Largura da tela
#define SCREEN_Y        600     // Altura da tela

#define FARVIEW       (SQUARESIZE * 25)  // FARVIEW da camera 
#define HALFFARVIEW   (FARVIEW / 2)      // Metade do Farview


#define CORNEBLINA_R    1.0      // Componente vermelha da neblina
#define CORNEBLINA_G    1.0      // Componente verde da neblina
#define CORNEBLINA_B    1.0      // Componente azul da neblina

/* Conversor de graus para radianos */
inline double deg2Rad(double x){return 6.2831853 * x/360.0;}

/*********************************************************************
 *                       Construtor da Engine                        *
 *********************************************************************/
engine::engine()
{
   /* Inicia as Listas Internas */
   gui  = new interface(NULL);
   mapa = NULL;
   PCs = NULL;
   NPCs = NULL;
   janMiniMapa = NULL;
   janAtalhos = NULL;

   /* Define a posicao da Camera Inicial */
   theta=35;
   phi=0;
   d=150;
   centroX = centroZ = 0;
   centroY = 30;
   deltaY = 0;
   cameraX = centroX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centroY + (float) d * sin(deg2Rad(theta));
   cameraZ = centroZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
   mouseX = 0;
   mouseY = 0;

   cursors = new(cursor);
   snd = new(sound);
   musica = NULL;

   option = new options("dcc.opc");
   snd->ChangeVolume(option->musicVolume, option->sndfxVolume);

   /* Define a ultima vez em que desenhou (so por simplicidade) */
   ultimaLeitura = SDL_GetTicks();
   ultimoMouse = ultimaLeitura;

}

/*********************************************************************
 *                      Destruidor da Engine                         *
 *********************************************************************/
engine::~engine()
{
   //glDeleteLists(mapaDesenhar,1);
   if(musica)
   {
      snd->StopMusic(musica);
   }

   delete(snd);

   delete(option);

   gluDeleteQuadric(atmosfera);
   glDeleteTextures(1, &ceu);
   if(NPCs)
      delete(NPCs);
   if(PCs)
      delete(PCs);
   if(gui)
      delete(gui);
   if(mapa)
   {
      delete(mapa);
   }
   delete(cursors);
}

void atualizaCarga(SDL_Surface* img, GLuint* texturaTexto, 
                   GLuint texturaCarga, char* texto,
                   GLdouble proj[16], GLdouble modl[16],GLint viewPort[4])
{
   glClearColor(0,0,0,1);
   glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
   glDeleteTextures(1,texturaTexto);
   cor_Definir(0,0,0);
   retangulo_Colorir(img,0,0,255,31,0);
   cor_Definir(200,20,20);
   selFonte(FFARSO,CENTRALIZADO,3);
   escxy(img,128,0,texto);
   carregaTextura(img,texturaTexto);
   
   AtualizaTela2D(texturaCarga,proj,modl,viewPort,272,236,527,363,0.01);
   AtualizaTela2D(*texturaTexto,proj,modl,viewPort,272,365,527,396,0.01);
   glFlush();
   SDL_GL_SwapBuffers();
}


/*********************************************************************
 *                       Carrega Mapa na Engine                      *
 *********************************************************************/
int engine::CarregaMapa(char* arqMapa, int RecarregaPCs)
{
   char texto[255];
   char* arq = (char*) malloc(sizeof(char)*255);
   /* to avoyd lose mapname while deleting map */
   strcpy(arq,arqMapa);
   glClearColor(0,0,0,1);
   glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
   glDisable(GL_LIGHTING);
   SDL_Surface* img = IMG_Load("../data/texturas/carregar.jpg");
   SDL_Surface* fig = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       img->w,img->h,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   SDL_BlitSurface(img,NULL,fig,NULL);
   SDL_FreeSurface(img);
   img = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       256,32,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);


   cor_Definir(0,0,0);
   retangulo_Colorir(img,0,0,255,31,0);
   cor_Definir(200,20,20);
   selFonte(FFARSO,CENTRALIZADO,3);
   sprintf(texto,LOAD_MAP,arqMapa);
   escxy(img,128,0,texto);
   GLuint texturaTexto;
   carregaTextura(img,&texturaTexto);

   GLuint texturaCarga;
   carregaTextura(fig,&texturaCarga);
   SDL_FreeSurface(fig);


   AtualizaFrustum(matrizVisivel,proj,modl);
   AtualizaTela2D(texturaCarga,proj,modl,viewPort,272,236,527,363,0.01);
   AtualizaTela2D(texturaTexto,proj,modl,viewPort,272,365,527,396,0.01);
   glFlush();
   SDL_GL_SwapBuffers();

   /* Carregando o Mapa */
   if(mapa) 
   {
     delete(mapa);
   }
   mapa = new(Map);
   mapa->open(arq);

   if(mapa->fog.enabled)
   {
      glEnable(GL_FOG);
      {
        //GLfloat fogCor[4] = {CORNEBLINA_R,CORNEBLINA_G,CORNEBLINA_B,1.0}; 
        glFogi(GL_FOG_MODE,GL_LINEAR);
        glFogfv(GL_FOG_COLOR,mapa->fog.color);
        glFogf(GL_FOG_DENSITY,mapa->fog.density);
        glHint(GL_FOG_HINT,GL_DONT_CARE);
        glFogf(GL_FOG_START,mapa->fog.start);
        glFogf(GL_FOG_END,mapa->fog.end);
      }
   }

   /*atualizaCarga(img,&texturaTexto,texturaCarga,
                 "Loading NPC: Logan",
                 proj, modl, viewPort);*/

   /* Carregando Entao os NPCs */
   if(NPCs)
     delete(NPCs);
   NPCs = new (Lpersonagem);
   personagem* per;
   /*per = NPCs->InserirPersonagem(7,6,10,6,
                                 "../data/pics/logan/portrait.jpg","Logan",
                        "../data/models/personagens/Logan/modelo.cfg");
   per->posicaoLadoX = 30;
   per->posicaoLadoZ = 20;*/

   sprintf(texto, LOAD_NPC, "Ente");
   atualizaCarga(img,&texturaTexto,texturaCarga,
                 texto,
                 proj, modl, viewPort);


   per = NPCs->InserirPersonagem(2,2,5,3,
                                 "../data/pics/logan/portrait.jpg","Jacaranda",
                       "../data/models/personagens/Jacaranda/modelo.cfg");
   per->posicaoLadoX = 300;
   per->posicaoLadoZ = 300;

   sprintf(texto, LOAD_NPC, "Arvore");
   atualizaCarga(img,&texturaTexto,texturaCarga,
                 texto,
                 proj, modl, viewPort);


   per = NPCs->InserirPersonagem(2,2,5,3,
                                 "../data/pics/logan/portrait.jpg","Arvore",
                       "../data/models/personagens/Arvore/modelo.cfg");
   per->posicaoLadoX = 80;
   per->posicaoLadoZ = 720;


   sprintf(texto, LOAD_NPC, "Ratazana");
   atualizaCarga(img,&texturaTexto,texturaCarga,
                 texto,
                 proj, modl, viewPort);

   per = NPCs->InserirPersonagem(2,2,5,3,
                                 "../data/pics/logan/portrait.jpg","Ratazana",
                       "../data/models/personagens/Ratazana/modelo.cfg");
   per->posicaoLadoX = 128;
   per->posicaoLadoZ = 400;

   sprintf(texto, LOAD_NPC, "Ameiva");
   atualizaCarga(img,&texturaTexto,texturaCarga,
                 texto,
                 proj, modl, viewPort);

   per = NPCs->InserirPersonagem(2,2,5,3,
                                 "../data/pics/logan/portrait.jpg","Ameiva",
                       "../data/models/personagens/Ameiva/modelo.cfg");
   per->posicaoLadoX = 360;
   per->posicaoLadoZ = 380;


   if(RecarregaPCs)
   {
       if(PCs)
          delete(PCs);
       PCs  = new (Lpersonagem);
       sprintf(texto, LOAD_PC, "Logan");
       atualizaCarga(img,&texturaTexto,texturaCarga,
                 texto,
                 proj, modl, viewPort);
       per = PCs->InserirPersonagem(7,6,9,7,"../data/pics/logan/portrait.jpg",
                              "Logan",
                       "../data/models/personagens/Logan/modelo.cfg");
       /*atualizaCarga(img,&texturaTexto,texturaCarga,
                 "Loading Character: Gushm",
                 proj, modl, viewPort);
       PCs->InserirPersonagem(7,6,9,7,"../data/pics/logan/portrait.jpg",
                              "Gushm",
                       "../data/models/personagens/Gushm/modelo.cfg");*/
   }

   atualizaCarga(img,&texturaTexto,texturaCarga,
                 LOAD_WINDOWS,
                 proj, modl, viewPort);

   if(janMiniMapa)
     janMiniMapa->Fechar(gui->ljan);
   if(janAtalhos)
     janAtalhos->Fechar(gui->ljan);
   abreMiniMapa();
   abreAtalhos();
      int aux;
      per = (personagem*) PCs->primeiro->proximo;
      for(aux=0;aux< PCs->total;aux++)
      {
         per->m_calModel->update(0); 
         per->CalculateBoundingBox();  
         per = (personagem*) per->proximo;
      }

   glEnable(GL_LIGHTING);

   free(arq);

   /* Coloca Personagem Ativo na posicao de Inicio do Mapa */
   PCs->personagemAtivo->posicaoLadoX = mapa->xInic;
   PCs->personagemAtivo->posicaoLadoZ = mapa->zInic;
   centroX = mapa->xInic;
   centroZ = mapa->zInic;
   PCs->personagemAtivo->ocupaQuad = mapa->squareInic;

   /* Define a Posicao do NPC em Squares 
   per = (personagem*) NPCs->primeiro->proximo;
   int posX =(int)floor((per->posicaoLadoX) / (SQUARESIZE))+1;
   int posZ =(int)floor((per->posicaoLadoZ) / (SQUARESIZE))+1;
   per->ocupaQuad = mapa->quadradoRelativo(posX,posZ);*/

   atualizaCarga(img,&texturaTexto,texturaCarga,
                 LOAD_DONE,
                 proj, modl, viewPort);

   SDL_FreeSurface(img);
   glDeleteTextures(1,&texturaCarga);
   glDeleteTextures(1,&texturaTexto);

   if(!mapa->music.empty())
   {
      snd->StopMusic(musica);
      musica = snd->LoadMusic(mapa->music);
   }

   return(1);

}

/*********************************************************************
 *                       Menu Inicial do Jogo                        *
 *********************************************************************/
int engine::TelaInicial(int Status, GLuint* idTextura)
{
   if(musica)
   {
     snd->StopMusic(musica);
   }
   musica = snd->LoadMusic("../data/music/musica1.ogg");
   AtualizaFrustum(matrizVisivel,proj,modl);
   initialScreen* inic = new(initialScreen);
   int result = inic->Execute(Status, proj, modl, viewPort, idTextura);
   delete(inic);
   return(result);
}

/*********************************************************************
 *                       Tela de Opcoes do Jogo                      *
 *********************************************************************/
int engine::TelaOpcoes(GLuint* idTextura)
{
   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);

   int optionW = OPTIONSW_OTHER;
   int tempo = SDL_GetTicks();
   int tempoAnterior = 0;
   Uint8* keys;
   int x,y;

   option->DisplayOptionsScreen(gui);

   while( (optionW != OPTIONSW_CANCEL) && 
          (optionW != OPTIONSW_CONFIRM))
   {
      tempo = SDL_GetTicks();
      if(tempo - tempoAnterior > 20) 
      {
         SDL_PumpEvents();
         glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
         glClearColor(0,0,0,1);
         keys = SDL_GetKeyState(NULL);
         Uint8 Mbotao = SDL_GetMouseState(&x,&y);
         gui->ManipulaEventos(x,y,Mbotao,keys);
         AtualizaTela2D(*idTextura,proj,modl,viewPort,0,0,799,599,0.012);
         gui->Desenhar(proj,modl,viewPort);
         glFlush();
         SDL_GL_SwapBuffers();
      }
      optionW = option->Treat(gui);
   }

   snd->ChangeVolume(option->musicVolume, option->sndfxVolume);

   glEnable(GL_LIGHTING);
   SDL_ShowCursor(SDL_DISABLE);

   return(optionW);
}

/*********************************************************************
 *                Tela de Criação/Evolução de Personagem             *
 *********************************************************************/
int engine::TelaPersonagens(GLuint* idTextura)
{
   int charCreation = CHAR_OTHER;
   int tempo = SDL_GetTicks();
   int tempoAnterior = 0;
   Uint8* keys;
   int x,y;

   skills* sk = new skills(SKILLS_DIR,"../data/skills/skills.skl"); 
   skillWindow* skWindow = new skillWindow(sk, 20, gui);

   while( (charCreation != CHAR_CANCEL) && 
          (charCreation != CHAR_CONFIRM))
   {
      tempo = SDL_GetTicks();
      if(tempo - tempoAnterior > 20) 
      {
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
      charCreation = skWindow->treat(gui);
   }
   delete(sk);
   delete(skWindow);
   return(charCreation);
}


/*********************************************************************
 *                 Redmensiona a Engine para a Tela                  *
 *********************************************************************/
void engine::Redmensiona(SDL_Surface *screen)
{
   glViewport (0, 0, (GLsizei) screen->w, (GLsizei) screen->h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, (GLsizei)screen->w / (GLsizei)screen->h, 1.0, FARVIEW);
   glGetIntegerv(GL_VIEWPORT, viewPort);
   glMatrixMode (GL_MODELVIEW);

}


/*********************************************************************
 *                   Inicia e Engine para Uso                        *
 *********************************************************************/
void engine::Iniciar(SDL_Surface *screen)
{
  
   Redmensiona(screen);
   
   /* Limpa */
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClearDepth(1.0);

   /* Define detalhamento */
   glDepthFunc(GL_LEQUAL);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);

   /* Definicao da Luz */
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
 
   //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
   
   /* Habilita a iluminacao */
   glEnable(GL_LIGHTING);
   //glEnable(GL_LIGHT0);
   //glDisable(GL_LIGHT0);
   //glEnable(GL_LIGHT1);
  

   atmosfera = gluNewQuadric ();
   //gluQuadricTexture(atmosfera, GL_TRUE);
   gluQuadricTexture(atmosfera, GL_FALSE);

   SDL_Surface* img = IMG_Load("../data/texturas/ceu.jpg");
   SDL_Surface* fg = SDL_CreateRGBSurface(SDL_HWSURFACE,
                      img->w,img->h,32,
                      0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   SDL_BlitSurface(img,NULL,fg,NULL);
   SDL_FreeSurface(img);

   glGenTextures(1, &ceu);
   glBindTexture(GL_TEXTURE_2D, ceu);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fg->w, fg->h, 
                0, GL_RGBA, GL_UNSIGNED_BYTE, fg->pixels);
   /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                   GL_LINEAR_MIPMAP_LINEAR );
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, fg->w,
                     fg->h, GL_RGBA, GL_UNSIGNED_BYTE, 
                     fg->pixels );

   SDL_FreeSurface(fg);

}

/*********************************************************************
 *        Verifica se ha intersecao entre dois bounding boxes        *
 *********************************************************************/
int estaDentro(GLfloat min1[3], GLfloat max1[3],
               GLfloat min2[3], GLfloat max2[3],
               int inverso)
{
   //testa o mínimo X do 2
   if( (min1[0] < min2[0]) && (max1[0] > min2[0]  ) )
   {
      //testa minimo Y
      //if( (min1[1] < min2[1]) && (max1[1] > min2[1]) )
      //{
         //testa minimoZ
         if( (min1[2] < min2[2]) && (max1[2] > min2[2]) )
         {
            return(1);
         }
         //testa maximoZ
         if( (min1[2] < max2[2]) && (max1[2] > max2[2]) )
         {
            return(1);
         }
      //}
      //testa maximo Y
      /*if( (min1[1] < max2[1]) && (max1[1] > max2[1]) )
      {
         //testa minimoZ
         if( (min1[2] < min2[2]) && (max1[2] > min2[2]) )
         {
            return(1);
         }
         //testa maximoZ
         if( (min1[2] < max2[2]) && (max1[2] > max2[2]) )
         {
            return(1);
         }
      }*/
   }
   
   //testa com o máximo X do 2
   if( (min1[0] < max2[0]) && (max1[0] > max2[0]) )
   {
      //testa minimo Y
      //if( (min1[1] < min2[1]) && (max1[1] > min2[1]) )
      //{
         //testa minimoZ
         if( (min1[2] < min2[2]) && (max1[2] > min2[2]) )
         {
            return(1);
         }
         //testa maximoZ
         if( (min1[2] < max2[2]) && (max1[2] > max2[2]) )
         {
            return(1);
         }
      //}
      //testa maximo Y
      /*if( (min1[1] < max2[1]) && (max1[1] > max2[1]) )
      {
         //testa minimoZ
         if( (min1[2] < min2[2]) && (max1[2] > min2[2]) )
         {
            return(1);
         }
         //testa maximoZ
         if( (min1[2] < max2[2]) && (max1[2] > max2[2]) )
         {
            return(1);
         }
      }*/
   }

   //testa casos de cruz + 
   if( (min2[0] < min1[0]) && (max2[0] > max1[0]) &&
       (min2[2] > min1[2]) && (min2[2] < max1[2]))
   {
      return(1);
   }
   if( (min2[2] < min1[2]) && (max2[2] > max1[2]) &&
       (min2[0] > min1[0]) && (min2[0] < max1[0]))
   {
      return(1);
   }

   if(inverso)
   {
      return( estaDentro(min2, max2, min1, max1, 0));
   }
   else 
   {
      return(0);
   }
}

/*********************************************************************
 *               Rotaciona e Translada o Bounding Box                *
 *********************************************************************/
void rotTransBoundingBox(GLfloat orientacao, GLfloat X[4], GLfloat Z[4],
                         GLfloat varX, GLfloat varMinY, GLfloat varMaxY, 
                         GLfloat varZ,
                         GLfloat min[3], GLfloat max[3])
{
   int aux;
   GLfloat x[4];
   GLfloat z[4];
   /* Rotaciona o bounding para a posicao correrta */
   GLfloat cosseno = cos(deg2Rad(orientacao));
   GLfloat seno = sin(deg2Rad(orientacao));
   for(aux = 0;aux<4;aux++)
   {
      x[aux] = (Z[aux]*seno) + (X[aux]*cosseno);
      z[aux] = (Z[aux]*cosseno) - (X[aux]*seno);
   }

   
   /* translada o bounding box para o local correto*/
   min[1] += varMinY;
   max[1] += varMaxY;
   for(aux=0;aux<4;aux++)
   {
     x[aux] += varX;
     z[aux] += varZ;
     if(aux == 0)
     {
        min[0] = x[0]; max[0] = x[0];
        min[2] = z[0]; max[2] = z[0];
     }
     else
     {
         if(x[aux] < min[0])
         {
            min[0] = x[aux];
         }
         if(x[aux] > max[0])
         {
            max[0] = x[aux];
         }
         if(z[aux] < min[2])
         {
            min[2] = z[aux];
         }
         if(z[aux] > max[2])
         {
            max[2] = z[aux];
         }

     }
   }
}

/*********************************************************************
 *                      Tratamento da Entrada/Saida                  *
 *********************************************************************/
int engine::TrataES(SDL_Surface *screen,int *forcaAtualizacao)
{
   bool redesenha = false;
   bool andou = false;
   bool passouTempo = false;
   //bool parado = false;
   Uint32 tempo;
   double varX, varZ; // para evitar de ter de calcular 2 vezes

   double passo;  // O quanto realmente anda, em decorrencia do tempo decorrido
   double rotacao; // O quanto realmente roda, em decorrencia do tempo (FPS)
   double varCamera;
   double varTempo;
   float wx,wy,wz;

   tempo = SDL_GetTicks();
   varTempo = (tempo-ultimaLeitura);
   if( ((varTempo)) >= 20)
   {
      redesenha = true;
      passouTempo = true;

      /* Atualiza Animacoes dos personagens */
      GLfloat segundos = varTempo / 1000.0;
      int aux;
      personagem *per = (personagem*) PCs->primeiro->proximo;
      for(aux=0;aux< PCs->total;aux++)
      {
         per->m_calModel->update(segundos); 
         //per->CalculateBoundingBox(); use only one bounding, for better walk
         per = (personagem*) per->proximo;
      }
      per = (personagem*) NPCs->primeiro->proximo;
      for(aux=0;aux < NPCs->total;aux++)
      {
         per->m_calModel->update(segundos);   
         per->CalculateBoundingBox();
         per = (personagem*) per->proximo;
      }

      /* Calcula as Modificações Reais no Andar, rotacionar, girar, etc */
      varTempo /= 20.0;
      passo = (varTempo)*ANDAR;
      if(passo > 9)
        passo = 9; /* Para evitar que atravesse paredes ao ter LAG*/
      rotacao = (varTempo)*GIRAR;
      varCamera = varTempo*DELTACAMERA;
      
      FPSatual = (FPSatual + (1000.0 / varTempo)) / 2; 
     
      SDL_PumpEvents();
      if( (janAtalhos) && (tempo-ultimaFPS >= 300))
      {
         ultimaFPS = tempo;
         char texto[15];
         sprintf(texto,"FPS: %3.2f",1000.0 / (tempo-ultimaLeitura));
         FPS->texto = texto;
         janAtalhos->Desenhar();
      }
      ultimaLeitura = tempo;

        
      //redesenha = TrataIA();
       
      /* Tratamento das Teclas */
      Uint8 *keys;
      keys = SDL_GetKeyState(NULL);

      int x,y;
      Uint8 Mbotao = SDL_GetMouseState(&x,&y);
      mouseX = x;
      mouseY = y;

      /* Trata A GUI */
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
                      janAtalhos->Desenhar();
                   }
                   if(Mbotao & SDL_BUTTON(1))
                   {
                      ultimaPressaoMouse = tempo;
                   }
                   pronto = 1;
               }
            }
         }

         /* Verificacao de Portas */
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
                    ObjTxt->texto = OBJ_DOOR; 
                    janAtalhos->Desenhar();
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


         /* TODO Abrir Inventorio */
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

            /* Rotaciona e translada o Bounding Box */
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
                   janAtalhos->Desenhar();
                }
                pronto = 1;
            }
            pers = (personagem*) pers->proximo;
         }

         /* TODO Abrir Conversas */
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

            /* Rotaciona e translada o Bounding Box */
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
                   janAtalhos->Desenhar();
                }
                pronto = 1;
            }
            pers = (personagem*) pers->proximo;
         }

         /* Verifica Conexão do Mapa */
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
                  janAtalhos->Desenhar();
               }
               cursors->SetActual(CURSOR_MAPTRAVEL);
               pronto = 1;
               if(Mbotao & SDL_BUTTON(1))
               {
                  CarregaMapa(quaux->mapConection.mapName, 1);
                  return(1);
               }
            }
         }

         if( (janAtalhos) && (!pronto) )
         {
            ObjTxt->texto = OBJ_NOTHING; 
            janAtalhos->Desenhar();
         }
        }
      }


      if ( keys[SDLK_ESCAPE] ) // Sai da Engine
         return(0);

      if(keys[SDLK_m])
      {
          if(!janMiniMapa)
          {
             abreMiniMapa();
             redesenha = true;
          }
      }

      if(keys[SDLK_n])
      {
          if(!janAtalhos)
          {
              abreAtalhos();
              redesenha = true;
          }
      }

      if(keys[SDLK_F1])
      {
         //Tela de Informações // HELP
         SDL_Surface* img = IMG_Load(TEXTURE_INFORMATION);
         SDL_Surface* fig = SDL_CreateRGBSurface(SDL_HWSURFACE,
                          img->w,img->h,32,
                          0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
         SDL_BlitSurface(img,NULL,fig,NULL);
         SDL_FreeSurface(img);

         GLuint texturaInfo;
         carregaTextura(fig,&texturaInfo);
         SDL_FreeSurface(fig);

         glDisable(GL_LIGHTING);
         AtualizaFrustum(matrizVisivel,proj,modl);
         AtualizaTela2D(texturaInfo,proj,modl,viewPort,272,44,527,555,0.0001);
         glEnable(GL_LIGHTING);
         glFlush();
         SDL_GL_SwapBuffers();

         SDL_Delay(100);
         SDL_PumpEvents();
         keys = SDL_GetKeyState(NULL);
         while(!(keys[SDLK_F1] || keys[SDLK_ESCAPE] || keys[SDLK_RETURN]) )
         {
            SDL_Delay(40);
            SDL_PumpEvents();
            keys = SDL_GetKeyState(NULL);
         }

         glDeleteTextures(1,&texturaInfo);
         SDL_Delay(100);
         ultimaLeitura = SDL_GetTicks();
         redesenha = true;
      }

      /* Tratamento das teclas para a Camera */
      if(keys[SDLK_UP])  // Aumenta o Zoom
      {
          if (d>ZOOMMAXIMO)
          {
             d-= varCamera;
             redesenha = true;
          }
      }
      if(keys[SDLK_DOWN]) // Diminui o Zoom
      {
         if(d<ZOOMMINIMO)
         {
             d+= varCamera; 
             redesenha = true;
         }
      }
      if(keys[SDLK_RIGHT]) // Roda Camera Antihorariamente
      {
          phi -= varCamera;  
          redesenha = true;
      }
      if(keys[SDLK_LEFT]) // Roda Camera Horariamente
      {
         phi += varCamera;
         redesenha = true;
      }
      if(keys[SDLK_PAGEUP]) // Sobe com a camera ate visao de cima
      {
            theta += varCamera;
            redesenha = true;
            if(theta > 89) 
               theta = 89;
      }
      if(keys[SDLK_PAGEDOWN]) // desce com a camera ate visao em 1ª pessoa
      {
         theta -= varCamera;
         redesenha = true;
         if(theta < 0)
            theta = 0;
      }
      if (keys[SDLK_HOME]) // Zoom Maximo
      {
         d = ZOOMMAXIMO;
         redesenha = true;
      }
      if(keys[SDLK_END]) // ZoomMinimo
      {
         d = ZOOMMINIMO;
         redesenha = true;
      }   
      if(keys[SDLK_INSERT]) //Maximo para cima
      {
         theta = 89;
         redesenha = true;
      }
      if(keys[SDLK_DELETE]) //Maximo para baixo
      {
         theta = 0;
         redesenha = true;
      }

      /* Tratamento da tecla para Movimentacao do Personagem */
      if(keys[SDLK_q] || keys[SDLK_e])
      {
          varX = passo * sin(deg2Rad(PCs->personagemAtivo->orientacao+90.0));
          varZ = passo * cos(deg2Rad(PCs->personagemAtivo->orientacao+90.0));
         // Anda com personagem de lado para esquerda
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
         else if( (varX != 0) && (podeAndar(varX,0,0)))
         {
            centroX += varX;
            PCs->personagemAtivo->posicaoLadoX += varX;
            redesenha = true;
            andou = true;
         }
         else if( (varZ != 0) && podeAndar(0,varZ,0)) 
         {
            centroZ += varZ;
            PCs->personagemAtivo->posicaoLadoZ += varZ;
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
         else if((varX != 0) && (podeAndar(varX,0,0)))
         {
              PCs->personagemAtivo->posicaoLadoX += varX;
              centroX += varX;
              redesenha = true;
              andou = true;
         }
         else if((varZ!=0) && (podeAndar(0,varZ,0)))
         {
              PCs->personagemAtivo->posicaoLadoZ += varZ;
              centroZ += varZ;
              redesenha = true;
              andou = true;
         }

      }

      if( (keys[SDLK_a]) || (keys[SDLK_d]))
      {
         // Gira personagem antihorariamente
         if((keys[SDLK_a]) && (podeAndar(0,0,rotacao)) )  
         {
            PCs->personagemAtivo->orientacao += rotacao;
            if(PCs->personagemAtivo->orientacao > 360.0)
               PCs->personagemAtivo->orientacao = 
                               PCs->personagemAtivo->orientacao  - 360.0;
            redesenha = true;
            andou = true;
         }
         // Gira o personagem horariamente
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
      if(keys[SDLK_TAB]) //troca de personagem ativo
      {
         if(keys[SDLK_LCTRL]) //para tras
         {
            PCs->personagemAtivo = (personagem*)PCs->personagemAtivo->anterior;
            if(PCs->personagemAtivo == PCs->primeiro)
               PCs->personagemAtivo = (personagem*)PCs->primeiro->anterior;
         }
         else //para frente
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
      
      /* Tratamento do Mouse */

      /* Tratamento do Mouse para Camera */
      if(x == 0)    // Gira a Camera horariamente
      {
         phi+=2; 
         redesenha = true;  
      }
      if(x == screen->w-1) // Gira a camera antihorariamente
      {
        phi-=2; 
        redesenha = true;
      }
   }
   }
   
   if( (redesenha) || ( (*forcaAtualizacao != 0)/* && ((tempo-ultimaLeitura)>=16)*/))
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

         janMiniMapa->Desenhar();
      }
  
      Desenhar();
      SDL_GL_SwapBuffers();
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
 
   return(1);
}

/*********************************************************************
 *                Funcao para desenhar a cena                        *
 *********************************************************************/
void engine::Desenhar()
{
   GLdouble x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4;

   glClear (GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();

   /* Redefine a posicao dinamica da camera */
   cameraX = centroX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centroY + deltaY + (float) d * sin(deg2Rad(theta));
   cameraZ = centroZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
   gluLookAt(cameraX,cameraY,cameraZ, centroX,centroY,centroZ,0,1,0);
 
   /* Atualiza para fazer o culling e o desenho da GUI */
   AtualizaFrustum(matrizVisivel,proj,modl);

   /* CEU */
   glPushMatrix();
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, ceu);
      glTranslatef(mapa->x*HALFSQUARESIZE, 0 , mapa->z*HALFSQUARESIZE);
      gluSphere(atmosfera,HALFFARVIEW,5,5);
      glDisable(GL_TEXTURE_2D);
   glPopMatrix();

   glPushMatrix();

   /* Desenha o Mundo, fazendo culling do view frustum */
   mapa->draw(cameraX,cameraY,cameraZ,matrizVisivel);
  
   /* Desenha os Personagens do Jogador (PCs) */
      personagem* per = (personagem*) PCs->primeiro->proximo;
      int aux;
      for(aux=0;aux < PCs->total;aux++)
      {
         glPushMatrix();
           glTranslatef(per->posicaoLadoX, per->posicaoLadoY,
                        per->posicaoLadoZ);
           glRotatef(per->orientacao,0,1,0);
           per->Render();
           /*glColor3f(0.6,0.1,0.1);
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


 

   /* Atualiza os Valores dos NPCS com o que já estiver no socket */
   //AtualizaNPCs
  
   /* Desenha Entao os NPCs */
      per = (personagem*) NPCs->primeiro->proximo;
      for(aux=0;aux < NPCs->total;aux++)
      {
         glPushMatrix();
           glTranslatef(per->posicaoLadoX, 0 ,per->posicaoLadoZ);
           glRotatef(per->orientacao,0,1,0);
           per->Render();
         glPopMatrix();
         per = (personagem*) per->proximo;
      }

   /* Faz o Desenho da GUI */
   gluUnProject(SCREEN_X,SCREEN_Y, 0.01, modl, proj, viewPort, &x1, &y1, &z1);
   gluUnProject(SCREEN_X,SCREEN_Y-80,0.01, modl, proj, viewPort, &x2, &y2, &z2);
   gluUnProject(SCREEN_X-60,SCREEN_Y-80,0.01,modl,proj,viewPort, &x3, &y3, &z3);
   gluUnProject(SCREEN_X-60,SCREEN_Y,0.01, modl, proj, viewPort, &x4, &y4, &z4);

   glDisable(GL_LIGHTING);

   /* Desenho do Portrait do jogador */
   per = (personagem*) PCs->personagemAtivo;
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, per->portrait );
   glBegin(GL_QUADS);
      glColor3f(1,1,1);
      glTexCoord2f(1,0);
      glVertex3f(x1,y1,z1);
      glTexCoord2f(1,1);
      glVertex3f(x2,y2,z2);
      glTexCoord2f(0,1);
      glVertex3f(x3,y3,z3);
      glTexCoord2f(0,0);
      glVertex3f(x4,y4,z4);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   gui->Desenhar(proj,modl,viewPort);

   glEnable(GL_BLEND);
   //glEnable(GL_ALPHA_TEST);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   //glAlphaFunc(GL_GREATER, 0.1f);
   /* Desnho do cursor do mouse */
   GLuint Y = SCREEN_Y - mouseY;
   gluUnProject(mouseX,Y, 0.01, modl, proj, viewPort, &x1, &y1, &z1);
   gluUnProject(mouseX,Y-32,0.01, modl, proj, viewPort, &x2, &y2, &z2);
   gluUnProject(mouseX+32,Y-32,0.01,modl,proj,viewPort, &x3, &y3, &z3);
   gluUnProject(mouseX+32,Y,0.01, modl, proj, viewPort, &x4, &y4, &z4);
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
 
   glFlush();
}


/******************************************/
/*    PARTE DA DETECCAO DE COLISAO!!!!!!! */
/******************************************/

//min[x,y,z] max[x,y,z]


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
   deltaY = res;

   
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
 *                       Carrega Janela de MiniMapa                  *
 *********************************************************************/
void engine::abreMiniMapa()
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
   janMiniMapa = gui->ljan->InserirJanela(0,344,255,471,WINDOW_MAP,1,1,
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
   
                   
   janMiniMapa->ptrExterno = &janMiniMapa;
   janMiniMapa->Abrir(gui->ljan);
}

/*********************************************************************
 *                       Carrega Janela de Atalhos                   *
 *********************************************************************/
void engine::abreAtalhos()
{
   janAtalhos=gui->ljan->InserirJanela(0,472,511,599,WINDOW_SHORTCUTS,1,1,
                                       NULL,NULL);
   FPS = janAtalhos->objetos->InserirQuadroTexto(8,20,100,45,0,
                                                 WINDOW_SHORTCUTS_FPS);
   ObjTxt = janAtalhos->objetos->InserirQuadroTexto(8,76,180,101,0,
                                                    WINDOW_SHORTCUTS_HELP);
   ObjTxt->Cores.corCont[1].R = 0; ObjTxt->Cores.corCont[1].G = 25; 
   ObjTxt->Cores.corCont[1].B = 255;
   ObjTxt = janAtalhos->objetos->InserirQuadroTexto(8,46,150,71,0,OBJ_NOTHING);
   
   janAtalhos->ptrExterno = &janAtalhos;
   janAtalhos->Abrir(gui->ljan);
}

/*********************************************************************
 *                          Roda a Engine                            *
 *********************************************************************/
int engine::Rodar(SDL_Surface *surface)
{

   if(!mapa->music.empty())
   {
      snd->StopMusic(musica);
      musica = snd->LoadMusic(mapa->music);
   }

   snd->LoadSample(SOUND_WALK,"../data/sndfx/passos.ogg");

   int forcaAtualizacao = 0; //forca a atualizacao da tela, qdo o npc anda
   FPSatual = 10.0;
   ultimaFPS = 0;

   
   /* Inicia AI -- Vai mudar com o tempo e ser relativa a cada NPC */
   ia = new(AI); 
   
   #ifdef REDE
     netevent_p_t eventoRede;
     
     initclientdata( &clientData );
     if ( ( startconnection( &clientData, server, DEFAULTPORT )) == -1 )
     {
         printf("Can't connect!\nAborting...\n");
         return(1);
     }
     entergame( &clientData );
   #endif
  
   /* Roda realmente a engine */
   Desenhar();
   while(TrataES(surface,&forcaAtualizacao))
   {
      /* Trata a Rede. Por padrao, nao estamos usando a rede, uma vez 
       * que a mesma nao se encontra no escopo inicial do projeto, somente
       * sendo util quando se estiver em um estado estável do jogo.
       * No mais, MMORPG é um saco. O bom de rede eh jogar com conhecidos.
       * Mais uma vez farrer fazendo juízos no meio do código. Deve ser louco
       * mesmo. argh!
       */
    #ifdef REDE
      while( (eventoRede = pollnet( &clientData ) ) != NULL )
      {
         switch(eventoRede->type)
         {
             case MT_NEWCHAR: /* Insere Novo Caractere */
             {
                personagem* per;
                per = NPCs->InserirPersonagem(6,8,3,8,
                             "../data/pics/logan/cara.bmp",0,0,
               "LoganNPC","../data/models/personagens/logan_completo_final.obj",
                  "../data/pics/logan/");
                per->posicaoLadoX = eventoRede->x;
                per->posicaoLadoZ = eventoRede->y; 
                per->orientacao = eventoRede->teta;
                per->ID = eventoRede->obj;
		forcaAtualizacao = 1;
                break; 
             }
             case MT_MOV:
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
             default:break; /* Por default nao faz nada! */
         }
      }
    #endif


   }

   if(janAtalhos)
      janAtalhos->Fechar(gui->ljan);
   if(janMiniMapa)
      janMiniMapa->Fechar(gui->ljan);

   delete(ia);
   return(1);
}


