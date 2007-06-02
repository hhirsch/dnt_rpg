/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "botao.h"
#include "desenho.h"
#include "janela.h"
#include "cores.h"
#include "fonte.h"

/*Desenha o botao (bot) na tela*/
void botao::Desenhar(int pres, void* jan, 
                     int Salvar )
{
   janela* j = (janela*) jan;
   
   Desenhar(pres, j->cara);
   
}

void botao::Desenhar(int pres, SDL_Surface* screen )
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
   retangulo_Colorir(screen,x1+1,y1+1,x2-1,y2-1,0);
   cor_Definir(R1,G1,B1);
   if(oval)
      retangulo_Oval(screen,x1,y1,x2,y2,R2,B2,G2,0);
   else
      retangulo_2Cores(screen,x1,y1,x2,y2,R2,B2,G2,0);
   /*Aqui deve entrar a escrita do texto*/

   cor_Definir(Cores.corTexto.R,Cores.corTexto.G,Cores.corTexto.B);
   int ya=y1;
   int xa=x1;
   if(pres) 
   {
     ya++;
     xa+=2;
   }
   selFonte(FFARSO,CENTRALIZADO,1);
   xa = ((xa+x2) /2);
   //y1-=6;
   if(!texto.compare("\36") || !texto.compare("\37"))
     ya -= 6;
   escxy(screen,xa,ya,texto.c_str());
   selFonte(FFARSO,ESQUERDA,1);
   
}


int botao::Pressionar(int Xjan, int Yjan, void* jan, 
                      int x, int y, Uint8 Mbotao, int* pronto)
{
 
   //janela* j = (janela*) jan;  

   *pronto = 0;
   int pres;

   pres = (mouse_NaArea(Xjan+x1,Yjan+y1,Xjan+x2,Yjan+y2,x,y));

   /* Testa se botao do mouse foi solto */
   *pronto = !(Mbotao & SDL_BUTTON(1));
      
   Desenhar(pres,jan,1);
   if(*pronto) Desenhar(0,jan,1);
   return(pres);
}


