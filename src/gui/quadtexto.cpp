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
         color_Set(Cores.colorButton.R,Cores.colorButton.G,Cores.colorButton.B);
         rectangle_Fill(screen,Xjan+x1+1,Yjan+y1+1,Xjan+x2-1,Yjan+y2-1);
      }
      color_Set(Cores.colorCont[0].R,Cores.colorCont[0].G,
                  Cores.colorCont[0].B);
      rectangle_2Colors(screen,Xjan+x1,Yjan+y1,Xjan+x2,Yjan+y2,
                       Cores.colorCont[1].R,Cores.colorCont[1].G,
                       Cores.colorCont[1].B);
   }
   color_Set(Cores.colorText.R,Cores.colorText.G,Cores.colorText.B);
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

