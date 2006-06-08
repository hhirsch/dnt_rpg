/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/


#ifndef _dccnit_h
#define _dccnit_h

#include "../gui/farso.h"
#include "../map/map.h"
#include "cursor.h"
#include "personagens.h"
#include "luta.h"
#include "../ia/ai.h"
#include "initial.h"
#include "options.h"
#include "skillwindow.h"
#include "../classes/skills.h"
#include "../sound/sound.h"
#include "../lang/lang.h"
#include "../particle/part1.h"
#include "../particle/part2.h"
#include "../particle/part3.h"
#include "../particle/part4.h"
#include "../particle/part5.h"
#include "../particle/part6.h"
#include "../particle/part7.h"
#ifdef REDE
  #include "../net/client.h"
#endif

class engine
{
   public:
 
      engine();                        /* Engine Constructor */
      ~engine();                       /* Engine Desctructor */
 
      Lpersonagem *NPCs;               /* NPC's List */
      Lpersonagem *PCs;                /* PC's List */

      #ifdef REDE
         char *server;                 /* Server to Connect */
         clientdata_t clientData;      /* Client Data */
      #endif

      double theta, phi, d;            /* Rotation Up, Sides and Zoom */
      double centroX,centroY,centroZ;  /* Camera Central Position */
      double cameraX,cameraY,cameraZ;  /* Camera Position */
      double deltaY;                   /* Camera DeltaY */
      interface* gui;                  /* GUI use on Engine */
      GLdouble proj[16];               /* Projection Matrix */
      GLdouble modl[16];               /* ModelView Matrix  */
      GLint viewPort[4];               /* ViewPort Matrix */

      /*************************************************************** 
       * Reason: Init Engine to utilization  
       * Param:
       *        screen -> Pointer to the Screen's Surface
       ***************************************************************/
      void Init(SDL_Surface *screen);

      /*************************************************************** 
       * Reason: Runs the Engine  
       * Param:
       *        screen -> Pointer to the Screen's Surface
       ***************************************************************/
      int  Run(SDL_Surface *surface);    

      /*************************************************************** 
       * Reason: Draws the Engine  
       * Param:
       * Obs:    Need to run SDL_GL_SwapBuffers() after;
       ***************************************************************/
      void Draw();

      /*************************************************************** 
       * Reason: Load and activate Map to engine
       * Param:
       *          arqMapa -> string with filename
       *          RecarregaPCs -> 1 to reload PCs 0 to not.
       ***************************************************************/
      int LoadMap(string arqMapa, int RecarregaPCs);

      /*************************************************************** 
       * Reason: Load Initial Menu
       * Param:
       *          Status -> ON_INIT or IN_GAME
       *          idTextura -> pointer to initial screen texture
       *          reloadMusic -> true to reload the music
       ***************************************************************/
      int InitialScreen(int Status, GLuint* idTextura, bool reloadMusic);

      /*************************************************************** 
       * Reason: Load Character's Screens
       * Param:
       *          idTextura -> pointer to initial screen texture
       ***************************************************************/
      int CharacterScreen(GLuint* idTextura);

      /*************************************************************** 
       * Reason: Load Options Screen
       * Param:
       *          idTextura -> pointer to initial screen texture
       ***************************************************************/
      int OptionsScreen(GLuint* idTextura);

      /*************************************************************** 
       * Reason: Load Informations Screen
       * Param:
       ***************************************************************/
      void InformationScreen();

      /*************************************************************** 
       * Reason: Load MiniMap Window
       * Param:
       ***************************************************************/
      void OpenMiniMapWindow();

      /*************************************************************** 
       * Reason: Load ShortCuts
       * Param:
       ***************************************************************/
      void OpenShortcutsWindow();

   private:

      /* Redefine a tela para o tamanho da tela atual
       * screen -> ponteiro para a tela3d atual */
      void Redmensiona(SDL_Surface *screen);

      /* Trata as opcoes de Entrada e Saida na Engine
       * screen -> ponteiro para a tela3d atual */
      int  TrataES(SDL_Surface *screen,int *forcaAtualizacao);

      /* Trata a IA dos NPCS */
      inline void verificaQuad(Square* quad);
      inline void verificaLinha(Square* centro);
      int TrataIA();

      /* Testa Colisão do personagem no ponto determinado. */
      int podeAndar(GLfloat varX, GLfloat varZ, GLfloat varAlpha);


      void drawSphereToList(int lats, int longs);

      Map* mapa;                   /* Mapa Utilizado pela Engine */
      cursor* cursors;             /* Utilized mouse cursors */
 
      GLfloat matrizVisivel[6][4]; /* Matriz do frustum atual */
      GLUquadricObj* atmosfera;    /* Poligono da Atmosfera, onde fica o ceu */
      GLuint listAtmosfera;
      GLuint ceu;                  /* Textura do Ceu, quando utilizado */
      int mouseX,mouseY;           /* Coordenada Atual do Mouse na Tela */
      double xReal, zReal, yReal;  /* Coordenada Atual do Mouse no Mundo */

      //texturasLista* listaTexturas;

      quadroTexto* FPS;            /* Quadro de Texto que mostra o n. de FPS */
      quadroTexto* ObjTxt;         /* Mostra o Objeto Atual */
      janela* janAtalhos;          /* Janela de Atalhos */
      janela* janMiniMapa;         /* Janela do Mini Mapa */

      botao* botPerMiniMap;
   
      Uint32 ultimaLeitura;        /* Ultima leitura da posicao do teclado */
      Uint32 ultimoMouse;          /* Ultima Leitura dos objetos apontados */
      Uint32 ultimaPressaoMouse;   /* Ultima Pressao do Mouse nos objetos */
      Uint32 ultimaFPS;            /* Ultima FPS obtida */
      double FPSatual;             /* FPS Atual */

      sound* snd;                  /* Actual Sounds thing */
      Mix_Music* musica;           /* Actual playing music */
      options* option;             /* Current Options */

      AI* ia;                      /* Modulo de IA. Temporiamente aqui. */

      lang language;

      part1* particula;
      part2* particula2;
      part3* particula3;
      part4* particula4;
      part5* particula5;
      part6* particula6;
      part7* particula7;
      GLfloat segundos; 
};

#endif

