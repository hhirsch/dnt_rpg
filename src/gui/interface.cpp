/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "interface.h"
#include <SDL/SDL_image.h>

int mouseX=0,mouseY=0;

/*********************************************************************
 *                             GUI Constructor                       *
 *********************************************************************/
interface::interface(char* arqfundo)
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
   focus = FOCUS_GAME;
}

/*********************************************************************
 *                           GUI Destructor                          *
 *********************************************************************/
interface::~interface()
{
   delete(ljan);
   delete(objects);
   SDL_FreeSurface(fundo);
}

/*********************************************************************
 *                   Take care of GUI I/O Events                     *
 *********************************************************************/
guiObject* interface::manipulateEvents(int x, int y, Uint8 Mbotao, 
                                       Uint8* tecla, int* eventInfo)
{
    int aux;
    dntFont fnt;

    if(!objAtivo)
    {
       focus = FOCUS_GAME;
    }

    if(ljan->getActiveWindow() == NULL)
    {
       *eventInfo = NOTHING;
       focus = FOCUS_GAME;
       return(NULL);
    }

    /* Keyboard Events */
    if ( (tecla[SDLK_ESCAPE]) && (focus != FOCUS_GAME))
    {
       focus = FOCUS_GAME;
       *eventInfo = EXIT;
       return(NULL);
    }

    /* Verify Main Super Menu */
    if(ljan->getMenu())
    {
       objAtivo = (guiObject*) ljan->getMenu();
       focus = FOCUS_MENU;
    }
    else /* Verify Window Super Menu */
    if(ljan->getActiveWindow()->getObjectsList()->getMenu())
    {
       objAtivo = (guiObject*) 
                           ljan->getActiveWindow()->getObjectsList()->getMenu();
       focus = FOCUS_MENU;
    }

    /* Mouse move to change focus */
    if( (x != mouseX || y != mouseY) && 
        (focus == FOCUS_GAME) )
    {
        mouseX = x;
        mouseY = y;

        if ((ljan->getActiveWindow() != NULL) &&
            (ljan->getActiveWindow()->isMouseIn(x,y)))
        {
            /* Verify All objects */
            guiObject *obj = 
                    ljan->getActiveWindow()->getObjectsList()->getFirst()->next;
            for(aux=0; aux < 
                       ljan->getActiveWindow()->getObjectsList()->getTotal();
                aux++)
            {
               /* Test selTexto */
               if(obj->type == GUI_SEL_TEXT) 
               {
                  selText *st = (selText*) obj;
                  int xa,ya,xb,yb;
                  st->getCoordinate(xa,ya,xb,yb);
                  if(ljan->getActiveWindow()->isMouseIn(x,y))
                  {
                      objAtivo = st;
                      focus = FOCUS_SEL_TEXT;
                  }
               }
               /* Verify Button Table */
               else if(obj->type == GUI_TAB_BUTTON)
               {
                  tabButton *tb = (tabButton*) obj;
                  if(tb->isMouseIn(x-ljan->getActiveWindow()->getX1(),
                                   y-ljan->getActiveWindow()->getY1()))
                  {
                     objAtivo = tb;
                     focus = FOCUS_TAB_BUTTON;
                  }
               }
               obj = obj->next;
            }
        }
    }

    /* Verify mouse button for focus change */
    if((Mbotao & SDL_BUTTON(1)) &&  (focus == FOCUS_GAME))
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
           focus = FOCUS_WINDOW_MOVE;
        }
        else if ( (ljan->getActiveWindow() != NULL) &&
                  (ljan->getActiveWindow()->isMouseIn(x,y)))
        {
            /* Here are the internal windows clicks verification */
            guiObject *obj = 
                    ljan->getActiveWindow()->getObjectsList()->getFirst()->next;
            int aux;
            for(aux=0; aux < 
                   ljan->getActiveWindow()->getObjectsList()->getTotal(); aux++)
            {
               if(obj->type == GUI_BUTTON)
               {
                  /* Verify Click on Button */
                  button *bot = (button*) obj;
                  if(bot->isMouseIn(x - ljan->getActiveWindow()->getX1(),
                                    y - ljan->getActiveWindow()->getY1()))
                  {
                      
                     objAtivo = bot;
                     focus = FOCUS_BUTTON;
                  }
               }
               /* Verify Click on TextBar */ 
               else if(obj->type == GUI_TEXT_BAR)
               {
                   textBar *bart = (textBar*) obj;
                   if(bart->isMouseIn(x-ljan->getActiveWindow()->getX1(),
                                      y-ljan->getActiveWindow()->getY1()))
                   {
                       objAtivo = bart;
                       bart->defineCursorPosition(x-
                                            ljan->getActiveWindow()->getX1(),
                                            y-ljan->getActiveWindow()->getY1());
                       focus = FOCUS_TEXT_BAR;
                   }
               }
               /* Verify RadioBoxes */
               else if(obj->type == GUI_SEL_BOX)
               {
                   cxSel* cx = (cxSel*) obj;
                   if(cx->isMouseIn(x-ljan->getActiveWindow()->getX1(),
                                    y-ljan->getActiveWindow()->getY1()))
                   {
                       objAtivo = cx;
                       focus = FOCUS_CX_SEL;
                   }
               }
               /* Verify Text Select */
               else if(obj->type == GUI_SEL_TEXT)
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
                     focus = FOCUS_SEL_TEXT;
                  }
               }
               obj = obj->next;
            }
            *eventInfo = CLICKED_WINDOW;
            return((guiObject*) ljan->getActiveWindow());
        }
        else /*if( (ljan->getActiveWindow() != NULL))*/
        {
           /* Test Other Windows Activation */
           int aux; 
           window *jaux=(window*)ljan->getFirst()->next;
           for(aux=0;aux<ljan->getTotal();aux++)
           {
               if( (jaux != ljan->getActiveWindow())  && 
                   (jaux->isMouseIn(x,y)))
               {
                    focus = FOCUS_GAME;
                    jaux->activate();
                    *eventInfo = ACTIVATED_WINDOW;
                    mouseX = -1;
                    return((guiObject*) jaux);
               }
               jaux = (window*) jaux->next;
           }
        } 
    }

    /*  FOCUS ON WINDOW MOVIMENTATION  */
    if (focus == FOCUS_WINDOW_MOVE)
    {
        if(!(ljan->getActiveWindow()->doMove(fundo,x,y,Mbotao)))
        {
           focus = FOCUS_GAME;
        }
        *eventInfo = MOVED_WINDOW;
        return(objAtivo);
    }

    /* FOCUS ON BUTTON PRESSED */
    else
    if(focus == FOCUS_BUTTON)
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
                  if(ljan->getActiveWindow()->canCloseWindow())
                  {
                     ljan->removeWindow(ljan->getActiveWindow());
                  }
                  focus = FOCUS_GAME;
                  *eventInfo = CLOSED_WINDOW;
                  return(NULL);
              }
              else
              {
                  focus = FOCUS_GAME;
              }
              
              *eventInfo = PRESSED_BUTTON;
              return(objAtivo);
           }
           else
           {
              /* Verify RolBar */
              guiObject *obj = 
                    ljan->getActiveWindow()->getObjectsList()->getFirst()->next;
              int aux;
              for(aux=0; aux < 
                  ljan->getActiveWindow()->getObjectsList()->getTotal(); aux++)
              {
                 if(obj->type == GUI_ROL_BAR)
                 {
                    rolBar* rb = (rolBar*)obj;
                    if(rb->eventGot(ON_PRESS_BUTTON, objAtivo))
                    {
                       ljan->getActiveWindow()->draw(0,0);
                       rb->redraw();
                    }
                 }
                 obj = obj->next;
              }

              *eventInfo = ON_PRESS_BUTTON;
              return(objAtivo);
           }
        }
        else if(pronto)
        {
           focus = FOCUS_GAME;
        }
        *eventInfo = ON_PRESS_BUTTON;
        return(objAtivo);
    }
 
    /* FOCUS ON BARTEXT WRITE */
    else 
    if (focus == FOCUS_TEXT_BAR)
    {
        textBar* bart = (textBar*)objAtivo;
           if((bart->doWrite(x - ljan->getActiveWindow()->getX1(),
                           y - ljan->getActiveWindow()->getY1(),
                           ljan->getActiveWindow()->getSurface(), 
                           Mbotao,tecla)))
           {
               focus = FOCUS_GAME;
               *eventInfo = WROTE_TEXT_BAR;
               return(objAtivo);
           }
        *eventInfo = WROTE_TEXT_BAR; 
        return(objAtivo);
    }
    
    /* FOCUS ON RADIOBOXES */
    else 
    if(focus == FOCUS_CX_SEL)
    {
       cxSel* cx = (cxSel*)objAtivo;
       cx->invertSelection();
       cx->draw(ljan->getActiveWindow()->getSurface());
       focus = FOCUS_GAME;
       *eventInfo = MODIFIED_CX_SEL;
       return(objAtivo);
    }

    /* FOCUS ON MENUS */
    else
    if ((focus == FOCUS_MENU) || (focus == FOCUS_WINDOW_MENU))
    {
       int pronto;
       menu* men = (menu*)objAtivo;
      
       int res = men->run(x,y,Mbotao,tecla,
                          ljan->getActiveWindow()->getSurface(),&pronto,
                          ljan->getActiveWindow()->getX1(),
                          ljan->getActiveWindow()->getY1());

       *eventInfo = MODIFIED_MENU;

        
       if((focus == FOCUS_WINDOW_MENU) && (res==4) && (pronto))
       {
           if(ljan->getActiveWindow()->canCloseWindow())
           {
              ljan->removeWindow(ljan->getActiveWindow());
           }
           else
           {
              ljan->getActiveWindow()->draw(x,y);
           }
           focus = FOCUS_GAME;
           *eventInfo = CLOSED_WINDOW;
           return(NULL);
       }
       else if((res) && (pronto)) 
       {
          ljan->getActiveWindow()->draw(x,y);
          *eventInfo = SELECTED_MENU;
          focus = FOCUS_GAME;
       }
       else if(pronto)
       {
          ljan->getActiveWindow()->draw(x,y);
          focus = FOCUS_GAME;
          *eventInfo = SELECTED_MENU;
       }
       return(objAtivo);
    }

    /* FOCUS ON TEXT SELECT  */
    else
    if ((focus == FOCUS_SEL_TEXT) /*&& (x != mouseX || y != mouseY)*/ )
    {
        mouseX = x;
        mouseY = y;
        selText *st = (selText*)objAtivo;
        int res = st->treat(x-ljan->getActiveWindow()->getX1(),
                             y-ljan->getActiveWindow()->getY1(),
                             Mbotao,ljan->getActiveWindow()->getSurface());
        if(res == -1)
        {
            focus = FOCUS_GAME;
            *eventInfo = NOTHING;
            /* Redraw, since the last selected now is -1! */
            ljan->getActiveWindow()->draw(0,0);
        }
        else if(res < 0)
        {
          *eventInfo = MODIFIED_SEL_TEXT;
          ljan->getActiveWindow()->draw(0,0);
        }
        else
        {
          *eventInfo = SELECTED_SEL_TEXT;
        }

        
        return(objAtivo);
    }

    /* FOCUS ON TABBUTTON */
    else
    if ((focus == FOCUS_TAB_BUTTON))
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
            /* Verify List Text */
            guiObject *obj = 
                    ljan->getActiveWindow()->getObjectsList()->getFirst()->next;
            int aux;
            for(aux=0; aux <
                       ljan->getActiveWindow()->getObjectsList()->getTotal(); 
                aux++)
            {
               if(obj->type == GUI_LIST_TEXT)
               {
                  listText* lt = (listText*)obj;
                  if(lt->eventGot(PRESSED_TAB_BUTTON, object))
                  {
                     ljan->getActiveWindow()->draw(0,0);
                     verified = true;
                     *eventInfo = SELECTED_LIST_TEXT;
                     focus = FOCUS_GAME;
                     return(lt);
                  }
               }
               obj = obj->next;
            }
            
            /* Is not a list text pressed, so return calling for treat 
             * the event! */
            if(!verified)
            {
               focus = FOCUS_GAME;
               *eventInfo = PRESSED_TAB_BUTTON;
               /* Change the saved mouse position to can take that it is
                * inner on a tabButton again. */
               mouseX = -1;
               return(object);
            }
         }
         else if(actType == TABBUTTON_ON_PRESS)
         {
            *eventInfo = ON_PRESS_TAB_BUTTON;
            return(object);
         }  
       }
       else
       {
          if(!tb->isMouseIn(x-ljan->getActiveWindow()->getX1(),
                            y-ljan->getActiveWindow()->getY1()))
          {
             focus = FOCUS_GAME;
          }
       }
    }
         

    /* If here, no actions were made on GUI */
    *eventInfo = NOTHING;
    return(NULL);
}

