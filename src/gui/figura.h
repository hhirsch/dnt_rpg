/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _figura_h
#define _figura_h

#include "draw.h"
#include "guiobject.h"

class figura: public guiObject
{
   public:
      int x1,y1,x2,y2;      /* Coordenadas Iniciais da Figura */
      SDL_Surface *fig;     /* A figura em si */

      /* Desenha na superficie a figura desejada
       * Xjan, Yjan -> coordenadas da janela
       * fig        -> figura a ser desenhada
       * salvar     -> se !=0 atualiza a superficie
       * screen     -> superficie em que sera desenha a figura*/
      void Desenhar(int Xjan,int Yjan,int salvar,SDL_Surface *screen);
    
      ~figura();
      figura(int x,int y,int w,int h,const char* arquivo);

};                 /* Figura Interna */



#endif

