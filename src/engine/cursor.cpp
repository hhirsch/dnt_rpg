#include "cursor.h"
#include "util.h"
#include "../etc/dirs.h"


/*****************************************************************
 *                         Constructor                           *
 *****************************************************************/
cursor::cursor()
{
   dirs dir;
   /* Generate OpenGL Textures */
   glGenTextures(CURSOR_TOTAL, texture);

   /* Load Cursors */
   loadCursor(dir.getRealFile("cursors/Walk.png"), CURSOR_WALK);
   loadCursor(dir.getRealFile("cursors/Attack.png"), CURSOR_ATTACK);
   loadCursor(dir.getRealFile("cursors/Defend.png"), CURSOR_DEFEND);
   loadCursor(dir.getRealFile("cursors/MapTravel.png"), CURSOR_MAPTRAVEL);
   loadCursor(dir.getRealFile("cursors/talk.png"), CURSOR_TALK);
   loadCursor(dir.getRealFile("cursors/Get.png"), CURSOR_GET);
   loadCursor(dir.getRealFile("cursors/Inventory.png"), CURSOR_INVENTORY);
   loadCursor(dir.getRealFile("cursors/Door.png"), CURSOR_DOOR);
   loadCursor(dir.getRealFile("cursors/forbidden.png"), CURSOR_FORBIDDEN);
   currentCursor = CURSOR_WALK;
}

/*****************************************************************
 *                          Destructor                           *
 *****************************************************************/
cursor::~cursor()
{
   /* Free Cursors Textures */
   glDeleteTextures(CURSOR_TOTAL,texture);
}

/*****************************************************************
 *                         Load Cursor                           *
 *****************************************************************/
void cursor::loadCursor(string fileName, int index)
{
   SDL_Surface* img = IMG_Load(fileName.c_str());
   if(img)
   {
      setTextureRGBA(img, texture[index]);
      sizeX[index] = img->w;
      sizeY[index] = img->h;
      SDL_FreeSurface(img);
   }
   else
   {
      printf(gettext("Can't load mouse cursor: %s\n"), fileName.c_str());
   }
}

/*****************************************************************
 *                               set                             *
 *****************************************************************/
void cursor::set(int nCursor)
{
   currentCursor = nCursor;
}

/*****************************************************************
 *                              set                              *
 *****************************************************************/
void cursor::set(SDL_Surface* img)
{
   /* Load the image to a texture */
   setTextureRGBA(img, texture[CURSOR_USER_IMAGE]);
   sizeX[CURSOR_USER_IMAGE] = img->w;
   sizeY[CURSOR_USER_IMAGE] = img->h;
   currentCursor = CURSOR_USER_IMAGE;
}

/*****************************************************************
 *                               get                             *
 *****************************************************************/
int cursor::get()
{
   return(currentCursor);
}


/*****************************************************************
 *                             Draw                              *
 *****************************************************************/
void cursor::draw(int mouseX, int mouseY)
{
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texture[currentCursor]);
   glPushMatrix();
      glTranslatef(mouseX, SCREEN_Y - mouseY, 0);
      glBegin(GL_QUADS);
         glTexCoord2f(0.0, 1.0);
         glVertex2f(0,0);
         glTexCoord2f(0.0, 0.0);
         glVertex2f(0, sizeY[currentCursor]);
         glTexCoord2f(1.0, 0.0);
         glVertex2f(sizeX[currentCursor], sizeY[currentCursor]);
         glTexCoord2f(1.0, 1.0);
         glVertex2f(sizeX[currentCursor], 0.0);
      glEnd();
   glPopMatrix();
}

