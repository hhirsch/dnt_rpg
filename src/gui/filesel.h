/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _farso_file_sel_h
#define _farso_file_sel_h

#include <string>

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

class guiList;

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
       * \param list -> pointer to the GUI objects list
       * \param surface -> surface to draw to
       * \param nav -> if will show directories to navigate or not */
      fileSel(int x, int y, bool load, std::string dir, guiList* list, 
            SDL_Surface* surface, bool nav=true);

      /*! Destructor */
      ~fileSel();

      /*! Not used, only for compatibility with guiObject */
      void draw();

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
      std::string getFileName();
 
      /*! Set the current file and dir to the one defined at the fileName
       * \param fileName -> full fileName to set new file and dir. */
      void setFileName(std::string fileName);

      /*! Set the current filter
       * \param newFilter -> new filter to set */
      void setFilter(std::string newFilter);

   protected:

      /*! Change the current displayed dir
       * \param newDir -> dir to change the display to */
      void changeCurDir(std::string newDir);

      /*! Verify if the string pass the current filter
       * \param s -> string to verify
       * \return -> true if passed*/
      bool passFilter(std::string s);

      button* cancelButton;    /**< The cancel Button */
      button* acceptButton;    /**< The accept Button */
      listText* textFiles;     /**< The files selector (as text) */
      textBox* textCurFile;    /**< The current file display (for load) */
      textBar* editCurFile;    /**< The current file edit (for save) */
      textBox* textFilter;     /**< The current filter display */

      bool navDirs;            /**< True if will diplay (and navigate) dirs */
      bool loading;            /**< If selector is a loading one, 
                                    or a saving one */

      std::string curDir;      /**< Current Opened directory */

      std::string filter;      /**< Files Filter */

      int lastDir;             /**< Last element on selector that is a 
                                    directory (this used to separate 
                                    strings directories of  strings files)*/

      guiList* intList;        /**< Internal Gui List used */

      int lastAction;          /**< Last action occurred  */

};


#endif

