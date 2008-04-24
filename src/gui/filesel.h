
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


/*! The fileSel class implements a file seector/navigator (usefull for
 *  open/close dialogs) */
class fileSel: public guiObject
{
   public:

      /*! Constructor
       * \param x -> x coordinate
       * \param y -> y coordinate
       * \param dir -> directory to display */
      fileSel(int x, int y, string dir) ;

      /*! Destructor */
      ~fileSel();

   protected:

      /*! Change the current displayed dir
       * \param newDir -> dir to change the display to */
      void changeCurDir(string newDir);

      button* cancelButton;    /**< The cancel Button */
      button* acceptButton;    /**< The accept Button */
      textBar* fileText;       /**< The current file name text */
      listText* textFiles;     /**< The files selector (as text) */

      string curDir;           /**< Current Opened directory */

};


#endif

