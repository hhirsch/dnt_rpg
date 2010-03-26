/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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

#include "button.h"
#include "draw.h"
#include "window.h"
#include "dntfont.h"

/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
button::button(int xa,int ya,int xb,int yb, string txt, bool isOval, 
      SDL_Surface* surface): guiObject(surface)
{
   x1 = xa; 
   y1 = ya;
   x2 = xb;
   y2 = yb;
   text = txt;
   oval = isOval;
   men = NULL;
   pressed = false;
   available = true;
   defineFont(DNT_FONT_ARIAL, 12);
   type = FARSO_OBJECT_BUTTON;
}

/***********************************************************
 *                        Destructor                       *
 ***********************************************************/
button::~button()
{
}

/***********************************************************
 *                      setTextColor                       *
 ***********************************************************/
void button::setTextColor(int r, int g, int b)
{
   Colors.colorText.R = r;
   Colors.colorText.G = g;
   Colors.colorText.B = b;
}

/***********************************************************
 *                           draw                          *
 ***********************************************************/
void button::draw()
{
   dntFont font;
   int R1,R2,G1,G2,B1,B2,A1,A2; 
   if(pressed) 
   {
      R1 = Colors.colorCont[1].R;
      G1 = Colors.colorCont[1].G;
      B1 = Colors.colorCont[1].B;
      A1 = Colors.colorCont[1].A;
      R2 = Colors.colorCont[0].R;
      G2 = Colors.colorCont[0].G;
      B2 = Colors.colorCont[0].B;
      A2 = Colors.colorCont[0].A;
   }
   else
   {
      R2 = Colors.colorCont[1].R;
      G2 = Colors.colorCont[1].G;
      B2 = Colors.colorCont[1].B;
      A2 = Colors.colorCont[1].A;
      R1 = Colors.colorCont[0].R;
      G1 = Colors.colorCont[0].G;
      B1 = Colors.colorCont[0].B; 
      A1 = Colors.colorCont[0].A;
   }
   
   color_Set(Colors.colorButton.R, 
             Colors.colorButton.G,
             Colors.colorButton.B,
             Colors.colorButton.A);
   rectangle_Fill(wSurface,x1+1,y1+1,x2-1,y2-1);
   color_Set(R1,G1,B1,A1);
   if(oval)
   {
      rectangle_Oval(wSurface,x1,y1,x2,y2,R2,B2,G2,A2);
   }
   else
   {
      rectangle_2Colors(wSurface,x1,y1,x2,y2,R2,B2,G2,A2);
   }

   /* Write the Text */
   color_Set(Colors.colorText.R, Colors.colorText.G,
             Colors.colorText.B, Colors.colorText.A);
   int ya=y1;
   int xa=x1;
   if(pressed) 
   {
     ya++;
     xa+=2;
   }
   font.defineFont(fontName, fontSize);
   font.defineFontAlign(DNT_FONT_ALIGN_CENTER);
   font.defineFontStyle(DNT_FONT_STYLE_NORMAL);

   //xa = ((xa+x2) /2);
   
   if( (text == font.createUnicode(0x25B2)) ||
       (text == font.createUnicode(0x25BC)) )
   {
      font.write(wSurface,xa+1,ya,getText().c_str(),xa+1,y1,x2+1,y2);
   }
   else if(text == font.createUnicode(0x25CF))
   {
      font.write(wSurface,xa+2,ya-2,getText().c_str(),xa+2,y1,x2+2,y2);
   }
   else if(text == "-")
   {
      font.write(wSurface,xa+2,ya-2,getText().c_str(),xa+2,y1,x2+2,y2);
   }
   else
   {
      if(isAvailable())
      {
         font.write(wSurface,xa,ya+3,getText().c_str(),xa,y1,x2,y2);
      }
      else
      {
         color_Set(Colors.colorCont[2].R, Colors.colorCont[2].G,
                   Colors.colorCont[2].B, Colors.colorCont[2].A);
         font.write(wSurface,xa+1,ya+4, getText(),xa,y1,x2,y2);
         color_Set(Colors.colorCont[1].R, Colors.colorCont[1].G,
                   Colors.colorCont[1].B, Colors.colorCont[1].A);
         font.write(wSurface,xa,ya+3, getText(), xa,y1,x2,y2);
      }
   }
   setChanged();
}

/***********************************************************
 *                           press                         *
 ***********************************************************/
bool button::press(int Xjan, int Yjan, int x, int y, Uint8 Mbotao, int* pronto)
{
   *pronto = 0;
   bool pres;

   pres = (isMouseIn(x-Xjan,y-Yjan));

   /* Verify if the mouse Button is left */
   *pronto = !(Mbotao & SDL_BUTTON(1));

   /* Verify if Must Redraw */
   if( (pres && (Mbotao & SDL_BUTTON(1))) != pressed)
   {
      pressed = pres && (Mbotao & SDL_BUTTON(1));
      draw();
   }

   return(pres && isAvailable());
}

/***********************************************************
 *                        defineFont                       *
 ***********************************************************/
void button::defineFont(string fileName, int size)
{
   fontName = fileName;
   fontSize = size;
}

