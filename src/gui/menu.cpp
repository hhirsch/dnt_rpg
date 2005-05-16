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
   
   SDL_Rect ret;
   ret.x = x1;
   ret.y = y1;
   ret.w = x2-x1;
   ret.h = y2-y1; 

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
   AtualizaTela2D(screen);
}


int menu::Rodar(int Xjan, int Yjan, SDL_Surface *screen)
{
   SDL_Rect ret;
   selFonte(FFARSO,ESQUERDA,1);
   /* Inicia coordenadas */
   ret.x = Xjan + x;
   ret.w = (maxCarac)*(fonte_incCP()+1)+5;
   ret.y = Yjan + y;
   ret.h = (total*11)+6;/*
   ret2.x = 0;
   ret2.w = ret.w;
   ret2.y = 0;
   ret2.h = ret.h;*/

   /* Salva inferior ao meu */
  // SDL_Surface *fundo = SDL_CreateRGBSurface(SDL_HWSURFACE,ret.w,ret.h, 
     //                         32, 0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   //SDL_BlitSurface(screen,&ret,fundo,&ret2);
   //SDL_Flip(screen);
   //SDL_GL_SwapBuffers();
//   AtualizaTela2D(screen);

   /* Desenha menu */
   Desenhar(Xjan,Yjan,0,screen);

   /* Executa */
   //SDL_Event evento;
   int mouseX = ret.x; 
   int mouseY = ret.y;
   int pronto = 0;
   int tecla = 0;
   int item = 1;
   int item2;
   Uint8 Mbotao;
   Uint8 *teclado;
   int xa,ya;
   Uint32 tempo = 0;
   Uint32 tempo2 = 0;
   Uint32 ultimaTecla = 0;

   cor_Definir(Cores.corCont[1].R,Cores.corCont[1].G,Cores.corCont[1].B);
   retangulo_Oval(screen,ret.x+2,(item-1)*11+ret.y+4,ret.x+ret.w-2,
                  (item)*11+ret.y+4,Cores.corCont[2].R,
                  Cores.corCont[2].G,Cores.corCont[2].B,1);
   while(!pronto)
   {
      item2 = item;
      SDL_PumpEvents();
      Mbotao = SDL_GetMouseState(&xa,&ya);
      teclado = SDL_GetKeyState(NULL);
      tempo2 = SDL_GetTicks();
      /* Verifica Movimentacao do Mouse */
      if((mouseX != xa || mouseY != ya) && 
            mouse_NaArea(ret.x,ret.y,ret.x+ret.w,ret.y+ret.h-3,xa,ya)) 
      {
          item2 = ((ya - ret.y-4) / 11) + 1;
          mouseX = xa;
          mouseY = ya;
      }
      /* Verifica Botao do Mouse Pressionado */
      if(Mbotao & SDL_BUTTON(1))
      {
         mouseX = xa;
         mouseY = ya;
         pronto = 1;
      }
      /* Faz a verificacao do Teclado */
      if(teclado[SDLK_UP] && (item-1 > 0))
      {
         if( (ultimaTecla != SDLK_UP) || (tempo2 >= tempo + TEMPOREPETICAO) )
         {
            item2 = item-1;
            tempo = tempo2;
         }
         ultimaTecla = SDLK_UP;
      }
      else if(teclado[SDLK_DOWN] && (item+1 <= total))
      {
         if( (ultimaTecla != SDLK_DOWN) || (tempo2 >= tempo + TEMPOREPETICAO) )
         {
            item2 = item + 1;
            tempo = tempo2;
         }
         ultimaTecla = SDLK_DOWN;
      }
      else if( (teclado[SDLK_RETURN] || teclado[SDLK_KP_ENTER]) )
      {
         pronto = 1;
         tecla = 1;
      }
      else if( teclado[SDLK_ESCAPE])
      {
         pronto = 1;
         tecla = 0;
      }
      /* Refaz o retangulo oval, se necessario */
      if(item2 != item)
      {
          cor_Definir(Cores.corMenu.R,Cores.corMenu.G,Cores.corMenu.B);
          retangulo_Oval(screen,ret.x+2,(item-1)*11+ret.y+4,
                         ret.x+ret.w-2,(item)*11+ret.y+4,
                         Cores.corMenu.R,Cores.corMenu.G,Cores.corMenu.B,0);
          item = item2;
          cor_Definir(Cores.corCont[1].R,Cores.corCont[1].G,Cores.corCont[1].B);
          retangulo_Oval(screen,ret.x+2,(item-1)*11+ret.y+4,
                         ret.x+ret.w-2,(item)*11+ret.y+4,
                         Cores.corCont[2].R,Cores.corCont[2].G,
                         Cores.corCont[2].B,1);
      }
   }

   /* Calcula o Retorno */
   if(mouse_NaArea(ret.x,ret.y,ret.x+ret.w,ret.y+ret.h-3,mouseX,mouseY) &&
      (!tecla))
   {
      item = ((mouseY - ret.y-4) / 11) + 1;
      if (!ItemDisponivel(item)) item = 0;
   }
   else if (!tecla)
      item = 0;

   /* Refaz o fundo salvo */
//   SDL_BlitSurface(fundo, NULL, screen, &ret);
   //SDL_Flip(screen);
   //SDL_GL_SwapBuffers();
//   AtualizaTela2D(screen);
//   SDL_FreeSurface(fundo);
   SDL_Delay(100);
   return(item);
}



