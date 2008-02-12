/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _rolbar_h
#define _rolbar_h

#include <string>
using namespace std;

#include "guiobject.h"
#include "color.h"
#include "draw.h"
#include "mouse.h"
#include "button.h"
#include "textbox.h"

/* /todo TODO -> support for different font sizes type and colors
 * trought text! */

/*! Rolling Text Bar Definition */
class rolBar: public guiObject
{
   public:
      /*! Constructor
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate
       * \param txt -> text on the rolbar
       * \param list -> pointer to the guiList
       * \param surface -> pointer to the screen surface */
      rolBar(int xa, int ya, int xb, int yb, string txt, void* list, 
             SDL_Surface* surface);
      /*! Destructor */
      ~rolBar();

      /*! Verify if some event is related to the rolBar
       * \param type -> Type of the occurred event
       * \param object -> object of the event 
       * \return -> true if event is related (and threated) by the rolBar */
      bool eventGot(int type, guiObject* object);

      /*! Set The Text of the Bar, putting them on top
       * \param txt -> desired text */
      void setText(string txt);

      /*! Add text to bar 
       * \param txt -> text to add*/
      void addText(string txt);

      /*! Redraw the bar when scrolling text */
      void redraw();

      int getActualInit(){return(actualInit);};

   private:
      SDL_Surface* wSurface;   /**< Window Surface */
      button* up;              /**< Up Button */
      button* down;            /**< Down Button */
      button* actualPressed;   /**< Actual Pressed Button */
      textBox* scrollText;     /**< Text of The Scroll */
      textBox* contorn;        /**< Contorn of the Scroll */
      textBox* position;       /**< Contorn Text Position */

      Uint32 lastUpdated;     /**< Last Time when updated */
      int maxHeight;          /**< Max Height */
      int actualInit;
      int actualEnd;
      void* intList;          /**< Internal GUI Objects List */
};

#endif

