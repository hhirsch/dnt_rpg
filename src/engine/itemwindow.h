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
       * \param inter -> gui interface to open the window */
      itemWindow(object* item, interface* inter);

      /*! Destructor */
      ~itemWindow();

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return 0 on close */
      int treat(guiObject* object, int eventInfo, interface* inter);

      window* intWindow;         /**< Pointer to the internal window */

   protected:
      object* curObject;         /**< Pointer to current object */

      button* okButton;          /**< The ok Button */

};

#endif


