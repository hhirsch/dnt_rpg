/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "seltexto.h"

void selTexto::Desenhar(int Xjan,int Yjan,int selecionado,int salvar,
                        SDL_Surface *screen)
{
   int ya = y1+Yjan+3;
   int aux;
   selFonte(FHELVETICA,ESQUERDA,1); 
   for(aux = 0; aux<5;aux++)
   {
      if(!texto[aux].empty())
      {
         if(aux!=selecionado)
            cor_Definir(Cores.corTexto.R,Cores.corTexto.G,Cores.corTexto.B);
         else
            cor_Definir(Cores.corTextoSel.R,Cores.corTextoSel.G,
                        Cores.corTextoSel.B);
         ya = escxy_Area(screen,Xjan+4+x1,ya,texto[aux].c_str(),
                    Xjan+x1+1,Yjan+y1+1,Xjan+x2-1,Yjan+y2-1);
         y[aux] = ya-Yjan;
         ya += 11;
     }
   }
   cor_Definir(Cores.corCont[1].R,Cores.corCont[1].G,Cores.corCont[1].B);
   retangulo_2Cores(screen,x1+Xjan,y1+Yjan,x2+Xjan,y2+Yjan/*ya+2*/,
                    Cores.corCont[0].R,Cores.corCont[0].G,Cores.corCont[0].B,0);
   y2 = (ya-Yjan)+2;
   if(salvar)
   {
     //SDL_Flip(screen);
     //AtualizaTela2D(screen);
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
         ya = y1+Yjan+3;
      else
         ya = Yjan+y[selecionado-1]+11; 
   }
   else
   {
      if(selec == -1) return;
      aux = selec;
      if(selec == 0)
         ya = y1+Yjan+3;
      else
         ya = Yjan+y[selec-1]+11;
      cor_Definir(Cores.corTexto.R,Cores.corTexto.G,Cores.corTexto.B);
   }
   selFonte(FHELVETICA,ESQUERDA,1);
   escxy_Area(screen,Xjan+4+x1,ya,texto[aux].c_str(),
                 Xjan+x1+1,Yjan+y1+1,Xjan+x2-1,Yjan+y2-1);
   selec = selecionado;
}

int selTexto::Selecionada(int ya, int Yjan)
{
   int aux;
   int selaux = -1;
   for(aux=0;aux<5;aux++)
   {
        if( (aux>0) && (aux<4) && 
            (ya > (Yjan+y[aux-1]+11)) && 
            (ya < (Yjan+y[aux+1]) ) )
        {
              selaux = aux;
        }
        else if( (aux==0) && (ya < (Yjan + y[aux+1])))
           selaux = aux;
        else if( (aux==4) && (ya >= Yjan + y[aux-1]+11) )
           selaux = aux;
   } 
   return(selaux);
}

int selTexto::Tratar(int xa,int ya, Uint8 Mbotao, SDL_Surface *screen, 
                     int Xjan, int Yjan)
{
    int selaux = 1;

    /* Descolore o ultimo selecionado */
    EscreveSelecionado(0,0,-1, screen);

    if(!mouse_NaArea(Xjan+x1,Yjan+y1,Xjan+x2,Yjan+y2,xa,ya))
       return(0);

    /* Trata o caso do mouse encima do texto */
    selaux = Selecionada(ya,Yjan);
 
    /* Colore o selecionado atual */
    EscreveSelecionado(0,0, selaux, screen);

    /* Testa pressionamento do texto */
    if( Mbotao & SDL_BUTTON(1) && procPres != NULL )
    {
        procPres(screen, selaux);
    }
    return(1);
}


