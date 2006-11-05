
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <math.h>
#include "../map/map.h"
#include "personagens.h"

/*! The collision Detection Class */
class collision
{
   public:
      collision();
      ~collision();

      void defineMap(Map* map);
      bool verifySquare(GLfloat min[3], GLfloat max[3],Square* quad);
      bool verifyMeioFio(GLfloat min[3],GLfloat max[3], muro* meiosFio);
      bool canWalk(GLfloat perX, GLfloat perY, GLfloat perZ, 
                        GLfloat perX1, GLfloat perY1, GLfloat perZ1, 
                        GLfloat perX2, GLfloat perY2, GLfloat perZ2, 
                        GLfloat perOrientation, Square* perQuad,
                        Lpersonagem* NPCs, GLfloat& varHeight,
                        GLfloat& nx, GLfloat& nz);
      
   private:
      Map* actualMap;
};


