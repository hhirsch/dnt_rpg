#ifndef _figura_h
#define _figura_h

#include "desenho.h"
#include "erro.h"
#include "objeto.h"

class figura: public Tobjeto
{
   public:
      int x,y;              /* Coordenadas Iniciais da Figura */
      SDL_Surface *fig;     /* A figura em si */

      /* Desenha na superficie a figura desejada
       * Xjan, Yjan -> coordenadas da janela
       * fig        -> figura a ser desenhada
       * salvar     -> se !=0 atualiza a superficie
       * screen     -> superficie em que sera desenha a figura*/
      void Desenhar(int Xjan,int Yjan,int salvar,SDL_Surface *screen);
    
      ~figura();


};                 /* Figura Interna */



#endif

