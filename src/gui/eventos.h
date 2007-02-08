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
#define TABBOTAOPRESSIONADO 11
#define BOTAOEMPRESSAO      12
#define MENUMODIFICADO      13

/* Constantes de Foco */
#define FOCO_JANELA       1
#define FOCO_BARRATEXTO   2
#define FOCO_BOTAO        3
#define FOCO_CXSEL        4
#define FOCO_MENU         5
#define FOCO_SELTEXTO     6
#define FOCO_JANELAMOVER  7
#define FOCO_MENUJANELA   8
#define FOCO_TABBUTTON    9
#define FOCO_JOGO         100


class interface
{
   public:
      Tobjeto* manipulateEvents(int x, int y, Uint8 Mbotao, Uint8* tecla,
                                int* eventInfo);
      void draw(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);
      interface(char* arqFundo);
      ~interface();

      void clearActiveObject();

      void closeWindow(janela *jan);

      janela* insertWindow(int xa,int ya,int xb,int yb,const char *text,
                           int maximiz,int redmens);

      void openWindow(janela* jan);
      
      int foco;

   private:
      SDL_Surface* fundo;
      Tlista* objects;
      Ljanela* ljan;
      Tobjeto* objAtivo;
      Tobjeto* chamador;

};



#endif

