/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "seltext.h"

/***************************************************************************
 *                            Constructor                                  *
 ***************************************************************************/
selText::selText(int xa,int ya,int xb,int yb, string text0, string text1,
                 string text2, string text3, string text4)
{
   type = GUI_SEL_TEXT;
   x1 = xa;
   y1 = ya;
   x2 = xb;
   y2 = yb;
   
   optText[0] = text0;
   optInfo[0] = 0;
   optText[1] = text1;
   optInfo[1] = 0;
   optText[2] = text2;
   optInfo[2] = 0;
   optText[3] = text3;
   optInfo[3] = 0;
   optText[4] = text4;
   optInfo[4] = 0;

   selec = -1;
   pressed = false;
}

/***************************************************************************
 *                             Destructor                                  *
 ***************************************************************************/
selText::~selText()
{
}

/***************************************************************************
 *                           setCoordinate                                 *
 ***************************************************************************/
void selText::setCoordinate(int xa,int ya,int xb,int yb)
{
   x1 = xa;
   y1 = ya;
   x2 = xb;
   y2 = yb;
   selec = -1;
   pressed = false;
}

/***************************************************************************
 *                           getCoordinate                                 *
 ***************************************************************************/
void selText::getCoordinate(int& xa,int& ya,int& xb,int& yb)
{
   xa = x1;
   ya = y1;
   xb = x2;
   yb = y2;
}


/***************************************************************************
 *                                draw                                     *
 ***************************************************************************/
void selText::draw(SDL_Surface *screen)
{
   int ya = y1+3;
   int aux;
   dntFont fnt;
   fnt.defineFont(DNT_FONT_ARIAL,10); 
   int height = fnt.getHeight();

   for(aux = 0; aux<5;aux++)
   {
      if(!optText[aux].empty())
      {
         if(aux != selec)
         {
            color_Set(Cores.colorText.R, Cores.colorText.G,
                      Cores.colorText.B, Cores.colorText.A);
         }
         else
         {
            color_Set(Cores.colorSelText.R, Cores.colorSelText.G,
                      Cores.colorSelText.B, Cores.colorSelText.A);
         }
         ya=fnt.write(screen,4+x1,ya,optText[aux].c_str(),x1+1,y1+1,x2-1,y2-1);
     }
     y[aux] = ya;
     ya += height;
   }
   //y2 = ya+2;
   color_Set(Cores.colorCont[1].R, Cores.colorCont[1].G,
             Cores.colorCont[1].B, Cores.colorCont[1].A);
   rectangle_2Colors(screen,x1,y1,x2,y2, Cores.colorCont[0].R,
                     Cores.colorCont[0].G, Cores.colorCont[0].B,
                     Cores.colorCont[0].A);
}

/***************************************************************************
 *                           writeSelected                                 *
 ***************************************************************************/
void selText::writeSelected(int selectedItem, SDL_Surface *screen)
{
   selec = selectedItem;
   draw(screen);
}

/***************************************************************************
 *                          getSelectedItem                                *
 ***************************************************************************/
int selText::getSelectedItem(int ya )
{
   dntFont fnt;
   fnt.defineFont(DNT_FONT_ARIAL,10);
   int height = fnt.getHeight();
   int aux;
   int selaux = -1;
   for(aux=0;aux<5;aux++)
   {
        if( (aux>0) && (aux<4) && 
            (ya > (+y[aux-1]+height)) && 
            (ya < (+y[aux+1]) ) )
        {
              selaux = aux;
        }
        else if( (aux==0) && (ya < ( + y[aux+1])))
        {
           selaux = aux;
        }
        else if( (aux==4) && (ya >=  + y[aux-1]+height) )
        {
           selaux = aux;
        }
   } 
   return(selaux);
}

/***************************************************************************
 *                                 treat                                   *
 ***************************************************************************/
int selText::treat(int xa,int ya, Uint8 Mbotao, SDL_Surface *screen)
{
    selec = -1;

    if(!isMouseIn(xa,ya))
    {
       return(-1);
    }

    /* Get the current item */
    selec = getSelectedItem(ya);

    /* Verify if it exists or not */
    if( (selec >= MAX_OPTIONS) || (selec < 0) || 
        (optText[selec].empty()) )
    {
       /* Not valid selection */
       selec = -1;
    }

    //No more draw here, since will redraw the 
    //window after treat, to avoid problems at
    //the transparent textbox.
    //writeSelected(selaux, screen);

    /* Test optText pression */
    if( Mbotao & SDL_BUTTON(1) )
    {
       pressed = true;
    }
    else if(pressed)
    {
      pressed = false;
      return(selec);
    }
    return(-2);
}

/***************************************************************************
 *                          getLastSelectedItem                            *
 ***************************************************************************/
int selText::getLastSelectedItem(int* info)
{
   if( (info) && (selec >= 0) && (selec < MAX_OPTIONS))
   {
      *info = optInfo[selec];
   }

   return(selec);
}

/***************************************************************************
 *                                setText                                  *
 ***************************************************************************/
void selText::setText(int opt, string txt, int info)
{
   if(opt < MAX_OPTIONS)
   {
      optText[opt] = txt;
      optInfo[opt] = info;
   }
}

/***************************************************************************
 *                               clearText                                 *
 ***************************************************************************/
void selText::clearText()
{
   int i;
   for(i = 0; i < MAX_OPTIONS; i++)
   {
      optText[i] = "";
   }
}

