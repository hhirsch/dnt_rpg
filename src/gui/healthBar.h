#ifndef _farso_healthBar_h
#define _farso_healthBar_h

#include "guiobject.h"
#include <SDL/SDL.h>

/*! The Health Bar Class */
class healthBar: public guiObject
{
   public:
  
      /*! Contructor
       * \param x1 -> coordinate on surface 
       * \param y1 -> coordinate on surface
       * \param x2 -> coordinate on surface
       * \param y2 -> coordinate on surface*/
      healthBar(int x1, int y1, int x2, int y2);
      /*! Destructor */
      ~healthBar();

      /*! Defines the Max Health of the Bar
       * \param health -> health value to be the max */
      void defineMaxHealth(int health);

      /*! Defines the Actual Health of the Bar
       * \param health -> health value to be the actual */
      void defineActualHealth(int health);

      /*! Draws the healthBar to Surface.
       * \param surface -> surface to draw to */
      void draw(SDL_Surface* surface);

   private:

      int actualHealth; /**< The Actual life on bar */
      int maxHealth;    /**< The Maximun life on bar */

};

#endif

