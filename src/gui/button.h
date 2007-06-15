/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _botao_h
#define _botao_h


#include <string>
using namespace std;

#include "guiobject.h"
#include "cores.h"
#include "desenho.h"
#include "mouse.h"

/*! The button class */
class button: public guiObject
{
   public:
      /*! Constructor
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate 
       * \param txt -> button text
       * \param isOval -> if the button is oval or rectangular */
      button(int xa,int ya,int xb,int yb, string txt, bool isOval );

      /*! Destructor */
      ~button();

      /*!Draws the Button on Surface
       * \param pres -> if button is pressed
       * \param screen -> surface where will draw the button */
      void draw(bool pres, SDL_Surface* screen );

      /*! Do the press proccess of the button.
       * \param Xjan -> window x1 coordinate 
       * \param Yjan -> window y1 coordinate
       * \param x -> mouseX coordinate
       * \param y -> mouseY coordinate
       * \param screen -> surface where the button is.
       * \param pronto -> will be != 0 when end the proccess
       * \param screen -> surface where draws the button
       * \return true when pressed, false if not pressed. */
       bool press(int Xjan, int Yjan, int x, int y, Uint8 Mbotao, int* pronto,
                  SDL_Surface* screen);

      void* men;        /**< Button Menu (buttons can have a menu to be 
                                          activated when button presses) */

   protected:
      bool oval;        /**< Oval Button?  */
      cores Colors;     /**< Colors */
};

#endif

