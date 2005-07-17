/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "cxsel.h"

void cxSel::Desenhar(SDL_Surface *screen)
{
   cor_Definir(Cores.corCont[0].R,Cores.corCont[0].G,Cores.corCont[0].B);
   retangulo_2Cores(screen,x,y,x+10,y+10,
                    Cores.corCont[1].R,Cores.corCont[1].G,Cores.corCont[1].B,0);
   cor_Definir(Cores.corCont[2].R,Cores.corCont[2].G,Cores.corCont[2].B);
   retangulo_Colorir(screen,x+1,y+1,
                            x+9,y+9,0);
   if(selecionada)
   {
      cor_Definir(Cores.corCont[0].R,Cores.corCont[0].G,Cores.corCont[0].B);
      linha_Desenhar(screen,x+2,y+2,x+8,y+8,0);
      linha_Desenhar(screen,x+1,y+2,x+7,y+8,0);
      linha_Desenhar(screen,x+3,y+2,x+9,y+8,0);
      linha_Desenhar(screen,x+8,y+2,x+2,y+8,0);
      linha_Desenhar(screen,x+7,y+2,x+1,y+8,0);
      linha_Desenhar(screen,x+9,y+2,x+3,y+8,0);
   }
}

