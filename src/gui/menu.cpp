/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "menu.h"

menu::~menu()
{
}

/* Insere um novo botao na lista */
void menu::InserirMenuItem(char *text, int dispon)
{
   menuItem* novo;
   novo = new menuItem;
   novo->texto = text;
   novo->disponivel = dispon;
   if(strlen(text) > maxCarac)
      maxCarac = strlen(text);
   novo->tipo = MENUITEM;
   InserirObj(novo);
} 

menuItem* menu::Item(int i)
{
   menuItem* it= (menuItem*) primeiro->proximo;
   if(i<=total)
   {
      int aux;
      for(aux=1;aux<i;aux++)
         it = (menuItem*) it->proximo;
      return(it);  
   }
   return(NULL);
}


/* Verifica se o item esta disponivel */
int menu::ItemDisponivel(int item)
{
   menuItem* it = Item(item);
   if(!it) return(0);
   return(it->disponivel);

}


void menu::Coordenada(int xa, int ya)
{
  x = xa;
  y = ya;
}


void menu::Desenhar(int Xjan, int Yjan,int pos, SDL_Surface *screen)
{
   selFonte(FFARSO,ESQUERDA,1);
   /* Inicia coordenadas */
   int x1 = Xjan + x;
   int x2 = x1 + (maxCarac)*(fonte_incCP()+1)+4; 
   int y1 = Yjan + y;
   int y2 = (total*11) + y1  + 5; /* bizarrice da versao do DOS ehhehe */
   
   /* Verifica extremos */
   if (x2 > screen->w-1)
   {
      x1 = (screen->w-1) - (x2 - x1);
      x2 = screen->w-1;
   }
   if (y2 > screen->h-1)
   {
      y1 = (screen->h-1) - (y2 - y1);
      y2 = screen->h-1;
   }

   /* Desenha o menu */
   cor_Definir(Cores.corMenu.R,Cores.corMenu.G,Cores.corMenu.B);
   retangulo_Colorir(screen,x1+1,y1+1,x2-1,y2-1,0);
   cor_Definir(Cores.corCont[2].R,Cores.corCont[2].G,Cores.corCont[2].B);
   retangulo_Oval(screen,x1,y1,x2,y2,Cores.corCont[1].R,Cores.corCont[1].G,
                  Cores.corCont[1].B,0);
   
      /*itens*/  
   int xa = x1+4;
   int ya = y1+3;
   int k;
   menuItem* item = (menuItem*) primeiro->proximo;
   for (k=0;k<total;k++)
   {
      cor_Definir(Cores.corCont[1].R,Cores.corCont[1].G,Cores.corCont[1].B);
      if (strcmp(item->texto,"-"))
      {
          if (item->disponivel) escxy(screen,x1+4,ya-3,item->texto);
          else
          {
              cor_Definir(Cores.corCont[2].R,Cores.corCont[2].G,
                          Cores.corCont[2].B);
              escxy(screen,x1+5,ya-2,item->texto);
              cor_Definir(Cores.corCont[1].R,Cores.corCont[1].G,
                          Cores.corCont[1].B);
              escxy(screen,x1+4,ya-3,item->texto);
          }
      } 
      else 
      {
        cor_Definir(Cores.corCont[1].R,Cores.corCont[1].G,Cores.corCont[1].B);
        retangulo_2Cores(screen,xa-2,ya+6,x2-2,ya+7,Cores.corCont[0].R,
                         Cores.corCont[0].G,Cores.corCont[0].B,0);
      }
      ya += 11;
      item = (menuItem*)item->proximo;
   }
  
//   SDL_Flip(screen);
//   SDL_GL_SwapBuffers();
//   AtualizaTela2D(screen);
}


int menu::Rodar(int mouseX, int mouseY, Uint8 Mbotao, Uint8* teclado,
                SDL_Surface *screen, int *pronto, int Xjan, int Yjan)
{
   selFonte(FFARSO,ESQUERDA,1);

   /* Desenha menu */
   Desenhar(0,0,0,screen);
   int altura = (total*11)+6;
   int largura = (maxCarac)*(fonte_incCP()+1)+5;

   /* Executa */
   *pronto = 0;
   int tecla = 0;
   //int xa,ya;

      /* Verifica Movimentacao do Mouse */
      if(mouse_NaArea(x+Xjan, y+Yjan, x+largura+Xjan,
                      y+altura+Yjan-3, mouseX, mouseY)) 
      {
          itemAtual = ((mouseY - (y+Yjan)-4) / 11) + 1;
      }
      /* Verifica Botao do Mouse Pressionado */
      if(Mbotao & SDL_BUTTON(1))
      {
         *pronto = 1;
      }
      /* Faz a verificacao do Teclado */
      if(teclado[SDLK_UP] && (itemAtual-1 > 0))
      {
         itemAtual --;
      }
      else if(teclado[SDLK_DOWN] && (itemAtual+1 <= total))
      {
         itemAtual++;
      }
      else if( (teclado[SDLK_RETURN] || teclado[SDLK_KP_ENTER]) )
      {
         *pronto = 1;
         tecla = 1;
      }
      else if( teclado[SDLK_ESCAPE])
      {
         *pronto = 1;
         tecla = 0;
      }

      cor_Definir(Cores.corCont[1].R,Cores.corCont[1].G,Cores.corCont[1].B);
      retangulo_Oval(screen,x+2,(itemAtual-1)*11+y+4,
                     x+largura-2,(itemAtual)*11+y+4,
                     Cores.corCont[2].R,Cores.corCont[2].G,
                     Cores.corCont[2].B,1);

   /* Calcula o Retorno */
   if(mouse_NaArea(x+Xjan, y+Yjan, x+largura+Xjan, y+altura+Yjan-3,
                   mouseX,mouseY) && (!tecla))
   {
      //itemAtual = ((mouseY - y-4) / 11) + 1;
      if (!ItemDisponivel(itemAtual)) itemAtual = 0;
   }
   else if ( (!tecla) )
      itemAtual = 0;
   //else
   //   item = itemAtual;

   return(itemAtual);
}



