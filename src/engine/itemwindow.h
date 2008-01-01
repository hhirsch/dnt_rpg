#ifndef _itemindow_h
#define _itemwindow_h

#include "../gui/farso.h"
#include "../classes/object.h"

/*! The item window is the one that shows objects infos to the user */
class itemWindow
{
   public:
      /*! Constructor
       * \param item -> objct to show info
       * \param interf -> gui interface to open the window */
      itemWindow(object* item, interface* interf);

      /*! Destructor */
      ~itemWindow();

      /*! Treat Events on Window. 
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return != 0 if event is gathered */
      int treat(guiObject* object, int eventInfo);

      window* intWindow;         /**< Pointer to the internal window */

   protected:
      object* curObject;         /**< Pointer to current object */

      interface* inter;          /**< Current GUI interface */
      picture* fig;              /**< The item figure */
      button* okButton;          /**< The ok Button */

};

#endif


