/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "lista.h"
#include "menu.h"
#include <stdio.h>
#include <SDL/SDL_image.h>


Tlista::Tlista()
{
   total = 0;
   primeiro = new Tobjeto;
   primeiro->tipo = -1;
   primeiro->proximo = primeiro;
   primeiro->anterior = primeiro;
   intMenu = NULL;
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
   if(intMenu)
   {
      removeMenu();
   }
}

void Tlista::Retirar(Tobjeto *obj)
{
   obj->anterior->proximo = obj->proximo;
   obj->proximo->anterior = obj->anterior;
   switch (obj->tipo) 
   {
      case BOTAO:
      {
         botao* b = (botao*) obj;
         delete(b);
         break;
      }
      case BARRATEXTO:
      {
         barraTexto* b = (barraTexto*) obj;
         delete(b);
         break;
      }
      case CXSEL:
      {
         cxSel* c = (cxSel*) obj;
         delete(c);
         break;
      }
      case FIGURA:
      {
         figura* f = (figura*) obj;
         delete(f);
         break;
      }
      case SELTEXTO:
      {
         selTexto* s = (selTexto*) obj;
         delete(s);
         break;
      }
      case QUADROTEXTO:
      {
         quadroTexto* q = (quadroTexto*) obj;
         delete(q);
         break;
      }
      default: 
      {
        delete(obj);
        break;
      }
   }
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
                int Ba,const char* text,int oval,
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
cxSel* Tlista::insertCxSel(int xa,int ya, bool selected)
{
   cxSel* novo;
   novo = new cxSel;
   novo->x = xa;
   novo->y = ya;
   novo->setSelection(selected);
   novo->tipo = CXSEL;
   novo->Colors.Iniciar();
   InserirObj(novo);
   return(novo);
}
 
/* Insere uma nova figura na lista */
figura* Tlista::InserirFigura(int x,int y,int w,int h,const char* arquivo)
{
   //printf("Pondo Figura\n");
   figura* novo;
   novo = new figura(x,y,w,h,arquivo);
   

   novo->tipo = FIGURA;
   InserirObj(novo);
   return(novo);
} 

tabButton* Tlista::InserirTabButton(int x,int y,int w,int h,const char* arquivo)
{
   tabButton* novo;

   if(arquivo)
   {
      novo = new tabButton(x,y,arquivo);
   }
   else
   {
      novo = new tabButton(x,y,w,h);
   }
   

   novo->tipo = TABBOTAO;
   InserirObj(novo);
   return(novo);
} 

barraTexto* Tlista::InserirBarraTexto(int xa,int ya,int xb,int yb,
                                      const char* text,int cript,
                       void (*procEditada)(barraTexto* bart,SDL_Surface *screen))
{
   barraTexto* novo;
   novo = new(barraTexto);
   novo->procEditada = procEditada;
   novo->x1 = xa;
   novo->x2 = xb;
   novo->y1 = ya;
   novo->y2 = yb;
   novo->inic = 0;
   novo->fim = 0;
   novo->pos = 0;
   novo->ultEsc = 0;
   novo->ultChar='\0';
   novo->cript = cript;
   novo->tipo = BARRATEXTO;
   novo->Cores.Iniciar();
   novo->texto = text;
   InserirObj(novo);
   return(novo);
} 
 
/* Insere um novo quadro de Texto na lista */
quadroTexto* Tlista::InserirQuadroTexto(int xa,int ya,int xb,int yb,
                                        int moldura,const char* text)
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
   novo->fonte = FHELVETICA;
   novo->tamFonte = 1;
   novo->aliFonte = ESQUERDA;
   novo->Cores.Iniciar();
   InserirObj(novo);
   return(novo);
} 

/* Insere um novo selTexto na lista */
selTexto* Tlista::insertSelTexto(int xa,int ya,int xb,int yb,
                                  string text0, string text1,
                                  string text2, string text3, 
                                  string text4)
{
   selTexto* novo;
   novo = new selTexto;
   novo->setCoordinate(xa,ya,xb,yb);
   novo->text[0] = text0;
   novo->text[1] = text1;
   novo->text[2] = text2;
   novo->text[3] = text3;
   novo->text[4] = text4;
   novo->tipo = SELTEXTO;
   InserirObj(novo);
   return(novo);
} 

rolBar* Tlista::insertRolBar(int xa,int ya,int xb,int yb,string txt,
                             SDL_Surface* surface)
{
   rolBar* novo;
   novo = new rolBar(xa,ya,xb,yb,txt, this, surface);
   InserirObj(novo);
   return(novo);
}

Tobjeto* Tlista::addMenu()
{
   if(intMenu)
   {
      removeMenu();
   }
   intMenu = (Tobjeto*)new menu();
   return(intMenu);
}

Tobjeto* Tlista::getMenu()
{
   return(intMenu);
}

void Tlista::removeMenu()
{
   if(intMenu)
   {
      menu* men = (menu*)intMenu;
      delete(men);
   }
   intMenu = NULL;
}


