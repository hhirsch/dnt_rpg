/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _janela_h
#define _janela_h

#include <stdlib.h>
#include "desenho.h"
#include "cores.h"
#include "fonte.h"
#include "erro.h"
#include "objeto.h"
#include "lista.h"

class janela: public Tobjeto
{
   public:
      int x1,y1,x2,y2;           /* Dimensoes da Janela */
      int minX,minY;             /* Tamanho minimo dos eixos da Janela */
      char *texto;               /* Texto de Titulo da Janela */
      int Rjan,Gjan,Bjan;        /* Cor da Janela */
      int Rbar,Gbar,Bbar;        /* Cor da barra principal da Janela */
      int Rtxt,Gtxt,Btxt;        /* Cor do Texto de titulo da Janela */
      int maximiz;               /* Permite a Maximizacao de uma janela */
      int redmens;               /* Permite o Redmensionamento de uma janela */
      Tlista *objetos;           /* Lista dos Objetos presentes */
      SDL_Surface *cara;         /* Cara da Janela */
      int fechavel;              /* != 0 se for fechavel */
      cores Cores;
                                 /* Procedimento na ativacao */
      void (*procAtiva)(janela* jan, SDL_Surface *screen);
                                 /* Procedimento qdo Pressionada nela */
      void (*procPres)(janela* jan,int x, int y, SDL_Surface *screen);

      /* Desenha na superficie a janela com todos seus derivados
       * jan    -> a janela a ser desenhada
       * salvar -> se !=0 atualiza a tela
       * screen -> a superficie na qual se desenhara a janela */
      void Desenhar(int salvar, SDL_Surface *screen);

      /**/
      void PoeCaraNaTela(int salvar,SDL_Surface *screen);

      /* Ativa a Janela jan, inativando a currentemente ativa 
       * jan    -> janela a ser ativada
       * screen -> superficie na qual sera ativada a janela */
      void Ativar(Tlista *lista,SDL_Surface* screen);

      /*Fecha a janela, limpando seu espaco na tela*/
      void Fechar(Tlista *ljan, SDL_Surface *screen, SDL_Surface* fundo);

      /* Faz a movimentacao da janela pela superficie 
       * jan    -> a janela a se movimentar
       * lista  -> a lista onde a janela esta presente
       * screen -> a superficie na qual esta a janela */
      void Mover(Tlista *lista, SDL_Surface *screen, SDL_Surface* fundo);

      /* Verifica se j esta sobreposta a h2 ou vice-versa
       * h2 -> outra janela */
      int Sobreposta(janela h2);

      //void janela_Redmensionar
      //void janela_MaxiMini

   private:
      void BarraInativa(SDL_Surface *screen);
      void RefazerFundo(Tlista *lista, SDL_Surface *screen, SDL_Surface* fundo);
      void FazMudanca(int mouseX, int mouseY, Tlista *lista,
                      SDL_Surface *screen, SDL_Surface* fundo);


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
      janela* InserirJanela(int xa,int ya,int xb,int yb,char *text,
                         int maximiz,int redmens, 
                         void (*procAtiva)(janela* jan, SDL_Surface *screen),
                         void (*procPres)(janela* jan,int x, int y, 
                               SDL_Surface *screen));

      void Ljanela::RetirarJanela(janela *jan,SDL_Surface *screen);

}; 



#endif

