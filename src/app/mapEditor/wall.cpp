#include "wall.h"

#define WALL_STATE_OTHER 0
#define WALL_ADD_X_INIT  1
#define WALL_ADD_Z_INIT  2
#define WALL_ADD2_X_INIT 3
#define WALL_ADD2_Z_INIT 4

/******************************************************
 *                      Constructor                   *
 ******************************************************/
wall::wall(Map* map)
{
   actualMap = map;
   state = WALL_STATE_OTHER;
}

/******************************************************
 *                      Destructor                    *
 ******************************************************/
wall::~wall()
{
   actualMap = NULL;
}

/******************************************************
 *                      verifyAction                  *
 ******************************************************/
void wall::verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        Uint8 mButton, int tool, GLuint actualTexture)
{
   actualTool = tool;
   texture = actualTexture;
   mX = mouseX;
   mY = mouseY;
   mZ = mouseZ;
   mB = mButton;
}

/******************************************************
 *                      drawTemporary                 *
 ******************************************************/
void wall::drawTemporary()
{
}

