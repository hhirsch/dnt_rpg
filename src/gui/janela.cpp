/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "janela.h"
#include "menu.h"
#include "botao.h"
#include "bartexto.h"
#include "cxsel.h"
#include "quadtexto.h"
#include "seltexto.h"

/*********************************************************************
 *                    Destroi a Lista de Janelas                     *
 *********************************************************************/
Ljanela::~Ljanela()
{
   janela* jan;
   jan = (janela*)primeiro->proximo;
   while(jan != primeiro )
   {
      //RetirarJanela((janela*)(primeiro->proximo),NULL);
      SDL_FreeSurface(jan->cara);
      delete(jan->objects);
      jan = (janela*)jan->proximo;
   }
   //delete(primeiro);
} 
 
/*********************************************************************
 *                Insere Uma nova Janela na Lista                    *
 *********************************************************************/
janela* Ljanela::InserirJanela(int xa,int ya,int xb,int yb,const char *text,
                         int maximiz,int redmens, 
                         void (*procAtiva)(janela* jan, SDL_Surface *screen),
                         void (*procPres)(janela* jan,int x, int y, 
                               SDL_Surface *screen))
{
   janela* novo;
   novo = new janela;
   novo->procAtiva = procAtiva;
   novo->procPres = procPres;
   novo->procFechar = NULL;
   novo->ptrExterno = NULL;
   novo->Cores.Iniciar();
   novo->x1 = xa;
   novo->x2 = xb;
   novo->y1 = ya;
   novo->y2 = yb;
   novo->minY = 15;
   novo->minX = 50; //a calcular ainda,assim q tiver redmensonamento(se tiver)
   novo->texto = text;
   novo->maximiz = maximiz;
   novo->redmens = redmens;
   novo->Rjan = novo->Cores.corJan.R;
   novo->Gjan = novo->Cores.corJan.G;
   novo->Bjan = novo->Cores.corJan.B;
   novo->Rbar = novo->Cores.corBarra.R;
   novo->Gbar = novo->Cores.corBarra.G;
   novo->Bbar = novo->Cores.corBarra.B;
   novo->Rtxt = novo->Cores.corTexto.R;
   novo->Gtxt = novo->Cores.corTexto.G;
   novo->Btxt = novo->Cores.corTexto.B;
   novo->fechavel = 1;
   novo->movivel = 1;

   /* Commented the smmalest power of two, since is no more rendered as a
    * texture, but as a raster2D. */
   novo->cara = SDL_CreateRGBSurface(SDL_HWSURFACE,
                                     /*smallestPowerOfTwo*/(xb-xa+1),
                                     /*smallestPowerOfTwo*/(yb-ya+1),32,
                                   0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);

   novo->objects = new Tlista;
   botao* tmp;
   tmp = novo->objects->InserirBotao(3,3,13,12,novo->Cores.corBot.R,
                  novo->Cores.corBot.G,novo->Cores.corBot.B,"_",0,
                  NULL);
   tmp->men = new menu();
   menu* men = (menu*) tmp->men;
   men->InserirMenuItem("Fechar",1);
   men->InserirMenuItem("-",0);
   men->InserirMenuItem("Maximizar",0);
   men->InserirMenuItem("Mover",0); 
   novo->objects->InserirBotao(14,3,24,12,novo->Cores.corBot.R,
                  novo->Cores.corBot.G,novo->Cores.corBot.B,"*",0,
                  NULL);
   novo->objects->InserirBotao(25,3,35,12,novo->Cores.corBot.R,
                  novo->Cores.corBot.G,novo->Cores.corBot.B,"\36",0,
                  NULL);
   novo->tipo = JANELA;
   InserirObj(novo);
   return(novo);
} 

/*********************************************************************
 *                Retira uma janela da lista                      *
 *********************************************************************/
void Ljanela::RetirarJanela(janela *jan)
{
   jan->anterior->proximo = jan->proximo;
   jan->proximo->anterior = jan->anterior;
   if (jan->cara == NULL)
      printf("Sujou: jan->cara == NULL!!!\n");
   SDL_FreeSurface(jan->cara);
   if(jan == janelaAtiva)
   {
      janelaAtiva = NULL;
      if((total > 1))
      {
         janela* j = (janela*)primeiro->proximo;
         j->Ativar(this);
      }
   }
   if(jan->ptrExterno != NULL)
   {
      *jan->ptrExterno = NULL;
   }
   delete(jan->objects);
   delete(jan);
   jan = NULL;
   total--;
}

/*********************************************************************
 *                Desenha a janela em seu buffer interno             *
 *********************************************************************/
