/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _window_h
#define _window_h

#include <stdlib.h>
#include "draw.h"
#include "color.h"
#include "fonte.h"
#include "guiobject.h"
#include "guilist.h"

#include <string>
using namespace std;

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
       * \param list -> guiList pointer to the list where the window is */
      window(int xa, int ya, int xb, int yb, string title, void* list);

      /*! Destructor */
      ~window();
      
      /*! Draw the Window to its internal surface
       * \param mouseX -> mouse X position
       * \param mouseY -> mouse Y position */
      void draw(int mouseX, int mouseY);

      /*! Activate the Window */
      void activate();

      /*! Open the Window */
      void open();

      /*! Do the Movimentation of the window
       * \param backGround -> surface used as background image, if have one.
       * \param xinic -> init X clicked
       * \param yinic -> init Y clicked
       * \param Mbotao -> mouse button state */
      int doMove(SDL_Surface* backGround, int xinic, int yinic, int Mbotao);

      /*! Get the object list
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

   protected:
      void* intList;             /**< Pointer to the list where the window is */
      int minX,                  /**< Min Size of X axys */
          minY;                  /**< Min Size of the Y axys */
      bool canMaximize;          /**< If can maximize the window */
      bool canScale;             /**< If can scale the window */
      bool canClose;             /**< If can close the window */
      bool canMove;              /**< If can move the window */
      guiList* objects;          /**< Objects on the Window */
      SDL_Surface *surface;      /**< Window Surface */
      int difx;                  /**< Difference of X where mouse clicked */
      int dify;                  /**< Difference of Y where mouse clicked */
      window **externPointer;    /**< Pointer to the extern window pointer */
      farso_colors Colors;       /**< Colors Used */

      /*! Draw the Title Bar as Inactive */
      void drawInactiveBar();

      /*! Draw the Title bar as Active */
      void drawActiveBar();
};

/*! The List of Windows Class (windowList) */
class windowList
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
       * \param title -> title of the window */
      window* insertWindow(int xa,int ya,int xb,int yb,string text);

      /*! Remove a window from the list
       * \param jan -> pointer to window to remove */
      void removeWindow(window *jan);

      /*! Get the active window
       * \return pointer to the active window */
      window* getActiveWindow(){return(activeWindow);};

      /*! Get the active window
       * \param jan -> pointer to the new active window */
      void setActiveWindow(window* jan){activeWindow = jan;};


      /*! Get the first window on the list (head Node)
       * \return pointer to the first list */
      window* getFirst(){return(first);};

      /*! Get the total number of windows on list
       * \return number of windows on list */
      int getTotal(){return(total);};


      /*! Add a internal Menu to the list
       * \return menu Inserted */
      guiObject* addMenu();

      /*! Get the Internal Menu of The List
       * \return internal menu */
      guiObject* getMenu();

      /*! Remove Internal Menu, if exists one. */
      void removeMenu();
   protected:
      window* activeWindow;   /**< The Active Window */
      window* first;          /**< Head Node */
      guiObject* intMenu;     /**< The internal Menu of the List */
      int total;              /**< Total Windows on the list */
}; 



#endif

