#include "shadow.h"
#include "util.h"

/*******************************************************************
 *                          Constructor                            *
 *******************************************************************/
shadow::shadow()
{
   //FIXME -> the shadow is too slow and not working at all
   avaible = false;
   enable = avaible;
}

/*******************************************************************
 *                           Destructor                            *
 *******************************************************************/
shadow::~shadow()
{
   if(avaible)
   {
      glDeleteTextures(1, &shadowMapTexture);
   }
}

/*******************************************************************
 *                              Init                               *
 *******************************************************************/
void shadow::init()
{
   if(avaible)
   {
      glGenTextures(1, &shadowMapTexture);
      glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                   SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT,
                   GL_UNSIGNED_BYTE, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   }
}

/*******************************************************************
 *                         defineLightView                         *
 *******************************************************************/
void shadow::defineLightView(GLfloat pX, GLfloat pY, GLfloat pZ)
{
   if(avaible)
   {
      /* Set the view to the light position */
      glLoadIdentity();
      gluPerspective(45.0f, 1.0f, 2.0f, OUTDOOR_FARVIEW);

      glLoadIdentity();
      gluLookAt(pX, pY, pZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

      glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
   }
}

/*******************************************************************
 *                        defineCameraView                         *
 *******************************************************************/
void shadow::defineCameraView(camera& cam, GLdouble proj[16],GLdouble modl[16])
{
   if(avaible)
   {
      glClear(GL_DEPTH_BUFFER_BIT);
      glMatrixMode(GL_PROJECTION);
      glLoadMatrixf((GLfloat*)proj);

      glMatrixMode(GL_MODELVIEW);
      glLoadMatrixf((GLfloat*)modl);

      glViewport(0, 0, SCREEN_X, SCREEN_Y);
   }
}

/*******************************************************************
 *                             isEnable                            *
 *******************************************************************/
bool shadow::isEnable()
{
   return(enable);
}

/*******************************************************************
 *                           setEnable                             *
 *******************************************************************/
void shadow::setEnable(bool en)
{
   enable = ( (en) && (avaible));
}

/*******************************************************************
 *                         saveShadorMap                           *
 *******************************************************************/
void shadow::saveShadowMap()
{
   glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
   glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 0, 0, 
                    SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0);
}

