/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "button.h"
#include "draw.h"
#include "window.h"
#include "dntfont.h"

/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
button::button(int xa,int ya,int xb,int yb, string txt, bool isOval )
{
   x1 = xa; 
   y1 = ya;
   x2 = xb;
   y2 = yb;
   text = txt;
   oval = isOval;
   men = NULL;
   pressed = false;
   avaible = true;
   defineFont(DNT_FONT_ARIAL, 12);
   type = GUI_BUTTON;
}

/***********************************************************
 *                        Destructor                       *
 ***********************************************************/
button::~button()
{
}

/***********************************************************
 *                           draw                          *
 ***********************************************************/
void button::draw(SDL_Surface* screen )
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
   rectangle_Fill(screen,x1+1,y1+1,x2-1,y2-1);
   color_Set(R1,G1,B1,A1);
   if(oval)
   {
      rectangle_Oval(screen,x1,y1,x2,y2,R2,B2,G2,A2);
   }
   else
   {
      rectangle_2Colors(screen,x1,y1,x2,y2,R2,B2,G2,A2);
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
      font.write(screen,xa+1,ya,getText().c_str(),xa+1,y1,x2+1,y2);
   }
   else if(text == font.createUnicode(0x25CF))
   {
      font.write(screen,xa+2,ya-2,getText().c_str(),xa+2,y1,x2+2,y2);
   }
   else if(text == "-")
   {
      font.write(screen,xa+2,ya-2,getText().c_str(),xa+2,y1,x2+2,y2);
   }
   else
   {
      if(isAvaible())
      {
         font.write(screen,xa,ya+3,getText().c_str(),xa,y1,x2,y2);
      }
      else
      {
         color_Set(Colors.colorCont[2].R, Colors.colorCont[2].G,
                   Colors.colorCont[2].B, Colors.colorCont[2].A);
         font.write(screen,xa+1,ya+4, getText(),xa,y1,x2,y2);
         color_Set(Colors.colorCont[1].R, Colors.colorCont[1].G,
                   Colors.colorCont[1].B, Colors.colorCont[1].A);
         font.write(screen,xa,ya+3, getText(), xa,y1,x2,y2);
      }
   }
   setChanged();
}

/***********************************************************
 *                           press                         *
 ***********************************************************/
bool button::press(int Xjan, int Yjan, int x, int y, Uint8 Mbotao, int* pronto,
                  SDL_Surface* screen)
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
      draw(screen);
   }

   return(pres && isAvaible());
}

/***********************************************************
 *                        defineFont                       *
 ***********************************************************/
void button::defineFont(string fileName, int size)
{
   fontName = fileName;
   fontSize = size;
}

