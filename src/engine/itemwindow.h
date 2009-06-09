/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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

#ifndef _dnt_itemwindow_h
#define _dnt_itemwindow_h

#include "../gui/farso.h"
#include "../classes/object.h"

/*! The item window is the one that shows objects infos to the user */
class itemWindow
{
   public:
      /*! Constructor
       * \param interf -> gui interface to open the window */
      itemWindow(guiInterface* interf);

      /*! Destructor */
      ~itemWindow();

      /*! Open the Item info window
       * \param item -> object to show info */
      void open(object* item);

      /*! Close the item info window */
      void close();

      /*! Treat Events on Window. 
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return != 0 if event is gathered */
      int treat(guiObject* object, int eventInfo);

      /*! Verify if the window is openned
       * \return true if opened, false otherwise. */
      bool isOpen();

   protected:
      window* intWindow;         /**< Pointer to the internal window */
      object* curObject;         /**< Pointer to current object */

      guiInterface* inter;       /**< Current GUI interface */
      picture* fig;              /**< The item figure */
      button* okButton;          /**< The ok Button */

};

#endif


