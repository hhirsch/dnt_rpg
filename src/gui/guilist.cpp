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

#include "guilist.h"
#include "menu.h"
#include "tabbox.h"

#include <stdio.h>

#ifdef __APPLE__
   #include <SDL_image/SDL_image.h>
#else
   #include <SDL/SDL_image.h>
#endif
using namespace std;
using namespace Farso;

/**************************************************************
 *                         Constructor                        *
 **************************************************************/
GuiList::GuiList(int sWidth, int sHeight, SDL_Surface* surface, 
                 bool hasDecor, int t)
{
   intMenu = NULL;
   wSurface = surface;
   wWidth = sWidth;
   wHeight = sHeight;
   wHasDecor = hasDecor;
   type = t;
   tab = NULL;
}

/**************************************************************
 *                          Destructor                        *
 **************************************************************/
GuiList::~GuiList()
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
 *                          clearList                         *
 **************************************************************/
void GuiList::clearList()
{
   std::list<GuiObject*>::iterator it;

   for(it=list.begin(); it != list.end(); it++)
   {
      freeElement(*it);
   }
   list.clear();
}

/**************************************************************
 *                            changed                         *
 **************************************************************/
bool GuiList::changed()
{
   std::list<GuiObject*>::iterator it;

   /* Verify all objects on list */
   for(it=list.begin(); it != list.end(); it++)
   {
      if((*it)->changed())
      {
         return(true);
      }
   }

   /* Verify tabBox Objects */
   if(tab != NULL)
   {
      TabBox* tb = (TabBox*)tab;
      if(tb->changed())
      {
         return(true);
      }

      if(tb->getActiveList() != NULL)
      {
         if(tb->getActiveList()->changed())
         {
            return(true);
         }
      }
   }

   return(false);
}

/**************************************************************
 *                            draw                            *
 **************************************************************/
