/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


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
 
      engine();                        /* Construtor da Engine */
      ~engine();                       /* Destruidor da engine */
 
      Lpersonagem *NPCs;               /* NPCs no mapa carregada atual */
      Lpersonagem *PCs;                /* PCs na engine */

      #ifdef REDE
         char *server;                 /* Define o servidor a se conectar */
         clientdata_t clientData;      /* Dados do Cliente */
      #endif

      double theta, phi, d;            /* Rotacao pra cima, pro lado e zoom */
      double centroX,centroY,centroZ;  /* Posicao central da camera */
      double cameraX,cameraY,cameraZ;  /* Posicao da Camera */
      double deltaY;                   /* DeltaY da camera */
      interface* gui;                  /* GUI utilizada na Engine */
      GLdouble proj[16];               /* Matriz de Projecao utilizada */
      GLdouble modl[16];               /* Matriz de ModelView Utilizada  */
      GLint viewPort[4];               /* Matriz de ViewPort Utilizada */

      /* Inicia a estrutura da Engine para utilizacao
       * screen -> ponteiro para a tela3d atual */
      void Iniciar(SDL_Surface *screen);

      /* Faz a rodagem principal da engine
       * surface -> ponteiro para a tela3d atual */
      int  Rodar(SDL_Surface *surface);    

      /* Faz o desenho da engine na tela atualmente
       * É necessario utilizar SDL_GL_SwapBuffers();
       * apos o termino da funcao. */
      void Desenhar();

      /* Faz a carga do Mapa */
      int CarregaMapa(string arqMapa, int RecarregaPCs);

      /* Abre a Tela Inicial, com seus tratamentos.
       * Retorna 0 se é para sair, 1 caso contrario */
      int TelaInicial(int Status, GLuint* idTextura, bool reloadMusic);

      /* Abre a tela de cricao de caracteres, retornando de acordo */
      int TelaPersonagens(GLuint* idTextura);

      /* Abre a tela de opções/ ajustes do jogo */
      int TelaOpcoes(GLuint* idTextura);

      void InformationScreen();

      /* Tratamento de Janelas Especiais */
      void abreMiniMapa();
      void abreAtalhos();

      int botaoMapa(void *jan,void *ljan,SDL_Surface *screen);

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

      Map* mapa;                   /* Mapa Utilizado pela Engine */
      cursor* cursors;             /* Utilized mouse cursors */
 
      GLfloat matrizVisivel[6][4]; /* Matriz do frustum atual */
      GLUquadricObj* atmosfera;    /* Poligono da Atmosfera, onde fica o ceu */
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
      GLfloat segundos; 
};

#endif

