#include "lista.h"
#include <stdio.h>


Tlista::Tlista()
{
   total = 0;
   primeiro = new Tobjeto;
   primeiro->tipo = -1;
   primeiro->proximo = primeiro;
   primeiro->anterior = primeiro;
}


Tlista::~Tlista()
{
   int tot = total;
   int aux;
   for(aux = 0;aux < tot;aux++)
   {
      Retirar(primeiro->proximo);
   }
   if (primeiro!= NULL)   delete primeiro;
}

void Tlista::Retirar(Tobjeto *obj)
{
   obj->anterior->proximo = obj->proximo;
   obj->proximo->anterior = obj->anterior;
   delete obj;
   total--;
}

void Tlista::InserirObj(Tobjeto* obj)
{
   obj->proximo = primeiro->proximo;
   obj->anterior = primeiro;
   primeiro->proximo = obj;
   obj->proximo->anterior = obj;
   total++;
}

botao* Tlista::InserirBotao(int xa,int ya,int xb,int yb,int Ra,int Ga,
                int Ba,char* text,int oval,
                int (*proc)(void *jan,void *ljan,SDL_Surface *screen))
{
   botao* novo;
   novo = new botao;
   novo->x1 = xa;
   novo->x2 = xb;
   novo->y1 = ya;
   novo->y2 = yb;
   novo->R = Ra;
   novo->G = Ga;
   novo->B = Ba;
   novo->procPres = proc;
   novo->texto = text;
   novo->oval = oval;
   novo->men = NULL;
   novo->tipo = BOTAO;
   novo->Cores.Iniciar();
   InserirObj(novo);
   return(novo);
}

/* Insere uma nova cxSel na lista */
cxSel* Tlista::InserirCxSel(int xa,int ya,int selecionada)
{
   cxSel* novo;
   novo = new cxSel;
   novo->x = xa;
   novo->y = ya;
   novo->selecionada = selecionada;
   novo->tipo = CXSEL;
   novo->Cores.Iniciar();
   InserirObj(novo);
   return(novo);
}

/* Insere uma nova figura na lista */
figura* Tlista::InserirFigura(int x,int y,char* arquivo)
{
   figura* novo;
   novo = new figura;
   novo->x = x;
   novo->y = y;
   novo->fig = SDL_LoadBMP(arquivo);
   if ( novo->fig == NULL )
        erro_Mensagem("Não foi possível carregar figura\n",10);
   novo->tipo = FIGURA;
   InserirObj(novo);
   return(novo);
} 

barraTexto* Tlista::InserirBarraTexto(int xa,int ya,int xb,int yb,
                                      char* text,int cript,
                       void (*procEditada)(barraTexto* bart,SDL_Surface *screen))
{
   barraTexto* novo;
   novo = new barraTexto;
   novo->procEditada = procEditada;
   novo->x1 = xa;
   novo->x2 = xb;
   novo->y1 = ya;
   novo->y2 = yb;
   novo->inic = 0;
   novo->fim = 0;
   novo->texto = text;
   novo->cript = cript;
   novo->tipo = BARRATEXTO;
   novo->Cores.Iniciar();
   InserirObj(novo);
   return(novo);
} 
 
/* Insere um novo quadro de Texto na lista */
quadroTexto* Tlista::InserirQuadroTexto(int xa,int ya,int xb,int yb,
                                        int moldura,char* text)
{
   quadroTexto* novo;
   novo = new quadroTexto;
   novo->x1 = xa;
   novo->x2 = xb;
   novo->y1 = ya;
   novo->y2 = yb;
   novo->moldura = moldura;
   novo->texto = text;
   novo->tipo = QUADROTEXTO;
   novo->Cores.Iniciar();
   InserirObj(novo);
   return(novo);
} 

/* Insere um novo selTexto na lista */
selTexto* Tlista::InserirSelTexto(int xa,int ya,int xb,int yb,char* text0,
                                  char* text1,char* text2,char* text3, 
                                  char* text4,int (*proc)(SDL_Surface *screen, 
                                                          int texto))
{
   selTexto* novo;
   novo = new selTexto;
   novo->x1 = xa;
   novo->x2 = xb;
   novo->y1 = ya;
   novo->y2 = yb;
   novo->procPres = proc;
   novo->texto[0] = text0;
   novo->texto[1] = text1;
   novo->texto[2] = text2;
   novo->texto[3] = text3;
   novo->texto[4] = text4;
   novo->selec = -1;
   novo->tipo = SELTEXTO;
   novo->Cores.Iniciar();
   InserirObj(novo);
   return(novo);
} 




