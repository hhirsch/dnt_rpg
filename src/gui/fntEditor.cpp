#include <stdlib.h>
#include <SDL/SDL.h>
#include "farso.h"

char caractere[15][15];   /* Caractere Atualmente editado */
fnt fonte;                /* Fonte em edicao */
int corAtv;               /* Cor Ativa-> 0 = transparente, !=0 corativa */
cores Cores;
barraTexto *arquivo;      /* Arquivo a ser salvo ou aberto */
int tecAberta;           /* Tecla Atualmente Aberta */

void zera()
{
  int aux,aux2;
  for (aux=0;aux<=256;aux++)
     for (aux2=0;aux2<15;aux2++)
          fonte.letra[aux][aux2] = 0;
  for(aux = 0;aux<15;aux++)
    for(aux2=0;aux2<15;aux2++)
       caractere[aux][aux2] = 0;
}

void poeLetra()
{
   int aux,aux2;
   int sobra;
   if(tecAberta != -1)
   {
      for(aux=0;aux<16;aux++)
      {
         sobra = 0;
         for (aux2=15;aux2>=0;aux2--)
         {
            if (caractere[aux][aux2])
               sobra += (2<<(aux2-1)); //Eleva 2 a aux2
         }
         fonte.letra[tecAberta][aux] = sobra;
      }
   }
}

void pegaLetra(int n)
{
   poeLetra();
   if(n == tecAberta) return;
   int sobra;
   int aux,aux2;
   for (aux=0;aux<16;aux++)
   {
      sobra = fonte.letra[n][aux];
      for (aux2=0;aux2<16;aux2++)
      {
          caractere[aux][aux2] = sobra % 2;
          sobra = sobra / 2;
      }
   }
   tecAberta = n;
}

void DefCP(barraTexto* bart, SDL_Surface *screen)
{
   int aux;
   fonte.incCP = 0;
   for(aux=0;bart->texto[aux]!='\0';aux++)
   {
      fonte.incCP += bart->texto[aux] - '0';
   }
}

int botao_CorAtiva(void* jan,void *ljan,SDL_Surface *screen)
{
    corAtv = 1;
    return(1);
}

int botao_CorInativa(void* jan,void *ljan,SDL_Surface *screen)
{
    corAtv = 0;
    return(1);
}

 
int botao_Novo(void* jan,void *ljan,SDL_Surface *screen)
{
  janela* j = (janela*) jan;
  zera();
  tecAberta = -1; 
  j->Ativar((Ljanela*)ljan,screen);
  return(0);
}

int botao_Abrir(void* jan,void *ljan,SDL_Surface *screen)
{
  janela* j = (janela*) jan;
  FILE *arq;
  if ( !((arq) = fopen (arquivo->texto, "rb")))
  { 
     printf("Arquivo Nao Aberto: %s\n",arquivo->texto);
     return(0);
  }
  fread(&fonte,sizeof(fnt),1,arq);
  fclose(arq);
  j->Ativar((Ljanela*)ljan,screen);
  tecAberta = -1;
  return(1);
}

int botao_Salvar(void *jan,void *ljan,SDL_Surface *screen)
{
   poeLetra();
   FILE *arq;
   if( !((arq) = fopen (arquivo->texto, "wb")))
   {
       printf("Arquivo nao foi salvo: %s\n",arquivo->texto);
       return(0);
   }
   fwrite(&fonte,sizeof(fnt),1,arq);
   fclose(arq);
   printf("Arquivo Salvo.\n");
   return(1);
}


void colocaCor(janela *jan,int x, int y, SDL_Surface *screen)
{
   x -= jan->x1;
   y -= jan->y1;
   int nx = ((x - 8) / 15) + 1;
   int ny = ((y - 18) / 15) + 1;
   if (nx>0 && nx<=15 && ny>0 && ny<=15)
   {
      if(corAtv)
         cor_Definir(255,255,255);
      else
         cor_Definir(0,0,0);
      retangulo_Colorir( jan->cara,((8) + (nx-1)*15)+1,((18) + (ny-1)*15)+1,
                         ((8) + (nx)*15)-1,((18)+ (ny)*15)-1,0);
      caractere[nx-1][ny-1]=corAtv;
      jan->PoeCaraNaTela(1,screen);
   }
}

