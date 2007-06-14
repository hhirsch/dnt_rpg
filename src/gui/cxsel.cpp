/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "cxsel.h"


cxSel::cxSel():guiObject()
{
   selected = false;
   lastChangeTime = SDL_GetTicks();
}

void cxSel::draw(SDL_Surface *screen)
{
   cor_Definir(Colors.corCont[0].R, Colors.corCont[0].G, Colors.corCont[0].B);
   retangulo_2Cores(screen, x, y, x+10, y+10, Colors.corCont[1].R,
                     Colors.corCont[1].G, Colors.corCont[1].B, 0);
   cor_Definir(Colors.corCont[2].R, Colors.corCont[2].G, Colors.corCont[2].B);
   retangulo_Colorir(screen, x+1, y+1, x+9, y+9, 0);
   if(selected)
   {
      cor_Definir(Colors.corCont[0].R, Colors.corCont[0].G, Colors.corCont[0].B);
      linha_Desenhar(screen, x+2, y+2, x+8, y+8, 0);
      linha_Desenhar(screen, x+1, y+2, x+7, y+8, 0);
      linha_Desenhar(screen, x+3, y+2, x+9, y+8, 0);
      linha_Desenhar(screen, x+8, y+2, x+2, y+8, 0);
      linha_Desenhar(screen, x+7, y+2, x+1, y+8, 0);
      linha_Desenhar(screen, x+9, y+2, x+3, y+8, 0);
   }
}

bool cxSel::isSelected()
{
   return(selected);
}

void cxSel::invertSelection()
{
   GLuint actualTime = SDL_GetTicks();
   if((actualTime - lastChangeTime) >= 100)
   {
      selected = !selected;
      lastChangeTime = actualTime;
   }
}

void cxSel::setSelection(bool value)
{
   selected = value;
}

