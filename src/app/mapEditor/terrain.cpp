#include "terrain.h"

#define STATE_TERRAIN_OTHER            0
#define STATE_TERRAIN_NIVELATE_STARTED 1

/********************************************************************
 *                             Constructor                          *
 ********************************************************************/
terrain::terrain(Map* map)
{
   actualMap = map;
   state = STATE_TERRAIN_OTHER;
}

/********************************************************************
 *                             Destructor                           *
 ********************************************************************/
terrain::~terrain()
{
   actualMap = NULL;
}

/********************************************************************
 *                           verifyAction                           *
 ********************************************************************/
void terrain::verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                           Uint8 mButton, int tool, GLuint actualTexture)
{
   quadX = (int) (mouseX / SQUARESIZE);
   quadZ = (int) (mouseZ / SQUARESIZE);

   actualTool = tool;
   if(tool == TOOL_TERRAIN_TEXTURE)
   {
      doTexture(mouseX, mouseY, mouseZ, mButton, actualTexture);
   }
   else if(tool == TOOL_TERRAIN_NIVELATE)
   {
      doNivelate(mouseX, mouseY, mouseZ, mButton);
   }
   else
   {
      doUpDown(mouseX, mouseY, mouseZ, mButton);
   }
}

/********************************************************************
 *                              doUpDown                            *
 ********************************************************************/
void terrain::doUpDown(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                       Uint8 mButton)
{
}

/********************************************************************
 *                            doNivelate                            *
 ********************************************************************/
void terrain::doNivelate(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                         Uint8 mButton)
{
}

/********************************************************************
 *                             doTexture                            *
 ********************************************************************/
void terrain::doTexture(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        Uint8 mButton, GLuint actualTexture )
{
   if(mButton & SDL_BUTTON(1))
   {
      Square* quad = actualMap->quadradoRelativo(quadX, quadZ);
      if(quad)
      {
         quad->textura = actualTexture;
      }
   }
}

/********************************************************************
 *                             drawTemporary                        *
 ********************************************************************/
void terrain::drawTemporary()
{
   Square* quad = actualMap->quadradoRelativo(quadX, quadZ); 
   if( (actualTool == TOOL_TERRAIN_TEXTURE) && (quad))
   {
      glDisable(GL_LIGHTING);
      glColor3f(0.2,0.2,0.9);
      glBegin(GL_POLYGON);
        glVertex3f(quad->x1,quad->h1+1,quad->z1);
        glVertex3f(quad->x1,quad->h2+1,quad->z2);
        glVertex3f(quad->x2,quad->h3+1,quad->z2);
        glVertex3f(quad->x2,quad->h4+1,quad->z1);
        //glVertex3f();
      glEnd();
      glEnable(GL_LIGHTING);
         
   }
}
