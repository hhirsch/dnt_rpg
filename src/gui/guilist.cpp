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
   /* Head Node. Of button type, since guiObject is virtual! */
   first = (guiObject*)new button(0,0, 0,0, "", false);
   first->type = -1;
   first->next = first;
   first->previous = first;
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
   for(aux = 0;aux < tot;aux++)
   {
      removeObject(first->next);
   }
   if (first != NULL)
   {
      delete((button*)first);
   }
   if(intMenu)
   {
      removeMenu();
   }
}

/**************************************************************
 *                         removeObject                       *
 **************************************************************/
void guiList::removeObject(guiObject *obj)
{
   obj->previous->next = obj->next;
   obj->next->previous = obj->previous;
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
      default: 
      {
        delete(obj);
        break;
      }
   }
   total--;
}

/**************************************************************
 *                         insertObject                       *
 **************************************************************/
void guiList::insertObject(guiObject* obj)
{
   obj->next = first->next;
   obj->previous = first;
   first->next = obj;
   obj->next->previous = obj;
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
   novo = new textBar(xa,ya,xb,yb, text, cript);
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
   novo = new selText(xa,ya,xb,yb,text0,text1,text2,text3,text4);
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


