/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "seltexto.h"

/***************************************************************************
 *                           setCoordinate                                 *
 ***************************************************************************/
void selTexto::setCoordinate(int xa,int ya,int xb,int yb)
{
   x1 = xa;
   y1 = ya;
   x2 = xb;
   y2 = yb;
   selec = -1;
   Cores.Iniciar();
   pressed = false;
}

/***************************************************************************
 *                           getCoordinate                                 *
 ***************************************************************************/
void selTexto::getCoordinate(int& xa,int& ya,int& xb,int& yb)
{
   xa = x1;
   ya = y1;
   xb = x2;
   yb = y2;
}


/***************************************************************************
 *                                draw                                     *
 ***************************************************************************/
void selTexto::draw(int selectedItem, SDL_Surface *screen)
{
   int ya = y1+3;
   int aux;
   selFonte(FHELVETICA,ESQUERDA,1); 
   for(aux = 0; aux<5;aux++)
   {
      if(!text[aux].empty())
      {
         if(aux!=selectedItem)
         {
            cor_Definir(Cores.corTexto.R,Cores.corTexto.G,Cores.corTexto.B);
         }
         else
         {
            cor_Definir(Cores.corTextoSel.R,Cores.corTextoSel.G,
                        Cores.corTextoSel.B);
         }
         ya = escxy_Area(screen,4+x1,ya,text[aux].c_str(), x1+1,y1+1,x2-1,y2-1);
     }
     y[aux] = ya;
     ya += 11;
   }
   cor_Definir(Cores.corCont[1].R,Cores.corCont[1].G,Cores.corCont[1].B);
   retangulo_2Cores(screen,x1,y1,x2,y2,
                    Cores.corCont[0].R,Cores.corCont[0].G,Cores.corCont[0].B,0);
   y2 = ya+2;
}

/***************************************************************************
 *                           writeSelected                                 *
 ***************************************************************************/
void selTexto::writeSelected(int selectedItem, SDL_Surface *screen)
{
   int ya;
   int aux;
   if(selectedItem != -1)
   {
      aux = selectedItem;
      cor_Definir(Cores.corTextoSel.R,Cores.corTextoSel.G,Cores.corTextoSel.B);
      if(selectedItem == 0)
         ya = y1+3;
      else
         ya = y[selectedItem-1]+11; 
   }
   else
   {
      if(selec == -1) return;
      aux = selec;
      if(selec == 0)
         ya = y1+3;
      else
         ya = y[selec-1]+11;
      cor_Definir(Cores.corTexto.R,Cores.corTexto.G,Cores.corTexto.B);
   }
   selFonte(FHELVETICA,ESQUERDA,1);
   escxy_Area(screen,4+x1,ya,text[aux].c_str(),
              x1+1,y1+1,x2-1,y2-1);
   selec = selectedItem;
}

/***************************************************************************
 *                          getSelectedItem                                *
 ***************************************************************************/
int selTexto::getSelectedItem(int ya )
{
   int aux;
   int selaux = -1;
   for(aux=0;aux<5;aux++)
   {
        if( (aux>0) && (aux<4) && 
            (ya > (+y[aux-1]+11)) && 
            (ya < (+y[aux+1]) ) )
        {
              selaux = aux;
        }
        else if( (aux==0) && (ya < ( + y[aux+1])))
        {
           selaux = aux;
        }
        else if( (aux==4) && (ya >=  + y[aux-1]+11) )
        {
           selaux = aux;
        }
   } 
   return(selaux);
}

/***************************************************************************
 *                                threat                                   *
 ***************************************************************************/
int selTexto::threat(int xa,int ya, Uint8 Mbotao, SDL_Surface *screen)
{
    int selaux = 1;

    /* Descolore o ultimo selectedItem */
    writeSelected(-1, screen);

    if(!mouse_NaArea(x1,y1,x2,y2,xa,ya))
    {
       return(-1);
    }

    /* Trata o caso do mouse encima do text */
    selaux = getSelectedItem(ya);
 
    /* Colore o selectedItem atual */
    writeSelected(selaux, screen);

    /* Testa pressionamento do text */
    if( Mbotao & SDL_BUTTON(1) )
    {
       pressed = true;
    }
    else if(pressed)
    {
      pressed = false;
      return(selaux);
    }
    return(-2);
}

/***************************************************************************
 *                          getLastSelectedItem                            *
 ***************************************************************************/
int selTexto::getLastSelectedItem()
{
   return(selec);
}


