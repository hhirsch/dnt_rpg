/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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

#include "interface.h"
#include "messages.h"
#include <SDL/SDL_image.h>

#include <iostream>
using namespace std;

int mouseX=0,mouseY=0;

/*********************************************************************
 *                             GUI Constructor                       *
 *********************************************************************/
guiInterface::guiInterface(char* arqfundo)
{
   ljan = new windowList;
   objects = new guiList;
   if(arqfundo != NULL)
   {
      fundo = IMG_Load(arqfundo);
      if(!fundo) printf("Nao abri arquivo de fundo\n");
   } 
   else
   {
       fundo = NULL;
   }
   focus = FARSO_FOCUS_GAME;
}

/*********************************************************************
 *                           GUI Destructor                          *
 *********************************************************************/
guiInterface::~guiInterface()
{
   delete(ljan);
   delete(objects);
   SDL_FreeSurface(fundo);
}

/*********************************************************************
 *                        verifyMouseInObjects                       *
 *********************************************************************/
void guiInterface::verifyMouseInObjects(int x, int y, guiList* list)
{
   int aux;
   /* Verify if the list is valid */
   if(list == NULL)
   {
      return;
   }

   guiObject *obj;
   obj = list->getFirst();
   for(aux=0; aux < list->getTotal(); aux++)
   {
      /* Test selTexto */
      if(obj->type == FARSO_OBJECT_SEL_TEXT) 
      {
         selText *st = (selText*) obj;
         int xa,ya,xb,yb;
         st->getCoordinate(xa,ya,xb,yb);
         if(ljan->getActiveWindow()->isMouseIn(x,y))
         {
            objAtivo = st;
            focus = FARSO_FOCUS_SEL_TEXT;
         }
      }
      /* Verify Button Table */
      else if(obj->type == FARSO_OBJECT_TAB_BUTTON)
      {
         tabButton *tb = (tabButton*) obj;
         if(tb->isMouseIn(x-ljan->getActiveWindow()->getX1(),
                  y-ljan->getActiveWindow()->getY1()))
         {
            objAtivo = tb;
            focus = FARSO_FOCUS_TAB_BUTTON;
         }
      }
      obj = obj->next;
   }

   /* Verify TabBox, if defined */
   if(list->getActiveTabBoxList() != NULL)
   {
      verifyMouseInObjects(x, y, list->getActiveTabBoxList());
   }
}

/*********************************************************************
 *                      verifyMousePressObjects                      *
 *********************************************************************/
void guiInterface::verifyMousePressObjects(int x, int y, guiList* list)
{
   /* Verify if the guiList is valid */
   if(!list)
   {
      return;
   }

   /* Here are the internal windows clicks verification */
   guiObject *obj = list->getFirst();
   int aux;

   for(aux=0; aux < list->getTotal(); aux++)
   {
      if(obj->type == FARSO_OBJECT_BUTTON)
      {
         /* Verify Click on Button */
         button *bot = (button*) obj;
         if(bot->isMouseIn(x - ljan->getActiveWindow()->getX1(),
                  y - ljan->getActiveWindow()->getY1()))
         {

            objAtivo = bot;
            focus = FARSO_FOCUS_BUTTON;
         }
      }
      /* Verify Click on TextBar */ 
      else if(obj->type == FARSO_OBJECT_TEXT_BAR)
      {
         textBar *bart = (textBar*) obj;
         if(bart->isMouseIn(x-ljan->getActiveWindow()->getX1(),
                  y-ljan->getActiveWindow()->getY1()))
         {
            objAtivo = bart;
            bart->defineCursorPosition(x-
                  ljan->getActiveWindow()->getX1(),
                  y-ljan->getActiveWindow()->getY1());
            focus = FARSO_FOCUS_TEXT_BAR;
         }
      }
      /* Verify RadioBoxes */
      else if(obj->type == FARSO_OBJECT_SEL_BOX)
      {
         cxSel* cx = (cxSel*) obj;
         if(cx->isMouseIn(x-ljan->getActiveWindow()->getX1(),
                  y-ljan->getActiveWindow()->getY1()))
         {
            objAtivo = cx;
            focus = FARSO_FOCUS_CX_SEL;
         }
      }
      /* Verify Text Select */
      else if(obj->type == FARSO_OBJECT_SEL_TEXT)
      {
         selText* st = (selText*) obj;
         int xa,ya,xb,yb;
         st->getCoordinate(xa,ya,xb,yb);
         if(isMouseAt(xa+ljan->getActiveWindow()->getX1(),
                  ya+ljan->getActiveWindow()->getY1(),
                  xb+ljan->getActiveWindow()->getX1(),
                  yb+ljan->getActiveWindow()->getY1(),x,y))
         {
            objAtivo = st;
            focus = FARSO_FOCUS_SEL_TEXT;
         }
      }
      obj = obj->next;
   }

   /* Verify inner tabbox if defined */
   if(list->getActiveTabBoxList() != NULL)
   {
      verifyMousePressObjects(x,y, list->getActiveTabBoxList());
   }
}

