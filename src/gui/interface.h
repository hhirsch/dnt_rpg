/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _farso_interface_h
#define _farso_interface_h

#include "window.h"
#include "mouse.h"
#include "menu.h"
#include "guilist.h"

namespace Farso
{

/********************************************************************
 *                      Return's constants                          *
 ********************************************************************/
enum FarsoEventsEnum
{
   EVENT_NONE,
   EVENT_EXIT,
   EVENT_MODIFIED_SEL_TEXT,
   EVENT_SELECTED_MENU,
   EVENT_PRESSED_BUTTON,
   EVENT_NOT_PRESSED_BUTTON,
   EVENT_WROTE_TEXT_BAR,
   EVENT_MODIFYING_CX_SEL,
   EVENT_MODIFIED_CX_SEL,
   EVENT_CLOSED_WINDOW,
   EVENT_ACTIVATED_WINDOW,
   EVENT_MOVED_WINDOW,
   EVENT_CLICKED_WINDOW,
   EVENT_PRESSED_TAB_BUTTON,
   EVENT_RIGHT_PRESSED_TAB_BUTTON,
   EVENT_ON_PRESS_TAB_BUTTON,
   EVENT_ON_RIGHT_PRESS_TAB_BUTTON,
   EVENT_ON_PRESS_BUTTON,
   EVENT_MODIFIED_MENU,
   EVENT_SELECTED_SEL_TEXT,
   EVENT_SELECTED_LIST_TEXT,
   EVENT_ON_FOCUS_TAB_BUTTON,
   EVENT_FILE_SEL_ACCEPT,
   EVENT_FILE_SEL_CANCEL,
   EVENT_FILE_SEL_CHANGED,
   EVENT_TAB_BOX_CHANGED,
   EVENT_ON_PRESS_TEXT_BOX,
   EVENT_ON_PRESS_PICTURE,
   EVENT_ON_WRITE_TEXT_BAR
};

/********************************************************************
 *                       Focus's constants                          *
 ********************************************************************/
enum FarsoFocusEnum
{
   FOCUS_WINDOW,
   FOCUS_TEXT_BAR,
   FOCUS_BUTTON,
   FOCUS_CX_SEL,
   FOCUS_MENU,
   FOCUS_SEL_TEXT,
   FOCUS_WINDOW_MOVE,
   FOCUS_WINDOW_MENU,
   FOCUS_TAB_BUTTON,
   FOCUS_TEXT_BOX,
   FOCUS_PICTURE,
   FOCUS_GAME
};


/*! GuiInterface is the GUI upper level class. Is from it that all GUI events
 * are treated, and all actions maybe be takken. */
class GuiInterface
{
   public:
      /*! Constructor
       * \param backImage -> name of a image file to be the background. 
       *                    NULL if no background is used*/
      GuiInterface(std::string backImage="");
      /*! Destructor  */
      ~GuiInterface();

      /*! Manipulate Events on Gui.
       * \param x -> mouseX position
       * \param y -> mouseY position
       * \param Mbotao -> mous button state
       * \param tecla -> keyboard state
       * \param eventInfo -> variable that will receive the information 
       *                     on the event that occurs
       * \return -> pointer to the active objet of the action */
      GuiObject* manipulateEvents(int x, int y, Uint8 Mbotao, Uint8* tecla,
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
      void closeWindow(Window *jan);
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
       * \param empty -> with window is without any self draw 
       * \return -> pointer to the inserted window */
      Window* insertWindow(int xa,int ya,int xb,int yb,std::string text,
            bool empty=false);
      /*! Open a Window
       * \param jan -> pointer to the window opened */
      void openWindow(Window* jan);
      
      /*! Get the guiInterface focus
       * \return focus constant of the current focus*/
      int getFocus(){return(focus);};

      /*! Get active Window
       * \return -> pointer to the active Window */
      Window* getActiveWindow();

      /*! Set a window to be the active one
       * \param w -> window to activate */
      void setActiveWindow(Window* w);

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
      void verifyMousePressObjects(int x, int y, GuiList* list);

      /*! Verify mouse in at objects
       * \param x -> mouse x coordinate
       * \param y -> mouse y coordinate
       * \param list -> pointer to the guiList to verify */
      void verifyMouseInObjects(int x, int y, GuiList* list);
      
      /*! Verify mouse at tab boxes
       * \param x -> mouse x coordinate
       * \param y -> mouse y coordinate
       * \param list -> pointer to the guiList to verify
       * \return -> tabBox pointer if event got */
      GuiObject* verifyTabBox(int x, int y, GuiList* list);

      /*! Verify RolBars (after got button pressed events)
       * \param x -> mouse x coordinate
       * \param y -> mouse y coordinate
       * \param guiList -> list of objects to verify rolBars */
      bool verifyRolBars(int x, int y, GuiList* list);

      /*! Verify events occured on gui at single components
       * \param x -> current mouse x
       * \param y -> current mouse y
       * \param Mbotao -> current mouse button state
       * \param tecla -> current keyboard state
       * \param eventInfo -> return event information got
       * \return -> pointer tot he active object */
      GuiObject* verifySingleEvents(int x, int y, Uint8 Mbotao, Uint8* tecla,
                                    int& eventInfo);

      /*! Verify events occureed on gui at composite components (those composed
       *  by more than one gui component)
       *  \param actObj -> active object got from verifySingleEvents 
       *  \param event info -> current event info from verifySingleEvents (will
       *  have the new one, if something occurs)
       *  \return -> new active object or the current one. */
      GuiObject* verifyCompositeEvents(GuiObject* actObj, int& eventInfo);

      /*! Verify events occureed on gui at file selectors (its an special case)
       *  \param actObj -> active object 
       *  \param event info -> current event
       *  \return -> new active object or the current one. */
      GuiObject* verifyFileSelectorsEvents(GuiObject* actObj, int& eventInfo);

      int focus;                /**< Current GUI focus */
      SDL_Surface* background;  /**< background surface, if has one */
      GuiList* objects;         /**< some no window objects */
      WindowList* lwindows;     /**< windows list */
      GuiObject* activeObject;  /**< pointer to the actived object */
      GuiObject* callerObject;  /**< pointer to the caller object */
      MouseCursor mouseCursor;  /**< current mouse cursor */

};

}

#endif

