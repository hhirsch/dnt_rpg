#ifndef _itemwindow_h
#define _itemwindow_h

#include "../gui/farso.h"
#include "../classes/object.h"

/*! The item window is the one that shows objects infos to the user */
class itemWindow
{
   public:
      /*! Constructor
       * \param interf -> gui interface to open the window */
      itemWindow(interface* interf);

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

      interface* inter;          /**< Current GUI interface */
      picture* fig;              /**< The item figure */
      button* okButton;          /**< The ok Button */

};

#endif


