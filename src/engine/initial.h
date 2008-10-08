/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#ifndef _initial_h
#define _initial_h

#include "../gui/farso.h"
#include "../sound/sound.h"
#include "cursor.h"

/* initialScreen Returns Values */
#define EXIT_GAME     0   /**< Exit Game Return Value */
#define NEW_GAME      1   /**< New Game Return Value */
#define LOAD_GAME     2   /**< Load Game Return Value */
#define SAVE_GAME     3   /**< Save Return Value */
#define CONTINUE_GAME 4   /**< Continue Return Value */
#define OPTIONS       5   /**< Number of Options */

/* initialScreen Status */
#define ON_INIT 1         /**< When Call Class On Game Init */
#define IN_GAME 2         /**< When Call Class in game execution */

/*! DccNiTghtmare's initial screen */
class initialScreen
{
   public:
     /*! initialScreen Constructor */
     initialScreen();
     /*! initialScreen Destructor */
     ~initialScreen();

     /*! Run the initialScreen.
      * \param Status -> game status when called screen 
      *                  ( \c ON_INIT or \c IN_GAME )
      * \param proj -> projection Matrix
      * \param modl -> modelView matrix
      * \param viewPort -> current view Port
      * \param idTextura -> loaded initial screen texture id.
      * \param snd -> sound engine to use
      * \param interf -> guiInterface to use
      * \return initialScreen Return values correspondent to action made */
     int run(int Status, GLdouble proj[16],
             GLdouble modl[16], GLint viewPort[4],
             GLuint idTextura, sound* snd,
             guiInterface* interf);
   private:
      guiInterface* gui; /**< GUI for Windows */

      int result;        /**< Result of the initial Screen */
      bool done;
      
      button* buttonNew;
      button* buttonContinue;
      button* buttonSave;
      button* buttonLoad;
      button* buttonOptions;
      button* buttonExit;
};

#endif

