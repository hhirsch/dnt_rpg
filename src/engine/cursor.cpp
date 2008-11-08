#include "cursor.h"

#include "util.h"
#include "../etc/dirs.h"

#include <iostream>
using namespace std;

/*****************************************************************
 *                         Constructor                           *
 *****************************************************************/
cursor::cursor()
{
}

/*****************************************************************
 *                          Destructor                           *
 *****************************************************************/
cursor::~cursor()
{
}

/*****************************************************************
 *                             init                              *
 *****************************************************************/
void cursor::init()
{
   dirs dir;
   /* Generate OpenGL Textures */
   glGenTextures(CURSOR_TOTAL, texture);

   /* Load Cursors */
   loadCursor(dir.getRealFile("cursors/Walk.png"), CURSOR_WALK);
   hotSpot[CURSOR_WALK][0] = 0;
   hotSpot[CURSOR_WALK][1] = 0;
   loadCursor(dir.getRealFile("cursors/Attack.png"), CURSOR_ATTACK);
   hotSpot[CURSOR_ATTACK][0] = 1;
   hotSpot[CURSOR_ATTACK][1] = 0;
   loadCursor(dir.getRealFile("cursors/Defend.png"), CURSOR_DEFEND);
   hotSpot[CURSOR_DEFEND][0] = 0;
   hotSpot[CURSOR_DEFEND][1] = 0;
   loadCursor(dir.getRealFile("cursors/MapTravel.png"), CURSOR_MAPTRAVEL);
   hotSpot[CURSOR_MAPTRAVEL][0] = 15;
   hotSpot[CURSOR_MAPTRAVEL][1] = 15;
   loadCursor(dir.getRealFile("cursors/talk.png"), CURSOR_TALK);
   hotSpot[CURSOR_TALK][0] = 15;
   hotSpot[CURSOR_TALK][1] = 15;
   loadCursor(dir.getRealFile("cursors/Get.png"), CURSOR_GET);
   hotSpot[CURSOR_GET][0] = 5;
   hotSpot[CURSOR_GET][1] = 2;
   loadCursor(dir.getRealFile("cursors/Inventory.png"), CURSOR_INVENTORY);
   hotSpot[CURSOR_INVENTORY][0] = 15;
   hotSpot[CURSOR_INVENTORY][1] = 15;
   loadCursor(dir.getRealFile("cursors/Door.png"), CURSOR_DOOR);
   hotSpot[CURSOR_DOOR][0] = 5;
   hotSpot[CURSOR_DOOR][1] = 0;
   loadCursor(dir.getRealFile("cursors/forbidden.png"), CURSOR_FORBIDDEN);
   hotSpot[CURSOR_FORBIDDEN][0] = 15;
   hotSpot[CURSOR_FORBIDDEN][1] = 15;
   loadCursor(dir.getRealFile("cursors/walk_cont.png"), CURSOR_WALK_CONT);
   hotSpot[CURSOR_WALK_CONT][0] = 0;
   hotSpot[CURSOR_WALK_CONT][1] = 0;
   loadCursor(dir.getRealFile("cursors/use.png"), CURSOR_USE);
   hotSpot[CURSOR_USE][0] = 15;
   hotSpot[CURSOR_USE][1] = 15;
   
   currentCursor = CURSOR_WALK;
}

/*****************************************************************
 *                              end                              *
 *****************************************************************/
void cursor::finish()
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
      propX[index] = (float)(img->w) / (float)smallestPowerOfTwo(img->w);
      propY[index] = (float)(img->h) / (float)smallestPowerOfTwo(img->h);
      SDL_FreeSurface(img);
   }
   else
   {
      cerr << "Can't load mouse cursor: " << fileName << endl;
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
   propX[CURSOR_USER_IMAGE] = (float)(img->w) / 
                              (float)smallestPowerOfTwo(img->w);
   propY[CURSOR_USER_IMAGE] = (float)(img->h) / 
                              (float)smallestPowerOfTwo(img->h);
   hotSpot[CURSOR_USER_IMAGE][0] = 0;
   hotSpot[CURSOR_USER_IMAGE][1] = 0;
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
void cursor::draw(int mouseX, int mouseY, float angle)
{
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

   glDisable(GL_DEPTH_TEST);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texture[currentCursor]);
   glPushMatrix();
      glTranslatef(mouseX+2-hotSpot[currentCursor][0], 
                   SCREEN_Y - (mouseY+2-hotSpot[currentCursor][0]), 
                   0.1);
      glRotatef(angle, 0, 0, 1);
      glBegin(GL_QUADS);
         glTexCoord2f(0.0, 0.0);
         glVertex2f(0,0);
         glTexCoord2f(0.0, propY[currentCursor]);
         glVertex2f(0, -sizeY[currentCursor]);
         glTexCoord2f(propX[currentCursor], propY[currentCursor]);
         glVertex2f(sizeX[currentCursor], -sizeY[currentCursor]);
         glTexCoord2f(propX[currentCursor], 0.0);
         glVertex2f(sizeX[currentCursor], 0.0);
      glEnd();
   glPopMatrix();

   glEnable(GL_DEPTH_TEST);

   glDisable(GL_BLEND);
   glDisable(GL_TEXTURE_2D);
}

/*****************************************************************
 *                         Static Members                        *
 *****************************************************************/
GLuint cursor::texture[CURSOR_TOTAL]; /**< Cursors Textures */
float cursor::sizeX[CURSOR_TOTAL];    /**< Cursors Widths */
float cursor::sizeY[CURSOR_TOTAL];    /**< Cursors Heights */
float cursor::propX[CURSOR_TOTAL];    /**< X Proportion */
float cursor::propY[CURSOR_TOTAL];    /**< Y Proportion */
float cursor::hotSpot[CURSOR_TOTAL][2]; /**< HotSpot */
int cursor::currentCursor;

