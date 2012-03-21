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

#include "interface.h"
#include "messages.h"

#ifdef __APPLE__
   #include <SDL_image/SDL_image.h>
#else
   #include <SDL/SDL_image.h>
#endif

#include <iostream>
using namespace std;
using namespace Farso;

int mouseX=0,mouseY=0;

/*********************************************************************
 *                             GUI Constructor                       *
 *********************************************************************/
GuiInterface::GuiInterface(string backImage)
{
   lwindows = new WindowList();
   /* No surface, as will only has windows */
   objects = new GuiList(0,0, NULL, true);
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
   focus = FOCUS_GAME;
   activeObject = NULL;
}

/*********************************************************************
 *                           GUI Destructor                          *
 *********************************************************************/
GuiInterface::~GuiInterface()
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
void GuiInterface::verifyMouseInObjects(int x, int y, GuiList* list)
{
   /* Verify if the list is valid */
   if(list == NULL)
   {
      return;
   }

   std::list<GuiObject*>::iterator it;
   GuiObject *obj;
   for(it=list->list.begin(); it != list->list.end(); it++)
   {
      obj = (*it);
      /* Test selTexto */
      if(obj->type == OBJECT_SEL_TEXT) 
      {
         SelText* st = (SelText*) obj;
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
            focus = FOCUS_SEL_TEXT;
         }
      }
      /* Verify Button Table */
      else if(obj->type == OBJECT_TAB_BUTTON)
      {
         TabButton* tb = (TabButton*) obj;
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
            focus = FOCUS_TAB_BUTTON;
         }
      }
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
void GuiInterface::verifyMousePressObjects(int x, int y, GuiList* list)
{
   /* Verify if the GuiList is valid */
   if(!list)
   {
      return;
   }

   /* Here are the internal windows clicks verification */
   std::list<GuiObject*>::iterator it;
   GuiObject *obj;
   for(it=list->list.begin(); it != list->list.end(); it++)
   {
      obj = (*it);
      if(obj->type == OBJECT_BUTTON)
      {
         /* Verify Click on Button */
         Button* bot = (Button*) obj;
         if(bot->isMouseIn(x - lwindows->getActiveWindow()->getX1(),
                  y - lwindows->getActiveWindow()->getY1()))
         {

            activeObject = bot;
            focus = FOCUS_BUTTON;
         }
      }
      /* Verify Click on TextBar */ 
      else if(obj->type == OBJECT_TEXT_BAR)
      {
         TextBar* bart = (TextBar*) obj;
         if(bart->isMouseIn(x-lwindows->getActiveWindow()->getX1(),
                  y-lwindows->getActiveWindow()->getY1()))
         {
            activeObject = bart;
            bart->defineCursorPosition(x-
                  lwindows->getActiveWindow()->getX1(),
                  y-lwindows->getActiveWindow()->getY1());
            focus = FOCUS_TEXT_BAR;
         }
      }
      /* Verify RadioBoxes */
      else if(obj->type == OBJECT_SEL_BOX)
      {
         CxSel* cx = (CxSel*) obj;
         if(cx->isMouseIn(x-lwindows->getActiveWindow()->getX1(),
                  y-lwindows->getActiveWindow()->getY1()))
         {
            activeObject = cx;
            focus = FOCUS_CX_SEL;
         }
      }
      /* Verify Text Select */
      else if(obj->type == OBJECT_SEL_TEXT)
      {
         SelText* st = (SelText*) obj;
         int xa,ya,xb,yb;
         st->getCoordinate(xa,ya,xb,yb);
         if(isMouseAt(xa+lwindows->getActiveWindow()->getX1(),
                  ya+lwindows->getActiveWindow()->getY1(),
                  xb+lwindows->getActiveWindow()->getX1(),
                  yb+lwindows->getActiveWindow()->getY1(),x,y))
         {
            activeObject = st;
            focus = FOCUS_SEL_TEXT;
         }
      }
      /* Verify text boxes */
      else if(obj->type == OBJECT_TEXT_BOX)
      {
         TextBox* tb = (TextBox*)obj;
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
               focus = FOCUS_TEXT_BOX;
            }
         }
      }
      /* Verify pictures */
      else if(obj->type == OBJECT_PICTURE)
      {
         Picture* p = (Picture*)obj;
         int xa,ya,xb,yb;
         p->getCoordinate(xa,ya,xb,yb);

         if(isMouseAt(xa+lwindows->getActiveWindow()->getX1(),
                  ya+lwindows->getActiveWindow()->getY1(),
                  xb+lwindows->getActiveWindow()->getX1(),
                  yb+lwindows->getActiveWindow()->getY1(),x,y))
         {
            activeObject = p;
            focus = FOCUS_PICTURE;
         }
      }
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
GuiObject* GuiInterface::verifyTabBox(int x, int y, GuiList* list)
{
   if(!list)
   {
      return(NULL);
   }
   Window* actWindow = lwindows->getActiveWindow();

   /* Verify the tabBox */
   if(list->getTabBox() != NULL)
   {
      TabBox* tb = (TabBox*)list->getTabBox();
      if(tb->isMouseIn(x - actWindow->getX1(), y - actWindow->getY1()))
      {
         if(tb->verifyChanges(x - actWindow->getX1(), y - actWindow->getY1()))
         {
            return((GuiObject*)tb);
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
bool GuiInterface::verifyRolBars(int x, int y, GuiList* list)
{
   /* Verify if list exists */
   if(list == NULL)
   {
      return(false);
   }

   /* Now search it for rolBars */
   std::list<GuiObject*>::iterator it;
   GuiObject *obj;
   for(it=list->list.begin(); it != list->list.end(); it++)
   {
      obj = (*it);

      if(obj->type == OBJECT_ROL_BAR)
      {
         RolBar* rb = (RolBar*)obj;
         if(rb->eventGot(EVENT_ON_PRESS_BUTTON, activeObject, y))
         {
            lwindows->getActiveWindow()->draw(0,0);
            rb->redraw();
            return(true);
         }
      }
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
GuiObject* GuiInterface::manipulateEvents(int x, int y, Uint8 Mbotao, 
                                       Uint8* tecla, int& eventInfo)
{
   GuiObject* actObj = verifySingleEvents(x,y,Mbotao,tecla, eventInfo);

   if( (eventInfo != EVENT_NONE) && 
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
GuiObject* GuiInterface::verifyFileSelectorsEvents(GuiObject* actObj, 
                                                   int& eventInfo)
{
   GuiList* list = lwindows->getActiveWindow()->getObjectsList();
   std::list<GuiObject*>::iterator it;
   GuiObject *obj;
   /* pass all objects, treating file selectors */
   for(it=list->list.begin(); it != list->list.end(); it++)
   {
      obj = (*it);
      /* Verify OBJECT_FILE_SEL */
      if(obj->type == OBJECT_FILE_SEL)
      {
         FileSel* fs = (FileSel*)obj;
         if(fs->eventGot(eventInfo, actObj))
         {
            actObj = obj;
            switch(fs->getLastAction())
            {
               case FileSel::ACTION_ACCEPT:
                  eventInfo = EVENT_FILE_SEL_ACCEPT;
               break;
               case FileSel::ACTION_CANCEL:
                  eventInfo = EVENT_FILE_SEL_CANCEL;
               break;
               case FileSel::ACTION_SELECT:
                  eventInfo = EVENT_FILE_SEL_CHANGED;
               break;
               default:
                  eventInfo = EVENT_NONE;
               break;
            }
            focus = FOCUS_GAME;
         }
      }
   }
   return(actObj);
}

/*********************************************************************
 *                      verifyCompositeEvents                        *
 *********************************************************************/
GuiObject* GuiInterface::verifyCompositeEvents(GuiObject* actObj, 
                                               int& eventInfo)
{
   GuiList* list = lwindows->getActiveWindow()->getObjectsList();
   std::list<GuiObject*>::iterator it;
   GuiObject *obj;
   /* pass all objects, treating those composited */
   for(it=list->list.begin(); it != list->list.end(); it++)
   {
      obj = (*it);

      /* Verify OBJECT_LIST_TEXT */
      if(obj->type == OBJECT_LIST_TEXT)
      {
         ListText* lt = (ListText*)obj;
         if(lt->eventGot(eventInfo, actObj))
         {
            lwindows->getActiveWindow()->draw(0,0);
            eventInfo = EVENT_SELECTED_LIST_TEXT;
            focus = FOCUS_GAME;
            actObj = obj;
            return(lt);
         }
      }
   }

   /* Verify Warning Window */
   Warning warn;
   if( (warn.isOpened()) && (eventInfo == EVENT_PRESSED_BUTTON) &&
       (actObj == warn.getOkButton()) )
   {
      warn.close();
   }

   return(actObj);
}

/*********************************************************************
 *                        verifySingleEvents                         *
 *********************************************************************/
GuiObject* GuiInterface::verifySingleEvents(int x, int y, Uint8 Mbotao, 
                                         Uint8* tecla, int& eventInfo)
{
    Font fnt;

    if(!activeObject)
    {
       focus = FOCUS_GAME;
    }

    if(lwindows->getActiveWindow() == NULL)
    {
       eventInfo = EVENT_NONE;
       focus = FOCUS_GAME;
       return(NULL);
    }

    /* Keyboard Events */
    if ( (tecla[SDLK_ESCAPE]) && (focus != FOCUS_GAME))
    {
       focus = FOCUS_GAME;
       eventInfo = EVENT_EXIT;
       return(NULL);
    }

    /* Verify Window Super Menu */
    if(lwindows->getActiveWindow()->getObjectsList()->getMenu())
    {
       activeObject = (GuiObject*) 
                       lwindows->getActiveWindow()->getObjectsList()->getMenu();
       focus = FOCUS_MENU;
    }

    /* Mouse move to change focus */
    if( (x != mouseX || y != mouseY) && 
        (focus == FOCUS_GAME) )
    {
        mouseX = x;
        mouseY = y;

        if ((lwindows->getActiveWindow() != NULL) &&
            (lwindows->getActiveWindow()->isMouseIn(x,y)))
        {
            /* Verify All objects */
            Window* actWindow = lwindows->getActiveWindow();
            verifyMouseInObjects(x,y,actWindow->getObjectsList());
        }
    }

    /* Verify mouse button for focus change */
    if((Mbotao & SDL_BUTTON(1)) &&  (focus == FOCUS_GAME))
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
           activeObject = (GuiObject*) lwindows->getActiveWindow();
           focus = FOCUS_WINDOW_MOVE;
        }
        else if ( (lwindows->getActiveWindow() != NULL) &&
                  (lwindows->getActiveWindow()->isMouseIn(x,y)))
        {
            /* Will search at two lists: the widow one and the 
             * active tabBox one (if exists) */
            Window* actWindow = lwindows->getActiveWindow();
            verifyMousePressObjects(x,y,actWindow->getObjectsList());

            GuiObject* ot = verifyTabBox(x,y, actWindow->getObjectsList());
            if(ot)
            {
               /* got event on tabbox */
               eventInfo = EVENT_TAB_BOX_CHANGED;
               return(ot);
            }
            
            eventInfo = EVENT_CLICKED_WINDOW;
            return((GuiObject*) lwindows->getActiveWindow());
        }
        else if( (!lwindows->getActiveWindow()->isModal()) )
        {
           /* Test Other Windows Activation (if the current one 
            * isn't a modal window) */
           std::list<Window*>::iterator it;
           Window* jaux;
           for(it=lwindows->list.begin(); it != lwindows->list.end(); it++)
           {
              jaux = (*it);
               if( (jaux->isVisible()) &&
                   (jaux != lwindows->getActiveWindow())  && 
                   (jaux->isMouseIn(x,y)))
               {
                    focus = FOCUS_GAME;
                    jaux->activate();
                    eventInfo = EVENT_ACTIVATED_WINDOW;
                    mouseX = -1;
                    return((GuiObject*) jaux);
               }
           }
        } 
    }

    /*  FOCUS ON WINDOW MOVIMENTATION  */
    if(focus == FOCUS_WINDOW_MOVE)
    {
        if(!(lwindows->getActiveWindow()->doMove(background, x, y, Mbotao)))
        {
           focus = FOCUS_GAME;
        }
        eventInfo = EVENT_MOVED_WINDOW;
        return(activeObject);
    }

    /* FOCUS ON BUTTON PRESSED */
    else
    if(focus == FOCUS_BUTTON)
    {
        int pronto;
        Button* bot = (Button*)activeObject;
        
        if(bot->press(lwindows->getActiveWindow()->getX1(), 
                      lwindows->getActiveWindow()->getY1(), x, y, 
                      Mbotao, &pronto))
        {
           if(pronto)
           {
              if (bot->men != NULL)
              {
                 callerObject = bot;
                 activeObject = (GuiObject*) bot->men;
                 Menu* men = (Menu*)activeObject;
                 men->setPosition(bot->getX1(),bot->getY2()+1);
                 if (!bot->getText().compare("-"))
                 {
                    focus = FOCUS_WINDOW_MENU;
                 }
                 else
                 {
                    focus = FOCUS_MENU;
                 }                 
              }
              else if (!bot->getText().compare(fnt.createUnicode(0x25CF)))
              {
                   /* Close Window */
                  if(lwindows->getActiveWindow()->canCloseWindow())
                  {
                     lwindows->removeWindow(lwindows->getActiveWindow());
                  }
                  focus = FOCUS_GAME;
                  eventInfo = EVENT_CLOSED_WINDOW;
                  return(NULL);
              }
              else
              {
                  focus = FOCUS_GAME;
              }
              
              eventInfo = EVENT_PRESSED_BUTTON;
              return(activeObject);
           }
           else
           {
              /* Verify RolBar */
              verifyRolBars(x, y, 
                    lwindows->getActiveWindow()->getObjectsList());
              eventInfo = EVENT_ON_PRESS_BUTTON;
              return(activeObject);
           }
        }
        else if(pronto)
        {
           focus = FOCUS_GAME;
        }
        eventInfo = EVENT_ON_PRESS_BUTTON;
        return(activeObject);
    }
 
    /* FOCUS ON BARTEXT WRITE */
    else 
    if (focus == FOCUS_TEXT_BAR)
    {
        TextBar* bart = (TextBar*)activeObject;
        if((bart->doWrite(x - lwindows->getActiveWindow()->getX1(),
                          y - lwindows->getActiveWindow()->getY1(),
                          Mbotao,tecla)))
        {
           focus = FOCUS_GAME;
           eventInfo = EVENT_WROTE_TEXT_BAR;
           return(activeObject);
        }
       eventInfo = EVENT_ON_WRITE_TEXT_BAR; 
       return(activeObject);
    }
    
    /* FOCUS ON RADIOBOXES */
    else 
    if(focus == FOCUS_CX_SEL)
    {
       CxSel* cx = (CxSel*)activeObject;
       if(cx->doPress(Mbotao))
       {
          cx->draw();
          focus = FOCUS_GAME;
          eventInfo = EVENT_MODIFIED_CX_SEL;
       }
       else
       {
          eventInfo = EVENT_MODIFYING_CX_SEL;
       }
       return(activeObject);
    }

    /* FOCUS ON MENUS */
    else
    if ((focus == FOCUS_MENU) || (focus == FOCUS_WINDOW_MENU))
    {
       int pronto;
       Menu* men = (Menu*)activeObject;
      
       int res = men->run(x,y,Mbotao,tecla, &pronto,
                          lwindows->getActiveWindow()->getX1(),
                          lwindows->getActiveWindow()->getY1());

       eventInfo = EVENT_MODIFIED_MENU;
       lwindows->getActiveWindow()->setChanged();

        
       if( (focus == FOCUS_WINDOW_MENU) && 
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
           focus = FOCUS_GAME;
           eventInfo = EVENT_CLOSED_WINDOW;
           return(NULL);
       }
       else if((res) && (pronto)) 
       {
          lwindows->getActiveWindow()->draw(x,y);
          eventInfo = EVENT_SELECTED_MENU;
          focus = FOCUS_GAME;
       }
       else if(pronto)
       {
          lwindows->getActiveWindow()->draw(x,y);
          focus = FOCUS_GAME;
          eventInfo = EVENT_SELECTED_MENU;
       }
       return(activeObject);
    }

    /* FOCUS ON TEXT SELECT  */
    else if (focus == FOCUS_SEL_TEXT)
    {
        mouseX = x;
        mouseY = y;
        SelText* st = (SelText*)activeObject;
        int res = st->treat(x-lwindows->getActiveWindow()->getX1(),
                             y-lwindows->getActiveWindow()->getY1(),
                             Mbotao);
        if(res == -1)
        {
            focus = FOCUS_GAME;
            eventInfo = EVENT_NONE;
            /* Redraw, since the last selected now is -1! */
            //lwindows->getActiveWindow()->draw(0,0);
        }
        else if(res < 0)
        {
          eventInfo = EVENT_MODIFIED_SEL_TEXT;
        }
        else
        {
          eventInfo = EVENT_SELECTED_SEL_TEXT;
        }

        return(activeObject);
    }

    /* FOCUS ON TABBUTTON */
    else if(focus == FOCUS_TAB_BUTTON)
    {
       int actType = 0;
       TabButton* tb = (TabButton*) activeObject;
       GuiObject* object = tb->verifyPosition(x,y,Mbotao,
                                     lwindows->getActiveWindow()->getX1(),
                                     lwindows->getActiveWindow()->getY1(),
                                     actType);
       if( object != NULL )
       {
         //selectObject = &object;
         if( (actType == TABBUTTON_PRESSED) || 
             (actType == TABBUTTON_RIGHT_PRESSED) )
         {
            focus = FOCUS_GAME;
            if(actType == TABBUTTON_PRESSED)
            {
               eventInfo = EVENT_PRESSED_TAB_BUTTON;
            }
            else
            {
               eventInfo = EVENT_RIGHT_PRESSED_TAB_BUTTON;
            }
            /* Change the saved mouse position to can take that it is
             * inner on a tabButton again. */
            mouseX = -1;
            return(object);
         }
         else if(actType == TABBUTTON_ON_PRESS)
         {
            eventInfo = EVENT_ON_PRESS_TAB_BUTTON;
            return(object);
         }
         else if(actType == TABBUTTON_ON_RIGHT_PRESS)
         {
            eventInfo = EVENT_ON_RIGHT_PRESS_TAB_BUTTON;
            return(object);
         }
         else
         {
            /* Still in focus, but no pressed */
            eventInfo = EVENT_ON_FOCUS_TAB_BUTTON;
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
             focus = FOCUS_GAME;
          }
       }
    }

    /* Focus on TEXT_BOX */
    else
    if(focus == FOCUS_TEXT_BOX)
    {
       focus = FOCUS_GAME;
       eventInfo = EVENT_ON_PRESS_TEXT_BOX;
       return(activeObject);
    }

    /* Focus on PICTURE */
    else
    if(focus == FOCUS_PICTURE)
    {
       focus = FOCUS_GAME;
       eventInfo = EVENT_ON_PRESS_PICTURE;
       return(activeObject);
    }
         
    eventInfo = EVENT_NONE;
    return(NULL);
}

/*********************************************************************
 *                   Draw Graphic User GuiInterface                     *
 *********************************************************************/
void GuiInterface::draw(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   double depth = 0.012;
   Window* jan;

   if(lwindows->getActiveWindow() == NULL)
   {
      return;
   }

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_DEPTH_TEST);

   /* Draw Inative Windows */
   std::list<Window*>::iterator it;
   for(it=lwindows->list.begin(); it != lwindows->list.end(); it++)
   {
      jan = (*it);

      if( (jan != lwindows->getActiveWindow()) && (jan->isVisible()) )
      {
         jan->render(depth);
          depth += 0.001;
      }
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
Window* GuiInterface::getActiveWindow()
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
void GuiInterface::setActiveWindow(Window* w)
{
   if(lwindows)
   {
      lwindows->setActiveWindow(w);
   }
}

/*********************************************************************
 *                          clearActiveObject                        *
 *********************************************************************/
void GuiInterface::clearActiveObject()
{
   focus = FOCUS_GAME;
   activeObject = NULL;
}

/*********************************************************************
 *                             closeWindow                           *
 *********************************************************************/
void GuiInterface::closeWindow(Window* jan)
{
   clearActiveObject();
   lwindows->removeWindow(jan);
}

/*********************************************************************
 *                           closeAllWindows                         *
 *********************************************************************/
void GuiInterface::closeAllWindows()
{
   lwindows->clearList();
   clearActiveObject();
}

/*********************************************************************
 *                            insertWindow                           *
 *********************************************************************/
Window* GuiInterface::insertWindow(int xa,int ya,int xb,int yb, string text,
            bool empty)
{
   return(lwindows->insertWindow(xa,ya,xb,yb,text, empty));
}

/*********************************************************************
 *                              openWindow                           *
 *********************************************************************/
void GuiInterface::openWindow(Window* jan)
{
   jan->open();
}

/*********************************************************************
 *                              mouseOnGui                           *
 *********************************************************************/
bool GuiInterface::mouseOnGui(int mouseX, int mouseY)
{
   Window* jaux;
   std::list<Window*>::iterator it;
   for(it=lwindows->list.begin(); it != lwindows->list.end(); it++)
   {
      jaux = (*it);

      if( (jaux->isVisible()) && (jaux->isMouseIn(mouseX,mouseY)) )
      {
         return(true);
      }
   }
   return(false);
}

/*********************************************************************
 *                               hideAll                             *
 *********************************************************************/
void GuiInterface::hideAll()
{
   std::list<Window*>::iterator it;
   for(it=lwindows->list.begin(); it != lwindows->list.end(); it++)
   {
      (*it)->hide();
   }
}

/*********************************************************************
 *                               showAll                             *
 *********************************************************************/
void GuiInterface::showAll()
{
   std::list<Window*>::iterator it;
   for(it=lwindows->list.begin(); it != lwindows->list.end(); it++)
   {
      (*it)->show();
   }
}

