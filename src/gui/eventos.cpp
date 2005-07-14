/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "eventos.h"

int mouseX=0,mouseY=0;

/*********************************************************************
 *                El Construtor de la Interface                      *
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
 *                El Destruidor de la Interface                      *
 *********************************************************************/
interface::~interface()
{
   delete(ljan);
   delete(objetos);
   SDL_FreeSurface(fundo);
}

/*********************************************************************
 *            Manipula a los Eventos de E/S de la GUI                *
 *********************************************************************/
int interface::ManipulaEventos(int x, int y, Uint8 Mbotao, Uint8* tecla)
{
    int aux;
   
    if(ljan->janelaAtiva == NULL)
       return(NADA);

    /* Verifica Eventos do Teclado */
    if ( (tecla[SDLK_ESCAPE]) && (foco != FOCO_JOGO))
    {
       foco = FOCO_JOGO;
       return(SAIR);
    }

    /* Verifica Movimentacao do Mouse para mudanca de foco */
    if( (x != mouseX || y != mouseY) && 
        (foco != FOCO_MENU && foco != FOCO_MENUJANELA) )
    {
        mouseX = x;
        mouseY = y;
        /* Testa selTexto */
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

    /* Verifica Pressao do mouse para mudanca de foco*/
    if((Mbotao & SDL_BUTTON(1)) &&  (foco == FOCO_JOGO))
    {
        if( (ljan->janelaAtiva != NULL) &&
             mouse_NaArea(ljan->janelaAtiva->x1+36,
                          ljan->janelaAtiva->y1, 
                          ljan->janelaAtiva->x2-3,
                          ljan->janelaAtiva->y1+12,x,y))
        {
            /* Movimentacao da Janela Ativa */
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
            /* Aqui entra o tratamento de cliques dentro da janela*/
            Tobjeto *obj = ljan->janelaAtiva->objetos->primeiro->proximo;
            int aux;
            for(aux=0; aux<ljan->janelaAtiva->objetos->total; aux++)
            {
               if(obj->tipo == BOTAO)
               {
                  /* Tratamento de Botoes */
                  botao *bot = (botao*) obj;
                  if(mouse_NaArea(bot->x1+ljan->janelaAtiva->x1,
                                  bot->y1+ljan->janelaAtiva->y1,
                                  bot->x2+ljan->janelaAtiva->x1,
                                  bot->y2+ljan->janelaAtiva->y1,x,y))
                  {
                      
                     objAtivo = bot;
                     foco = FOCO_BOTAO;
                     //return(BOTAONAOPRESSIONADO);
                  }
               }
               /* Tratamento das Barras de Texto */ 
               else if(obj->tipo == BARRATEXTO)
               {
                   barraTexto *bart = (barraTexto*) obj;
                   if(mouse_NaArea(bart->x1+ljan->janelaAtiva->x1,
                                   bart->y1+ljan->janelaAtiva->y1,
                                   bart->x2+ljan->janelaAtiva->x1,
                                   bart->y2+ljan->janelaAtiva->y1,x,y))
                   {
                       objAtivo = bart;
                       foco = FOCO_BARRATEXTO;
                   }
               }
               /* Tratamento de Caixas de Selecao */
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
               obj = obj->proximo;
            }
            /* Se saiu do for, sem objeto algum, chama o proc da janela */
            if(ljan->janelaAtiva->procPres != NULL)
            {
                ljan->janelaAtiva->procPres(ljan->janelaAtiva,x,y,NULL);
                return(JANELACLICADA);
            }
        }
        else if ( (ljan->janelaAtiva != NULL))
        {
           /* Testa a Ativacao de Outras Janelas */
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

    /*   FOCO NA MOVIMENTACAO DA JANELA  */
    if (foco == FOCO_JANELAMOVER)
    {
        if(!(ljan->janelaAtiva->Mover(ljan,NULL,fundo,x,y,Mbotao)))
           foco = FOCO_JOGO;
        return(JANELAMOVIMENTADA);
    }

    /* FOCO NO PRESSIONAMENTO DE UM BOTAO */
    else
    if(foco == FOCO_BOTAO)
    {
        int pronto;
        botao* bot = (botao*)objAtivo;
        if (bot->Pressionar(ljan->janelaAtiva->x1,
                            ljan->janelaAtiva->y1,
                            ljan->janelaAtiva->cara, x, y, Mbotao, 
                            &pronto))
        {
           if(pronto)
           {
              if(bot->procPres != NULL)
                  bot->procPres(ljan->janelaAtiva,ljan,NULL);
              else if (bot->men != NULL)
              {
                 objAtivo = (Tobjeto*) bot->men;
                 menu* men = (menu*)objAtivo;
                 men->Coordenada(bot->x1,bot->y2+1);
                 men->itemAtual = 1;
                 if (!strcmp(bot->texto,"_"))
                    foco = FOCO_MENUJANELA;
                 else
                    foco = FOCO_MENU;
                 
              }
              else if (!strcmp(bot->texto,"*"))
              {
                   /* Fecha a Janela */
                  if(ljan->janelaAtiva->fechavel)
                       ljan->janelaAtiva->Fechar(ljan);
                       return(JANELAFECHADA);
                  foco = FOCO_JOGO;
              }
              else
                  foco = FOCO_JOGO;
           }
        }
        else if(pronto)
           foco = FOCO_JOGO;
        return(BOTAOPRESSIONADO);
    }
 
    /* FOCO NA ESCRITA DE UMA BARRATEXTO */
    else 
    if (foco == FOCO_BARRATEXTO)
    {
        barraTexto* bart = (barraTexto*)objAtivo;
        //TODO
        bart->Escrever(ljan->janelaAtiva->x1,
                       ljan->janelaAtiva->y1,
                       x,y,NULL);
        if(bart->procEditada != NULL)
        {
              bart->procEditada(bart,NULL);
        }
        return(BARRATEXTOESCRITA);
    }
    
    /* FOCO NO TRATAMENTO DE CAIXAS DE TEXTO */
    else 
    if(foco == FOCO_CXSEL)
    {
       cxSel* cx = (cxSel*)objAtivo;
       if(cx->selecionada)
           cx->selecionada = 0;
       else
           cx->selecionada = 1;
       cx->Desenhar(ljan->janelaAtiva->x1,
                    ljan->janelaAtiva->y1,
                    1,NULL);
       foco = FOCO_JOGO;
       return(CXSELMODIFICADA);
    }

    /* FOCO NO TRATAMENTO DOS MENUS */
    else
    if ((foco == FOCO_MENU) || (foco == FOCO_MENUJANELA))
    {
       int pronto;
       menu* men = (menu*)objAtivo;
      
       int res = men->Rodar(x,y,Mbotao,tecla,
                            ljan->janelaAtiva->cara,&pronto,
                            ljan->janelaAtiva->x1,
                            ljan->janelaAtiva->y1);
        
       if((foco == FOCO_MENUJANELA) && (res==4) && (pronto))
       {
           //TODO BUG!
           if(ljan->janelaAtiva->fechavel)
              ljan->janelaAtiva->Fechar(ljan);
           printf("saiu!\n");
           foco = FOCO_JOGO;
           return(JANELAFECHADA);
       }
       else if((res) && (men->procSelecionado) &&(pronto)) 
       {
           men->procSelecionado(ljan->janelaAtiva,
                                NULL,men->Item(res),
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

    /* FOCO NA MOVIMENTACAO DO MOUSE NUMA SELTEXTO  */
    else
    if ((foco == FOCO_SELTEXTO) /*&& (x != mouseX || y != mouseY)*/ )
    {
        mouseX = x;
        mouseY = y;
        selTexto *st = (selTexto*)objAtivo;
        if(!st->Tratar(x,y,Mbotao,ljan->janelaAtiva->cara,
                       ljan->janelaAtiva->x1, ljan->janelaAtiva->y1))
            foco = FOCO_JOGO;
        return(SELTEXTOMODIFICADA);
    }

    /* Nao fez nada de util para a GUI */
    return(NADA);
}

void interface::Desenhar(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   int aux;
   double profundidade = 0.012;
   janela* jan = (janela*) ljan->primeiro->proximo;

   if(ljan->janelaAtiva == NULL)
     return;
   
   /* Desenha as Janelas Inativas */
   for(aux = 0;aux<ljan->total;aux++)
   {
      if(jan != ljan->janelaAtiva)
      {
          AtualizaTela2D(jan->cara,proj,modl,viewPort,jan->x1,jan->y1, 
                         profundidade);
          profundidade += 0.001;
      }
      jan = (janela*) jan->proximo;
   }

   /* Desenha a janelaAtiva */
   AtualizaTela2D(ljan->janelaAtiva->cara,proj,modl,viewPort,
                     ljan->janelaAtiva->x1,ljan->janelaAtiva->y1,0.011);
}


