#ifndef _cursor_h
#define _cursor_h

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <string>
using namespace std;

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#define CURSOR_WALK         0 /**< Walk Mouse Cursor */
#define CURSOR_ATTACK       1 /**< Attack Mouse Cursor */
#define CURSOR_DEFEND       2 /**< Defend Mouse Cursor */
#define CURSOR_MAPTRAVEL    3 /**< Map Travel Mouse Cursor */
#define CURSOR_TALK         4 /**< Talk Mouse Cursor */
#define CURSOR_GET          5 /**< Get Mouse Cursor */
#define CURSOR_INVENTORY    6 /**< Inventory Mouse Cursor */
#define CURSOR_DOOR         7 /**< Door Mouse Cursor */
#define CURSOR_FORBIDDEN    8 /**< Forbidden Mouse Cursor */
#define CURSOR_WALK_CONT    9 /**< Continuous Walk Cursor */
#define CURSOR_USE         10 /**< Object Use */
#define CURSOR_USER_IMAGE  11 /**< Any user image seted as cursor */
#define CURSOR_TOTAL       12 /**< Total number of mouse cursors */

/*! A mouse cursor class.*/
class cursor
{
   public:
      /*! cursor Constructor */
      cursor();
      /*! cursor Destructor */
      ~cursor();

      /*! init the cursor use 
       * \note musr be called at engine init */
      void init();
      /*! Finish the cursor use
       * \note must be called at engine end*/
      void finish();

      /*! Set current mouse Cursor 
       * \param nCursor -> cursor Number to use.*/
      void set(int nCursor);

      /*! Set the current mouse Cursor to an image
       * \param img -> sdl surface to set as cursor */
      void set(SDL_Surface* img);

      /*! Draw th Cursor to screen */
      void draw(int mouseX, int mouseY, float angle=0.0);

      /*! Gets the actual mouse cursor
       * \return -> pointer to the surface of the mouse cursor */
      int get();
      
   private:
      static GLuint texture[CURSOR_TOTAL]; /**< Cursors Textures */
      static float sizeX[CURSOR_TOTAL];    /**< Cursors Widths */
      static float sizeY[CURSOR_TOTAL];    /**< Cursors Heights */
      static float propX[CURSOR_TOTAL];    /**< X Proportion */
      static float propY[CURSOR_TOTAL];    /**< Y Proportion */
      static float hotSpot[CURSOR_TOTAL][2]; /**< HotSpot */
      static int currentCursor;            /**< Current Cursor Index */
      
      /*! Load Cursor file 
       * \param fileName -> file name of cursor
       * \param index -> internal cursor index */
      void loadCursor(string fileName, int index);

      
};

#endif

