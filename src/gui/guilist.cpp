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

#include "guilist.h"
#include "menu.h"
#include "tabbox.h"

#include <stdio.h>
#include <SDL/SDL_image.h>

/**************************************************************
 *                         Constructor                        *
 **************************************************************/
guiList::guiList(int sWidth, int sHeight, SDL_Surface* surface, 
                 bool hasDecor, int t):dntList(t)
{
   total = 0;
   first = NULL;
   intMenu = NULL;
   wSurface = surface;
   wWidth = sWidth;
   wHeight = sHeight;
   wHasDecor = hasDecor;
   tab = NULL;
}

/**************************************************************
 *                          Destructor                        *
 **************************************************************/
guiList::~guiList()
{
   /* Clear the list */
   clearList();

   /* Remove internal menu, if any */
   if(intMenu)
   {
      removeMenu();
   }

   /* Remove internal tab, if any */
   if(tab)
   {
      delete(tab);
   }
}

/**************************************************************
 *                            changed                         *
 **************************************************************/
bool guiList::changed()
{
   bool result = false;

   guiObject *obj = (guiObject*)first;
   int aux;
   for(aux=0; aux < total; aux++)
   {
      result |= obj->changed();
      obj = (guiObject*)obj->getNext();
   }

   /* Verify tabBox Objects */
   if(tab != NULL)
   {
      tabBox* tb = (tabBox*)tab;
      result |= tb->changed();

      if(tb->getActiveList() != NULL)
      {
         result |= tb->getActiveList()->changed();
      }
   }

   return(result);
}

/**************************************************************
 *                            draw                            *
 **************************************************************/
void guiList::draw()
{
   guiObject *obj = (guiObject*)first;
   int aux;
   for(aux=0; aux < total; aux++)
   {
      switch(obj->type)
      {
         case FARSO_OBJECT_BUTTON:
         {
              button *b = (button*) obj;   
              b->draw();
              break;
         }
         case FARSO_OBJECT_TEXT_BAR:
         {
              textBar *bart = (textBar*) obj; 
              bart->draw();
              break;
         }
         case FARSO_OBJECT_SEL_BOX:
         {
              cxSel *cx = (cxSel*) obj;
              cx->draw();
              break;
         }
         case FARSO_OBJECT_SEL_TEXT:
         {
              selText *st = (selText*) obj;
              st->draw();
              break;
         }
         case FARSO_OBJECT_PICTURE:
         {
              picture* fig = (picture*) obj;
              fig->draw();
              break;
         }
         case FARSO_OBJECT_TEXT_BOX:
         {
              textBox *quad = (textBox*) obj;
              quad->draw();
              break;
         }
         case FARSO_OBJECT_TAB_BUTTON:
         {
              tabButton *bt = (tabButton*) obj; 
              bt->setCurrent(-1);
              bt->draw();
              break;
         }
         case FARSO_OBJECT_HEALTH_BAR:
         {
              healthBar* hb = (healthBar*) obj;
              hb->draw();
              break;
         }
         default:break;
       
      } //case
      obj = (guiObject*)obj->getNext();
   }

   /* TabBox Draw */
   if(tab != NULL)
   {
      tab->draw();
   }
}

/**************************************************************
 *                         freeElement                        *
 **************************************************************/
void guiList::freeElement(dntListElement *obj)
{
   guiObject* gobj = (guiObject*)obj;

   /* Delete the memory used */
   switch(gobj->type) 
   {
      case FARSO_OBJECT_BUTTON:
      {
         button* b = (button*) obj;
         delete(b);
         break;
      }
      case FARSO_OBJECT_TEXT_BAR:
      {
         textBar* b = (textBar*) obj;
         delete(b);
         break;
      }
      case FARSO_OBJECT_SEL_BOX:
      {
         cxSel* c = (cxSel*) obj;
         delete(c);
         break;
      }
      case FARSO_OBJECT_PICTURE:
      {
         picture* f = (picture*) obj;
         delete(f);
         break;
      }
      case FARSO_OBJECT_SEL_TEXT:
      {
         selText* s = (selText*) obj;
         delete(s);
         break;
      }
      case FARSO_OBJECT_TEXT_BOX:
      {
         textBox* q = (textBox*) obj;
         delete(q);
         break;
      }
      case FARSO_OBJECT_TAB_BUTTON:
      {
         tabButton* tb = (tabButton*) obj;
         delete(tb);
         break;
      }
      case FARSO_OBJECT_ROL_BAR:
      {
         rolBar* b = (rolBar*) obj;
         delete(b);
         break;
      }
      case FARSO_OBJECT_LIST_TEXT:
      {
         listText* lt = (listText*) obj;
         delete(lt);
         break;
      }
      case FARSO_OBJECT_FILE_SEL:
      {
         fileSel* fs = (fileSel*)obj;
         delete(fs);
         break;
      }
      case FARSO_OBJECT_HEALTH_BAR:
      {
         healthBar* hb = (healthBar*)obj;
         delete(hb);
         break;
      }
      default: 
      {
        delete(obj);
        break;
      }
   }

}

/**************************************************************
 *                         insertButton                       *
 **************************************************************/
