/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _seltexto_h
#define _seltexto_h

#include "desenho.h"
#include "fonte.h"
#include "cores.h"
#include "mouse.h"
#include "objeto.h"

class selTexto: public Tobjeto
{
   public:
      char *texto[5];      /* 5 Opcoes de Texto a selecionar */
      int y[5];            /* 5 y de cada um - preenchido depois de desenhado */
      int selec;           /* Ultimo Item a ser selecionado no texto */
                           /* Procedimento de qdo seleciona item */
      int (*procPres)(SDL_Surface *screen, int texto);
      int x1,y1,x2,y2;     /* Coordenadas do selTexto */
      cores Cores;

      /* Desenha uma selTexto na superficie
       * Xjan, Yjan  -> coordenadas superiores da janela em que esta a seltexto
       * selecionado -> qual texto esta selecionado
       * salvar      -> !=0 atualiza a superficie
       * screen      -> superficie na qual sera desenhada  */
      void Desenhar(int Xjan,int Yjan,int selecionado,int salvar,
                    SDL_Surface *screen);

      /* Enquanto o mouse estiver sobre a seltexto faz o tratamento da mesma
       * Xjan, Yjan -> coordenadas da janela
       * screen     -> superficie na qual se encontra a selTexto */
      int Tratar(int xa,int ya, Uint8 Mbotao, SDL_Surface *screen,
                 int Xjan, int Yjan);

   private:
   
      void EscreveSelecionado(int Xjan,int Yjan,int selecionado,
                              SDL_Surface *screen);


};                /* Tipo de selecao de 1 entre 5 textos */


#endif


