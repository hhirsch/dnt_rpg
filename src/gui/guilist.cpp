/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "guilist.h"
#include "menu.h"
#include <stdio.h>
#include <SDL/SDL_image.h>

/**************************************************************
 *                         Constructor                        *
 **************************************************************/
guiList::guiList()
{
   total = 0;
   first = NULL;
   intMenu = NULL;
   wSurface = NULL;
}

/**************************************************************
 *                          Destructor                        *
 **************************************************************/
guiList::~guiList()
{
   int tot = total;
   int aux;
   for(aux = 0; aux < tot;aux++)
   {
      removeObject(first);
   }
   if(intMenu)
   {
      removeMenu();
   }
}

/**************************************************************
 *                            changed                         *
 **************************************************************/
bool guiList::changed()
{
   bool result = false;

   guiObject *obj= first;
   int aux;
   for(aux=0; aux < total; aux++)
   {
      result |= obj->changed();
      obj = obj->next;
   }

   return(result);
}

/**************************************************************
 *                            draw                            *
 **************************************************************/
void guiList::draw(SDL_Surface* surface)
{
   guiObject *obj = first;
   int aux;
   for(aux=0; aux < total; aux++)
   {
      switch(obj->type)
      {
         case GUI_BUTTON:
         {
              button *b = (button*) obj;   
              b->draw(surface);
              break;
         }
         case GUI_TEXT_BAR:
         {
              textBar *bart = (textBar*) obj; 
              bart->draw(surface);
              break;
         }
         case GUI_SEL_BOX:
         {
              cxSel *cx = (cxSel*) obj;
              cx->draw(surface);
              break;
         }
         case GUI_SEL_TEXT:
         {
              selText *st = (selText*) obj;
              st->draw(surface);
              break;
         }
         case GUI_PICTURE:
         {
              picture* fig = (picture*) obj;
              fig->draw(surface);
              break;
         }
         case GUI_TEXT_BOX:
         {
              textBox *quad = (textBox*) obj;
              quad->draw();
              break;
         }
         case GUI_TAB_BUTTON:
         {
              tabButton *bt = (tabButton*) obj; 
              bt->setCurrent(-1);
              bt->draw(surface);
              break;
         }
         case GUI_HEALTH_BAR:
         {
              healthBar* hb = (healthBar*) obj;
              hb->draw(surface);
              break;
         }
         default:break;
       
      } //case
      obj = obj->next;
   }
}

/**************************************************************
 *                         removeObject                       *
 **************************************************************/
void guiList::removeObject(guiObject *obj)
{
   /* Update first, if needed */
   if(obj == first)
   {
      first = obj->next;
   }

   /* Update pointers */
   obj->previous->next = obj->next;
   obj->next->previous = obj->previous;

   /* Delete the memory used */
   switch (obj->type) 
   {
      case GUI_BUTTON:
      {
         button* b = (button*) obj;
         delete(b);
         break;
      }
      case GUI_TEXT_BAR:
      {
         textBar* b = (textBar*) obj;
         delete(b);
         break;
      }
      case GUI_SEL_BOX:
      {
         cxSel* c = (cxSel*) obj;
         delete(c);
         break;
      }
      case GUI_PICTURE:
      {
         picture* f = (picture*) obj;
         delete(f);
         break;
      }
      case GUI_SEL_TEXT:
      {
         selText* s = (selText*) obj;
         delete(s);
         break;
      }
      case GUI_TEXT_BOX:
      {
         textBox* q = (textBox*) obj;
         delete(q);
         break;
      }
      case GUI_TAB_BUTTON:
      {
         tabButton* tb = (tabButton*) obj;
         delete(tb);
         break;
      }
      case GUI_ROL_BAR:
      {
         rolBar* b = (rolBar*) obj;
         delete(b);
         break;
      }
      case GUI_LIST_TEXT:
      {
         listText* lt = (listText*) obj;
         delete(lt);
         break;
      }
      case GUI_FILE_SEL:
      {
         fileSel* fs = (fileSel*)obj;
         delete(fs);
         break;
      }
      case GUI_HEALTH_BAR:
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

   /* Decrement things */
   total--;

   /* Verify if list still exists */
   if(total <= 0)
   {
      first = NULL;
   }
}

/**************************************************************
 *                         insertObject                       *
 **************************************************************/
void guiList::insertObject(guiObject* obj)
{
   if(first == NULL)
   {
      obj->next = obj;
      obj->previous = obj;
   }
   else
   {
      obj->next = first;
      obj->previous = first->previous;
      obj->next->previous = obj;
      obj->previous->next = obj;
   }

   first = obj;
   total++;
}

/**************************************************************
 *                         insertButton                       *
 **************************************************************/
button* guiList::insertButton(int xa,int ya,int xb,int yb,
                           string text, bool oval)
{
   button* novo;
   novo = new button(xa,ya,xb,yb, text, oval);
   novo->type = GUI_BUTTON;
   insertObject(novo);
   return(novo);
}

/**************************************************************
 *                         insertCxSel                        *
 **************************************************************/
cxSel* guiList::insertCxSel(int xa,int ya, bool selected)
{
   cxSel* novo;
   novo = new cxSel(xa, ya);
   novo->setSelection(selected);
   insertObject(novo);
   return(novo);
}

/**************************************************************
 *                        insertPicture                       *
 **************************************************************/
picture* guiList::insertPicture(int x,int y,int w,int h,const char* arquivo)
{
   picture* novo;
   novo = new picture(x,y,w,h,arquivo);
   insertObject(novo);
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
      novo = new tabButton(x,y,arquivo);
   }
   else
   {
      novo = new tabButton(x,y,w,h);
   }
   insertObject(novo);
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
   insertObject(novo);
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
   insertObject(novo);
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
   insertObject(novo);
   return(novo);
} 

/**************************************************************
 *                         insertRolBar                       *
 **************************************************************/
rolBar* guiList::insertRolBar(int xa,int ya,int xb,int yb,string txt)
{
   rolBar* novo;
   novo = new rolBar(xa,ya,xb,yb,txt, this, wSurface);
   insertObject(novo);
   return(novo);
}

/**************************************************************
 *                        insertListText                      *
 **************************************************************/
listText* guiList::insertListText(int xa,int ya,int xb,int yb)
{
   listText* novo;
   novo = new listText(xa,ya,xb,yb, wSurface, this);
   insertObject(novo);
   return(novo);
}

/**************************************************************
 *                        insertFileSel                       *
 **************************************************************/
fileSel* guiList::insertFileSel(int xa, int ya, bool load, 
                                string dir, bool nav)
{
   fileSel* n = new fileSel(xa, ya, load, dir, this, nav);
   insertObject(n);
   return(n);
}

/**************************************************************
 *                      insertHealthBar                       *
 **************************************************************/
healthBar* guiList::insertHealthBar(int xa, int ya, int xb, int yb, int max)
{
   healthBar* n = new healthBar(xa, ya, xb, yb);
   insertObject(n);
   n->defineMaxHealth(max);
   return(n);
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
   intMenu = (guiObject*)new menu(0,0);
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


