/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


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
     //AtualizaTela2D(screen);
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
      // AtualizaTela2D(screen);
   }
}

int barraTexto::Escrever(int Xjan,int Yjan,int mouseX,
                          int mouseY, SDL_Surface *screen,
                          Uint8 Mbotao, Uint8* teclas)
{
   //SDLMod Modificador;
   int c = -1;
   int pronto = 0;
   //unsigned int pos = (mouseX-(Xjan+x1+2)) / (fonte_incCP()+1);
   //if (pos>strlen(texto)) pos = strlen(texto);
   ultEsc = SDL_GetTicks();
   int maiuscula = (teclas[SDLK_CAPSLOCK] || teclas[SDLK_LSHIFT] || 
                    teclas[SDLK_RSHIFT]);

   //Uint8* teclas = SDL_getKeyState(NULL);

   /* Teclas de Finalizacao */
   if(teclas[SDLK_ESCAPE] || teclas[SDLK_RETURN] || teclas[SDLK_KP_ENTER])
      pronto = 1;
   /* Teclas de Movimentacao */
   else
   if (teclas[SDLK_HOME])
       pos = 0;
   else
   if (teclas[SDLK_END])
       pos = strlen(texto);
   else
   if (teclas[SDLK_RIGHT])
   {
       if(pos<strlen(texto)) pos++; 
   }
   else
   if (teclas[SDLK_LEFT])
   {
       if(pos>0) pos--;
   }
   else
      /* Teclas de Apagar */
   if(teclas[SDLK_DELETE])
   {
       if(pos<strlen(texto))
       {   
          texto = string_Deletar(texto,pos,1);
       }
   }
   else
   if(teclas[SDLK_BACKSPACE])
   {
      if(pos>0)
      {
         pos--;
         texto = string_Deletar(texto,pos,1);
      }
   }
   else
   {
      if( teclas[SDLK_KP_DIVIDE])
      {
          if(maiuscula)
            c = '?';
          else 
            c = '/';
      }
      else 
      if( teclas[SDLK_a])
      {
          if (maiuscula)
            c = 'A';
          else
            c = 'a';
      } 
      else 
      if( teclas[SDLK_b])
      {
          if (maiuscula)
            c = 'B';
          else
            c = 'b';
      }
      else 
      if( teclas[SDLK_c])
      {
          if (maiuscula)
            c = 'C';
          else
            c = 'c';
      } 
      else 
      if( teclas[SDLK_d])
      {
          if (maiuscula)
            c = 'D';
          else
            c = 'd';
      }
      else 
      if( teclas[SDLK_e])
      {
          if (maiuscula)
            c = 'E';
          else
            c = 'e';
      }
      else 
      if( teclas[SDLK_f])
      {
          if (maiuscula)
            c = 'F';
          else
            c = 'f';
      }
      else 
      if( teclas[SDLK_g])
      {
          if (maiuscula)
            c = 'G';
          else
            c = 'g';
      }
      else 
      if( teclas[SDLK_h])
      {
          if (maiuscula)
            c = 'H';
          else
            c = 'h';
      }
      else 
      if( teclas[SDLK_i])
      {
          if (maiuscula)
            c = 'I';
          else
            c = 'i';
      }
      else 
      if( teclas[SDLK_j])
      {
          if (maiuscula)
            c = 'J';
          else
            c = 'j';
      }
      else 
      if( teclas[SDLK_k])
      {
          if (maiuscula)
            c = 'K';
          else
            c = 'k';
      }
      else 
      if( teclas[SDLK_l])
      {
          if (maiuscula)
            c = 'L';
          else
            c = 'l';
      }
      else 
      if( teclas[SDLK_m])
      {
          if (maiuscula)
            c = 'M';
          else
            c = 'm';
      }
      else 
      if( teclas[SDLK_n])
      {
          if (maiuscula)
            c = 'N';
          else
            c = 'n';
      }
      else 
      if( teclas[SDLK_o])
      {
          if (maiuscula)
            c = 'O';
          else
            c = 'o';
      }
      else 
      if( teclas[SDLK_p])
      {
          if (maiuscula)
            c = 'P';
          else
            c = 'p';
      }
      else 
      if( teclas[SDLK_q])
      {
          if (maiuscula)
            c = 'Q';
          else
            c = 'q';
      }
      else 
      if( teclas[SDLK_r])
      {
          if (maiuscula)
            c = 'R';
          else
            c = 'r';
      }
      else 
      if( teclas[SDLK_s])
      {
          if (maiuscula)
            c = 'S';
          else
            c = 's';
      }
      else 
      if( teclas[SDLK_t])
      {
          if (maiuscula)
            c = 'T';
          else
            c = 't';
      }
      else 
      if( teclas[SDLK_u])
      {
          if (maiuscula)
            c = 'U';
          else
            c = 'u';
      }
      else 
      if( teclas[SDLK_w])
      {
          if (maiuscula)
            c = 'W';
          else
            c = 'w';
      }
      else 
      if( teclas[SDLK_x])
      {
          if (maiuscula)
            c = 'X';
          else
            c = 'x';
      }
      else 
      if( teclas[SDLK_y])
      {
          if (maiuscula)
            c = 'Y';
          else
            c = 'y';
      }
      else 
      if( teclas[SDLK_z])
      {
          if (maiuscula)
            c = 'Z';
          else
            c = 'z';
      }
      else 
      if( teclas[SDLK_SLASH])
      {
          if (maiuscula)
            c = '?';
          else
            c = '/';
      }
      else 
      if( teclas[SDLK_BACKSLASH])
      {
          if (maiuscula)
            c = '|';
          else
            c = '\\';
      }
      else 
      if( teclas[SDLK_SEMICOLON])
      {
          if (maiuscula)
            c = ':';
          else
            c = ';';
      }
      else 
      if( teclas[SDLK_0])
      {
          if (maiuscula)
            c = ')';
          else
            c = '0';
      }
      else 
      if( teclas[SDLK_1])
      {
          if (maiuscula)
            c = '!';
          else
            c = '1';
      }
      else 
      if( teclas[SDLK_2])
      {
          if (maiuscula)
            c = '@';
          else
            c = '2';
      }
      else 
      if( teclas[SDLK_3])
      {
          if (maiuscula)
            c = '#';
          else
            c = '3';
      }
      else 
      if( teclas[SDLK_4])
      {
          if (maiuscula)
            c = '$';
          else
            c = '4';
      }
      else 
      if( teclas[SDLK_5])
      {
          if (maiuscula)
            c = '%';
          else
            c = '5';
      }
      else 
      if( teclas[SDLK_6])
      {
          if (maiuscula)
            c = '"';
          else
            c = '6';
      }
      else 
      if( teclas[SDLK_7])
      {
          if (maiuscula)
            c = '7';
          else
            c = '&';
      }
      else 
      if( teclas[SDLK_8])
      {
          if (maiuscula)
            c = '*';
          else
            c = '8';
      }
      else 
      if( teclas[SDLK_9])
      {
          if (maiuscula)
            c = '(';
          else
            c = '9';
      }
      else 
      if( teclas[SDLK_BACKQUOTE])
      {
          if (maiuscula)
            c = '"';
          else
            c = '\'';
      }
      else 
      if( teclas[SDLK_MINUS])
      {
          if (maiuscula)
            c = '_';
          else
            c = '-';
      }
      else 
      if( teclas[SDLK_PERIOD])
      {
          if (maiuscula)
            c = '>';
          else
            c = '.';
      }
      else 
      if( teclas[SDLK_COMMA])
      {
          if (maiuscula)
            c = '<';
          else
            c = ',';
      }
      else 
      if( teclas[SDLK_SPACE])
      {
          if (maiuscula)
            c = ' ';
          else
            c = ' ';
      }      

      if(c!=-1)
      {
        texto = string_Colocar(texto,c,pos);
        pos++;
      }
       
   }
   if(Mbotao & SDL_BUTTON(1))
   {
       if(mouse_NaArea(Xjan+x1,Yjan+y1,Xjan+x2,Yjan+y2,mouseX,mouseY))
       {
           /* Calcula a nova posicao */
           pos=(mouseX -(Xjan+x1+2))/(fonte_incCP()+1) + inic;
           if (pos>strlen(texto)) pos = strlen(texto);
               PoeTexto(Xjan,Yjan,1,pos,1,screen);
       }
       else
          pronto = 1;
  }

  if(!pronto)
    PoeTexto(Xjan,Yjan,1,pos,1,screen);
  else
    PoeTexto(Xjan,Yjan,1,0,0,screen);

  return(pronto);
   //PoeTexto(Xjan,Yjan,1,0,0,screen);
   //SDL_Delay(150);
}



