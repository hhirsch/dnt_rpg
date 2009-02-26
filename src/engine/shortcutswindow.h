#ifndef _dnt_shortcutswindow_h
#define _dnt_shortcutswindow_h

#include "../gui/farso.h"

#define SHORTCUTS_WINDOW_NONE       -1
#define SHORTCUTS_WINDOW_OTHER       0
#define SHORTCUTS_WINDOW_MENU        1
#define SHORTCUTS_WINDOW_LOAD        2
#define SHORTCUTS_WINDOW_SAVE        3
#define SHORTCUTS_WINDOW_ATTACK_MODE 4
#define SHORTCUTS_WINDOW_JOURNAL     5
#define SHORTCUTS_WINDOW_MAP         6
#define SHORTCUTS_WINDOW_INVENTORY   7
#define SHORTCUTS_WINDOW_ASSIGN      8
#define SHORTCUTS_WINDOW_GROUP       9
#define SHORTCUTS_WINDOW_CHARACTER  10
#define SHORTCUTS_WINDOW_END_TURN   11

/*! The shortcuts window, if quick access buttons and some
 * text infos. */
class shortcutsWindow
{
   public:
      /*! Constructor */
      shortcutsWindow();
      /*! Destructor */
      ~shortcutsWindow();

      /*! Open the window 
       * \param gui -> interface used */
      void open(guiInterface* gui);
      /*! Close the window 
       * \param gui -> interface used */
      void close(guiInterface* gui);
      /*! Verify if the window is opened
       * \return -> true if the window is opened */
      bool isOpened();

      /*! Treat the window events
       * \param object -> current active gui object
       * \param eventInfo -> current event 
       * \param engineMode -> current engine mode
       * \return -> shortcutsWindow return value constant */
      int treat(guiObject* object, int eventInfo, int engineMode);

      /*! Set the thing bellow mouse
       * \param thingName -> the name of the thing */
      void setThing(string thingName);
      /*! Set the total number of particles active
       * \param total -> number of particles */
      void setParticlesNumber(int total);
      /*! Set the current FPS value
       * \param fps -> current FPS value */
      void setFPS(float fps);
      /*! Set the hour text
       * \param hour -> new hour text */
      void setHour(string hour);

   protected:
      static textBox* fpsTxt;                /**< shows FPS */
      static textBox* partTxt;               /**< shows Particles count */
      static textBox* thingTxt;              /**< shows thing at mouse */
      static textBox* hourTxt;               /**< shows Hour */

      static button* buttonMenu;             /**< call Menu */
      static button* buttonSave;             /**< call Save Function */
      static button* buttonLoad;             /**< call Load Function */
      static oneTabButton* buttonAttackMode; /**< Enter on Attack Mode */
      static oneTabButton* buttonJournal;    /**< Open Journal Window */
      static oneTabButton* buttonMap;        /**< Call MiniMap */
      static oneTabButton* buttonInventory;  /**< Call Inventory */
      static oneTabButton* buttonAssign;     /**< assign attack */
      static oneTabButton* buttonGroup;      /**< open group window */
      static oneTabButton* buttonCharacter;  /**< open Character Window */
      static oneTabButton* buttonEndTurn;    /**< End Turn Button */

      static window* shortCutsWindow;        /**< ShortCuts Window */
};

#endif

