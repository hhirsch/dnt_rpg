/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _menu_h
#define _menu_h

#include <stdlib.h>
#include <string>
using namespace std;
#include "color.h"
#include "draw.h"
#include "dntfont.h"
#include "mouse.h"
#include "guiobject.h"
#include "guilist.h"

#define MENU_ITEM_HEIGHT  12

/*! The Menu GUI class */
class menu: public guiList
{
   public:
      /*! Constructor
       * \param xa -> x coordinate
       * \param ya -> y coordinate */
      menu(int xa, int ya);

      /*! Destructor */
      ~menu();

      /*! Insert a Item on the Menu
       * \param text -> item Text
       * \param avaible -> if the item is avaible or not */
      void insertItem(string text, bool avaible);

      /*! Insert a Item on the Menu
       * \param text -> item Text
       * \param imageFile -> item Image
       * \note imageFile must be of Size: 10x10 pixels
       * \param avaible -> if the item is avaible or not */
      void insertItem(string text, string imageFile, bool avaible);

      /*! Set the Position of the Menu
       * \param xa -> x coordinate
       * \param ya -> y coordinate */
      void setPosition(int xa, int ya);

      /*! Do the execution of the menu. Return selected item or -1,
       * \param mouseX -> X mouse coordinate
       * \param mouseY -> Y mouse coordinate
       * \param Mbotao -> mouse button state
       * \param teclado -> keyboard state
       * \param screen -> superface where the menu is
       * \param pronto -> it will be 1 when the execution ends 
       * \param Xjan -> x coordinate of the window
       * \param Yjan -> y coordinate of the window */
      int run(int mouseX, int mouseY, Uint8 Mbotao, Uint8* teclado,
              SDL_Surface *screen, int* pronto, int Xjan, int Yjan);

      /*! Get a item
       * \param i -> position of the item on menu
       * \return pointer to desired item, or NULL, if not exists. */
      guiObject* getItem(int i);

      /*! Get the actual Item */
      int getActualItem();

      /*! Get the actual max character */
      int getMaxCharac();

      /*! Verify if the item is available
       * \param item -> item number on the menu
       * \return -> true if is available, false otherwise */
      bool itemAvailable(int item);

      /*! Set the item avaible status
       *  \param item -> item number on the menu
       *  \param av -> if available or not */
      void setItemAvailable(int item, bool av);

   protected:
      /*! Draw the Menu
       * \param pos -> menu position selected 
       * \param screen -> surface to draw */
      void draw(int pos, SDL_Surface *screen);

      int x,                   /**< Menu X Coordinate */
          y;                   /**< Menu Y Coordinate */   
      unsigned int maxCharac;  /**< Size of the bigger string on menu */
      farso_colors Colors;     /**< Colors */
      int numPictures;         /**< Number of pictures at the menu */
      int actualItem;          /**< Actual Selected item */
      bool pressed;            /**< Control to take the release button press */

};

#endif

