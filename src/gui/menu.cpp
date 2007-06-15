/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "menu.h"

menu::~menu()
{
}

/* Insere um novo botao na lista */
void menu::InserirMenuItem(string text, int dispon)
{
   menuItem* novo;
   novo = new menuItem;
   novo->texto = text;
   novo->disponivel = dispon;
   if(text.length() > maxCarac)
      maxCarac = text.length();
   novo->type = GUI_MENU_ITEM;
   InserirObj(novo);
} 

menuItem* menu::Item(int i)
{
   menuItem* it= (menuItem*) first->next;
   if(i<=total)
   {
      int aux;
      for(aux=1;aux<i;aux++)
         it = (menuItem*) it->next;
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
   color_Set(Cores.colorMenu.R,Cores.colorMenu.G,Cores.colorMenu.B);
   rectangle_Fill(screen,x1+1,y1+1,x2-1,y2-1);
   color_Set(Cores.colorCont[2].R,Cores.colorCont[2].G,Cores.colorCont[2].B);
   rectangle_Oval(screen,x1,y1,x2,y2,Cores.colorCont[1].R,Cores.colorCont[1].G,
                  Cores.colorCont[1].B);
   
      /*itens*/  
   int xa = x1+4;
   int ya = y1+3;
   int k;
   menuItem* item = (menuItem*) first->next;
   for (k=0;k<total;k++)
   {
      color_Set(Cores.colorCont[1].R,Cores.colorCont[1].G,Cores.colorCont[1].B);
      if (item->texto.compare("-"))
      {
          if (item->disponivel) escxy(screen,x1+4,ya-3,item->texto.c_str());
          else
          {
              color_Set(Cores.colorCont[2].R,Cores.colorCont[2].G,
                          Cores.colorCont[2].B);
              escxy(screen,x1+5,ya-2,item->texto.c_str());
              color_Set(Cores.colorCont[1].R,Cores.colorCont[1].G,
                          Cores.colorCont[1].B);
              escxy(screen,x1+4,ya-3,item->texto.c_str());
          }
      } 
      else 
      {
        color_Set(Cores.colorCont[1].R,Cores.colorCont[1].G,Cores.colorCont[1].B);
        rectangle_2Colors(screen,xa-2,ya+6,x2-2,ya+7,Cores.colorCont[0].R,
                         Cores.colorCont[0].G,Cores.colorCont[0].B);
      }
      ya += 11;
      item = (menuItem*)item->next;
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
         pressed = true; 
      }
      else if(pressed)
      {
         /* Got the release event! */
         *pronto = 1;
         pressed = false;
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

      color_Set(Cores.colorCont[1].R,Cores.colorCont[1].G,Cores.colorCont[1].B);
      rectangle_Oval(screen,x+2,(itemAtual-1)*11+y+4,
                     x+largura-2,(itemAtual)*11+y+4,
                     Cores.colorCont[2].R,Cores.colorCont[2].G,
                     Cores.colorCont[2].B);

   /* Calcula o Retorno */
   if(mouse_NaArea(x+Xjan, y+Yjan, x+largura+Xjan, y+altura+Yjan-3,
                   mouseX,mouseY) && (!tecla) &&(*pronto))
   {
      //itemAtual = ((mouseY - y-4) / 11) + 1;
      if (!ItemDisponivel(itemAtual)) itemAtual = 0;
   }
   else if ( (!tecla) && (*pronto))
      itemAtual = 0;
   //else
   //   item = itemAtual;

   return(itemAtual);
}



