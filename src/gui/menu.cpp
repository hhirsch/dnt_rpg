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

#include "menu.h"
#include "farsoopts.h"

/*********************************************************
 *                       Destructor                      *
 *********************************************************/
menu::~menu()
{
}

/*********************************************************
 *                      Constructor                      *
 *********************************************************/
menu::menu(int xa, int ya, int sWidth, int sHeight, SDL_Surface* surface)
     :guiList(sWidth, sHeight, surface, true, LIST_TYPE_ADD_AT_END)
{
   x = xa;
   y = ya;
   actualItem = 1;
   maxCharac = 0;
   numPictures = 0;
   pressed = false;
   wSurface = surface;
   wWidth = sWidth;
   wHeight = sHeight;
}

/*********************************************************
 *                      insertItem                       *
 *********************************************************/
void menu::insertItem(std::string text, bool avaible)
{
   insertItem(text, "", avaible);
} 

/*********************************************************
 *                      insertItem                       *
 *********************************************************/
void menu::insertItem(std::string text, std::string imageFile, bool avaible)
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
   int count=0;
   /* If searching for greater than total, for sure doesn't exist */
   if(i > (list.size()-numPictures))
   {
      return(NULL);
   }

   std::list<guiObject*>::iterator it;
   for(it=list.begin(); it != list.end(); it++)
   {
      if((*it)->type != FARSO_OBJECT_PICTURE)
      {
         count++;
         if(count == i)
         {
            return(*it);  
         }
      }
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
void menu::draw(int pos)
{
   dntFont fnt;
   farsoOptions opt;
   fnt.defineFont(opt.getDefaultFont(), 10);
   fnt.defineFontAlign(DNT_FONT_ALIGN_LEFT);
   fnt.defineFontStyle(DNT_FONT_STYLE_NORMAL);

   /* Define Coordinates */
   int x1 = x;
   int x2 = x1 + (maxCharac)*(fnt.getIncCP()+1)+4; 
   int y1 = y;
   int y2 = ((list.size()-numPictures) * MENU_ITEM_HEIGHT) + 
                                         y1 + 5; /* bizarre from DOS version */
   
   /* Verify Sides */
   if(x2 > wWidth-1)
   {
      x1 = (wWidth-1) - (x2 - x1);
      x2 = wWidth-1;
      x = x1;
   }
   if(y2 > wHeight-1)
   {
      y1 = (wHeight-1) - (y2 - y1);
      y2 = wHeight-1;
      y = y1;
   }

   /* Draw the Menu */
   color_Set(Colors.colorMenu.R, Colors.colorMenu.G,
             Colors.colorMenu.B, Colors.colorMenu.A);
   rectangle_Fill(wSurface,x1+1,y1+1,x2-1,y2-1);
   color_Set(Colors.colorCont[2].R, Colors.colorCont[2].G,
             Colors.colorCont[2].B, Colors.colorCont[2].A);
   rectangle_Oval(wSurface,x1,y1,x2,y2,Colors.colorCont[1].R,
                  Colors.colorCont[1].G, Colors.colorCont[1].B,
                  Colors.colorCont[1].A);
      /*itens*/  
   int xa = x1+4;
   int ya = y1+3;
   int med = (MENU_ITEM_HEIGHT - 10) / 2;
   guiObject* item;

   std::list<guiObject*>::iterator it;
   for(it=list.begin(); it != list.end(); it++)
   {
      item = (*it);
      /* Treat Pictures */
      if(item->type == FARSO_OBJECT_PICTURE)
      {
         picture* pic = (picture*)item;
         pic->setCoordinate(xa, ya+med+1, xa+10, ya+10+med+1);
         pic->draw();
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
               fnt.write(wSurface,xa,ya+med,item->getText());
            }
            else
            {
               color_Set(Colors.colorCont[2].R, Colors.colorCont[2].G,
                     Colors.colorCont[2].B, Colors.colorCont[2].A);
               fnt.write(wSurface,xa+1,ya+med+1,item->getText());
               color_Set(Colors.colorCont[1].R, Colors.colorCont[1].G,
                     Colors.colorCont[1].B, Colors.colorCont[1].A);
               fnt.write(wSurface,xa,ya+med,item->getText());
            }
         } 

         /* Menu Separators */
         else 
         {
            color_Set(Colors.colorCont[1].R, Colors.colorCont[1].G,
                  Colors.colorCont[1].B, Colors.colorCont[1].A);
            rectangle_2Colors(wSurface,xa-2,ya+6,x2-2,ya+7,
                  Colors.colorCont[0].R,
                  Colors.colorCont[0].G,Colors.colorCont[0].B,
                  Colors.colorCont[0].A);
         }
         xa = x1+4;
         ya += MENU_ITEM_HEIGHT;
      }
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
              int *pronto, int Xjan, int Yjan)
{
   farsoOptions opt;
   dntFont fnt;
   fnt.defineFont(opt.getDefaultFont(), 10);

   /* Draws */
   draw(0);
   int altura = ((list.size()-numPictures)*MENU_ITEM_HEIGHT)+6;
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
   else if(teclado[SDLK_DOWN] && (actualItem+1 <= (int)list.size()-numPictures))
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
      rectangle_Oval(wSurface,x+2,(actualItem-1)*MENU_ITEM_HEIGHT+y+4,
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



