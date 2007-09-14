#ifndef _barter_window_h
#define _barter_window_h

#include <libintl.h>
#include "../gui/farso.h"
#include "../classes/barter.h"
#include "inventwindow.h"

/*! The Barter Window Class. In the barter window, objects to sell and buy 
 * are inserted, and you can offer the barter, impose the barter or cancel. */
class barterWindow
{
   public:
      /*! Constructor
       * \param inventSeller -> pointer to the seller inventory
       * \param inventBuyer -> pointer to the buyer inventory 
       * \param inter -> GUI interface used */
      barterWindow(inventory *inventSeller, inventory* inventBuyer,
                   interface* inter);

      /*! Destructor */
      ~barterWindow();

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

      int curSellSlot;           /**< Index of the actual sell slot */
      int curBuySlot;            /**< Index of the actual buy slot */

      barter* barterInventory;   /**< The barter inventory */
      interface* gui;            /**< The used GUI */
      textBox* sellerTotals;     /**< The Seller Totals */
      textBox* buyerTotals;      /**< The Buyer Totals */
      window* intWindow;         /**< The window used */
      button* imposeButton;      /**< The Impose Button */
      button* offerButton;       /**< The Offer Button */
      button* cancelButton;      /**< The Cancel Button */
      tabButton* barterTabButton;/**< The Barter TabButton */
      oneTabButton* seller1;     /**< The first inventory */
      oneTabButton* seller2;     /**< The second inventory */
      oneTabButton* seller3;     /**< The third inventory */
      oneTabButton* sellerInv;   /**< The actual Inventory */
      inventWindow* sellerWindow;/**< The pointer to the seller inventory 
                                      window, if it is openned here. */
      oneTabButton* buyer1;      /**< The first inventory */
      oneTabButton* buyer2;      /**< The second inventory */
      oneTabButton* buyer3;      /**< The third inventory */
      oneTabButton* buyerInv;    /**< The actual Inventory */
      inventWindow* buyerWindow; /**< Pointer to the buyer inventory window,
                                      if it is opened here. */
};



#endif

