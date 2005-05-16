/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _botao_h
#define _botao_h

#include "objeto.h"
#include "cores.h"
#include "desenho.h"
#include "mouse.h"

/* Onde eh definido um botao e suas rotinas de utilizacao */

class botao: public Tobjeto{
   public:
      int x1,y1,x2,y2;        // coordenadas
                              // Procedimento a ser chamado qdo pressionado
      int (*procPres)(void *jan,void *ljan,SDL_Surface *screen);
      void* men;              // Menu Vinculado ao Botao
      int R; int G; int B;    // Cor do botao
      int oval;               // Oval ou Retangular
      char* texto;            // Texto Mostrado
      cores Cores;

      /* Desenha o botao na superficie
       * Xjan,Yjan -> coordenada de origem da janela em que o botao esta 
       * pres      -> !=0 se o botao estiver pressionado
       * salvar    -> atualiza a tela se != 0
       * screen    -> superficie na qual se quer desenhar o botao */
      void Desenhar(int Xjan, int Yjan,int pres, int salvar, 
                     SDL_Surface *screen);

      /* Faz o Pressionamento do botao, retornando != 0 se ele ficou pressio
       * nado ate o final
       * Xjan, Yjan -> coordenadas da janela que o botao esta
       * screen     -> superficie em que o botao esta */
       int Pressionar(int Xjan,int Yjan, SDL_Surface *screen);

};

#endif

