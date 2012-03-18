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

#ifndef _farso_font_h
#define _farso_font_h

#include <SDL/SDL.h>

#ifdef __APPLE__
   #include <SDL_ttf/SDL_ttf.h>
#else
   #include <SDL/SDL_ttf.h>
#endif

#include <string>
#include <list>

namespace Farso
{

/*! Single loaded font */
class LoadedFont
{
   public:
      std::string fontName;   /**< The font Name */
      int fontSize;           /**< The font size */

      TTF_Font* font;         /**< The loaded TTF font */
};

/*! The loaded fonts */
class LoadedFontList
{
   public:
      /*! Constructor */
      LoadedFontList();
      /*! Destructor */
      ~LoadedFontList();

      std::list<LoadedFont*>fonts;

};

/*! The font class. Used to write texts on surfaces with ttf fonts. */
class Font
{
   public:

      enum FontAlignEnum
      {
         ALIGN_LEFT=0,
         ALIGN_CENTER
      };

      static const int STYLE_NORMAL = TTF_STYLE_NORMAL;
      static const int STYLE_BOLD = TTF_STYLE_BOLD;
      static const int STYLE_ITALIC = TTF_STYLE_ITALIC;
      static const int STYLE_UNDERLINE = TTF_STYLE_UNDERLINE;
      static const int MAX_TEXT = 8192;


      /*! Init the font system to use. \note Only needed to be called one 
       * time at the init of the engine. */
      void init();
      /*! End the font system. \note Only needed to be called at 
       * the exit of the engine. */
      void end();

      /*! Define the Active Font
       * \param fileName -> filename of the font
       * \param size -> font size
       * \return -> true if success. */
      bool defineFont(std::string fileName, int size);

      /*! Define the font align
       * \param align -> the new font align */
      void defineFontAlign(int align);

      /*! Define the current font style (normal, bold or italic)
       * \param style -> font style constant (and combinations) */
      void defineFontStyle(int style);
   
      /*! Set the font outline */
      void defineFontOutline(int outLine);

      /*! Get the max width of the font (in a "gambiarra")  */
      int getIncCP();

      /*! Get the normal height of the font
       * \return normal height of the font */
      int getHeight();

      /*! Get the string width
       * \param s -> string to calculate width
       * \return string width */
      int getStringWidth(std::string s);

      /*! Get the string width
       * \param s -> string to calculate width
       * \param isUtf8 -> if string in utf8
       * \return string width */
      int getStringWidth(std::string s, bool isUtf8);

      /*! Write the Text on the Surface
       * \param screen -> surface where write the text
       * \param x -> X coordinate of the text
       * \param y -> Y coordinate of the text
       * \param text -> text to write*/
      void write(SDL_Surface *screen,int x,int y, std::string text, 
                 bool solid=false);

      /*! Write the Text on the Surface From init to end
       * \param screen -> surface where write the text
       * \param x -> X coordinate of the text
       * \param y -> Y coordinate of the text
       * \param text -> text to write
       * \param init -> init text position to write
       * \param end  -> end text position to write */
      void write(SDL_Surface *screen,int x,int y, std::string text,
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
      int write(SDL_Surface *screen,int x, int y, std::string text,
                int x1,int y1,int x2,int y2, bool solid=false);

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
      int write(SDL_Surface *screen,int x,int y, std::string text,int init,
                int end, int x1,int y1,int x2,int y2, bool solid=false);

      /*! Write the Text on the Surface, with limit area, at a single area
       * \param screen -> surface where write the text
       * \param x -> X coordinate of the text
       * \param y -> Y coordinate of the text
       * \param text -> text to write
       * \param x1 -> X1 limit
       * \param y1 -> Y1 limit
       * \param x2 -> X2 limit
       * \param y2 -> Y2 limit */
      void writeSingleLine(SDL_Surface* screen, int x, int y, 
                           std::string text, int x1, int y1, int x2, int y2, 
                           bool solid=false);

      /*! Write the Text on the Surface
       * \param screen -> surface where write the text
       * \param x -> X coordinate of the text
       * \param y -> Y coordinate of the text
       * \param text -> text to write, coded on unicode mode */
      void writeUnicode(SDL_Surface* screen, int x, int y, std::string text,
                        bool solid=false);

      /*! Create a special unicode character (beginning with \\)
       * \param character -> unicode character code
       * \return -> string with the unicode character begining with \\*/
      std::string createUnicode(Uint16 character);

      /*! Get Next line of the string.
       * \param source -> string to get line from
       * \param lastLinePos -> last position read (at init = 0)
       * \param maxWidth -> max width of the line
       * \return -> the desired line got. */
      std::string getNextLine(std::string source, int& lastLinePos, 
                              int maxWidth);

   protected:

      /*! Load a font to the list, if it not already there
       * \param fontName -> file name of the font to load
       * \param fontSize -> size of the font to load
       * \return -> loadedFont pointer */
      LoadedFont* loadFont(std::string fontName, int fontSize);
      /*! Search for a font on the list
       * \param fontName -> file name of the font to load
       * \param fontSize -> size of the font to load
       * \return -> loadedFont pointer with the founded font, 
       *             or NULL, if not found. */
      LoadedFont* findFont(std::string fontName, int fontSize);

      /*! Render text to a new surface
       * \param str -> text to render
       * \param color -> color to render text
       * \param isUtf8 -> if text in utf8 */
      SDL_Surface* renderText(std::string str, SDL_Color color, bool isUtf8);

      /*! Blit the text to the screen surface*/
      void blitText(SDL_Surface *screen, SDL_Surface* writeSurface, 
                    int x,int y, std::string text, int x1,int y1,
                    int x2,int y2, bool solid, bool isUtf8);
      
      static LoadedFontList* fonts;  /**< List of loaded fonts */
      static int activeFontAlign;    /**< The active font alignment */
      static int activeFontStyle;    /**< The active Font Style */
      static LoadedFont* activeFont; /**< The active font */
      static SDL_Rect rect; /**< rectangle */

};

}

#endif

