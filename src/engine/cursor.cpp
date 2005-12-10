#include "cursor.h"

cursor::cursor()
{
   textura[CURSOR_WALK] = LoadCursor("../data/cursors/Walk.png");
   textura[CURSOR_ATTACK] = LoadCursor("../data/cursors/Attack.png");
   textura[CURSOR_DEFEND] = LoadCursor("../data/cursors/Defend.png");
   textura[CURSOR_MAPTRAVEL] = LoadCursor("../data/cursors/MapTravel.png");
   textura[CURSOR_TALK] = LoadCursor("../data/cursors/talk.png");
   textura[CURSOR_GET] = LoadCursor("../data/cursors/Get.png");
   actualCursor = CURSOR_WALK;
}

cursor::~cursor()
{
   int aux;
   for(aux = 0; aux < CURSOR_TOTAL;aux++)
   {
      glDeleteTextures(1,&(textura[aux]));
   }
}

GLuint cursor::LoadCursor(char* fileName)
{
   GLuint indice;
   SDL_Surface* img = IMG_Load(fileName);

   SDL_Surface* fig = SDL_DisplayFormatAlpha(img);
   
   glGenTextures(1, &(indice));
   glBindTexture(GL_TEXTURE_2D, indice);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,fig->w,fig->h, 
                0,GL_RGBA, GL_UNSIGNED_BYTE, fig->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   SDL_FreeSurface(img);
   SDL_FreeSurface(fig);
   return(indice);
}

void cursor::SetActual(int nCursor)
{
   actualCursor = textura[nCursor];
}

