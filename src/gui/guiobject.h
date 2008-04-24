/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _guiobject_h
#define _guiobject_h

#define GUI_WINDOW        0
#define GUI_BUTTON        1
#define GUI_TEXT_BAR      2
#define GUI_SEL_BOX       3
#define GUI_PICTURE       4
#define GUI_SEL_TEXT      5
#define GUI_TEXT_BOX      6
#define GUI_MENU_ITEM     7
#define GUI_TAB_BUTTON    8
#define GUI_ROL_BAR       9
#define GUI_LIST_TEXT    10
#define GUI_SEL_FILE     11

#include <SDL/SDL.h>
#include <string>
using namespace std;

/*! The Class guiObject is a generic interface to all GUI related objects. */
class guiObject
{
   public:
      int type;            /**< Object Type */ 
      guiObject* next;     /**< Next Object On List */
      guiObject* previous; /**< Previous Object On List */

      /*! Virtual Destructor */
      virtual ~guiObject(){};

      /*! Virtual Draw Function
       * \param screen -> screen to draw the object to. */
      virtual void draw(SDL_Surface* screen)=0;

      /*! Verify if the mouse is in the object or not
       * \param mouseX -> mouse X Coordinate
       * \param mouseY -> mouse Y Coordinate */
      bool isMouseIn(int mouseX, int mouseY)
      {
         return( ( (mouseX>=x1) && (mouseX<=x2) ) && 
                 ( (mouseY>=y1) && (mouseY<=y2) ) );
      };

      /*! Gets the object text
       * \return object text */
      string getText(){return(text);};

      /*! Set the object text
       * \param txt -> new text */
      void setText(string txt){text = txt;};

      /*! Gets the X1 Coordinate
       * \return x1 coordinate */
      int getX1(){return(x1);};

      /*! Gets the X2 Coordinate
       * \return x2 coordinate */
      int getX2(){return(x2);};

      /*! Gets the y1 Coordinate
       * \return y1 coordinate */
      int getY1(){return(y1);};

      /*! Gets the Y2 Coordinate
       * \return y2 coordinate */
      int getY2(){return(y2);};

      /*! Set Object Coordinate
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate */
      void setCoordinate(int xa, int ya, int xb, int yb)
      {
         x1 = xa; x2 = xb; y1 = ya; y2 = yb;
      };

      /*! Verify if the object is avaible
       * return true if is avaible */
      bool isAvaible() {return(avaible);};

      /*! Set if the item is avaible or not
       * \param av -> true if is avaible, false otherwise */
      void setAvaible(bool av){avaible = av;};

      /*! Verify if the draw state changes */
      bool changed(){bool prev = hadChanged; hadChanged = false;return(prev);};

      /*! Set the window as changed */
      void setChanged(){hadChanged = true;};

   protected:
      int x1,              /**< Coordinate on Window */
          y1,              /**< Coordinate on Window */
          x2,              /**< Coordinate on Window */
          y2;              /**< Coordinate on Window */
      bool avaible;        /**< Avaible? */
      bool hadChanged;           /**< Flag if the had changed its draw state */
      string text;         /**< Text on the bar */
};



#endif

