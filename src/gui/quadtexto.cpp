/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "quadtexto.h"

quadroTexto::~quadroTexto()
{
}

void quadroTexto::Desenhar(int Xjan,int Yjan, int Salvar, SDL_Surface *screen)
{
   if(moldura)
   {
      if(moldura == 1)
      {
         cor_Definir(Cores.corBot.R,Cores.corBot.G,Cores.corBot.B);
         retangulo_Colorir(screen,Xjan+x1+1,Yjan+y1+1,Xjan+x2-1,Yjan+y2-1,0);
      }
      cor_Definir(Cores.corCont[0].R,Cores.corCont[0].G,Cores.corCont[0].B);
      retangulo_2Cores(screen,Xjan+x1,Yjan+y1,Xjan+x2,Yjan+y2,
                       Cores.corCont[1].R,Cores.corCont[1].G,
                       Cores.corCont[1].B,0);
   }
   cor_Definir(Cores.corTexto.R,Cores.corTexto.G,Cores.corTexto.B);
   selFonte(fonte.c_str(),aliFonte,tamFonte);
   escxy_Area(screen, Xjan+x1+5, Yjan+y1, texto.c_str(), Xjan+x1+5,
                      Yjan+y1, Xjan+x2, Yjan+y2);
   if(Salvar)
   {
      //SDL_Flip(screen);
      //SDL_GL_SwapBuffers();
      //AtualizaTela2D(screen);
   }
}

