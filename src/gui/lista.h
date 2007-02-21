/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _lista_h
#define _lista_h

#include <string>
using namespace std;

#include "objeto.h"
#include "bartexto.h"
#include "botao.h"
#include "cxsel.h"
#include "figura.h"
#include "erro.h"
#include "quadtexto.h"
#include "seltexto.h"
#include "tabbotao.h"
#include "rolbar.h"

class Tlista{
   public:
      int total;         // total de objetos na lista
      Tobjeto *primeiro; // nodo cabeca
   
      Tlista();
      ~Tlista();

      void Retirar(Tobjeto* obj);

      /* Insere um novo botao na lista
       * xa,ya,xb,yb -> coordenadas do novo botao
       * Ra,Ga,Ba    -> coloracao do novo botao
       * text        -> texto do novo botao
       * oval        -> !=0 se o botao for arrrendondado
       * proc        -> ponteiro pro proc a ser chamado qdo o botao 
       *                for pressionado*/
      botao* InserirBotao(int xa,int ya,int xb,int yb,int Ra,int Ga,
                int Ba,const char* text,int oval,
                int (*proc)(void *jan,void *ljan,SDL_Surface *screen));

      /*! Insert a new cxSel on the List
       * \param xa -> X coordinate
       * \param ya -> Y coordinate
       * \param selecionada -> if selected or not */
      cxSel* insertCxSel(int xa, int ya, bool selecionada);

      /* Insere nova figura na lista de figuras
       * x,y     -> coordenada da figura
       * arquivo -> nome do arquivo que contem a figura
       * lista   -> lista onde sera inserida a figura */
      figura* InserirFigura(int x,int y,int w,int h,const char* arquivo);
      tabButton* InserirTabButton(int x,int y,int w,int h,const char* arquivo);

      /* Insere uma nova barra de texto na lista
       * xa,ya,xb,yb -> Dimensoes da barra de texto
       * text        -> Texto da Barra de Texto
       * cript       -> <>0 se estiver "criptografada"
       * lista       -> ponteiro para lista de barra de texto */
      barraTexto* InserirBarraTexto(int xa,int ya,int xb,int yb,
                                    const char* text, int cript,
                     void (*procEditada)(barraTexto* bart,SDL_Surface *screen));

      /* Insere um novo quadro de Texto na lista 
       * xa,ya,xb,yb -> coordenadas do novo quadro de texto
       * moldura     -> != se o novo quadro de texto tiver moldura
       * text        -> texto do novo quadro de texto
       * lista       -> lista onde se inserira o novo quadro de texto*/
      quadroTexto* InserirQuadroTexto(int xa,int ya,int xb,int yb,int moldura,
                                       const char* text);

      /* Insere um novo selTexto na lista
       * xa,ya,xb,yb -> coordenadas da selTexto (yb eh calculada ao desenhar)
       * textX       -> texto de escolha de numero X
       * proc        -> procedimento a rodar qdo ocorrer escolha
       * lista       -> lista na qual a selTexto vai ser inserida*/
      selTexto* InserirSelTexto(int xa,int ya,int xb,int yb,string text0,
                            string text1,string text2,string text3, 
                            string text4,int (*proc)(SDL_Surface *screen, 
                            int texto));

      rolBar* InsertRolBar(int xa,int ya,int xb,int yb,string txt,
                           SDL_Surface* surface);

      Tobjeto* addMenu();
      Tobjeto* getMenu();
      void removeMenu();

   protected:
      void InserirObj(Tobjeto* obj);

      Tobjeto* intMenu;

};

#endif

