/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _botao_h
#define _botao_h


#include <string>
using namespace std;

#include "guiobject.h"
#include "color.h"
#include "draw.h"
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
       * \param screen -> surface where will draw the button */
      void draw(SDL_Surface* screen );

      /*! Do the press proccess of the button.
       * \param Xjan -> window x1 coordinate 
       * \param Yjan -> window y1 coordinate
       * \param x -> mouseX coordinate
       * \param y -> mouseY coordinate
       * \param screen -> surface where the button is.
       * \param pronto -> will be != 0 when end the proccess
       * \param screen -> surface where draws the button
       * \param Mbotao -> mouse button state
       * \return true when pressed, false if not pressed. */
       bool press(int Xjan, int Yjan, int x, int y, Uint8 Mbotao, int* pronto,
                  SDL_Surface* screen);


      /*! Define the font to use for the button
       * \param fileName -> fileName of the font to use
       * \param size -> size of the font to use */
      void defineFont(string fileName, int size);

      void* men;        /**< Button Menu (buttons can have a menu to be 
                                          activated when button presses) */

   protected:
      bool pressed;         /**< If the button is pressed or not */
      bool oval;            /**< Oval Button?  */
      farso_colors Colors;  /**< Colors */
      string fontName;      /**< Font Name */
      int fontSize;         /**< Font Size */
};

#endif