/*********************************************************************
 *                   Draw Graphic User Interface                     *
 *********************************************************************/
void interface::draw(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   int aux;
   double profundidade = 0.012;
   window* jan = (window*) ljan->getFirst()->next;

   if(ljan->getActiveWindow() == NULL)
     return;

   //glColor4f(1.0,1.0,0.0,0.9);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_DEPTH_TEST);

   glDisable(GL_BLEND);

   /* Draw Inative Windows */
   for(aux = 0;aux<ljan->getTotal();aux++)
   {
      if(jan != ljan->getActiveWindow())
      {
         glRasterPos2f(jan->getX1(), SCREEN_Y-jan->getY1());
         glPixelZoom(1.0, -1.0);
         glDrawPixels((jan->getX2()-jan->getX1())+1,
                      (jan->getY2()-jan->getY1())+1, 
                      GL_RGBA, GL_UNSIGNED_BYTE, jan->getSurface()->pixels);
          profundidade += 0.001;
      }
      jan = (window*) jan->next;
   }

   /* Draw Active Window */
   glRasterPos2f(ljan->getActiveWindow()->getX1(), 
                 SCREEN_Y-ljan->getActiveWindow()->getY1());
   glPixelZoom(1.0, -1.0);
   glDrawPixels((ljan->getActiveWindow()->getX2() - 
                 ljan->getActiveWindow()->getX1())+1, 
                (ljan->getActiveWindow()->getY2() - 
                 ljan->getActiveWindow()->getY1())+1, 
                GL_RGBA, GL_UNSIGNED_BYTE, 
                ljan->getActiveWindow()->getSurface()->pixels);

   glEnable(GL_DEPTH_TEST);

   glColor4f(1.0,1.0,1.0,1.0);
}

