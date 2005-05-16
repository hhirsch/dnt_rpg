/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "eventos.h"

int mouseX=0,mouseY=0;

interface::interface(char* arqfundo)
{
   ljan = new Ljanela;
   objetos = new Tlista;
   tela2D =  SDL_CreateRGBSurface(SDL_HWSURFACE,512, 512,32,0x000000FF, 
			0x0000FF00, 
			0x00FF0000, 
			0xFF000000);
   if(arqfundo != NULL)
   {
      fundo = SDL_LoadBMP(arqfundo);
      if(!fundo) printf("Nao abri arquivo de fundo\n");
   } 
   else
   {
       fundo =  SDL_CreateRGBSurface(SDL_HWSURFACE,512, 512,32,0x000000FF, 
			0x0000FF00, 
			0x00FF0000, 
			0xFF000000);
       cor_Definir(0,0,0);
       retangulo_Colorir(fundo,0,0,511,511,0);
   }
   SDL_BlitSurface(fundo,NULL,tela2D,NULL);
}

interface::~interface()
{
   delete(ljan);
   delete(objetos);
   SDL_FreeSurface(tela2D);
   SDL_FreeSurface(fundo);
}

int interface::ManipulaEventos(SDL_Surface *screen)
{
   Uint8 Mbotao;
   int x,y;
   int aux;
   Uint8 *tecla;


   SDL_PumpEvents();
   Mbotao = SDL_GetMouseState(&x,&y);
   tecla = SDL_GetKeyState(NULL);
   
    /* Verifica Eventos do Teclado */
    if (tecla[SDLK_ESCAPE])
    {
       return(SAIR);
    }

    /* Verifica Movimentacao do Mouse */
    if(x != mouseX || y != mouseY)
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
//            selTexto *st=ljan->janelaAtiva->selsTexto.primeiro->proximo;
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
                       st->Tratar(ljan->janelaAtiva->x1,
                                  ljan->janelaAtiva->y1,tela2D);    
                       return(SELTEXTOMODIFICADA);
                  }
               }
               obj = obj->proximo;
            }
        }
    }
    /* Verifica Pressao do mouse */
    if(Mbotao & SDL_BUTTON(1))
    {
        if( (ljan->janelaAtiva != NULL) &&
             mouse_NaArea(ljan->janelaAtiva->x1+36,
                          ljan->janelaAtiva->y1, 
                          ljan->janelaAtiva->x2-3,
                          ljan->janelaAtiva->y1+12,x,y))
        {
            /* Movimentacao da Janela Ativa */
            ljan->janelaAtiva->Mover(ljan,tela2D,fundo);
            return(JANELAMOVIMENTADA);
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
                      if (bot->Pressionar(ljan->janelaAtiva->x1,
                                          ljan->janelaAtiva->y1,tela2D))
                      {
                          if(bot->procPres != NULL)
                               bot->procPres(ljan->janelaAtiva,ljan,tela2D);
                          else if (bot->men != NULL)
                          {
                               menu* men = (menu*)bot->men;
                               bot->Desenhar(ljan->janelaAtiva->x1,
                                             ljan->janelaAtiva->y1,
                                             1,1,tela2D);
                               men->Coordenada(bot->x1,bot->y2+1);
                               int res = men->Rodar(ljan->janelaAtiva->x1,
                                                    ljan->janelaAtiva->y1,
                                                    tela2D);
                               //bot->Desenhar(ljan->janelaAtiva->x1,
                                 //            ljan->janelaAtiva->y1,
                                  //           0,1,tela2D);
                               ljan->janelaAtiva->PoeCaraNaTela(1,tela2D);
                               if(!strcmp(bot->texto,"_") && res==4)
                               {
                                  if(ljan->janelaAtiva->fechavel)
                                     ljan->janelaAtiva->Fechar(ljan,
                                                         tela2D,fundo);
                                  return(JANELAFECHADA);
                               }
                               if((res) && (men->procSelecionado)) 
                                  men->procSelecionado(ljan->janelaAtiva,
                                                       bot,men->Item(res),
                                                       tela2D);
                               return(MENUSELECIONADO);
                          }
                          else if (!strcmp(bot->texto,"*"))
                          {
                               /* Fecha a Janela */
                             if(ljan->janelaAtiva->fechavel)
                                 ljan->janelaAtiva->Fechar(ljan,
                                                    tela2D,fundo);
                               return(JANELAFECHADA);
                          }
                          return(BOTAOPRESSIONADO);
                     }
                     return(BOTAONAOPRESSIONADO);
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
                        bart->Escrever(ljan->janelaAtiva->x1,
                                       ljan->janelaAtiva->y1,
                                       x,y,tela2D);
                        if(bart->procEditada != NULL)
                        {
                            bart->procEditada(bart,tela2D);
                        }
                        return(BARRATEXTOESCRITA);
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
                       if(cx->selecionada)
                           cx->selecionada = 0;
                       else
                           cx->selecionada = 1;
                       cx->Desenhar(ljan->janelaAtiva->x1,
                                    ljan->janelaAtiva->y1,
                                    1,tela2D);
                       SDL_Delay(TEMPOREPETICAO);
                       return(CXSELMODIFICADA);
                   }
               }
               obj = obj->proximo;
            }
            /* Se saiu do for, sem objeto algum, chama o proc da janela */
            if(ljan->janelaAtiva->procPres != NULL)
            {
                ljan->janelaAtiva->procPres(ljan->janelaAtiva,x,y,tela2D);
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
                    jaux->Ativar(ljan,tela2D);
                    return(JANELAATIVADA);
               }
               jaux = (janela*) jaux->proximo;
           }
        } 
    }
    return(NADA);
}




