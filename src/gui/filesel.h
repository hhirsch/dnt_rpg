
#ifndef _farso_file_sel_h
#define _farso_file_sel_h

#include <string>
using namespace std;

#include "button.h"
#include "guiobject.h"
#include "color.h"
#include "draw.h"
#include "mouse.h"
#include "listtext.h"
#include "textbar.h"
#include "textbox.h"


/*! The fileSel class implements a file seector/navigator (usefull for
 *  open/close dialogs) */
class fileSel: public guiObject
{
   public:

      /*! Constructor
       * \param x -> x coordinate
       * \param y -> y coordinate
       * \param dir -> directory to display
       * \param list -> pointer to the GUI objects list */
      fileSel(int x, int y, string dir, void* list);

      /*! Destructor */
      ~fileSel();

      /*! Verify if some event is related to the listText
       * \param type -> Type of the occurred event
       * \param object -> object of the event 
       * \return -> true if event is related (and treated) by the listText */
      bool eventGot(int type, guiObject* object);


   protected:

      /*! Change the current displayed dir
       * \param newDir -> dir to change the display to */
      void changeCurDir(string newDir);

      button* cancelButton;    /**< The cancel Button */
      button* acceptButton;    /**< The accept Button */
      listText* textFiles;     /**< The files selector (as text) */
      textBox* textCurFile;    /**< The current file display */

      string curDir;           /**< Current Opened directory */

      void* intList;           /**< Internal Gui List used */

};


#endif

