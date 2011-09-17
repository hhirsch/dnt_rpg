/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#ifdef __APPLE__
   #include <SDL_image/SDL_image.h>
#else
   #include <SDL/SDL_image.h>
#endif

#include <iostream>
using namespace std;

int mouseX=0,mouseY=0;

/*********************************************************************
 *                             GUI Constructor                       *
 *********************************************************************/
guiInterface::guiInterface(string backImage)
{
   lwindows = new windowList;
   /* No surface, as will only has windows */
   objects = new guiList(0,0, NULL, true);
   if(!backImage.empty())
   {
      background = IMG_Load(backImage.c_str());
      if(!background)
      {
         cerr << "ERROR: Couldn't open background image: " << backImage << endl;
      }
   } 
   else
   {
       background = NULL;
   }
   focus = FARSO_FOCUS_GAME;
   activeObject = NULL;
}

/*********************************************************************
 *                           GUI Destructor                          *
 *********************************************************************/
guiInterface::~guiInterface()
{
   delete(lwindows);
   delete(objects);

   if(background)
   {
      SDL_FreeSurface(background);
   }
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
   obj = (guiObject*)list->getFirst();
   for(aux=0; aux < list->getTotal(); aux++)
   {
      /* Test selTexto */
      if(obj->type == FARSO_OBJECT_SEL_TEXT) 
      {
         selText *st = (selText*) obj;
         int xa,ya,xb,yb;
         st->getCoordinate(xa,ya,xb,yb);
         if(lwindows->getActiveWindow()->isMouseIn(x,y))
         {
            /* Set mouse hint */
            if(!st->getMouseHint().empty())
            {
               mouseCursor.setTextOver(st->getMouseHint());
            }
            /* Set active object */
            activeObject = st;
            focus = FARSO_FOCUS_SEL_TEXT;
         }
      }
      /* Verify Button Table */
      else if(obj->type == FARSO_OBJECT_TAB_BUTTON)
      {
         tabButton *tb = (tabButton*) obj;
         if(tb->isMouseIn(x-lwindows->getActiveWindow()->getX1(),
                  y-lwindows->getActiveWindow()->getY1()))
         {
            /* Set mouse hint if any */
            if(!tb->getMouseHint().empty())
            {
               mouseCursor.setTextOver(tb->getMouseHint());
            }

            /* Set active object */
            activeObject = tb;
            focus = FARSO_FOCUS_TAB_BUTTON;
         }
      }
      obj = (guiObject*)obj->getNext();
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
   guiObject *obj = (guiObject*)list->getFirst();
   int aux;

   for(aux=0; aux < list->getTotal(); aux++)
   {
      if(obj->type == FARSO_OBJECT_BUTTON)
      {
         /* Verify Click on Button */
         button *bot = (button*) obj;
         if(bot->isMouseIn(x - lwindows->getActiveWindow()->getX1(),
                  y - lwindows->getActiveWindow()->getY1()))
         {

            activeObject = bot;
            focus = FARSO_FOCUS_BUTTON;
         }
      }
      /* Verify Click on TextBar */ 
      else if(obj->type == FARSO_OBJECT_TEXT_BAR)
      {
         textBar *bart = (textBar*) obj;
         if(bart->isMouseIn(x-lwindows->getActiveWindow()->getX1(),
                  y-lwindows->getActiveWindow()->getY1()))
         {
            activeObject = bart;
            bart->defineCursorPosition(x-
                  lwindows->getActiveWindow()->getX1(),
                  y-lwindows->getActiveWindow()->getY1());
            focus = FARSO_FOCUS_TEXT_BAR;
         }
      }
      /* Verify RadioBoxes */
      else if(obj->type == FARSO_OBJECT_SEL_BOX)
      {
         cxSel* cx = (cxSel*) obj;
         if(cx->isMouseIn(x-lwindows->getActiveWindow()->getX1(),
                  y-lwindows->getActiveWindow()->getY1()))
         {
            activeObject = cx;
            focus = FARSO_FOCUS_CX_SEL;
         }
      }
      /* Verify Text Select */
      else if(obj->type == FARSO_OBJECT_SEL_TEXT)
      {
         selText* st = (selText*) obj;
         int xa,ya,xb,yb;
         st->getCoordinate(xa,ya,xb,yb);
         if(isMouseAt(xa+lwindows->getActiveWindow()->getX1(),
                  ya+lwindows->getActiveWindow()->getY1(),
                  xb+lwindows->getActiveWindow()->getX1(),
                  yb+lwindows->getActiveWindow()->getY1(),x,y))
         {
            activeObject = st;
            focus = FARSO_FOCUS_SEL_TEXT;
         }
      }
      /* Verify text boxes */
      else if(obj->type == FARSO_OBJECT_TEXT_BOX)
      {
         textBox* tb = (textBox*)obj;
         if(tb->receiveEvents())
         {
            int xa,ya,xb,yb;
            tb->getCoordinate(xa,ya,xb,yb);

            if(isMouseAt(xa+lwindows->getActiveWindow()->getX1(),
                     ya+lwindows->getActiveWindow()->getY1(),
                     xb+lwindows->getActiveWindow()->getX1(),
                     yb+lwindows->getActiveWindow()->getY1(),x,y))
            {
               activeObject = tb;
               focus = FARSO_FOCUS_TEXT_BOX;
            }
         }
      }
      /* Verify pictures */
      else if(obj->type == FARSO_OBJECT_PICTURE)
      {
         picture* p = (picture*)obj;
         int xa,ya,xb,yb;
         p->getCoordinate(xa,ya,xb,yb);

         if(isMouseAt(xa+lwindows->getActiveWindow()->getX1(),
                  ya+lwindows->getActiveWindow()->getY1(),
                  xb+lwindows->getActiveWindow()->getX1(),
                  yb+lwindows->getActiveWindow()->getY1(),x,y))
         {
            activeObject = p;
            focus = FARSO_FOCUS_PICTURE;
         }
      }
      obj = (guiObject*)obj->getNext();
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
   window* actWindow = lwindows->getActiveWindow();

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
bool guiInterface::verifyRolBars(int x, int y, guiList* list)
{
   /* Verify if list exists */
   if(list == NULL)
   {
      return(false);
   }

   /* Now search it for rolBars */
   guiObject *obj;
   obj = (guiObject*)list->getFirst();
   int aux;
   for(aux=0; aux < list->getTotal(); aux++)
   {
      if(obj->type == FARSO_OBJECT_ROL_BAR)
      {
         rolBar* rb = (rolBar*)obj;
         if(rb->eventGot(FARSO_EVENT_ON_PRESS_BUTTON, activeObject, y))
         {
            lwindows->getActiveWindow()->draw(0,0);
            rb->redraw();
            return(true);
         }
      }
      obj = (guiObject*)obj->getNext();
   }

   /* Verify Nested tabBox */
   if(list->getActiveTabBoxList() != NULL)
   {
      return(verifyRolBars(x, y, list->getActiveTabBoxList()));
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

   if( (eventInfo != FARSO_EVENT_NONE) && 
       (lwindows->getActiveWindow() != NULL) )
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
   guiObject *obj = (guiObject*)
                      lwindows->getActiveWindow()->getObjectsList()->getFirst();

   /* pass all objects, treating file selectors */
   for(aux=0; aux < lwindows->getActiveWindow()->getObjectsList()->getTotal(); 
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
      obj = (guiObject*)obj->getNext();
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
   guiObject *obj = (guiObject*)
                      lwindows->getActiveWindow()->getObjectsList()->getFirst();

   /* pass all objects, treating those composited */
   for(aux=0; aux < lwindows->getActiveWindow()->getObjectsList()->getTotal(); 
       aux++)
   {
      /* Verify FARSO_OBJECT_LIST_TEXT */
      if(obj->type == FARSO_OBJECT_LIST_TEXT)
      {
         listText* lt = (listText*)obj;
         if(lt->eventGot(eventInfo, actObj))
         {
            lwindows->getActiveWindow()->draw(0,0);
            eventInfo = FARSO_EVENT_SELECTED_LIST_TEXT;
            focus = FARSO_FOCUS_GAME;
            actObj = obj;
            return(lt);
         }
      }
      obj = (guiObject*)obj->getNext();
   }

   /* Verify Warning Window */
   warning warn;
   if( (warn.isOpened()) && (eventInfo == FARSO_EVENT_PRESSED_BUTTON) &&
       (actObj == warn.getOkButton()) )
   {
      warn.close();
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

    if(!activeObject)
    {
       focus = FARSO_FOCUS_GAME;
    }
    else if(!activeObject->getMouseHint().empty())
    {
       /* Set mouse hint, if any */
       mouseCursor.setTextOver(activeObject->getMouseHint());
    }

    if(lwindows->getActiveWindow() == NULL)
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

    /* Verify Window Super Menu */
    if(lwindows->getActiveWindow()->getObjectsList()->getMenu())
    {
       activeObject = (guiObject*) 
                       lwindows->getActiveWindow()->getObjectsList()->getMenu();
       focus = FARSO_FOCUS_MENU;
    }

    /* Mouse move to change focus */
    if( (x != mouseX || y != mouseY) && 
        (focus == FARSO_FOCUS_GAME) )
    {
        mouseX = x;
        mouseY = y;

        if ((lwindows->getActiveWindow() != NULL) &&
            (lwindows->getActiveWindow()->isMouseIn(x,y)))
        {
            /* Verify All objects */
            window* actWindow = lwindows->getActiveWindow();
            verifyMouseInObjects(x,y,actWindow->getObjectsList());
        }
    }

    /* Verify mouse button for focus change */
    if((Mbotao & SDL_BUTTON(1)) &&  (focus == FARSO_FOCUS_GAME))
    {
        if( ( (lwindows->getActiveWindow() != NULL) && 
              (lwindows->getActiveWindow()->canMoveWindow()) ) &&
              (isMouseAt(lwindows->getActiveWindow()->getX1()+36,
                         lwindows->getActiveWindow()->getY1(), 
                          lwindows->getActiveWindow()->getX2()-3,
                          lwindows->getActiveWindow()->getY1()+12,x,y)))
        {
           /* Active Window Moves */
           lwindows->getActiveWindow()->setDiff(
                 x - lwindows->getActiveWindow()->getX1(),
                 y - lwindows->getActiveWindow()->getY1());
           activeObject = (guiObject*) lwindows->getActiveWindow();
           focus = FARSO_FOCUS_WINDOW_MOVE;
        }
        else if ( (lwindows->getActiveWindow() != NULL) &&
                  (lwindows->getActiveWindow()->isMouseIn(x,y)))
        {
            /* Will search at two lists: the widow one and the 
             * active tabBox one (if exists) */
            window* actWindow = lwindows->getActiveWindow();
            verifyMousePressObjects(x,y,actWindow->getObjectsList());

            guiObject* ot = verifyTabBox(x,y, actWindow->getObjectsList());
            if(ot)
            {
               /* got event on tabbox */
               eventInfo = FARSO_EVENT_TAB_BOX_CHANGED;
               return(ot);
            }
            
            eventInfo = FARSO_EVENT_CLICKED_WINDOW;
            return((guiObject*) lwindows->getActiveWindow());
        }
        else if( (!lwindows->getActiveWindow()->isModal()) )
        {
           /* Test Other Windows Activation (if the current one 
            * isn't a modal window) */
           int aux; 
           window *jaux=(window*)lwindows->getFirst();
           for(aux=0; aux < lwindows->getTotal(); aux++)
           {
               if( (jaux->isVisible()) &&
                   (jaux != lwindows->getActiveWindow())  && 
                   (jaux->isMouseIn(x,y)))
               {
                    focus = FARSO_FOCUS_GAME;
                    jaux->activate();
                    eventInfo = FARSO_EVENT_ACTIVATED_WINDOW;
                    mouseX = -1;
                    return((guiObject*) jaux);
               }
               jaux = (window*) jaux->getNext();
           }
        } 
    }

    /*  FARSO_FOCUS ON WINDOW MOVIMENTATION  */
    if(focus == FARSO_FOCUS_WINDOW_MOVE)
    {
        if(!(lwindows->getActiveWindow()->doMove(background, x, y, Mbotao)))
        {
           focus = FARSO_FOCUS_GAME;
        }
        eventInfo = FARSO_EVENT_MOVED_WINDOW;
        return(activeObject);
    }

    /* FARSO_FOCUS ON BUTTON PRESSED */
    else
    if(focus == FARSO_FOCUS_BUTTON)
    {
        int pronto;
        button* bot = (button*)activeObject;
        
        if(bot->press(lwindows->getActiveWindow()->getX1(), 
                      lwindows->getActiveWindow()->getY1(), x, y, 
                      Mbotao, &pronto))
        {
           if(pronto)
           {
              if (bot->men != NULL)
              {
                 callerObject = bot;
                 activeObject = (guiObject*) bot->men;
                 menu* men = (menu*)activeObject;
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
                  if(lwindows->getActiveWindow()->canCloseWindow())
                  {
                     lwindows->removeWindow(lwindows->getActiveWindow());
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
              return(activeObject);
           }
           else
           {
              /* Verify RolBar */
              verifyRolBars(x, y, 
                    lwindows->getActiveWindow()->getObjectsList());
              eventInfo = FARSO_EVENT_ON_PRESS_BUTTON;
              return(activeObject);
           }
        }
        else if(pronto)
        {
           focus = FARSO_FOCUS_GAME;
        }
        eventInfo = FARSO_EVENT_ON_PRESS_BUTTON;
        return(activeObject);
    }
 
    /* FARSO_FOCUS ON BARTEXT WRITE */
    else 
    if (focus == FARSO_FOCUS_TEXT_BAR)
    {
        textBar* bart = (textBar*)activeObject;
        if((bart->doWrite(x - lwindows->getActiveWindow()->getX1(),
                          y - lwindows->getActiveWindow()->getY1(),
                          Mbotao,tecla)))
        {
           focus = FARSO_FOCUS_GAME;
           eventInfo = FARSO_EVENT_WROTE_TEXT_BAR;
           return(activeObject);
        }
       eventInfo = FARSO_EVENT_WROTE_TEXT_BAR; 
       return(activeObject);
    }
    
    /* FARSO_FOCUS ON RADIOBOXES */
    else 
    if(focus == FARSO_FOCUS_CX_SEL)
    {
       cxSel* cx = (cxSel*)activeObject;
       if(cx->doPress(Mbotao))
       {
          cx->draw();
          focus = FARSO_FOCUS_GAME;
          eventInfo = FARSO_EVENT_MODIFIED_CX_SEL;
       }
       else
       {
          eventInfo = FARSO_EVENT_MODIFYING_CX_SEL;
       }
       return(activeObject);
    }

    /* FARSO_FOCUS ON MENUS */
    else
    if ((focus == FARSO_FOCUS_MENU) || (focus == FARSO_FOCUS_WINDOW_MENU))
    {
       int pronto;
       menu* men = (menu*)activeObject;
      
       int res = men->run(x,y,Mbotao,tecla, &pronto,
                          lwindows->getActiveWindow()->getX1(),
                          lwindows->getActiveWindow()->getY1());

       eventInfo = FARSO_EVENT_MODIFIED_MENU;
       lwindows->getActiveWindow()->setChanged();

        
       if( (focus == FARSO_FOCUS_WINDOW_MENU) && 
           (res == WINDOW_MENU_CLOSE) && (pronto))
       {
           if(lwindows->getActiveWindow()->canCloseWindow())
           {
              lwindows->removeWindow(lwindows->getActiveWindow());
           }
           else
           {
              lwindows->getActiveWindow()->draw(x,y);
           }
           focus = FARSO_FOCUS_GAME;
           eventInfo = FARSO_EVENT_CLOSED_WINDOW;
           return(NULL);
       }
       else if((res) && (pronto)) 
       {
          lwindows->getActiveWindow()->draw(x,y);
          eventInfo = FARSO_EVENT_SELECTED_MENU;
          focus = FARSO_FOCUS_GAME;
       }
       else if(pronto)
       {
          lwindows->getActiveWindow()->draw(x,y);
          focus = FARSO_FOCUS_GAME;
          eventInfo = FARSO_EVENT_SELECTED_MENU;
       }
       return(activeObject);
    }

    /* FARSO_FOCUS ON TEXT SELECT  */
    else
    if ((focus == FARSO_FOCUS_SEL_TEXT) /*&& (x != mouseX || y != mouseY)*/ )
    {
        mouseX = x;
        mouseY = y;
        selText *st = (selText*)activeObject;
        int res = st->treat(x-lwindows->getActiveWindow()->getX1(),
                             y-lwindows->getActiveWindow()->getY1(),
                             Mbotao);
        if(res == -1)
        {
            focus = FARSO_FOCUS_GAME;
            eventInfo = FARSO_EVENT_NONE;
            /* Redraw, since the last selected now is -1! */
            //lwindows->getActiveWindow()->draw(0,0);
        }
        else if(res < 0)
        {
          eventInfo = FARSO_EVENT_MODIFIED_SEL_TEXT;
        }
        else
        {
          eventInfo = FARSO_EVENT_SELECTED_SEL_TEXT;
        }

        return(activeObject);
    }

    /* FARSO_FOCUS ON TABBUTTON */
    else
    if ((focus == FARSO_FOCUS_TAB_BUTTON))
    {
       int actType = 0;
       tabButton* tb = (tabButton*) activeObject;
       guiObject* object = tb->verifyPosition(x,y,Mbotao,
                                     lwindows->getActiveWindow()->getX1(),
                                     lwindows->getActiveWindow()->getY1(),
                                     actType);
       if( object != NULL )
       {
         //selectObject = &object;
         if( (actType == TABBUTTON_PRESSED) || 
             (actType == TABBUTTON_RIGHT_PRESSED) )
         {
            focus = FARSO_FOCUS_GAME;
            if(actType == TABBUTTON_PRESSED)
            {
               eventInfo = FARSO_EVENT_PRESSED_TAB_BUTTON;
            }
            else
            {
               eventInfo = FARSO_EVENT_RIGHT_PRESSED_TAB_BUTTON;
            }
            /* Change the saved mouse position to can take that it is
             * inner on a tabButton again. */
            mouseX = -1;
            return(object);
         }
         else if(actType == TABBUTTON_ON_PRESS)
         {
            eventInfo = FARSO_EVENT_ON_PRESS_TAB_BUTTON;
            return(object);
         }
         else if(actType == TABBUTTON_ON_RIGHT_PRESS)
         {
            eventInfo = FARSO_EVENT_ON_RIGHT_PRESS_TAB_BUTTON;
            return(object);
         }
         else
         {
            /* Still in focus, but no pressed */
            eventInfo = FARSO_EVENT_ON_FOCUS_TAB_BUTTON;
            /* Set mouse hint for the button area on tabButton (if any) */
            if( (object) && (!object->getMouseHint().empty()) )
            {
               mouseCursor.setTextOver(object->getMouseHint());
            }
            return(object);
         }
       }
       else
       {
          if(!tb->isMouseIn(x-lwindows->getActiveWindow()->getX1(),
                            y-lwindows->getActiveWindow()->getY1()))
          {
             focus = FARSO_FOCUS_GAME;
          }
       }
    }

    /* Focus on TEXT_BOX */
    else
    if(focus == FARSO_FOCUS_TEXT_BOX)
    {
       focus = FARSO_FOCUS_GAME;
       eventInfo = FARSO_EVENT_ON_PRESS_TEXT_BOX;
       return(activeObject);
    }

    /* Focus on PICTURE */
    else
    if(focus == FARSO_FOCUS_PICTURE)
    {
       focus = FARSO_FOCUS_GAME;
       eventInfo = FARSO_EVENT_ON_PRESS_PICTURE;
       return(activeObject);
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
   window* jan = (window*) lwindows->getFirst();

   if(lwindows->getActiveWindow() == NULL)
     return;

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_DEPTH_TEST);

   /* Draw Inative Windows */
   for(aux = 0;aux<lwindows->getTotal();aux++)
   {
      if( (jan != lwindows->getActiveWindow()) && (jan->isVisible()) )
      {
         jan->render(depth);
          depth += 0.001;
      }
      jan = (window*) jan->getNext();
   }

   /* Draw Active Window */
   lwindows->getActiveWindow()->render(depth);

   glEnable(GL_DEPTH_TEST);
   glDisable(GL_BLEND);

   glColor4f(1.0,1.0,1.0,1.0);
}

/*********************************************************************
 *                           getActiveWindow                         *
 *********************************************************************/
window* guiInterface::getActiveWindow()
{
   if(lwindows)
   {
      return(lwindows->getActiveWindow());
   }
   return(NULL);
}

/*********************************************************************
 *                           setActiveWindow                         *
 *********************************************************************/
void guiInterface::setActiveWindow(window* w)
{
   if(lwindows)
   {
      lwindows->setActiveWindow(w);
   }
}

/*********************************************************************
 *                          clearActiveObject                        *
 *********************************************************************/
void guiInterface::clearActiveObject()
{
   focus = FARSO_FOCUS_GAME;
   activeObject = NULL;
}

/*********************************************************************
 *                             closeWindow                           *
 *********************************************************************/
void guiInterface::closeWindow(window *jan)
{
   clearActiveObject();
   lwindows->removeWindow(jan);
}

/*********************************************************************
 *                           closeAllWindows                         *
 *********************************************************************/
void guiInterface::closeAllWindows()
{
   lwindows->clearList();
   clearActiveObject();
}

/*********************************************************************
 *                            insertWindow                           *
 *********************************************************************/
window* guiInterface::insertWindow(int xa,int ya,int xb,int yb, string text,
            bool empty)
{
   return(lwindows->insertWindow(xa,ya,xb,yb,text, empty));
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
   window *jaux=(window*)lwindows->getFirst();
   for(aux=0; aux < lwindows->getTotal(); aux++)
   {
      if( (jaux->isVisible()) && (jaux->isMouseIn(mouseX,mouseY)) )
      {
         return(true);
      }
      jaux = (window*) jaux->getNext();
   }
   return(false);
}

/*********************************************************************
 *                               hideAll                             *
 *********************************************************************/
void guiInterface::hideAll()
{
   int aux;
   window *jaux=(window*)lwindows->getFirst();
   for(aux=0; aux < lwindows->getTotal(); aux++)
   {
      jaux->hide();
      jaux = (window*) jaux->getNext();
   }
}

/*********************************************************************
 *                               showAll                             *
 *********************************************************************/
void guiInterface::showAll()
{
   int aux;
   window *jaux=(window*)lwindows->getFirst();
   for(aux=0; aux < lwindows->getTotal(); aux++)
   {
      jaux->show();
      jaux = (window*) jaux->getNext();
   }
}

