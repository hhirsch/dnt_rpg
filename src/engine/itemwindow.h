/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
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
      itemWindow(Farso::GuiInterface* interf);

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
      int treat(Farso::GuiObject* object, int eventInfo);

      /*! Verify if the window is openned
       * \return true if opened, false otherwise. */
      bool isOpen();

   protected:
      Farso::Window* intWindow;  /**< Pointer to the internal window */
      object* curObject;         /**< Pointer to current object */

      Farso::GuiInterface* inter;/**< Current GUI interface */
      Farso::Picture* fig;       /**< The item figure */
      Farso::Button* okButton;   /**< The ok Button */

};

#endif


