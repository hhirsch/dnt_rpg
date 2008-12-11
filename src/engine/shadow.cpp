#include "shadow.h"
#include "util.h"

#include "../etc/extensions.h"

/*******************************************************************
 *                          Constructor                            *
 *******************************************************************/
shadow::shadow()
{
   //FIXME -> the shadow is too slow and not working at all
   avaible = false;
   enable = false;//avaible;
}

/*******************************************************************
 *                           Destructor                            *
 *******************************************************************/
shadow::~shadow()
{
   extensions ext;
   if(avaible)
   {
      ext.extDeleteFramebuffers(1, &shadowFrameBuffer);
      glDeleteTextures(1, &shadowMapTexture);
   }
}

/*******************************************************************
 *                              Init                               *
 *******************************************************************/
void shadow::init()
{
   extensions ext;
   if(avaible)
   {
      /* Create the texture */
      //FIXME: NOT RGBA!!
      glGenTextures(1, &shadowMapTexture);
      glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
                   SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

      /* Create the Frame Buffer */
      ext.extGenFramebuffers(1, &shadowFrameBuffer);
      ext.extBindFramebuffer(GL_FRAMEBUFFER_EXT, shadowFrameBuffer); 
      ext.extFramebufferTexture2D(GL_FRAMEBUFFER_EXT, 
                                  GL_COLOR_ATTACHMENT0_EXT, 
                                  GL_TEXTURE_2D, shadowMapTexture, 0);

      GLenum status = ext.extCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);
      if(status != GL_FRAMEBUFFER_COMPLETE_EXT) 
      {
         cerr << "Error: Can't Init the FRAME_BUFFER GL extension!" << endl;
         enable = false; 
         if(status == GL_FRAMEBUFFER_UNSUPPORTED_EXT)
         {
            cerr << "Got Unsupported!" << endl;
         }
         cerr << "Status: " << status << endl;
      }
      ext.extBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
      enable = true;
   }
}

/*******************************************************************
 *                         defineLightView                         *
 *******************************************************************/
void shadow::defineLightView(GLfloat pX, GLfloat pY, GLfloat pZ,
                             GLfloat mapX, GLfloat mapZ)
{
   if(avaible)
   {
      /* Normalize the vector */
      //normalize(pX, pY, pZ);

      /* and set it near "infinity" */
      //pX *= (OUTDOOR_FARVIEW-20);
      //pY *= (OUTDOOR_FARVIEW-20);
      //pZ *= (OUTDOOR_FARVIEW-20);

      /* Set the view to the light position */
      glLoadIdentity();
      gluPerspective(45.0f, 1024.0 / 768.0, 1.0f, OUTDOOR_FARVIEW);

      glLoadIdentity();
      gluLookAt(pX, pY, pZ, mapX, 0.0f, mapZ, 
                0.0f, 1.0f, 0.0f);

      //glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
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

