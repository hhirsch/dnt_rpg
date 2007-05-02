/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _rolbar_h
#define _rolbar_h

#include <string>
using namespace std;

#include "objeto.h"
#include "cores.h"
#include "desenho.h"
#include "mouse.h"
#include "botao.h"
#include "quadtexto.h"

/*! Rolling Text Bar Definition */
class rolBar: public Tobjeto
{
   public:
      /*! Constructor */
      rolBar(int xa, int ya, int xb, int yb, string txt, void* list, 
             SDL_Surface* surface);
      /*! Destructor */
      ~rolBar();

      /*! Verify if some event is related to the rolBar
       * \param type -> Type of the occurred event
       * \param object -> object of the event 
       * \return -> true if event is related (and threated) by the rolBar */
      bool eventGot(int type, Tobjeto* object);

      /*! Set The Text of the Bar, putting them on top
       * \param txt -> desired text */
      void setText(string txt);

      /*! Add text to bar 
       * \param txt -> text to add*/
      void addText(string txt);

      /*! Redraw the bar when scrolling text */
      void redraw();

      /*! Get the actual init of text displayed
       * \return actualInit */
      Uint32 getActualInit(){return(actualInit);};

   private:
      SDL_Surface* wSurface;  /**< Window Surface */
      int x1,                 /**< X1 Coordinate */
          x2,                 /**< X2 Coordinate */
          y1,                 /**< Y1 Coordinate */
          y2;                 /**< Y2 Coordinate */
      botao* up;              /**< Up Button */
      botao* down;            /**< Down Button */
      botao* actualPressed;   /**< Actual Pressed Button */
      quadroTexto* text;      /**< Text of The Scroll */
      quadroTexto* contorn;   /**< Contorn of the Scroll */
      quadroTexto* position;  /**< Contorn Text Position */

      string fullText;        /**< Full Text of the rolBar */
      Uint32 actualInit,      /**< Actual Initial Line of the Text */
             actualEnd;       /**< Actual End Line of The Text */
      Uint32 lastActualized,  /**< Last Time when actualized */
             maxLines;        /**< Max Number of Text Lines */
      int charPerLine;        /**< Number of Characters per Line of Text */
      Uint32 totalLines;      /**< Total Full Text Lines */
      void* intList;          /**< Internal GUI Objects List */
};

#endif

