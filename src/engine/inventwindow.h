/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
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

#ifndef _dnt_invent_window
#define _dnt_invent_window

#include <libintl.h>
#include "../classes/inventory.h"
#include "../map/map.h"
#include "character.h"
#include "cursor.h"
#include "modstate.h"
#include "util.h"
#include "itemwindow.h"

#define INVENTORY_ACTION_NONE           0
#define INVENTORY_ACTION_INTERNAL       1
#define INVENTORY_ACTION_EQUIPED        2
#define INVENTORY_ACTION_USE_ITEM       4
#define INVENTORY_ACTION_REMOVE_ITEM    5
#define INVENTORY_ACTION_CHANGE_ITEM    6

#define MENU_TYPE_INVENTORY             1
#define MENU_TYPE_EQUIPED               2

/*! Inventory Window Class */
class inventWindow
{
   public:
      /*! Constructor
       * \param invent -> pointer to the character whose inventories will show
       * \param inter -> GUI interface to use 
       * \param itemWindow -> pointer to the info window used */
      inventWindow(character *invent, guiInterface* inter, itemWindow* infoW);

      /*! Constructor
       * \param xa -> position X to open the window
       * \param ya -> y position to open the window
       * \param title -> title of the inventory window
       * \param invent -> pointer to the character whose inventories will show
       * \param inter -> GUI interface to use 
       * \param itemWindow -> pointer to the info window used */
      inventWindow(int xa, int ya, string title, 
                   character *invent, guiInterface* inter,
                   itemWindow* infoW);

      /*! Destructor */
      ~inventWindow();

      /*! Treat Events on Window. 
       * \param guiObj -> last GUI object
       * \param eventInfo -> last GUI Event 
       * \param mouseCursor -> the mouse cursor
       * \param actualMap -> pointer to the opened map
       * \param X -> character that ownes the inventory X position
       * \param Z ->     "       "    "    "      "     Z    "  
       * \param seller -> if is the seller inventory
       * \return actionCode if event is threated, 0 otherwise. */
      int treat(guiObject* guiObj, int eventInfo, cursor* mouseCursor,
                 Map* actualMap, GLfloat X, GLfloat Z, bool seller=false);

      /*! Verify if window is still opened
       * \return true if window is openned, false otherwise */
      bool isOpen();

      /*! Redraw the Inventory Window, to actualize things on it. */
      void reDraw();

   private:

      /*! Init the window */
      void init(int xa, int ya, string title, character *invent,
                guiInterface* inter, itemWindow* infoW);

      /*! Open the Inventory Object Menu  */
      void openMenu(int x, int y, int type, bool seller);

      /*! Do an use object action, if possible */
      void verifyUseObject();

      itemWindow* infoWindow;         /**< The item window to show object's */

      inventory* inventories;         /**< Extern Inventories*/
      character* owner;               /**< The inventories owner */

      guiInterface* interf;              /**< Pointer to Interface */
      window* intWindow;                 /**< Internal Window */
      tabButton* inventoryTabButton[4];  /**< Inventory Tab Button */
      oneTabButton* inventoryButton[4];  /**< Button to acces inventory */
      tabBox* inventoryTabBox;           /**< The Inventory selector */
      
      tabButton* characterTabButton;  /**< Character Tab Button */
      oneTabButton* headButton;       /**< Head Button */
      oneTabButton* leftHandButton;   /**< Left Hand Button */
      oneTabButton* rightHandButton;  /**< Right Hand Button */
      oneTabButton* leftFingerButton; /**< Left Finger Button */
      oneTabButton* rightFingerButton;/**< Right Finger Button */
      oneTabButton* neckButton;       /**< Neck Button */
      oneTabButton* footButton;       /**< Foot Button */
      oneTabButton* bodyButton;       /**< Body (Torso) Button */

      int state;                      /**< Internal State of the Window */
      int currentInventory;           /**< Current Opened Inventory */
      menu* objectMenu;               /**< Pointer to Object Actions Menu */
      int menuType;                   /**< Type of the menu opened */
      int objWhere;                   /**< Where The Object is on Inventory */
      int objX;                       /**< X of the object in inventory */
      int objY;                       /**< Y of the object in inventory */

      object* activeObject;           /**< Active Object */

      int previousCursor;             /**< The previous mouse Cursor */

};

#endif

