/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _textbox_h
#define _textbox_h

#include "color.h"
#include "draw.h"
#include "dntfont.h"
#include "guiobject.h"
#include <string>
using namespace std;

/*! The textLine represents each line of text in the
 * textBox, with his own font parameters and colors */
class textLine
{  
   public:
      string text;         /**< Text of the line */
      int R,               /**< Red Color */
          G,               /**< Green Color */
          B;               /**< Blue Color */
      string fontName;     /**< Name of the Font Used */
      int fontSize;        /**< Size of the font */
      int fontAlign;       /**< Align of the font */
      int height;          /**< Height of the line */

      textLine* next;      /**< Next line */
      textLine* previous;  /**< Previous Line */
};

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

      /*! Get total lines on current the text box
       * \return -> totalLines on the textBox */
      int getTotalLines();

      /*! Draw in the Surface the text bar
       * \param screen -> surface where will draw
       * \return -> number of the last writed line */
      int draw(SDL_Surface *screen);
      
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

      /*! Set the object text
       * \param txt -> new text */
      void setText(string txt);

      int lastDrawableLine();

      void setFirstLine(int line);

      int getFirstLine();

      /*! Add the string to the textBox
       * \param txt -> text screen to add */
      void addText(string txt);

      void addText(string txt, string font, int size,
                   int align);

      void addText(string txt, string font, int size,
                   int align, int R, int G, int B);
   private:

      /*! Clear the lines list */
      void clear();

      /*! Create the text lines
       * \param txt -> string base to create the lines */
      void createLines(string txt, string font, int size,
                       int align, int R, int G, int B);

      /*! Insert textLine to the list
       * \param line -> line to insert to the list */
      void insertLine(textLine* line);
      
      textLine* fullText;  /**< The full text */
      int totalLines;      /**< Total Text Lines */
      int firstLine;       /**< Actual Init Line Draw */

      int framed;          /**< Type of the frame. 0 for none */
      string fontName;     /**< Font FileName */
      int fontAlign;       /**< Font Align */
      int fontSize;        /**< Font Size */
      farso_colors Colors; /**< Colors */
};


#endif

