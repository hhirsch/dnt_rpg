/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _farso_tab_button_h
#define _farso_tab_button_h

#include "picture.h"
#include "color.h"

#define MAX_TABBUTTONS      20 /**< Max Buttons per table of buttons */
#define TABBUTTON_NONE       0 /**< No Action */
#define TABBUTTON_ON_PRESS       40 /**< Some button pressing */
#define TABBUTTON_PRESSED        41 /**< Some Button pressed */
#define TABBUTTON_ON_RIGHT_PRESS 42 /**< Right pressing some button */
#define TABBUTTON_RIGHT_PRESSED  43 /**< Some Button right pressed */
#define TABBUTTON_BELLOW    32 /**< Max guiObject bellow */

enum
{
   FARSO_TAB_BUTTON_STYLE_NORMAL,
   FARSO_TAB_BUTTON_STYLE_HIGH,
   FARSO_TAB_BUTTON_STYLE_LIST_TEXT
};

/*! oneTabButton is a class to define what is a button on a table of buttons */
class oneTabButton: public guiObject
{
   public:
      /*! Constructor */
      oneTabButton():guiObject(NULL){};
      /*! From guiObject */
      void draw(){};

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
       * \param arquivo -> fileName of the image to Load 
       * \param surface -> window surface where object is */
      tabButton(int x,int y,const char* arquivo, SDL_Surface* surface);
      /*! Constructor without Image
       * \param x -> x position of the tab button
       * \param y -> y position of the tab button
       * \param w -> width of the tab button
       * \param h -> heigh of the tab button 
       * \param surface -> window surface where object is */
      tabButton(int x, int y, int w, int h, SDL_Surface* surface);
      /*! Destructor */
      ~tabButton(){};
      /*! insert Button in the table of buttons
       * \param x1 -> X Coordinate
       * \param y1 -> Y Coordinate
       * \param x2 -> X Coordinate 
       * \param y2 -> Y Coordinate 
       * \return Pointer to the oneTableButton Created */
      oneTabButton* insertButton(int x1, int y1, int x2, int y2); 
      /*! Draws the table button to the surface */
      void draw();
      /*! Verify Mouse Press on Buttons 
       * \param mouseX -> mouse X position
       * \param mouseY -> mouse Y position
       * \param Mbuttons -> SDL mouse buttons state
       * \param Xjan -> window X coordinate
       * \param Yjan -> window Y coordinate
       * \param actionType -> ID of action done
       * \return Pointer to the button pressed.*/
      guiObject* verifyPosition(int mouseX, int mouseY, Uint8 Mbuttons, 
                                int Xjan, int Yjan, int& actionType);


      /*! Set the tabButton style
       * \param st -> new style */
      void setStyle(int st);

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
      bool rightPressed;                    /**< Internal Verifier on Treat */
      int current;                          /**< Current Button selected */
      int style;                            /**< Current Style */
      guiObject* objectsBelow[TABBUTTON_BELLOW];  /**< Object Below */
      int curBelow;                         /**< Current objects below */
      farso_colors cor;
};


#endif