button* guiList::insertButton(int xa,int ya,int xb,int yb,
                           string text, bool oval)
{
   button* novo;
   novo = new button(xa,ya,xb,yb, text, oval, wSurface);
   insert(novo);
   return(novo);
}

/**************************************************************
 *                         insertCxSel                        *
 **************************************************************/
cxSel* guiList::insertCxSel(int xa,int ya, bool selected)
{
   cxSel* novo;
   novo = new cxSel(xa, ya, wSurface);
   novo->setSelection(selected);
   insert(novo);
   return(novo);
}

/**************************************************************
 *                        insertPicture                       *
 **************************************************************/
picture* guiList::insertPicture(int x,int y,int w,int h,const char* arquivo)
{
   picture* novo;
   novo = new picture(x,y,w,h,arquivo,wSurface);
   /* Use alpha from source at empty windows */
   if(!wHasDecor)
   {
      novo->setUseAlphaFromSource();
   }
   insert(novo);
   return(novo);
} 

/**************************************************************
 *                       insertTabButton                      *
 **************************************************************/
tabButton* guiList::insertTabButton(int x,int y,int w,int h,const char* arquivo)
{
   tabButton* novo;

   if(arquivo)
   {
      novo = new tabButton(x,y,arquivo,wSurface);
   }
   else
   {
      novo = new tabButton(x,y,w,h, wSurface);
   }
   insert(novo);
   return(novo);
} 

/**************************************************************
 *                        insertTextBar                       *
 **************************************************************/
textBar* guiList::insertTextBar(int xa,int ya,int xb,int yb, string text,
                                int cript)
{
   textBar* novo;
   novo = new textBar(xa,ya,xb,yb, text, cript, wSurface);
   insert(novo);
   return(novo);
} 

/**************************************************************
 *                         insertTextBox                      *
 **************************************************************/
textBox* guiList::insertTextBox(int xa,int ya,int xb,int yb,
                                   int frameType, string text)
{
   textBox* novo;
   novo = new textBox(xa,ya,xb,yb,frameType,wSurface);
   novo->setText(text);
   insert(novo);
   return(novo);
} 

/**************************************************************
 *                        insertSelText                       *
 **************************************************************/
selText* guiList::insertSelText(int xa,int ya,int xb,int yb,
                                string text0, string text1,
                                string text2, string text3, 
                                string text4)
{
   selText* novo;
   novo = new selText(xa,ya,xb,yb,text0,text1,text2,text3,text4,wSurface);
   insert(novo);
   return(novo);
} 

/**************************************************************
 *                         insertRolBar                       *
 **************************************************************/
rolBar* guiList::insertRolBar(int xa,int ya,int xb,int yb,string txt)
{
   rolBar* novo;
   novo = new rolBar(xa,ya,xb,yb,txt, this, wSurface);
   insert(novo);
   return(novo);
}

/**************************************************************
 *                        insertListText                      *
 **************************************************************/
listText* guiList::insertListText(int xa,int ya,int xb,int yb)
{
   listText* novo;
   novo = new listText(xa,ya,xb,yb, wSurface, this);
   insert(novo);
   return(novo);
}

/**************************************************************
 *                        insertFileSel                       *
 **************************************************************/
fileSel* guiList::insertFileSel(int xa, int ya, bool load, 
                                string dir, bool nav)
{
   fileSel* n = new fileSel(xa, ya, load, dir, this, wSurface, nav);
   insert(n);
   return(n);
}

/**************************************************************
 *                      insertHealthBar                       *
 **************************************************************/
healthBar* guiList::insertHealthBar(int xa, int ya, int xb, int yb, int max)
{
   healthBar* n = new healthBar(xa, ya, xb, yb, wSurface);
   insert(n);
   n->defineMaxHealth(max);
   return(n);
}

/*********************************************************************
 *                          defineTabBox                             *
 *********************************************************************/
guiObject* guiList::defineTabBox(int x1, int y1, int x2, int y2)
{
   if(tab != NULL)
   {
      tabBox* tb = (tabBox*)tab;
      delete(tb);
   }

   tab = (guiObject*)new tabBox(x1,y1,x2,y2, wSurface);
   return(tab);
}

/*********************************************************************
 *                            getTabBox                              *
 *********************************************************************/
guiObject* guiList::getTabBox()
{
   return(tab);
}

/*********************************************************************
 *                       getActiveTabBoxList                         *
 *********************************************************************/
guiList* guiList::getActiveTabBoxList()
{
   if(tab != NULL)
   {
      tabBox* tb = (tabBox*)tab;
      return(tb->getActiveList());
   }

   return(NULL);
}

/**************************************************************
 *                            addMenu                         *
 **************************************************************/
guiObject* guiList::addMenu()
{
   if(intMenu)
   {
      removeMenu();
   }
   intMenu = (guiObject*)new menu(0,0,wWidth, wHeight, wSurface);
   return(intMenu);
}

/**************************************************************
 *                           getMenu                          *
 **************************************************************/
guiObject* guiList::getMenu()
{
   return(intMenu);
}

/**************************************************************
 *                          removeMenu                        *
 **************************************************************/
void guiList::removeMenu()
{
   if(intMenu)
   {
      menu* men = (menu*)intMenu;
      delete(men);
   }
   intMenu = NULL;
}


