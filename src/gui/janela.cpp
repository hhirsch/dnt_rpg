#include "janela.h"
#include "menu.h"
#include "botao.h"
#include "bartexto.h"
#include "cxsel.h"
#include "quadtexto.h"
#include "seltexto.h"

Ljanela::~Ljanela()
{
   janela* jan;
   jan = (janela*)primeiro->proximo;
   while(jan->proximo != primeiro )
   {
      //RetirarJanela((janela*)(primeiro->proximo),NULL);
      SDL_FreeSurface(jan->cara);
      delete(jan->objetos);
      jan = (janela*)jan->proximo;
   }
   //delete(primeiro);
} 
 

janela* Ljanela::InserirJanela(int xa,int ya,int xb,int yb,char *text,
                         int maximiz,int redmens, 
                         void (*procAtiva)(janela* jan, SDL_Surface *screen),
                         void (*procPres)(janela* jan,int x, int y, 
                               SDL_Surface *screen))
{
   janela* novo;
   novo = new janela;
   novo->procAtiva = procAtiva;
   novo->procPres = procPres;
   novo->Cores.Iniciar();
   novo->x1 = xa;
   novo->x2 = xb;
   novo->y1 = ya;
   novo->y2 = yb;
   novo->minY = 15;
   novo->minX = 50; //a calcular ainda,assim q tiver redmensonamento(se tiver)
   novo->texto = text;
   novo->maximiz = maximiz;
   novo->redmens = redmens;
   novo->Rjan = novo->Cores.corJan.R;
   novo->Gjan = novo->Cores.corJan.G;
   novo->Bjan = novo->Cores.corJan.B;
   novo->Rbar = novo->Cores.corBarra.R;
   novo->Gbar = novo->Cores.corBarra.G;
   novo->Bbar = novo->Cores.corBarra.B;
   novo->Rtxt = novo->Cores.corTexto.R;
   novo->Gtxt = novo->Cores.corTexto.G;
   novo->Btxt = novo->Cores.corTexto.B;
   novo->fechavel = 1;
   novo->cara = SDL_CreateRGBSurface(SDL_HWSURFACE,xb-xa+1,yb-ya+1,32,
                         0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   novo->objetos = new Tlista;
   botao* tmp;
   tmp = novo->objetos->InserirBotao(3,3,13,12,novo->Cores.corBot.R,
                  novo->Cores.corBot.G,novo->Cores.corBot.B,"_",0,
                  NULL);
   tmp->men = new menu();
   menu* men = (menu*) tmp->men;
   men->InserirMenuItem("Fechar",1);
   men->InserirMenuItem("-",0);
   men->InserirMenuItem("Maximizar",0);
   men->InserirMenuItem("Mover",0); 
   novo->objetos->InserirBotao(14,3,24,12,novo->Cores.corBot.R,
                  novo->Cores.corBot.G,novo->Cores.corBot.B,"*",0,
                  NULL);
   novo->objetos->InserirBotao(25,3,35,12,novo->Cores.corBot.R,
                  novo->Cores.corBot.G,novo->Cores.corBot.B,"\36",0,
                  NULL);
   novo->tipo = JANELA;
   InserirObj(novo);
   return(novo);
} 

void Ljanela::RetirarJanela(janela *jan,SDL_Surface *screen)
{
   jan->anterior->proximo = jan->proximo;
   jan->proximo->anterior = jan->anterior;
   if (jan->cara == NULL)
      printf("Sujou: jan->cara == NULL!!!\n");
   SDL_FreeSurface(jan->cara);
   if(jan == janelaAtiva)
   {
      janelaAtiva = NULL;
      if((screen != NULL) && (total > 1))
      {
         janela* j = (janela*)primeiro->proximo;
         j->Ativar(this,screen);
      }
   } 
   delete(jan->objetos);
   delete(jan);
   total--;
}

void janela::PoeCaraNaTela(int salvar,SDL_Surface *screen)
{
   SDL_Rect retangulo;
   int dx = x2 - x1;  /* Tamanho no eixo X da janela */
   int dy = y2 - y1;  /* Tamanho no eixo Y da janela */
 
   retangulo.x = x1;
   retangulo.y = y1;
   retangulo.h = dy+1;
   retangulo.w = dx+1;
   SDL_BlitSurface(cara,NULL,screen,&retangulo);
   if(salvar)
   {
      //SDL_Flip(screen);
      //SDL_GL_SwapBuffers();
      AtualizaTela2D(screen);
   }
}

void janela::Desenhar(int salvar, SDL_Surface *screen)
{
   int dx = x2 - x1 + 1;
   int dy = y2 - y1 + 1;


   cor_Definir(Rjan,Gjan,Bjan);
   retangulo_Colorir(cara, 3,3,dx-3,dy-3,0);
   cor_Definir(Cores.corCont[0].R,Cores.corCont[0].G,Cores.corCont[0].B);
   retangulo_Desenhar(cara,0,0,dx-1,dy-1,0);
   cor_Definir(Cores.corBot.R,Cores.corBot.G,Cores.corBot.B);
   retangulo_Desenhar(cara,1,1,dx-2,dy-2,0);
   cor_Definir(Cores.corCont[2].R,Cores.corCont[2].G,Cores.corCont[2].B);
   linha_Desenhar(cara,2,13,dx-4,13,0);
   cor_Definir(Cores.corCont[0].R,Cores.corCont[0].G,Cores.corCont[0].B);
   retangulo_2Cores(cara,2,2,dx-3,dy-3,
                    Cores.corCont[2].R,Cores.corCont[2].G,Cores.corCont[2].B,0);
   cor_Definir(Cores.corBarra.R,Cores.corBarra.G,Cores.corBarra.B);
   retangulo_Colorir(cara,25,3,dx-4,12,0);
   /* Escrita do Titulo */
   cor_Definir(Cores.corTexto.R,Cores.corTexto.G,Cores.corTexto.B);
   if (!selFonte(FFARSO,ESQUERDA,1))
   {
      erro_Mensagem("Fonte farso.fnt nao encontrada!\n",3);
   }
   escxy(cara,39,-2,texto);
   /* Desenho dos Botoes */
   Tobjeto *obj=objetos->primeiro->proximo;
   int aux;
   for(aux=0;aux<objetos->total;aux++)
   {
      switch(obj->tipo)
      {
         case BOTAO:{
              botao *b = (botao*) obj;   
              b->Desenhar(0,0,0,0,cara);
              break;
         }
         case BARRATEXTO:{
              barraTexto *bart = (barraTexto*) obj; 
              bart->Desenhar(0,0,0,cara);
              break;
         }
         case CXSEL:{
              cxSel *cx = (cxSel*) obj;
              cx->Desenhar(0,0,0,cara);
              break;
         }
         case SELTEXTO:{
              selTexto *st = (selTexto*) obj;
              st->Desenhar(0,0,-1,0,cara);
              break;
         }
         case FIGURA:{
              figura* fig = (figura*) obj;
              fig->Desenhar(0,0,0,cara);
              break;
         }
         case QUADROTEXTO:{
              quadroTexto *quad = (quadroTexto*) obj;
              quad->Desenhar(0,0,0,cara);
              break;
         }
         default:break;
       
      } //case
      obj = obj->proximo;
   }
   PoeCaraNaTela(salvar,screen);

}


int janela::Sobreposta(janela h2)
{
   return( ( ( (x1<=h2.x1) && (x2>=h2.x1) ) || 
           ( (x1<=h2.x2) && (x2>=h2.x2) ) ||
           ( (x1>=h2.x1) && (x2<=h2.x2) ) )  &&
           (( (y1<=h2.y1) && (y2>=h2.y1) ) || 
           ( (y1<=h2.y2) && (y2>=h2.y2) ) ||
           ( (y1>=h2.y1) && (y2<=h2.y2) )));
}


void janela::BarraInativa( SDL_Surface *screen)
{
   cor_Definir(Cores.corJan.R,Cores.corJan.G,Cores.corJan.B);
   retangulo_Colorir(screen,x1+36,y1+3,x2-3,y1+12,0);
   cor_Definir(0,0,0);
   selFonte(FFARSO,ESQUERDA,1);
   escxy(screen,x1+39,y1-2,texto);
}


void janela::Ativar(Tlista *lista,SDL_Surface* screen)
{
  Ljanela *ljan = (Ljanela*) lista;
  if (ljan->janelaAtiva != NULL)
  {
     ljan->janelaAtiva->BarraInativa(screen);
  }
  ljan->janelaAtiva = this;
  Desenhar(0,screen);
  if(procAtiva != NULL)
  {
     procAtiva(this,screen);
     PoeCaraNaTela(0,screen);
  }
//  SDL_Flip(screen);
//  SDL_GL_SwapBuffers();
  AtualizaTela2D(screen);
}


void janela::RefazerFundo(Tlista *lista, SDL_Surface *screen,SDL_Surface* fundo)
{
   SDL_Rect ret;
//   SDL_Rect dest;

   ret.x = x1;
   ret.y = y1;
   ret.w = (x2-x1)+1;
   ret.h = (y2-y1)+1;

   janela *jaux=(janela*)lista->primeiro->proximo;  /* janela auxiliar */
   int aux;                                /* auxiliar */
   //cor_Definir(0,0,0); /* Trocar isso pelo fundo!!!! */
   //retangulo_Colorir(screen,x1,y1,x2,y2,0);
   SDL_BlitSurface(fundo,&ret,screen,&ret);
   

   for(aux=1;aux<=lista->total;aux++)
   {
      if( (this!=jaux) && (Sobreposta(*jaux)))
      {
         //janela_Desenhar(*jaux,0,screen);
         jaux->PoeCaraNaTela(0,screen);
         jaux->BarraInativa(screen);
      }
      jaux=(janela*)jaux->proximo;
   }
}


void janela::Fechar(Tlista *ljan, SDL_Surface *screen,SDL_Surface* fundo)
{
   Ljanela* lista = (Ljanela*) ljan;
   RefazerFundo(lista,screen,fundo);
   lista->RetirarJanela(this,screen);
   //SDL_Flip(screen);
   //SDL_GL_SwapBuffers();
   AtualizaTela2D(screen);
}


/* Redesenha a janela no ponto desejado */
void janela::FazMudanca(int mouseX, int mouseY, Tlista *lista, 
                        SDL_Surface *screen, SDL_Surface* fundo)
{
   SDL_Rect retangulo, ret2;
   int dx = x2 - x1;  /* Tamanho no eixo X da janela */
   int dy = y2 - y1;  /* Tamanho no eixo Y da janela */
 
   retangulo.x = x1;
   retangulo.y = y1;
   retangulo.h = dy+1;
   retangulo.w = dx+1;
   ret2.x = 0;
   ret2.y = 0;
   ret2.w = dx+1;
   ret2.h = dy+1;
   SDL_BlitSurface(screen,&retangulo,cara,&ret2);
   RefazerFundo(lista,screen,fundo);
   x1 = mouseX;
   x2 = mouseX + dx;
   y1 = mouseY;
   y2 = mouseY + dy;
   retangulo.x = mouseX;
   retangulo.y = mouseY;
   SDL_BlitSurface(cara,NULL,screen,&retangulo);
   //SDL_Flip(screen);
   //SDL_GL_SwapBuffers();
   AtualizaTela2D(screen);
   //janela_Desenhar(*jan,1,screen);
}

void janela::Mover(Tlista *lista, SDL_Surface *screen, SDL_Surface* fundo)
{
   int dx = x2 - x1;        /* Tamanho no eixo X da janela */
   int dy = y2 - y1;        /* Tamanho no eixo Y da janela */
   int pronto=0;                      /* Controle de Fluxo */
   int mouseX=x1,mouseY=y1; /* Ultimas posicoes do Mouse */
   
#ifdef MOVIMENTO_OPACO	
   int x,y;         /* Coordenadas do Mouse */
   int xinic,yinic; /* Utilizados para manter posicao relativa inicial */
   /* Come o primeiro para nao ter gaio com  movimentacao anterior
    * a de mover a janela.*/
   SDL_PumpEvents();
   SDL_GetMouseState(&xinic,&yinic);
   xinic -= x1;
   yinic -= y1;
   while (!pronto)
   {
      SDL_PumpEvents();
      if (SDL_GetMouseState(&x,&y) & SDL_BUTTON(1))
      {
         //printf("%d %d\n",x,y);
         x -= xinic; 
         y -= yinic;
         if(x + dx > screen->w-1)
         { 
             x -= (x + dx) - (screen->w-1);
         }
         if(y + dy > screen->h-1) 
         {
             y -= (y+dy) - (screen->h-1);
         }
         if(x < 0)
            x = 0;
         if(y <0)
            y = 0;
         if( (x != mouseX) || (y != mouseY))
         {
            mouseX = x;
            mouseY = y;
             
            FazMudanca(mouseX,mouseY,lista,screen, fundo); 
         }
      }
      else 
      {
         pronto = 1;
      }
   }
#else

#if 0
	 SDL_Event evento;                  /* Manipulador de Eventos */
         
	 SDL_Rect resquerdo, rdireito, rcima, rbaixo;
	 SDL_Surface * scima, * sbaixo, * sesquerdo, * sdireito;
	 int posX, posY;

	 resquerdo.x = jan->x1;
	 resquerdo.y = jan->y1;
	 resquerdo.w = 1;
	 resquerdo.h = dy;
	 rdireito.x = jan->x2 - 1;
	 rdireito.y = jan->y1;
	 rdireito.w = 1;
	 rdireito.h = dy;
	 rcima.x = jan->x1;
	 rcima.y = jan->y1;
	 rcima.w = dx;
	 rcima.h = 1;
	 rbaixo.x = jan->x1;
	 rbaixo.y = jan->y2 - 1;
	 rbaixo.w = dx;
	 rbaixo.h = 1;
	 scima = SDL_CreateRGBSurface(SDL_HWSURFACE, dx, 1, 16, 0, 0, 0, 0);
	 sbaixo = SDL_CreateRGBSurface(SDL_HWSURFACE, dx, 1, 16, 0, 0, 0, 0);
	 sesquerdo = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, dy, 16, 0, 0, 0, 0);
	 sdireito = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, dy, 16, 0, 0, 0, 0);
	 SDL_BlitSurface(screen, &resquerdo, sesquerdo, NULL);
	 SDL_BlitSurface(screen, &rdireito, sdireito, NULL);
	 SDL_BlitSurface(screen, &rcima, scima, NULL);
	 SDL_BlitSurface(screen, &rbaixo, sbaixo, NULL);
	 cor_Definir(255, 255, 255);
	 retangulo_Desenhar(screen, jan->x1, jan->y1, jan->x2 - 1, jan->y2 - 1, 1);
	 posX = jan->x1;
	 posY = jan->y1;
	 while (!pronto)
	 {
		  if(SDL_PollEvent(&evento))
		  {
			   switch(evento.type)
			   {
				    case SDL_MOUSEMOTION:
					     /* Faz a movimentacao da janela enquanto ainda move */
					     mouseX = posX + evento.motion.xrel;
					     mouseY = posY + evento.motion.yrel;
					     posX = mouseX;
					     posY = mouseY;
					     /* Nao deixa a janela passar do limite da superficie */
					     if(mouseX + dx > screen->w-1)
					     { 
						      mouseX -= (mouseX+ dx) - (screen->w-1);
					     }
					     if(mouseY + dy > screen->h-1) 
					     {
						      mouseY -= (mouseY+dy) - (screen->h-1);
					     }
					     if(mouseY < 0) mouseY = 0;
					     if(mouseX < 0) mouseX = 0;
					     SDL_BlitSurface(sesquerdo, NULL, screen, &resquerdo);
					     SDL_BlitSurface(sdireito, NULL, screen, &rdireito);
					     SDL_BlitSurface(scima, NULL, screen, &rcima);
					     SDL_BlitSurface(sbaixo, NULL, screen, &rbaixo);
					     resquerdo.x = mouseX;
					     resquerdo.y = mouseY;
					     resquerdo.w = 1;
					     resquerdo.h = dy;
					     rdireito.x = mouseX + dx - 1;
					     rdireito.y = mouseY;
					     rdireito.w = 1;
					     rdireito.h = dy;
					     rcima.x = mouseX;
					     rcima.y = mouseY;
					     rcima.w = dx;
					     rcima.h = 1;
					     rbaixo.x = mouseX;
					     rbaixo.y = mouseY + dy - 1;
					     rbaixo.w = dx;
					     rbaixo.h = 1;
					     SDL_BlitSurface(screen, &resquerdo, sesquerdo, NULL);
					     SDL_BlitSurface(screen, &rdireito, sdireito, NULL);
					     SDL_BlitSurface(screen, &rcima, scima, NULL);
					     SDL_BlitSurface(screen, &rbaixo, sbaixo, NULL);
					     cor_Definir(255, 255, 255);
					     retangulo_Desenhar(screen, mouseX, mouseY, mouseX + dx - 1, mouseY + dy - 1, 1);
					     break; 
				    case SDL_MOUSEBUTTONUP:
					     /* Se soltou o botao esquerdo, acabou o movimento */
					     SDL_BlitSurface(sesquerdo, NULL, screen, &resquerdo);
					     SDL_BlitSurface(sdireito, NULL, screen, &rdireito);
					     SDL_BlitSurface(scima, NULL, screen, &rcima);
					     SDL_BlitSurface(sbaixo, NULL, screen, &rbaixo);
					     pronto = (evento.button.button == SDL_BUTTON_LEFT);
					     janela_FazMudanca(jan, mouseX,mouseY,lista,screen);
					     break;    
			   }
		  }
		  /*      else if( (mouseX != jan->x1) || (mouseY != jan->y1))
			   {
			   janela_FazMudanca(jan, mouseX, mouseY, lista,screen);
			   }*/
	 }


#endif

#endif

}

