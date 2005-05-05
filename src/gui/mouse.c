#include "mouse.h"

int mouse_NaArea(int x1,int y1,int x2,int y2, int mouseX, int mouseY)
{
   return( ( (mouseX>=x1) && (mouseX<=x2) ) && 
           ( (mouseY>=y1) && (mouseY<=y2) ) );
}


