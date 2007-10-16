#ifndef _grass_window_h
#define _grass_window_h

#include "../../particle/grass.h"
#include "../../gui/farso.h"

/*! The Grass Controller Window */
class grassWindow
{
   public:
      /*! Constructor
       * \param actualGui -> the actual GUI interface*/
      grassWindow(interface* actualGui);
      /*! Destructor */
      ~grassWindow();

      /*! Set the current grass (and open the window, if it is closed)
       * \param gr -> grass to show params. */
      void setGrass(grass* gr);

      /*! Draw the temporary things. */
      void drawTemporary();

      /*! Verify if some event is related to the window
       * \param type -> Type of the occurred event
       * \param object -> object of the event 
       * \return -> true if event is related (and treated) by the window */
      bool eventGot(int type, guiObject* object);
   protected:
      /*! Open the Window */
      void openWindow();
      
      /*! Define all the values on the window */
      void defineValues();

      grass* actualGrass;     /**< the actual on edition grass */
      interface* gui;         /**< the actual used GUI */
      
      window* actualWindow;         /**< the actual opened window pointer */
      textBox* textPovoation;       /**< the povoation text */
      textBox* textSize;            /**< The size text */
      button* buttonSizeMore;       /**< More size button */
      button* buttonSizeLess;       /**< Less size button */
      button* buttonPovoationMore;  /**< More povoation button */
      button* buttonPovoationLess;  /**< Less povoation button */
      tabButton* navigationTab;     /**< The navigation Table */
      oneTabButton* buttonNext;     /**< Next Button */
      oneTabButton* buttonPrevious; /**< Previous Button */
      oneTabButton* buttonDestroy;  /**< Destroy Button */
};

#endif

