/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "bartexto.h"

barraTexto::barraTexto()
{
}

barraTexto::~barraTexto()
{
}

/* Escreve o texto da barra nela mesma */
void barraTexto::PoeTexto(int Xjan, int Yjan,int salvar,unsigned int pos, int marca,
                          SDL_Surface *screen)
{
   unsigned int maxCarac = ((x2-3)-(x1+3)) / (fonte_incCP()+1);
   string text = texto;
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
   else if (text.length()-1>=maxCarac)
   {
      fim = inic+maxCarac-1;
   }
   else
     fim=text.length()-1;

   escxy_Def(screen,Xjan+x1+3,Yjan+y1-2,text.c_str(),inic,fim);
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
   char c[1];
   c[0] = -1;
   int pronto = 0;
   //unsigned int pos = (mouseX-(Xjan+x1+2)) / (fonte_incCP()+1);
   //if (pos>strlen(texto)) pos = strlen(texto);
   int ult = SDL_GetTicks();
   int maiuscula = (teclas[SDLK_CAPSLOCK] || teclas[SDLK_LSHIFT] || 
                    teclas[SDLK_RSHIFT]);

   //Uint8* teclas = SDL_getKeyState(NULL);

   /* Teclas de Finalizacao */
   if(teclas[SDLK_ESCAPE] || teclas[SDLK_RETURN] || teclas[SDLK_KP_ENTER])
      pronto = 1;
   /* Teclas de Movimentacao */
   else
   if ( (teclas[SDLK_HOME]) && ((ult - ultEsc) >= 100))
   {
       pos = 0;
       ultEsc = ult;
   }
   else
   if ( (teclas[SDLK_END]) && ((ult - ultEsc) >= 100))
   {
       pos = texto.length();
       ultEsc = ult;
   }
   else
   if ((teclas[SDLK_RIGHT]) && ((ult - ultEsc) >= 100))
   {
       if(pos<texto.length()) pos++; 
       ultEsc = ult;
   }
   else
   if ((teclas[SDLK_LEFT]) && ((ult - ultEsc) >= 100))
   {
       if(pos>0) pos--;
       ultEsc = ult;
   }
   else
      /* Teclas de Apagar */
   if( (teclas[SDLK_DELETE]) && ((ult - ultEsc) >= 100))
   {
       if(pos<texto.length())
       {   
          //texto = string_Deletar(texto,pos,1);
          texto.erase(pos,1);
       }
       ultEsc = ult;
   }
   else
   if((teclas[SDLK_BACKSPACE]) && ((ult - ultEsc) >= 100))
   {
      if(pos>0)
      {
         pos--;
         //texto = string_Deletar(texto,pos,1);
         texto.erase(pos,1);
      }
      ultEsc = ult;
   }
   else
   {
      if( teclas[SDLK_KP_DIVIDE])
      {
          if(maiuscula)
            c[0] = '?';
          else 
            c[0] = '/';
      }
      else 
      if( teclas[SDLK_a])
      {
          if (maiuscula)
            c[0] = 'A';
          else
            c[0] = 'a';
      } 
      else 
      if( teclas[SDLK_b])
      {
          if (maiuscula)
            c[0] = 'B';
          else
            c[0] = 'b';
      }
      else 
      if( teclas[SDLK_c])
      {
          if (maiuscula)
            c[0] = 'C';
          else
            c[0] = 'c';
      } 
      else 
      if( teclas[SDLK_d])
      {
          if (maiuscula)
            c[0] = 'D';
          else
            c[0] = 'd';
      }
      else 
      if( teclas[SDLK_e])
      {
          if (maiuscula)
            c[0] = 'E';
          else
            c[0] = 'e';
      }
      else 
      if( teclas[SDLK_f])
      {
          if (maiuscula)
            c[0] = 'F';
          else
            c[0] = 'f';
      }
      else 
      if( teclas[SDLK_g])
      {
          if (maiuscula)
            c[0] = 'G';
          else
            c[0] = 'g';
      }
      else 
      if( teclas[SDLK_h])
      {
          if (maiuscula)
            c[0] = 'H';
          else
            c[0] = 'h';
      }
      else 
      if( teclas[SDLK_i])
      {
          if (maiuscula)
            c[0] = 'I';
          else
            c[0] = 'i';
      }
      else 
      if( teclas[SDLK_j])
      {
          if (maiuscula)
            c[0] = 'J';
          else
            c[0] = 'j';
      }
      else 
      if( teclas[SDLK_k])
      {
          if (maiuscula)
            c[0] = 'K';
          else
            c[0] = 'k';
      }
      else 
      if( teclas[SDLK_l])
      {
          if (maiuscula)
            c[0] = 'L';
          else
            c[0] = 'l';
      }
      else 
      if( teclas[SDLK_m])
      {
          if (maiuscula)
            c[0] = 'M';
          else
            c[0] = 'm';
      }
      else 
      if( teclas[SDLK_n])
      {
          if (maiuscula)
            c[0] = 'N';
          else
            c[0] = 'n';
      }
      else 
      if( teclas[SDLK_o])
      {
          if (maiuscula)
            c[0] = 'O';
          else
            c[0] = 'o';
      }
      else 
      if( teclas[SDLK_p])
      {
          if (maiuscula)
            c[0] = 'P';
          else
            c[0] = 'p';
      }
      else 
      if( teclas[SDLK_q])
      {
          if (maiuscula)
            c[0] = 'Q';
          else
            c[0] = 'q';
      }
      else 
      if( teclas[SDLK_r])
      {
          if (maiuscula)
            c[0] = 'R';
          else
            c[0] = 'r';
      }
      else 
      if( teclas[SDLK_s])
      {
          if (maiuscula)
            c[0] = 'S';
          else
            c[0] = 's';
      }
      else 
      if( teclas[SDLK_t])
      {
          if (maiuscula)
            c[0] = 'T';
          else
            c[0] = 't';
      }
      else 
      if( teclas[SDLK_u])
      {
          if (maiuscula)
            c[0] = 'U';
          else
            c[0] = 'u';
      }
      else 
      if( teclas[SDLK_v])
      {
          if (maiuscula)
            c[0] = 'V';
          else
            c[0] = 'v';
      }
      else 
      if( teclas[SDLK_w])
      {
          if (maiuscula)
            c[0] = 'W';
          else
            c[0] = 'w';
      }
      else 
      if( teclas[SDLK_x])
      {
          if (maiuscula)
            c[0] = 'X';
          else
            c[0] = 'x';
      }
      else 
      if( teclas[SDLK_y])
      {
          if (maiuscula)
            c[0] = 'Y';
          else
            c[0] = 'y';
      }
      else 
      if( teclas[SDLK_z])
      {
          if (maiuscula)
            c[0] = 'Z';
          else
            c[0] = 'z';
      }
      else 
      if( teclas[SDLK_SLASH])
      {
          if (maiuscula)
            c[0] = '?';
          else
            c[0] = '/';
      }
      else 
      if( teclas[SDLK_BACKSLASH])
      {
          if (maiuscula)
            c[0] = '?';
          else
            c[0] = '\\';
      }
      else 
      if( teclas[SDLK_SEMICOLON])
      {
          if (maiuscula)
            c[0] = ':';
          else
            c[0] = ';';
      }
      else 
      if( teclas[SDLK_0])
      {
          if (maiuscula)
            c[0] = ')';
          else
            c[0] = '0';
      }
      else 
      if( teclas[SDLK_1])
      {
          if (maiuscula)
            c[0] = '!';
          else
            c[0] = '1';
      }
      else 
      if( teclas[SDLK_2])
      {
          if (maiuscula)
            c[0] = '@';
          else
            c[0] = '2';
      }
      else 
      if( teclas[SDLK_3])
      {
          if (maiuscula)
            c[0] = '#';
          else
            c[0] = '3';
      }
      else 
      if( teclas[SDLK_4])
      {
          if (maiuscula)
            c[0] = '$';
          else
            c[0] = '4';
      }
      else 
      if( teclas[SDLK_5])
      {
          if (maiuscula)
            c[0] = '%';
          else
            c[0] = '5';
      }
      else 
      if( teclas[SDLK_6])
      {
          if (maiuscula)
            c[0] = '"';
          else
            c[0] = '6';
      }
      else 
      if( teclas[SDLK_7])
      {
          if (maiuscula)
            c[0] = '&';
          else
            c[0] = '7';
      }
      else 
      if( teclas[SDLK_8])
      {
          if (maiuscula)
            c[0] = '*';
          else
            c[0] = '8';
      }
      else 
      if( teclas[SDLK_9])
      {
          if (maiuscula)
            c[0] = '(';
          else
            c[0] = '9';
      }
      else 
      if( teclas[SDLK_BACKQUOTE])
      {
          if (maiuscula)
            c[0] = '"';
          else
            c[0] = '\'';
      }
      else 
      if( teclas[SDLK_MINUS])
      {
          if (maiuscula)
            c[0] = '_';
          else
            c[0] = '-';
      }
      else 
      if( teclas[SDLK_PERIOD])
      {
          if (maiuscula)
            c[0] = '>';
          else
            c[0] = '.';
      }
      else 
      if( teclas[SDLK_COMMA])
      {
          if (maiuscula)
            c[0] = '<';
          else
            c[0] = ',';
      }
      else 
      if( teclas[SDLK_SPACE])
      {
          if (maiuscula)
            c[0] = ' ';
          else
            c[0] = ' ';
      }      
      if( (((ult - ultEsc) >= 100) || (ultChar!=c[0])) && 
          (c[0]!=-1))
      {
        ultChar = c[0];
        ultEsc = ult;
        texto.insert(pos,c);
        pos++;
      }
       
   }

   if(Mbotao & SDL_BUTTON(1))
   {
       if(mouse_NaArea(Xjan+x1,Yjan+y1,Xjan+x2,Yjan+y2,mouseX,mouseY))
       {
           /* Calcula a nova posicao */
           pos=(mouseX -(Xjan+x1+2))/(fonte_incCP()+1) + inic;
           if (pos>texto.length()) pos = texto.length();
               PoeTexto(0,0,1,pos,1,screen);
       }
       else
          pronto = 1;
  }

  if(!pronto)
    PoeTexto(0,0,1,pos,1,screen);
  else
    PoeTexto(0,0,1,0,0,screen);

  return(pronto);
   //PoeTexto(Xjan,Yjan,1,0,0,screen);
   //SDL_Delay(150);
}



