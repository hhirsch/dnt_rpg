/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "shadow.h"
#include "util.h"

#include "../etc/extensions.h"
using namespace std;

/*******************************************************************
 *                          Constructor                            *
 *******************************************************************/
shadow::shadow()
{
   //FIXME: Shadow maps probaly won't goot for
   //outdoor: the light is too much far away!
   avaible = false;
   enable = false;//avaible;
}

/*******************************************************************
 *                           Destructor                            *
 *******************************************************************/
shadow::~shadow()
{
   extensions ext;
   if(enable)
   {
      ext.extDeleteFramebuffers(1, &shadowFrameBuffer);
      //ext.extDeleteRenderbuffers(1, &depthBuffer);
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
      glGenTextures(1, &shadowMapTexture);
      glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24_ARB, 
                   SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT,
                   GL_UNSIGNED_BYTE, NULL);
      /*glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);*/
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

      /* Create the Depth Buffer */
      //ext.extGenRenderbuffers(1, &depthBuffer);
      //ext.extBindRenderbuffer(GL_RENDERBUFFER_EXT, depthBuffer);
      //ext.extRenderbufferStorage(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
      //                              SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
      //ext.extBindRenderbuffer(GL_RENDERBUFFER_EXT, 0);


      /* Create the Frame Buffer */
      ext.extGenFramebuffers(1, &shadowFrameBuffer);
      ext.extBindFramebuffer(GL_FRAMEBUFFER_EXT, shadowFrameBuffer);

      /* Attach Texture to Frame Buffer */
      ext.extFramebufferTexture2D(GL_FRAMEBUFFER_EXT, 
                                  GL_DEPTH_ATTACHMENT_EXT, 
                                  GL_TEXTURE_2D, shadowMapTexture, 0);

      /* Disable DrawBuffer and ReadBuffer (depth only) */
      glDrawBuffer(GL_NONE);
      glReadBuffer(GL_NONE);

      /* Attach Depth Buffer to the Frame Buffer */
      //ext.extFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT,
      //                               GL_DEPTH_ATTACHMENT_EXT,
      //                               GL_RENDERBUFFER_EXT, depthBuffer);

      /* Check Status */
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
    
      /* Create and load the shader */
      if(shadowMapShader.load("shaders/shadowmap.vert",
               "shaders/shadowmap.frag"))
      {
         /* Set as enabled */
         enable = true;
      }
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
      gluPerspective(45.0f, 1.0, 1.0f, OUTDOOR_FARVIEW);

      glLoadIdentity();
      gluLookAt(pX, pY, pZ, mapX, 0.0f, mapZ, 
                0.0f, 1.0f, 0.0f);

      glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
   }
}

/*******************************************************************
 *                        beginLightRender                         *
 *******************************************************************/
void shadow::beginLightRender()
{
   extensions ext;
   if(enable)
   {
      ext.extBindFramebuffer(GL_FRAMEBUFFER_EXT, shadowFrameBuffer);
      glEnable( GL_DEPTH_TEST);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   }
}

/*******************************************************************
 *                         endLightRender                          *
 *******************************************************************/
void shadow::endLightRender()
{
   extensions ext;
   if(enable)
   {
      ext.extBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
   }
}

/*******************************************************************
 *                       beginShadowRender                         *
 *******************************************************************/
void shadow::beginShadowRender()
{
   if(enable)
   {
      shadowMapShader.enable();
      shadowMapShader.setUniformVariable("ShadowMap", (GLint)shadowMapTexture);
   }
}

/*******************************************************************
 *                        endShadowRender                          *
 *******************************************************************/
void shadow::endShadowRender()
{
   if(enable)
   {
      shadowMapShader.disable();
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

