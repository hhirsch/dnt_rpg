#include "tabbotao.h"
#include "desenho.h"
#include "mouse.h"

tabBotao::tabBotao(int x,int y,const char* arquivo):figura(x,y,0,0,arquivo)
{
   numBotoes = 0;
}

void tabBotao::inserirBotao(int x1, int y1, int x2, int y2,
                        int (*proc)(void *jan,void *ljan,SDL_Surface *screen))
{
   if(numBotoes < MAX_TABBOTOES)
   {
      Botoes[numBotoes].x1 = x1;
      Botoes[numBotoes].y1 = y1;
      Botoes[numBotoes].x2 = x2;
      Botoes[numBotoes].y2 = y2;
      Botoes[numBotoes].procPres = proc;
      numBotoes++;
   }
}

void tabBotao::Desenhar(int mouseX, int mouseY, 
                        int Xjan,int Yjan,SDL_Surface *screen)
{ 
   int i;
   figura* fig;
   fig = this;
   fig->Desenhar(0, 0, 0,  screen);
   for(i=0;i<numBotoes;i++)
   {
      if(mouse_NaArea(Xjan + x1 + Botoes[i].x1, Yjan + y1 + Botoes[i].y1,
                      Xjan + x1 + Botoes[i].x2, Yjan + y1 + Botoes[i].y2,
                      mouseX, mouseY))
      {
          cor_Definir(248,2,11);
          retangulo_2Cores(screen,x1+Botoes[i].x1,y1+Botoes[i].y1,
                           x1+Botoes[i].x2,y1+Botoes[i].y2,
                           86,1,4,0);
      }
   }
}

bool tabBotao::VerificarSobrePosicao(int mouseX, int mouseY, int Xjan, int Yjan)
{
   int i;
   for(i=0;i<numBotoes;i++)
   {
      if(mouse_NaArea(Xjan + x1 + Botoes[i].x1, Yjan + y1 + Botoes[i].y1,
                      Xjan + x1 + Botoes[i].x2, Yjan + y1 + Botoes[i].y2,
                      mouseX, mouseY))
      {
         if(Botoes[i].procPres != NULL)
         {
            Botoes[i].procPres(NULL, NULL, NULL);
         }
         return(true);
      }
   }
   return(false);
}

