/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "cxsel.h"

void cxSel::Desenhar(int Xjan,int Yjan,int salvar,SDL_Surface *screen)
{
   cor_Definir(Cores.corCont[0].R,Cores.corCont[0].G,Cores.corCont[0].B);
   retangulo_2Cores(screen,x+Xjan,y+Yjan,x+10+Xjan,y+10+Yjan,
                    Cores.corCont[1].R,Cores.corCont[1].G,Cores.corCont[1].B,0);
   cor_Definir(Cores.corCont[2].R,Cores.corCont[2].G,Cores.corCont[2].B);
   retangulo_Colorir(screen,x+1+Xjan,y+1+Yjan,
                            x+9+Xjan,y+9+Yjan,0);
   if(selecionada)
   {
      cor_Definir(Cores.corCont[0].R,Cores.corCont[0].G,Cores.corCont[0].B);
      linha_Desenhar(screen,Xjan+x+2,Yjan+y+2,Xjan+x+8,Yjan+y+8,0);
      linha_Desenhar(screen,Xjan+x+1,Yjan+y+2,Xjan+x+7,Yjan+y+8,0);
      linha_Desenhar(screen,Xjan+x+3,Yjan+y+2,Xjan+x+9,Yjan+y+8,0);
      linha_Desenhar(screen,Xjan+x+8,Yjan+y+2,Xjan+x+2,Yjan+y+8,0);
      linha_Desenhar(screen,Xjan+x+7,Yjan+y+2,Xjan+x+1,Yjan+y+8,0);
      linha_Desenhar(screen,Xjan+x+9,Yjan+y+2,Xjan+x+3,Yjan+y+8,0);
   }
   if (salvar)
   {
      //SDL_Flip(screen);
      //SDL_GL_SwapBuffers();
      //AtualizaTela2D(screen);
   }
}

