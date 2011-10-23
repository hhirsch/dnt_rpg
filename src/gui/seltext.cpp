/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "seltext.h"
#include <iostream>
using namespace std;


/***************************************************************************
 *                            Constructor                                  *
 ***************************************************************************/
selText::selText(int xa,int ya,int xb,int yb, string text0, string text1,
                 string text2, string text3, string text4,
                 SDL_Surface* screen):guiObject(screen)
{
   type = FARSO_OBJECT_SEL_TEXT;
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
void selText::draw()
{
   int ya = y1+3;
   int aux;
   dntFont fnt;
   fnt.defineFont(DNT_FONT_ARIAL,10); 
   fnt.defineFontAlign(DNT_FONT_ALIGN_LEFT);
   fnt.defineFontStyle(DNT_FONT_STYLE_NORMAL);
   
   int height = fnt.getHeight();
   
   /* No draw when hidden */
   if(!isVisible())
   {
      return;
   }

   /* Clear the current */
   color_Set(Cores.colorWindow.R, Cores.colorWindow.G, 
             Cores.colorWindow.B, Cores.colorWindow.A);
   rectangle_Fill(wSurface,x1,y1,x2,y2);
   color_Set(Cores.colorCont[1].R, Cores.colorCont[1].G,
             Cores.colorCont[1].B, Cores.colorCont[1].A);
   rectangle_2Colors(wSurface,x1,y1,x2,y2, Cores.colorCont[0].R,
                     Cores.colorCont[0].G, Cores.colorCont[0].B,
                     Cores.colorCont[0].A);

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
         ya=fnt.write(wSurface,4+x1,ya,
               optText[aux].c_str(),x1+1,y1+1,x2-1,y2-1);
     }
     y[aux] = ya;
     ya += height;
   }
}

/***************************************************************************
 *                           writeSelected                                 *
 ***************************************************************************/
void selText::writeSelected(int selectedItem)
{
   selec = selectedItem;
   draw();
}

/***************************************************************************
 *                          getSelectedItem                                *
 ***************************************************************************/
int selText::getSelectedItem(int ya )
{
   dntFont fnt;
   fnt.defineFont(DNT_FONT_ARIAL,10);
   fnt.defineFontAlign(DNT_FONT_ALIGN_LEFT);
   fnt.defineFontStyle(DNT_FONT_STYLE_NORMAL);
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
int selText::treat(int xa,int ya, Uint8 Mbotao)
{
   int lastSelec = selec;
   selec = -1;

   if(!isMouseIn(xa,ya))
   {
      /* Redraw, if needed */
      if(lastSelec != -1)
      {
         draw();
         setChanged();
      }
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

   /* Redraw, if needed */
   if(lastSelec != selec)
   {
      draw();
      setChanged();
   }

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

      draw();
      setChanged();
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
   draw();
   setChanged();
}

