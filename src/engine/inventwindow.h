#ifndef _invent_window
#define _invent_window

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "../classes/inventory.h"
#include "util.h"

/*! Inventory Window Class */
class inventWindow
{
   public:
      /*! Constructor
       * \param invent -> inventories to show
       * \param inter -> GUI interface to use */
      inventWindow(inventory *invent[INVENTORY_PER_CHARACTER],interface* inter);

      /*! Destructor */
      ~inventWindow();

      /*! Treat Events on Window. 
       * \param activeObject -> pointer to the active object pointer
       * \param guiObject -> last GUI object
       * \param eventInfo -> last GUI Event */
      void treat(object** activeObject, Tobjeto* guiObject, int eventInfo);

      /*! Verify if window is still opened
       * \return true if window is openned, false otherwise */
      bool isOpen();

      /*! Redraw the Inventory Window, to actualize things on it. */
      void reDraw();

   private:
      inventory *inventories[INVENTORY_PER_CHARACTER]; /**< Extern Inventories*/

      interface* interf;              /**< Pointer to Interface */
      janela* window;                 /**< Internal Window */
      tabButton* inventoryTabButton;  /**< Inventory Tab Button */
      oneTabButton* inventoryButton;
      oneTabButton* inv1Button;
      oneTabButton* inv2Button;
      oneTabButton* inv3Button;
      oneTabButton* inv4Button;
      
      tabButton* characterTabButton;  /**< Character Tab Button */
      oneTabButton* headButton;
      oneTabButton* leftHandButton;
      oneTabButton* rightHandButton;
      oneTabButton* leftFingerButton;
      oneTabButton* rightFingerButton;
      oneTabButton* neckButton;
      oneTabButton* footButton;
      oneTabButton* bodyButton;
      
};

#endif