void janela::Desenhar(int mouseX, int mouseY)
{
   int dx = x2 - x1 + 1;
   int dy = y2 - y1 + 1;


   cor_Definir(Rjan,Gjan,Bjan);
   retangulo_Colorir(cara, 3,3,dx-3,dy-3,0);
   cor_Definir(Cores.corCont[0].R,Cores.corCont[0].G,Cores.corCont[0].B);
   retangulo_Desenhar(cara,0,0,dx-1,dy-1,0);
   cor_Definir(Cores.corBot.R,Cores.corBot.G,Cores.corBot.B);
   retangulo_Desenhar(cara,1,1,dx-2,dy-2,0);
   cor_Definir(Cores.corCont[2].R,Cores.corCont[2].G,Cores.corCont[2].B);
   linha_Desenhar(cara,2,13,dx-4,13,0);
   cor_Definir(Cores.corCont[0].R,Cores.corCont[0].G,Cores.corCont[0].B);
   retangulo_2Cores(cara,2,2,dx-3,dy-3,
                    Cores.corCont[2].R,Cores.corCont[2].G,Cores.corCont[2].B,0);
   cor_Definir(Cores.corBarra.R,Cores.corBarra.G,Cores.corBarra.B);
   retangulo_Colorir(cara,25,3,dx-4,12,0);
   /* Escrita do Titulo */
   cor_Definir(Cores.corTexto.R,Cores.corTexto.G,Cores.corTexto.B);
   if (!selFonte(FFARSO,ESQUERDA,1))
   {
      erro_Mensagem("Fonte farso.fnt nao encontrada!\n",3);
   }
   escxy(cara,39,-2,texto.c_str());
   /* Desenho dos Botoes */
   Tobjeto *obj=objects->primeiro->proximo;
   int aux;
   for(aux=0;aux<objects->total;aux++)
   {
      switch(obj->tipo)
      {
         case BOTAO:{
              botao *b = (botao*) obj;   
              b->Desenhar(0,this,0);
              break;
         }
         case BARRATEXTO:{
              barraTexto *bart = (barraTexto*) obj; 
              bart->Desenhar(0,0,0,cara);
              break;
         }
         case CXSEL:{
              cxSel *cx = (cxSel*) obj;
              cx->draw(cara);
              break;
         }
         case SELTEXTO:{
              selTexto *st = (selTexto*) obj;
              st->draw(-1,cara);
              break;
         }
         case FIGURA:{
              figura* fig = (figura*) obj;
              fig->Desenhar(0,0,0,cara);
              break;
         }
         case QUADROTEXTO:{
              quadroTexto *quad = (quadroTexto*) obj;
              quad->Desenhar(0,0,0,cara);
              break;
         }
         case TABBOTAO:{
              tabButton *bt = (tabButton*) obj; 
              bt->draw(mouseX, mouseY, x1, y1, cara);
              break;
         }
         default:break;
       
      } //case
      obj = obj->proximo;
   }
}

/*********************************************************************
 *     Desenha a barra de inatividade no buffer interno da janela    *
 *********************************************************************/
void janela::BarraInativa()
{
   int dx = x2-x1;
   cor_Definir(Cores.corJan.R,Cores.corJan.G,Cores.corJan.B);
   retangulo_Colorir(cara,36,3,dx-3,12,0);
   cor_Definir(0,0,0);
   selFonte(FFARSO,ESQUERDA,1);
   escxy(cara,39,-2,texto.c_str());
}

void janela::BarraAtiva()
{
   int dx = x2-x1;
   cor_Definir(Cores.corBarra.R,Cores.corBarra.G,Cores.corBarra.B);
   retangulo_Colorir(cara,36,3,dx-3,12,0);
   cor_Definir(Cores.corTexto.R,Cores.corTexto.G,Cores.corTexto.B);
   selFonte(FFARSO,ESQUERDA,1);
   escxy(cara,39,-2,texto.c_str());
}

/*********************************************************************
 *                       Torna a janela Ativa                        *
 *********************************************************************/
void janela::Ativar(Tlista *lista)
{
  Ljanela *ljan = (Ljanela*) lista;
  if (ljan->janelaAtiva != NULL)
  {
     ljan->janelaAtiva->BarraInativa();
  }
  ljan->janelaAtiva = this;
  BarraAtiva();
  //Desenhar();
  if(procAtiva != NULL)
  {
     procAtiva(this,NULL);
  }
}

void janela::Abrir(Tlista *lista)
{
   Ativar(lista);
   Desenhar(0,0);
}

/*********************************************************************
 *                            Fecha a Janela                         *
 *********************************************************************/
void janela::Fechar(Tlista *ljan)
{
   if(procFechar)
   {
      procFechar();
   }
   if(ptrExterno != NULL)
     *ptrExterno = NULL;
   Ljanela* lista = (Ljanela*) ljan;
   lista->RetirarJanela(this);
}

/*********************************************************************
 *                Faz a Movimentacao da Janela                       *
 *********************************************************************/
int janela::Mover(Tlista *lista, SDL_Surface *screen, SDL_Surface* fundo,
                  int xinic, int yinic, int Mbotao)
{

   int dx = x2 - x1;        /* Tamanho no eixo X da janela */
   int dy = y2 - y1;        /* Tamanho no eixo Y da janela */
   
   int x,y;         /* Coordenadas do Mouse */
   
      if (Mbotao & SDL_BUTTON(1))
      {
         x = xinic - difx; 
         y = yinic - dify;
         if(x + dx > 800-1)
         { 
             x -= (x + dx) - (800-1);
         }
         if(y + dy > 600-1) 
         {
             y -= (y+dy) - (600-1);
         }
         if(x < 0)
            x = 0;
         if(y <0)
            y = 0;
         x1 = x;
         x2 = x + dx;
         y1 = y;
         y2 = y+ dy;
      }
      else 
      {
         //Soltou o botao, entao acabou a movimentacao
         return(0);
      }
      //Continua a moverse ainda
      return(1);
}


