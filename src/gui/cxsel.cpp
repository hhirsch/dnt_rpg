/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "cxsel.h"

/**********************************************************
 *                       Constructor                      *
 **********************************************************/
cxSel::cxSel(int x, int y):guiObject()
{
   x1 = x;
   y1 = y;
   x2 = x+10;
   y2 = y+10;
   selected = false;
   lastChangeTime = SDL_GetTicks();
   type = GUI_SEL_BOX;
}

/**********************************************************
 *                           draw                         *
 **********************************************************/
void cxSel::draw(SDL_Surface *screen)
{
   color_Set(Colors.colorCont[0].R, Colors.colorCont[0].G, 
             Colors.colorCont[0].B, Colors.colorCont[0].A);
   rectangle_2Colors(screen, x1, y1, x1+10, y1+10, Colors.colorCont[1].R,
                     Colors.colorCont[1].G, Colors.colorCont[1].B,
                     Colors.colorCont[1].A);
   color_Set(Colors.colorCont[2].R,Colors.colorCont[2].G,Colors.colorCont[2].B,
             Colors.colorButton.A);
   rectangle_Fill(screen, x1+1, y1+1, x1+9, y1+9);
   if(selected)
   {
      color_Set(Colors.colorBar.R, Colors.colorBar.G, 
                Colors.colorBar.B, Colors.colorCont[0].A);
      line_Draw(screen, x1+2, y1+2, x1+8, y1+8);
      line_Draw(screen, x1+1, y1+2, x1+7, y1+8);
      line_Draw(screen, x1+3, y1+2, x1+9, y1+8);
      line_Draw(screen, x1+8, y1+2, x1+2, y1+8);
      line_Draw(screen, x1+7, y1+2, x1+1, y1+8);
      line_Draw(screen, x1+9, y1+2, x1+3, y1+8);
   }
}

/**********************************************************
 *                       isSelected                       *
 **********************************************************/
bool cxSel::isSelected()
{
   return(selected);
}

/**********************************************************
 *                     invertSelection                    *
 **********************************************************/
void cxSel::invertSelection()
{
   GLuint actualTime = SDL_GetTicks();
   if((actualTime - lastChangeTime) >= 100)
   {
      setChanged();
      selected = !selected;
      lastChangeTime = actualTime;
   }
}

/**********************************************************
 *                       setSelection                     *
 **********************************************************/
void cxSel::setSelection(bool value)
{
   setChanged();
   selected = value;
}

