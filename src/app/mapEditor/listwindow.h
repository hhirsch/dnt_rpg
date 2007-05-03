#ifndef _listwindow_h
#define _listwindow_h

#include "../../gui/farso.h"

/* NPCs */
#define STATE_CHARACTERS   1

/* Objects */
#define STATE_BUILDING    10
#define STATE_CARS        11 
#define STATE_COMMON      12
#define STATE_DOORS       13       
#define STATE_GUNS        14
#define STATE_ICEX        15
#define STATE_NATURE      16

/* Particles */
#define STATE_FIRE        30
#define STATE_GRASS       31
#define STATE_SMOKE       32
#define STATE_SNOW        33
#define STATE_WATERFALL   34

/*! The list Window, for selecting objects */
class listWindow
{
   public:
      /*! Constructor
       * \param gui -> pointer to the interface used */
      listWindow(interface* gui);
      /*! Destructor */
      ~listWindow();

      /*! Set State
       * \param st -> state value */
      void setState(int st);

   private:
      /*! Open the window */
      void open();

      /*! Load list from file */
      void loadFromFile(string fileName);

      int state;              /**< Internal State */
      janela* intWindow;      /**< Pointer to the window */
      interface* intGui;      /**< Pointer to the interface */
      listText* list;         /**< Pointer to the listText */

};

#endif

