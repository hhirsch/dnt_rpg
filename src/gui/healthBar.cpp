#include "healthBar.h"
#include "desenho.h"

healthBar::healthBar(int x1, int y1, int x2, int y2)
{
   this->x1 = x1;
   this->y1 = y1;
   this->x2 = x2;
   this->y2 = y2;
   this->maxHealth = 1;
   this->actualHealth = 1;
}

healthBar::~healthBar()
{
}

void healthBar::defineMaxHealth(int health)
{
   maxHealth = health;
}

void healthBar::defineActualHealth(int health)
{
   actualHealth = health;
}


void healthBar::draw(SDL_Surface* surface)
{
   cor_Definir(223,223,223);
   retangulo_Desenhar(surface, x1,y1,x2,y2, 0);
   cor_Definir(255,45,45);

   /* Defines the percentual colored of the bar */
   double perc = actualHealth / (double) maxHealth;
   int mx2 = (int) (perc * (x2-x1-2)) + x1+1;
   retangulo_Colorir(surface, x1+1, y1+1, mx2, y2-1, 0);
}

