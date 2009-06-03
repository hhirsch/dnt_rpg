/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _farso_textbox_h
#define _farso_textbox_h

#include "color.h"
#include "draw.h"
#include "dntfont.h"
#include "guiobject.h"
#include <string>
using namespace std;

#define TEXT_BOX_NOT_FRAMED          0 /**< Text without frames */
#define TEXT_BOX_FRAMED_FILLED       1 /**< Text with frames and BG color */
#define TEXT_BOX_FRAMED_TRANSPARENT  2 /**< Text with frames and transparent */
#define TEXT_BOX_NOT_FRAMED_FILLED   3 /**< Without frames, but filled */

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
      int fontStyle;       /**< Style of the font */
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
       * \param framedType -> type of the frame. 0 for no frames.
       * \param screen -> surface to draw to */
      textBox(int xa, int ya, int xb, int yb, int framedType,
              SDL_Surface *screen);
      /*! Destructor */
      ~textBox();

      /*! Get total lines on current the text box
       * \return -> totalLines on the textBox */
      int getTotalLines();

      /*! Draw in the Surface the text bar
       * \return -> number of the last writed line */
      int draw();

      /*! Draw the textbar (from guiObject)
       * \param screen -> ignored. */
      void draw(SDL_Surface* screen);
      
      /*! Define the Font
       * \param name -> font file name
       * \param align -> font aligment
       * \param size -> font size */
      void setFont(string name, int size, int align);

      /*! Define the Font
       * \param name -> font file name
       * \param align -> font aligment
       * \param size -> font size
       * \param style -> style constant */
      void setFont(string name, int size, int align, int style);

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

      /*! Get the text from the line number
       * \param line -> number of line to get text from 
       * \note -> first line is 0. */
      string getTextLine(int line);

      /*! Set the object text
       * \param txt -> new text */
      void setText(string txt);

      /*! Get the number of the last drawable line
       * \return -> number of the last drawable line */
      int lastDrawableLine();

      /*! Set the first displayed line of the textBox
       * \param line -> line number (from 0 to totalLines) */
      void setFirstLine(int line);

      /*! Set the last displayed line of the textbox, showing as many lines
       * as possible
       * \param line -> number of the line to be the last one */
      void setLastLine(int line);

      /*! Get the current first displayed line on the textBox
       * \return -> first displayed line */
      int getFirstLine();

      /*! Add the string to the textBox
       * \param txt -> text screen to add */
      void addText(string txt);

      /*! Add the string to the textBox
       * \param txt -> text screen to add
       * \param font -> font file to use
       * \param size -> size of the font to use
       * \param align -> alignment of the font to use
       * \param style -> style of the font to use  */
      void addText(string txt, string font, int size,
                   int align, int style);

      /*! Add the string to the textBox
       * \param txt -> text screen to add
       * \param font -> font file to use
       * \param size -> size of the font to use
       * \param align -> alignment of the font to use
       * \param style -> style of the font to use
       * \param R -> red color of the font
       * \param G -> green color of the font
       * \param B -> blue color of the font */
      void addText(string txt, string font, int size,
                   int align, int style, int R, int G, int B);
   private:

      /*! Clear the lines list */
      void clear();

      /*! Create the text lines
       * \param txt -> string base to create the lines */
      void createLines(string txt, string font, int size,
                       int align, int style, int R, int G, int B);

      /*! Insert textLine to the list
       * \param line -> line to insert to the list */
      void insertLine(textLine* line);

      SDL_Surface* wSurface; /**< Window Surface */
      
      textLine* fullText;  /**< The full text */
      int totalLines;      /**< Total Text Lines */
      int firstLine;       /**< Actual Init Line Draw */

      int framed;          /**< Type of the frame. 0 for none */
      string fontName;     /**< Font FileName */
      int fontAlign;       /**< Font Align */
      int fontSize;        /**< Font Size */
      int fontStyle;       /**< Font Style */
      farso_colors Colors; /**< Colors */
};


#endif

