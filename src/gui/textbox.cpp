/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "textbox.h"

/*******************************************************
 *                       Constructor                   *
 *******************************************************/
textBox::textBox(int xa, int ya, int xb, int yb, int frameType)
{
   type = GUI_TEXT_BOX;
   x1 = xa;
   y1 = ya;
   x2 = xb;
   y2 = yb;
   framed = frameType;
}

/*******************************************************
 *                        Destructor                   *
 *******************************************************/
textBox::~textBox()
{
}

/*******************************************************
 *                          draw                       *
 *******************************************************/
void textBox::draw(SDL_Surface *screen)
{
   if(framed)
   {
      if(framed == 1)
      {
         color_Set(Colors.colorButton.R,
                   Colors.colorButton.G,
                   Colors.colorButton.B);
         rectangle_Fill(screen,x1+1,y1+1,x2-1,y2-1);
      }
      color_Set(Colors.colorCont[0].R,Colors.colorCont[0].G,
                Colors.colorCont[0].B);
      rectangle_2Colors(screen,x1,y1,x2,y2,
                       Colors.colorCont[1].R,Colors.colorCont[1].G,
                       Colors.colorCont[1].B);
   }
   color_Set(Colors.colorText.R,Colors.colorText.G,Colors.colorText.B);
   defineFont(fontName.c_str(),fontAlign,fontSize);
   if(fontAlign == ALIGN_LEFT)
   {
      write(screen, x1+5, y1, text.c_str(), x1+5, y1, x2, y2);
   }
   else if(fontAlign == ALIGN_CENTER)
   {
      write(screen, ((x2-x1-5) / 2)+x1+5, y1, text.c_str(), x1+5, y1, x2, y2);
   }
   else
   {
      write(screen, x2, y1, text.c_str(), x1+5, y1, x2, y2);
   }
}

/*******************************************************
 *                        setFont                      *
 *******************************************************/
void textBox::setFont(string name, int size, int align)
{
   fontName = name;
   fontAlign = align;
   fontSize = size;
}

/*******************************************************
 *                       setColor                      *
 *******************************************************/
void textBox::setColor(int R, int G, int B)
{
   Colors.colorText.R = R;
   Colors.colorText.G = G;
   Colors.colorText.B = B;
}

/*******************************************************
 *                     setBackColor                    *
 *******************************************************/
void textBox::setBackColor(int R, int G, int B)
{
   Colors.colorButton.R = R;
   Colors.colorButton.G = G;
   Colors.colorButton.B = B;
}


