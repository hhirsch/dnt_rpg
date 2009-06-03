/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _farso_textbar_h
#define _farso_textbar_h

#include <stdlib.h>
#include <string.h>
#include "color.h"
#include "draw.h"
#include "dntfont.h"
#include "mouse.h"
#include "guiobject.h"

/*! Text Bar Definition */
class textBar: public guiObject
{
   public:
      /*! Constructor
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate 
       * \param yb -> y2 coordinate
       * \param text1 -> textBar text
       * \param cripto -> if is crypt, or not
       * \param screen -> surface to draw to */
      textBar(int xa,int ya,int xb,int yb, string text1, bool cripto,
              SDL_Surface* screen);
      /*! Destructor */
      ~textBar();
      
      /*!
       * Draw the text bar on a surface
       * \param screen > suface to draw */
      void draw(SDL_Surface *screen);

      /*!
       * Make the write thing on text bar
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate
       * \param screen -> surface to draw the text bar
       * \param Mbotao -> SDL mouse buttons state info
       * \param teclas -> SDL keyboard state info */
      int doWrite(int mouseX,int mouseY, SDL_Surface *screen, 
                Uint8 Mbotao, Uint8* teclas);

      /*! Set the current text at the bar
       * \param txt -> new text to the bar */
      void setText(string txt);

      /*! Define current cursor position, based on mouse position
       * (for example at a click inside the bar) 
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate */
      void defineCursorPosition(int mouseX, int mouseY);

   private:
      /*! Put the text on the surface
       * \param pos -> current text position
       * \param marca -> current mark position 
       * \param screen -> surface to draw */
      void putText(unsigned int pos, int marca, SDL_Surface *screen);

      bool cript;           /**< Pseudo-criptography? */
      int init,             /**< Actual first visible character on string */
          end;              /**< Actual last visible character on string */
      unsigned int pos;     /**< Current cursor position */
      int lastWrite;        /**< Last time when write */
      string lastChar;      /**< Last Character writed */
      SDL_Surface* wSurface;/**< Window Surface to draw to */
      farso_colors Colors;  /**< Internal Colors */   
};
 
#endif
