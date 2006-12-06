#include "cursor.h"
#include "util.h"

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
   actualCursor = CURSOR_WALK;
}

cursor::~cursor()
{
   int aux;
   for(aux = 0; aux < CURSOR_TOTAL;aux++)
   {
      ///glDeleteTextures(1,&(textura[aux]));
      SDL_FreeSurface(textura[aux]);
   }
}

SDL_Surface* cursor::loadCursor(char* fileName)
{
   SDL_Surface* img = IMG_Load(fileName);

   /*glGenTextures(1, &(indice));
   glBindTexture(GL_TEXTURE_2D, indice);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   SDL_FreeSurface(img);
   return(indice);*/
   return(img);
}

void cursor::setActual(int nCursor)
{
   actualCursor = textura[nCursor];
}

void cursor::draw(int mouseX, int mouseY)
{
   glRasterPos2f(mouseX, 600 - mouseY);
   glPixelZoom(1.0, -1.0);
   glDrawPixels(actualCursor->w, actualCursor->h, GL_RGBA, GL_UNSIGNED_BYTE, 
                actualCursor->pixels);
}

