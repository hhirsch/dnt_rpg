/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _interface_h
#define _interface_h

#include <string.h>
#include "window.h"
#include "mouse.h"
#include "menu.h"
#include "guilist.h"

/********************************************************************
 *                      Return's constants                          *
 ********************************************************************/
#define NOTHING             -1
#define EXIT                 0
#define MODIFIED_SEL_TEXT    1
#define SELECTED_MENU        2
#define PRESSED_BUTTON       3
#define NOT_PRESSED_BUTTON   4
#define WROTE_TEXT_BAR       5
#define MODIFIED_CX_SEL      6
#define CLOSED_WINDOW        7
#define ACTIVATED_WINDOW     8
#define MOVED_WINDOW         9
#define CLICKED_WINDOW      10
#define PRESSED_TAB_BUTTON  11
#define ON_PRESS_TAB_BUTTON 12
#define ON_PRESS_BUTTON     13
#define MODIFIED_MENU       14
#define SELECTED_SEL_TEXT   15
#define SELECTED_LIST_TEXT  16
#define ON_FOCUS_TAB_BUTTON 17
#define SELECTED_FILE       18

/********************************************************************
 *                       Focus's constants                          *
 ********************************************************************/
#define FOCUS_WINDOW         1
#define FOCUS_TEXT_BAR       2
#define FOCUS_BUTTON         3
#define FOCUS_CX_SEL         4
#define FOCUS_MENU           5
#define FOCUS_SEL_TEXT       6
#define FOCUS_WINDOW_MOVE    7
#define FOCUS_WINDOW_MENU    8
#define FOCUS_TAB_BUTTON     9
#define FOCUS_GAME         100


/*! Interface is the GUI upper level class. Is from it that all GUI events
 * are treated, and all actions maybe be takken. */
class interface
{
   public:
      /*! Constructor
       * \param arqFundo -> name of a image file to be the background. 
       *                    NULL if no background is used*/
      interface(char* arqFundo);
      /*! Destructor  */
      ~interface();

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
      /*! Draw all the interface (all windows and objects)
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
      /*! Close all interface openned windows */
      void closeAllWindows();
      /*! Verify if the mouse is on some window or not
       * \param mouseX -> mouse x position
       * \param mouseY -> mouse y position
       * \return true if mouse is in some window, false otherwise. */
      bool mouseOnGui(int mouseX, int mouseY);
      /*! Insert a window on the interface
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
      
      /*! Get the interface focus
       * \return focus constant of the current focus*/
      int getFocus(){return(focus);};

      /*! Get active Window
       * \return -> pointer to the active Window */
      window* getActiveWindow();

   private:

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

      int focus;              /**< Current GUI focus */
      SDL_Surface* fundo;     /**< background surface, if has one */
      guiList* objects;       /**< some no window objects */
      windowList* ljan;       /**< windows list */
      guiObject* objAtivo;    /**< pointer to the actived object */
      guiObject* chamador;    /**< pointer to the caller object */

};



#endif

