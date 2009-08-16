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
menu::menu(int xa, int ya): guiList(DNT_LIST_TYPE_ADD_AT_END)
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
   textBox* novo;

   /* Insert Picture */
   if(!imageFile.empty())
   {
      insertPicture(x,y,0,0, imageFile.c_str());
      numPictures++;
   }

   /* Insert Text Box */
   novo = insertTextBox(x,y,x+200,y+200,0,text);
   novo->setAvailable(avaible);
   if(text.length() > maxCharac)
   {
      maxCharac = text.length();
   }
}

/*********************************************************
 *                        getItem                        *
 *********************************************************/
guiObject* menu::getItem(int i)
{
   guiObject* it= (guiObject*) first;
   if(i <= (total-numPictures))
   {
      while(it->type == FARSO_OBJECT_PICTURE)
      {
         /* Ignore Pictures */
         it = (guiObject*)it->getNext();
      }

      int aux;
      for(aux=1;((aux < i) && (it));aux++)
      {
         it = (guiObject*)it->getNext();
         while(it->type == FARSO_OBJECT_PICTURE)
         {
            /* Ignore Pictures */
            it = (guiObject*)it->getNext();
         }
      }
      return(it);  
   }
   return(NULL);
}

/*********************************************************
 *                      itemAvaible                      *
 *********************************************************/
bool menu::itemAvailable(int item)
{
   guiObject* it = getItem(item);
   if(!it)
   {
      return(false);
   }
   return(it->isAvailable());
}

/*********************************************************
 *                     setItemAvaible                    *
 *********************************************************/
void menu::setItemAvailable(int item, bool av)
{
   guiObject* it = getItem(item);
   if(it)
   {
      it->setAvailable(av);
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
   int y2 = ((total-numPictures) * MENU_ITEM_HEIGHT) + 
                                         y1 + 5; /* bizarre from DOS version */
   
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
   int med = (MENU_ITEM_HEIGHT - 10) / 2;
   guiObject* item = (guiObject*) first;
   for (k=0; k < total; k++)
   {
      /* Treat Pictures */
      if(item->type == FARSO_OBJECT_PICTURE)
      {
         picture* pic = (picture*)item;
         pic->setCoordinate(xa, ya+med+1, xa+10, ya+10+med+1);
         pic->draw(screen);
         /* The next text will be translated right */
         xa = x1+15;
      }
      /* treat Texts */
      else if(item->type == FARSO_OBJECT_TEXT_BOX)
      {
         color_Set(Colors.colorText.R,
               Colors.colorText.G,
               Colors.colorText.B,
               Colors.colorText.A);

         /* Menu Texts */
         if (item->getText().compare("-"))
         {
            if (item->isAvailable()) 
            {
               fnt.write(screen,xa,ya+med,item->getText());
            }
            else
            {
               color_Set(Colors.colorCont[2].R, Colors.colorCont[2].G,
                     Colors.colorCont[2].B, Colors.colorCont[2].A);
               fnt.write(screen,xa+1,ya+med+1,item->getText());
               color_Set(Colors.colorCont[1].R, Colors.colorCont[1].G,
                     Colors.colorCont[1].B, Colors.colorCont[1].A);
               fnt.write(screen,xa,ya+med,item->getText());
            }
         } 

         /* Menu Separators */
         else 
         {
            color_Set(Colors.colorCont[1].R, Colors.colorCont[1].G,
                  Colors.colorCont[1].B, Colors.colorCont[1].A);
            rectangle_2Colors(screen,xa-2,ya+6,x2-2,ya+7,
                  Colors.colorCont[0].R,
                  Colors.colorCont[0].G,Colors.colorCont[0].B,
                  Colors.colorCont[0].A);
         }
         xa = x1+4;
         ya += MENU_ITEM_HEIGHT;
      }
      item = (guiObject*)item->getNext();
   }
  
}

/*********************************************************
 *                     getActualItem                     *
 *********************************************************/
int menu::getActualItem()
{
   if(itemAvailable(actualItem))
   {
      return(actualItem);
   }
   return(0);
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
   int altura = ((total-numPictures)*MENU_ITEM_HEIGHT)+6;
   int largura = (maxCharac)*(fnt.getIncCP()+1)+5;

   /* Runs */
   *pronto = 0;
   int tecla = 0;

   /* Verify Mouse Moviments */
   if(isMouseAt(x+Xjan, y+Yjan, x+largura+Xjan,
            y+altura+Yjan-3, mouseX, mouseY)) 
   {
      actualItem = ((mouseY - (y+Yjan)-4) / MENU_ITEM_HEIGHT) + 1;
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
   else if(teclado[SDLK_DOWN] && (actualItem+1 <= total-numPictures))
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
      rectangle_Oval(screen,x+2,(actualItem-1)*MENU_ITEM_HEIGHT+y+4,
            x+largura-2,(actualItem)*MENU_ITEM_HEIGHT+y+4,
            Colors.colorCont[2].R, Colors.colorCont[2].G,
            Colors.colorCont[2].B, Colors.colorCont[2].A);
   }

   /* Verify if the mouse is at menu */
   if(!isMouseAt(x+Xjan, y+Yjan, x+largura+Xjan, y+altura+Yjan-3, 
                 mouseX, mouseY) && 
      (!tecla) && (*pronto))
   {
      actualItem = 0;
   }

   /* Define the Return */
   return(actualItem);
}



