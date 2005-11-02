/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _quadtexto_h
#define _quadtexto_h

#include "cores.h"
#include "desenho.h"
#include "fonte.h"
#include "objeto.h"

class quadroTexto: public Tobjeto
{
   public:
      ~quadroTexto();
      int x1,y1,x2,y2;     /* Coordenadas da Janela */
      int moldura;         /* Possui Moldura, se != 0 */
      char* texto;         /* Texto Escrito */
      cores Cores;

      /* Desenha na superficie um qudroTexto
       * Xjan, Yjan -> coordenadas da janela
       * qt         -> quadroTexto a ser desenhado
       * Salvar     -> !=0 para atualizar a superficie
       * screen     -> superficie onde sera desenhado o quadroTexto */
      void Desenhar(int Xjan,int Yjan, int Salvar, SDL_Surface *screen);

};          /* Quadro de Texto */



#endif

