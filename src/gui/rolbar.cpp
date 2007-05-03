/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "rolbar.h"
#include "fonte.h"
#include "lista.h"
#include "eventos.h"

#define ACTUALIZE_RATE 50

/*********************************************************************
 *                            Constructor                            *
 *********************************************************************/
rolBar::rolBar(int xa, int ya, int xb, int yb, string txt, void* list,
               SDL_Surface* surface)
{
   wSurface = surface;
   lastActualized = SDL_GetTicks();
   if(!list)
   {
      printf("Unknow Objects List when inserting rolBar!\n");
      return;
   }
   intList = list;

   selFonte(FMINI, ESQUERDA, 1);
   tipo = ROLBAR;
   x1 = xa;
   y1 = ya;
   x2 = xb;
   y2 = yb;
   maxLines = ((yb-ya) / 11)-1;
   charPerLine = (xb-xa) / fonte_incCP();
   
   Tlista* l = (Tlista*)list;

   /* Contorno */
   position = l->InserirQuadroTexto(xb-10, ya+2, xb-2, yb-26, 1, "");
   contorn = l->InserirQuadroTexto(xb-12, ya, xb, yb-24, 1, "");
   
   /* Botoes */
   up = l->InserirBotao(xb-12,yb-23,xb,yb-12, contorn->Cores.corBot.R, 
                        contorn->Cores.corBot.G, contorn->Cores.corBot.B,
                        "\36",0, NULL);
   down = l->InserirBotao(xb-12,yb-11,xb,yb, contorn->Cores.corBot.R, 
                          contorn->Cores.corBot.G, contorn->Cores.corBot.B,
                          "\37",0, NULL);

   /* Texto */
   text = l->InserirQuadroTexto(xa,ya,xb-13,yb,2,""); 
   text->Cores.corTexto.R = 246;
   text->Cores.corTexto.G = 190;
   text->Cores.corTexto.B = 190;
   text->fonte = FMINI;
   text->tamFonte = 1;

   actualPressed = NULL;

   setText(txt);

}

/*********************************************************************
 *                             Destructor                            *
 *********************************************************************/
rolBar::~rolBar()
{
   //Tlista* l = (Tlista*)intList;
   /* Remove from List all objects used by the rolBar */
   /*l->Retirar(contorn);
   l->Retirar(up);
   l->Retirar(down);
   l->Retirar(text);*/
}

/*********************************************************************
 *                              eventGot                             *
 *********************************************************************/
bool rolBar::eventGot(int type, Tobjeto* object)
{
   if((SDL_GetTicks() - lastActualized) >= ACTUALIZE_RATE)
   {
      lastActualized = SDL_GetTicks();
      if(type == BOTAOEMPRESSAO)
      {
         if(object == (Tobjeto*)up)
         {
            if(actualInit > 0)
            {
               actualInit -= 1;
               actualEnd = actualInit + maxLines -1;
            }
            
            text->texto = copyLines(fullText, actualInit,actualEnd);
            actualPressed = up;
            return(true);
         }
         else if(object == (Tobjeto*)down)
         {
            if(actualEnd <= totalLines)
            {
               actualEnd += 1;
               actualInit = (actualEnd - maxLines) +1;
               if(actualInit < 0)
               {
                  actualInit = 0;
               }
            }

            text->texto = copyLines(fullText, actualInit,actualEnd);
            actualPressed = down;
            return(true);
         }
         else
         {
            actualPressed = NULL;
         }
      }
   }
   return(false);
}

/*********************************************************************
 *                               redraw                              *
 *********************************************************************/
void rolBar::redraw()
{
   contorn->Desenhar(0,0,0,wSurface);
   if(maxLines <= totalLines)
   {
      position->y1 = (int) ((y1+2) + ((float)actualInit/(float)totalLines)*
                            (y2-28-y1));
      position->y2 = (int) ((y1+2) + ((float)(actualEnd-1)/(float)totalLines)*
                            (y2-28-y1));
   }
   position->Desenhar(0,0,0,wSurface);   
   text->Desenhar(0,0,0,wSurface);

   /* Mantain the draw of button pressed */
   if(actualPressed)
   {
      actualPressed->Desenhar(1, wSurface);
   }
}

/*********************************************************************
 *                              setText                              *
 *********************************************************************/
void rolBar::setText(string txt)
{
   fullText = txt;
   totalLines = getTotalLines(fullText);
   actualInit = 0;
   actualEnd = maxLines - 1;
   if(maxLines <= totalLines)
   {
      position->y1 = (int) ((y1+2) + ((float)actualInit/(float)totalLines)*
                            (y2-28-y1));
      position->y2 = (int) ((y1+2) + ((float)(actualEnd-1)/(float)totalLines)*
                            (y2-28-y1));
   }
   else
   {
      position->y1 = y1+2;
      position->y2 = y2-26;
   }
   text->texto = copyLines(fullText, actualInit,actualEnd);
}

/*********************************************************************
 *                              addText                              *
 *********************************************************************/
void rolBar::addText(string txt)
{
   /* Add Text */
   fullText += txt;
   /* Set the new text */
   setText(fullText);
   /* Put the rolling bar to the end */
   actualEnd = totalLines+1;
   actualInit = (actualEnd - maxLines) +1;
   if(actualInit < 0)
   {
      actualInit = 0;
   }
   text->texto = copyLines(fullText, actualInit,actualEnd);
   redraw();
}

