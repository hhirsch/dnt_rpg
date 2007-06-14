/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _guiobject_h
#define _guiobject_h

#define GUI_WINDOW        0
#define GUI_BUTTON        1
#define GUI_TEXT_BAR      2
#define GUI_SEL_BOX       3
#define GUI_PICTURE       4
#define GUI_SEL_TEXT      5
#define GUI_TEXT_BOX      6
#define GUI_MENU_ITEM     7
#define GUI_TAB_BUTTON    8
#define GUI_ROL_BAR       9
#define GUI_LIST_TEXT    10

#include <string>
using namespace std;

/*! The Class guiObject is a generic interface to all GUI related objects. */
class guiObject
{
   public:
      int type;            /**< Object Type */ 
      guiObject* next;     /**< Next Object On List */
      guiObject* previous; /**< Previous Object On List */

      /*! Verify if the mouse is in the object or not
       * \param mouseX -> mouse X Coordinate
       * \param mouseY -> mouse Y Coordinate */
      bool isMouseIn(int mouseX, int mouseY)
      {
         return( ( (mouseX>=x1) && (mouseX<=x2) ) && 
                 ( (mouseY>=y1) && (mouseY<=y2) ) );
      };

      /*! Gets the object text
       * \return object text */
      string getText(){return(text);};

      /*! Set the object text
       * \param txt -> new text */
      void setText(string txt){text = txt;};

   protected:
      int x1,              /**< Coordinate on Window */
          y1,              /**< Coordinate on Window */
          x2,              /**< Coordinate on Window */
          y2;              /**< Coordinate on Window */
      bool avaible;        /**< Avaible? */
      string text;         /**< Text on the bar */
};



#endif

