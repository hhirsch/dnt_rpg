/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "lista.h"
#include "menu.h"
#include <stdio.h>
#include <SDL/SDL_image.h>


Tlista::Tlista()
{
   total = 0;
   first = new guiObject;
   first->type = -1;
   first->next = first;
   first->previous = first;
   intMenu = NULL;
}


Tlista::~Tlista()
{
   int tot = total;
   int aux;
   for(aux = 0;aux < tot;aux++)
   {
      Retirar(first->next);
   }
   if (first != NULL)
   {
      delete first;
   }
   if(intMenu)
   {
      removeMenu();
   }
}

void Tlista::Retirar(guiObject *obj)
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
         figura* f = (figura*) obj;
         delete(f);
         break;
      }
      case GUI_SEL_TEXT:
      {
         selTexto* s = (selTexto*) obj;
         delete(s);
         break;
      }
      case GUI_TEXT_BOX:
      {
         quadroTexto* q = (quadroTexto*) obj;
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

void Tlista::InserirObj(guiObject* obj)
{
   obj->next = first->next;
   obj->previous = first;
   first->next = obj;
   obj->next->previous = obj;
   total++;
}

button* Tlista::insertButton(int xa,int ya,int xb,int yb,
                           string text, bool oval)
{
   button* novo;
   novo = new button(xa,ya,xb,yb, text, oval);
   novo->type = GUI_BUTTON;
   InserirObj(novo);
   return(novo);
}

/* Insere uma nova cxSel na lista */
cxSel* Tlista::insertCxSel(int xa,int ya, bool selected)
{
   cxSel* novo;
   novo = new cxSel(xa, ya);
   novo->setSelection(selected);
   InserirObj(novo);
   return(novo);
}
 
/* Insere uma nova figura na lista */
figura* Tlista::InserirFigura(int x,int y,int w,int h,const char* arquivo)
{
   //printf("Pondo Figura\n");
   figura* novo;
   novo = new figura(x,y,w,h,arquivo);
   

   novo->type = GUI_PICTURE;
   InserirObj(novo);
   return(novo);
} 

tabButton* Tlista::InserirTabButton(int x,int y,int w,int h,const char* arquivo)
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
   

   novo->type = GUI_TAB_BUTTON;
   InserirObj(novo);
   return(novo);
} 

textBar* Tlista::insertTextBar(int xa,int ya,int xb,int yb, string text,int cript)
{
   textBar* novo;
   novo = new textBar(xa,ya,xb,yb, text, cript);
   InserirObj(novo);
   return(novo);
} 
 
/* Insere um novo quadro de Texto na lista */
quadroTexto* Tlista::InserirQuadroTexto(int xa,int ya,int xb,int yb,
                                        int moldura,const char* text)
{
   quadroTexto* novo;
   novo = new quadroTexto;
   novo->x1 = xa;
   novo->x2 = xb;
   novo->y1 = ya;
   novo->y2 = yb;
   novo->moldura = moldura;
   novo->texto = text; 
   novo->type = GUI_TEXT_BOX;
   novo->fonte = FHELVETICA;
   novo->tamFonte = 1;
   novo->aliFonte = ESQUERDA;
   InserirObj(novo);
   return(novo);
} 

/* Insere um novo selTexto na lista */
selTexto* Tlista::insertSelTexto(int xa,int ya,int xb,int yb,
                                  string text0, string text1,
                                  string text2, string text3, 
                                  string text4)
{
   selTexto* novo;
   novo = new selTexto;
   novo->setCoordinate(xa,ya,xb,yb);
   novo->text[0] = text0;
   novo->text[1] = text1;
   novo->text[2] = text2;
   novo->text[3] = text3;
   novo->text[4] = text4;
   novo->type = GUI_SEL_TEXT;
   InserirObj(novo);
   return(novo);
} 

rolBar* Tlista::insertRolBar(int xa,int ya,int xb,int yb,string txt,
                             SDL_Surface* surface)
{
   rolBar* novo;
   novo = new rolBar(xa,ya,xb,yb,txt, this, surface);
   InserirObj(novo);
   return(novo);
}

listText* Tlista::insertListText(int xa,int ya,int xb,int yb,
                                 SDL_Surface* surface)
{
   listText* novo;
   novo = new listText(xa,ya,xb,yb, surface, this);
   InserirObj(novo);
   return(novo);
}

guiObject* Tlista::addMenu()
{
   if(intMenu)
   {
      removeMenu();
   }
   intMenu = (guiObject*)new menu();
   return(intMenu);
}

guiObject* Tlista::getMenu()
{
   return(intMenu);
}

void Tlista::removeMenu()
{
   if(intMenu)
   {
      menu* men = (menu*)intMenu;
      delete(men);
   }
   intMenu = NULL;
}


