/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "eventos.h"
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
   foco = FOCO_JOGO;
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
Tobjeto* interface::manipulateEvents(int x, int y, Uint8 Mbotao, Uint8* tecla,
                                     int* eventInfo)
{
    int aux;

    if(!objAtivo)
    {
       foco = FOCO_JOGO;
    }

    if(ljan->janelaAtiva == NULL)
    {
       *eventInfo = NADA;
       foco = FOCO_JOGO;
       return(NULL);
    }

    /* Keyboard Events */
    if ( (tecla[SDLK_ESCAPE]) && (foco != FOCO_JOGO))
    {
       foco = FOCO_JOGO;
       *eventInfo = SAIR;
       return(NULL);
    }

    /* Verify Main Super Menu */
    if(ljan->getMenu())
    {
       objAtivo = (Tobjeto*) ljan->getMenu();
       menu* men = (menu*)objAtivo;
       men->itemAtual = 1;
       foco = FOCO_MENU;
    }
    else /* Verify Window Super Menu */
    if(ljan->janelaAtiva->objects->getMenu())
    {
       objAtivo = (Tobjeto*) ljan->janelaAtiva->objects->getMenu();
       menu* men = (menu*)objAtivo;
       men->itemAtual = 1;
       foco = FOCO_MENU;
    }

    /* Mouse move to change focus */
    if( (x != mouseX || y != mouseY) && 
        (foco == FOCO_JOGO) )
    {
        mouseX = x;
        mouseY = y;
        
        if ((ljan->janelaAtiva != NULL) &&
             mouse_NaArea(ljan->janelaAtiva->x1,
                          ljan->janelaAtiva->y1,
                          ljan->janelaAtiva->x2, 
                          ljan->janelaAtiva->y2,
                          x, y))
        {
            /* Verify All objects */
            Tobjeto *obj = ljan->janelaAtiva->objects->primeiro->proximo;
            for(aux=0;aux<ljan->janelaAtiva->objects->total;aux++)
            {
               /* Test selTexto */
               if(obj->tipo == SELTEXTO) 
               {
                  selTexto *st = (selTexto*) obj;
                  int xa,ya,xb,yb;
                  st->getCoordinate(xa,ya,xb,yb);
                  if(mouse_NaArea(ljan->janelaAtiva->x1+xa,
                                  ljan->janelaAtiva->y1+ya,
                                  ljan->janelaAtiva->x1+xb, 
                                  ljan->janelaAtiva->y1+yb,x,y))
                  {
                      objAtivo = st;
                      foco = FOCO_SELTEXTO;
                  }
               }
               /* Verify Button Table */
               else if(obj->tipo == TABBOTAO)
               {
                  tabButton *tb = (tabButton*) obj;
                  if(mouse_NaArea(ljan->janelaAtiva->x1+tb->x1,
                                  ljan->janelaAtiva->y1+tb->y1,
                                  ljan->janelaAtiva->x1+tb->x2, 
                                  ljan->janelaAtiva->y1+tb->y2,x,y))
                  {
                     objAtivo = tb;
                     foco = FOCO_TABBUTTON;
                  }
               }
               obj = obj->proximo;
            }
        }
    }

    /* Verify mouse button for focus change */
    if((Mbotao & SDL_BUTTON(1)) &&  (foco == FOCO_JOGO))
    {
        if( ( (ljan->janelaAtiva != NULL) && (ljan->janelaAtiva->movivel) ) &&
             mouse_NaArea(ljan->janelaAtiva->x1+36,
                          ljan->janelaAtiva->y1, 
                          ljan->janelaAtiva->x2-3,
                          ljan->janelaAtiva->y1+12,x,y))
        {
            /* Active Window Moves */
            ljan->janelaAtiva->difx = x - ljan->janelaAtiva->x1;
            ljan->janelaAtiva->dify = y - ljan->janelaAtiva->y1;
            objAtivo = (Tobjeto*) ljan->janelaAtiva;
            foco = FOCO_JANELAMOVER;
        }
        else if ( (ljan->janelaAtiva != NULL) &&
                   mouse_NaArea(ljan->janelaAtiva->x1,
                                ljan->janelaAtiva->y1,
                                ljan->janelaAtiva->x2, 
                                ljan->janelaAtiva->y2,x,y))
        {
            /* Here are the internal windows clicks verification */
            Tobjeto *obj = ljan->janelaAtiva->objects->primeiro->proximo;
            int aux;
            for(aux=0; aux<ljan->janelaAtiva->objects->total; aux++)
            {
               if(obj->tipo == BOTAO)
               {
                  /* Verify Click on Button */
                  botao *bot = (botao*) obj;
                  if(mouse_NaArea(bot->x1+ljan->janelaAtiva->x1,
                                  bot->y1+ljan->janelaAtiva->y1,
                                  bot->x2+ljan->janelaAtiva->x1,
                                  bot->y2+ljan->janelaAtiva->y1,x,y))
                  {
                      
                     objAtivo = bot;
                     foco = FOCO_BOTAO;
                  }
               }
               /* Verify Click on TextBar */ 
               else if(obj->tipo == BARRATEXTO)
               {
                   barraTexto *bart = (barraTexto*) obj;
                   if(mouse_NaArea(bart->x1+ljan->janelaAtiva->x1,
                                   bart->y1+ljan->janelaAtiva->y1,
                                   bart->x2+ljan->janelaAtiva->x1,
                                   bart->y2+ljan->janelaAtiva->y1,x,y))
                   {
                       objAtivo = bart;
                       bart->pos = (mouseX-(ljan->janelaAtiva->x1+bart->x1+2)) 
                                               / (fonte_incCP()+1);
                       if (bart->pos > bart->texto.length()) 
                           bart->pos = bart->texto.length();
                       foco = FOCO_BARRATEXTO;
                   }
               }
               /* Verify RadioBoxes */
               else if(obj->tipo == CXSEL)
               {
                   cxSel* cx = (cxSel*) obj;
                   if(mouse_NaArea(cx->x+ljan->janelaAtiva->x1,
                                   cx->y+ljan->janelaAtiva->y1,
                                   cx->x+10+ljan->janelaAtiva->x1,
                                   cx->y+10+ljan->janelaAtiva->y1,x,y))
                   {
                       objAtivo = cx;
                       foco = FOCO_CXSEL;
                   }
               }
               /* Verify Text Select */
               else if(obj->tipo == SELTEXTO)
               {
                  selTexto* st = (selTexto*) obj;
                  int xa,ya,xb,yb;
                  st->getCoordinate(xa,ya,xb,yb);
                  if(mouse_NaArea(xa+ljan->janelaAtiva->x1,
                                  ya+ljan->janelaAtiva->y1,
                                  xb+ljan->janelaAtiva->x1,
                                  yb+ljan->janelaAtiva->y1,x,y))
                  {
                     objAtivo = st;
                     foco = FOCO_SELTEXTO;
                  }
               }
               obj = obj->proximo;
            }
            /* If out of for, without objects, call window external function */
            if(ljan->janelaAtiva->procPres != NULL)
            {
                ljan->janelaAtiva->procPres(ljan->janelaAtiva,x,y,NULL);
            }
            *eventInfo = JANELACLICADA;
            return((Tobjeto*) ljan->janelaAtiva);
        }
        else /*if( (ljan->janelaAtiva != NULL))*/
        {
           /* Test Other Windows Activation */
           int aux; 
           janela *jaux=(janela*)ljan->primeiro->proximo;
           for(aux=0;aux<ljan->total;aux++)
           {
               if( (jaux != ljan->janelaAtiva)  && 
                   mouse_NaArea(jaux->x1,jaux->y1,jaux->x2,jaux->y2,x,y))
               {
                    foco = FOCO_JOGO;
                    jaux->Ativar(ljan);
                    *eventInfo = JANELAATIVADA;
                    return((Tobjeto*) jaux);
               }
               jaux = (janela*) jaux->proximo;
           }
        } 
    }

    /*  FOCUS ON WINDOW MOVIMENTATION  */
    if (foco == FOCO_JANELAMOVER)
    {
        if(!(ljan->janelaAtiva->Mover(ljan,NULL,fundo,x,y,Mbotao)))
        {
           foco = FOCO_JOGO;
        }
        *eventInfo = JANELAMOVIMENTADA;
        return(objAtivo);
    }

    /* FOCUS ON BUTTON PRESSED */
    else
    if(foco == FOCO_BOTAO)
    {
        int pronto;
        botao* bot = (botao*)objAtivo;
        if (bot->Pressionar(ljan->janelaAtiva->x1,
                            ljan->janelaAtiva->y1,
                            ljan->janelaAtiva, x, y, Mbotao, 
                            &pronto))
        {
           if(pronto)
           {
              if(bot->procPres != NULL)
              {
                  bot->procPres(ljan->janelaAtiva,ljan,NULL);
                  foco = FOCO_JOGO;
              }
              else if (bot->men != NULL)
              {
                 chamador = bot;
                 objAtivo = (Tobjeto*) bot->men;
                 menu* men = (menu*)objAtivo;
                 men->Coordenada(bot->x1,bot->y2+1);
                 men->itemAtual = 1;
                 if (!bot->texto.compare("_"))
                    foco = FOCO_MENUJANELA;
                 else
                    foco = FOCO_MENU;
                 
              }
              else if (!bot->texto.compare("*"))
              {
                   /* Close Window */
                  if(ljan->janelaAtiva->fechavel)
                  {
                     ljan->janelaAtiva->Fechar(ljan);
                  }
                  foco = FOCO_JOGO;
                  *eventInfo = JANELAFECHADA;
                  return(NULL);
              }
              else
              {
                  foco = FOCO_JOGO;
              }
              
              *eventInfo = BOTAOPRESSIONADO;
              return(objAtivo);
           }
           else
           {
              /* Verify RolBar */
              Tobjeto *obj = ljan->janelaAtiva->objects->primeiro->proximo;
              int aux;
              for(aux=0; aux<ljan->janelaAtiva->objects->total; aux++)
              {
                 if(obj->tipo == ROLBAR)
                 {
                    rolBar* rb = (rolBar*)obj;
                    if(rb->eventGot(BOTAOEMPRESSAO, objAtivo))
                    {
                       ljan->janelaAtiva->Desenhar(0,0);
                       rb->redraw();
                    }
                 }
                 obj = obj->proximo;
              }

              *eventInfo = BOTAOEMPRESSAO;
              return(objAtivo);
           }
        }
        else if(pronto)
        {
           foco = FOCO_JOGO;
        }
        *eventInfo = BOTAOEMPRESSAO;
        return(objAtivo);
    }
 
    /* FOCUS ON BARTEXT WRITE */
    else 
    if (foco == FOCO_BARRATEXTO)
    {
        barraTexto* bart = (barraTexto*)objAtivo;
           if((bart->Escrever(ljan->janelaAtiva->x1,
                          ljan->janelaAtiva->y1,
                          x,y,ljan->janelaAtiva->cara,Mbotao,tecla)))
           {
               foco = FOCO_JOGO;
               if(bart->procEditada != NULL)
               {
                     bart->procEditada(bart,NULL);
               }
               *eventInfo = BARRATEXTOESCRITA;
               return(objAtivo);
           }
        *eventInfo = BARRATEXTOESCRITA; 
        return(objAtivo);
    }
    
    /* FOCUS ON RADIOBOXES */
    else 
    if(foco == FOCO_CXSEL)
    {
       cxSel* cx = (cxSel*)objAtivo;
       cx->invertSelection();
       cx->draw(ljan->janelaAtiva->cara);
       foco = FOCO_JOGO;
       *eventInfo = CXSELMODIFICADA;
       return(objAtivo);
    }

    /* FOCUS ON MENUS */
    else
    if ((foco == FOCO_MENU) || (foco == FOCO_MENUJANELA))
    {
       int pronto;
       menu* men = (menu*)objAtivo;
      
       int res = men->Rodar(x,y,Mbotao,tecla,
                            ljan->janelaAtiva->cara,&pronto,
                            ljan->janelaAtiva->x1,
                            ljan->janelaAtiva->y1);

       *eventInfo = MENUMODIFICADO;

        
       if((foco == FOCO_MENUJANELA) && (res==4) && (pronto))
       {
           if(ljan->janelaAtiva->fechavel)
              ljan->janelaAtiva->Fechar(ljan);
           else
              ljan->janelaAtiva->Desenhar(x,y);
           foco = FOCO_JOGO;
           *eventInfo = JANELAFECHADA;
           return(NULL);
       }
       else if((res) && (pronto)) 
       {
          if(men->procSelecionado)
          {
            men->procSelecionado(ljan->janelaAtiva,
                                 chamador,men->Item(res),
                                 NULL);
          }
          ljan->janelaAtiva->Desenhar(x,y);
          *eventInfo = MENUSELECIONADO;
          foco = FOCO_JOGO;
       }
       else if(pronto)
       {
          ljan->janelaAtiva->Desenhar(x,y);
          foco = FOCO_JOGO;
          *eventInfo = MENUSELECIONADO;
       }
       return(objAtivo);
    }

    /* FOCUS ON TEXT SELECT  */
    else
    if ((foco == FOCO_SELTEXTO) /*&& (x != mouseX || y != mouseY)*/ )
    {
        mouseX = x;
        mouseY = y;
        selTexto *st = (selTexto*)objAtivo;
        int res = st->threat(x-ljan->janelaAtiva->x1,y-ljan->janelaAtiva->y1,
                       Mbotao,ljan->janelaAtiva->cara);
        if(res == -1)
        {
            foco = FOCO_JOGO;
            *eventInfo = NADA; 
        }
        else if(res < 0)
        {
          *eventInfo = SELTEXTOMODIFICADA;
        }
        else
        {
          *eventInfo = SELTEXTOSELECIONADA;
        }

        
        return(objAtivo);
    }

    /* FOCUS ON TABBUTTON */
    else
    if ((foco == FOCO_TABBUTTON))
    {
       int actType = 0;
       tabButton* tb = (tabButton*) objAtivo;
       Tobjeto* object = tb->verifyPosition(x,y,Mbotao,
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
               Tobjeto *obj = ljan->janelaAtiva->objects->primeiro->proximo;
               int aux;
               for(aux=0; aux<ljan->janelaAtiva->objects->total; aux++)
               {
                  if(obj->tipo == LISTTEXT)
                  {
                     listText* lt = (listText*)obj;
                     if(lt->eventGot(TABBOTAOPRESSIONADO, object))
                     {
                        ljan->janelaAtiva->Desenhar(0,0);
                        verified = true;
                        *eventInfo = LISTTEXT_SELECTED;
                        foco = FOCO_JOGO;
                        return(lt);
                     }
                  }
                  obj = obj->proximo;
               }
               
               /* Is not a list text pressed, so return calling for treat 
                * the event! */
               if(!verified)
               {
                  foco = FOCO_JOGO;
                  *eventInfo = TABBOTAOPRESSIONADO;
                  return(object);
               }
            }
            else if(actType == TABBUTTON_ON_PRESS)
            {
               *eventInfo = TABBOTAOEMPRESSAO;
               return(object);
            }  
       }
       else
       {
            if(!mouse_NaArea(ljan->janelaAtiva->x1+tb->x1,
                             ljan->janelaAtiva->y1+tb->y1,
                             ljan->janelaAtiva->x1+tb->x2, 
                             ljan->janelaAtiva->y1+tb->y2,x,y))
            {
               foco = FOCO_JOGO;
            }
       }
    }
         

    /* If here, no actions were made on GUI */
    *eventInfo = NADA;
    return(NULL);
}

/*********************************************************************
 *                   Draw Graphic User Interface                     *
 *********************************************************************/
void interface::draw(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   int aux;
   double profundidade = 0.012;
   janela* jan = (janela*) ljan->primeiro->proximo;

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
      jan = (janela*) jan->proximo;
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
   foco = FOCO_JOGO;
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
   janela* j = (janela*)ljan->primeiro->proximo;
   janela* tmp;
   while(j != (janela*)ljan->primeiro)
   {
      tmp = j;
      j = (janela*)j->proximo;
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
   janela *jaux=(janela*)ljan->primeiro->proximo;
   for(aux=0;aux<ljan->total;aux++)
   {
      if(mouse_NaArea(jaux->x1,jaux->y1,jaux->x2,jaux->y2,mouseX,mouseY))
      {
         return(true);
      }
      jaux = (janela*) jaux->proximo;
   }
   return(false);
}

