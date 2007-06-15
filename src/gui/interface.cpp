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
   ljan = new Ljanela;
   objects = new Tlista;
   if(arqfundo != NULL)
   {
      fundo = IMG_Load(arqfundo);
      if(!fundo) printf("Nao abri arquivo de fundo\n");
   } 
   else
   {
       /*fundo =  SDL_CreateRGBSurface(SDL_HWSURFACE,512, 512,32,0x000000FF, 
			0x0000FF00, 
			0x00FF0000, 
			0xFF000000);
       cor_Definir(0,0,0);
       retangulo_Colorir(fundo,0,0,511,511,0);*/
       fundo = NULL;
   }
   foco = FOCUS_GAME;
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
guiObject* interface::manipulateEvents(int x, int y, Uint8 Mbotao, Uint8* tecla,
                                     int* eventInfo)
{
    int aux;

    if(!objAtivo)
    {
       foco = FOCUS_GAME;
    }

    if(ljan->janelaAtiva == NULL)
    {
       *eventInfo = NOTHING;
       foco = FOCUS_GAME;
       return(NULL);
    }

    /* Keyboard Events */
    if ( (tecla[SDLK_ESCAPE]) && (foco != FOCUS_GAME))
    {
       foco = FOCUS_GAME;
       *eventInfo = EXIT;
       return(NULL);
    }

    /* Verify Main Super Menu */
    if(ljan->getMenu())
    {
       objAtivo = (guiObject*) ljan->getMenu();
       foco = FOCUS_MENU;
    }
    else /* Verify Window Super Menu */
    if(ljan->janelaAtiva->objects->getMenu())
    {
       objAtivo = (guiObject*) ljan->janelaAtiva->objects->getMenu();
       foco = FOCUS_MENU;
    }

    /* Mouse move to change focus */
    if( (x != mouseX || y != mouseY) && 
        (foco == FOCUS_GAME) )
    {
        mouseX = x;
        mouseY = y;
        
        if ((ljan->janelaAtiva != NULL) &&
             isMouseAt(ljan->janelaAtiva->x1,
                          ljan->janelaAtiva->y1,
                          ljan->janelaAtiva->x2, 
                          ljan->janelaAtiva->y2,
                          x, y))
        {
            /* Verify All objects */
            guiObject *obj = ljan->janelaAtiva->objects->first->next;
            for(aux=0;aux<ljan->janelaAtiva->objects->total;aux++)
            {
               /* Test selTexto */
               if(obj->type == GUI_SEL_TEXT) 
               {
                  selText *st = (selText*) obj;
                  int xa,ya,xb,yb;
                  st->getCoordinate(xa,ya,xb,yb);
                  if(isMouseAt(ljan->janelaAtiva->x1+xa,
                               ljan->janelaAtiva->y1+ya,
                               ljan->janelaAtiva->x1+xb, 
                               ljan->janelaAtiva->y1+yb,x,y))
                  {
                      objAtivo = st;
                      foco = FOCUS_SEL_TEXT;
                  }
               }
               /* Verify Button Table */
               else if(obj->type == GUI_TAB_BUTTON)
               {
                  tabButton *tb = (tabButton*) obj;
                  if(tb->isMouseIn(x-ljan->janelaAtiva->x1,
                                   y-ljan->janelaAtiva->y1))
                  {
                     objAtivo = tb;
                     foco = FOCUS_TAB_BUTTON;
                  }
               }
               obj = obj->next;
            }
        }
    }

    /* Verify mouse button for focus change */
    if((Mbotao & SDL_BUTTON(1)) &&  (foco == FOCUS_GAME))
    {
        if( ( (ljan->janelaAtiva != NULL) && (ljan->janelaAtiva->movivel) ) &&
             isMouseAt(ljan->janelaAtiva->x1+36,
                          ljan->janelaAtiva->y1, 
                          ljan->janelaAtiva->x2-3,
                          ljan->janelaAtiva->y1+12,x,y))
        {
            /* Active Window Moves */
            ljan->janelaAtiva->difx = x - ljan->janelaAtiva->x1;
            ljan->janelaAtiva->dify = y - ljan->janelaAtiva->y1;
            objAtivo = (guiObject*) ljan->janelaAtiva;
            foco = FOCUS_WINDOW_MOVE;
        }
        else if ( (ljan->janelaAtiva != NULL) &&
                   isMouseAt(ljan->janelaAtiva->x1,
                                ljan->janelaAtiva->y1,
                                ljan->janelaAtiva->x2, 
                                ljan->janelaAtiva->y2,x,y))
        {
            /* Here are the internal windows clicks verification */
            guiObject *obj = ljan->janelaAtiva->objects->first->next;
            int aux;
            for(aux=0; aux<ljan->janelaAtiva->objects->total; aux++)
            {
               if(obj->type == GUI_BUTTON)
               {
                  /* Verify Click on Button */
                  button *bot = (button*) obj;
                  if(bot->isMouseIn(x - ljan->janelaAtiva->x1,
                                    y - ljan->janelaAtiva->y1))
                  {
                      
                     objAtivo = bot;
                     foco = FOCUS_BUTTON;
                  }
               }
               /* Verify Click on TextBar */ 
               else if(obj->type == GUI_TEXT_BAR)
               {
                   textBar *bart = (textBar*) obj;
                   if(bart->isMouseIn(x-ljan->janelaAtiva->x1,
                                      y-ljan->janelaAtiva->y1))
                   {
                       objAtivo = bart;
                       bart->defineCursorPosition(x-ljan->janelaAtiva->x1,
                                                  y-ljan->janelaAtiva->y1);
                       foco = FOCUS_TEXT_BAR;
                   }
               }
               /* Verify RadioBoxes */
               else if(obj->type == GUI_SEL_BOX)
               {
                   cxSel* cx = (cxSel*) obj;
                   if(cx->isMouseIn(x-ljan->janelaAtiva->x1,
                                    y-ljan->janelaAtiva->y1))
                   {
                       objAtivo = cx;
                       foco = FOCUS_CX_SEL;
                   }
               }
               /* Verify Text Select */
               else if(obj->type == GUI_SEL_TEXT)
               {
                  selText* st = (selText*) obj;
                  int xa,ya,xb,yb;
                  st->getCoordinate(xa,ya,xb,yb);
                  if(isMouseAt(xa+ljan->janelaAtiva->x1,
                               ya+ljan->janelaAtiva->y1,
                               xb+ljan->janelaAtiva->x1,
                               yb+ljan->janelaAtiva->y1,x,y))
                  {
                     objAtivo = st;
                     foco = FOCUS_SEL_TEXT;
                  }
               }
               obj = obj->next;
            }
            /* If out of for, without objects, call window external function */
            if(ljan->janelaAtiva->procPres != NULL)
            {
                ljan->janelaAtiva->procPres(ljan->janelaAtiva,x,y,NULL);
            }
            *eventInfo = CLICKED_WINDOW;
            return((guiObject*) ljan->janelaAtiva);
        }
        else /*if( (ljan->janelaAtiva != NULL))*/
        {
           /* Test Other Windows Activation */
           int aux; 
           janela *jaux=(janela*)ljan->first->next;
           for(aux=0;aux<ljan->total;aux++)
           {
               if( (jaux != ljan->janelaAtiva)  && 
                   isMouseAt(jaux->x1,jaux->y1,jaux->x2,jaux->y2,x,y))
               {
                    foco = FOCUS_GAME;
                    jaux->Ativar(ljan);
                    *eventInfo = ACTIVATED_WINDOW;
                    return((guiObject*) jaux);
               }
               jaux = (janela*) jaux->next;
           }
        } 
    }

    /*  FOCUS ON WINDOW MOVIMENTATION  */
    if (foco == FOCUS_WINDOW_MOVE)
    {
        if(!(ljan->janelaAtiva->Mover(ljan,NULL,fundo,x,y,Mbotao)))
        {
           foco = FOCUS_GAME;
        }
        *eventInfo = MOVED_WINDOW;
        return(objAtivo);
    }

    /* FOCUS ON BUTTON PRESSED */
    else
    if(foco == FOCUS_BUTTON)
    {
        int pronto;
        button* bot = (button*)objAtivo;
        if (bot->press(ljan->janelaAtiva->x1, ljan->janelaAtiva->y1, x, y, 
                       Mbotao, &pronto, ljan->janelaAtiva->cara))
        {
           if(pronto)
           {
              if (bot->men != NULL)
              {
                 chamador = bot;
                 objAtivo = (guiObject*) bot->men;
                 menu* men = (menu*)objAtivo;
                 men->setPosition(bot->getX1(),bot->getY2()+1);
                 if (!bot->getText().compare("_"))
                 {
                    foco = FOCUS_WINDOW_MENU;
                 }
                 else
                 {
                    foco = FOCUS_MENU;
                 }                 
              }
              else if (!bot->getText().compare("*"))
              {
                   /* Close Window */
                  if(ljan->janelaAtiva->fechavel)
                  {
                     ljan->janelaAtiva->Fechar(ljan);
                  }
                  foco = FOCUS_GAME;
                  *eventInfo = CLOSED_WINDOW;
                  return(NULL);
              }
              else
              {
                  foco = FOCUS_GAME;
              }
              
              *eventInfo = PRESSED_BUTTON;
              return(objAtivo);
           }
           else
           {
              /* Verify RolBar */
              guiObject *obj = ljan->janelaAtiva->objects->first->next;
              int aux;
              for(aux=0; aux<ljan->janelaAtiva->objects->total; aux++)
              {
                 if(obj->type == GUI_ROL_BAR)
                 {
                    rolBar* rb = (rolBar*)obj;
                    if(rb->eventGot(ON_PRESS_BUTTON, objAtivo))
                    {
                       ljan->janelaAtiva->Desenhar(0,0);
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
           foco = FOCUS_GAME;
        }
        *eventInfo = ON_PRESS_BUTTON;
        return(objAtivo);
    }
 
    /* FOCUS ON BARTEXT WRITE */
    else 
    if (foco == FOCUS_TEXT_BAR)
    {
        textBar* bart = (textBar*)objAtivo;
           if((bart->write(x - ljan->janelaAtiva->x1,
                           y - ljan->janelaAtiva->y1,
                           ljan->janelaAtiva->cara, Mbotao,tecla)))
           {
               foco = FOCUS_GAME;
               *eventInfo = WROTE_TEXT_BAR;
               return(objAtivo);
           }
        *eventInfo = WROTE_TEXT_BAR; 
        return(objAtivo);
    }
    
    /* FOCUS ON RADIOBOXES */
    else 
    if(foco == FOCUS_CX_SEL)
    {
       cxSel* cx = (cxSel*)objAtivo;
       cx->invertSelection();
       cx->draw(ljan->janelaAtiva->cara);
       foco = FOCUS_GAME;
       *eventInfo = MODIFIED_CX_SEL;
       return(objAtivo);
    }

    /* FOCUS ON MENUS */
    else
    if ((foco == FOCUS_MENU) || (foco == FOCUS_WINDOW_MENU))
    {
       int pronto;
       menu* men = (menu*)objAtivo;
      
       int res = men->run(x,y,Mbotao,tecla,
                          ljan->janelaAtiva->cara,&pronto,
                          ljan->janelaAtiva->x1,
                          ljan->janelaAtiva->y1);

       *eventInfo = MODIFIED_MENU;

        
       if((foco == FOCUS_WINDOW_MENU) && (res==4) && (pronto))
       {
           if(ljan->janelaAtiva->fechavel)
              ljan->janelaAtiva->Fechar(ljan);
           else
              ljan->janelaAtiva->Desenhar(x,y);
           foco = FOCUS_GAME;
           *eventInfo = CLOSED_WINDOW;
           return(NULL);
       }
       else if((res) && (pronto)) 
       {
          ljan->janelaAtiva->Desenhar(x,y);
          *eventInfo = SELECTED_MENU;
          foco = FOCUS_GAME;
       }
       else if(pronto)
       {
          ljan->janelaAtiva->Desenhar(x,y);
          foco = FOCUS_GAME;
          *eventInfo = SELECTED_MENU;
       }
       return(objAtivo);
    }

    /* FOCUS ON TEXT SELECT  */
    else
    if ((foco == FOCUS_SEL_TEXT) /*&& (x != mouseX || y != mouseY)*/ )
    {
        mouseX = x;
        mouseY = y;
        selText *st = (selText*)objAtivo;
        int res = st->threat(x-ljan->janelaAtiva->x1,
                             y-ljan->janelaAtiva->y1,
                             Mbotao,ljan->janelaAtiva->cara);
        if(res == -1)
        {
            foco = FOCUS_GAME;
            *eventInfo = NOTHING; 
        }
        else if(res < 0)
        {
          *eventInfo = MODIFIED_SEL_TEXT;
        }
        else
        {
          *eventInfo = SELECTED_SEL_TEXT;
        }

        
        return(objAtivo);
    }

    /* FOCUS ON TABBUTTON */
    else
    if ((foco == FOCUS_TAB_BUTTON))
    {
       int actType = 0;
       tabButton* tb = (tabButton*) objAtivo;
       guiObject* object = tb->verifyPosition(x,y,Mbotao,
                                            ljan->janelaAtiva->x1,
                                            ljan->janelaAtiva->y1,
                                            ljan->janelaAtiva->cara,
                                            actType);
       if( object != NULL )
       {
            //selectObject = &object;
            if(actType == TABBUTTON_PRESSED)
            {
               bool verified = false;
               /* Verify List Text */
               guiObject *obj = ljan->janelaAtiva->objects->first->next;
               int aux;
               for(aux=0; aux<ljan->janelaAtiva->objects->total; aux++)
               {
                  if(obj->type == GUI_LIST_TEXT)
                  {
                     listText* lt = (listText*)obj;
                     if(lt->eventGot(PRESSED_TAB_BUTTON, object))
                     {
                        ljan->janelaAtiva->Desenhar(0,0);
                        verified = true;
                        *eventInfo = SELECTED_LIST_TEXT;
                        foco = FOCUS_GAME;
                        return(lt);
                     }
                  }
                  obj = obj->next;
               }
               
               /* Is not a list text pressed, so return calling for treat 
                * the event! */
               if(!verified)
               {
                  foco = FOCUS_GAME;
                  *eventInfo = PRESSED_TAB_BUTTON;
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
            if(!tb->isMouseIn(x-ljan->janelaAtiva->x1,
                              y-ljan->janelaAtiva->y1))
            {
               foco = FOCUS_GAME;
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
   janela* jan = (janela*) ljan->first->next;

   if(ljan->janelaAtiva == NULL)
     return;

   //glColor4f(1.0,1.0,0.0,0.9);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_DEPTH_TEST);

   glDisable(GL_BLEND);
   
   /* Draw Inative Windows */
   for(aux = 0;aux<ljan->total;aux++)
   {
      if(jan != ljan->janelaAtiva)
      {
         glRasterPos2f(jan->x1, 600-jan->y1);
         glPixelZoom(1.0, -1.0);
         glDrawPixels((jan->x2-jan->x1)+1, (jan->y2-jan->y1)+1, 
                      GL_RGBA, GL_UNSIGNED_BYTE, jan->cara->pixels);
          /*AtualizaTela2D(jan->caraTextura,proj,modl,viewPort,jan->x1,jan->y1, 
                         jan->x2,jan->y2,profundidade);*/
          profundidade += 0.001;
      }
      jan = (janela*) jan->next;
   }

   /* Draw Active Window */
   glRasterPos2f(ljan->janelaAtiva->x1, 600-ljan->janelaAtiva->y1);
   glPixelZoom(1.0, -1.0);
   glDrawPixels((ljan->janelaAtiva->x2 - ljan->janelaAtiva->x1)+1, 
                (ljan->janelaAtiva->y2 - ljan->janelaAtiva->y1)+1, 
                GL_RGBA, GL_UNSIGNED_BYTE, ljan->janelaAtiva->cara->pixels);

   glEnable(GL_DEPTH_TEST);

   glColor4f(1.0,1.0,1.0,1.0);

   /*AtualizaTela2D(ljan->janelaAtiva->caraTextura,proj,modl,viewPort,
                     ljan->janelaAtiva->x1,ljan->janelaAtiva->y1,
                     ljan->janelaAtiva->x2,ljan->janelaAtiva->y2, 0.011);*/
}

/*********************************************************************
 *                          clearActiveObject                        *
 *********************************************************************/
void interface::clearActiveObject()
{
   foco = FOCUS_GAME;
   objAtivo = NULL;
}

/*********************************************************************
 *                             closeWindow                           *
 *********************************************************************/
void interface::closeWindow(janela *jan)
{
   clearActiveObject();
   ljan->RetirarJanela(jan);
}

/*********************************************************************
 *                           closeAllWindows                         *
 *********************************************************************/
void interface::closeAllWindows()
{
   janela* j = (janela*)ljan->first->next;
   janela* tmp;
   while(j != (janela*)ljan->first)
   {
      tmp = j;
      j = (janela*)j->next;
      closeWindow(tmp);
   }
   clearActiveObject();
}

/*********************************************************************
 *                            insertWindow                           *
 *********************************************************************/
janela* interface::insertWindow(int xa,int ya,int xb,int yb,const char *text,
                            int maximiz,int redmens)
{
   return(ljan->InserirJanela(xa,ya,xb,yb,text,maximiz,redmens, NULL, NULL));
}

/*********************************************************************
 *                              openWindow                           *
 *********************************************************************/
void interface::openWindow(janela* jan)
{
   jan->Abrir(ljan);
}

/*********************************************************************
 *                              mouseOnGui                           *
 *********************************************************************/
bool interface::mouseOnGui(int mouseX, int mouseY)
{
   int aux; 
   janela *jaux=(janela*)ljan->first->next;
   for(aux=0;aux<ljan->total;aux++)
   {
      if(isMouseAt(jaux->x1,jaux->y1,jaux->x2,jaux->y2,mouseX,mouseY))
      {
         return(true);
      }
      jaux = (janela*) jaux->next;
   }
   return(false);
}

