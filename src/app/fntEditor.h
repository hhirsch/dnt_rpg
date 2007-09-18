#ifndef _fntEditor_h
#define _fntEditor_h

#include <stdlib.h>
#include <SDL/SDL.h>
#include "../gui/farso.h"
#include "../engine/culling.h"
#include "../engine/util.h"

/*! The font editor class */
class fntEditor
{
   public:
      /*! Constructor */
      fntEditor();
      /*! Destructor */
      ~fntEditor();

      /*! Run the Editor */
      void run();

   protected:
      /*! Clear the active font (aka -> do a new one) */
      void clear();
      /*! Put the actual letter to the actual font */
      void putLetter();
      /*! Get a letter to edit
       * \param n -> character code */
      void getLetter(int n);
      /*! Define the inccp of the actual font, based on the one 
       * at the text bar */
      void defCP();
      /*! Open a new font */
      void open();
      /*! Save the actual Font */
      void save();
      /*! Put the active color to the position
       * \param x -> mouse X coordinate
       * \param y -> mouse Y coordinate */
      void putColor(int x, int y);
      /*! Draw the grid */
      void drawGrid();
      /*! Define Character
       * \param keys -> keyboard state
       * \param c -> define character */
      void defineCharacter(Uint8* keys, char* c);


      GLdouble proj[16];           /**< Projection Matrix */
      GLdouble modl[16];           /**< ModelView Matrix */
      GLint viewPort[4];           /**< ViewPort Vector */
      GLfloat visibleMatrix[6][4]; /**< VisibleMatrix */
      SDL_Surface *screen;         /**< The SDL main Screen */

      interface* interf;        /**< The GUI interface */
      farso_colors color;       /**< Internal Colors */

      char character[16][16];   /**< Actual editing character */
      fnt actualFont;           /**< On edition font */
      int actualColor;          /**< Active Color if != 0. Inactive if 0 */
      int actualCharacter;      /**< Actual Opened Character code */
      bool done;                /**< Main Flux Controller */

      window *mainWindow;       /**< Main Window */
      textBar* textFileName;    /**< Text bar with for the fileName */
      textBar* textCP;          /**< TextBar with actual incCP */
      textBar* textGoto;        /**< TextBar with the character to goto */
      button *newButton,        /**< The NEW button */
             *openButton,       /**< The OPEN button */
             *saveButton,       /**< The SAVE button */
             *exitButton,       /**< The EXIT button */
             *activeButton,     /**< The ACTIVE COLOR button */
             *inactiveButton,   /**< The INACTIVE COLOR button */
             *gotoButton;       /**< The GOTO button */

      window *editWindow;       /**< Edit Window  */
      picture* grid;            /**< Editing Grid */

};


#endif

