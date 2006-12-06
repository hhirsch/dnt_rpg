#ifndef _cursor_h
#define _cursor_h

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define CURSOR_WALK      0 /**< Walk Mouse Cursor */
#define CURSOR_ATTACK    1 /**< Attack Mouse Cursor */
#define CURSOR_DEFEND    2 /**< Defend Mouse Cursor */
#define CURSOR_MAPTRAVEL 3 /**< Map Travel Mouse Cursor */
#define CURSOR_TALK      4 /**< Talk Mouse Cursor */
#define CURSOR_GET       5 /**< Get Mouse Cursor */
#define CURSOR_INVENTORY 6 /**< Inventory Mouse Cursor */
#define CURSOR_DOOR      7 /**< Door Mouse Cursor */
#define CURSOR_TOTAL     8 /**< Total number of mouse cursors */

/*! A mouse cursor class.*/
class cursor
{
   public:
      /*! cursor Constructor */
      cursor();
      /*! cursor Destructor */
      ~cursor();  

      /*! Set actual mouse Cursor 
       * \param nCursor -> cursor Number to use.*/
      void setActual(int nCursor); 

      /*! Draw th Cursor to screen */
      void draw(int mouseX, int mouseY);
      
   private:
      SDL_Surface* textura[CURSOR_TOTAL];  /**< internal GL texture of cursors */
      SDL_Surface* actualCursor;           /**< Actual Used Cursor */
      
      /*! Load Cursor file to textures 
       * \param fileName -> file name of cursor
       * \return \c GLuint with the texture ID. */
      //GLuint loadCursor(char* fileName); 
      SDL_Surface* loadCursor(char* fileName);

      
};

#endif

