#ifndef _cursor_h
#define _cursor_h

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define CURSOR_WALK      0
#define CURSOR_ATTACK    1
#define CURSOR_DEFEND    2
#define CURSOR_MAPTRAVEL 3
#define CURSOR_TALK      4
#define CURSOR_GET       5
#define CURSOR_TOTAL     6

class cursor
{
   public:
      cursor();
      ~cursor();
      int actualCursor;
      void   SetActual(int nCursor);
   private:
      GLuint textura[CURSOR_TOTAL];
      GLuint LoadCursor(char* fileName);
};

#endif

