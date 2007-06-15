/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "menu.h"

/*********************************************************
 *                       Destructor                      *
 *********************************************************/
menu::~menu()
{
}

/*********************************************************
 *                      Constructor                      *
 *********************************************************/
menu::menu(int xa, int ya): Tlista()
{
   x = xa;
   y = ya;
   actualItem = 1;
   maxCharac = 0;
   pressed = false;
}

/*********************************************************
 *                      insertItem                       *
 *********************************************************/
void menu::insertItem(string text, bool avaible)
{
   guiObject* novo;
   novo = new guiObject;
   novo->setText(text);
   novo->setAvaible(avaible);
   if(text.length() > maxCharac)
   {
      maxCharac = text.length();
   }
   novo->type = GUI_MENU_ITEM;
   InserirObj(novo);
} 

/*********************************************************
 *                        getItem                        *
 *********************************************************/
guiObject* menu::getItem(int i)
{
   guiObject* it= (guiObject*) first->next;
   if(i<=total)
   {
      int aux;
      for(aux=1;aux<i;aux++)
         it = (guiObject*) it->next;
      return(it);  
   }
   return(NULL);
}

/*********************************************************
 *                      Constructor                      *
 *********************************************************/
bool menu::itemAvaible(int item)
{
   guiObject* it = getItem(item);
   if(!it)
   {
      return(0);
   }
   return(it->isAvaible());
}

/*********************************************************
 *                      setCoordinate                    *
 *********************************************************/
void menu::setPosition(int xa, int ya)
{
  x = xa;
  y = ya;
}

/*********************************************************
 *                          draw                         *
 *********************************************************/
void menu::draw(int pos, SDL_Surface *screen)
{
   selFonte(FFARSO,ESQUERDA,1);

   /* Define Coordinates */
   int x1 = x;
   int x2 = x1 + (maxCharac)*(fonte_incCP()+1)+4; 
   int y1 = y;
   int y2 = (total*11) + y1  + 5; /* bizarre thing from DOS version */
   
   /* Verify Sides */
   if (x2 > screen->w-1)
   {
      x1 = (screen->w-1) - (x2 - x1);
      x2 = screen->w-1;
   }
   if (y2 > screen->h-1)
   {
      y1 = (screen->h-1) - (y2 - y1);
      y2 = screen->h-1;
   }

   /* Draw the Menu */
   color_Set(Colors.colorMenu.R,Colors.colorMenu.G,Colors.colorMenu.B);
   rectangle_Fill(screen,x1+1,y1+1,x2-1,y2-1);
   color_Set(Colors.colorCont[2].R,Colors.colorCont[2].G,Colors.colorCont[2].B);
   rectangle_Oval(screen,x1,y1,x2,y2,Colors.colorCont[1].R,Colors.colorCont[1].G,
                  Colors.colorCont[1].B);
   
      /*itens*/  
   int xa = x1+4;
   int ya = y1+3;
   int k;
   guiObject* item = (guiObject*) first->next;
   for (k=0;k<total;k++)
   {
      color_Set(Colors.colorCont[1].R,
                Colors.colorCont[1].G,
                Colors.colorCont[1].B);
      if (item->getText().compare("-"))
      {
          if (item->isAvaible()) 
          {
            escxy(screen,x1+4,ya-3,item->getText().c_str());
          }
          else
          {
              color_Set(Colors.colorCont[2].R,Colors.colorCont[2].G,
                          Colors.colorCont[2].B);
              escxy(screen,x1+5,ya-2,item->getText().c_str());
              color_Set(Colors.colorCont[1].R,Colors.colorCont[1].G,
                          Colors.colorCont[1].B);
              escxy(screen,x1+4,ya-3,item->getText().c_str());
          }
      } 
      else 
      {
        color_Set(Colors.colorCont[1].R,Colors.colorCont[1].G,
                  Colors.colorCont[1].B);
        rectangle_2Colors(screen,xa-2,ya+6,x2-2,ya+7,Colors.colorCont[0].R,
                         Colors.colorCont[0].G,Colors.colorCont[0].B);
      }
      ya += 11;
      item = (guiObject*)item->next;
   }
  
}

/*********************************************************
 *                     getActualItem                     *
 *********************************************************/
int menu::getActualItem()
{
   return(actualItem);
}

/*********************************************************
 *                      getMaxCharac                     *
 *********************************************************/
int menu::getMaxCharac()
{
   return(maxCharac);
}


/*********************************************************
 *                          run                          *
 *********************************************************/
int menu::run(int mouseX, int mouseY, Uint8 Mbotao, Uint8* teclado,
              SDL_Surface *screen, int *pronto, int Xjan, int Yjan)
{
   selFonte(FFARSO,ESQUERDA,1);

   /* Draws */
   draw(0,screen);
   int altura = (total*11)+6;
   int largura = (maxCharac)*(fonte_incCP()+1)+5;

   /* Runs */
   *pronto = 0;
   int tecla = 0;

      /* Verify Mouse Moviments */
      if(isMouseAt(x+Xjan, y+Yjan, x+largura+Xjan,
                      y+altura+Yjan-3, mouseX, mouseY)) 
      {
          actualItem = ((mouseY - (y+Yjan)-4) / 11) + 1;
      }
      /* Verify Mouse Button */
      if(Mbotao & SDL_BUTTON(1))
      {
         pressed = true; 
      }
      else if(pressed)
      {
         /* Got the release event! */
         *pronto = 1;
         pressed = false;
      }

      /* Verify Keyboard */
      if(teclado[SDLK_UP] && (actualItem-1 > 0))
      {
         actualItem --;
      }
      else if(teclado[SDLK_DOWN] && (actualItem+1 <= total))
      {
         actualItem++;
      }
      else if( (teclado[SDLK_RETURN] || teclado[SDLK_KP_ENTER]) )
      {
         *pronto = 1;
         tecla = 1;
      }
      else if( teclado[SDLK_ESCAPE])
      {
         *pronto = 1;
         tecla = 0;
      }

      color_Set(Colors.colorCont[1].R,
                Colors.colorCont[1].G,
                Colors.colorCont[1].B);
      rectangle_Oval(screen,x+2,(actualItem-1)*11+y+4,
                     x+largura-2,(actualItem)*11+y+4,
                     Colors.colorCont[2].R,Colors.colorCont[2].G,
                     Colors.colorCont[2].B);

   /* Define the Return */
   if(isMouseAt(x+Xjan, y+Yjan, x+largura+Xjan, y+altura+Yjan-3,
                   mouseX,mouseY) && (!tecla) &&(*pronto))
   {
      if (!itemAvaible(actualItem))
      {
         actualItem = 0;
      }
   }
   else if ( (!tecla) && (*pronto))
   {
      actualItem = 0;
   }

   return(actualItem);
}



