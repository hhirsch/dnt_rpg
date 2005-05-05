#include "botao.h"
#include "desenho.h"
#include "cores.h"
#include "fonte.h"

/*Desenha o botao (bot) na tela*/
void botao::Desenhar(int Xjan,int Yjan,int pres,int salvar,SDL_Surface *screen)
{
   int R1,R2,G1,G2,B1,B2; //cores do contorno
   if(pres) 
   {
      R1 = Cores.corCont[1].R;
      G1 = Cores.corCont[1].G;
      B1 = Cores.corCont[1].B;
      R2 = Cores.corCont[0].R;
      G2 = Cores.corCont[0].G;
      B2 = Cores.corCont[0].B;
   }
   else
   {
      R2 = Cores.corCont[1].R;
      G2 = Cores.corCont[1].G;
      B2 = Cores.corCont[1].B;
      R1 = Cores.corCont[0].R;
      G1 = Cores.corCont[0].G;
      B1 = Cores.corCont[0].B; 
   }
   
   cor_Definir(R,G,B);
   retangulo_Colorir(screen,x1+Xjan+1,y1+Yjan+1,x2+Xjan-1,y2+Yjan-1,0);
   cor_Definir(R1,G1,B1);
   if(oval)
      retangulo_Oval(screen,x1+Xjan,y1+Yjan,x2+Xjan,y2+Yjan,R2,B2,G2,0);
   else
      retangulo_2Cores(screen,x1+Xjan,y1+Yjan,x2+Xjan,y2+Yjan,R2,B2,G2,0);
   /*Aqui deve entrar a escrita do texto*/

   cor_Definir(Cores.corTexto.R,Cores.corTexto.G,Cores.corTexto.B);
   int ya=y1+Yjan;
   int xa=x1+Xjan;
   if(pres) 
   {
     ya++;
     xa+=2;
   }
   selFonte(FFARSO,CENTRALIZADO,1);
   xa = ((xa+x2+Xjan) /2);
   //y1-=6;
   if(!strcmp("\36",texto) || !strcmp("\37",texto))
     ya -= 6;
   escxy(screen,xa,ya,texto);
   selFonte(FFARSO,ESQUERDA,1);

   if (salvar)
   {
      // SDL_Flip(screen);
      // SDL_GL_SwapBuffers();
      AtualizaTela2D(screen);
   }
}

int botao::Pressionar(int Xjan,int Yjan, SDL_Surface *screen)
{
   int pronto = 0;
   int pres = 1;
   int Mbotao;
   int x,y, xAntes = 0,yAntes = 0;
   Desenhar(Xjan,Yjan,pres,1,screen);
   while(!pronto)
   {
      SDL_PumpEvents();
      Mbotao = SDL_GetMouseState(&x,&y);
      if (!pres && mouse_NaArea(Xjan+x1,Yjan+y1,Xjan+x2,Yjan+y2,
                                x,y) && ( (x!=xAntes) || (y!=yAntes)))
      {
          pres = 1;
          Desenhar(Xjan,Yjan,pres,1,screen);
      }
      else if(pres && !mouse_NaArea(Xjan+x1,Yjan+y1,Xjan+x2,Yjan+y2,
                                    x,y)&& ( (x!=xAntes) || (y!=yAntes)) )
      {
         pres = 0;
         Desenhar(Xjan,Yjan,pres,1,screen);
      }
      /* Testa se botao do mouse foi solto */
      pronto = !(Mbotao & SDL_BUTTON(1));
      
   }

   if(pres) Desenhar(Xjan,Yjan,0,1,screen);
   return(pres);

}


