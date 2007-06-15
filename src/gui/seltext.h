/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _seltexto_h
#define _seltexto_h

#include <string>
using namespace std;

#include "draw.h"
#include "fonte.h"
#include "color.h"
#include "mouse.h"
#include "guiobject.h"

#define MAX_OPTIONS 5  /**< Max Options on Sel Text */

/*! Select Text Class (from 1 to 5 texts) */
class selText: public guiObject
{
   public:
      /*! Constructor
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate
       * \param text0 -> option 0 text
       * \param text1 -> option 1 text
       * \param text2 -> option 2 text
       * \param text3 -> option 3 text
       * \param text4 -> option 4 text*/
      selText(int xa,int ya,int xb,int yb, string text0, string text1,
              string text2, string text3, string text4);

      /*! Destructor */
      ~selText();

      /*! Set the SelTexto coordinates
       * \param xa -> x1 coordinate
       * \param xb -> x2 coordinate
       * \param ya -> y1 coordinate
       * \param yb -> y2 coordinate */
      void setCoordinate(int xa,int ya,int xb,int yb);

      /*! Get the SelTexto coordinates
       * \param xa -> x1 coordinate
       * \param xb -> x2 coordinate
       * \param ya -> y1 coordinate
       * \param yb -> y2 coordinate */
      void getCoordinate(int& xa, int& ya, int& xb, int& yb);


      /*! Draw the seltext on a surface
       * \param selectedItem -> number of the selected text
       * \param screen -> surface to draw */
      void draw(int selectedItem, SDL_Surface *screen);

      /*! Threat the selTexto
       * \param screen  -> surface where the selTexto is
       * \param xa -> mouse X coordinate
       * \param ya -> mouse Y coordinate
       * \param Mbotao -> mouse button state*/
      int threat(int xa,int ya, Uint8 Mbotao, SDL_Surface *screen);

      /*! Get the last selected item 
       * \return -> last selected item */
      int getLastSelectedItem();

      /*! Set the text of an option
       * \param opt -> option number
       * \param txt -> option txt */
      void setText(int opt, string txt);

   private:
      /*! Write to the surface the selected item on differente color.
       * \param selectItem -> number of the selected text
       * \param screen -> surface to draw */
      void writeSelected(int selectItem, SDL_Surface *screen);

      /*! based on y, get the seleted item
       * \param ya -> mouse y coordinate
       * \return -1 if no item selected */
      int getSelectedItem(int ya);

   protected:
      int y[MAX_OPTIONS];          /**< Final Y of each option */
      string optText[MAX_OPTIONS]; /**< Text Options */
      int selec;                   /**< last item selected */
      farso_colors Cores;          /**< colors */
      bool pressed;                /**< while pressed == true */


};

#endif


