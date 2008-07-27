#ifndef _barter_window_h
#define _barter_window_h

#include <libintl.h>
#include "../gui/farso.h"
#include "../classes/barter.h"
#include "inventwindow.h"
#include "itemwindow.h"

/*! The Barter Window Class. In the barter window, objects to sell and buy 
 * are inserted, and you can offer the barter, impose the barter or cancel. */
class barterWindow
{
   public:
      /*! Constructor */
      barterWindow();

      /*! Destructor */
      ~barterWindow();

      /*! Open the barter window 
       * \param seller -> pointer to the seller character
       * \param buyer -> pointer to the buyer character 
       * \param inter -> GUI interface used */
      void open(character *seller, character* buyer,
                guiInterface* inter, itemWindow* infoW);

      /*! Close / Remove structures from the barter window */
      void close();

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
      /*! Cancel the barter */
      void cancel();

      /*! Offer the Barter
       * \return -> true if is accepted */
      bool offer();

      /*! Impose the Barter
       * \return -> true if the imposition is accepted */
      bool impose();

      static int curSellSlot;           /**< Index of the actual sell slot */
      static int curBuySlot;            /**< Index of the actual buy slot */

      static barter* barterInventory;   /**< The barter inventory */
      static guiInterface* gui;         /**< The used GUI */
      static textBox* sellerTotals;     /**< The Seller Totals */
      static textBox* buyerTotals;      /**< The Buyer Totals */
      static window* intWindow;         /**< The window used */
      static button* imposeButton;      /**< The Impose Button */
      static button* offerButton;       /**< The Offer Button */
      static button* cancelButton;      /**< The Cancel Button */
      static tabButton* barterTabButton;/**< The Barter TabButton */
      static oneTabButton* seller1;     /**< The first inventory */
      static oneTabButton* seller2;     /**< The second inventory */
      static oneTabButton* seller3;     /**< The third inventory */
      static oneTabButton* sellerInv;   /**< The actual Inventory */
      static inventWindow* sellerWindow;/**< Pointer to the seller inventory
                                             window, if it is openned here. */
      static oneTabButton* buyer1;      /**< The first inventory */
      static oneTabButton* buyer2;      /**< The second inventory */
      static oneTabButton* buyer3;      /**< The third inventory */
      static oneTabButton* buyerInv;    /**< The actual Inventory */
      static inventWindow* buyerWindow; /**< Pointer to the buyer inventory 
                                             window, if it is opened here. */

      static itemWindow* infoWindow;    /**< Pointer to the item window used */
};



#endif

