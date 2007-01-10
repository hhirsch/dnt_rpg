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

/*! Class of text bar (quadroTexto) representation. */
class quadroTexto: public Tobjeto
{
   public:
      /*! Destructor */
      ~quadroTexto();
      
      int x1,y1,x2,y2;     /* Coordenadas da Janela */
      int moldura;         /* 1, moldura e fundo, 2 so moldura */
      string texto;        /* Texto Escrito */
      cores Cores;
      string fonte;
      int aliFonte;
      int tamFonte;

      /*! Draw in the Surface the text bar (quadroTexto)
       * \param Xjan -> X window Coordinate
       * \param Yjan -> Y Window Coordinate
       * \param Salvar -> !=0 to actualize surface
       * \param screen -> surface where quadroTexto will draw */
      void Desenhar(int Xjan,int Yjan, int Salvar, SDL_Surface *screen);

};


#endif

