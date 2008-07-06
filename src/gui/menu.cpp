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
menu::menu(int xa, int ya): guiList()
{
   x = xa;
   y = ya;
   actualItem = 1;
   maxCharac = 0;
   numPictures = 0;
   pressed = false;
}

/*********************************************************
 *                      insertItem                       *
 *********************************************************/
void menu::insertItem(string text, bool avaible)
{
   insertItem(text, "", avaible);
} 

/*********************************************************
 *                      insertItem                       *
 *********************************************************/
void menu::insertItem(string text, string imageFile, bool avaible)
{
   /* Insert Text Box */
   textBox* novo;
   novo = insertTextBox(x,y,x+200,y+200,0,text);
   novo->setAvaible(avaible);
   if(text.length() > maxCharac)
   {
      maxCharac = text.length();
   }

   /* Insert Picture */
   if(!imageFile.empty())
   {
      insertPicture(x,y,0,0, imageFile.c_str());
      numPictures++;
   }
}

/*********************************************************
 *                        getItem                        *
 *********************************************************/
guiObject* menu::getItem(int i)
{
   guiObject* it= (guiObject*) first->next;
   if(i <= (total-numPictures))
   {
      int aux;
      for(aux=1;((aux < i) && (it));aux++)
      {
         if(it->type == GUI_PICTURE)
         {
            /* Ignore Pictures */
            aux--;
         }
         it = (guiObject*) it->next;
      }
      return(it);  
   }
   return(NULL);
}

/*********************************************************
 *                      itemAvaible                      *
 *********************************************************/
bool menu::itemAvaible(int item)
{
   guiObject* it = getItem(item);
   if(!it)
   {
      return(false);
   }
   return(it->isAvaible());
}

/*********************************************************
 *                     setItemAvaible                    *
 *********************************************************/
void menu::setItemAvaible(int item, bool avaible)
{
   guiObject* it = getItem(item);
   if(it)
   {
      it->setAvaible(avaible);
   }
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
   dntFont fnt;
   fnt.defineFont(DNT_FONT_ARIAL, 10);
   fnt.defineFontAlign(DNT_FONT_ALIGN_LEFT);
   fnt.defineFontStyle(DNT_FONT_STYLE_NORMAL);

   /* Define Coordinates */
   int x1 = x;
   int x2 = x1 + (maxCharac)*(fnt.getIncCP()+1)+4; 
   int y1 = y;
   int y2 = ((total-numPictures)*11) + y1 + 5; /* bizarre from DOS version */
   
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
   color_Set(Colors.colorMenu.R, Colors.colorMenu.G,
             Colors.colorMenu.B, Colors.colorMenu.A);
   rectangle_Fill(screen,x1+1,y1+1,x2-1,y2-1);
   color_Set(Colors.colorCont[2].R, Colors.colorCont[2].G,
             Colors.colorCont[2].B, Colors.colorCont[2].A);
   rectangle_Oval(screen,x1,y1,x2,y2,Colors.colorCont[1].R,
                  Colors.colorCont[1].G, Colors.colorCont[1].B,
                  Colors.colorCont[1].A);
   
      /*itens*/  
   int xa = x1+4;
   int ya = y1+3;
   int k;
   guiObject* item = (guiObject*) first->next;
   for (k=0; k < total; k++)
   {
      /* Treat Pictures */
      if(item->type == GUI_PICTURE)
      {
         picture* pic = (picture*)item;
         pic->setCoordinate(xa, ya, xa+10, ya+10);
         pic->draw(screen);
         /* The next text will be translated right */
         xa = x1+15;
      }
      /* treat Texts */
      else if(item->type == GUI_TEXT_BOX)
      {
         color_Set(Colors.colorText.R,
               Colors.colorText.G,
               Colors.colorText.B,
               Colors.colorText.A);

         /* Menu Texts */
         if (item->getText().compare("-"))
         {
            if (item->isAvaible()) 
            {
               fnt.write(screen,xa,ya,item->getText());
            }
            else
            {
               color_Set(Colors.colorCont[2].R, Colors.colorCont[2].G,
                     Colors.colorCont[2].B, Colors.colorCont[2].A);
               fnt.write(screen,xa+1,ya+1,item->getText());
               color_Set(Colors.colorCont[1].R, Colors.colorCont[1].G,
                     Colors.colorCont[1].B, Colors.colorCont[1].A);
               fnt.write(screen,xa,ya,item->getText());
            }
         } 

         /* Menu Separators */
         else 
         {
            color_Set(Colors.colorCont[1].R, Colors.colorCont[1].G,
                  Colors.colorCont[1].B, Colors.colorCont[1].A);
            rectangle_2Colors(screen,xa-2,ya+6,x2-2,ya+7,Colors.colorCont[0].R,
                  Colors.colorCont[0].G,Colors.colorCont[0].B,
                  Colors.colorCont[0].A);
         }
         xa = x1+4;
         ya += 11;
      }
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
   dntFont fnt;
   fnt.defineFont(DNT_FONT_ARIAL, 10);

   /* Draws */
   draw(0,screen);
   int altura = ((total-numPictures)*11)+6;
   int largura = (maxCharac)*(fnt.getIncCP()+1)+5;

   /* Runs */
   *pronto = 0;
   int tecla = 0;

      /* Verify Mouse Moviments */
      if(isMouseAt(x+Xjan, y+Yjan, x+largura+Xjan,
                   y+altura+Yjan-3, mouseX, mouseY)) 
      {
         actualItem = ((mouseY - (y+Yjan)-4) / 11) + 1;
         if(actualItem < 0)
         {
            actualItem = 0;
         }
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

      if(actualItem > 0)
      {
         color_Set(Colors.colorCont[1].R,
                   Colors.colorCont[1].G,
                   Colors.colorCont[1].B,
                   Colors.colorCont[1].A);
         rectangle_Oval(screen,x+2,(actualItem-1)*11+y+4,
                        x+largura-2,(actualItem)*11+y+4,
                        Colors.colorCont[2].R, Colors.colorCont[2].G,
                        Colors.colorCont[2].B, Colors.colorCont[2].A);
      }

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



