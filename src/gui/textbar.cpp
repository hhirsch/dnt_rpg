/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "textbar.h"
#include "farsoopts.h"
using namespace std;
using namespace Farso;

#define REFRESH_RATE 170

/********************************************************************
 *                          Constructor                             *
 ********************************************************************/
TextBar::TextBar(int xa,int ya,int xb,int yb, string text1, bool cripto,
                 SDL_Surface* screen)
        :GuiObject(screen)
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
   type = Farso::OBJECT_TEXT_BAR;
   text = text1;
}

/********************************************************************
 *                          Constructor                             *
 ********************************************************************/
TextBar::~TextBar()
{
}

/********************************************************************
 *                            putText                               *
 ********************************************************************/
void TextBar::putText(unsigned int pos, int marca)
{
   Options opt;
   Font fnt;
   fnt.defineFont(opt.getDefaultFont(), 12);
   fnt.defineFontAlign(Font::ALIGN_LEFT);
   fnt.defineFontStyle(Font::STYLE_NORMAL);

   if(available)
   {
      color_Set(colors.colorCont[2].R, colors.colorCont[2].G,
                colors.colorCont[2].B, colors.colorCont[2].A);
   }
   else
   {
      color_Set(colors.colorCont[0].R, colors.colorCont[0].G,
                colors.colorCont[0].B, colors.colorCont[0].A);
   }
   rectangle_Fill(wSurface,x1+1, y1+1, x2-1, y2-1);

   if(available)
   {
      color_Set(colors.colorCont[1].R, colors.colorCont[1].G,
            colors.colorCont[1].B, colors.colorCont[1].A);
   }
   else
   {
      color_Set(colors.colorText.R, colors.colorText.G,
            colors.colorText.B, colors.colorText.A);
   }

   init = 0;
   end = text.length();
   string writeText = text;

   int maxWidth = ((x2-1) - (x1+3));

   while( fnt.getStringWidth(writeText) > maxWidth )
   {
      if(end > (int)pos)
      {
         end--;
         writeText.erase(writeText.length()-1);
      }
      else
      {
         init++;
         writeText.erase(0,1);
      }
   }

   fnt.write(wSurface, x1+3, y1+1, writeText);

   if (marca)
   {
      /* Define mark position */
      string s = text.substr(init,init+pos);
      int x = x1 + 2 + fnt.getStringWidth(s);
      line_Draw(wSurface,x,y1+3,x,y2-3);
   }
   setChanged();
}

/********************************************************************
 *                              setText                             *
 ********************************************************************/
void TextBar::setText(string txt)
{
   text = txt;
   draw();
}

/********************************************************************
 *                              draw                                *
 ********************************************************************/
void TextBar::draw()
{
   /* No draw when hidden */
   if(!isVisible())
   {
      return;
   }
   
   color_Set(colors.colorCont[0].R, colors.colorCont[0].G,
             colors.colorCont[0].B, colors.colorCont[0].A);
   rectangle_2Colors(wSurface,x1,y1,x2,y2, colors.colorCont[1].R,
                     colors.colorCont[1].G,colors.colorCont[1].B,
                     colors.colorCont[1].A);
   putText(0,0);
   setChanged();
}

/********************************************************************
 *                    defineCursorPosition                          *
 ********************************************************************/
void TextBar::defineCursorPosition(int mouseX, int mouseY)
{
   Options opt;
   Font fnt;
   fnt.defineFont(opt.getDefaultFont(),12);
   fnt.defineFontAlign(Font::ALIGN_LEFT);
   fnt.defineFontStyle(Font::STYLE_NORMAL);
   string s = text.substr(init,end);

   string saux = "";
   pos = init;
   int i = 0;
   int xPos = mouseX-(x1+2);

   while( (i < (int)s.length()) && (fnt.getStringWidth(saux) < xPos) )
   {
      pos++;
      saux += s[i];
      i++;
   }
}

/********************************************************************
 *                             Write                                *
 ********************************************************************/
int TextBar::doWrite(int mouseX, int mouseY, Uint8 Mbotao, Uint8* teclas)
{
   string c;
   c = "";
   int pronto = 0;
   int ult = SDL_GetTicks();
   int maiuscula = (teclas[SDLK_CAPSLOCK] || teclas[SDLK_LSHIFT] || 
                    teclas[SDLK_RSHIFT]);

   if(!available)
   {
      return(1);
   }

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
      if( (teclas[SDLK_0]) || (teclas[SDLK_KP0]) )
      {
          if (maiuscula)
            c = ')';
          else
            c = '0';
      }
      else 
      if( (teclas[SDLK_1])  || (teclas[SDLK_KP1]) )
      {
          if (maiuscula)
            c = '!';
          else
            c = '1';
      }
      else 
      if( (teclas[SDLK_2])  || (teclas[SDLK_KP2]) )
      {
          if (maiuscula)
            c = '@';
          else
            c = '2';
      }
      else 
      if( (teclas[SDLK_3])  || (teclas[SDLK_KP3]) )
      {
          if (maiuscula)
            c = '#';
          else
            c = '3';
      }
      else 
      if( (teclas[SDLK_4])  || (teclas[SDLK_KP4]) )
      {
          if (maiuscula)
            c = '$';
          else
            c = '4';
      }
      else 
      if( (teclas[SDLK_5])  || (teclas[SDLK_KP5]) )
      {
          if (maiuscula)
            c = '%';
          else
            c = '5';
      }
      else 
      if( (teclas[SDLK_6])  || (teclas[SDLK_KP6]) )
      {
          if (maiuscula)
            c = '"';
          else
            c = '6';
      }
      else 
      if( (teclas[SDLK_7])  || (teclas[SDLK_KP7]) )
      {
          if (maiuscula)
            c = '&';
          else
            c = '7';
      }
      else 
      if( (teclas[SDLK_8])  || (teclas[SDLK_KP8]) )
      {
          if (maiuscula)
            c = '*';
          else
            c = '8';
      }
      else 
      if( (teclas[SDLK_9])  || (teclas[SDLK_KP9]) )
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
           putText(pos,1);
       }
       else
       {
          pronto = 1;
       }
  }

  if(!pronto)
  {
    putText(pos,1);
  }
  else
  {
    putText(0,0);
  }

  return(pronto);
}