void grade(janela *jan, SDL_Surface *screen)
{
    cor_Definir(0,0,0);
    retangulo_Colorir(jan->cara,8,18,233,243,0);
    int y = 18;
    int x =  8;
    cor_Definir(Cores.corCont[1].R,Cores.corCont[1].G,Cores.corCont[1].B);
    int aux;
    for(aux=1;aux<17;aux++)
    {
       linha_Desenhar(jan->cara,8,y,233,y,0);
       linha_Desenhar(jan->cara,x,18,x,243,0);
       y+=15;
       x+=15;
    }
    int aux2;
    for(aux=1;aux<16;aux++)
    {
        for(aux2=1;aux2<16;aux2++)
        {
           if(caractere[aux-1][aux2-1])
           { 
               cor_Definir(255,255,255);
               retangulo_Colorir( jan->cara,((8) + (aux-1)*15)+1,
                                  ((18) + (aux2-1)*15)+1,
                                  ((8) + (aux)*15)-1,((18)+ (aux2)*15)-1,0);
           }
        }
    }
    //janela_PoeCaraNaTela(jan,1,screen);

}


int main(int argc, char *argv[])
{
    int aux;
    for(aux = 0;aux<16;aux++)
       caractere[aux][aux] = 0;
    corAtv = 1;

    Cores.Iniciar();
    

    SDL_Surface *screen;
    Farso_Iniciar(&screen,"Editor de Fnt");
    glViewport(0,0,800,600);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   glOrtho(0.0f, 800, 600, 0.0f, -1.0f, 1.0f);
	
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

//    cor_Definir(0,0,0);
//    retangulo_Colorir(screen,0,0,639,479,0);
    fonte.incCP = 1;
    corAtv = 1;

    //Ljanela j;
    interface* interf = new interface(NULL);
    janela *jan, *janelaEdicao;
    //Ljanela_Iniciar(&j);
    jan = interf->ljan->InserirJanela(0,0,200,200,"fntEditor",1,1,NULL,NULL);
    jan->fechavel = 0;
    arquivo = jan->objetos->InserirBarraTexto(24,20,176,34,
                                              "Arquivo a Abrir",0,NULL);
    jan->objetos->InserirBotao(24,42,74,60,Cores.corBot.R,Cores.corBot.G,
                               Cores.corBot.B,"Nova",1,&botao_Novo);
    jan->objetos->InserirBotao(75,42,125,60,Cores.corBot.R,Cores.corBot.G,
                               Cores.corBot.B,"Abrir",1,&botao_Abrir);
    jan->objetos->InserirBotao(126,42,176,60,Cores.corBot.R,Cores.corBot.G,
                               Cores.corBot.B,"Salvar",1,&botao_Salvar);
    jan->objetos->InserirQuadroTexto(24,67,61,80,0,"INCP:");
    jan->objetos->InserirBarraTexto(60,66,84,80,"1",0,&DefCP);
    jan->objetos->InserirBotao(13,90,93,108,Cores.corBot.R,Cores.corBot.G,
                            Cores.corBot.B,"Cor Ativa",1,&botao_CorAtiva);
    jan->objetos->InserirBotao(96,90,185,108,Cores.corBot.R,Cores.corBot.G,
                         Cores.corBot.B,"Cor Inativa",1,&botao_CorInativa);
    jan->Ativar(interf->ljan,interf->tela2D);

    janelaEdicao = interf->ljan->InserirJanela(10,210,250,465,
                                  "Caractere",1,1,&grade,&colocaCor);
    janelaEdicao->fechavel = 0;
    zera();
    janelaEdicao->Ativar(interf->ljan,interf->tela2D);

    
    int pronto = 0;
    int c;
    tecAberta = 0;
    SDL_Event evento;
    SDLMod Modificador;
    
    while (!pronto)
    {
       pronto = !interf->ManipulaEventos(screen);
       if(SDL_PollEvent(&evento))
       {
           if(evento.type == SDL_KEYDOWN)
           {
               switch(evento.key.keysym.sym)
               {
                  /*Caracteres Ignorados*/
                  case SDLK_CAPSLOCK:
                  case SDLK_NUMLOCK:
                  case SDLK_SCROLLOCK:
                  case SDLK_RSHIFT:
                  case SDLK_LSHIFT:
                  case SDLK_RCTRL:
                  case SDLK_LCTRL:
                  case SDLK_RALT:
                  case SDLK_LALT:
                  break;
                  /* As Demais Definem */
                  case SDLK_KP_DIVIDE:
                     c='/';
                     pegaLetra(c);
                     janelaEdicao->Ativar(interf->ljan,interf->tela2D);
                  break;
                  default:
                  {
                      c = evento.key.keysym.sym;
                      Modificador=SDL_GetModState();
                      if( ((c >='a') && (c<='z') ) && 
                         (Modificador & KMOD_SHIFT) | (Modificador & KMOD_CAPS))
                          c+= 'A' - 'a';
                      pegaLetra(c); 
                      janelaEdicao->Ativar(interf->ljan,interf->tela2D);
                  }
                  break;
               }
           }
       }
    }

    atexit(SDL_Quit);

    delete(interf);
    
    return(0);

}

