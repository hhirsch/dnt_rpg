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

      /*! Verify if the rolBar is the owner of aguiObject
       * \param obj -> pointer to the desired guiObject
       * \return -> true if obj is from the rolBar (a button, for example) */
      bool isOwner(guiObject* obj);

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

      /*! Add the string to the rolBar
       * \param txt -> text screen to add
       * \param font -> font file to use
       * \param size -> size of the font to use
       * \param align -> alignment of the font to use
       * \param style -> style of the font to use  */
      void addText(string txt, string font, int size, int style,
                   int align);

      /*! Add the string to the rolBar
       * \param txt -> text screen to add
       * \param font -> font file to use
       * \param size -> size of the font to use
       * \param align -> alignment of the font to use
       * \param style -> style of the font to use
       * \param R -> red color of the font
       * \param G -> green color of the font
       * \param B -> blue color of the font */
      void addText(string txt, string font, int size, int style,
                   int align, int R, int G, int B);

      /*! Set the first displayed line of the rolBar
       * \param line -> line number (from 0 to totalLines) */
      void setFirstLine(int line);

      /*! Get the current first displayed line on the rolBar
       * \return -> first displayed line */
      int getFirstLine();

      /*! Redraw the bar when scrolling text */
      void redraw();

      /*! Draw the RolBar (from guiObject)
       * \param screen -> window surface */
      void draw(SDL_Surface* screen);

      /*! Get last line text on the rolbar
       * /param -> text of the last line */
      string getLastLine();

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
      
      void* intList;          /**< Internal GUI Objects List */
};

#endif

