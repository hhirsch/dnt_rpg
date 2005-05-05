#include "bartexto.h"

/* Escreve o texto da barra nela mesma */
void barraTexto::PoeTexto(int Xjan, int Yjan,int salvar,unsigned int pos, int marca,
                          SDL_Surface *screen)
{
   unsigned int maxCarac = ((x2-3)-(x1+3)) / (fonte_incCP()+1);
   char* text = texto;
   cor_Definir(Cores.corCont[2].R,Cores.corCont[2].G,Cores.corCont[2].B);
   retangulo_Colorir(screen,x1+1+Xjan,y1+1+Yjan,x2-1+Xjan,y2-1+Yjan,0);
   selFonte(FFARSO,ESQUERDA,1);
   cor_Definir(Cores.corCont[1].R,Cores.corCont[1].G,Cores.corCont[1].B);
   inic = 0;
   if(pos+1>maxCarac) 
   {
      inic = pos-maxCarac;
      if (inic<0) inic = 0;
      fim = pos-1;
   }
   else if (strlen(text)-1>=maxCarac)
   {
      fim = inic+maxCarac-1;
   }
   else
     fim=strlen(text)-1;

   escxy_Def(screen,Xjan+x1+3,Yjan+y1-2,text,inic,fim);
   int x = (Xjan+x1+2)+(pos-inic)*(fonte_incCP()+1);
   if (marca)
      linha_Desenhar(screen,x,Yjan+y1+3,x,Yjan+y2-3,0);
   if(salvar)
   {
     //SDL_Flip(screen);
     //SDL_GL_SwapBuffers();
     AtualizaTela2D(screen);
   }
}


void barraTexto::Desenhar(int Xjan, int Yjan,int salvar, SDL_Surface *screen)
{
   cor_Definir(Cores.corCont[0].R,Cores.corCont[0].G,Cores.corCont[0].B);
   retangulo_2Cores(screen,x1+Xjan,y1+Yjan,x2+Xjan,y2+Yjan,
                  Cores.corCont[1].R,Cores.corCont[1].G,Cores.corCont[1].B,0);
   PoeTexto(Xjan,Yjan,0,0,0,screen);
   if (salvar)
   {
       //SDL_Flip(screen);
       //SDL_GL_SwapBuffers();
       AtualizaTela2D(screen);
   }
}

void barraTexto::Escrever(int Xjan,int Yjan,int mouseX,
                          int mouseY, SDL_Surface *screen)
{
   SDLMod Modificador;
   SDL_Event evento;
   int c;
   int pronto = 0;
   unsigned int pos = (mouseX-(Xjan+x1+2)) / (fonte_incCP()+1);
   int x;
   int y;
   if (pos>strlen(texto)) pos = strlen(texto);
   PoeTexto(Xjan,Yjan,1,pos,1,screen);
   while (!pronto)
   {
      if(SDL_PollEvent(&evento))
      {
         switch(evento.type)
         {
            case SDL_KEYDOWN:
               switch(evento.key.keysym.sym)
               {
                  /* Teclas de Finalizacao */
                  case SDLK_ESCAPE:
                     pronto = 1;
                  break;
                  case SDLK_RETURN:
                  case SDLK_KP_ENTER:
                     pronto = 1;
                  break;
                  /* Teclas de Movimentacao */
                  case SDLK_HOME:
                     pos = 0;
                  break;
                  case SDLK_END:
                     pos = strlen(texto);
                  break;
                  case SDLK_RIGHT:
                     if(pos<strlen(texto)) pos++; 
                  break;
                  case SDLK_LEFT:
                     if(pos>0) pos--;
                  break;
                  /* Teclas de Apagar */
                  case SDLK_DELETE:
                     if(pos<strlen(texto))
                     {   
                        texto = string_Deletar(texto,pos,1);
                     }
                  break;
                  case SDLK_BACKSPACE:
                     if(pos>0)
                     {
                       pos--;
                       texto = string_Deletar(texto,pos,1);
                     }
                  break;
                  /*Caracteres Ignorados*/
                  case SDLK_CAPSLOCK:
                  case SDLK_NUMLOCK:
                  case SDLK_SCROLLOCK:
                  case SDLK_RSHIFT:
                  case SDLK_LSHIFT:
                  case SDLK_RCTRL:
                  case SDLK_LCTRL:
                  case SDLK_RALT:
                  case SDLK_LALT:
                  break;
                  /* As Demais Escrevem */
                  case SDLK_KP_DIVIDE:
                     c='/';
                     texto = string_Colocar(texto,c,pos);
                     pos++;
                  break;
                  default:
                  {
                     c = evento.key.keysym.sym;
                     Modificador=SDL_GetModState();
                     if( ((c >='a') && (c<='z') ) && 
                         (Modificador & KMOD_SHIFT) | (Modificador & KMOD_CAPS))
                         c+= 'A' - 'a';
                     texto = string_Colocar(texto,c,pos);
                     pos++; 
                  }
                  break;
               }
               PoeTexto(Xjan,Yjan,1,pos,1,screen);
            break;
         }
      }
      if(SDL_GetMouseState(&x,&y) & SDL_BUTTON(1))
      {
          if(mouse_NaArea(Xjan+x1,Yjan+y1,Xjan+x2,Yjan+y2,x,y))
               {
                   /* Calcula a nova posicao */
                   pos=(x-(Xjan+x1+2))/(fonte_incCP()+1) + inic;
                   if (pos>strlen(texto)) pos = strlen(texto);
                   PoeTexto(Xjan,Yjan,1,pos,1,screen);
               }
               else
                  pronto = 1;
      }

   }
   PoeTexto(Xjan,Yjan,1,0,0,screen);
   SDL_Delay(150);
}



