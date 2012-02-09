/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

#ifndef _farso_window_h
#define _farso_window_h

#include <stdlib.h>
#include "draw.h"
#include "color.h"
#include "dntfont.h"
#include "guiobject.h"
#include "guilist.h"
#include "tabbox.h"
#include "../lang/translate.h"

#include <string>


#define WINDOW_MENU_MIN_MAX  1
#define WINDOW_MENU_CLOSE    3

class windowList;

/*! The Window Object */
class window: public guiObject
{
   public:
      /*! Constructor
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate 
       * \param title -> title of the window
       * \param list -> guiList pointer to the list where the window is 
       * \param empty -> with window is without any self draw */ 
      window(int xa, int ya, int xb, int yb, std::string title,
             windowList* list, bool empty=false);

      /*! Destructor */
      ~window();
      
      /*! Draw the Window to its internal surface
       * \param mouseX -> mouse X position
       * \param mouseY -> mouse Y position
       * \param drawBar -> if will draw the bar or not */
      void draw(int mouseX, int mouseY, bool drawBar=true);

      /*! Draw, from guiObject */
      void draw();

      /*! Activate the Window */
      void activate();

      /*! Open the Window */
      void open();

      /*! Hide the Window (opposite of show()) */
      void hide();

      /*! Show the window (opposite of hide()) */
      void show();

      /*! True if the Window is visible (not hide), false if is hided
       * \return true if not hide */
      bool isVisible();

      /*! Do the Movimentation of the window
       * \param backGround -> surface used as background image, if have one.
       * \param xinic -> init X clicked
       * \param yinic -> init Y clicked
       * \param Mbotao -> mouse button state */
      int doMove(SDL_Surface* backGround, int xinic, int yinic, int Mbotao);

      /*! Get the objects list
       * \return guiList of objects on the window */
      guiList* getObjectsList(){return(objects);};

      /*! Get the surface of the window
       * \return window surface */
      SDL_Surface* getSurface(){return(surface);};

      /*! Set the Atributes of the window
       * \param close -> if can close
       * \param move -> if can move
       * \param scale -> if can scale
       * \param maximize -> if can maximize */
      void setAttributes(bool close, bool move, bool scale, bool maximize);

      /*! Verify if the window can close
       * \return true if can close, false, otherwise */
      bool canCloseWindow(){return(canClose);};

      /*! Verify if the window can move
       * \return true if can move, false, otherwise */
      bool canMoveWindow(){return(canMove);};

      /*! Set the extern pointer of the window */
      void setExternPointer(window** extr){externPointer=extr;};

      /*! Set the mouse click difference, usually for move the window */
      void setDiff(int x, int y){difx = x; dify = y;};

      /*! Flush the surface to the texture */
      void flush();

      /*! Render the current window "face" to the screen
       * \param depth -> depth to render the window */
      void render(float depth);

      /*! Verify if the window face changes
       * \return true if some object changed its draw state (or the window
       * itself)  */
      bool changed();

      /*! Verify if the window is a modal one or not
       * \return -> true if is a modal window, false otherwise */
      bool isModal();

      /*! Set the window as a modal one
       * \note the modal window don't allow window changes 
       * while it is opened */
      void setModal();

   protected:
      windowList* intList;       /**< Pointer to the list where the window is */
      int minX,                  /**< Min Size of X axys */
          minY;                  /**< Min Size of the Y axys */
      bool canMaximize;          /**< If can maximize the window */
      bool canScale;             /**< If can scale the window */
      bool canClose;             /**< If can close the window */
      bool canMove;              /**< If can move the window */
      bool modal;                /**< If the window is modal or not */
      bool visible;              /**< If the window is visible or not */
      guiList* objects;          /**< Objects on the Window */
      SDL_Surface *surface;      /**< Window Surface */
      GLuint texture;            /**< GL texture for the window */
      GLfloat alpha;             /**< Alpha Value of the texture \fixme! */
      GLfloat propX,             /**< Texture X proportion */
              propY;             /**< Texture Y proportion */
      int difx;                  /**< Difference of X where mouse clicked */
      int dify;                  /**< Difference of Y where mouse clicked */
      window **externPointer;    /**< Pointer to the extern window pointer */
      farso_colors Colors;       /**< Colors Used */
      button* closeButton;    /**< Button to close Window */
      button* menuButton;     /**< Button to open Window Menu */
      button* minMaxButton;   /**< Button to minimize, maximize the window */

      bool hasSelfDraw;          /**< If has title bar or not */

      /*! Draw the Title Bar as Inactive */
      void drawInactiveBar();

      /*! Draw the Title bar as Active */
      void drawActiveBar();
};

/*! The List of Windows Class (windowList) */
class windowList: public dntList
{
   public:
      /*! Constructor */
      windowList();

      /*! Destructor */
      ~windowList();
 
      /*! Insert a new window on the list
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate 
       * \param text -> title of the window
       * \param empty -> with window is without any self draw */ 
      window* insertWindow(int xa,int ya,int xb,int yb, std::string text, 
            bool empty=false);

      /*! Remove a window from the list
       * \param jan -> pointer to window to remove */
      void removeWindow(window *jan);

      /*! Get the active window
       * \return pointer to the active window */
      window* getActiveWindow(){return(activeWindow);};

      /*! Set the active window
       * \param jan -> pointer to the new active window */
      void setActiveWindow(window* jan);

   protected:

      /*! Delete a window pointer
       * \param obj -> pointer to the window */
      void freeElement(dntListElement* obj);

      /*! Get the most recent modal window on the list
       * \return pointer to the most recent modal window or NULL if no modal 
       *         windos at all. */
      window* getModalWindow();

      window* activeWindow;   /**< The Active Window */
}; 



#endif

