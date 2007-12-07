#ifndef _dntfont_h
#define _dntfont_h

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>
using namespace std;

#define DNT_FONT_ARIAL     "../data/fnt/arial.ttf"
#define DNT_FONT_TIMES     "../data/fnt/times.ttf"
#define DNT_FONT_COURRIER  "../data/fnt/courrier.ttf"

#define DNT_FONT_ALIGN_LEFT    0
#define DNT_FONT_ALIGN_CENTER  1

#define MAX_TEXT  8192

/*! The DNT font class. Used to write texts on surfaces with ttf fonts. */
class dntFont
{
   public:
      /*! Init the font system to use. \note Only needed to be called one time at 
       * the init of the engine. */
      void init();
      /*! End the font system. \note Only needed to be called at the exit of the
       * engine. */
      void end();

      /*! Define the Active Font
       * \param fileName -> filename of the font
       * \param size -> font size
       * \return -> true if success. */
      bool defineFont(string fileName, int size);

      /*! Define the font align
       * \param align -> the new font align */
      void defineFontAlign(int align);

      /*! Get the max width of the font (in a "gambiarra")  */
      int getIncCP();

      /*! Get the normal height of the font
       * \return normal height of the font */
      int getHeight();

      /*! Get the string width
       * \param s -> string to calculate width
       * \return string width */
      int getStringWidth(string s);

      /*! Write the Text on the Surface
       * \param screen -> surface where write the text
       * \param x -> X coordinate of the text
       * \param y -> Y coordinate of the text
       * \param text -> text to write*/
      void write(SDL_Surface *screen,int x,int y, string text, 
                 bool solid=false);

      /*! Write the Text on the Surface From init to end
       * \param screen -> surface where write the text
       * \param x -> X coordinate of the text
       * \param y -> Y coordinate of the text
       * \param text -> text to write
       * \param init -> init text position to write
       * \param end  -> end text position to write */
      void write(SDL_Surface *screen,int x,int y, string text,
                 int init, int end, bool solid=false);

      /*! Write the Text on the Surface, with limit area
       * \param screen -> surface where write the text
       * \param x -> X coordinate of the text
       * \param y -> Y coordinate of the text
       * \param text -> text to write
       * \param x1 -> X1 limit
       * \param y1 -> Y1 limit
       * \param x2 -> X2 limit
       * \param y2 -> Y2 limit
       * \return -> Y where ends the write. */
      int write(SDL_Surface *screen,int x, int y, string text,int x1,int y1,
                int x2,int y2, bool solid=false);

      /*! Write the Text on the Surface, with limit area
       * \param screen -> surface where write the text
       * \param x -> X coordinate of the text
       * \param y -> Y coordinate of the text
       * \param init -> init text position to write
       * \param end  -> end text position to write
       * \param text -> text to write
       * \param x1 -> X1 limit
       * \param y1 -> Y1 limit
       * \param x2 -> X2 limit
       * \param y2 -> Y2 limit
       * \return -> Y where ends the write.*/
      int write(SDL_Surface *screen,int x,int y,string text,int init,
                int end, int x1,int y1,int x2,int y2, bool solid=false);

      /*! Write the Text on the Surface
       * \param screen -> surface where write the text
       * \param x -> X coordinate of the text
       * \param y -> Y coordinate of the text
       * \param text -> text to write, coded on unicode mode */
      void writeUnicode(SDL_Surface* screen, int x, int y, string text,
                        bool solid=false);

      /*! Create a special unicode character (beginning with \\)
       * \param character -> unicode character code
       * \return -> string with the unicode character begining with \\*/
      string createUnicode(Uint16 character);

      /*! Copies lines from source, based on | dividers
       * \param source -> Source String
       * \param firstLine -> first line to be copied
       * \param lastLine -> last line to be copied
       * \param x1 -> first X coordinate
       * \param x2 -> last X coordinate
       * \return -> copied string */
      string copyLines(string source, int firstLine, int lastLine, 
                       int x1, int x2);

      /*! Count total lines of the source, based on | dividers
       * \param source -> Source String
       * \param x1 -> first X coordinate
       * \param x2 -> last X coordinate
       * \return total lines of the source string */
      int getTotalLines(string source, int x1, int x2);

      Uint16* convertToUnicode(Uint16 *unicode, const char *text, int len);

   protected:
      static TTF_Font* activeFont;  /**< The active Font */
      static string activeFontName; /**< The active font file name */
      static int activeFontSize;    /**< The active font size */
      static int activeFontAlign;   /**< The active Font Align */

      Uint16 curUnicode[MAX_TEXT];
      Uint16 strLine[MAX_TEXT];
};


#endif

