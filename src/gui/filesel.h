
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

#define FILE_SEL_ACTION_NONE    0
#define FILE_SEL_ACTION_SELECT  1
#define FILE_SEL_ACTION_ACCEPT  2
#define FILE_SEL_ACTION_CANCEL  3


/*! The fileSel class implements a file seector/navigator (usefull for
 *  open/close dialogs) */
class fileSel: public guiObject
{
   public:

      /*! Constructor
       * \param x -> x coordinate
       * \param y -> y coordinate
       * \param load -> true if the selector is a loading one, false if it
       *                is a saving one. 
       * \param dir -> directory to display
       * \param list -> pointer to the GUI objects list */
      fileSel(int x, int y, bool load, string dir, void* list);

      /*! Destructor */
      ~fileSel();

      /*! Not used, only for compatibility with guiObject
       * \param s -> ignored */
      void draw(SDL_Surface* s);

      /*! Verify if some event is related to the listText
       * \param type -> Type of the occurred event
       * \param object -> object of the event 
       * \return -> true if event is related (and treated) by the listText */
      bool eventGot(int type, guiObject* object);

      /*! Get last action occurred on the file selector
       * \return -> last action occurred */
      int getLastAction();

      /*! Get the current selected fileName
       * \param -> complete selected fileName path (if one) */
      string getFileName();

      /*! Set the current filter
       * \param newFilter -> new filter to set */
      void setFilter(string newFilter);

   protected:

      /*! Change the current displayed dir
       * \param newDir -> dir to change the display to */
      void changeCurDir(string newDir);

      button* cancelButton;    /**< The cancel Button */
      button* acceptButton;    /**< The accept Button */
      listText* textFiles;     /**< The files selector (as text) */
      textBox* textCurFile;    /**< The current file display (for load) */
      textBar* editCurFile;    /**< The current file edit (for save) */
      textBox* textFilter;     /**< The current filter display */

      bool loading;            /**< If selector is a loading one, 
                                    or a saving one */

      string curDir;           /**< Current Opened directory */

      string filter;           /**< Files Filter */

      int lastDir;             /**< Last element on selector that is a 
                                    directory (this used to separate 
                                    strings directories of  strings files)*/

      void* intList;           /**< Internal Gui List used */

      int lastAction;          /**< Last action occurred  */

};


#endif

