/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _quadtexto_h
#define _quadtexto_h

#include "cores.h"
#include "desenho.h"
#include "fonte.h"
#include "objeto.h"
#include <string>
using namespace std;

class quadroTexto: public Tobjeto
{
   public:
      ~quadroTexto();
      int x1,y1,x2,y2;     /* Coordenadas da Janela */
      int moldura;         /* 1, moldura e fundo, 2 so moldura */
      string texto;         /* Texto Escrito */
      cores Cores;
      string fonte;
      int aliFonte;
      int tamFonte;

      /* Desenha na superficie um qudroTexto
       * Xjan, Yjan -> coordenadas da janela
       * qt         -> quadroTexto a ser desenhado
       * Salvar     -> !=0 para atualizar a superficie
       * screen     -> superficie onde sera desenhado o quadroTexto */
      void Desenhar(int Xjan,int Yjan, int Salvar, SDL_Surface *screen);

};          /* Quadro de Texto */



#endif