/*********************************************************************
 *                           verifyTabBox                            *
 *********************************************************************/
guiObject* guiInterface::verifyTabBox(int x, int y, guiList* list)
{
   if(!list)
   {
      return(NULL);
   }
   window* actWindow = ljan->getActiveWindow();

   /* Verify the tabBox */
   if(list->getTabBox() != NULL)
   {
      tabBox* tb = (tabBox*)list->getTabBox();
      if(tb->isMouseIn(x - actWindow->getX1(), y - actWindow->getY1()))
      {
         if(tb->verifyChanges(x - actWindow->getX1(), y - actWindow->getY1()))
         {
            return((guiObject*)tb);
         }
      }

      /* Verify Nested Tabbox */
      if(list->getActiveTabBoxList() != NULL)
      {
          return(verifyTabBox(x,y,list->getActiveTabBoxList()));
      }
   }

   return(NULL);
}

/*********************************************************************
 *                           verifyRolBars                           *
 *********************************************************************/
bool guiInterface::verifyRolBars(guiList* list)
{
   /* Verify if list exists */
   if(list == NULL)
   {
      return(false);
   }

   /* Now search it for rolBars */
   guiObject *obj;
   obj = list->getFirst();
   int aux;
   for(aux=0; aux < list->getTotal(); aux++)
   {
      if(obj->type == FARSO_OBJECT_ROL_BAR)
      {
         rolBar* rb = (rolBar*)obj;
         if(rb->eventGot(FARSO_EVENT_ON_PRESS_BUTTON, objAtivo))
         {
            ljan->getActiveWindow()->draw(0,0);
            rb->redraw();
            return(true);
         }
      }
      obj = obj->next;
   }

   /* Verify Nested tabBox */
   if(list->getActiveTabBoxList() != NULL)
   {
      return(verifyRolBars(list->getActiveTabBoxList()));
   }

   return(false);

}

/*********************************************************************
 *                   Take care of GUI I/O Events                     *
 *********************************************************************/
guiObject* guiInterface::manipulateEvents(int x, int y, Uint8 Mbotao, 
                                       Uint8* tecla, int& eventInfo)
{
   guiObject* actObj = verifySingleEvents(x,y,Mbotao,tecla, eventInfo);

   if( (eventInfo != FARSO_EVENT_NONE) && (ljan->getActiveWindow() != NULL) )
   {
      actObj = verifyCompositeEvents(actObj, eventInfo);
      actObj = verifyFileSelectorsEvents(actObj, eventInfo);
   }

   return(actObj);
}

/*********************************************************************
 *                   verifyFileSelectorsEvents                       *
 *********************************************************************/
guiObject* guiInterface::verifyFileSelectorsEvents(guiObject* actObj, 
                                                   int& eventInfo)
{
   int aux;
   guiObject *obj = ljan->getActiveWindow()->getObjectsList()->getFirst();

   /* pass all objects, treating file selectors */
   for(aux=0; aux < ljan->getActiveWindow()->getObjectsList()->getTotal(); 
       aux++)
   {
      /* Verify FARSO_OBJECT_FILE_SEL */
      if(obj->type == FARSO_OBJECT_FILE_SEL)
      {
         fileSel* fs = (fileSel*)obj;
         if(fs->eventGot(eventInfo, actObj))
         {
            actObj = obj;
            switch(fs->getLastAction())
            {
               case FILE_SEL_ACTION_ACCEPT:
                  eventInfo = FARSO_EVENT_FILE_SEL_ACCEPT;
               break;
               case FILE_SEL_ACTION_CANCEL:
                  eventInfo = FARSO_EVENT_FILE_SEL_CANCEL;
               break;
               case FILE_SEL_ACTION_SELECT:
                  eventInfo = FARSO_EVENT_FILE_SEL_CHANGED;
               break;
               default:
                  eventInfo = FARSO_EVENT_NONE;
               break;
            }
            focus = FARSO_FOCUS_GAME;
         }
      }
      obj = obj->next;
   }
   return(actObj);
}