/*********************************************************************
 *                          clearActiveObject                        *
 *********************************************************************/
void interface::clearActiveObject()
{
   focus = FOCUS_GAME;
   objAtivo = NULL;
}

/*********************************************************************
 *                             closeWindow                           *
 *********************************************************************/
void interface::closeWindow(window *jan)
{
   clearActiveObject();
   ljan->removeWindow(jan);
}

/*********************************************************************
 *                           closeAllWindows                         *
 *********************************************************************/
void interface::closeAllWindows()
{
   window* j = (window*)ljan->getFirst()->next;
   window* tmp;
   while(j != (window*)ljan->getFirst())
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
window* interface::insertWindow(int xa,int ya,int xb,int yb, string text)
{
   return(ljan->insertWindow(xa,ya,xb,yb,text));
}

/*********************************************************************
 *                              openWindow                           *
 *********************************************************************/
void interface::openWindow(window* jan)
{
   jan->open();
}

/*********************************************************************
 *                              mouseOnGui                           *
 *********************************************************************/
bool interface::mouseOnGui(int mouseX, int mouseY)
{
   int aux;
   window *jaux=(window*)ljan->getFirst()->next;
   for(aux=0;aux<ljan->getTotal();aux++)
   {
      if(jaux->isMouseIn(mouseX,mouseY))
      {
         return(true);
      }
      jaux = (window*) jaux->next;
   }
   return(false);
}

