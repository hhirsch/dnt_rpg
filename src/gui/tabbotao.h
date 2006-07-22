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

/*! The table of Buttons class. Its a figure divided in squares each one 
    representing (and acting as) a button. */
class tabButton: public figura
{
   public:
      /*! Constructor 
       * \param x -> x position of the tab button
       * \param y -> y position of the tab button
       * \param arquivo -> fileName of the image to Load */
      tabButton(int x,int y,const char* arquivo);
      /*! Destructor */
      ~tabButton(){};
      /*! insert Button in the table of buttons
       * \param x1 -> X Coordinate
       * \param y1 -> Y Coordinate
       * \param x2 -> X Coordinate 
       * \param y2 -> Y Coordinate 
       * \return Pointer to the oneTableButton Created */
      oneTabButton* insertButton(int x1, int y1, int x2, int y2); 
      /*! Draws the table button to the surface
       * \param mouseX -> mouse X position
       * \param mouseY -> mouse Y position
       * \param Xjan -> window X position
       * \param Yjan -> window Y position
       * \param screen -> pointer to the surface where the table will be 
       *                  draw (usually an window).  */
      void draw(int mouseX, int mouseY, int Xjan, int Yjan,
                    SDL_Surface *screen);
      /*! Verify Mouse Press on Buttons 
       * \param mouseX -> mouse X position
       * \param mouseY -> mouse Y position
       * \param Mbuttons -> SDL mouse buttons state
       * \param Xjan -> window X coordinate
       * \param Yjan -> window Y coordinate
       * \return Pointer to the button pressed.*/
      Tobjeto* verifyPosition(int mouseX, int mouseY, Uint8 Mbuttons, 
                          int Xjan, int Yjan);

   private:
      int numButtons;                       /**< Number of active buttons */
      oneTabButton Buttons[MAX_TABBUTTONS]; /**< Buttons Vector */
};


#endif

