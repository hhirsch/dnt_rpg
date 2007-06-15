/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _janela_h
#define _janela_h

#include <stdlib.h>
#include "draw.h"
#include "color.h"
#include "fonte.h"
#include "guiobject.h"
#include "lista.h"

#include <string>
using namespace std;


class janela: public guiObject
{
   public:
      int x1,y1,x2,y2;           /* Dimensoes da Janela */
      int minX,minY;             /* Tamanho minimo dos eixos da Janela */
      string texto;              /* Texto de Titulo da Janela */
      int Rjan,Gjan,Bjan;        /* Cor da Janela */
      int Rbar,Gbar,Bbar;        /* Cor da barra principal da Janela */
      int Rtxt,Gtxt,Btxt;        /* Cor do Texto de titulo da Janela */
      int maximiz;               /* Permite a Maximizacao de uma janela */
      int redmens;               /* Permite o Redmensionamento de uma janela */
      Tlista *objects;           /* Lista dos Objetos presentes */
      SDL_Surface *cara;         /* Cara da Janela */
      int fechavel;              /* != 0 se for fechavel */
      int movivel;
      int difx;                  /* Diferenca de qdo o mouse clicou */
      int dify;                  /* No momento de inicio da movimentacao */
      farso_colors Cores;
      janela **ptrExterno;       /* Ponteiro pro Ponteiro externo da Janela */
                                 /* Procedimento na ativacao */
      void (*procAtiva)(janela* jan, SDL_Surface *screen);
                                 /* Procedimento qdo Pressionada nela */
      void (*procPres)(janela* jan,int x, int y, SDL_Surface *screen);

      void (*procFechar)();

      /* Desenha na superficie a janela com todos seus derivados
       * jan    -> a janela a ser desenhada
       */
      void Desenhar(int mouseX, int mouseY);

      /* Ativa a Janela jan, inativando a currentemente ativa 
       * jan    -> janela a ser ativada
       */
      void Ativar(Tlista *lista);

      void Abrir(Tlista *lista);

      /*Fecha a janela, limpando seu espaco na tela*/
      void Fechar(Tlista *ljan);

      /* Faz a movimentacao da janela pela superficie 
       * jan    -> a janela a se movimentar
       * lista  -> a lista onde a janela esta presente
       * screen -> a superficie na qual esta a janela */
      int Mover(Tlista *lista, SDL_Surface *screen, SDL_Surface* fundo,
                int xinic, int yinic, int Mbotao);

      void BarraInativa();
      void BarraAtiva();


   private:
      
};                     /* Janela */


class Ljanela: public Tlista
{
   public:
      janela* janelaAtiva; /* A janela que se encontra ativa no momento */
      Ljanela():Tlista(){janelaAtiva=NULL;};
      ~Ljanela();
 
      /* Insere uma nova janela de cor padrao na lista
       * xa,ya,xb,yb -> dimensoes da janela
       * text        -> titulo da janela
       * procAtiva   -> procedimento chamado qdo a janela for ativada
       * maximiz     -> !=0 se maximizavel
       * redmens     -> !=0 se redmensionavel*/
      janela* InserirJanela(int xa,int ya,int xb,int yb,const char *text,
                         int maximiz,int redmens, 
                         void (*procAtiva)(janela* jan, SDL_Surface *screen),
                         void (*procPres)(janela* jan,int x, int y, 
                               SDL_Surface *screen));

      void RetirarJanela(janela *jan);

}; 



#endif

