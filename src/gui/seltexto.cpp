/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "seltexto.h"

void selTexto::Desenhar(int Xjan,int Yjan,int selecionado,int salvar,
                        SDL_Surface *screen)
{
   int ya = y1+Yjan;
   int aux;
   selFonte(FHELVETICA,ESQUERDA,1); 
   for(aux = 0; aux<5;aux++)
   {
      if(aux!=selecionado)
         cor_Definir(Cores.corTexto.R,Cores.corTexto.G,Cores.corTexto.B);
      else
         cor_Definir(Cores.corTextoSel.R,Cores.corTextoSel.G,
                     Cores.corTextoSel.B);
      ya = escxy_Area(screen,Xjan+4+x1,ya,texto[aux],
                 Xjan+x1+1,Yjan+y1+1,Xjan+x2-1,Yjan+y2-1);
      y[aux] = ya-Yjan;
      ya += 11;
   }
   cor_Definir(Cores.corCont[1].R,Cores.corCont[1].G,Cores.corCont[1].B);
   retangulo_2Cores(screen,x1+Xjan,y1+Yjan,x2+Xjan,ya+2,
                    Cores.corCont[0].R,Cores.corCont[0].G,Cores.corCont[0].B,0);
   y2 = (ya-Yjan)+2;
   if(salvar)
   {
     //SDL_Flip(screen);
     AtualizaTela2D(screen);
   }
}

void selTexto::EscreveSelecionado(int Xjan,int Yjan,int selecionado,
                                   SDL_Surface *screen)
{
   int ya;
   int aux;
   if(selecionado != -1)
   {
      aux = selecionado;
      cor_Definir(Cores.corTextoSel.R,Cores.corTextoSel.G,Cores.corTextoSel.B);
      if(selecionado == 0)
         ya = y1+Yjan;
      else
         ya = Yjan+y[selecionado-1]+11; 
   }
   else
   {
      if(selec == -1) return;
      aux = selec;
      if(selec == 0)
         ya = y1+Yjan;
      else
         ya = Yjan+y[selec-1]+11;
      cor_Definir(Cores.corTexto.R,Cores.corTexto.G,Cores.corTexto.B);
   }
   selFonte(FHELVETICA,ESQUERDA,1);
   escxy_Area(screen,Xjan+4+x1,ya,texto[aux],
                 Xjan+x1+1,Yjan+y1+1,Xjan+x2-1,Yjan+y2-1);
   selec = selecionado;

   /*SDL_Flip(screen);
   SDL_GL_SwapBuffers();*/
   AtualizaTela2D(screen);
}

void selTexto::Tratar(int Xjan,int Yjan,SDL_Surface *screen)
{
    int pronto = 0;
    int selecionado = -1;
    int selaux;
    int aux;
    int xa,ya;
    Uint8 Mbotao;

    while(!pronto)
    {
        SDL_PumpEvents();
        Mbotao = SDL_GetMouseState(&xa,&ya);
        if(!mouse_NaArea(Xjan+x1,Yjan+y1,Xjan+x2,Yjan+y2,xa,ya))
           pronto = 1;
        else
        {
        /* Trata o caso do mouse encima do texto */
        for(aux=0;aux<5;aux++)
        {
            if((aux>0) && (aux<4))
            {
               if((ya > (Yjan+y[aux-1]+11)) && (ya < (Yjan+y[aux+1]) ) )
                  selaux = aux;
            }
            else if( (aux==0) && (ya < (Yjan + y[aux+1])))
               selaux = aux;
            else if( (aux==4) && (ya > Yjan + y[aux-1]+11) )
               selaux = aux;
        } 
        if(selaux != selecionado)
        {
           selecionado = selaux;
           /* Descolore o ultimo selecionado */
           EscreveSelecionado(Xjan,Yjan,-1, screen);
           /* Colore o selecionado atual */
           EscreveSelecionado(Xjan,Yjan, selecionado, screen);
        }
        /* Testa pressionamento do texto */
        if( Mbotao & SDL_BUTTON(1) && procPres != NULL )
        {
            procPres(screen, selecionado);
        }
        }
    }
    EscreveSelecionado(Xjan,Yjan, -1, screen);
}


