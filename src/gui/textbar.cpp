/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "textbar.h"

#define REFRESH_RATE 170

/********************************************************************
 *                          Constructor                             *
 ********************************************************************/
textBar::textBar(int xa,int ya,int xb,int yb, string text1, bool cripto)
{
   lastWrite = 0;
   x1 = xa;
   x2 = xb;
   y1 = ya;
   y2 = yb;
   init = 0;
   end = 0;
   pos = 0;
   lastChar='\0';
   cript = cripto;
   type = GUI_TEXT_BAR;
   text = text1;
}

/********************************************************************
 *                          Constructor                             *
 ********************************************************************/
textBar::~textBar()
{
}

/********************************************************************
 *                            putText                               *
 ********************************************************************/
void textBar::putText(unsigned int pos, int marca, SDL_Surface *screen)
{
   defineFont(FFARSO,ALIGN_LEFT,1);
   unsigned int maxCarac = ((x2-3)-(x1+3)) / (font_incCP()+1);
   color_Set(Colors.colorCont[2].R,Colors.colorCont[2].G,
               Colors.colorCont[2].B);
   rectangle_Fill(screen,x1+1, y1+1, x2-1, y2-1);
   color_Set(Colors.colorCont[1].R,Colors.colorCont[1].G,
               Colors.colorCont[1].B);
   init = 0;
   if(pos+1>maxCarac) 
   {
      init = pos-maxCarac;
      if (init < 0)
      {
         init = 0;
      }
      end = pos-1;
   }
   else if (text.length()-1 >= maxCarac)
   {
      end = init + maxCarac-1;
   }
   else
   {
     end = text.length()-1;
   }

   write(screen, x1+3, y1-2, text.c_str(), init, end);
   int x = (x1+2)+ ((pos-init)*(font_incCP()));
   if (marca)
   {
      line_Draw(screen,x,y1+3,x,y2-3);
   }
}

/********************************************************************
 *                              draw                                *
 ********************************************************************/
void textBar::draw(SDL_Surface *screen)
{
   color_Set(Colors.colorCont[0].R,Colors.colorCont[0].G,
               Colors.colorCont[0].B);
   rectangle_2Colors(screen,x1,y1,x2,y2, Colors.colorCont[1].R,
                     Colors.colorCont[1].G,Colors.colorCont[1].B);
   putText(0,0,screen);
}

/********************************************************************
 *                    defineCursorPosition                          *
 ********************************************************************/
void textBar::defineCursorPosition(int mouseX, int mouseY)
{
   pos = (mouseX-(x1+2)) / (font_incCP());
   if(pos > text.length()) 
   {
      pos = text.length();
   }
}

/********************************************************************
 *                             Write                                *
 ********************************************************************/
int textBar::doWrite(int mouseX, int mouseY, SDL_Surface *screen, 
                     Uint8 Mbotao, Uint8* teclas)
{
   string c;
   c = "";
   int pronto = 0;
   int ult = SDL_GetTicks();
   int maiuscula = (teclas[SDLK_CAPSLOCK] || teclas[SDLK_LSHIFT] || 
                    teclas[SDLK_RSHIFT]);

   /* End Edition Keys */
   if(teclas[SDLK_ESCAPE] || teclas[SDLK_RETURN] || teclas[SDLK_KP_ENTER])
   {
      pronto = 1;
   }

   /* Cursor Movimentation Keys */
   else
   if ( (teclas[SDLK_HOME]) && ((ult - lastWrite) >= REFRESH_RATE))
   {
       pos = 0;
       lastWrite = ult;
   }
   else
   if ( (teclas[SDLK_END]) && ((ult - lastWrite) >= REFRESH_RATE))
   {
       pos = text.length();
       lastWrite = ult;
   }
   else
   if ((teclas[SDLK_RIGHT]) && ((ult - lastWrite) >= REFRESH_RATE))
   {
       if(pos<text.length()) pos++; 
       lastWrite = ult;
   }
   else
   if ((teclas[SDLK_LEFT]) && ((ult - lastWrite) >= REFRESH_RATE))
   {
       if(pos>0) pos--;
       lastWrite = ult;
   }
   else
      /* Delete Keys */
   if( (teclas[SDLK_DELETE]) && ((ult - lastWrite) >= REFRESH_RATE))
   {
       if(pos<text.length())
       {   
          text.erase(pos,1);
       }
       lastWrite = ult;
   }
   else
      /* Input Keys */
   if((teclas[SDLK_BACKSPACE]) && ((ult - lastWrite) >= REFRESH_RATE))
   {
      if(pos>0)
      {
         pos--;
         text.erase(pos,1);
      }
      lastWrite = ult;
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
      if( teclas[SDLK_v])
      {
          if (maiuscula)
            c = 'V';
          else
            c = 'v';
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
            c = '?';
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
            c = '&';
          else
            c = '7';
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
      if( (((ult - lastWrite) >= REFRESH_RATE) || (lastChar!=c)) && 
          (c!=""))
      {
        lastChar = c;
        lastWrite = ult;
        text.insert(pos,c.c_str());
        pos++;
      }
       
   }

   if(Mbotao & SDL_BUTTON(1))
   {
       if(isMouseIn(mouseX,mouseY))
       {
           /* Calculate the New Position */
           defineCursorPosition(mouseX, mouseY);
           putText(pos,1,screen);
       }
       else
       {
          pronto = 1;
       }
  }

  if(!pronto)
  {
    putText(pos,1,screen);
  }
  else
  {
    putText(0,0,screen);
  }

  return(pronto);
}



