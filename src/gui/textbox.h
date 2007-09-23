/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _textbox_h
#define _textbox_h

#include "color.h"
#include "draw.h"
#include "fonte.h"
#include "guiobject.h"
#include <string>
using namespace std;

/*! Class of text box representation. */
class textBox: public guiObject
{
   public:
      /*! Constructor 
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate
       * \param framedType -> type of the frame. 0 for no frames. */
      textBox(int xa, int ya, int xb, int yb, int framedType);
      /*! Destructor */
      ~textBox();
      
      /*! Draw in the Surface the text bar (quadroTexto)
       * \param screen -> surface where quadroTexto will draw */
      void draw(SDL_Surface *screen);

      /*! Define the Font
       * \param name -> font file name
       * \param align -> font aligment
       * \param size -> font size */
      void setFont(string name, int size, int align);

      /*! Set the text color
       * \param R -> red color
       * \param G -> green color
       * \param B -> blue color */
      void setColor(int R, int G, int B);

      /*! Set the Back color
       * \param R -> red color
       * \param G -> green color
       * \param B -> blue color */
      void setBackColor(int R, int G, int B);
   private:
      int framed;          /**< Type of the frame. 0 for none */
      string fontName;     /**< Font FileName */
      int fontAlign;       /**< Font Align */
      int fontSize;        /**< Font Size */
      farso_colors Colors; /**< Colors */
};


#endif

