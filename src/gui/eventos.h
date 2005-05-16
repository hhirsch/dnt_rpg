/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _eventos_h
#define _eventos_h

#include <string.h>
#include "janela.h"
#include "botao.h"
#include "bartexto.h"
#include "cxsel.h"
#include "mouse.h"
#include "seltexto.h"
#include "menu.h"
#include "lista.h"

/* Constantes de Retorno */
#define NADA                -1
#define SAIR                 0
#define SELTEXTOMODIFICADA   1
#define MENUSELECIONADO      2
#define BOTAOPRESSIONADO     3
#define BOTAONAOPRESSIONADO  4
#define BARRATEXTOESCRITA    5
#define CXSELMODIFICADA      6
#define JANELAFECHADA        7
#define JANELAATIVADA        8
#define JANELAMOVIMENTADA    9
#define JANELACLICADA       10

class interface
{
   public:
      Ljanela* ljan;
      Tlista* objetos;
      SDL_Surface* tela2D;
      SDL_Surface* fundo;
      int ManipulaEventos(SDL_Surface *screen);
      interface(char* arqFundo);
      ~interface();

};



#endif