/*********************************************************************
 *                      verifyCompositeEvents                        *
 *********************************************************************/
guiObject* guiInterface::verifyCompositeEvents(guiObject* actObj, 
                                               int& eventInfo)
{
   int aux;
   guiObject *obj = ljan->getActiveWindow()->getObjectsList()->getFirst();

   /* pass all objects, treating those composited */
   for(aux=0; aux < ljan->getActiveWindow()->getObjectsList()->getTotal(); 
       aux++)
   {
      /* Verify FARSO_OBJECT_LIST_TEXT */
      if(obj->type == FARSO_OBJECT_LIST_TEXT)
      {
         listText* lt = (listText*)obj;
         if(lt->eventGot(eventInfo, actObj))
         {
            ljan->getActiveWindow()->draw(0,0);
            eventInfo = FARSO_EVENT_SELECTED_LIST_TEXT;
            focus = FARSO_FOCUS_GAME;
            actObj = obj;
            return(lt);
         }
      }
      obj = obj->next;
   }

   /* Verify Warning Window */
   warning warn;
   if( (warn.isOpened()) && (eventInfo == FARSO_EVENT_PRESSED_BUTTON) &&
       (actObj == warn.getOkButton()) )
   {
      warn.close();
   }
   /* Verify Qty Window */
   quantityWindow qty;
   if(qty.isOpened())
   {
      qty.verifyEvents(actObj, eventInfo);
   }

   return(actObj);
}

/*********************************************************************
 *                        verifySingleEvents                         *
 *********************************************************************/
