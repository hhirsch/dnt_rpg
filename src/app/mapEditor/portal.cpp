#include "portal.h"

/******************************************************
 *                      Constructor                   *
 ******************************************************/
portal::portal(Map* map)
{
   actualMap = map;
}

/******************************************************
 *                       Destructor                   *
 ******************************************************/
portal::~portal()
{
   actualMap = NULL;
}

/******************************************************
 *                       VerifyAction                 *
 ******************************************************/
void portal::verifyAction(GLfloat mouseX, GLfloat mouseY, 
                          GLfloat mouseZ, Uint8 mButton, int tool)
{
}

/******************************************************
 *                       VerifyAction                 *
 ******************************************************/
void portal::drawTemporary()
{
}


