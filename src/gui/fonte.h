/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _fonte_h
#define _fonte_h

#include "draw.h"
#include <string.h>

#include <string>
using namespace std;

/* Font Aligns */
#define ALIGN_LEFT         0
#define ALIGN_RIGHT        1
#define ALIGN_CENTER       2

#define REPEAT_TIME  100

#define FFARSO     "../data/fnt/farso.fnt"
#define FHELVETICA "../data/fnt/Helve.fnt"
#define FMINI      "../data/fnt/mini.fnt"
#define FNORMAL    "../data/fnt/normal.fnt"
#define FNEGRITO   "../data/fnt/Neg.fnt"
#define FPLATINA   "../data/fnt/Plat.fnt"
#define FTIMES     "../data/fnt/times.fnt"

/*! fnt is a font itself */
typedef struct
{
   unsigned short int letra[257][16]; /**< Each Hexadecimal character */
   unsigned short int incCP;          /**< The Font Increment */
}fnt;

/*! Write the Text on the Surface
 * \param screen -> surface where write the text
 * \param x -> X coordinate of the text
 * \param y -> Y coordinate of the text
 * \param text -> text to write*/
void write(SDL_Surface *screen,int x,int y, const char* text);

/*! Write the Text on the Surface From init to end
 * \param screen -> surface where write the text
 * \param x -> X coordinate of the text
 * \param y -> Y coordinate of the text
 * \param text -> text to write
 * \param init -> init text position to write
 * \param end  -> end text position to write */
void write(SDL_Surface *screen,int x,int y, const char* text,
           int init, int end);

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
int write(SDL_Surface *screen,int x, int y, const char* text,int x1,int y1,
          int x2,int y2);

/*! Gets the incCP of the current font (incCP is how much the X axis
 *  is incremented each wrote character. 
 *  \return the current font incCP*/
int font_incCP();

/*! Define the Active Font
 * \param name -> filename of the font
 * \param align -> font align
 * \param size -> font size
 * \return != 0  if success. */
int defineFont(const char* name, int align, int size);

/*! Copies lines from source, based on | dividers
 * \param source -> Source String
 * \param firstLine -> first line to be copied
 * \param lastLine -> last line to be copied
 * \return -> copied string */
string copyLines(string source, int firstLine, int lastLine);

/*! Count total lines of the source, based on | dividers
 * \param source -> Source String
 * \return total lines of the source string */
int getTotalLines(string source);

#endif