guiObject* guiInterface::verifySingleEvents(int x, int y, Uint8 Mbotao, 
                                         Uint8* tecla, int& eventInfo)
{
    dntFont fnt;

    if(!objAtivo)
    {
       focus = FARSO_FOCUS_GAME;
    }

    if(ljan->getActiveWindow() == NULL)
    {
       eventInfo = FARSO_EVENT_NONE;
       focus = FARSO_FOCUS_GAME;
       return(NULL);
    }

    /* Keyboard Events */
    if ( (tecla[SDLK_ESCAPE]) && (focus != FARSO_FOCUS_GAME))
    {
       focus = FARSO_FOCUS_GAME;
       eventInfo = FARSO_EVENT_EXIT;
       return(NULL);
    }

    /* Verify Main Super Menu */
    if(ljan->getMenu())
    {
       objAtivo = (guiObject*) ljan->getMenu();
       focus = FARSO_FOCUS_MENU;
    }
    else /* Verify Window Super Menu */
    if(ljan->getActiveWindow()->getObjectsList()->getMenu())
    {
       objAtivo = (guiObject*) 
                           ljan->getActiveWindow()->getObjectsList()->getMenu();
       focus = FARSO_FOCUS_MENU;
    }

    /* Mouse move to change focus */
    if( (x != mouseX || y != mouseY) && 
        (focus == FARSO_FOCUS_GAME) )
    {
        mouseX = x;
        mouseY = y;

        if ((ljan->getActiveWindow() != NULL) &&
            (ljan->getActiveWindow()->isMouseIn(x,y)))
        {
            /* Verify All objects */
            window* actWindow = ljan->getActiveWindow();
            verifyMouseInObjects(x,y,actWindow->getObjectsList());
        }
    }

    /* Verify mouse button for focus change */
    if((Mbotao & SDL_BUTTON(1)) &&  (focus == FARSO_FOCUS_GAME))
    {
        if( ( (ljan->getActiveWindow() != NULL) && 
              (ljan->getActiveWindow()->canMoveWindow()) ) &&
              (isMouseAt(ljan->getActiveWindow()->getX1()+36,
                         ljan->getActiveWindow()->getY1(), 
                          ljan->getActiveWindow()->getX2()-3,
                          ljan->getActiveWindow()->getY1()+12,x,y)))
        {
           /* Active Window Moves */
           ljan->getActiveWindow()->setDiff(x-ljan->getActiveWindow()->getX1(),
                                            y-ljan->getActiveWindow()->getY1());
           objAtivo = (guiObject*) ljan->getActiveWindow();
           focus = FARSO_FOCUS_WINDOW_MOVE;
        }
        else if ( (ljan->getActiveWindow() != NULL) &&
                  (ljan->getActiveWindow()->isMouseIn(x,y)))
        {
            /* Will search at two lists: the widow one and the 
             * active tabBox one (if exists) */
            window* actWindow = ljan->getActiveWindow();
            verifyMousePressObjects(x,y,actWindow->getObjectsList());

            guiObject* ot = verifyTabBox(x,y, actWindow->getObjectsList());
            if(ot)
            {
               /* got event on tabbox */
               eventInfo = FARSO_EVENT_TAB_BOX_CHANGED;
               return(ot);
            }
            
            eventInfo = FARSO_EVENT_CLICKED_WINDOW;
            return((guiObject*) ljan->getActiveWindow());
        }
        else if( (!ljan->getActiveWindow()->isModal()) )
        {
           /* Test Other Windows Activation (if the current one 
            * isn't a modal window) */
           int aux; 
           window *jaux=(window*)ljan->getFirst();
           for(aux=0; aux < ljan->getTotal(); aux++)
           {
               if( (jaux->isVisible()) &&
                   (jaux != ljan->getActiveWindow())  && 
                   (jaux->isMouseIn(x,y)))
               {
                    focus = FARSO_FOCUS_GAME;
                    jaux->activate();
                    eventInfo = FARSO_EVENT_ACTIVATED_WINDOW;
                    mouseX = -1;
                    return((guiObject*) jaux);
               }
               jaux = (window*) jaux->next;
           }
        } 
    }

    /*  FARSO_FOCUS ON WINDOW MOVIMENTATION  */
    if(focus == FARSO_FOCUS_WINDOW_MOVE)
    {
        if(!(ljan->getActiveWindow()->doMove(fundo,x,y,Mbotao)))
        {
           focus = FARSO_FOCUS_GAME;
        }
        eventInfo = FARSO_EVENT_MOVED_WINDOW;
        return(objAtivo);
    }

    /* FARSO_FOCUS ON BUTTON PRESSED */
    else
    if(focus == FARSO_FOCUS_BUTTON)
    {
        int pronto;
        button* bot = (button*)objAtivo;
        if (bot->press(ljan->getActiveWindow()->getX1(), 
                       ljan->getActiveWindow()->getY1(), x, y, 
                       Mbotao, &pronto, ljan->getActiveWindow()->getSurface()))
        {
           if(pronto)
           {
              if (bot->men != NULL)
              {
                 chamador = bot;
                 objAtivo = (guiObject*) bot->men;
                 menu* men = (menu*)objAtivo;
                 men->setPosition(bot->getX1(),bot->getY2()+1);
                 if (!bot->getText().compare("-"))
                 {
                    focus = FARSO_FOCUS_WINDOW_MENU;
                 }
                 else
                 {
                    focus = FARSO_FOCUS_MENU;
                 }                 
              }
              else if (!bot->getText().compare(fnt.createUnicode(0x25CF)))
              {
                   /* Close Window */
                  if(ljan->getActiveWindow()->canCloseWindow())
                  {
                     ljan->removeWindow(ljan->getActiveWindow());
                  }
                  focus = FARSO_FOCUS_GAME;
                  eventInfo = FARSO_EVENT_CLOSED_WINDOW;
                  return(NULL);
              }
              else
              {
                  focus = FARSO_FOCUS_GAME;
              }
              
              eventInfo = FARSO_EVENT_PRESSED_BUTTON;
              return(objAtivo);
           }
           else
           {
              /* Verify RolBar */
              verifyRolBars(ljan->getActiveWindow()->getObjectsList());
              eventInfo = FARSO_EVENT_ON_PRESS_BUTTON;
              return(objAtivo);
           }
        }
        else if(pronto)
        {
           focus = FARSO_FOCUS_GAME;
        }
        eventInfo = FARSO_EVENT_ON_PRESS_BUTTON;
        return(objAtivo);
    }
 
    /* FARSO_FOCUS ON BARTEXT WRITE */
    else 
    if (focus == FARSO_FOCUS_TEXT_BAR)
    {
        textBar* bart = (textBar*)objAtivo;
        if((bart->doWrite(x - ljan->getActiveWindow()->getX1(),
                          y - ljan->getActiveWindow()->getY1(),
                          ljan->getActiveWindow()->getSurface(), 
                          Mbotao,tecla)))
        {
           focus = FARSO_FOCUS_GAME;
           eventInfo = FARSO_EVENT_WROTE_TEXT_BAR;
           return(objAtivo);
        }
       eventInfo = FARSO_EVENT_WROTE_TEXT_BAR; 
       return(objAtivo);
    }
    
    /* FARSO_FOCUS ON RADIOBOXES */
    else 
    if(focus == FARSO_FOCUS_CX_SEL)
    {
       cxSel* cx = (cxSel*)objAtivo;
       cx->invertSelection();
       cx->draw(ljan->getActiveWindow()->getSurface());
       focus = FARSO_FOCUS_GAME;
       eventInfo = FARSO_EVENT_MODIFIED_CX_SEL;
       return(objAtivo);
    }

    /* FARSO_FOCUS ON MENUS */
    else
    if ((focus == FARSO_FOCUS_MENU) || (focus == FARSO_FOCUS_WINDOW_MENU))
    {
       int pronto;
       menu* men = (menu*)objAtivo;
      
       int res = men->run(x,y,Mbotao,tecla,
                          ljan->getActiveWindow()->getSurface(),&pronto,
                          ljan->getActiveWindow()->getX1(),
                          ljan->getActiveWindow()->getY1());

       eventInfo = FARSO_EVENT_MODIFIED_MENU;
       ljan->getActiveWindow()->setChanged();

        
       if( (focus == FARSO_FOCUS_WINDOW_MENU) && 
           (res == WINDOW_MENU_CLOSE) && (pronto))
       {
           if(ljan->getActiveWindow()->canCloseWindow())
           {
              ljan->removeWindow(ljan->getActiveWindow());
           }
           else
           {
              ljan->getActiveWindow()->draw(x,y);
           }
           focus = FARSO_FOCUS_GAME;
           eventInfo = FARSO_EVENT_CLOSED_WINDOW;
           return(NULL);
       }
       else if((res) && (pronto)) 
       {
          ljan->getActiveWindow()->draw(x,y);
          eventInfo = FARSO_EVENT_SELECTED_MENU;
          focus = FARSO_FOCUS_GAME;
       }
       else if(pronto)
       {
          ljan->getActiveWindow()->draw(x,y);
          focus = FARSO_FOCUS_GAME;
          eventInfo = FARSO_EVENT_SELECTED_MENU;
       }
       return(objAtivo);
    }

    /* FARSO_FOCUS ON TEXT SELECT  */
    else
    if ((focus == FARSO_FOCUS_SEL_TEXT) /*&& (x != mouseX || y != mouseY)*/ )
    {
        mouseX = x;
        mouseY = y;
        selText *st = (selText*)objAtivo;
        int res = st->treat(x-ljan->getActiveWindow()->getX1(),
                             y-ljan->getActiveWindow()->getY1(),
                             Mbotao,ljan->getActiveWindow()->getSurface());
        if(res == -1)
        {
            focus = FARSO_FOCUS_GAME;
            eventInfo = FARSO_EVENT_NONE;
            /* Redraw, since the last selected now is -1! */
            //ljan->getActiveWindow()->draw(0,0);
        }
        else if(res < 0)
        {
          eventInfo = FARSO_EVENT_MODIFIED_SEL_TEXT;
        }
        else
        {
          eventInfo = FARSO_EVENT_SELECTED_SEL_TEXT;
        }

        return(objAtivo);
    }

    /* FARSO_FOCUS ON TABBUTTON */
    else
    if ((focus == FARSO_FOCUS_TAB_BUTTON))
    {
       int actType = 0;
       tabButton* tb = (tabButton*) objAtivo;
       guiObject* object = tb->verifyPosition(x,y,Mbotao,
                                         ljan->getActiveWindow()->getX1(),
                                         ljan->getActiveWindow()->getY1(),
                                         ljan->getActiveWindow()->getSurface(),
                                         actType);
       if( object != NULL )
       {
         //selectObject = &object;
         if(actType == TABBUTTON_PRESSED)
         {
            bool verified = false;
            /* Is not a list text pressed, so return calling for treat 
             * the event! */
            if(!verified)
            {
               focus = FARSO_FOCUS_GAME;
               eventInfo = FARSO_EVENT_PRESSED_TAB_BUTTON;
               /* Change the saved mouse position to can take that it is
                * inner on a tabButton again. */
               mouseX = -1;
               return(object);
            }
         }
         else if(actType == TABBUTTON_ON_PRESS)
         {
            eventInfo = FARSO_EVENT_ON_PRESS_TAB_BUTTON;
            return(object);
         }  
         else
         {
            /* Still in focus, but no pressed */
            eventInfo = FARSO_EVENT_ON_FOCUS_TAB_BUTTON;
            return(object);
         }
       }
       else
       {
          if(!tb->isMouseIn(x-ljan->getActiveWindow()->getX1(),
                            y-ljan->getActiveWindow()->getY1()))
          {
             focus = FARSO_FOCUS_GAME;
          }
       }
    }
         
    eventInfo = FARSO_EVENT_NONE;
    return(NULL);
}

