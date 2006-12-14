/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#ifndef _initial_h
#define _initial_h

#include "../gui/farso.h"
#include "../lang/lang.h"
#include "../sound/sound.h"

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

     /*! Execute the initialScreen.
      * \param Status -> game status when called screen 
      *                  ( \c ON_INIT or \c IN_GAME )
      * \param proj -> projection Matrix
      * \param modl -> modelView matrix
      * \param viewPort -> current view Port
      * \param idTextura -> initial screen texture id (will be loaded).
      * \param snd -> sound engine to use
      * \return initialScreen Return values correspondent to action made */
     int Execute(int Status, GLdouble proj[16],
                           GLdouble modl[16], GLint viewPort[4],
                           GLuint* idTextura, sound* snd);
   private:
      interface* gui;  /**< GUI for Windows */
      lang language;   /**< language internationalization */

      int result;      /**< Result of the initial Screen */
      bool done;
      
      botao* buttonNew;
      botao* buttonContinue;
      botao* buttonSave;
      botao* buttonLoad;
      botao* buttonOptions;
      botao* buttonExit;
};

#endif

