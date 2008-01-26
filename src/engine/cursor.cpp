#include "cursor.h"
#include "util.h"
#include "../etc/dirs.h"


/*****************************************************************
 *                         Constructor                           *
 *****************************************************************/
cursor::cursor()
{
   dirs dir;
   textura[CURSOR_WALK] = loadCursor(dir.getRealFile("cursors/Walk.png"));
   textura[CURSOR_ATTACK] = loadCursor(dir.getRealFile("cursors/Attack.png"));
   textura[CURSOR_DEFEND] = loadCursor(dir.getRealFile("cursors/Defend.png"));
   textura[CURSOR_MAPTRAVEL] = loadCursor(
                                      dir.getRealFile("cursors/MapTravel.png"));
   textura[CURSOR_TALK] = loadCursor(dir.getRealFile("cursors/talk.png"));
   textura[CURSOR_GET] = loadCursor(dir.getRealFile("cursors/Get.png"));
   textura[CURSOR_INVENTORY] = loadCursor(
                                      dir.getRealFile("cursors/Inventory.png"));
   textura[CURSOR_DOOR] = loadCursor(dir.getRealFile("cursors/Door.png"));
   textura[CURSOR_FORBIDDEN] = loadCursor(
                                      dir.getRealFile("cursors/forbidden.png"));
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
   glRasterPos2f(mouseX, SCREEN_Y - mouseY);
   glPixelZoom(1.0, -1.0);
   glDrawPixels(actualCursor->w, actualCursor->h, GL_RGBA, GL_UNSIGNED_BYTE, 
                actualCursor->pixels);
}

