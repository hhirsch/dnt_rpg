/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _cxsel_h
#define _cxsel_h

#include "desenho.h"
#include "objeto.h"
#include "cores.h"

/*! Selection Box Class */
class cxSel: public Tobjeto
{
   public:
      int x,                 /**< X Window Coordinate */
          y;                 /**< Y Window Coordinate */
      bool selected;         /**< True if selected, false otherwise  */
      cores Colors;          /**< Colors of the object */

      /*! Draws the Selected Box
       * \param screen -> surface */
      void draw(SDL_Surface *screen);

};

#endif

