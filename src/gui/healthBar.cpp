#include "healthBar.h"
#include "draw.h"

/****************************************************************
 *                        Constructor                           *
 ****************************************************************/
healthBar::healthBar(int x1, int y1, int x2, int y2)
{
   this->x1 = x1;
   this->y1 = y1;
   this->x2 = x2;
   this->y2 = y2;
   this->maxHealth = 1;
   this->actualHealth = 1;
}

/****************************************************************
 *                         Destructor                           *
 ****************************************************************/
healthBar::~healthBar()
{
}

/****************************************************************
 *                      defineMaxHealth                         *
 ****************************************************************/
void healthBar::defineMaxHealth(int health)
{
   maxHealth = health;
}

/****************************************************************
 *                     defineActualHealth                       *
 ****************************************************************/
void healthBar::defineActualHealth(int health)
{
   actualHealth = health;
}

/****************************************************************
 *                            draw                              *
 ****************************************************************/
void healthBar::draw(SDL_Surface* surface)
{
   color_Set(223,223,223);
   rectangle_Fill(surface, x1,y1,x2,y2);
   color_Set(153,51,51);

   /* Defines the percentual colored of the bar */
   double perc = actualHealth / (double) maxHealth;
   if(perc <= 0)
   {
      /* Don't draw the red bar when no health! */
      return;
   }
   int mx2 = (int) (perc * (x2-x1-4)) + x1+2;
   rectangle_Fill(surface, x1+2, y1+2, mx2, y2-2);
}

