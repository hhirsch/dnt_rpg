/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _dccnit_h
#define _dccnit_h

#include "../gui/farso.h"
#include "../map/map.h"
#include "personagens.h"
#include "luta.h"
#include "../ia/ai.h"
#ifdef REDE
  #include "../net/client.h"
//  #include "../net/common.h"
#endif

class engine
{
   public:
 
      /* Construtor da Engine
       * map -> ponteiro para o mapa utilizado pela engine */
      engine();
      /* Destruidor da engine */
      ~engine();
 
      //Tmapa mapa;        // Mapa
      Lpersonagem *NPCs; // NPCs presentes na tela carregada atual
      Lpersonagem *PCs;  // PCs na engine
      int x,y;           // Coordenada Central do mapa atualmente visto

      int RotacaoX,RotacaoY,RotacaoZ; //variaveis de testeatual

      #ifdef REDE
         char *server; /* Define o servidor a se conectar */
         clientdata_t clientData;
      #endif

      /* Variaveis de Controle da Camera/olho */
      double theta, phi, d;  //Rotacao pra cima, pro lado e zoom
      double centroX,centroY,centroZ; // posicao central da camera
      double cameraX,cameraY,cameraZ;
//      double deltaCameraX, deltaCameraZ; // espera para movimentar camera

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
      int CarregaMapa(char* arqMapa, int RecarregaPCs);

      /* Abre a Tela Inicial, com seus tratamentos.
       * Retorna 0 se é para sair, 1 caso contrario */
      int TelaInicial();

      interface* gui;
 
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

      int podeAndar(GLfloat varX, GLfloat varZ, GLfloat varAlpha);

      /* Tratamento de Janelas Especiais */
      void abreMiniMapa();
      void engine::abreAtalhos();

      Map* mapa; /* Mapa Utilizado pela Engine */
 
      GLfloat matrizVisivel[6][4]; /* MAtriz do frustum atual */
      GLdouble proj[16];
      GLdouble modl[16];
      GLint viewPort[4];
      GLUquadricObj* atmosfera;
      GLuint ceu;

      quadroTexto* FPS;
      janela* janAtalhos;
      janela* janMiniMapa;
   
      Uint32 ultimaLeitura; //marca a ultima leitura da posicao do teclado
      Uint32 ultimaFPS;
      double FPSatual;

      AI* ia;

};

#endif

