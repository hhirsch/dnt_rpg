/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "lista.h"
#include <stdio.h>
#include <SDL/SDL_image.h>


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
figura* Tlista::InserirFigura(int x,int y,const char* arquivo)
{
   //printf("Pondo Figura\n");
   figura* novo;
   novo = new figura;
   novo->x = x;
   novo->y = y;
 
   //SDL_Rect ret;
   //ret.w = 0; ret.h = 0; ret.x = img->w; ret.y = img->h;
   
   if(arquivo!=NULL)
   {
      //printf("Arquivo: %s\n",arquivo);
      SDL_Surface* img = IMG_Load(arquivo);
      novo->fig = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       img->w,img->h,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
      SDL_BlitSurface(img,NULL,novo->fig,NULL);
      SDL_FreeSurface(img);
   }
   else
   {
      //printf("NULL\n");
      novo->fig = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       240,95,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   }

   if ( novo->fig == NULL )
        erro_Mensagem("Não foi possível carregar figura\n",10);

   novo->tipo = FIGURA;
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
   novo->texto = (char*) malloc(256*sizeof(char));
   strcpy(novo->texto,text);
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
   novo->texto = (char*) malloc(256*sizeof(char));
   sprintf(novo->texto,"%s",text);
   novo->tipo = QUADROTEXTO;
   novo->Cores.Iniciar();
   InserirObj(novo);
   return(novo);
} 

/* Insere um novo selTexto na lista */
selTexto* Tlista::InserirSelTexto(int xa,int ya,int xb,int yb,
                                  string text0, string text1,
                                  string text2, string text3, 
                                  string text4,
                             int (*proc)(SDL_Surface *screen,int texto))
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




