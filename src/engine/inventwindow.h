#ifndef _invent_window
#define _invent_window

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "../classes/inventory.h"
#include "../lang/lang.h"
#include "util.h"

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
       * \return true if event is threated, false otherwise. */
      bool treat(guiObject* guiObj, int eventInfo);

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
      janela* window;                 /**< Internal Window */
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

      lang language;                  /**< Language internationalization */
      
};

#endif

