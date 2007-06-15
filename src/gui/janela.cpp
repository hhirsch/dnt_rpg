/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "janela.h"
#include "menu.h"

/*********************************************************************
 *                    Destroi a Lista de Janelas                     *
 *********************************************************************/
Ljanela::~Ljanela()
{
   janela* jan;
   jan = (janela*)first->next;
   while(jan != first )
   {
      //RetirarJanela((janela*)(first->next),NULL);
      SDL_FreeSurface(jan->cara);
      delete(jan->objects);
      jan = (janela*)jan->next;
   }
   //delete(first);
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
   novo->x1 = xa;
   novo->x2 = xb;
   novo->y1 = ya;
   novo->y2 = yb;
   novo->minY = 15;
   novo->minX = 50; //a calcular ainda,assim q tiver redmensonamento(se tiver)
   novo->texto = text;
   novo->maximiz = maximiz;
   novo->redmens = redmens;
   novo->Rjan = novo->Cores.colorWindow.R;
   novo->Gjan = novo->Cores.colorWindow.G;
   novo->Bjan = novo->Cores.colorWindow.B;
   novo->Rbar = novo->Cores.colorBar.R;
   novo->Gbar = novo->Cores.colorBar.G;
   novo->Bbar = novo->Cores.colorBar.B;
   novo->Rtxt = novo->Cores.colorText.R;
   novo->Gtxt = novo->Cores.colorText.G;
   novo->Btxt = novo->Cores.colorText.B;
   novo->fechavel = 1;
   novo->movivel = 1;

   /* Commented the smmalest power of two, since is no more rendered as a
    * texture, but as a raster2D. */
   novo->cara = SDL_CreateRGBSurface(SDL_HWSURFACE,
                                     /*smallestPowerOfTwo*/(xb-xa+1),
                                     /*smallestPowerOfTwo*/(yb-ya+1),32,
                                   0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);

   novo->objects = new Tlista;
   button* tmp;
   tmp = novo->objects->insertButton(3,3,13,12,"_",0);
   tmp->men = new menu(0,0);
   menu* men = (menu*) tmp->men;
   men->insertItem("Fechar",1);
   men->insertItem("-",0);
   men->insertItem("Maximizar",0);
   men->insertItem("Mover",0); 
   novo->objects->insertButton(14,3,24,12,"*",0);
   novo->objects->insertButton(25,3,35,12,"\36",0);
   novo->type = GUI_WINDOW;
   InserirObj(novo);
   return(novo);
} 

/*********************************************************************
 *                Retira uma janela da lista                      *
 *********************************************************************/
void Ljanela::RetirarJanela(janela *jan)
{
   jan->previous->next = jan->next;
   jan->next->previous = jan->previous;
   if (jan->cara == NULL)
      printf("Sujou: jan->cara == NULL!!!\n");
   SDL_FreeSurface(jan->cara);
   if(jan == janelaAtiva)
   {
      janelaAtiva = NULL;
      if((total > 1))
      {
         janela* j = (janela*)first->next;
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


   color_Set(Rjan,Gjan,Bjan);
   rectangle_Fill(cara, 3,3,dx-3,dy-3);
   color_Set(Cores.colorCont[0].R,Cores.colorCont[0].G,Cores.colorCont[0].B);
   rectangle_Draw(cara,0,0,dx-1,dy-1);
   color_Set(Cores.colorButton.R,Cores.colorButton.G,Cores.colorButton.B);
   rectangle_Draw(cara,1,1,dx-2,dy-2);
   color_Set(Cores.colorCont[2].R,Cores.colorCont[2].G,Cores.colorCont[2].B);
   line_Draw(cara,2,13,dx-4,13);
   color_Set(Cores.colorCont[0].R,Cores.colorCont[0].G,Cores.colorCont[0].B);
   rectangle_2Colors(cara,2,2,dx-3,dy-3,Cores.colorCont[2].R,
                     Cores.colorCont[2].G,Cores.colorCont[2].B);
   color_Set(Cores.colorBar.R,Cores.colorBar.G,Cores.colorBar.B);
   rectangle_Fill(cara,25,3,dx-4,12);
   /* Escrita do Titulo */
   color_Set(Cores.colorText.R,Cores.colorText.G,Cores.colorText.B);
   if (!selFonte(FFARSO,ESQUERDA,1))
   {
      printf("Error: Not found farso.fnt font!\n");
   }
   escxy(cara,39,-2,texto.c_str());
   /* Desenho dos Buttonoes */
   guiObject *obj=objects->first->next;
   int aux;
   for(aux=0;aux<objects->total;aux++)
   {
      switch(obj->type)
      {
         case GUI_BUTTON:{
              button *b = (button*) obj;   
              b->draw(0,cara);
              break;
         }
         case GUI_TEXT_BAR:{
              textBar *bart = (textBar*) obj; 
              bart->draw(cara);
              break;
         }
         case GUI_SEL_BOX:{
              cxSel *cx = (cxSel*) obj;
              cx->draw(cara);
              break;
         }
         case GUI_SEL_TEXT:{
              selTexto *st = (selTexto*) obj;
              st->draw(-1,cara);
              break;
         }
         case GUI_PICTURE:{
              picture* fig = (picture*) obj;
              fig->draw(cara);
              break;
         }
         case GUI_TEXT_BOX:{
              quadroTexto *quad = (quadroTexto*) obj;
              quad->Desenhar(0,0,0,cara);
              break;
         }
         case GUI_TAB_BUTTON:{
              tabButton *bt = (tabButton*) obj; 
              bt->draw(mouseX, mouseY, x1, y1, cara);
              break;
         }
         default:break;
       
      } //case
      obj = obj->next;
   }
}

/*********************************************************************
 *     Desenha a barra de inatividade no buffer interno da janela    *
 *********************************************************************/
void janela::BarraInativa()
{
   int dx = x2-x1;
   color_Set(Cores.colorWindow.R,Cores.colorWindow.G,Cores.colorWindow.B);
   rectangle_Fill(cara,36,3,dx-3,12);
   color_Set(0,0,0);
   selFonte(FFARSO,ESQUERDA,1);
   escxy(cara,39,-2,texto.c_str());
}

void janela::BarraAtiva()
{
   int dx = x2-x1;
   color_Set(Cores.colorBar.R,Cores.colorBar.G,Cores.colorBar.B);
   rectangle_Fill(cara,36,3,dx-3,12);
   color_Set(Cores.colorText.R,Cores.colorText.G,Cores.colorText.B);
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


