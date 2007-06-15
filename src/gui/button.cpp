/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "button.h"
#include "desenho.h"
#include "janela.h"
#include "fonte.h"

/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
button::button(int xa,int ya,int xb,int yb, string txt, bool isOval )
{
   x1 = xa; 
   y1 = ya;
   x2 = xb;
   y2 = yb;
   text = txt;
   oval = isOval;
   men = NULL;
   type = GUI_BUTTON;
}

/***********************************************************
 *                        Destructor                       *
 ***********************************************************/
button::~button()
{
}

/***********************************************************
 *                           draw                          *
 ***********************************************************/
void button::draw(bool pres, SDL_Surface* screen )
{
   int R1,R2,G1,G2,B1,B2; 
   if(pres) 
   {
      R1 = Colors.colorCont[1].R;
      G1 = Colors.colorCont[1].G;
      B1 = Colors.colorCont[1].B;
      R2 = Colors.colorCont[0].R;
      G2 = Colors.colorCont[0].G;
      B2 = Colors.colorCont[0].B;
   }
   else
   {
      R2 = Colors.colorCont[1].R;
      G2 = Colors.colorCont[1].G;
      B2 = Colors.colorCont[1].B;
      R1 = Colors.colorCont[0].R;
      G1 = Colors.colorCont[0].G;
      B1 = Colors.colorCont[0].B; 
   }
   
   cor_Definir(Colors.colorButton.R, 
                 Colors.colorButton.G,
                 Colors.colorButton.B);
   retangulo_Colorir(screen,x1+1,y1+1,x2-1,y2-1,0);
   cor_Definir(R1,G1,B1);
   if(oval)
      retangulo_Oval(screen,x1,y1,x2,y2,R2,B2,G2,0);
   else
      retangulo_2Cores(screen,x1,y1,x2,y2,R2,B2,G2,0);

   /* Write the Text */
   cor_Definir(Colors.colorText.R,Colors.colorText.G,Colors.colorText.B);
   int ya=y1;
   int xa=x1;
   if(pres) 
   {
     ya++;
     xa+=2;
   }
   selFonte(FFARSO,CENTRALIZADO,1);
   xa = ((xa+x2) /2);
   
   /* Verify the arrows buttons */
   if(!getText().compare("\36") || !getText().compare("\37"))
   {
     ya -= 6;
   }
   escxy(screen,xa,ya,getText().c_str());
   selFonte(FFARSO,ESQUERDA,1);
   
}

/***********************************************************
 *                           press                         *
 ***********************************************************/
bool button::press(int Xjan, int Yjan, int x, int y, Uint8 Mbotao, int* pronto,
                  SDL_Surface* screen)
{
   *pronto = 0;
   int pres;

   pres = (isMouseIn(x-Xjan,y-Yjan));

   /* Verify if the mouse Button is left */
   *pronto = !(Mbotao & SDL_BUTTON(1));
      
   draw(pres,screen);
   if(*pronto)
   {
      draw(0, screen);
   }
   return(pres);
}


