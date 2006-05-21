/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "figura.h"

figura::~figura()
{
   if (fig)
      SDL_FreeSurface(fig);
}

void figura::Desenhar(int Xjan,int Yjan,int salvar,SDL_Surface *screen)
{
   SDL_Rect Ret;
   Ret.x = x1+Xjan;
   Ret.y = y1+Yjan;
   Ret.w = fig->w;
   Ret.h = fig->h;
   SDL_BlitSurface(fig,NULL,screen,&Ret);
   if(salvar)
   {
      //SDL_Flip(screen);
      //SDL_GL_SwapBuffers();
      //AtualizaTela2D(screen);
   }
}
