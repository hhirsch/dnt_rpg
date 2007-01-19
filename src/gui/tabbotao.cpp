#include "tabbotao.h"
#include "desenho.h"
#include "mouse.h"

tabButton::tabButton(int x,int y,const char* arquivo):figura(x,y,0,0,arquivo)
{
   numButtons = 0;
}

tabButton::tabButton(int x, int y, int w, int h):figura(x,y,w,h, NULL)
{
   numButtons = 0;
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
          cor_Definir(248,2,11);
          retangulo_2Cores(screen,x1+Buttons[i].x1,y1+Buttons[i].y1,
                           x1+Buttons[i].x2,y1+Buttons[i].y2,
                           86,1,4,0);
      }
   }
}

Tobjeto* tabButton::verifyPosition(int mouseX, int mouseY, Uint8 Mbuttons, 
                               int Xjan, int Yjan, SDL_Surface *screen)
{
   int i;
   draw(mouseX, mouseY, Xjan, Yjan, screen);
   for(i=0;i<numButtons;i++)
   {
      if(mouse_NaArea(Xjan + x1 + Buttons[i].x1, Yjan + y1 + Buttons[i].y1,
                      Xjan + x1 + Buttons[i].x2, Yjan + y1 + Buttons[i].y2,
                      mouseX, mouseY))
      {
         if(Mbuttons & SDL_BUTTON(1))
         {
            return((Tobjeto*) &Buttons[i]);
         }
         else
         {
            return(NULL);
         }
      }
   }
   return(NULL);
}

