/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _menu_h
#define _menu_h

/* Onde sao definidos os menus, os itens de menus, submenus, etc */

#include <stdlib.h>
#include "cores.h"
#include "desenho.h"
#include "fonte.h"
#include "string.h"
#include "mouse.h"
#include "objeto.h"
#include "lista.h"

class menuItem: public Tobjeto
{
   public:
      char* texto;        /* Texto do item */
      int disponivel;     /* !=0 se disponivel */
};             /* Estrutura de um item do Menu */

class menu: public Tlista
{
   public:
      int x,y;              /* Coordenada do menu */   
      unsigned int maxCarac;/* tam da Maior string  presente */
      void (*procSelecionado)(void* jan, void* botao,menuItem* item,
                             SDL_Surface* screen);
      cores Cores;
      int itemAtual;
   
      menu(): Tlista()
           {itemAtual = 1;procSelecionado=NULL;maxCarac=0;Cores.Iniciar();};
      ~menu();

      /* Insere um novo menuItem no menu
       * texto      -> texto do menuItem
       * disponivel ->  != 0 se o item estiver disponivel
       * men        -> menu no qual sera inserido o item */
      void InserirMenuItem(char *text, int dispon);

      /* Redefine a coordenada de um menu - Util p/ menus relativos ao mouse
       * x,y -> coordenada */
      void Coordenada(int xa, int ya);

      /* Faz a execucao do menu, retorna o valor do item selecionado ou -1;
       * men    -> menu a ser executado 
       * screen -> superficie na qual esta o menu*/
      int Rodar(int mouseX, int mouseY, Uint8 Mbotao, Uint8* teclado,
                SDL_Surface *screen, int* pronto, int Xjan, int Yjan);

      /*Retorna o i-esimo item*/
      menuItem* Item(int i);

   protected:
      /* Verifica se o item esta disponivel */
      int ItemDisponivel(int item);
      void Desenhar(int Xjan, int Yjan,int pos, SDL_Surface *screen);

};                 /* O menu em si */


#endif

