/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _bartexto_h
#define _bartexto_h

#include <stdlib.h>
#include <string.h>
#include "cores.h"
#include "desenho.h"
#include "fonte.h"
#include "mouse.h"
#include "objeto.h"

#include <string>
using namespace std;


/* Definicao e Procedimentos de uma Barra de Texto */

class barraTexto: public Tobjeto
{
   public:
      barraTexto();
      ~barraTexto();
      int x1,y1,x2,y2;    /* Coordenadas na janela */
      int disp,           /* Disponivel? */
      cript;              /* Pseudo criptografada? */
      int inic,fim;       /* Extremos do texto visivel atualmente */
      string texto;       /* Texto Presente na Caixa */
      unsigned int pos;
      int ultEsc;
      char ultChar;
                          /* Procedimento Acionado Depois de Editada */
      void (*procEditada)(barraTexto* bart,SDL_Surface *screen);
      cores Cores;

      /* Desenha uma barra de texto na tela
       * Xjan, Yjan -> origem da janela em que esta inserida
       * salvar     -> 0 nao atualiza a tela 1 atualiza
       * screen     -> superficie em que esta presente */
      void Desenhar(int Xjan, int Yjan,int salvar,SDL_Surface *screen);

      /* Procede a escrita de texto em uma barra de texto
       * Xjan,Yjan     -> coordenada da janela na qual esta a barra  
       * mouseX,mouseY -> coordenada do mouse ao entrar na barra 
       * screen        -> a superficie na qual se encontra a barra*/
      int Escrever(int Xjan,int Yjan,int mouseX,int mouseY,
                    SDL_Surface *screen, Uint8 Mbotao, Uint8* teclas);

   private:
      void PoeTexto(int Xjan, int Yjan,int salvar,unsigned int pos, int marca,
                         SDL_Surface *screen);


};          /* Barra de Texto */


#endif
