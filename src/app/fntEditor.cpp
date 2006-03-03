/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include <stdlib.h>
#include <SDL/SDL.h>
#include "../gui/farso.h"
#include "../engine/culling.h"

char caractere[16][16];   /* Caractere Atualmente editado */
fnt fonte;                /* Fonte em edicao */
int corAtv;               /* Cor Ativa-> 0 = transparente, !=0 corativa */
cores Cores;
barraTexto *arquivo;      /* Arquivo a ser salvo ou aberto */
int tecAberta;           /* Tecla Atualmente Aberta */
int sair = 0;
figura* grid;
janela *jan, *janelaEdicao;
barraTexto* btCP;
barraTexto* btVaiPara;



void zera()
{
  int aux,aux2;
  for (aux=0;aux<=256;aux++)
     for (aux2=0;aux2<16;aux2++)
          fonte.letra[aux][aux2] = 0;
  for(aux = 0;aux<16;aux++)
    for(aux2=0;aux2<16;aux2++)
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
          if(caractere[aux][aux2])
             cor_Definir(255,255,255);
          else
             cor_Definir(0,0,0);
          retangulo_Colorir( grid->fig,((aux)*14)+1,((aux2)*14)+1,
                             ((aux+1)*14)-1,((aux2+1)*14)-1,0);
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

int botao_Sair(void* jan,void *ljan,SDL_Surface *screen)
{
    sair = 1;
    return(1);
}

int botao_VaiPara(void* jan,void *ljan,SDL_Surface *screen)
{
    int cDesejado =  atoi(btVaiPara->texto.c_str());
    printf("%c %d\n",cDesejado,cDesejado);
    pegaLetra(cDesejado);
    janelaEdicao->Desenhar();
    return(1);
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
  //janela* j = (janela*) jan;
  zera();
  tecAberta = -1; 
//  j->Ativar((Ljanela*)ljan);
  return(0);
}

int botao_Abrir(void* jan,void *ljan,SDL_Surface *screen)
{
  char tmp[4];
  janela* j = (janela*) jan;
  FILE *arq;
  if ( !((arq) = fopen (arquivo->texto.c_str(), "rb")))
  { 
     printf("Arquivo Nao Aberto: %s\n",arquivo->texto.c_str());
     return(0);
  }
  fread(&fonte,sizeof(fnt),1,arq);
  fclose(arq);
  sprintf(tmp,"%d",fonte.incCP);
  btCP->texto = tmp;
  j->Desenhar();
  tecAberta = -1;
  return(1);
}

int botao_Salvar(void *jan,void *ljan,SDL_Surface *screen)
{
   poeLetra();
   FILE *arq;
   if( !((arq) = fopen (arquivo->texto.c_str(), "wb")))
   {
       printf("Arquivo nao foi salvo: %s\n",arquivo->texto.c_str());
       return(0);
   }
   fwrite(&fonte,sizeof(fnt),1,arq);
   fclose(arq);
   printf("Arquivo Salvo.\n");
   return(1);
}


void colocaCor(janela* jan, int x, int y, SDL_Surface* suf)
{
   x -= jan->x1;
   y -= jan->y1;
   int nx = ((x) / 14) ;
   int ny = ((y) / 14) ;
   if (nx>0 && nx<=16 && ny>0 && ny<=16)
   {
      if(corAtv)
         cor_Definir(255,255,255);
      else
         cor_Definir(0,0,0);
      retangulo_Colorir( grid->fig,((nx-1)*14)+1,((ny-1)*14)+1,
                         ((nx)*14)-1,((ny)*14)-1,0);
      caractere[nx-1][ny-1]=corAtv;
      jan->Desenhar();
   }
}

void grade()
{
    cor_Definir(0,0,0);
    retangulo_Colorir(grid->fig,0,0,224,224,0);
    int y = 0;
    int x = 0;
    cor_Definir(Cores.corCont[1].R,Cores.corCont[1].G,Cores.corCont[1].B);
    int aux;
    for(aux=0;aux<17;aux++)
    {
       linha_Desenhar(grid->fig,0,y,224,y,0);
       linha_Desenhar(grid->fig,x,0,x,224,0);
       y+=14;
       x+=14;
    }
    int aux2;
    for(aux=0;aux<16;aux++)
    {
        for(aux2=0;aux2<16;aux2++)
        {
           if(caractere[aux][aux2])
           { 
               cor_Definir(255,255,255);
               retangulo_Colorir( grid->fig,((aux)*14)+1,
                                  ((aux2)*14)+1,
                                  ((aux+1)*14)-1,((aux2+1)*14)-1,0);
           }
        }
    }
    //janela_PoeCaraNaTela(jan,1,screen);
    janelaEdicao->Desenhar();

}

char defineCaractere(Uint8* teclas)
{
   char c[1];

   int maiuscula = (teclas[SDLK_CAPSLOCK] || teclas[SDLK_LSHIFT] || 
                    teclas[SDLK_RSHIFT]);

   if( teclas[SDLK_KP_DIVIDE])
      {
          if(maiuscula)
            c[0] = '?';
          else 
            c[0] = '/';
      }
      else 
      if( teclas[SDLK_a])
      {
          if (maiuscula)
            c[0] = 'A';
          else
            c[0] = 'a';
      } 
      else 
      if( teclas[SDLK_b])
      {
          if (maiuscula)
            c[0] = 'B';
          else
            c[0] = 'b';
      }
      else 
      if( teclas[SDLK_c])
      {
          if (maiuscula)
            c[0] = 'C';
          else
            c[0] = 'c';
      } 
      else 
      if( teclas[SDLK_d])
      {
          if (maiuscula)
            c[0] = 'D';
          else
            c[0] = 'd';
      }
      else 
      if( teclas[SDLK_e])
      {
          if (maiuscula)
            c[0] = 'E';
          else
            c[0] = 'e';
      }
      else 
      if( teclas[SDLK_f])
      {
          if (maiuscula)
            c[0] = 'F';
          else
            c[0] = 'f';
      }
      else 
      if( teclas[SDLK_g])
      {
          if (maiuscula)
            c[0] = 'G';
          else
            c[0] = 'g';
      }
      else 
      if( teclas[SDLK_h])
      {
          if (maiuscula)
            c[0] = 'H';
          else
            c[0] = 'h';
      }
      else 
      if( teclas[SDLK_i])
      {
          if (maiuscula)
            c[0] = 'I';
          else
            c[0] = 'i';
      }
      else 
      if( teclas[SDLK_j])
      {
          if (maiuscula)
            c[0] = 'J';
          else
            c[0] = 'j';
      }
      else 
      if( teclas[SDLK_k])
      {
          if (maiuscula)
            c[0] = 'K';
          else
            c[0] = 'k';
      }
      else 
      if( teclas[SDLK_l])
      {
          if (maiuscula)
            c[0] = 'L';
          else
            c[0] = 'l';
      }
      else 
      if( teclas[SDLK_m])
      {
          if (maiuscula)
            c[0] = 'M';
          else
            c[0] = 'm';
      }
      else 
      if( teclas[SDLK_n])
      {
          if (maiuscula)
            c[0] = 'N';
          else
            c[0] = 'n';
      }
      else 
      if( teclas[SDLK_o])
      {
          if (maiuscula)
            c[0] = 'O';
          else
            c[0] = 'o';
      }
      else 
      if( teclas[SDLK_p])
      {
          if (maiuscula)
            c[0] = 'P';
          else
            c[0] = 'p';
      }
      else 
      if( teclas[SDLK_q])
      {
          if (maiuscula)
            c[0] = 'Q';
          else
            c[0] = 'q';
      }
      else 
      if( teclas[SDLK_r])
      {
          if (maiuscula)
            c[0] = 'R';
          else
            c[0] = 'r';
      }
      else 
      if( teclas[SDLK_s])
      {
          if (maiuscula)
            c[0] = 'S';
          else
            c[0] = 's';
      }
      else 
      if( teclas[SDLK_t])
      {
          if (maiuscula)
            c[0] = 'T';
          else
            c[0] = 't';
      }
      else 
      if( teclas[SDLK_u])
      {
          if (maiuscula)
            c[0] = 'U';
          else
            c[0] = 'u';
      }
      else 
      if( teclas[SDLK_w])
      {
          if (maiuscula)
            c[0] = 'W';
          else
            c[0] = 'w';
      }
      else 
      if( teclas[SDLK_x])
      {
          if (maiuscula)
            c[0] = 'X';
          else
            c[0] = 'x';
      }
      else 
      if( teclas[SDLK_y])
      {
          if (maiuscula)
            c[0] = 'Y';
          else
            c[0] = 'y';
      }
      else 
      if( teclas[SDLK_z])
      {
          if (maiuscula)
            c[0] = 'Z';
          else
            c[0] = 'z';
      }
      else 
      if( teclas[SDLK_SLASH])
      {
          if (maiuscula)
            c[0] = '?';
          else
            c[0] = '/';
      }
      else 
      if( teclas[SDLK_BACKSLASH])
      {
          if (maiuscula)
            c[0] = '|';
          else
            c[0] = '\\';
      }
      else 
      if( teclas[SDLK_SEMICOLON])
      {
          if (maiuscula)
            c[0] = ':';
          else
            c[0] = ';';
      }
      else 
      if( teclas[SDLK_0])
      {
          if (maiuscula)
            c[0] = ')';
          else
            c[0] = '0';
      }
      else 
      if( teclas[SDLK_1])
      {
          if (maiuscula)
            c[0] = '!';
          else
            c[0] = '1';
      }
      else 
      if( teclas[SDLK_2])
      {
          if (maiuscula)
            c[0] = '@';
          else
            c[0] = '2';
      }
      else 
      if( teclas[SDLK_3])
      {
          if (maiuscula)
            c[0] = '#';
          else
            c[0] = '3';
      }
      else 
      if( teclas[SDLK_4])
      {
          if (maiuscula)
            c[0] = '$';
          else
            c[0] = '4';
      }
      else 
      if( teclas[SDLK_5])
      {
          if (maiuscula)
            c[0] = '%';
          else
            c[0] = '5';
      }
      else 
      if( teclas[SDLK_6])
      {
          if (maiuscula)
            c[0] = '"';
          else
            c[0] = '6';
      }
      else 
      if( teclas[SDLK_7])
      {
          if (maiuscula)
            c[0] = '&';
          else
            c[0] = '7';
      }
      else 
      if( teclas[SDLK_8])
      {
          if (maiuscula)
            c[0] = '*';
          else
            c[0] = '8';
      }
      else 
      if( teclas[SDLK_9])
      {
          if (maiuscula)
            c[0] = '(';
          else
            c[0] = '9';
      }
      else 
      if( teclas[SDLK_BACKQUOTE])
      {
          if (maiuscula)
            c[0] = '"';
          else
            c[0] = '\'';
      }
      else 
      if( teclas[SDLK_MINUS])
      {
          if (maiuscula)
            c[0] = '_';
          else
            c[0] = '-';
      }
      else 
      if( teclas[SDLK_PERIOD])
      {
          if (maiuscula)
            c[0] = '>';
          else
            c[0] = '.';
      }
      else 
      if( teclas[SDLK_COMMA])
      {
          if (maiuscula)
            c[0] = '<';
          else
            c[0] = ',';
      }
      else 
      if( teclas[SDLK_SPACE])
      {
          if (maiuscula)
            c[0] = ' ';
          else
            c[0] = ' ';
      }
    return(c[0]);
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
    
    jan = interf->ljan->InserirJanela(0,0,255,255,"fntEditor",1,1,NULL,NULL);
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
    btCP = jan->objetos->InserirBarraTexto(60,66,84,80,"1",0,&DefCP);
    jan->objetos->InserirBotao(13,90,93,108,Cores.corBot.R,Cores.corBot.G,
                            Cores.corBot.B,"Cor Ativa",1,&botao_CorAtiva);
    jan->objetos->InserirBotao(96,90,185,108,Cores.corBot.R,Cores.corBot.G,
                         Cores.corBot.B,"Cor Inativa",1,&botao_CorInativa);
    jan->objetos->InserirBotao(130,220,180,238,Cores.corBot.R,Cores.corBot.G,
                               Cores.corBot.B,"Sair",1,&botao_Sair);

    btVaiPara = jan->objetos->InserirBarraTexto(40,112,84,126,"233",0,NULL);
    jan->objetos->InserirBotao(90,110,155,128,Cores.corBot.R,Cores.corBot.G,
                               Cores.corBot.B,"VaiPara",1,&botao_VaiPara);

    jan->Abrir(interf->ljan);

    janelaEdicao = interf->ljan->InserirJanela(10,210,265,465,
                                  "Caractere",1,1,NULL,&colocaCor);
    janelaEdicao->fechavel = 0;
    grid = janelaEdicao->objetos->InserirFigura(8,18,256,256,NULL);
    grade();
    zera();
    janelaEdicao->Abrir(interf->ljan);

    
    int pronto = 0;
    char c[1];
    char ultimaLetra = 0;
    tecAberta = 0;

    Uint8 *keys;
    int x,y;

    GLdouble proj[16];
    GLdouble modl[16];
    GLint viewPort[4];
    GLfloat matrizVisivel[6][4];

    glGetIntegerv(GL_VIEWPORT, viewPort);

    AtualizaFrustum(matrizVisivel,proj,modl);
    
    while (!sair)
    {
       SDL_PumpEvents();
       keys = SDL_GetKeyState(NULL);
       glClearColor(0,0,0,1);
       glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
       Uint8 Mbotao = SDL_GetMouseState(&x,&y);
       pronto = !interf->ManipulaEventos(x,y,Mbotao, keys);
       if(pronto != BARRATEXTOESCRITA)
       {
          c[0] = defineCaractere(keys);
          if(c[0] != ultimaLetra)
          {
             pegaLetra(c[0]);
//             janelaEdicao->Ativar(interf->ljan);
             janelaEdicao->Desenhar();
          }
       }
       
       interf->Desenhar(proj,modl,viewPort);
       glFlush();
       SDL_GL_SwapBuffers();
    }

    atexit(SDL_Quit);

    delete(interf);

    int d = '|';
    printf("%c %d\n",d,d);
    d = 'É';
    printf("%c %d\n",d,d);    

    return(0);

}

