/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _guiInterface_h
#define _guiInterface_h

#include <string.h>
#include "window.h"
#include "mouse.h"
#include "menu.h"
#include "guilist.h"

/********************************************************************
 *                      Return's constants                          *
 ********************************************************************/
enum
{
   FARSO_EVENT_NONE,
   FARSO_EVENT_EXIT,
   FARSO_EVENT_MODIFIED_SEL_TEXT,
   FARSO_EVENT_SELECTED_MENU,
   FARSO_EVENT_PRESSED_BUTTON,
   FARSO_EVENT_NOT_PRESSED_BUTTON,
   FARSO_EVENT_WROTE_TEXT_BAR,
   FARSO_EVENT_MODIFIED_CX_SEL,
   FARSO_EVENT_CLOSED_WINDOW,
   FARSO_EVENT_ACTIVATED_WINDOW,
   FARSO_EVENT_MOVED_WINDOW,
   FARSO_EVENT_CLICKED_WINDOW,
   FARSO_EVENT_PRESSED_TAB_BUTTON,
   FARSO_EVENT_ON_PRESS_TAB_BUTTON,
   FARSO_EVENT_ON_PRESS_BUTTON,
   FARSO_EVENT_MODIFIED_MENU,
   FARSO_EVENT_SELECTED_SEL_TEXT,
   FARSO_EVENT_SELECTED_LIST_TEXT,
   FARSO_EVENT_ON_FOCUS_TAB_BUTTON,
   FARSO_EVENT_FILE_SEL_ACCEPT,
   FARSO_EVENT_FILE_SEL_CANCEL,
   FARSO_EVENT_FILE_SEL_CHANGED,
   FARSO_EVENT_TAB_BOX_CHANGED
};

/********************************************************************
 *                       Focus's constants                          *
 ********************************************************************/
enum
{
   FARSO_FOCUS_WINDOW,
   FARSO_FOCUS_TEXT_BAR,
   FARSO_FOCUS_BUTTON,
   FARSO_FOCUS_CX_SEL,
   FARSO_FOCUS_MENU,
   FARSO_FOCUS_SEL_TEXT,
   FARSO_FOCUS_WINDOW_MOVE,
   FARSO_FOCUS_WINDOW_MENU,
   FARSO_FOCUS_TAB_BUTTON,
   FARSO_FOCUS_GAME
};


/*! guiInterface is the GUI upper level class. Is from it that all GUI events
 * are treated, and all actions maybe be takken. */
class guiInterface
{
   public:
      /*! Constructor
       * \param arqFundo -> name of a image file to be the background. 
       *                    NULL if no background is used*/
      guiInterface(char* arqFundo);
      /*! Destructor  */
      ~guiInterface();

      /*! Manipulate Events on Gui.
       * \param x -> mouseX position
       * \param y -> mouseY position
       * \param Mbotao -> mous button state
       * \param tecla -> keyboard state
       * \param eventInfo -> variable that will receive the information 
       *                     on the event that occurs
       * \return -> pointer to the active objet of the action */
      guiObject* manipulateEvents(int x, int y, Uint8 Mbotao, Uint8* tecla,
                                  int& eventInfo);
      /*! Draw all the guiInterface (all windows and objects)
       * \param proj -> project view matrix
       * \param modl -> model view matrix
       * \param viewPort -> the viewport vector*/
      void draw(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);
      /*! Clear the internal pointer to the actived object. 
       *  Use it when close by force some window */
      void clearActiveObject();
      /*! Close a window
       * \param jan -> pointer to the window */
      void closeWindow(window *jan);
      /*! Close all guiInterface openned windows */
      void closeAllWindows();
      /*! Verify if the mouse is on some window or not
       * \param mouseX -> mouse x position
       * \param mouseY -> mouse y position
       * \return true if mouse is in some window, false otherwise. */
      bool mouseOnGui(int mouseX, int mouseY);
      /*! Insert a window on the guiInterface
       * \param xa -> x1 window position
       * \param ya -> y1 window position
       * \param xb -> x2 window posiiton 
       * \param yb -> y2 window position
       * \param text -> title of the window 
       * \return -> pointer to the inserted window */
      window* insertWindow(int xa,int ya,int xb,int yb,string text);
      /*! Open a Window
       * \param jan -> pointer to the window opened */
      void openWindow(window* jan);
      
      /*! Get the guiInterface focus
       * \return focus constant of the current focus*/
      int getFocus(){return(focus);};

      /*! Get active Window
       * \return -> pointer to the active Window */
      window* getActiveWindow();

      /*! Hide All Current Windows */
      void hideAll();

      /*! Show All Current Windows */
      void showAll();

   private:

      /*! Verify mouse press at objects
       * \param x -> mouse x coordinate
       * \param y -> mouse y coordinate
       * \param list -> pointer to the guiList to verify 
       * \note -> call this function after verify the mouse is pressed. */
      void verifyMousePressObjects(int x, int y, guiList* list);

      /*! Verify mouse in at objects
       * \param x -> mouse x coordinate
       * \param y -> mouse y coordinate
       * \param list -> pointer to the guiList to verify */
      void verifyMouseInObjects(int x, int y, guiList* list);
      
      /*! Verify mouse at tab boxes
       * \param x -> mouse x coordinate
       * \param y -> mouse y coordinate
       * \param list -> pointer to the guiList to verify
       * \return -> tabBox pointer if event got */
      guiObject* verifyTabBox(int x, int y, guiList* list);

      /*! Verify RolBars (after got button pressed events)
       * \param guiList -> list of objects to verify rolBars */
      void verifyRolBars(guiList* list);

      /*! Verify events occured on gui at single components
       * \param x -> current mouse x
       * \param y -> current mouse y
       * \param Mbotao -> current mouse button state
       * \param tecla -> current keyboard state
       * \param eventInfo -> return event information got
       * \return -> pointer tot he active object */
      guiObject* verifySingleEvents(int x, int y, Uint8 Mbotao, Uint8* tecla,
                                    int& eventInfo);

      /*! Verify events occureed on gui at composite components (those composed
       *  by more than one gui component)
       *  \param actObj -> active object got from verifySingleEvents 
       *  \param event info -> current event info from verifySingleEvents (will
       *  have the new one, if something occurs)
       *  \return -> new active object or the current one. */
      guiObject* verifyCompositeEvents(guiObject* actObj, int& eventInfo);

      /*! Verify events occureed on gui at file selectors (its an special case)
       *  \param actObj -> active object 
       *  \param event info -> current event
       *  \return -> new active object or the current one. */
      guiObject* verifyFileSelectorsEvents(guiObject* actObj, int& eventInfo);

      int focus;              /**< Current GUI focus */
      SDL_Surface* fundo;     /**< background surface, if has one */
      guiList* objects;       /**< some no window objects */
      windowList* ljan;       /**< windows list */
      guiObject* objAtivo;    /**< pointer to the actived object */
      guiObject* chamador;    /**< pointer to the caller object */

};



#endif

