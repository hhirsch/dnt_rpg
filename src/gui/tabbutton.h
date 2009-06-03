/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _tabbotao_h
#define _tabbotao_h

#include "picture.h"
#include "color.h"

#define MAX_TABBUTTONS      20 /**< Max Buttons per table of buttons */
#define TABBUTTON_NONE       0 /**< No Action */
#define TABBUTTON_ON_PRESS  40 /**< Some button pressing */
#define TABBUTTON_PRESSED   41 /**< Some Button pressed */

/*! oneTabButton is a class to define what is a button on a table of buttons */
class oneTabButton: public guiObject
{
   public:
      /*! From guiObject. Not used. */
      void draw(SDL_Surface* screen){};

      int x1,  /**< X Coordinate */
          y1,  /**< Y Coordinate */
          x2,  /**< X Coordinate */
          y2;  /**< Y Coordinate */
};

/*! The table of Buttons class. Its a figure divided in squares each one 
    representing (and acting as) a button. */
class tabButton: public picture
{
   public:
      /*! Constructor 
       * \param x -> x position of the tab button
       * \param y -> y position of the tab button
       * \param arquivo -> fileName of the image to Load */
      tabButton(int x,int y,const char* arquivo);
      /*! Constructor without Image
       * \param x -> x position of the tab button
       * \param y -> y position of the tab button
       * \param w -> width of the tab button
       * \param h -> heigh of the tab button */
      tabButton(int x, int y, int w, int h);
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
       * \param screen -> pointer to the surface where the table will be 
       *                  draw (usually an window).  */
      void draw(SDL_Surface *screen);
      /*! Verify Mouse Press on Buttons 
       * \param mouseX -> mouse X position
       * \param mouseY -> mouse Y position
       * \param Mbuttons -> SDL mouse buttons state
       * \param Xjan -> window X coordinate
       * \param Yjan -> window Y coordinate
       * \param screen -> surface where will draw
       * \param actionType -> ID of action done
       * \return Pointer to the button pressed.*/
      guiObject* verifyPosition(int mouseX, int mouseY, Uint8 Mbuttons, 
                                int Xjan, int Yjan, SDL_Surface *screen,
                                int& actionType);


      /*! Set the current button
       * \param i -> current button number */
      void setCurrent(int i);

      /*! Set the object below the tabbuton (this object will always draw
       * before draw the tabButton) */
      void setObjectBelow(guiObject* obj);

   private:
      int numButtons;                       /**< Number of active buttons */
      oneTabButton Buttons[MAX_TABBUTTONS]; /**< Buttons Vector */
      bool pressed;                         /**< Internal Verifier on Treat */
      int current;                          /**< Current Button selected */
      guiObject* objectBelow;               /**< Object Below */
      farso_colors cor;
};


#endif

