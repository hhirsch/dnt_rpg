#ifndef _farso_messages_h
#define _farso_messages_h

#include "farso.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;

/*! The quantity windows is used to get a number inner a range 
 * from the user */
class quantityWindow
{
   public:
      /*! Set the current range of the integer to get
       * \param min -> min value
       * \param max -> max value */
      void setRange(float min, float max);

      /*! Get the current value 
       * \return -> current value */
      float getValue();

      /*! Set the current value
       * \param val -> current value */
      void setValue(float val);

      /*! Set the delta variation when using button 
       * \param val -> new delta */
      void setDelta(float val);

      /*! Show the window to user
       * \param title -> title of the window
       * \param message -> message to show to user
       * \param gui -> pointer to the current interface */
      void show(string title, string message, guiInterface* gui);

      /*! Treat the events
       * \param actObj -> current active object 
       * \param eventInfo -> current event got */
      bool verifyEvents(guiObject* actObj, int eventInfo);

      /*! Verify if the window is opened
       * \return -> true, if opened, false otherwise */
      bool isOpened();

      /*! Close the window */
      void close();

   protected:
      /*! Update the current text  */
      void updateText();

      static window* qtyWindow;         /**< The quantity window */
      static guiInterface* lastGui;     /**< The guiInterface in use */
      static textBox* qtyText;          /**< The quantity text */
      static button* moreButton;        /**< The more button */
      static button* lessButton;        /**< The less button */
      static button* okButton;          /**< The Ok Button */
      static float qty;                 /**< The current value */
      static float delta;               /**< The current delta */
      static float min;                 /**< The current min */
      static float max;                 /**< The current max */
};

/*! The warning class is used to show messages to the user */
class warning
{
   public:
      /*! Show a message to user
       * \param title -> title of the window
       * \param message -> message to show to user
       * \param gui -> pointer to the current interface */
      void show(string title, string message, guiInterface* gui);

      /*! Get the okButton pointer
       * \return -> pointer to the okButton */
      button* getOkButton();

      /*! Verify if the warnWindow is opened
       * \return -> true, if opened, false otherwise */
      bool isOpened();

      /*! Close the warning window */
      void close();

   protected:
      static window* warnWindow;         /**< The warning window */
      static guiInterface* lastGui;      /**< The guiInterface in use */
      static button* okButton;           /**< The Ok Button */
};

#endif

