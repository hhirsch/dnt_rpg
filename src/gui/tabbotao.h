#ifndef _tabbotao_h
#define _tabbotao_h

#include "figura.h"
#include "cores.h"

#define MAX_TABBUTTONS 20 /**< Max Buttons per table of buttons */

/*! oneTabButton is a class to define what is a button on a table of buttons */
class oneTabButton :Tobjeto
{
   public:
      int x1,  /**< X Coordinate */
          y1,  /**< Y Coordinate */
          x2,  /**< X Coordinate */
          y2;  /**< Y Coordinate */
};

/*! The table of Buttons class. Its a figura divided in squares each one 
    representing (and acting as) a button. */
class tabButton: public figura
{
   public:
      tabButton(int x,int y,const char* arquivo);
      ~tabButton(){};
      oneTabButton* insertButton(int x1, int y1, int x2, int y2); 
      void draw(int mouseX, int mouseY, int Xjan, int Yjan,
                    SDL_Surface *screen);
      Tobjeto* verifyPosition(int mouseX, int mouseY, Uint8 Mbuttons, 
                          int Xjan, int Yjan);

   private:
      int numButtons;
      oneTabButton Buttons[MAX_TABBUTTONS];
};


#endif

