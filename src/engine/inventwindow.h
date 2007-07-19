#ifndef _invent_window
#define _invent_window

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "../classes/inventory.h"
#include "../lang/lang.h"
#include "cursor.h"
#include "util.h"

#define INVENTORY_ACTION_NONE           0
#define INVENTORY_ACTION_INTERNAL       1
#define INVENTORY_ACTION_EQUIPED_WEAPON 2
#define INVENTORY_ACTION_EQUIPED_ARMOR  3
#define INVENTORY_ACTION_USE_ITEM       4

/*! Inventory Window Class */
class inventWindow
{
   public:
      /*! Constructor
       * \param invent -> inventories to show
       * \param inter -> GUI interface to use */
      inventWindow(inventory *invent,interface* inter);

      /*! Destructor */
      ~inventWindow();

      /*! Treat Events on Window. 
       * \param guiObj -> last GUI object
       * \param eventInfo -> last GUI Event 
       * \param mouseCursor -> the mouse cursor
       * \return true if event is threated, false otherwise. */
      bool treat(guiObject* guiObj, int eventInfo, cursor* mouseCursor);

      /*! Verify if window is still opened
       * \return true if window is openned, false otherwise */
      bool isOpen();

      /*! Redraw the Inventory Window, to actualize things on it. */
      void reDraw();

   private:

      /*! Open the Inventory Object Menu  */
      void openMenu(int x, int y);

      inventory *inventories;         /**< Extern Inventories*/

      interface* interf;              /**< Pointer to Interface */
      window* intWindow;              /**< Internal Window */
      tabButton* inventoryTabButton;  /**< Inventory Tab Button */
      oneTabButton* inventoryButton;  /**< Inventory Spaces Button */
      oneTabButton* inv1Button;       /**< First Inventory Button */
      oneTabButton* inv2Button;       /**< Second Inventory Button */
      oneTabButton* inv3Button;       /**< Third Inventory Button */
      oneTabButton* inv4Button;       /**< Forth Inventory Button */
      
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
      int objWhere;                   /**< Where The Object is on Inventory */
      int objX;                       /**< X of the object in inventory */
      int objY;                       /**< Y of the object in inventory */

      object* activeObject;           /**< Active Object */

      SDL_Surface* previousCursor;    /**< The previous mouse Cursor */

      lang language;                  /**< Language internationalization */
      
};

#endif

