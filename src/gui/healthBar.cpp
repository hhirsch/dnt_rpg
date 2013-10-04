/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "healthBar.h"
#include "draw.h"
using namespace Farso;

/****************************************************************
 *                        Constructor                           *
 ****************************************************************/
HealthBar::HealthBar(int x1, int y1, int x2, int y2, SDL_Surface* surface)
          :GuiObject(surface)
{
   this->x1 = x1;
   this->y1 = y1;
   this->x2 = x2;
   this->y2 = y2;
   this->maxHealth = 1;
   this->actualHealth = 1;
   this->type = Farso::OBJECT_HEALTH_BAR;
}

/****************************************************************
 *                         Destructor                           *
 ****************************************************************/
HealthBar::~HealthBar()
{
}

/****************************************************************
 *                      defineMaxHealth                         *
 ****************************************************************/
void HealthBar::defineMaxHealth(int health)
{
   maxHealth = health;
}

/****************************************************************
 *                     defineActualHealth                       *
 ****************************************************************/
void HealthBar::defineActualHealth(int health)
{
   actualHealth = health;
   draw();
}

/****************************************************************
 *                            draw                              *
 ****************************************************************/
void HealthBar::draw()
{
   if( (wSurface != NULL) && (isVisible()) )
   {
      color_Set(223,223,223,255);
      rectangle_Fill(wSurface, x1,y1,x2,y2);
      color_Set(153,51,51,255);

      /* Defines the percentual colored of the bar */
      double perc = actualHealth / (double) maxHealth;
      if(perc <= 0)
      {
         /* Don't draw the red bar when no health! */
         return;
      }
      int mx2 = (int) (perc * (x2-x1-4)) + x1+2;
      rectangle_Fill(wSurface, x1+2, y1+2, mx2, y2-2);
      setChanged();
   }
}