void GuiList::draw()
{ 
   std::list<GuiObject*>::iterator it;
   GuiObject* obj;

   for(it=list.begin(); it != list.end(); it++)
   {
      obj = (*it);
      /* Only draw visible objects */
      if(obj->isVisible())
      {
         switch(obj->type)
         {
            case OBJECT_BUTTON:
            {
                 Button *b = (Button*) obj;   
                 b->draw();
                 break;
            }
            case OBJECT_TEXT_BAR:
            {
                 TextBar *bart = (TextBar*) obj; 
                 bart->draw();
                 break;
            }
            case OBJECT_SEL_BOX:
            {
                 CxSel *cx = (CxSel*) obj;
                 cx->draw();
                 break;
            }
            case OBJECT_SEL_TEXT:
            {
                 SelText *st = (SelText*) obj;
                 st->draw();
                 break;
            }
            case OBJECT_PICTURE:
            {
                 Picture* fig = (Picture*) obj;
                 fig->draw();
                 break;
            }
            case OBJECT_TEXT_BOX:
            {
                 TextBox *quad = (TextBox*) obj;
                 quad->draw();
                 break;
            }
            case OBJECT_TAB_BUTTON:
            {
                 TabButton *bt = (TabButton*) obj; 
                 bt->setCurrent(-1);
                 bt->draw();
                 break;
            }
            case OBJECT_HEALTH_BAR:
            {
                 HealthBar* hb = (HealthBar*) obj;
                 hb->draw();
                 break;
            }
            default:break;
          
         } //case
      }
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
void GuiList::freeElement(GuiObject* obj)
{
   /* Delete the memory used */
   switch(obj->type) 
   {
      case OBJECT_BUTTON:
      {
         Button* b = (Button*) obj;
         delete(b);
         break;
      }
      case OBJECT_TEXT_BAR:
      {
         TextBar* b = (TextBar*) obj;
         delete(b);
         break;
      }
      case OBJECT_SEL_BOX:
      {
         CxSel* c = (CxSel*) obj;
         delete(c);
         break;
      }
      case OBJECT_PICTURE:
      {
         Picture* f = (Picture*) obj;
         delete(f);
         break;
      }
      case OBJECT_SEL_TEXT:
      {
         SelText* s = (SelText*) obj;
         delete(s);
         break;
      }
      case OBJECT_TEXT_BOX:
      {
         TextBox* q = (TextBox*) obj;
         delete(q);
         break;
      }
      case OBJECT_TAB_BUTTON:
      {
         TabButton* tb = (TabButton*) obj;
         delete(tb);
         break;
      }
      case OBJECT_ROL_BAR:
      {
         RolBar* b = (RolBar*) obj;
         delete(b);
         break;
      }
      case OBJECT_LIST_TEXT:
      {
         ListText* lt = (ListText*) obj;
         delete(lt);
         break;
      }
      case OBJECT_FILE_SEL:
      {
         FileSel* fs = (FileSel*)obj;
         delete(fs);
         break;
      }
      case OBJECT_HEALTH_BAR:
      {
         HealthBar* hb = (HealthBar*)obj;
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
 *                            insert                          *
 **************************************************************/
void GuiList::insert(GuiObject* obj)
{
   if(type == LIST_TYPE_ADD_AT_BEGIN)
   {
      list.push_front(obj);
   }
   else /*if(type == LIST_TYPE_ADD_AT_END)*/
   {
      list.push_back(obj);
   }
}

/**************************************************************
 *                         insertButton                       *
 **************************************************************/
Button* GuiList::insertButton(int xa,int ya,int xb,int yb,
                           string text, bool oval)
{
   Button* novo;
   novo = new Button(xa,ya,xb,yb, text, oval, wSurface);
   insert(novo);
   return(novo);
}

/**************************************************************
 *                         insertCxSel                        *
 **************************************************************/
CxSel* GuiList::insertCxSel(int xa,int ya, bool selected)
{
   CxSel* novo;
   novo = new CxSel(xa, ya, wSurface);
   novo->setSelection(selected);
   insert(novo);
   return(novo);
}

/**************************************************************
 *                        insertPicture                       *
 **************************************************************/
Picture* GuiList::insertPicture(int x,int y,int w,int h,const char* arquivo)
{
   Picture* novo;
   novo = new Picture(x,y,w,h,arquivo,wSurface);
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
TabButton* GuiList::insertTabButton(int x,int y,int w,int h,const char* arquivo)
{
   TabButton* novo;

   if(arquivo)
   {
      novo = new TabButton(x,y,arquivo,wSurface);
   }
   else
   {
      novo = new TabButton(x,y,w,h, wSurface);
   }
   insert(novo);
   return(novo);
} 

/**************************************************************
 *                        insertTextBar                       *
 **************************************************************/
TextBar* GuiList::insertTextBar(int xa,int ya,int xb,int yb, string text,
                                int cript)
{
   TextBar* novo;
   novo = new TextBar(xa,ya,xb,yb, text, cript, wSurface);
   insert(novo);
   return(novo);
} 

/**************************************************************
 *                         insertTextBox                      *
 **************************************************************/
TextBox* GuiList::insertTextBox(int xa,int ya,int xb,int yb,
                                   int frameType, string text)
{
   TextBox* novo;
   novo = new TextBox(xa,ya,xb,yb,frameType,wSurface);
   novo->setText(text);
   insert(novo);
   return(novo);
} 

/**************************************************************
 *                        insertSelText                       *
 **************************************************************/
SelText* GuiList::insertSelText(int xa,int ya,int xb,int yb,
                                string text0, string text1,
                                string text2, string text3, 
                                string text4)
{
   SelText* novo;
   novo = new SelText(xa,ya,xb,yb,text0,text1,text2,text3,text4,wSurface);
   insert(novo);
   return(novo);
} 

/**************************************************************
 *                         insertRolBar                       *
 **************************************************************/
RolBar* GuiList::insertRolBar(int xa,int ya,int xb,int yb,string txt)
{
   RolBar* novo;
   novo = new RolBar(xa,ya,xb,yb,txt, this, wSurface);
   insert(novo);
   return(novo);
}

/**************************************************************
 *                        insertListText                      *
 **************************************************************/
ListText* GuiList::insertListText(int xa,int ya,int xb,int yb)
{
   ListText* novo;
   novo = new ListText(xa,ya,xb,yb, wSurface, this);
   insert(novo);
   return(novo);
}

/**************************************************************
 *                        insertFileSel                       *
 **************************************************************/
FileSel* GuiList::insertFileSel(int xa, int ya, bool load, 
                                string dir, bool nav)
{
   FileSel* n = new FileSel(xa, ya, load, dir, this, wSurface, nav);
   insert(n);
   return(n);
}

/**************************************************************
 *                      insertHealthBar                       *
 **************************************************************/
HealthBar* GuiList::insertHealthBar(int xa, int ya, int xb, int yb, int max)
{
   HealthBar* n = new HealthBar(xa, ya, xb, yb, wSurface);
   insert(n);
   n->defineMaxHealth(max);
   return(n);
}

/*********************************************************************
 *                          defineTabBox                             *
 *********************************************************************/
GuiObject* GuiList::defineTabBox(int x1, int y1, int x2, int y2)
{
   if(tab != NULL)
   {
      TabBox* tb = (TabBox*)tab;
      delete(tb);
   }

   tab = (GuiObject*)new TabBox(x1,y1,x2,y2, wSurface);
   return(tab);
}

/*********************************************************************
 *                            getTabBox                              *
 *********************************************************************/
GuiObject* GuiList::getTabBox()
{
   return(tab);
}

/*********************************************************************
 *                       getActiveTabBoxList                         *
 *********************************************************************/
GuiList* GuiList::getActiveTabBoxList()
{
   if(tab != NULL)
   {
      TabBox* tb = (TabBox*)tab;
      return(tb->getActiveList());
   }

   return(NULL);
}

/**************************************************************
 *                            addMenu                         *
 **************************************************************/
GuiObject* GuiList::addMenu()
{
   if(intMenu)
   {
      removeMenu();
   }
   intMenu = (GuiObject*)new Menu(0,0,wWidth, wHeight, wSurface);
   return(intMenu);
}

/**************************************************************
 *                           getMenu                          *
 **************************************************************/
GuiObject* GuiList::getMenu()
{
   return(intMenu);
}

/**************************************************************
 *                          removeMenu                        *
 **************************************************************/
void GuiList::removeMenu()
{
   if(intMenu)
   {
      Menu* men = (Menu*)intMenu;
      delete(men);
   }
   intMenu = NULL;
}


