/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _cxsel_h
#define _cxsel_h

#include "desenho.h"
#include "guiobject.h"
#include "color.h"

/*! Selection Box Class */
class cxSel: public guiObject
{
   public:
      int x,                 /**< X Window Coordinate */
          y;                 /**< Y Window Coordinate */

      /*! Constructor */
      cxSel();
      /*! Draws the Selected Box
       * \param screen -> surface */
      void draw(SDL_Surface *screen);

      /*! Verify if the cxSel is selected 
       * \return true if the cxsel is selected */
      bool isSelected();

      /*! Invert the selection of the cxSel */
      void invertSelection();

      /*! Set the state to value
       * \param value -> new value of the selection */
      void setSelection(bool value);

   private:
      farso_colors Colors;   /**< Colors of the object */
      bool selected;         /**< True if selected, false otherwise  */
      GLuint lastChangeTime; /**< Last time when box was changed */

};

#endif

