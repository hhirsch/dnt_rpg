/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _farso_guiobject_h
#define _farso_guiobject_h

#include <SDL/SDL.h>
#include <iostream>
#include <string>
using namespace std;

/*! The farso guiObject types */
enum
{
   FARSO_OBJECT_WINDOW,
   FARSO_OBJECT_BUTTON,
   FARSO_OBJECT_TEXT_BAR,
   FARSO_OBJECT_SEL_BOX,
   FARSO_OBJECT_PICTURE,
   FARSO_OBJECT_SEL_TEXT,
   FARSO_OBJECT_TEXT_BOX,
   FARSO_OBJECT_MENU_ITEM,
   FARSO_OBJECT_TAB_BUTTON,
   FARSO_OBJECT_ROL_BAR,
   FARSO_OBJECT_LIST_TEXT,
   FARSO_OBJECT_FILE_SEL,
   FARSO_OBJECT_HEALTH_BAR,
   FARSO_OBJECT_TAB_BOX
};

/*! The Class guiObject is a generic interface to all GUI related objects. */
class guiObject
{
   public:
      int type;            /**< Object Type */ 
      guiObject* next;     /**< Next Object On List */
      guiObject* previous; /**< Previous Object On List */

      /*! Constructor */
      guiObject(){available=true;text="";};

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

      /*! Verify if the object is available
       * return true if is avaible */
      bool isAvailable() {return(available);};

      /*! Set if the item is available or not
       * \param av -> true if is available, false otherwise */
      void setAvailable(bool av){available = av;};

      /*! Verify if the draw state changes */
      bool changed(){bool prev = hadChanged; hadChanged = false;return(prev);};

      /*! Set the window as changed */
      void setChanged(){hadChanged = true;};

   protected:
      int x1,              /**< Coordinate on Window */
          y1,              /**< Coordinate on Window */
          x2,              /**< Coordinate on Window */
          y2;              /**< Coordinate on Window */
      bool available;      /**< Available? */
      string text;         /**< Text on the bar */
   
   private:
      bool hadChanged;     /**< Flag if the had changed its draw state */
};



#endif

