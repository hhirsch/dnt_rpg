/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "eventos.h"

int mouseX=0,mouseY=0;

/*********************************************************************
 *                             GUI Constructor                       *
 *********************************************************************/
interface::interface(char* arqfundo)
{
   ljan = new Ljanela;
   objetos = new Tlista;
   if(arqfundo != NULL)
   {
      fundo = SDL_LoadBMP(arqfundo);
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
   delete(objetos);
   SDL_FreeSurface(fundo);
}

/*********************************************************************
 *                   Take care of GUI I/O Events                     *
 *********************************************************************/
int interface::ManipulaEventos(int x, int y, Uint8 Mbotao, Uint8* tecla)
{
    int aux;
    if(ljan->janelaAtiva == NULL)
       return(NADA);

    /* Keyboard Events */
    if ( (tecla[SDLK_ESCAPE]) && (foco != FOCO_JOGO))
    {
       foco = FOCO_JOGO;
       return(SAIR);
    }

    /* Mouse move to change focus */
    if( (x != mouseX || y != mouseY) && 
        (foco == FOCO_JOGO) )
    {
        mouseX = x;
        mouseY = y;

        /* Test selTexto */
        if ((ljan->janelaAtiva != NULL) &&
             mouse_NaArea(ljan->janelaAtiva->x1,
                          ljan->janelaAtiva->y1,
                          ljan->janelaAtiva->x2, 
                          ljan->janelaAtiva->y2,
                          x, y))
        {
            Tobjeto *obj = ljan->janelaAtiva->objetos->primeiro->proximo;
            for(aux=0;aux<ljan->janelaAtiva->objetos->total;aux++)
            {
               if(obj->tipo == SELTEXTO)
               {
                  selTexto *st = (selTexto*) obj;
                  if(mouse_NaArea(ljan->janelaAtiva->x1+st->x1,
                                  ljan->janelaAtiva->y1+st->y1,
                                  ljan->janelaAtiva->x1+st->x2, 
                                  ljan->janelaAtiva->y1+st->y2,x,y))
                  {
                      objAtivo = st;
                      foco = FOCO_SELTEXTO;
                  }
               }
               obj = obj->proximo;
            }
        }
    }

    /* Verify mouse button for focus change */
    if((Mbotao & SDL_BUTTON(1)) &&  (foco == FOCO_JOGO))
    {
        if( (ljan->janelaAtiva != NULL) &&
             mouse_NaArea(ljan->janelaAtiva->x1+36,
                          ljan->janelaAtiva->y1, 
                          ljan->janelaAtiva->x2-3,
                          ljan->janelaAtiva->y1+12,x,y))
        {
            /* Active Window Moves */
            ljan->janelaAtiva->difx = x - ljan->janelaAtiva->x1;
            ljan->janelaAtiva->dify = y - ljan->janelaAtiva->y1;
            foco = FOCO_JANELAMOVER;
        }
        else if ( (ljan->janelaAtiva != NULL) &&
                   mouse_NaArea(ljan->janelaAtiva->x1,
                                ljan->janelaAtiva->y1,
                                ljan->janelaAtiva->x2, 
                                ljan->janelaAtiva->y2,x,y))
        {
            /* Here are the internal windows clicks verification */
            Tobjeto *obj = ljan->janelaAtiva->objetos->primeiro->proximo;
            int aux;
            for(aux=0; aux<ljan->janelaAtiva->objetos->total; aux++)
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
                       if (bart->pos>strlen(bart->texto)) 
                           bart->pos = strlen(bart->texto);
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
               else if(obj->tipo == SELTEXTO)
               {
                  selTexto* st = (selTexto*) obj;
                  if(mouse_NaArea(st->x1+ljan->janelaAtiva->x1,
                                  st->y1+ljan->janelaAtiva->y1,
                                  st->x2+ljan->janelaAtiva->x1,
                                  st->y2+ljan->janelaAtiva->y1,x,y))
                  {
                     objAtivo = st;
                     if(st->procPres != NULL)
                        st->procPres(NULL, 
                            st->Selecionada(y,ljan->janelaAtiva->y1));
                     foco = FOCO_SELTEXTO;
                  }
               }
               obj = obj->proximo;
            }
            /* If out of for, without objects, call window external function */
            if(ljan->janelaAtiva->procPres != NULL)
            {
                ljan->janelaAtiva->procPres(ljan->janelaAtiva,x,y,NULL);
                return(JANELACLICADA);
            }
            return(JANELACLICADA);
        }
        else if ( (ljan->janelaAtiva != NULL))
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
                    return(JANELAATIVADA);
               }
               jaux = (janela*) jaux->proximo;
           }
        } 
    }

    /*  FOCUS ON WINDOW MOVIMENTATION  */
    if (foco == FOCO_JANELAMOVER)
    {
        if(!(ljan->janelaAtiva->Mover(ljan,NULL,fundo,x,y,Mbotao)))
           foco = FOCO_JOGO;
        return(JANELAMOVIMENTADA);
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
                  return(JANELAFECHADA);
              }
              else
                  foco = FOCO_JOGO;
           }
        }
        else if(pronto)
           foco = FOCO_JOGO;
        return(BOTAOPRESSIONADO);
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
               ljan->janelaAtiva->AtualizaCara();
               return(BARRATEXTOESCRITA);
           }
        ljan->janelaAtiva->AtualizaCara();
        return(BARRATEXTOESCRITA);
    }
    
    /* FOCUS ON RADIOBOXES */
    else 
    if(foco == FOCO_CXSEL)
    {
       cxSel* cx = (cxSel*)objAtivo;
       cx->selecionada = !cx->selecionada;
       cx->Desenhar(ljan->janelaAtiva->cara);
       ljan->janelaAtiva->AtualizaCara();
       foco = FOCO_JOGO;
       return(CXSELMODIFICADA);
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

       ljan->janelaAtiva->AtualizaCara();

        
       if((foco == FOCO_MENUJANELA) && (res==4) && (pronto))
       {
           if(ljan->janelaAtiva->fechavel)
              ljan->janelaAtiva->Fechar(ljan);
           else
              ljan->janelaAtiva->Desenhar();
           foco = FOCO_JOGO;
           return(JANELAFECHADA);
       }
       else if((res) && (men->procSelecionado) &&(pronto)) 
       {
           men->procSelecionado(ljan->janelaAtiva,
                                chamador,men->Item(res),
                                NULL);
           foco = FOCO_JOGO;
       }
       else if(pronto)
       {
          ljan->janelaAtiva->Desenhar();
          foco = FOCO_JOGO;
       }
       return(MENUSELECIONADO);
    }

    /* FOCUS ON TEXT SELECT  */
    else
    if ((foco == FOCO_SELTEXTO) /*&& (x != mouseX || y != mouseY)*/ )
    {
        mouseX = x;
        mouseY = y;
        selTexto *st = (selTexto*)objAtivo;
        if(!st->Tratar(x,y,Mbotao,ljan->janelaAtiva->cara,
                       ljan->janelaAtiva->x1, ljan->janelaAtiva->y1))
            foco = FOCO_JOGO;

        ljan->janelaAtiva->AtualizaCara();

        return(SELTEXTOMODIFICADA);
    }

    /* If here, no actions were made on GUI */
    return(NADA);
}

/*********************************************************************
 *                   Draw Graphic User Interface                     *
 *********************************************************************/
void interface::Desenhar(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   int aux;
   double profundidade = 0.012;
   janela* jan = (janela*) ljan->primeiro->proximo;

   if(ljan->janelaAtiva == NULL)
     return;
   
   /* Draw Inative Windows */
   for(aux = 0;aux<ljan->total;aux++)
   {
      if(jan != ljan->janelaAtiva)
      {
          AtualizaTela2D(jan->caraTextura,proj,modl,viewPort,jan->x1,jan->y1, 
                         jan->x2,jan->y2,profundidade);
          profundidade += 0.001;
      }
      jan = (janela*) jan->proximo;
   }

   /* Draw Active Window */
   AtualizaTela2D(ljan->janelaAtiva->caraTextura,proj,modl,viewPort,
                     ljan->janelaAtiva->x1,ljan->janelaAtiva->y1,
                     ljan->janelaAtiva->x2,ljan->janelaAtiva->y2, 0.011);
}


