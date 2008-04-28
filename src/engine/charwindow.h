#ifndef _charwindow_h
#define _charwindow_h

#include "../gui/farso.h"
#include "character.h"

/*! The character window displays information about a
 * character current states. */
class charWindow
{
   public:
      /*! Constructor
       * \param interf -> gui interface to open the window */
      charWindow(guiInterface* interf);

      /*! Destructor */
      ~charWindow();

      /*! Open the window with info about the character
       * \param pers -> character to show info */
      void open(character* pers);

      /*! Close the charWindow (if opened) */
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

      /*! Write at the infoBar about the weapon from the inventory place
       * \param inventoryPlace -> place where the weapon is (usually 
       *        INVENTORY_LEFT_HAND and INVENTORY_RIGHT_HAND)*/
      void writeAboutWeapon(int inventoryPlace);

      window* intWindow;         /**< Pointer to the internal window */
      object* curObject;         /**< Pointer to current object */

      character* current;        /**< Current character */

      guiInterface* inter;       /**< Current GUI interface */
      button* okButton;          /**< The ok Button */
      button* levelUpButton;     /**< The Level Up Button */
      rolBar* infoBar;           /**< The Info Bar */
};

#endif