/*********************************************************************
 *                   Draw Graphic User guiInterface                     *
 *********************************************************************/
void guiInterface::draw(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   int aux;
   double depth = 0.012;
   window* jan = (window*) ljan->getFirst();

   if(ljan->getActiveWindow() == NULL)
     return;

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_DEPTH_TEST);

   /* Draw Inative Windows */
   for(aux = 0;aux<ljan->getTotal();aux++)
   {
      if( (jan != ljan->getActiveWindow()) && (jan->isVisible()) )
      {
         jan->render(depth);
          depth += 0.001;
      }
      jan = (window*) jan->next;
   }

   /* Draw Active Window */
   ljan->getActiveWindow()->render(depth);

   glEnable(GL_DEPTH_TEST);
   glDisable(GL_BLEND);

   glColor4f(1.0,1.0,1.0,1.0);
}

/*********************************************************************
 *                          clearActiveObject                        *
 *********************************************************************/
window* guiInterface::getActiveWindow()
{
   if(ljan)
   {
      return(ljan->getActiveWindow());
   }
   return(NULL);
}

/*********************************************************************
 *                          clearActiveObject                        *
 *********************************************************************/
void guiInterface::clearActiveObject()
{
   focus = FARSO_FOCUS_GAME;
   objAtivo = NULL;
}

