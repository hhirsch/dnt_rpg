#include "tabbotao.h"
#include "draw.h"
#include "mouse.h"

tabButton::tabButton(int x,int y,const char* arquivo):figura(x,y,0,0,arquivo)
{
   numButtons = 0;
   pressed = false;
}

tabButton::tabButton(int x, int y, int w, int h):figura(x,y,0,0, NULL)
{
   numButtons = 0;
   x1 = x;
   y1 = y;
   x2 = x+w;
   y2 = y+h;
   pressed = false;
}

oneTabButton* tabButton::insertButton(int x1, int y1, int x2, int y2)
{
   if(numButtons < MAX_TABBUTTONS)
   {
      Buttons[numButtons].x1 = x1;
      Buttons[numButtons].y1 = y1;
      Buttons[numButtons].x2 = x2;
      Buttons[numButtons].y2 = y2;
      numButtons++;
      return(&Buttons[numButtons-1]);
   }
   return(NULL);
}

void tabButton::draw(int mouseX, int mouseY, 
                     int Xjan,int Yjan,SDL_Surface *screen)
{ 
   int i;
   figura* fig;
   fig = this;
   fig->Desenhar(0, 0, 0,  screen);
   for(i=0;i<numButtons;i++)
   {
      if(mouse_NaArea(Xjan + x1 + Buttons[i].x1, Yjan + y1 + Buttons[i].y1,
                      Xjan + x1 + Buttons[i].x2, Yjan + y1 + Buttons[i].y2,
                      mouseX, mouseY))
      {
          color_Set(cor.colorCont[0].R,cor.colorCont[0].G,cor.colorCont[0].B);
          rectangle_2Colors(screen,x1+Buttons[i].x1,y1+Buttons[i].y1,
                            x1+Buttons[i].x2,y1+Buttons[i].y2,
                            cor.colorCont[1].R,cor.colorCont[1].G,
                            cor.colorCont[1].B);
      }
      else if(fig->fig == NULL)
      {
         color_Set(cor.colorWindow.R, cor.colorWindow.G, cor.colorWindow.B);
         rectangle_Draw(screen, x1+Buttons[i].x1,y1+Buttons[i].y1,
                        x1+Buttons[i].x2,y1+Buttons[i].y2);
      }
   }
}

guiObject* tabButton::verifyPosition(int mouseX, int mouseY, Uint8 Mbuttons, 
                                   int Xjan, int Yjan, SDL_Surface *screen,
                                   int &actionType)
{
   int i;
   actionType = TABBUTTON_NONE;
   draw(mouseX, mouseY, Xjan, Yjan, screen);
   for(i=0;i<numButtons;i++)
   {
      if(mouse_NaArea(Xjan + x1 + Buttons[i].x1, Yjan + y1 + Buttons[i].y1,
                      Xjan + x1 + Buttons[i].x2, Yjan + y1 + Buttons[i].y2,
                      mouseX, mouseY))
      {
         if(Mbuttons & SDL_BUTTON(1))
         {
            /* Only return when released the mouse button! */
            actionType = TABBUTTON_ON_PRESS;
            pressed = true;  
            return((guiObject*) &Buttons[i]);
         }
         else
         {
            if(pressed)
            {
               actionType = TABBUTTON_PRESSED;
               pressed = false;
               return((guiObject*) &Buttons[i]);
            }
            else
            {
               return(NULL);
            }
         }
      }
   }
   return(NULL);
}

