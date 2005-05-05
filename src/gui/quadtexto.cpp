#include "quadtexto.h"

void quadroTexto::Desenhar(int Xjan,int Yjan, int Salvar, SDL_Surface *screen)
{
   cor_Definir(Cores.corCont[1].R,Cores.corCont[1].G,Cores.corCont[1].B);
   selFonte(FHELVETICA,ESQUERDA,1);
   escxy_Area(screen, Xjan+x1+4, Yjan+y1, texto, Xjan+x1+2,
                      Yjan+y1, Xjan+x2-2, Yjan+y2);
   if(moldura)
   {
      cor_Definir(Cores.corCont[0].R,Cores.corCont[0].G,Cores.corCont[0].B);
      retangulo_2Cores(screen,Xjan+x1,Yjan+y1,Xjan+x2,Yjan+y2,
                       Cores.corCont[1].R,Cores.corCont[1].G,
                       Cores.corCont[1].B,0);
   }
   if(Salvar)
   {
      //SDL_Flip(screen);
      //SDL_GL_SwapBuffers();
      AtualizaTela2D(screen);
   }
}