/*********************************************************************
 *                             closeWindow                           *
 *********************************************************************/
void guiInterface::closeWindow(window *jan)
{
   clearActiveObject();
   ljan->removeWindow(jan);
}

/*********************************************************************
 *                           closeAllWindows                         *
 *********************************************************************/
void guiInterface::closeAllWindows()
{
   int i;
   int total = ljan->getTotal();

   window* j = (window*)ljan->getFirst();
   window* tmp;

   for(i = 0; i < total; i++)
   {
      tmp = j;
      j = (window*)j->next;
      closeWindow(tmp);
   }
   clearActiveObject();
}

/*********************************************************************
 *                            insertWindow                           *
 *********************************************************************/
window* guiInterface::insertWindow(int xa,int ya,int xb,int yb, string text)
{
   return(ljan->insertWindow(xa,ya,xb,yb,text));
}

/*********************************************************************
 *                              openWindow                           *
 *********************************************************************/
void guiInterface::openWindow(window* jan)
{
   jan->open();
}

/*********************************************************************
 *                              mouseOnGui                           *
 *********************************************************************/
bool guiInterface::mouseOnGui(int mouseX, int mouseY)
{
   int aux;
   window *jaux=(window*)ljan->getFirst();
   for(aux=0; aux < ljan->getTotal(); aux++)
   {
      if( (jaux->isVisible()) && (jaux->isMouseIn(mouseX,mouseY)) )
      {
         return(true);
      }
      jaux = (window*) jaux->next;
   }
   return(false);
}

/*********************************************************************
 *                               hideAll                             *
 *********************************************************************/
void guiInterface::hideAll()
{
   int aux;
   window *jaux=(window*)ljan->getFirst();
   for(aux=0; aux < ljan->getTotal(); aux++)
   {
      jaux->hide();
      jaux = (window*) jaux->next;
   }
}

/*********************************************************************
 *                               showAll                             *
 *********************************************************************/
void guiInterface::showAll()
{
   int aux;
   window *jaux=(window*)ljan->getFirst();
   for(aux=0; aux < ljan->getTotal(); aux++)
   {
      jaux->show();
      jaux = (window*) jaux->next;
   }
}

