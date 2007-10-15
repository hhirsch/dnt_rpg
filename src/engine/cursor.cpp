#include "cursor.h"
#include "util.h"

/*****************************************************************
 *                         Constructor                           *
 *****************************************************************/
cursor::cursor()
{
   textura[CURSOR_WALK] = loadCursor("../data/cursors/Walk.png");
   textura[CURSOR_ATTACK] = loadCursor("../data/cursors/Attack.png");
   textura[CURSOR_DEFEND] = loadCursor("../data/cursors/Defend.png");
   textura[CURSOR_MAPTRAVEL] = loadCursor("../data/cursors/MapTravel.png");
   textura[CURSOR_TALK] = loadCursor("../data/cursors/talk.png");
   textura[CURSOR_GET] = loadCursor("../data/cursors/Get.png");
   textura[CURSOR_INVENTORY] = loadCursor("../data/cursors/Inventory.png");
   textura[CURSOR_DOOR] = loadCursor("../data/cursors/Door.png");
   textura[CURSOR_FORBIDDEN] = loadCursor("../data/cursors/forbidden.png");
   actualCursor = CURSOR_WALK;
}

/*****************************************************************
 *                          Destructor                           *
 *****************************************************************/
cursor::~cursor()
{
   int aux;
   for(aux = 0; aux < CURSOR_TOTAL;aux++)
   {
      ///glDeleteTextures(1,&(textura[aux]));
      SDL_FreeSurface(textura[aux]);
   }
}

/*****************************************************************
 *                         Load Cursor                           *
 *****************************************************************/
SDL_Surface* cursor::loadCursor(string fileName)
{
   SDL_Surface* img = IMG_Load(fileName.c_str());
   return(img);
}

/*****************************************************************
 *                          set Actual                           *
 *****************************************************************/
void cursor::setActual(int nCursor)
{
   actualCursor = textura[nCursor];
}

/*****************************************************************
 *                          set Actual                           *
 *****************************************************************/
void cursor::setActual(SDL_Surface* img)
{
   actualCursor = img;
}

/*****************************************************************
 *                          get Actual                           *
 *****************************************************************/
SDL_Surface* cursor::getActual()
{
   return(actualCursor);
}


/*****************************************************************
 *                             Draw                              *
 *****************************************************************/
void cursor::draw(int mouseX, int mouseY)
{
   glRasterPos2f(mouseX, 600 - mouseY);
   glPixelZoom(1.0, -1.0);
   glDrawPixels(actualCursor->w, actualCursor->h, GL_RGBA, GL_UNSIGNED_BYTE, 
                actualCursor->pixels);
}

